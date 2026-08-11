# DEC-STUDIO-NAME-001 Market Method Amendment

## Decision ID
DEC-STUDIO-NAME-001

## Record ID
REC-DEC-STUDIO-NAME-001-MARKET-METHOD-AMENDMENT-001

## Author Agent ID
research-method-governance-lead

## Created Date
2026-08-11

## Source References
`docs/plans/DEC-STUDIO-NAME-001-founder-research-mandate.md`; `docs/studio/research-policy.md`; `docs/studio/decision-protocol.md`; `docs/studio/quality-gates.md`; `docs/research/DEC-STUDIO-NAME-001-research-method.md`; `docs/research/DEC-STUDIO-NAME-001-sampling-framework.md`; `docs/plans/DEC-STUDIO-NAME-001-research-execution-plan.md`; `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-consolidated-results.csv`; `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-recovery-queue.csv`; `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-summary.md`; `docs/research/market-verification/DEC-STUDIO-NAME-001-verification-recovery-plan.md`

## Conflict of Interest Declaration
Bu amendment'ın yazarı aynı amendment'ı nihai olarak onaylayamaz. Bilinen başka çıkar çatışması yoktur.

## Previous Stage References
REC-DEC-STUDIO-NAME-001-RESEARCH-EXECUTION-PLAN-001; committed MV-A–MV-H verification outputs and consolidation at `91891070eb7dbd962ec9267214f7543bfa0a9343`

## Next Stage
Independent Governance Audit; kabul edilirse kontrollü recovery ve Verified Analytical Sample kurulumu

## Review Target
`quality-assurance-director`; amendment yazarı olmayan `independent-red-team-auditor` veya bağımsız governance reviewer; `FOUNDER`

## Independence Declaration
Bu kayıt yöntem değişikliği önerir; araştırma yapmaz, kaynak doğrulamaz, kayıt statüsü değiştirmez, isim veya aday marka üretmez ve Brand Strategy başlatmaz. Yazar kendi değişikliğini onaylamaz.

## Karar ihtiyacı

Mevcut yöntem, 180 aktif cohort kaydının tamamının kayıt başına iki bağımsız sinyal ve en az bir birincil/resmî kaynakla canlı doğrulanmasını downstream analiz için fiilî önkoşul hâline getirmiştir. Konsolidasyon bu eşiğin 22/180 kayıtta karşılandığını, 158 aktif kaydın `STATUS_UNCERTAIN` kaldığını gösterir. Tam 180/180 recovery, keşifsel stüdyo ismi/pattern araştırmasının amacıyla orantısız maliyet üretirken doğrulanmamış kayıtların analizde kullanılmasını haklı kılmaz.

## Programatik olarak doğrulanan mevcut durum

2026-08-11 tarihinde consolidated results ve recovery queue CSV'leri yeniden sayılmıştır:

| Ölçüm | Sonuç |
|---|---:|
| Toplam Context Inventory | 200 |
| Aktif cohort | 180 |
| Tarihsel cohort | 20 |
| `VERIFIED_ACTIVE` | 22 |
| Aktif `STATUS_UNCERTAIN` | 158 |
| `VERIFIED_HISTORICAL_CASE` | 15 |
| Tarihsel `STATUS_UNCERTAIN` | 5 |
| Recovery queue | 163 |
| P1 | 44 |
| P2 | 45 |
| P3 | 74 |

Bu sayımlar rapordan aktarılmamış; `Verification Result`, `Record Type` ve `Recovery Priority` alanlarından yeniden hesaplanmıştır.

## Metodolojik karar

`180/180` bağımsız canlı doğrulama şartı, Market Context Inventory'nin korunması için değil yalnız Verified Analytical Sample oluşturulması bakımından **SUPERSEDED** edilmelidir. Tam sayım hedefi nüfus parametresi tahmini yapılmayan, keşifsel ve stratejik bir pattern araştırması için gerekli değildir. Ancak daha küçük sample, uygunluk veya kolaylık örneklemine dönüşemez; yeterlilik hem sayısal taban hem önceden ilan edilmiş temsil guardrail'leriyle belirlenir.

Önerilen aktif minimum **60 `VERIFIED_ACTIVE` kayıttır**. Bu sayı:

