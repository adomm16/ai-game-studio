# DEC-STUDIO-NAME-001 Verification Consolidation Summary

## Scope and method

This package consolidates the committed MV-A through MV-H verification outputs. All totals below were recomputed programmatically from the batch CSV files; batch report totals were used only as comparison points. No new source research, status inflation, candidate naming, Brand Strategy, or main-register update was performed.

## Observations

- Total records: 200 (180 active cohort; 20 historical cohort).
- VERIFIED_ACTIVE: 22/180 (12.2%).
- VERIFIED_HISTORICAL_CASE: 15/20 (75.0%).
- STATUS_UNCERTAIN: 163 total.
- Source review rows: 432; REVIEWED 276, NEEDS_RECHECK 97, REJECTED 59.
- New sources: 32.
- Recovery queue: 163 records; P1 44, P2 45, P3 74.
- Recovery records with a network/access failure: 77/163.

| Batch | Verified | Total | Success rate |
|---|---:|---:|---:|
| MV-A | 6 | 30 | 20.0% |
| MV-B | 8 | 30 | 26.7% |
| MV-C | 3 | 30 | 10.0% |
| MV-D | 0 | 30 | 0.0% |
| MV-E | 1 | 30 | 3.3% |
| MV-F | 4 | 30 | 13.3% |
| MV-G | 7 | 10 | 70.0% |
| MV-H | 8 | 10 | 80.0% |

Most frequent source-level failure indicators (not mutually exclusive): access/network 129, claim support 152, identity 129.

## Interpretation

The evidence threshold remains unmet for most records. Access/network problems contribute materially, but claim-support, identity, primary-source, and independence defects also remain; network recovery alone therefore cannot be assumed to close the queue. Priority is a recovery work order, not governance severity.

## Gate

The consolidated evidence is an auditable recovery input, not a final market-research approval. The main Market Landscape validator is expected to fail because the protected main sample register remains fail-closed with zero ACTIVE records; this task intentionally does not mutate that register.
