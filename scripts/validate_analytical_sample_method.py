#!/usr/bin/env python3
"""Fail-closed semantic controls for DEC-STUDIO-NAME-001 analytical sampling."""

from __future__ import annotations

import csv
import hashlib
import math
import sys
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path
from typing import Iterable, Mapping, Sequence


ROOT = Path(__file__).resolve().parents[1]
METHOD_VERSION = "ASA-METHOD-v1.0.0"
SELECTION_VERSION = "ASA-SELECTION-v1.0.0"
STABILITY_VERSION = "ASA-STABILITY-v1.0.0"
LINEAGE_VERSION = "ASA-LINEAGE-v1.0.0"

RESULTS_PATH = "docs/research/market-verification/DEC-STUDIO-NAME-001-verification-consolidated-results.csv"
SOURCE_REVIEW_PATH = "docs/research/market-verification/DEC-STUDIO-NAME-001-verification-consolidated-source-review.csv"
REGISTER_PATH = "docs/research/DEC-STUDIO-NAME-001-market-sample-register.csv"
QUEUE_PATH = "docs/research/market-verification/DEC-STUDIO-NAME-001-verification-recovery-queue.csv"
SELECTION_SPEC = "docs/governance/DEC-STUDIO-NAME-001-analytical-sample-selection-spec.md"
STABILITY_SPEC = "docs/governance/DEC-STUDIO-NAME-001-pattern-stability-codebook.md"
LINEAGE_SPEC = "docs/governance/DEC-STUDIO-NAME-001-analytical-lineage-freshness-spec.md"
SUPERSESSION_MAP = "docs/governance/DEC-STUDIO-NAME-001-market-method-supersession-map.md"
FOUNDER_GATE = "docs/governance/DEC-STUDIO-NAME-001-founder-acceptance-gate.md"
AMENDMENT = "docs/decisions/inbox/DEC-STUDIO-NAME-001-market-method-amendment.md"

AUTHORITATIVE_PATHS = (RESULTS_PATH, SOURCE_REVIEW_PATH, REGISTER_PATH, QUEUE_PATH)

STRATUM_COUNTS = {
    "Büyük uluslararası yayıncılar": 30,
    "Bağımsız stüdyolar": 45,
    "Mobil oyun şirketleri": 25,
    "PC/konsol stüdyoları": 25,
    "Strateji/MMO üreticileri": 25,
    "Teknoloji/yaratıcı üretim markaları": 30,
}
REGIONS = {
    "Kuzey Amerika",
    "Avrupa",
    "Türkiye ve yakın bölge",
    "Doğu Asya",
    "Güney ve Güneydoğu Asya",
    "Diğer bölgeler",
}
RECOVERY_PRIORITY = {"P1": 0, "P2": 1, "P3": 2}
PRIMARY_METRICS = (
    "single_word", "two_word", "compound", "coined", "descriptive",
    "founder_surname", "geographic", "acronym", "suffix_games",
    "suffix_studios", "suffix_interactive", "suffix_entertainment",
)


class MethodValidationError(ValueError):
    """Raised when a governance control fails closed."""


def read_csv(relative_path: str, root: Path = ROOT) -> list[dict[str, str]]:
    with (root / relative_path).open(encoding="utf-8-sig", newline="") as handle:
        return list(csv.DictReader(handle))


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for block in iter(lambda: handle.read(65536), b""):
            digest.update(block)
    return digest.hexdigest()


def snapshot_hash(path_hashes: Mapping[str, str]) -> str:
    payload = "\n".join(f"{path}={path_hashes[path]}" for path in sorted(path_hashes))
    return hashlib.sha256(payload.encode("utf-8")).hexdigest()


def tie_break_hash(frozen_snapshot_hash: str, counted_sample_id: str) -> str:
    return hashlib.sha256(f"{frozen_snapshot_hash}|{counted_sample_id}".encode("utf-8")).hexdigest()


