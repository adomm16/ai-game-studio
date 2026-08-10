#!/usr/bin/env python3
"""Fail-closed checks for DEC-STUDIO-NAME-001 verification consolidation."""
from __future__ import annotations

import csv
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MV = ROOT / "docs/research/market-verification"
RESULTS = MV / "DEC-STUDIO-NAME-001-verification-consolidated-results.csv"
SOURCES = MV / "DEC-STUDIO-NAME-001-verification-consolidated-source-review.csv"
QUEUE = MV / "DEC-STUDIO-NAME-001-verification-recovery-queue.csv"
PARTITION = MV

RESULT_STATUSES = {"VERIFIED_ACTIVE", "VERIFIED_HISTORICAL_CASE", "STATUS_UNCERTAIN"}
SOURCE_STATUSES = {"REVIEWED", "NEEDS_RECHECK", "REJECTED"}
GOOD_ACCESS = {"VERIFIED_ACCESSIBLE", "VERIFIED_WITH_REDIRECT"}


def rows(path: Path):
    with path.open(encoding="utf-8-sig", newline="") as handle:
        return list(csv.DictReader(handle))


def validate(results_path=RESULTS, sources_path=SOURCES, queue_path=QUEUE, partition_dir=PARTITION):
    errors: list[str] = []

    def require(condition, message):
        if not condition:
            errors.append(message)

    results = rows(Path(results_path))
    sources = rows(Path(sources_path))
    queue = rows(Path(queue_path))
    batches = {f"MV-{x}" for x in "ABCDEFGH"}
    counted = [r.get("Counted Sample ID", "") for r in results]
    canonical = [r.get("Canonical Entity ID", "") for r in results]

    require(len(results) == 200, f"expected 200 total results, got {len(results)}")
    require(len(set(counted)) == 200 and all(counted), "expected 200 unique Counted Sample IDs")
    require(len(set(canonical)) == 200 and all(canonical), "expected 200 unique Canonical Entity IDs")
    require({r.get("Verification Batch ID") for r in results} == batches, "incomplete batch coverage")
    require(all(r.get("Verification Result") in RESULT_STATUSES for r in results), "invalid result status")

    active = [r for r in results if r.get("Record Type") == "ACTIVE"]
    historical = [r for r in results if r.get("Record Type") == "HISTORICAL"]
    require(len(active) == 180, f"expected 180 active records, got {len(active)}")
    require(len(historical) == 20, f"expected 20 historical records, got {len(historical)}")
    require(sum(r.get("Verification Batch ID") in {f"MV-{x}" for x in "ABCDEF"} for r in results) == 180,
            "MV-A through MV-F must total 180")
    require(sum(r.get("Verification Batch ID") in {"MV-G", "MV-H"} for r in results) == 20,
            "MV-G through MV-H must total 20")
    require(all(r["Verification Result"] in {"VERIFIED_ACTIVE", "STATUS_UNCERTAIN"} for r in active),
            "active cohort has invalid result")
    require(all(r["Verification Result"] in {"VERIFIED_HISTORICAL_CASE", "STATUS_UNCERTAIN"} for r in historical),
            "historical cohort has invalid result")

    source_ids = [r.get("Source ID", "") for r in sources]
    require(len(sources) == 432, f"expected 432 source-review rows, got {len(sources)}")
    require(len(source_ids) == len(set(source_ids)) and all(source_ids), "duplicate or blank Source ID")
    require(all(r.get("Final Review Status") in SOURCE_STATUSES for r in sources), "invalid source status")
    by_source = {r["Source ID"]: r for r in sources}
    by_sample = defaultdict(list)
    for row in sources:
        by_sample[row.get("Sample ID")].append(row)

    for row in results:
        verified = row["Verification Result"].startswith("VERIFIED")
        for key in ("Qualifying Source 1 ID", "Qualifying Source 2 ID"):
            sid = row.get(key)
            if verified and sid and sid != "NOT_APPLICABLE":
                require(sid in by_source, f"missing qualifying source {sid}")
                if sid in by_source:
                    source = by_source[sid]
                    require(source["Sample ID"] == row["Counted Sample ID"], f"qualifying source allocation mismatch {sid}")
                    require(source["Final Review Status"] == "REVIEWED", f"unreviewed qualifying source {sid}")
                    require(source["Evidence Qualification"] == "YES", f"non-qualifying source referenced {sid}")
                    require(source["Accessibility Result"] in GOOD_ACCESS, f"bad-access qualifying source {sid}")
                    require(source["Identity Result"] == "PASS" and source["Claim Support Result"] == "PASS",
                            f"identity/claim failure in qualifying source {sid}")
        if verified:
            q = [by_source[s] for s in (row["Qualifying Source 1 ID"], row["Qualifying Source 2 ID"]) if s in by_source]
            require(len(q) >= 2, f"verified result lacks two qualifying sources {row['Counted Sample ID']}")
            require(len({s["Publisher/Institution"] for s in q}) >= 2, f"publisher independence failed {row['Counted Sample ID']}")
            require(len({s["Source Origin Group"] for s in q}) >= 2, f"origin independence failed {row['Counted Sample ID']}")
            require(any(any(token in s["Primary/Official Classification"].upper() for token in ("PRIMARY", "OFFICIAL", "YES")) for s in q),
                    f"primary/official source missing {row['Counted Sample ID']}")

    uncertain = {r["Counted Sample ID"] for r in results if r["Verification Result"] == "STATUS_UNCERTAIN"}
    queue_ids = [r.get("Counted Sample ID", "") for r in queue]
    require(len(queue_ids) == len(set(queue_ids)), "duplicate recovery queue record")
    require(set(queue_ids) == uncertain, "STATUS_UNCERTAIN count/membership does not match recovery queue")
    require(all(r.get("Current Verification Result") == "STATUS_UNCERTAIN" for r in queue),
            "recovery queue contains verified record")
    require(all(r.get("Recovery Priority") in {"P1", "P2", "P3"} for r in queue), "invalid recovery priority")

    partition_ids = set()
    partition_canonical = {}
    for letter in "ABCDEFGH":
        part_rows = rows(Path(partition_dir) / f"DEC-STUDIO-NAME-001-MV-{letter}.csv")
        for row in part_rows:
            cid = row["Counted Sample ID"]
            require(cid not in partition_ids, f"duplicate partition record {cid}")
            partition_ids.add(cid)
            partition_canonical[cid] = row["Canonical Entity ID"]
    require(set(counted) == partition_ids, "results do not exactly cover original partitions")
    for row in results:
        require(partition_canonical.get(row["Counted Sample ID"]) == row["Canonical Entity ID"],
                f"partition identity mismatch {row['Counted Sample ID']}")

    counts = Counter(r["Verification Result"] for r in results)
    require(counts == Counter({"STATUS_UNCERTAIN": 163, "VERIFIED_ACTIVE": 22, "VERIFIED_HISTORICAL_CASE": 15}),
            f"unexpected recomputed result totals: {dict(counts)}")
    source_counts = Counter(r["Final Review Status"] for r in sources)
    require(source_counts == Counter({"REVIEWED": 276, "NEEDS_RECHECK": 97, "REJECTED": 59}),
            f"unexpected recomputed source totals: {dict(source_counts)}")
    require(sum(r["Existing/New"] == "NEW" for r in sources) == 32, "expected 32 new sources")
    return errors


def main():
    errors = validate()
    if errors:
        print("MARKET VERIFICATION CONSOLIDATION VALIDATION FAILED")
        for error in errors:
            print("-", error)
        return 1
    print("MARKET VERIFICATION CONSOLIDATION VALIDATION PASSED")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
