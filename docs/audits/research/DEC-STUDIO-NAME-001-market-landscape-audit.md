# DEC-STUDIO-NAME-001 Market Landscape Independent QA Audit

## Audit identity and independence

- Decision ID: `DEC-STUDIO-NAME-001`
- Audited branch base: `research/DEC-STUDIO-NAME-001-market-landscape`
- Audit branch: `audit/DEC-STUDIO-NAME-001-market-landscape`
- Audited commits: `cc1664fc0083e0aa2c8642eb64b365628d340f52`; `c6e5d64a2349579828acf92e9c683851c55f05a4`
- Audit date: `2026-08-08`
- Auditor role: Independent Market Research QA Auditor
- Independence declaration: The research package's summaries and self-checks were not accepted as evidence. Both CSV files, their references, aggregates, source URLs and method gates were checked directly. No research row, source status, pattern, bias entry or handoff record was edited.
- Conflict declaration: No known financial or personal relationship with the sampled organisations. This audit does not approve its own remediation; a later re-audit must examine any fixes.

## Final decision

**REQUIRED FIXES BEFORE MARKET RESEARCH HANDOFF**

The arithmetic and file structure are reproducible, but open P1 findings prevent handoff. The package does not satisfy the approved rule for genuinely independent activity evidence, contains active-entity/historical-event double counting, leaves all 400 sources unreviewed, and does not provide dated substantive evidence sufficient to uphold all 180 `ACTIVE` decisions. The audit report cannot substitute for row-level source lifecycle updates because this audit-only task is not authorised to change the source register.

## Scope and method

The two CSV files were parsed with Python `csv.DictReader` using `utf-8-sig`. Counts, controlled values, blank fields, normalized names, reference joins, evidence allocations, URL duplication, Markdown aggregates and pattern percentages were recomputed from the rows. All 400 URLs were requested with a fixed audit user agent, redirects enabled, a 15-second timeout and TLS verification bypass only to work around the local interception certificate; bypassing local certificate verification did not turn an HTTP error or unsupported claim into a pass.

Live classifications mean:

- `VERIFIED_ACCESSIBLE`: HTTP success at the recorded URL.
- `VERIFIED_WITH_REDIRECT`: HTTP success after a redirect to a different normalized URL.
- `ACCESS_BLOCKED`: HTTP 401/403/406/418/429/436/451 or equivalent access control.
- `BROKEN_LINK`: HTTP 404/410.
- `RECHECK_REQUIRED`: timeout, DNS/TLS/network failure or other result that could not be confirmed.

HTTP reachability is not substantive claim verification. Page ownership, title and the narrow supported claim were separately inspected in a deterministic stratified record sample. Records were SHA-256 ranked within each active layer; the required number was taken from every layer, then all 20 historical events and every Turkey/near-region and South/Southeast Asia row were added. Overlap was retained and reported rather than counted twice.

Representative commands:

```text
python <temporary audit script>
python scripts/validate_studio.py
python -m unittest discover -s tests -v
git diff --check
```

The temporary audit script was outside the repository and was removed before the audit commit.

## Audit coverage

| Measure | Audited count |
|---|---:|
| Total sample rows | 200 |
| Rows receiving full structural validation | 200 |
| Unique Counted Sample IDs checked | 200 |
| Total source rows | 400 |
| Source IDs receiving structural and live URL checks | 400 |
| Unique records in substantive stratified audit | 89 |
| Source rows marked `UNREVIEWED` | 400 |

### Substantive audit distribution

| Primary layer | Required minimum | Audited records |
|---|---:|---:|
| Büyük uluslararası yayıncılar | 10 | 10 |
| Bağımsız stüdyolar | 15 | 17 |
| Mobil oyun şirketleri | 8 | 16 |
| PC/konsol stüdyoları | 8 | 8 |
| Strateji/MMO üreticileri | 8 | 8 |
| Teknoloji/yaratıcı üretim markaları | 10 | 10 |
| Başarılı rebrand örnekleri | 10 | 10 |
| Marka sorunu örnekleri | 10 | 10 |

Additional required regional coverage overlapped those layer selections: all 12 Turkey/near-region rows and all 3 South/Southeast Asia rows were included. The 89 unique audited records comprised North America 35, Europe 28, East Asia 11, Turkey/near region 12 and South/Southeast Asia 3.

### Live URL results

| Result | Source count |
|---|---:|
| `VERIFIED_ACCESSIBLE` | 221 |
| `VERIFIED_WITH_REDIRECT` | 82 |
| `ACCESS_BLOCKED` | 22 |
| `BROKEN_LINK` | 55 |
| `RECHECK_REQUIRED` | 20 |
| **Total** | **400** |