def _index_unique(rows: Iterable[Mapping[str, str]], key: str, label: str) -> dict[str, Mapping[str, str]]:
    index: dict[str, Mapping[str, str]] = {}
    for row in rows:
        value = row.get(key, "").strip()
        if not value:
            raise MethodValidationError(f"{label}: missing {key}")
        if value in index:
            raise MethodValidationError(f"{label}: duplicate {key} {value}")
        index[value] = row
    return index


def authoritative_join(
    population: Sequence[Mapping[str, str]],
    register: Sequence[Mapping[str, str]],
) -> list[dict[str, str]]:
    """Join population to dimensions with exact 1:1 and integrity checks."""
    pop_index = _index_unique(population, "Counted Sample ID", "population")
    reg_index = _index_unique(register, "Counted Sample ID", "register")
    joined: list[dict[str, str]] = []
    for counted_id, row in pop_index.items():
        dimension = reg_index.get(counted_id)
        if dimension is None:
            raise MethodValidationError(f"missing register join {counted_id}")
        canonical = row.get("Canonical Entity ID", "").strip()
        reg_canonical = dimension.get("Canonical Entity ID", "").strip()
        if not canonical or canonical != reg_canonical:
            raise MethodValidationError(f"canonical mismatch {counted_id}")
        for field in ("Primary Layer", "Region"):
            value = dimension.get(field, "").strip()
            if not value:
                raise MethodValidationError(f"missing dimension {field} for {counted_id}")
            population_value = row.get(field, "").strip()
            if population_value and population_value != value:
                raise MethodValidationError(f"conflicting {field} for {counted_id}")
        record_type = row.get("Record Type", "").strip()
        if record_type not in {"ACTIVE", "HISTORICAL"}:
            raise MethodValidationError(f"incompatible Record Type for {counted_id}")
        combined = dict(row)
        combined.update({
            "Primary Layer": dimension["Primary Layer"].strip(),
            "Region": dimension["Region"].strip(),
            "Scale": dimension.get("Scale Category", "").strip() or dimension.get("Scale", "").strip(),
            "Scale Review Status": dimension.get("Scale Review Status", "").strip(),
        })
        joined.append(combined)
    return joined


def stratum_target_band(n: int, original_count: int) -> tuple[float, int, int]:
    if n <= 0 or original_count <= 0:
        raise MethodValidationError("N and original count must be positive")
    expected = n * original_count / 180
    lower = max(6, math.floor(0.80 * expected))
    upper = min(math.floor(0.30 * n), math.ceil(1.20 * expected))
    return expected, lower, upper


def validate_stratum_distribution(counts: Mapping[str, int], n: int) -> None:
    if sum(counts.get(name, 0) for name in STRATUM_COUNTS) != n:
        raise MethodValidationError("stratum counts do not sum to N")
    for name, original in STRATUM_COUNTS.items():
        _, lower, upper = stratum_target_band(n, original)
        if lower > upper:
            raise MethodValidationError(f"infeasible stratum band at N={n}")
        observed = counts.get(name, 0)
        if observed < lower or observed > upper:
            raise MethodValidationError(f"stratum target-band failure: {name}={observed}, band={lower}..{upper}")


def validate_region_caps(region_counts: Mapping[str, int], n: int) -> None:
    if n <= 0 or sum(region_counts.values()) != n:
        raise MethodValidationError("region counts do not sum to N")
    if any(region_counts.get(region, 0) < 3 for region in REGIONS):
        raise MethodValidationError("region presence floor failure")
    shares = sorted((count / n for count in region_counts.values()), reverse=True)
    if shares and shares[0] > 0.35 + 1e-12:
        raise MethodValidationError("single region cap failure")
    if sum(shares[:2]) > 0.60 + 1e-12:
        raise MethodValidationError("top-two region cap failure")


def cross_cell_warnings(rows: Sequence[Mapping[str, str]]) -> list[tuple[str, str, int]]:
    n = len(rows)
    if not n:
        return []
    counts = Counter((row["Primary Layer"], row["Region"]) for row in rows)
    return [(layer, region, count) for (layer, region), count in sorted(counts.items()) if count / n > 0.15]


