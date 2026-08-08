# DEC-STUDIO-NAME-001 Market Source Register

## Scope

The canonical source rows are in `DEC-STUDIO-NAME-001-market-source-register.csv`: **400 unique Source IDs**, two allocated to each counted sample/event. All 23 mandatory fields from the approved source schema are populated; unavailable publication/data dates are explicitly `NOT FOUND`.

## Source quality and review state

- Primary classification: **400/400 (100%)**. Here `PRIMARY` means an organisation-controlled page or direct official platform record; it is not an independence claim.
- Volatility: **400 HIGHLY_CHANGEABLE**; recheck date `2026-08-15`.
- Review state: **400 UNREVIEWED**; reviewer `NOT ASSIGNED`.
- Conflict declaration: `NONE KNOWN` for the research author; evaluated organisations may have self-reporting incentives, captured in limitations.

## Evidence allocation

Each source row points to one `Counted Sample ID` and either `primary` or `corroborating` activity/event evidence. Search snippets, community wikis and generative-AI output are not used as source rows. A source may establish only the narrow claim written in `Supported Claim`; it does not establish market share, legal clearance, reputation, ownership, financial size or brand suitability.

## Recheck protocol

Before Research Memo/proposal handoff, an independent reviewer must open every direct URL, confirm institution identity and supported claim, record redirects/access failures, and append replacement Source IDs rather than silently overwriting rows. Any active record losing one of two signals becomes `STATUS_UNCERTAIN` and is escalated; the quota then becomes incomplete until replaced under QA control.