Because dynamic sites produced minor run-to-run timeout variation, the table records the final complete run. Of the 180 active records, 109 had two technically accessible source URLs, 57 had one, and 14 had none. Accessibility alone did not cure common-origin or claim-support defects.

## Findings

### MRQA-001 — Active entities and their historical events are double-counted

- Finding ID: `MRQA-001`
- Severity: **P1**
- Affected records: at minimum `CNT-DEC-STUDIO-NAME-001-019` and `-184` (Plaion); `-119` and `-186` (Dontnod/Don't Nod). Related corporate/product overlaps also require review: `-001`/`-196` (Electronic Arts/EA Sports), `-003`/`-198` (Ubisoft/title rename), `-013`/`-197` (Blizzard/character rename), `-014`/`-191` (Bethesda/ZeniMax dispute), `-077`/`-192` (King/Candy), and `-181`/`-200` (Meta/Facebook lineage).
- Files: market sample CSV; sampling framework; market handoff.
- Evidence: The framework requires an active organisation and its historical event to be linked but represented by only one `Counted Sample ID` in the 200 quota. The listed rows have distinct Counted Sample IDs, `Related Sample/Event ID` is `NOT APPLICABLE`, and four official URLs are duplicated under separate Source IDs across active and historical allocations (`bethesda.net`, `plaion.com`, `king.com`, `dont-nod.com`).
- Risk: The nominal 200 total and affected 10-row historical quotas contain hidden correlated/duplicate count units; quota completion is therefore not established even though ID strings are unique.
- Recommended action: Build explicit entity/event relationships, select one counted unit under the framework, replace vacated quota rows through the controlled candidate-pool process, and re-run all aggregates.
- Blocks next stage: **Yes**.

### MRQA-002 — The two independent activity-signal rule is systematically unmet

- Finding ID: `MRQA-002`
- Severity: **P1**
- Affected records: all 180 `ACTIVE` rows.
- Files: both market CSV files; research method; sampling framework.
- Evidence: For every active row, both linked source rows carry the same `Publisher/Institution`. The usual pair is the organisation homepage plus another page controlled by the same organisation. The approved audit rule explicitly says two pages on one company site, a parent and controlled site, or two URLs from one platform are not automatically independent evidence. The sample CSV nevertheless uses the same generic evidence text for all rows: an organisation-controlled presence plus a separate official product/news/platform page, without recording institutional independence.
- Risk: A dormant, acquired, redirected or abandoned brand can be classified `ACTIVE` from one corporate origin. This invalidates the active-status threshold and the 150+30 active quota foundation.
- Recommended action: Add a genuinely independent second evidence type/institution for every active row, record provenance separately, use `ACQUIRED_ACTIVE`/`ACTIVE_REBRANDED` where supported, and move unresolved rows to `STATUS_UNCERTAIN` under fail-closed rules.
- Blocks next stage: **Yes**.

### MRQA-003 — Active-status evidence is undated and 71 active rows lack two live sources

- Finding ID: `MRQA-003`
- Severity: **P1**
- Affected records: all 180 active rows for source dating; 71 active rows for fewer than two technically accessible sources.
- Files: both market CSV files; research method; source schema.
- Evidence: `Data Date` is `NOT FOUND` in all 400 source rows. Activity fields do not identify a dated release, update, filing or store record. The live audit found only 109/180 active rows with two accessible URLs; 57 had one and 14 had zero. A website's presence alone does not prove continuing activity, and inaccessible records cannot be passed by this audit.
- Risk: The required 2021-01-01–2026-08-08 activity signal and seven-day recheck gate cannot be reproduced. Some `ACTIVE` classifications may be stale or unsupported.
- Recommended action: Replace or supplement broken/blocked sources, capture the actual page title and dated activity asserted, record `Data Date`, and reclassify any unresolved active row fail-closed.
- Blocks next stage: **Yes**.

### MRQA-004 — Source lifecycle and classification do not support handoff

- Finding ID: `MRQA-004`
- Severity: **P1**
- Affected records: all `SRC-DEC-STUDIO-NAME-001-001` through `-400`.
- Files: market source CSV/Markdown; source-register schema; handoff.
- Evidence: All 400 rows are `UNREVIEWED` with reviewer `NOT ASSIGNED`, yet all 400 are classified `PRIMARY` under one generic Source Type. The rows do not distinguish a company statement from a platform listing, government page or secondary source. Examples include Steam product pages classified identically to company homepages. Source names are generated labels such as “— primary evidence,” not captured page titles. Under the approved lifecycle, a reviewer must check mandatory fields, direct source and claim allocation before `REVIEWED` may be used.
- Risk: The reported 100% primary-source ratio is not independently established, and downstream users cannot distinguish self-report, platform evidence or independent corroboration. An audit narrative outside the register cannot make the rows reviewed or populate their reviewer identities.
- Recommended action: After remediation, an authorised independent reviewer must update or append row-level review status and reviewer identity, correct Source Type/Primary-Secondary classification, capture page titles and limitations, and reject unsupported sources. Do not use the package downstream while the register remains 400/400 unreviewed.
- Blocks next stage: **Yes**.

### MRQA-005 — Historical-event quotas lack consistently claim-specific evidence and criteria

- Finding ID: `MRQA-005`
- Severity: **P1**
- Affected records: `CNT-DEC-STUDIO-NAME-001-181` through `-200`, especially `-185`, `-191`, `-192`, `-193` and `-200`.
- Files: sample/source CSV files; market patterns; sampling framework.
- Evidence: Several historical pairs are a current homepage plus another generic corporate, legal or agency page. For example, the King/Candy record cites the King homepage and the USPTO trademarks landing page, neither of which by itself documents the claimed controversy; the ZeniMax/Scrolls record cites a general legal page and Bethesda homepage; the Behaviour rebrand pair is a homepage and games page. No row records a substantive criterion for “successful” rebrand. All source claims use the same generic wording and all data dates are missing.
- Risk: The 10 rebrand and 10 naming-problem quotas may include true events, but the submitted evidence does not prove each event or the assigned case category. Pattern statements about rebrand/problem causes therefore rest on unreviewed assertions.
- Recommended action: Add event-specific official records or reliable independent reporting with publication dates, document case inclusion criteria, and reject/replace any event that cannot be substantiated.
- Blocks next stage: **Yes**.

### MRQA-006 — Scale categories are not tied to record-specific measurable evidence

- Finding ID: `MRQA-006`
- Severity: **P2**
- Affected records: the 180 active rows.
- Files: sample CSV; sampling framework; bias register.
- Evidence: Every active row uses the same scale criterion, “Official site/portfolio breadth coding; employee count not asserted.” It provides no cited employee count, financial metric, portfolio threshold or official self-description and no date. The totals 81/52/47 are arithmetically reproducible, but the category assignment is not reproducible from an explicit rule.
- Risk: Large-versus-independent comparisons can change with undocumented analyst judgment.
- Recommended action: Define thresholds, cite a record-specific official measure where available, use `SCALE_UNKNOWN` where evidence is absent, and recompute pattern comparisons.
- Blocks next stage: **No by itself; remediation required with the P1 package**.

### MRQA-007 — Numeric pattern tables reproduce, but qualitative comparisons exceed reviewed evidence

- Finding ID: `MRQA-007`
- Severity: **P2**
- Affected records: all pattern inputs; historical rows `181`–`200`.
- Files: market patterns; sample CSV; source CSV.
- Evidence: Structural-form counts (1, 24, 2, 4, 28, 23, 29, 89), suffix counts, regional totals, scale totals and status totals exactly reproduce from the CSV with a denominator of 200 and one-decimal percentage rounding. However, the large/independent comparison is not accompanied by a cross-tab, and stated rebrand/problem causes rely on the unreviewed historical sources described in MRQA-005. The document correctly avoids causal certainty and avoids producing candidates.
- Risk: Downstream readers may treat qualitative observations as audited facts although only the arithmetic is reproduced.
- Recommended action: Add explicit cross-tabs, distinguish 180 active-entity and 20 event denominators where analytically relevant, and retain historical-cause statements only after event evidence review.
- Blocks next stage: **No by itself; remediation required with the P1 package**.

### MRQA-008 — Regional concentration is disclosed but remains analytically limiting

- Finding ID: `MRQA-008`
- Severity: **P3**
- Affected records: regional sample distribution.
- Files: sample register, bias register, patterns, handoff.
- Evidence: Recomputed distribution is Europe 77, North America 71, East Asia 31, Turkey/near region 12, other 6, South/Southeast Asia 3, totaling 200. Europe plus North America is 148/200 (74%); South/Southeast Asia is 1.5%. The bias and handoff documents disclose this limitation and do not call the sample a global census.
- Risk: Structural frequencies primarily reflect European/North American and English-accessible sources; regional spelling/script conclusions would be fragile.
- Recommended action: Preserve the limitation and, after P1 remediation, consider controlled expansion or replacement from underrepresented regions without silently changing layer quotas.
- Blocks next stage: **No by itself**.

## Arithmetic, CSV and reference integrity

- CSV parsing and encoding: **PASS** — both files parse as UTF-8 with BOM, consistent row widths and valid quoting.
- Required fields: **PASS structurally** — 26 sample columns and 23 source columns; no empty strings. `NOT FOUND`/`NOT APPLICABLE` values are semantically audited separately above.
- Counted Sample IDs: **PASS syntactically** — 200 unique, nonblank, well-formed IDs.
- Source IDs: **PASS syntactically** — 400 unique IDs; no template/sample rows.
- References: **PASS** — no missing sample-to-source reference, unreferenced Source ID or invalid Evidence Allocation Counted Sample ID.
- Layer arithmetic: **PASS arithmetically** — 30+45+25+25+25=150 and 30+10+10=50; 200 total.
- Hidden entity/event duplication: **FAIL** — MRQA-001 means unique strings do not establish unique count units.
- Markdown totals: **PASS arithmetically** — sample, region, scale, status, structural-form and suffix tables reproduce from CSV.
- Duplicate URL rows: Four URL values are repeated under separate Source IDs. Some reuse may be legitimate context, but here it also exposes the active/event relationships omitted from the sample linkage fields.

## Status, source independence and substantive conclusions

- Reported status distribution of 180 `ACTIVE`, 10 `HISTORICAL_REBRAND_CASE`, and 10 `HISTORICAL_NAMING_PROBLEM_CASE` reproduces exactly.
- Historical rows are not included in the literal `ACTIVE` count, but active entities and their related historical events are counted separately in the 200 quota.
- Two Source IDs per active row exist structurally; they are not two independent institutional origins.
- The claimed 100% primary/resmî-source ratio is **not accepted**. The uniform classification obscures company-controlled, platform, government and other source classes.
- The substantive 89-record sample meets every requested layer and special-region minimum, but missing dates, generic claim text, common institutional origin and event-generic pages prevent substantive pass.

## Bias, patterns and handoff review

All seven required bias risks exist with affected layer, mitigation, residual risk and confidence. Regional, English-source and live-access risks are carried into handoff. The bias register is therefore structurally adequate, but its mitigation claim that two signals control live-data risk is contradicted by MRQA-002 and MRQA-003.

The pattern document contains no generated studio name, word pool, candidate list or recommendation. Its numeric tables reproduce, but qualitative event causes and scale comparisons are not ready for downstream factual use.

The handoff correctly excludes Brand Strategy, Localization conclusions, Legal/IP clearance, domain/username results and final brand advice. It discloses `UNREVIEWED` status. That disclosure is not a waiver of the lifecycle gate: with 400/400 rows unreviewed and open P1 findings, the package cannot proceed to Brand Strategy or proposal use.

## UNREVIEWED decision

1. This audit report is **not** sufficient as a row-level source review record. It reports defects but does not alter source lifecycle state.
2. The method requires accepted rows to be checked and their `Review Status` and `Reviewer Agent ID` recorded in the source register through authorised remediation.
3. A package with `Reviewer Agent ID = NOT ASSIGNED` on all 400 sources must not be used as an accepted factual handoff.
4. Because this task is audit-only, the safe decision is `REQUIRED FIXES BEFORE MARKET RESEARCH HANDOFF`; the research owner must remediate, and a different independent reviewer must re-audit.
5. No conditional downstream use is approved. The current files may be used only as a defect/remediation input, not as accepted naming-process evidence.

## Prohibited-content scan

**PASS.** The generated market files contain existing organisations and historical events only. No new studio name, candidate brand, word pool, name combination, domain/username availability result, logo, slogan, legal-clearance conclusion or final brand recommendation was found.

## Technical verification

- `python scripts/validate_studio.py`: **PASS** — `STUDIO VALIDATION PASSED`; 29 agent profiles and 51 required files checked. The validator emitted its two known non-failing semantic/parser warnings.
- `python -m unittest discover -s tests -v`: **PASS** — 98 tests, `Ran 98 tests in 73.680s`, `OK`.
- `git diff --check`: **PASS** — no output.
- Worktree cleanliness: verified after the single audit commit in the delivery check.

## Required remediation and re-audit gate

The research owner should correct MRQA-001 through MRQA-005 without modifying this audit. MRQA-006 and MRQA-007 should be handled in the same remediation because they affect downstream analysis; MRQA-008 remains a disclosed sampling limitation. The remediated source register must record row-level reviewer status and identity, and a reviewer independent from both the original research package and remediation must issue a new audit before handoff.