def deterministic_candidate_order(
    candidates: Sequence[Mapping[str, str]],
    current_rows: Sequence[Mapping[str, str]],
    frozen_snapshot_hash: str,
) -> list[str]:
    """Return stable candidate IDs using the preregistered lexicographic tuple."""
    _index_unique(candidates, "Counted Sample ID", "candidates")
    n_next = len(current_rows) + 1
    stratum_counts = Counter(row["Primary Layer"] for row in current_rows)
    region_counts = Counter(row["Region"] for row in current_rows)
    cross_counts = Counter((row["Primary Layer"], row["Region"]) for row in current_rows)

    def key(row: Mapping[str, str]) -> tuple[object, ...]:
        layer = row["Primary Layer"]
        region = row["Region"]
        if layer not in STRATUM_COUNTS or region not in REGIONS:
            raise MethodValidationError(f"unknown dimension for {row['Counted Sample ID']}")
        expected, lower, _ = stratum_target_band(n_next, STRATUM_COUNTS[layer])
        hard_deficit = 0 if stratum_counts[layer] < lower else 1
        region_deficit = 0 if region_counts[region] < 3 else 1
        expected_deficit = expected - stratum_counts[layer]
        priority = RECOVERY_PRIORITY.get(row.get("Recovery Priority", ""), 99)
        return (
            hard_deficit,
            region_deficit,
            -expected_deficit,
            cross_counts[(layer, region)],
            priority,
            tie_break_hash(frozen_snapshot_hash, row["Counted Sample ID"]),
        )

    return [row["Counted Sample ID"] for row in sorted(candidates, key=key)]


def parse_timestamp(value: str) -> datetime:
    cleaned = value.strip().replace("Z", "+00:00")
    if not cleaned:
        raise MethodValidationError("missing timestamp")
    if len(cleaned) == 10:
        cleaned += "T00:00:00+00:00"
    parsed = datetime.fromisoformat(cleaned)
    if parsed.tzinfo is None:
        raise MethodValidationError("timezone required")
    return parsed.astimezone(timezone.utc)


def require_fresh(verification_timestamp: str, freeze_timestamp: str, max_days: int = 7) -> None:
    verified = parse_timestamp(verification_timestamp)
    frozen = parse_timestamp(freeze_timestamp)
    age_seconds = (frozen - verified).total_seconds()
    if age_seconds < 0 or age_seconds > max_days * 86400:
        raise MethodValidationError("STALE_RECHECK_REQUIRED")


def validate_historical_all_eligible(
    eligible_ids: Iterable[str], included_ids: Iterable[str], strata: Mapping[str, int]
) -> None:
    eligible = set(eligible_ids)
    included_list = list(included_ids)
    included = set(included_list)
    if len(included_list) != len(included):
        raise MethodValidationError("duplicate historical inclusion")
    if included != eligible:
        raise MethodValidationError("historical all-eligible rule failure")
    if len(included) < 12 or len(strata) != 2 or any(value < 5 for value in strata.values()):
        raise MethodValidationError("historical minimum gate failure")


def stability_pass(
    checkpoint_rates: Mapping[int, Mapping[str, float]],
    checkpoint_missing_rates: Mapping[int, float],
    stop_n: int,
) -> bool:
    if stop_n not in {60, 70, 80, 90}:
        raise MethodValidationError("invalid checkpoint")
    checkpoints = (stop_n - 20, stop_n - 10, stop_n)
    for checkpoint in checkpoints:
        if checkpoint not in checkpoint_rates or checkpoint not in checkpoint_missing_rates:
            raise MethodValidationError("missing checkpoint")
        if checkpoint_missing_rates[checkpoint] > 5.0:
            return False
        missing_metrics = set(PRIMARY_METRICS) - set(checkpoint_rates[checkpoint])
        if missing_metrics:
            raise MethodValidationError("missing preregistered metric")
    for previous, current in zip(checkpoints, checkpoints[1:]):
        if max(abs(checkpoint_rates[current][m] - checkpoint_rates[previous][m]) for m in PRIMARY_METRICS) > 7.5:
            return False
    return True


