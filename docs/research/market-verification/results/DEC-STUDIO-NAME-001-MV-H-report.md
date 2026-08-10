# DEC-STUDIO-NAME-001 MV-H Independent Verification Report

- Batch: `MV-H`
- Reviewer: `independent-market-evidence-reviewer-MV-H`
- Review date: `2026-08-10`
- Scope: 10 `HISTORICAL_NAMING_PROBLEM_CASE` records in CNT-191 through CNT-200 only.
- Collector/reviewer separation: collector role in the source register is `market-research-director`; this review was performed by the independent reviewer role above.

## Verdict summary

Eight cases meet the fail-closed historical threshold and are `VERIFIED_HISTORICAL_CASE`. Two are `STATUS_UNCERTAIN`: CNT-191 lacks an accessible case-specific official source paired with the independent report, and CNT-193 lacks a reliable independent supporting source for the official impersonation warning.

Reasons are classified separately from event verification. CNT-194, CNT-197, and CNT-198 remain `REASON_UNVERIFIED`; in particular, Ubisoft denied that the Monster Energy opposition caused the Gods & Monsters rename, so the causal claim was not promoted.

## Counts

| Measure | Count |
|---|---:|
| Records reviewed | 10 |
| VERIFIED_HISTORICAL_CASE | 8 |
| STATUS_UNCERTAIN | 2 |
| Source rows reviewed | 40 |
| REVIEWED sources | 25 |
| NEEDS_RECHECK sources | 0 |
| REJECTED sources | 15 |
| New sources | 20 |
| Network/access problems | 10 |

Network/access problems comprise six existing broken/blocked sources and four new candidate sources blocked by HTTP 403. Blocked candidates were retained in the source review and replaced where an accessible alternative was available.

## Programmatic checks

The batch result contains exactly 10 unique MV-H sample IDs and no out-of-batch row. Every `VERIFIED_HISTORICAL_CASE` has at least one qualifying primary/official source and at least one qualifying independent source from a distinct origin group. No `CONFIRMED_REASON` is assigned without explicit source support.

## Scope controls

Only the three MV-H result files were created. The partition input, main market CSV/Markdown registers, other batches, studio naming, candidate brands, domains, usernames, logos, slogans, Brand Strategy, and Legal/IP conclusions were not changed or produced.
