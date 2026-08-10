# DEC-STUDIO-NAME-001 MV-E Independent Market Evidence Verification

- Batch ID: MV-E
- Reviewer: independent-market-evidence-reviewer-mv-e
- Review date: 2026-08-10
- Scope: exactly 30 MV-E records and 60 existing Source IDs.
- Rule: fail-closed; same-organisation pages do not form an independent pair.

## Outcome

- Records reviewed: 30
- VERIFIED_ACTIVE: 1
- STATUS_UNCERTAIN: 29
- REVIEWED sources: 38
- NEEDS_RECHECK sources: 11
- REJECTED sources: 11
- New sources: 0
- Network/access problems: 16

Only CNT-DEC-STUDIO-NAME-001-147 met the full threshold through SRC-DEC-STUDIO-NAME-001-293 (official Kakao Games) and SRC-DEC-STUDIO-NAME-001-294 (independent Wikipedia reference). All other records remain STATUS_UNCERTAIN.

## Method

All 60 existing URLs were requested live with redirects on 2026-08-10. HTTP outcome, final URL, identity, current-activity claim support, publisher/institution, corporate control, and origin group were reviewed. Python-client CA failures were discarded and repeated with the system curl TLS stack. No main market register was modified.
