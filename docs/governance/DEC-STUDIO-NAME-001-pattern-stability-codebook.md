# DEC-STUDIO-NAME-001 Pattern Stability Codebook

## Decision ID
DEC-STUDIO-NAME-001

## Record ID
REC-DEC-STUDIO-NAME-001-PATTERN-STABILITY-CODEBOOK-001

## Author Agent ID
research-method-governance-lead

## Created Date
2026-08-11

## Source References
REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-INDEPENDENT-AUDIT-001; `docs/research/DEC-STUDIO-NAME-001-market-sample-register.csv`

## Conflict of Interest Declaration
Codebook remediation yazarı kendi stability sonucunu bağımsız onaylayamaz.

## Previous Stage References
REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-INDEPENDENT-AUDIT-001

## Next Stage
Independent governance re-audit ve Founder acceptance; sonra recovery öncesi immutable freeze

## Review Target
`quality-assurance-director`; bağımsız governance reviewer; `FOUNDER`

## Independence Declaration
Codebook yalnız mevcut kontrollü yapısal alanları kategorize eder; isim veya yeni pattern fikri üretmez ve mevcut veriyi yeniden kodlamaz.

## Version ve freeze

- Codebook version: `ASA-STABILITY-v1.0.0`.
- Recovery başlamadan dosya SHA-256'sı lineage manifestte dondurulur.
- Sonuç görüldükten sonra metric eklemek/silmek, mapping veya threshold değiştirmek ve checkpoint seçmek yasaktır. Değişiklik yeni version, method amendment, independent re-audit ve Founder acceptance gerektirir.

## Authoritative controlled fields

Primary metrics yalnız market sample register'daki `Name Structural Form` ve `Suffix/Descriptor` alanlarından hesaplanır. Free-text çıkarım yapılamaz. Exact normalized mappings:

| Primary metric | Controlled value/mapping |
|---|---|
| `single_word` | `Name Structural Form` = `Tek kelime` veya `Single word` |
| `two_word` | `Name Structural Form` = `İki kelime` veya `Two words` |
| `compound` | `Name Structural Form` = `Birleşik kelime` veya `Compound` |
| `coined` | `Name Structural Form` = `Uydurma/coined` veya `Coined` |
| `descriptive` | `Name Structural Form` = `Açıklayıcı` veya `Descriptive` |
| `founder_surname` | `Name Structural Form` = `Founder/surname` veya `Kurucu/soyadı` |
| `geographic` | `Name Structural Form` = `Geographic` veya `Coğrafi` |
| `acronym` | `Name Structural Form` = `Acronym` veya `Kısaltma` |
| `suffix_games` | `Suffix/Descriptor` exact token `Games` |
| `suffix_studios` | `Suffix/Descriptor` exact token `Studios` |
| `suffix_interactive` | `Suffix/Descriptor` exact token `Interactive` |
| `suffix_entertainment` | `Suffix/Descriptor` exact token `Entertainment` |

Whitespace trim ve Unicode casefold dışında normalizasyon yapılmaz. Bir row bir structural metric ve ayrıca en fazla bir suffix metric taşıyabilir. Controlled value listesinde olmayan boş/değer `MISSING_OR_AMBIGUOUS` olur; post-hoc sınıflandırılmaz.

## Checkpoints ve metric

Checkpointler deterministic selection order'ın prefix'leridir: `N=40`, `N=50`, `N=60`; gerekirse `70`, `80`, `90`. Her primary metric için `rate_N = coded_true_N / N * 100`. Ardışık değişim `absolute percentage-point delta = abs(rate_N - rate_(N-10))`dır.

## Stability PASS

İki ardışık +10 blokta bütün preregistered primary metricler için maksimum absolute delta `<=7.5` percentage point olmalıdır. `N=60`ta durmak için hem `40->50` hem `50->60` geçmelidir. Missing/ambiguous structural coding toplam sample'ın `%5`ini aşarsa stability `FAIL`dır.

`N=60`ta FAIL ise deterministic recovery `70`, `80`, `90` checkpointlerine devam eder ve her checkpointte son iki ardışık blok değerlendirilir. `N=90`da PASS yoksa `GOVERNANCE_ESCALATION_REQUIRED`; Brand Strategy gate kapalıdır. Checkpoint atlamak veya yalnız uygun blokları seçmek yasaktır.

## Output evidence

Her checkpoint sample version/hash, codebook version/hash, N, metric numerator/denominator/rate, adjacent delta, maximum delta, missing/ambiguous count/rate ve PASS/FAIL taşır. Raw entity adları yeni kategori üretmek için incelenmez.

## Status

`REQUIRES_INDEPENDENT_VERIFICATION`; recovery gate `CLOSED_PENDING_INDEPENDENT_REAUDIT_AND_FOUNDER_ACCEPTANCE`.
