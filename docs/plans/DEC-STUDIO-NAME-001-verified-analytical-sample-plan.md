# DEC-STUDIO-NAME-001 Verified Analytical Sample Plan

## Decision ID
DEC-STUDIO-NAME-001

## Record ID
REC-DEC-STUDIO-NAME-001-VERIFIED-ANALYTICAL-SAMPLE-PLAN-001

## Author Agent ID
research-method-governance-lead

## Created Date
2026-08-11

## Source References
REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-001; `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-consolidated-results.csv`; `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-recovery-queue.csv`; `docs/research/DEC-STUDIO-NAME-001-sampling-framework.md`

## Conflict of Interest Declaration
Plan yazarı amendment yazarıdır ve bu planın bağımsız kabulünü yapamaz.

## Previous Stage References
REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-001

## Next Stage
Independent Governance Audit; ardından yalnız kabul edilen planla recovery execution

## Review Target
`quality-assurance-director`; amendment yazarı olmayan bağımsız governance reviewer; `FOUNDER`

## Independence Declaration
Bu plan kayıt statüsü değiştirmez, kaynak araştırmaz, isim üretmez ve Brand Strategy başlatmaz. Yalnız önerilen amendment kabul edilirse uygulanacak seçim ve gate kontrollerini tanımlar.

## Amaç ve başlangıç noktası

200 kayıtlık Market Context Inventory'yi değiştirmeden en az 60 kayıtlık, stratified ve yalnız `VERIFIED_ACTIVE` üyelerden oluşan yeniden üretilebilir analitik sample kurmak. Mevcut çekirdek 22 `VERIFIED_ACTIVE` kayıttır; minimuma ulaşmak için en az 38 ek kayıt gerekir. Recovery queue 44 P1, 45 P2 ve 74 P3 kayıt içerir.

## Değişmez sınırlar

- Ana sample/source register veya consolidated verification statüleri bu plan tarafından değiştirilmez.
- Yeni kaynak ve yeni araştırma bu belge hazırlanırken yapılmaz.
- Inclusion yalnız önceden doğrulanmış sonuçlara referans verir.
- Context Inventory ve analytical sample ayrı sürüm ve paydalar taşır.
- P1/P2/P3 çalışma sırası governance severity veya otomatik inclusion sırası değildir.

## Uygulama adımları

1. Amendment için bağımsız governance audit sonucu alınır. `PASS`/açık kabul yoksa bu plan başlamaz.
2. 200 kayıtlık Context Inventory snapshot'ı hash, tarih ve kaynak commit ile dondurulur.
3. Mevcut 22 `VERIFIED_ACTIVE` kayıt stratum, bölge ve ölçek bazında sayılır; eksik guardrail hücreleri yayımlanır.
4. Recovery P1 kayıtları, global sıranın yanında eksik stratum ve bölge hücrelerine göre çalışma görünümüyle önceliklendirilir. Kayıt ancak mevcut cohort-specific evidence eşiğini gerçekten geçerse eligible olur.
5. P1 yeterli değilse eksik zorunlu hücreler için P2, sonra P3 çalışılır. Her sıra sapması ve bias etkisi kaydedilir.
6. Eligible havuz 60'ı geçtiğinde seçim, özgün aktif katman ağırlıklarına yaklaşmayı ve bölge yoğunlaşmasını azaltmayı amaçlayan önceden kaydedilmiş deterministik sıra ile yapılır; tanınmışlık veya uygun sonuç elle seçilemez.
7. Inclusion manifest üretilir ve bağımsız QA aynı CSV'lerden sayımları yeniden yapar.
8. Sample `n=60`, sonra her ek 10 kayıtta ana pattern kodları için istikrar kontrolü yapılır. Son ek blok yeni bir ana pattern, ana pattern yön değişimi veya stratum duyarlılığı yaratıyorsa recovery sürer.
9. Tarihsel analiz ayrı manifest ve payda ile, yalnız doğrulanmış tarihsel vakalardan oluşturulur.
10. Bütün gate kanıtları kabul edilmeden Brand Strategy'ye handoff yapılmaz.

## Inclusion manifest zorunlu alanları

`Analytical Sample Version`, `Counted Sample ID`, `Canonical Entity ID`, `Verification Result`, `Verification Evidence Version`, `Primary Layer`, `Region`, `Scale`, `Qualifying Source IDs`, `Primary Source Present`, `Independent Sources Present`, `Reviewer Agent ID`, `Verification Date`, `Included/Excluded`, `Selection Order`, `Selection Rationale`, `Related Sample/Event ID`, `Bias Flags`.