- 180 aktif kayıtlık bağlam evreninin üçte birini kapsayarak yalnız birkaç örnekten çıkarım riskini azaltır;
- altı ana aktif katmanda ortalama on kayıtlık inceleme alanı bırakır;
- nüfus tahmini, hata payı veya temsilî rastgele örnek iddiası oluşturmaz;
- mevcut 22 kayda ek olarak en az 38 doğrulanmış kayıt gerektirir ve recovery'yi anlamlı ölçüde sürdürür;
- ancak aşağıdaki stratification ve bölge guardrail'leri sağlanırsa yeterlidir.

`60` bir başarı garantisi veya gerekçesiz sabit kota değildir. Pattern istikrar kontrolü başarısızsa, yeni doğrulamalar maddi biçimde yeni pattern üretmeye devam ediyorsa ya da guardrail'ler sağlanamıyorsa sample 60'ın üzerine çıkarılır. Çok küçük alt gruplar için güçlü sonuç çıkarılmaz.

## İki veri katmanı

### 1. Market Context Inventory

Orijinal 200 benzersiz sayım birimi, sekiz birincil katman, ilişkiler, durum ve bias görünürlüğüyle korunur. Bu katman pazar kapsamını ve araştırma evrenini belgeler.

`STATUS_UNCERTAIN` kayıtlar:

- kesin istatistik veya pattern yüzdesi üretiminde kullanılmaz;
- downstream marka kararı için kanıt sayılmaz;
- yalnız kapsam, eksik temsil, recovery ve bias görünürlüğü sağlar;
- `VERIFIED` kayıtlarla birleştirilmiş bir paydaya konulamaz.

### 2. Verified Analytical Sample

Brand Strategy ve naming-pattern analizine yalnız cohort koşullarını geçen `VERIFIED_ACTIVE` kayıtlar girebilir. Sample üyeliği, mevcut verification sonucuna referans veren sürümlü bir inclusion manifest ile belirlenir; ana kayıt statüleri bu amendment tarafından değiştirilmez.

## Aktif stratification gate

60 kayıtlık minimum için bütün kurallar aynı anda sağlanır:

| Birincil katman | Mutlak taban | Gerekçe |
|---|---:|---|
| Büyük uluslararası yayıncı | 6 | Tekil örnek etkisini azaltan, fakat alt grup nüfus tahmini iddiası taşımayan ortak taban |
| Bağımsız stüdyo | 6 | En büyük özgün katmanın erişim kolaylığı nedeniyle bastırılmasını önler |
| Mobil | 6 | Platform pattern'inin görünür kalmasını sağlar |
| PC/konsol | 6 | Platform pattern'inin görünür kalmasını sağlar |
| Strateji/MMO | 6 | Tür-odaklı pattern'in görünür kalmasını sağlar |
| Teknoloji/yaratıcı marka | 6 | Yakın marka karşılaştırmasını aktif oyun şirketlerinden ayırır |

Kalan en az 24 yer, Context Inventory'deki özgün aktif katman ağırlıkları (30/45/25/25/25/30) başlangıç hedefi alınarak dağıtılır; erişilebilirlik uğruna tek bir katmana yığılmaz. Hiçbir aktif katman toplam Verified Analytical Sample'ın `%30`undan fazlasını oluşturamaz. Bu yüzde, en büyük özgün katmanın payı olan `%25` için sınırlı uygulama esnekliği tanıyan bir yoğunlaşma alarmıdır; hedef dağılım değildir. Bir katmanın tabanı sağlansa bile özgün ağırlıktan önemli sapma gerekçe, bias etkisi ve bağımsız reviewer kabulü olmadan geçemez.

Bir alt grup `n < 10` ise o alt gruba ait yüzde yalnız açıkça “küçük-n betimsel sinyal” olarak gösterilebilir; güçlü karşılaştırma, sıralama veya genelleme yapılamaz.

## Bölgesel guardrail

Sampling Framework'teki altı bölge grubunun tamamı — Kuzey Amerika; Avrupa; Türkiye ve yakın bölge; Doğu Asya; Güney ve Güneydoğu Asya; diğer bölgeler — Verified Analytical Sample içinde görünmelidir.

- Her bölge için mutlak taban `n >= 3` olur. Bu taban bölgenin yokluğunu ve tek kayıtla temsil edilmesini önler; bölgesel yüzde veya güçlü bölgesel sonuç üretmeye yetmez.
- Tek bir bölge sample'ın `%35`inden fazlasını oluşturamaz.
- En büyük iki bölgenin birleşik payı `%60`ı aşamaz.
- Bölgesel karşılaştırma ancak karşılaştırılan her bölge `n >= 10` ise yapılabilir; aksi hâlde yalnız eksik temsil ve betimsel gözlem raporlanır.

