# DEC-STUDIO-NAME-001 Karar Süreci Çalışma Planı

## Decision ID
DEC-STUDIO-NAME-001

## Record ID
REC-DEC-STUDIO-NAME-001-PLAN-001

## Author Agent ID
studio-orchestrator

## Created Date
2026-08-08

## Source References
GitHub Issue #4 (`DEC-STUDIO-NAME-001 — Studio name research and selection`); REC-DEC-STUDIO-NAME-001-BRIEF-001; REC-DEC-STUDIO-NAME-001-RESEARCH-REQUEST-001; `docs/plans/PLANS.md`; `docs/studio/decision-protocol.md`; `docs/plans/DEC-STUDIO-NAME-001-founder-research-mandate.md`; `docs/audits/governance/DEC-STUDIO-NAME-001-initiation-audit.md`

## Conflict of Interest Declaration
Bilinen çıkar çatışması yoktur. Studio Orchestrator yalnız lojistik ve kayıt akışını koordine eder; proposal, bağımsız güvence bulgusu veya founder decision içeriğini onaylamaz.

## Previous Stage References
REC-DEC-STUDIO-NAME-001-BRIEF-001; REC-DEC-STUDIO-NAME-001-RESEARCH-REQUEST-001

## Next Stage
Kurul kapsam incelemesi; proposal öncesi ağırlık kilidi; Research Memo

## Review Target
FOUNDER ve zorunlu uzman roller

## Independence Declaration
Plan görev ayrılığını korur: proposal sahipleri birbirinden bağımsızdır; critique yazarı hedef proposal’ın sahibi değildir; red-team hiçbir proposal’ın sahibi değildir; güvence rolleri doğrudan `FOUNDER` hattına raporlar.

## Amaç
DEC-STUDIO-NAME-001 kararını araştırmadan founder review’a kadar izlenebilir, fail-closed ve rol sınırlarına uygun biçimde yürütmek. Bu plan isim üretmez veya seçmez.

## Kapsam dışı
Nihai veya aday isim; alan adı/kullanıcı adı edinimi; marka ya da şirket tescili; logo/renk; ücretli servis; hukuki uygunluk garantisi; public release.

## Kilometre taşları
1. Bağlayıcı kurucu araştırma girdileri veri sözlüğüne aktarılır; katmanlı örneklem ve 2021-01-01–2026-08-08 analiz dönemi uygulanır.
2. Research Memo, dil kontrol girdileri, ayrı Legal/IP ön inceleme yöntemleri ve değişebilir uygunluk görüntüleri 2026-08-15 planlama hedefine göre tamamlanır.
3. Kurul, on alt ölçütün beş protokol boyutuna tekil eşlemesini ve toplam 100 ağırlıkları proposal görmeden önce inceler; 2026-08-16 hedefiyle açıkça kilitler.
4. Üç bağımsız proposal turu 2026-08-20 planlama hedefine göre yürütülür; critique ve rebuttal zinciri tamamlanır.
5. Scorecard yalnız yeterli veri ve kilitli ağırlıklarla oluşturulur; kanıt tahsis sicili ile resmî `Legal`, `Security`, `Privacy`, `IP` stop-gate'leri ayrı kaydedilir.
6. Ethics/Accessibility advisory review, bağımsız red-team ve yönetim tavsiyesi hazırlanır; 2026-08-23 hedefiyle founder kısa liste incelemesi yapılır.
7. Kurucu kabul/revizyon/ret kararı sonrası Final Decision Record yazılır.

## Görev matrisi

