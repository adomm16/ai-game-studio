# DEC-STUDIO-NAME-001 Analytical Lineage and Freshness Specification

## Decision ID
DEC-STUDIO-NAME-001

## Record ID
REC-DEC-STUDIO-NAME-001-ANALYTICAL-LINEAGE-FRESHNESS-SPEC-001

## Author Agent ID
research-method-governance-lead

## Created Date
2026-08-11

## Source References
REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-INDEPENDENT-AUDIT-001; authoritative CSV inputs named below

## Conflict of Interest Declaration
Remediation yazarı kendi lineage/freshness kontrolünü bağımsız doğrulanmış ilan edemez.

## Previous Stage References
REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-INDEPENDENT-AUDIT-001

## Next Stage
Independent governance re-audit; Founder acceptance; sonra versioned manifest freeze

## Review Target
`quality-assurance-director`; bağımsız governance reviewer; `FOUNDER`

## Independence Declaration
Bu specification kaynak araştırmaz, erişim doğrulamaz veya status değiştirmez; yalnız gelecekteki execution için fail-closed lineage/freshness sözleşmesidir.

## Specification version

`ASA-LINEAGE-v1.0.0`

## Authoritative ownership

| Owned field | Repository path | Schema version |
|---|---|---|
| Verification Result, Record Type, qualifying source IDs, reviewer | `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-consolidated-results.csv` | `MV-CONSOLIDATED-v1` |
| Evidence/source accessibility, identity, claim support, qualification and review date | `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-consolidated-source-review.csv` | `MV-SOURCE-REVIEW-v1` |
| Primary Layer, Region, Scale, canonical dimensions and structural fields | `docs/research/DEC-STUDIO-NAME-001-market-sample-register.csv` | `MARKET-SAMPLE-v1` |
| Recovery population and Recovery Priority | `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-recovery-queue.csv` | `MV-RECOVERY-v1` |

Her input manifest row'u repository path, source commit SHA, raw file SHA-256, schema version ve header hariç record count taşır. Manifest freeze timestamp timezone-aware ISO-8601 UTC olmalıdır. Snapshot hash selection spec'teki canonical `path=sha256` algoritmasıyla hesaplanır.

## Join and precedence

- Primary join key `Counted Sample ID`; source-review dosyasındaki eşdeğer alan `Sample ID`dır.
- Secondary integrity key `Canonical Entity ID`dır; source review bu alanı taşımadığından result/register/queue arasında doğrulanır.
- Her authoritative population içinde key unique olmalı ve gereken join tam `1:1` olmalıdır.
- Duplicate, missing join, `1:n`, `n:1`, Canonical Entity mismatch, incompatible Record Type, conflicting Primary Layer veya conflicting Region `LINEAGE_FAIL` üretir.
- `Verification Result` için consolidated result authoritative'dir. Sample register'daki eski status bunu override edemez; status drift manifestte görünür warning olur, fakat yalnız consolidated result eligibility belirler.
- Boyut alanı boşsa, farklı authoritative girdiler çelişiyorsa veya evidence source ID source review'da tekil `REVIEWED` qualifying row'a bağlanmıyorsa inclusion fail-closed olur.

## Freshness

Inclusion yaş hesabı: `manifest freeze timestamp - verification timestamp`. Verification timestamp, qualifying source review kayıtlarındaki en eski geçerli review timestamp ve record-level verification timestamp içinden en eski olanıdır; sadece tarih varsa gün sonu varsayılmaz, UTC gün başlangıcı kullanılarak konservatif hesap yapılır. Age `>7 days`, timestamp missing/naive/future veya source review date missing ise `STALE_RECHECK_REQUIRED`; analytical inclusion yapılamaz.

Brand Strategy handoff'tan hemen önce aynı lineage snapshot üzerinde freshness yeniden çalıştırılır. Kaynak erişilebilirlik/review tarihi de aynı 7 günlük gate'e tabidir. Recheck yeni append-only evidence/version üretir; eski tarih sessizce değiştirilemez.

## Inclusion rules

- Freshness ve evidence şartlarını geçen bütün `VERIFIED_ACTIVE` kayıtlar active manifestte `Included` olmalıdır.
- Freshness ve evidence şartlarını geçen bütün `VERIFIED_HISTORICAL_CASE` kayıtlar historical manifestte `Included` olmalıdır; subsampling yasaktır.
- `STATUS_UNCERTAIN`, stale, missing-date veya join-failed kayıt `Excluded` olur ve objective exclusion code taşır; analitik paydaya giremez.
- Active ve historical manifest/payda ayrıdır; `Counted Sample ID` her paydada tekildir.

## Manifest fields

`Manifest Version`, `Freeze Timestamp`, `Source Commit`, `Input Path`, `Input SHA-256`, `Schema Version`, `Record Count`, `Snapshot Hash`, `Counted Sample ID`, `Canonical Entity ID`, `Record Type`, `Verification Result`, `Primary Layer`, `Region`, `Scale`, `Scale Review Status`, `Qualifying Source IDs`, `Reviewer Agent ID`, `Verification Timestamp`, `Freshness Age`, `Included/Excluded`, `Objective Reason`, `Algorithm/Codebook Version`.

## Status

`REQUIRES_INDEPENDENT_VERIFICATION`; no live recheck was performed. Recovery gate remains `CLOSED_PENDING_INDEPENDENT_REAUDIT_AND_FOUNDER_ACCEPTANCE`.