Bir kayıt `Included` olabilmek için `VERIFIED_ACTIVE`, iki qualifying bağımsız source, en az bir primary/official source, reviewer ve doğrulama tarihi taşımalıdır. Boş veya `STATUS_UNCERTAIN` kayıt fail-closed biçimde dışarıda kalır.

## Aktif acceptance matrix

| Gate | PASS koşulu |
|---|---|
| Sample size | `VERIFIED_ACTIVE n >= 60` |
| Stratum presence | Altı aktif birincil katmanın her birinde `n >= 6` |
| Stratum concentration | Hiçbir katman `%30` üzerinde değil; özgün ağırlıktan maddi sapma gerekçeli ve bağımsız kabul edilmiş |
| Region presence | Altı bölgenin her birinde `n >= 3` |
| Region concentration | Tek bölge `<= %35`; en büyük iki bölge birlikte `<= %60` |
| Evidence | Her included kayıt iki bağımsız qualifying source ve en az bir primary/official source taşıyor |
| Dedup | Her `Counted Sample ID` analitik paydada bir kez sayılıyor |
| Reproducibility | Snapshot, manifest, seçim sırası, script/komut çıktısı ve reviewer kaydı mevcut |
| Stability | Son kontrol bloğu ana pattern setini veya yönünü maddi biçimde değiştirmiyor; duyarlılık açıklanmış |
| Independence | Sample üreticisi dışındaki QA/governance reviewer yazılı kabul vermiş |

Herhangi bir satır başarısızsa gate kapalıdır; toplam `n` fazlası başka bir guardrail açığını telafi etmez.

## Tarihsel acceptance matrix

| Gate | PASS koşulu |
|---|---|
| Sample size | `VERIFIED_HISTORICAL_CASE n >= 12` |
| Strata | Başarılı rebrand ve naming/brand problem katmanlarının her birinde `n >= 5` |
| Separation | Active ve historical paydalar ayrı; `Event ID` ve `Counted Sample ID` dedup kontrolü geçmiş |
| Interpretation | Yalnız keşifsel vaka pattern'i; nedensellik ve prevalans iddiası yok |

Mevcut 15 vaka (7 + 8) sayısal gate'i karşılayan başlangıç paketidir; beş uncertain vaka dahil edilmez.

## Raporlama şablonu

Her analitik tablo şu dipnotu doldurur:

`Sample version: [id] | Cohort: [active/historical] | n=[x] | Denominator: verified included records only | Cut-off: [date] | Confidence: [level] | Limitations: [small-n/coverage/bias]`

Context tablosu ayrıca `CONTEXT ONLY — NOT AN ANALYTICAL DENOMINATOR` etiketi taşır. Context `n=200` ile analytical `n` aynı yüzde veya toplamda birleştirilmez.

## Bias kontrolleri

- Verified ve uncertain dağılımları stratum, bölge ve ölçek bazında yan yana raporlanır; status değiştirilmez.
- İngilizce kaynak, büyük/tanınmış marka, erişilebilirlik, survivorship, platform ve bölge bias'ları güncel bias register'a eklenir.
- En büyük stratum çıkarma duyarlılık testi yapılır.
- Küçük alt grup `n < 10` ise yalnız betimsel sinyal üretilir.
- Eligible fakat seçilmeyen kayıtların dışlama gerekçesi korunur; sonucu desteklemeyen verified kayıtlar elenemez.

## Stop ve escalation

Minimum ve bütün guardrail'ler geçince kalan P2/P3 recovery zorunlu değildir. Unresolved kayıtlar Context Inventory'de kalır. Zorunlu stratum/bölge hücresi, evidence, dedup, stability veya bağımsız review başarısızsa recovery sürer. Guardrail değişikliği yalnız yeni bir method amendment ve bağımsız audit ile yapılabilir.

## Handoff paketi

Brand Strategy'ye yalnız: dondurulmuş manifest; yeniden hesaplanan gate tablosu; bias register; dedup raporu; stability/sensitivity sonucu; sample sürümü; açık limitations; independent audit referansı verilir. Brand Strategy Context Inventory kayıtlarını analitik kanıta yükseltemez.

## Status

**PROPOSED — REQUIRES INDEPENDENT GOVERNANCE AUDIT**