Bu eşikler, özgün `%40` context alarmından daha sıkıdır; daha küçük analitik sample'da yoğunlaşmanın büyüyen etkisini sınırlar. Kaynak kıtlığı guardrail'i sessizce düşürme gerekçesi değildir; recovery devam eder veya bağımsız audit'e escalation yapılır.

## Tarihsel sample kuralı

Tarihsel analiz yalnız `VERIFIED_HISTORICAL_CASE` kayıtları kullanır. `STATUS_UNCERTAIN` olan beş vaka dışarıda kalır. Ayrı tarihsel yeterlilik tabanı **12 doğrulanmış vaka**, iki özgün tarihsel katmanın her birinde en az **5 vaka** olarak belirlenir. Bu taban iki vaka türünün de birkaç tekil olaya indirgenmemesini sağlar; tarihsel evren veya görülme sıklığı tahmini değildir.

Mevcut 15 doğrulanmış vaka (MV-G 7; MV-H 8) bu sayısal ve katman tabanını karşılar. Yine de yalnız nitel/keşifsel vaka pattern'leri için kullanılabilir; nedensellik, piyasa prevalansı veya güçlü alt grup yüzdesi üretilemez. Aktif kuruluş kaydı ile ilişkili tarihsel olay `Sample ID`/`Event ID` ve tek `Counted Sample ID` kuralıyla ayrı tutulur.

## Analiz ve raporlama kuralları

1. Pattern hesaplarının pay ve paydasında yalnız dondurulmuş `VERIFIED_ANALYTICAL_SAMPLE` sürümü kullanılabilir.
2. Context Inventory sayıları ile Verified Analytical Sample yüzdeleri aynı metrikte veya belirsiz bir paydada karıştırılamaz.
3. Her tablo başlık veya dipnotunda sample sürümü, `n`, cohort, dahil etme tarihi ve kullanılan payda gösterilir.
4. Confidence ve limitations her bulguda görünür olur; küçük-n ve stratification sapmaları özellikle belirtilir.
5. Amaçlı/katmanlı sample'dan nedensellik, nüfus prevalansı, hata payı veya istatistiksel temsil iddiası çıkarılamaz.
6. `STATUS_UNCERTAIN` kayıtlar bulguyu desteklemek, örnek sayısını büyütmek veya karşı kanıtı bastırmak için kullanılamaz.
7. Aynı `Counted Sample ID` aynı tabloda birden fazla kez sayılamaz. Aktif ve tarihsel analizler ayrı tutulur.
8. Pattern sonuçları stratum bazında duyarlılık kontrolünden geçirilir: en büyük stratum çıkarıldığında ana sonucun yönü değişiyorsa sonuç “yoğunlaşmaya duyarlı” olarak işaretlenir.

## Recovery stop rule

Brand Strategy gate ancak bağımsız audit bu amendment'ı kabul ettikten ve aşağıdakilerin tümü sürümlü kanıtla gösterildikten sonra açılabilir:

- en az 60 `VERIFIED_ACTIVE`;
- altı aktif stratum tabanı ve yoğunlaşma kontrolleri;
- altı bölgenin görünürlüğü ve bölgesel yoğunlaşma kontrolleri;
- her dahil aktif kayıt için iki bağımsız qualifying source, en az bir primary/official source, reviewer ve doğrulama tarihi;
- historical analiz yapılacaksa en az 12 doğrulanmış vaka ve iki tarihsel stratumda en az beşer kayıt;
- inclusion manifest, dedup kontrolü, bias register ve yeniden üretilebilir sayım çıktısı;
- 60 kayıt eşiğinde pattern istikrar/doygunluk kontrolünün yeni eklemelerle maddi yeni ana pattern üretmediğine dair kaydı.

Bu koşullar sağlanırsa kalan P2/P3 kayıtlarının tamamının recovery'si downstream gate için zorunlu değildir; unresolved kayıtlar Context Inventory'de `STATUS_UNCERTAIN` kalabilir. P1 öncelikli havuzdur ama P1 etiketi analitik dahil edilme garantisi değildir.