| Görev | Sorumlu agent | Girdiler | Çıktılar | Bağımlılıklar | Kabul kriterleri | Handoff hedefi |
|---|---|---|---|---|---|---|
| Pazar ve rakip araştırması | `market-research-director` | Brief; Research Request; Founder Research Mandate | Research Memo, veri sözlüğü, kaynak sicili girdileri, karşı kanıt ve güven düzeyleri | En az 150 oyun stüdyosu/yayıncı + 50 yakın marka; tanımlı katmanlar; 2021-01-01–2026-08-08 dönem; ücretsiz ilk tur bütçesi | Birincil/resmî kaynak önceliği; dahil etme/dışlama ve katman sayıları; tarih/sorgu ayrıntıları; tam piyasa sayımı iddiası yok; değişebilir sonuç etiketi; uydurma sonuç yok | `brand-marketing-director`, `community-localization-director`, `legal-ip-risk-advisor` |
| Marka stratejisi kriterleri | `brand-marketing-director` | Brief; Research Memo; kurucu hedefi | Konumlandırma ölçütleri, eleme kriterleri, mesaj kanıt matrisi taslağı | Pazar araştırması | Ölçütler isimden bağımsız ve ölçülebilir; AI vurgusu dahil karşı görüşler; marka seçimi yok | Üç proposal sahibi ve `studio-orchestrator` |
| Dil ve uluslararası anlam kontrolü | `community-localization-director` | Research Memo; ilk tur Türkçe/İngilizce/Almanca/Fransızca/İspanyolca/Portekizce; finalist Japonca/Korece | Telaffuz, sözlük anlamı, argo, çağrışım ve yanlış okunma risk matrisi; doğrulanmamış alanlar | Founder Research Mandate; adaylar için proposal aşaması | Yetkili dil kaynakları ve mümkünse bağımsız konuşur kontrolü; dil başına kapsam/güven; eksik dilde stop/escalation | Proposal sahipleri; `legal-ip-risk-advisor`; `accessibility-ethics-reviewer` |
| Legal ve IP ön risk yöntemleri | `legal-ip-risk-advisor` | Brief; Türkiye/ABD/BK/Kanada/AB kapsamı; Nice sınıfları; TÜRKPATENT, EUIPO, WIPO, USPTO, UKIPO ve belirlenecek Kanada resmî kaynağı | Ayrı Legal ve IP ön tarama protokolleri, kaynak otoritesi, risk sınıfları ve ayrı stop-gate yöntemleri | Founder Research Mandate; araştırma veri paketi | Kesin hukuk hükmü yok; resmî kayıt/sorgu kapsamı; ayrı `PASS/FAIL/NOT REVIEWED`; her iki gate doğrudan `FOUNDER` raporu | `FOUNDER`; lojistik kopya `studio-orchestrator` |
| Alan adı ve sosyal kullanıcı adı yöntemi | `market-research-director` | `.com`, `.studio`, `.games`; Steam, Epic Games Store, YouTube, Instagram, TikTok, X, Discord, Twitch, LinkedIn | Zaman damgalı sorgu protokolü ve değişebilirlik etiketi | Founder Research Mandate; adaylar için proposal aşaması | Doğrudan registry/platform kontrolleri; bölge/dil/sorgu kaydı; satın alma/hesap açma yok; uygunluk garantisi yok | Proposal sahipleri; `brand-marketing-director` |
| Bağımsız Proposal Turu 1 | `brand-marketing-director` | Kilitli Brief; Research Memo; dil ve yöntem girdileri | Proposal Record 1: aday yaklaşım, gerekçe, alternatif, maliyet/risk/ölçüm | Araştırma kabulü ve ağırlık kilidi | Diğer proposal’ları görmeden üretim; kaynaklı iddialar; isim seçimi yok; benzersiz Record ID | Atanmış bağımsız critique yazarı |
| Bağımsız Proposal Turu 2 | `creative-director` | Kilitli Brief; Research Memo; marka/dil kriterleri | Proposal Record 2 | Araştırma kabulü ve ağırlık kilidi | Tur 1’den bağımsız; görsel üretim yok; kaynak, risk ve geri alma; benzersiz Record ID | Atanmış bağımsız critique yazarı |
| Bağımsız Proposal Turu 3 | `community-localization-director` | Kilitli Brief; Research Memo; marka/dil kriterleri | Proposal Record 3 | Araştırma kabulü ve ağırlık kilidi | Önceki proposal’lardan bağımsız; dil/kültür riskleri; kaynak ve belirsizlik; benzersiz Record ID | Atanmış bağımsız critique yazarı |
| Critique | `finance-monetization-director`, `market-research-director`, `legal-ip-risk-advisor` ayrı atamalarla | İlgili proposal; Brief; Research Memo | Her proposal için ayrı Critique Record; güçlü yön, karşı kanıt, açık risk ve sorular | Üç proposal tamamlanmış | Hiçbir yazar hedef proposal’ın sahibi değil; her proposal en az bir critique alır; uzmanlık dışı kesin hüküm yok | İlgili proposal sahibi |
| Rebuttal | Her proposal’ın özgün sahibi | Kendi proposal’ı ve ona bağlı critique | Üç ayrı Rebuttal Record | İlgili critique | Yazar proposal sahibiyle aynı; kabul edilen eleştiriler, kanıtlı yanıt ve kalan belirsizlik; yeni kanıtsız iddia yok | `quality-assurance-director` ve scorecard süreci |
| Scorecard | `quality-assurance-director` | Kilitli Market 38, Creative 22, Finance 18, Technical 10, Legal 12; bütün proposal/critique/rebuttal kayıtları; araştırma kanıtı | Beş protokol boyutunda 1–5 puan, alt ölçüt hesabı, kanıt tahsis sicili, korelasyon/güven ve hesap doğrulaması veya `EKSİK VERİ` | Rebuttal zinciri; veri yeterliliği; ağırlık kilidi | On alt ölçüt tek birincil boyuta bağlı; toplam 100; aynı kanıt otomatik çift sayılmaz; yüksek korelasyonda güven düşer; şüphede bağımsız QA incelemesi ve toplam yok; stop-gate’ler ayrı | Güvence rolleri ve `independent-red-team-auditor` |
| Legal stop-gate | `legal-ip-risk-advisor` | Proposal’lar; resmî hukuk ön tarama kanıtı | Bağımsız Legal `PASS/FAIL/NOT REVIEWED` ve risk notu | Legal yöntem ve aday kayıtları | `FAIL/NOT REVIEWED` ilerlemeyi ve `APPROVED` sonucunu durdurur; toplam puanla maskelenmez; doğrudan `FOUNDER` | `FOUNDER`; red-team’e kanıt paketi |
| IP stop-gate | `legal-ip-risk-advisor` | Proposal’lar; marka/IP ön tarama kanıtı | Bağımsız IP `PASS/FAIL/NOT REVIEWED` ve risk notu | IP yöntemi ve aday kayıtları | `FAIL/NOT REVIEWED` ilerlemeyi ve `APPROVED` sonucunu durdurur; toplam puanla maskelenmez; doğrudan `FOUNDER` | `FOUNDER`; red-team’e kanıt paketi |
| Security stop-gate | `security-privacy-lead` | Kayıt zinciri, veri/hesap/servis planı | Bağımsız Security sonucu | Proposal ve araştırma yöntemi | En az ayrıcalık/secret/veri riski incelenir; `FAIL/NOT REVIEWED` durdurur | `FOUNDER`; red-team |
| Privacy stop-gate | `security-privacy-lead` | Araştırma/algı testi veri planı | Bağımsız Privacy sonucu | Örneklem ve veri toplama yöntemi | Veri minimizasyonu, kişisel veri ve onay gereksinimi; `FAIL/NOT REVIEWED` durdurur | `FOUNDER`; red-team |
| Ethics/Accessibility advisory review | `accessibility-ethics-reviewer` | Dil/kültür matrisi; algı araştırması; proposal’lar | Bağımsız advisory bulgu ve açık riskler | Dil kontrolü ve proposal’lar | Zarar, dışlayıcılık, yanıltma ve temsil riski görünür; resmî blocking stop-gate gibi sunulmaz; doğrudan `FOUNDER` | `FOUNDER`; red-team |
| Red-Team | `independent-red-team-auditor` | Bütün proposal’lar; scorecard; dört resmî stop-gate; Ethics/Accessibility advisory review; karşı kanıt | Red-Team Report, başarısızlık/kötüye kullanım senaryoları, açık riskler | Scorecard, Legal/Security/Privacy/IP sonuçları ve advisory review tamamlanmış | Hiçbir proposal yazarlığı yok; bulgular orchestrator tarafından değiştirilemez; kapsam atlatma ve yanlış güven senaryoları | Doğrudan `FOUNDER`; lojistik kopya `studio-orchestrator` |
| Yönetim kurulu tavsiyesi | `studio-orchestrator` (tarafsız sentez) | Tüm zincir ve bağımsız bulgular | Alternatifleri/karşı görüşleri koruyan tavsiye paketi | Red-Team; stop-gate’lerin tümü `PASS` | Bağımsız bulgular değiştirilmez; eksikler görünür; nihai isim seçimi yapılmaz | `FOUNDER` |
| Founder review | `FOUNDER` (karar sahibi; agent değildir), koordinasyon `studio-orchestrator` | Tam karar paketi, açık riskler, stop-gate’ler | Açık kabul/revizyon/ret ve tarih; ardından Final Decision Record girdisi | Eksiksiz zincir; tüm stop-gate’ler `PASS` | Sessizlik/çoğunluk onay değildir; korunmuş karar yazılıdır; satın alma/tescil ayrı açık onay gerektirir | `studio-orchestrator` Final Decision Record; gerekirse araştırma/revizyon sahibi |

