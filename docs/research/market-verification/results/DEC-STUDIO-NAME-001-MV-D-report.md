# DEC-STUDIO-NAME-001 MV-D Independent Evidence Verification Report

- Batch ID: MV-D
- Reviewer: independent-market-evidence-verification-reviewer-mv-d
- Review timestamp: 2026-08-10T12:25:06.795802+03:00
- Records reviewed: 30
- VERIFIED_ACTIVE: 0
- STATUS_UNCERTAIN: 30
- REVIEWED sources: 1
- NEEDS_RECHECK sources: 59
- REJECTED sources: 0
- New sources: 0
- Network/access problems: 59

## Method and verdict

All 60 existing source IDs assigned to the 30 MV-D records were requested live with TLS verification and redirect capture. Identity and current-presence claim support were checked against the returned page content. A source was marked REVIEWED only when accessibility, identity, and claim support passed. Source-pair independence was evaluated using publisher/institution and Source Origin Group; pages controlled by the same organisation were not counted as independent evidence.

The active-status rule was applied fail-closed. A record is VERIFIED_ACTIVE only with at least two qualifying REVIEWED sources from two distinct publishers and two distinct origin groups, including at least one primary/official source. Otherwise it remains STATUS_UNCERTAIN. No main market register was modified, and no studio name, candidate brand, domain, username, logo, slogan, Brand Strategy, or Legal/IP conclusion was produced.

## Verification checks

- MV-D programmatic checks: PASS (30 unique in-batch records; 60 unique existing source reviews; VERIFIED_ACTIVE invariants enforced)
- `python scripts/validate_studio.py`: PASS; two non-failing warnings reported for limited semantic founder-authority scanning and Markdown heading-anchor verification
- `python -m unittest discover -s tests -v`: PASS; 105 tests
- `git diff --check`: PASS
