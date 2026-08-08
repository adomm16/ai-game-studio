# DEC-STUDIO-NAME-001 Market Sample Register

The CSV is canonical. It contains 200 unique Counted Sample IDs and 200 unique Canonical Entity IDs across fixed layer quotas: 150 game/publisher slots, 30 adjacent technology/creative slots, 10 rebrand cases, and 10 naming-problem cases.

Seven active rows that overlapped retained historical cases were replaced in-place under the dedup ledger. No neighbouring-layer transfer occurred. Every row now records normalized identity, parent/relationship fields, counted layer, historical relationship, and duplicate-review status.

## Fail-closed status

All 180 current-entity rows are `STATUS_UNCERTAIN`, not `ACTIVE`, because the live source recheck could not establish two independently reviewed activity signals in this execution environment. Their scale is `UNKNOWN`; historical scale is `NOT_APPLICABLE`. The nominal layer slots remain visible, but the active quota gate is not satisfied.

## Distribution

- Europe: 77
- North America: 70
- East Asia: 32
- Turkey and near region: 12
- Other regions: 6
- South and Southeast Asia: 3

This purposive sample is not a global census. Europe and North America remain materially overrepresented.