def recovery_gate(founder_decision: str, reaudit_present: bool) -> str:
    if founder_decision == "ACCEPTED" and reaudit_present:
        return "OPEN"
    return "CLOSED_PENDING_INDEPENDENT_REAUDIT_AND_FOUNDER_ACCEPTANCE"


def _require_text(path: str, snippets: Sequence[str], root: Path = ROOT) -> None:
    text = (root / path).read_text(encoding="utf-8")
    for snippet in snippets:
        if snippet not in text:
            raise MethodValidationError(f"{path}: missing semantic control {snippet!r}")


def validate_repository(root: Path = ROOT) -> str:
    for relative in AUTHORITATIVE_PATHS + (SELECTION_SPEC, STABILITY_SPEC, LINEAGE_SPEC, SUPERSESSION_MAP, FOUNDER_GATE):
        if not (root / relative).is_file():
            raise MethodValidationError(f"missing authoritative/spec path {relative}")

    results = read_csv(RESULTS_PATH, root)
    register = read_csv(REGISTER_PATH, root)
    queue = read_csv(QUEUE_PATH, root)
    active_queue = [row for row in queue if row.get("Record Type") == "ACTIVE" and row.get("Current Verification Result") == "STATUS_UNCERTAIN"]
    authoritative_join(active_queue, register)
    authoritative_join(results, register)
    _index_unique(results, "Counted Sample ID", "results")

    source_rows = read_csv(SOURCE_REVIEW_PATH, root)
    if not source_rows or any(not row.get("Sample ID", "").strip() for row in source_rows):
        raise MethodValidationError("source review missing Sample ID")

    _require_text(SELECTION_SPEC, (SELECTION_VERSION, "SHA256", "floor(0.80", "ceil(1.20", "%35", "%60", "CROSS_CELL_CONCENTRATION_WARNING"), root)
    _require_text(STABILITY_SPEC, (STABILITY_VERSION, "40->50", "50->60", "7.5", "N=90", "MISSING_OR_AMBIGUOUS"), root)
    _require_text(LINEAGE_SPEC, (LINEAGE_VERSION, "Counted Sample ID", "Canonical Entity ID", "1:1", ">7 days", "STALE_RECHECK_REQUIRED", "bütün `VERIFIED_HISTORICAL_CASE`"), root)
    _require_text(FOUNDER_GATE, ("Founder Decision", "ACCEPTED", "PENDING", "CLOSED_PENDING_INDEPENDENT_REAUDIT_AND_FOUNDER_ACCEPTANCE"), root)
    _require_text(SUPERSESSION_MAP, ("Primary-source requirement", "Legal stop gate", "Security stop gate", "Privacy stop gate", "IP stop gate", "Founder final authority", "agent self-approval"), root)
    _require_text(AMENDMENT, ("Context Inventory", "Analytical Sample", "RECOVERY GATE CLOSED"), root)

    path_hashes = {path: sha256_file(root / path) for path in AUTHORITATIVE_PATHS}
    if len(snapshot_hash(path_hashes)) != 64:
        raise MethodValidationError("snapshot hash failure")

    founder_text = (root / FOUNDER_GATE).read_text(encoding="utf-8")
    decision = "PENDING" if "Founder Decision: `PENDING`" in founder_text else ""
    return recovery_gate(decision, reaudit_present=False)


def main() -> int:
    try:
        gate = validate_repository()
    except (OSError, MethodValidationError) as exc:
        print(f"METHOD_INVALID: {exc}", file=sys.stderr)
        return 1
    print("METHOD_VALID")
    print(f"METHOD_VERSION={METHOD_VERSION}")
    print(f"RECOVERY_GATE_{gate}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