## Bağımlılık ve stop koşulları
- Kurul incelemesi ve ağırlık kilidi olmadan proposal başlamaz.
- Research Memo kabul edilmeden proposal başlamaz.
- Founder Research Mandate'teki örneklem, dönem, pazar, dil, TLD, platform, yargı alanı ve bütçe kapsamından doğrulanmamış sapma ilgili araştırmayı tamamlanmış saydırmaz.
- Kritik veri eksikliği puanlama/toplamı engeller ve araştırmaya geri döndürür.
- Legal, Security, Privacy veya IP stop-gate'lerinden herhangi biri `FAIL` veya `NOT REVIEWED` ise red-team kanıt incelemesi yapılabilir ancak `APPROVED` sonucu veya founder approval tavsiyesi ilerlemez.
- Aynı kanıt iki puanı otomatik olarak etkileyemez; birden fazla etki ayrı gerekçe ister. Yüksek korelasyonda güven düşürülür, çift sayım şüphesinde QA incelemesi tamamlanmadan toplam üretilmez.
- Çıkar çatışması veya bağımsızlık ihlali ilgili kaydı geçersiz kılar ve yeniden atama gerektirir.

## Doğrulama
- Her karar kaydında manifestte zorunlu metadata alanları dolu ve Record ID benzersiz olmalıdır.
- Proposal sayısı en az üç ve yazarları farklı olmalıdır; critique/rebuttal eşleşmeleri validator kurallarını karşılamalıdır.
- `python scripts/validate_studio.py`, `python -m unittest discover -s tests -v` ve `git diff --check` her teslimde geçmelidir.
- Kaynak ve değişebilirlik etiketleri insan incelemesine sunulmalıdır; validator semantik doğruluğun yerine geçmez.

