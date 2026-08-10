# DEC-STUDIO-NAME-001 MV-F Independent Market Evidence Verification Report

- Batch: `MV-F`
- Reviewer role: `independent-market-evidence-verification-reviewer-mv-f`
- Collector role: `market-research-director` (different from reviewer)
- Review date: `2026-08-10`
- Records reviewed: 30
- `VERIFIED_ACTIVE`: 4
- `STATUS_UNCERTAIN`: 26
- Sources reviewed: 60 existing, 0 new
- Final source status: 47 `REVIEWED`, 10 `NEEDS_RECHECK`, 3 `REJECTED`
- Network/access problems: 10

## Method

All 60 existing source IDs were re-requested live. Accessibility was classified only with the allowed vocabulary. Identity and scoped claim support were evaluated separately. Same-company pages, controlled sibling domains, and two URLs from the same platform or institution were not treated as independent. `BROKEN_LINK`, `ACCESS_BLOCKED`, `RECHECK_REQUIRED`, and identity-mismatched evidence was retained but did not qualify.

Python's TLS client reported a local certificate-chain validation failure broadly, so it was not used as proof that remote links were broken. A second live `curl -L` check determined completed HTTP accessibility. Partial timeout/TLS/protocol results remained `RECHECK_REQUIRED` under fail-closed handling.

## Verification outcome

Only Unity Technologies, Stability AI, SideFX, and Samsung met the complete two-source rule using an accessible official source plus an accessible independent reference from a different publisher and origin group. The other 26 records remain `STATUS_UNCERTAIN`; source-level accessibility does not override missing source independence.

## Scope declaration

Only the three MV-F result files were created. No batch input, main market CSV/Markdown register, other batch, studio name, candidate brand, domain, username, logo, slogan, Brand Strategy, or Legal/IP conclusion was changed or produced.
