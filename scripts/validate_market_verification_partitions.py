#!/usr/bin/env python3
"""Validate deterministic DEC-STUDIO-NAME-001 verification partitions."""
from __future__ import annotations

import csv
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SAMPLE_REGISTER = ROOT / "docs/research/DEC-STUDIO-NAME-001-market-sample-register.csv"
PARTITION_DIR = ROOT / "docs/research/market-verification"
PREFIX = "DEC-STUDIO-NAME-001-MV-"

ACTIVE_BATCHES = {letter: 30 for letter in "ABCDEF"}
HISTORICAL_BATCHES = {"G": 10, "H": 10}
BATCH_SIZES = {**ACTIVE_BATCHES, **HISTORICAL_BATCHES}
HISTORICAL_STATUS = {
    "G": "HISTORICAL_REBRAND_CASE",
    "H": "HISTORICAL_NAMING_PROBLEM_CASE",
}

ACTIVE_FIELDS = [
    "Verification Batch ID",
    "Counted Sample ID",
    "Canonical Entity ID",
    "Normalized Entity Name",
    "Current Status",
    "Primary Counted Layer",
    "Region",
    "Existing Source IDs",
    "Required Independent Source Count",
    "Required Primary Source Count",
    "Assigned Reviewer Role",
    "Verification Status",
    "Notes",
]
HISTORICAL_FIELDS = ACTIVE_FIELDS + [
    "Historical Case Type",
    "Former Name",
    "Current Name or Resolution",
    "Event Date",
    "Reason Classification",
    "Existing Case Source IDs",
]


def read_csv(path: Path) -> tuple[list[str], list[dict[str, str]]]:
    with path.open(encoding="utf-8-sig", newline="") as handle:
        reader = csv.DictReader(handle)
        return list(reader.fieldnames or []), list(reader)


def expected_batches(samples: list[dict[str, str]]) -> dict[str, list[dict[str, str]]]:
    active = sorted(
        (row for row in samples if row.get("Status") == "STATUS_UNCERTAIN"),
        key=lambda row: row["Counted Sample ID"],
    )
    batches = {
        letter: active[index * 30 : (index + 1) * 30]
        for index, letter in enumerate("ABCDEF")
    }
    for letter, status in HISTORICAL_STATUS.items():
        batches[letter] = sorted(
            (row for row in samples if row.get("Status") == status),
            key=lambda row: row["Counted Sample ID"],
        )
    return batches