Zorunlu bir stratum tabanı eksikse, bölgesel temsil/yoğunlaşma kuralı başarısızsa, primary-source veya bağımsızlık koşulu eksikse, dedup belirsizse ya da istikrar kontrolü başarısızsa recovery devam eder. Gerekli hücre için P2/P3 kayda geçilebilir; global sıra, temsili tamamlamayı engellememelidir ve sapma kaydedilir.

## Governance değerlendirmesi

### Founder Research Mandate

Uyumludur: 200 kayıtlık, 150 oyun stüdyosu/yayıncı ve 50 yakın marka bağlam evreni ile sekiz grup korunur; mandate örneklemin tam piyasa sayımı olmadığını zaten belirtir. Amendment isim üretmez ve kurucuya ayrılmış marka kararını açmaz. Yöntemin kabulü bağımsız audit ve gerektiğinde açık `FOUNDER` kararı gerektirir.

### Research Policy ve reproducibility

Uyumludur: analize yalnız kaynak ve tarih eşiğini geçen kayıtları alır. Inclusion manifest, sürüm, açık payda, dedup ve guardrail sayımları reproducibility'yi güçlendirir. Kalan risk, doğrulanabilirliği yüksek kuruluşların sample'a seçilmesiyle oluşan verification/access bias'tır; bu risk silinmez, bias register'da görünür tutulur.

### Bias ve double-count

Doğrulama kolaylığı; İngilizce kaynak, büyük/tanınmış marka, bölge, survivorship ve platform bias'ı üretebilir. Stratification/bölge tabanları ve yoğunlaşma kontrolleri bu riskleri azaltır fakat ortadan kaldırmaz. `Counted Sample ID`, ayrı active/historical paydalar ve inclusion manifest double-count riskinin zorunlu kontrolleridir.

### Downstream Brand Strategy

Gate daha erken fakat daha dar bir kanıt tabanıyla açılabilir. Brand Strategy yalnız verified sample'ı yorumlar; Context Inventory'yi nicel kanıt gibi kullanamaz. Sonuçlar stratejik/keşifsel sinyal olarak kalır ve karar protokolündeki proposal, critique, red-team, stop-gate ve kurucu yetkilerinin hiçbirini atlamaz.

## Açık amendment / supersession haritası

| Mevcut hüküm | Durum | Yeni yorum |
|---|---|---|
| Sampling Framework: 200 hedefin tamamı bağlayıcı; eksik kota tamamlanmış sample değildir | **AMENDED** | 200 Context Inventory için bağlayıcıdır; analitik sample, bu amendment'ın verified minimum ve guardrail'leriyle ayrı değerlendirilir |
| Sampling Framework: `STATUS_UNCERTAIN` aktif kotaya giremez | **PRESERVED** | Verified Analytical Sample'a giremez; Context Inventory'de görünür kalır |
| Sampling Framework: çözülmemiş kota proposal'a geçemez | **SUPERSEDED** | Context Inventory eksiksiz ve verified sample gate'i geçiyorsa bütün 180 kaydın doğrulanması proposal/Brand Strategy için zorunlu değildir |
| Research Execution Plan: “200 ana kayıt tamamlanır” ve her dahil kaydın tam aktiflik kanıtı | **AMENDED** | 200 envanter korunur; analitik kullanım yalnız verified inclusion manifest'teki kayıtlara aittir |
| Research Execution Plan: resmî kaynak çelişkiliyse ilgili teslim hazır değildir | **AMENDED** | İlgili kayıt analitik sample için hazır değildir; bu tek başına bütün Context Inventory'yi bloke etmez, zorunlu temsil hücresini etkiliyorsa gate'i bloke eder |
| Recovery Plan: herhangi unresolved kayıt sonraki recovery queue'da kalır | **AMENDED** | Kayıt Context Inventory'de uncertain kalabilir; sample gate sağlandıktan sonra tüm P2/P3'ün kapanması zorunlu değildir |
| İki bağımsız sinyal ve en az bir primary/official source | **PRESERVED** | Verified Analytical Sample üyeliğinin kayıt başına zorunlu eşiğidir |
| Tek birincil sayım, ayrı Event ID ve `Counted Sample ID` | **PRESERVED** | Bütün context ve analytical tablolarda geçerlidir |

Eski dosyalar sessizce yeniden yazılmamıştır. Bu kayıt bağımsız audit ile kabul edilene kadar mevcut yöntem yürürlüktedir.

## Status

**PROPOSED — REQUIRES INDEPENDENT GOVERNANCE AUDIT**