## Kurucu tarafından çözülen araştırma girdileri

- Marka hedefi, pazarlar/yargı alanları, birinci tur ve finalist dilleri, TLD/platform kapsamı, isim kısıtları, katmanlı örneklem, veri dönemi, bütçe ve hedef tarihler `docs/plans/DEC-STUDIO-NAME-001-founder-research-mandate.md` içinde bağlayıcı olarak kaydedilmiştir.
- İlk tur ücretli kaynak veya hukuk danışmanı kullanmaz. Finalist 3–5 aday için olası ücretli inceleme ayrı kurucu onayı gerektirir.
- Başlangıç ağırlıkları beş protokol boyutuna eşlenmiştir fakat henüz kilitli değildir; kurul proposal görmeden önce aynen kilitleme veya revizyon kararını açıkça kaydetmelidir.

## Açık uygulama soruları

1. `legal-ip-risk-advisor`, belirtilen yargı alanları için ilgili Nice sınıflarını ve Kanada resmî ön tarama kaynağını yöntem kaydında belirlemelidir.
2. Market Research Director, bağlayıcı asgari örneklem içinde katman başına sayıları ve aktiflik/dahil etme-dışlama ölçütlerini veri sözlüğünde operasyonelleştirmelidir.
3. Kurul, 2026-08-16 hedefinden önce ağırlıkların kilitli sürümünü ve scorecard hesap yöntemini açıkça onaylamalıdır.

## Güncel durum
- 2026-08-08: Issue #4 karar talebi alındı; Brief, Research Request ve çalışma planı taslakları oluşturuldu.
- 2026-08-08: Araştırma, aday/proposal üretimi, stop-gate incelemesi, puanlama ve founder review henüz yapılmadı.
- 2026-08-08: Kurucu araştırma girdileri kaydedildi; audit bulguları için puanlama eşlemesi, resmî stop-gate taksonomisi ve çift sayım kontrolleri plana işlendi. Bu kayıt araştırmanın yapıldığı anlamına gelmez.

## Değişiklik günlüğü
- 2026-08-08: `REC-DEC-STUDIO-NAME-001-PLAN-001` ile ilk plan oluşturuldu.
- 2026-08-08: Audit remediation ile Founder Research Mandate, beş boyutlu puanlama eşlemesi, kanıt tahsis rubric'i ve resmî gate ayrımı eklendi; Record ID değiştirilmedi.