def validate(
    sample_path: Path | str = SAMPLE_REGISTER,
    partition_dir: Path | str = PARTITION_DIR,
) -> list[str]:
    errors: list[str] = []
    sample_path = Path(sample_path)
    partition_dir = Path(partition_dir)

    try:
        _, samples = read_csv(sample_path)
    except (OSError, csv.Error) as exc:
        return [f"cannot read sample register: {exc}"]

    expected = expected_batches(samples)
    sample_by_counted = {row.get("Counted Sample ID", ""): row for row in samples}
    all_rows: list[dict[str, str]] = []

    for letter, expected_size in BATCH_SIZES.items():
        path = partition_dir / f"{PREFIX}{letter}.csv"
        try:
            fields, rows = read_csv(path)
        except (OSError, csv.Error) as exc:
            errors.append(f"MV-{letter}: cannot read partition: {exc}")
            continue

        required_fields = ACTIVE_FIELDS if letter in ACTIVE_BATCHES else HISTORICAL_FIELDS
        if fields != required_fields:
            errors.append(f"MV-{letter}: unexpected CSV header")
        if len(rows) != expected_size:
            errors.append(f"MV-{letter}: expected {expected_size} rows, got {len(rows)}")

        expected_ids = [row["Counted Sample ID"] for row in expected[letter]]
        actual_ids = [row.get("Counted Sample ID", "") for row in rows]
        if actual_ids != expected_ids:
            errors.append(f"MV-{letter}: membership/order is not the deterministic partition")

        for row in rows:
            counted_id = row.get("Counted Sample ID", "")
            source = sample_by_counted.get(counted_id)
            if row.get("Verification Batch ID") != f"MV-{letter}":
                errors.append(f"MV-{letter}: wrong Verification Batch ID for {counted_id}")
            if row.get("Verification Status") != "NOT_STARTED":
                errors.append(f"MV-{letter}: Verification Status must be NOT_STARTED for {counted_id}")
            if source is None:
                continue
            mappings = {
                "Canonical Entity ID": "Canonical Entity ID",
                "Normalized Entity Name": "Normalized Entity Name",
                "Current Status": "Status",
                "Primary Counted Layer": "Primary Counted Layer",
                "Region": "Region",
                "Existing Source IDs": "Source IDs",
            }
            for partition_field, source_field in mappings.items():
                if row.get(partition_field) != source.get(source_field):
                    errors.append(f"MV-{letter}: {partition_field} mismatch for {counted_id}")
            if row.get("Required Independent Source Count") != "2":
                errors.append(f"MV-{letter}: independent source requirement must be 2 for {counted_id}")
            if row.get("Required Primary Source Count") != "1":
                errors.append(f"MV-{letter}: primary source requirement must be 1 for {counted_id}")
            if not row.get("Assigned Reviewer Role"):
                errors.append(f"MV-{letter}: reviewer role is blank for {counted_id}")

            if letter in ACTIVE_BATCHES:
                if source.get("Status") != "STATUS_UNCERTAIN":
                    errors.append(f"MV-{letter}: historical record in active partition: {counted_id}")
            else:
                if source.get("Status") != HISTORICAL_STATUS[letter]:
                    errors.append(f"MV-{letter}: wrong historical case type for {counted_id}")
                historical_mappings = {
                    "Historical Case Type": "Historical Case Classification",
                    "Former Name": "Historical Old Name",
                    "Current Name or Resolution": "Historical New Name or Resolution",
                    "Event Date": "Historical Event Date",
                    "Reason Classification": "Historical Verified Reason",
                    "Existing Case Source IDs": "Source IDs",
                }
                for partition_field, source_field in historical_mappings.items():
                    if row.get(partition_field) != source.get(source_field):
                        errors.append(f"MV-{letter}: {partition_field} mismatch for {counted_id}")
        all_rows.extend(rows)

    counted_ids = [row.get("Counted Sample ID", "") for row in all_rows]
    canonical_ids = [row.get("Canonical Entity ID", "") for row in all_rows]
    sample_ids = [row.get("Counted Sample ID", "") for row in samples]
    if len(all_rows) != 200:
        errors.append(f"expected 200 partition rows, got {len(all_rows)}")
    if len(counted_ids) != len(set(counted_ids)):
        errors.append("duplicate Counted Sample ID")
    if len(canonical_ids) != len(set(canonical_ids)):
        errors.append("duplicate Canonical Entity ID")
    if set(counted_ids) != set(sample_ids):
        missing = sorted(set(sample_ids) - set(counted_ids))
        extra = sorted(set(counted_ids) - set(sample_ids))
        if missing:
            errors.append(f"missing Counted Sample IDs: {','.join(missing)}")
        if extra:
            errors.append(f"extra Counted Sample IDs: {','.join(extra)}")

    batch_counts = Counter(row.get("Verification Batch ID") for row in all_rows)
    if sum(batch_counts[f"MV-{letter}"] for letter in ACTIVE_BATCHES) != 180:
        errors.append("MV-A through MV-F must total 180 rows")
    return errors


def main() -> int:
    errors = validate()
    if errors:
        print("MARKET VERIFICATION PARTITION VALIDATION FAILED")
        for error in errors:
            print(f"- {error}")
        return 1
    print("MARKET VERIFICATION PARTITION VALIDATION PASSED")
    print("MV-A=30 MV-B=30 MV-C=30 MV-D=30 MV-E=30 MV-F=30 MV-G=10 MV-H=10")
    print("TOTAL=200 UNIQUE_COUNTED=200 UNIQUE_CANONICAL=200")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
