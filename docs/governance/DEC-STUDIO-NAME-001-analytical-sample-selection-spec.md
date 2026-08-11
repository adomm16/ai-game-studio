# DEC-STUDIO-NAME-001 Analytical Sample Selection Specification

## Decision ID
DEC-STUDIO-NAME-001

## Record ID
REC-DEC-STUDIO-NAME-001-ANALYTICAL-SELECTION-SPEC-001

## Author Agent ID
research-method-governance-lead

## Created Date
2026-08-11

## Source References
REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-001; REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-INDEPENDENT-AUDIT-001; `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-recovery-queue.csv`; `docs/research/DEC-STUDIO-NAME-001-market-sample-register.csv`

## Conflict of Interest Declaration
Bu specification amendment remediation yazarınca hazırlanmıştır; yazar kendi kontrolünü bağımsız doğrulanmış veya onaylanmış ilan edemez.

## Previous Stage References
REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-INDEPENDENT-AUDIT-001

## Next Stage
Independent governance re-audit; ardından yalnız açık Founder kabulü varsa recovery execution

## Review Target
`quality-assurance-director`; amendment yazarından bağımsız governance reviewer; `FOUNDER`

## Independence Declaration
Bu belge araştırma yapmaz, source doğrulamaz, kayıt statüsü değiştirmez, isim üretmez ve recovery başlatmaz.

## Version ve population

- Algorithm version: `ASA-SELECTION-v1.0.0`.
- Active recovery population yalnız `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-recovery-queue.csv` içindeki `Record Type=ACTIVE` ve `Current Verification Result=STATUS_UNCERTAIN` satırlarıdır.
- `Primary Layer`, `Region` ve gerektiğinde `Scale` yalnız `docs/research/DEC-STUDIO-NAME-001-market-sample-register.csv` üzerinden `Counted Sample ID` ile join edilir.
- Join cardinality tam `1:1`dır. Duplicate, missing, `1:n`, `n:1`, Canonical Entity ID mismatch, incompatible Record Type, conflicting Primary Layer veya conflicting Region durumunda `SELECTION_INPUT_INVALID` üretilir ve işlem durur.
- Freshness ve evidence gate'lerini geçen bütün mevcut `VERIFIED_ACTIVE` kayıtlar otomatik `Included` olur. Sonuç, bilinirlik, çekicilik veya araştırmacı tercihi nedeniyle çıkarılamaz.

## Stratum target bands

Original active context counts sabittir: Büyük uluslararası yayıncılar 30; Bağımsız stüdyolar 45; Mobil oyun şirketleri 25; PC/konsol stüdyoları 25; Strateji/MMO üreticileri 25; Teknoloji/yaratıcı üretim markaları 30. Toplam 180.

Her sample büyüklüğü `N` için:

```text
Expected_i(N) = N * original_stratum_count_i / 180
Lower_i(N) = max(6, floor(0.80 * Expected_i(N)))
Upper_i(N) = min(floor(0.30 * N), ceil(1.20 * Expected_i(N)))
```

Bu target band “maddi sapma” yerine geçer. Reviewer takdiriyle değiştirilemez. Her stratum `Lower_i(N) <= observed_i <= Upper_i(N)` sağlamalıdır. `18/18/6/6/6/6`, `N=60` target-band kontrolünde geçerli değildir.

## Deterministic candidate ordering

Her recovery adımında eligible `STATUS_UNCERTAIN` adayları şu lexicographic tuple ile artan sıralanır:

1. Hard stratum lower-bound deficit kapatanlar önce (`0`, diğerleri `1`).
2. Mevcut `Region n < 3` açığını kapatanlar önce (`0`, diğerleri `1`).
3. Stratum expected-share deficit büyüklüğü azalan (`Expected_i(N+1) - current_i`; sort key bunun negatifi).
4. Mevcut Stratum x Region hücre sayısı artan.
5. Recovery Priority rank: `P1=0`, `P2=1`, `P3=2`.
6. Stable tie-break artan hexadecimal:

```text
SHA256(frozen_input_snapshot_hash + "|" + Counted Sample ID)
```

`frozen_input_snapshot_hash`, lineage manifestteki dört authoritative input için sıralı `path=sha256` satırlarının LF ile birleştirilmiş UTF-8 SHA-256 değeridir. Aynı snapshot ve candidate pool aynı sırayı üretmelidir. İsim/pattern sonucu, marka çekiciliği, bilinirlik veya araştırmacı tercihi ranking girdisi olamaz.

## Caps ve candidate skip

Yeni verified kayıt eklendiğinde tek stratum `%30` üstüne, tek region `%35` üstüne veya en büyük iki region toplamı `%60` üstüne çıkacaksa candidate `HARD_CAP_SKIP` olur ve sıradaki eligible aday değerlendirilir. Bütün adaylar cap ihlali yaratıyorsa `GOVERNANCE_ESCALATION_REQUIRED`; status değişikliği ve gate açılması yasaktır.

## Cross-cell control

Her snapshot için tam Stratum x Region matrisi üretilir. Bir hücre toplam sample'ın `%15`inden fazlaysa `CROSS_CELL_CONCENTRATION_WARNING` oluşturulur. Bu tek başına hard block değildir; bias register, largest-cell removal sensitivity, downstream limitation ve independent reviewer değerlendirmesi zorunludur. Hard stratum/region caps değişmez. `n=3` yalnız region presence'tır; temsiliyet değildir.

## Selection log

Her adım append-only logda şunları taşır: candidate pool ve sıralı tuple'lar; selection öncesi stratum/region/cross-cell deficits; selected `Counted Sample ID`; selection reason; algorithm version; snapshot hash; tie-break hash; UTC timestamp; hard-cap skip/escalation kayıtları. Eligible olup seçilmeyenler ve gerekçeleri korunur.

## Scale

Scale recovery selection için hard criterion değildir. Yalnız `Scale Review Status=REVIEWED` ve evidence alanları doluysa analitik karşılaştırmada kullanılabilir. Reviewed scale coverage `<80%` ise scale percentage/comparison yasaktır; yalnız data-gap/bias raporlanır. Coverage `>=80%` olsa da unknown kayıtlar denominator'da görünür ve confidence/limitations zorunludur.

## Gate

Bu specification tek başına execution yetkisi vermez. Durum: `REQUIRES_INDEPENDENT_VERIFICATION`; recovery gate: `CLOSED_PENDING_INDEPENDENT_REAUDIT_AND_FOUNDER_ACCEPTANCE`.
