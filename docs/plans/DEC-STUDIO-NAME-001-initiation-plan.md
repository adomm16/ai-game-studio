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
GitHub Issue #4 (`DEC-STUDIO-NAME-001 — Studio name research and selection`); REC-DEC-STUDIO-NAME-001-BRIEF-001; REC-DEC-STUDIO-NAME-001-RESEARCH-REQUEST-001; `docs/plans/PLANS.md`; `docs/studio/decision-protocol.md`

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
1. Kurul, brief kapsamını ve proposal öncesi 100 toplamlı ağırlıkları inceler ve açıkça kilitler.
2. Research Memo, dil kontrol girdileri, ön Legal/IP yöntemi ve değişebilir uygunluk görüntüleri tamamlanır.
3. Üç bağımsız proposal turu, critique ve rebuttal zinciri tamamlanır.
4. Scorecard yalnız yeterli veri ve kilitli ağırlıklarla oluşturulur; dört stop-gate ayrı kaydedilir.
5. Bağımsız red-team ve yönetim tavsiyesi hazırlanır; founder review yapılır.
6. Kurucu kabul/revizyon/ret kararı sonrası Final Decision Record yazılır.

## Görev matrisi

| Görev | Sorumlu agent | Girdiler | Çıktılar | Bağımlılıklar | Kabul kriterleri | Handoff hedefi |
|---|---|---|---|---|---|---|
| Pazar ve rakip araştırması | `market-research-director` | Brief; Research Request; kurulun pazar/dönem kararı | Research Memo, veri sözlüğü, kaynak sicili girdileri, karşı kanıt ve güven düzeyleri | Kapsam ve kaynak bütçesi kararı | Birincil/resmî kaynak önceliği; tarih/sorgu ayrıntıları; örneklem ve sınırlamalar; değişebilir sonuç etiketi; uydurma sonuç yok | `brand-marketing-director`, `community-localization-director`, `legal-ip-risk-advisor` |
| Marka stratejisi kriterleri | `brand-marketing-director` | Brief; Research Memo; kurucu hedefi | Konumlandırma ölçütleri, eleme kriterleri, mesaj kanıt matrisi taslağı | Pazar araştırması | Ölçütler isimden bağımsız ve ölçülebilir; AI vurgusu dahil karşı görüşler; marka seçimi yok | Üç proposal sahibi ve `studio-orchestrator` |
| Dil ve uluslararası anlam kontrolü | `community-localization-director` | Dil öncelikleri; Research Memo; daha sonra proposal adayları | Telaffuz/yazım protokolü, dil-risk matrisi, doğrulanmamış alanlar | Kurulun hedef dil/pazar kararı; adaylar için proposal aşaması | Yetkili dil kaynakları ve mümkünse bağımsız konuşur kontrolü; olumsuz anlam riski/güven düzeyi; eksik dilde stop/escalation | Proposal sahipleri; `legal-ip-risk-advisor`; `accessibility-ethics-reviewer` |
| Legal/IP ön risk yöntemi | `legal-ip-risk-advisor` | Brief; hedef yargı alanı/Nice sınıfları; resmî veri kaynakları | Ön tarama protokolü, kaynak otoritesi, risk sınıfları ve Legal/IP stop-gate yöntemi | Kurucunun pazar/şirket planı; araştırma veri paketi | Kesin hukuk hükmü yok; resmî kayıt ve sorgu kapsamı; benzerlik/karışıklık varsayımları; `PASS/FAIL/NOT REVIEWED`; doğrudan `FOUNDER` raporu | `FOUNDER`; lojistik kopya `studio-orchestrator` |
| Alan adı ve sosyal kullanıcı adı yöntemi | `market-research-director` | Öncelikli TLD/platform listesi; Research Request | Zaman damgalı sorgu protokolü ve değişebilirlik etiketi | Kurul TLD/platform kararı | Doğrudan registry/platform kontrolleri; bölge/dil/sorgu kaydı; satın alma yok; uygunluk garantisi yok | Proposal sahipleri; `brand-marketing-director` |
| Bağımsız Proposal Turu 1 | `brand-marketing-director` | Kilitli Brief; Research Memo; dil ve yöntem girdileri | Proposal Record 1: aday yaklaşım, gerekçe, alternatif, maliyet/risk/ölçüm | Araştırma kabulü ve ağırlık kilidi | Diğer proposal’ları görmeden üretim; kaynaklı iddialar; isim seçimi yok; benzersiz Record ID | Atanmış bağımsız critique yazarı |
| Bağımsız Proposal Turu 2 | `creative-director` | Kilitli Brief; Research Memo; marka/dil kriterleri | Proposal Record 2 | Araştırma kabulü ve ağırlık kilidi | Tur 1’den bağımsız; görsel üretim yok; kaynak, risk ve geri alma; benzersiz Record ID | Atanmış bağımsız critique yazarı |
| Bağımsız Proposal Turu 3 | `community-localization-director` | Kilitli Brief; Research Memo; marka/dil kriterleri | Proposal Record 3 | Araştırma kabulü ve ağırlık kilidi | Önceki proposal’lardan bağımsız; dil/kültür riskleri; kaynak ve belirsizlik; benzersiz Record ID | Atanmış bağımsız critique yazarı |
| Critique | `finance-monetization-director`, `market-research-director`, `legal-ip-risk-advisor` ayrı atamalarla | İlgili proposal; Brief; Research Memo | Her proposal için ayrı Critique Record; güçlü yön, karşı kanıt, açık risk ve sorular | Üç proposal tamamlanmış | Hiçbir yazar hedef proposal’ın sahibi değil; her proposal en az bir critique alır; uzmanlık dışı kesin hüküm yok | İlgili proposal sahibi |
| Rebuttal | Her proposal’ın özgün sahibi | Kendi proposal’ı ve ona bağlı critique | Üç ayrı Rebuttal Record | İlgili critique | Yazar proposal sahibiyle aynı; kabul edilen eleştiriler, kanıtlı yanıt ve kalan belirsizlik; yeni kanıtsız iddia yok | `quality-assurance-director` ve scorecard süreci |
| Scorecard | `quality-assurance-director` | Kilitli ağırlıklar; bütün proposal/critique/rebuttal kayıtları; araştırma kanıtı | Boyut bazlı 1–5 puan, gerekçe, güven ve hesap doğrulaması veya `EKSİK VERİ` | Rebuttal zinciri; veri yeterliliği | Ağırlıklar Brief’teki kilitli sürümle aynı; toplam 100; eksik veride puan/toplam yok; stop-gate’ler toplamdan ayrı | Güvence rolleri ve `independent-red-team-auditor` |
| Legal/IP stop-gate | `legal-ip-risk-advisor` | Proposal’lar; resmî ön tarama kanıtı; scorecard girdileri | Bağımsız `PASS/FAIL/NOT REVIEWED` ve risk notu | Legal yöntem ve aday kayıtları | `FAIL/NOT REVIEWED` ilerlemeyi durdurur; toplam puanla maskelenmez; doğrudan `FOUNDER` | `FOUNDER`; red-team’e kanıt paketi |
| Security stop-gate | `security-privacy-lead` | Kayıt zinciri, veri/hesap/servis planı | Bağımsız Security sonucu | Proposal ve araştırma yöntemi | En az ayrıcalık/secret/veri riski incelenir; `FAIL/NOT REVIEWED` durdurur | `FOUNDER`; red-team |
| Privacy stop-gate | `security-privacy-lead` | Araştırma/algı testi veri planı | Bağımsız Privacy sonucu | Örneklem ve veri toplama yöntemi | Veri minimizasyonu, kişisel veri ve onay gereksinimi; `FAIL/NOT REVIEWED` durdurur | `FOUNDER`; red-team |
| Ethics stop-gate | `accessibility-ethics-reviewer` | Dil/kültür matrisi; algı araştırması; proposal’lar | Bağımsız Ethics sonucu | Dil kontrolü ve proposal’lar | Zarar, dışlayıcılık, yanıltma ve temsil riski; `FAIL/NOT REVIEWED` durdurur | `FOUNDER`; red-team |
| Red-Team | `independent-red-team-auditor` | Bütün proposal’lar; scorecard; stop-gate kayıtları; karşı kanıt | Red-Team Report, başarısızlık/kötüye kullanım senaryoları, açık riskler | Scorecard ve dört stop-gate tamamlanmış | Hiçbir proposal yazarlığı yok; bulgular orchestrator tarafından değiştirilemez; kapsam atlatma ve yanlış güven senaryoları | Doğrudan `FOUNDER`; lojistik kopya `studio-orchestrator` |
| Yönetim kurulu tavsiyesi | `studio-orchestrator` (tarafsız sentez) | Tüm zincir ve bağımsız bulgular | Alternatifleri/karşı görüşleri koruyan tavsiye paketi | Red-Team; stop-gate’lerin tümü `PASS` | Bağımsız bulgular değiştirilmez; eksikler görünür; nihai isim seçimi yapılmaz | `FOUNDER` |
| Founder review | `FOUNDER` (karar sahibi; agent değildir), koordinasyon `studio-orchestrator` | Tam karar paketi, açık riskler, stop-gate’ler | Açık kabul/revizyon/ret ve tarih; ardından Final Decision Record girdisi | Eksiksiz zincir; tüm stop-gate’ler `PASS` | Sessizlik/çoğunluk onay değildir; korunmuş karar yazılıdır; satın alma/tescil ayrı açık onay gerektirir | `studio-orchestrator` Final Decision Record; gerekirse araştırma/revizyon sahibi |

## Bağımlılık ve stop koşulları
- Kurul incelemesi ve ağırlık kilidi olmadan proposal başlamaz.
- Research Memo kabul edilmeden proposal başlamaz.
- Hedef pazar, dil, TLD, platform ve yargı alanı belirsizliği ilgili araştırmayı tamamlanmış saydırmaz.
- Kritik veri eksikliği puanlama/toplamı engeller ve araştırmaya geri döndürür.
- Herhangi bir stop-gate `FAIL` veya `NOT REVIEWED` ise red-team kanıt incelemesi yapılabilir ancak founder approval tavsiyesi ilerlemez.
- Çıkar çatışması veya bağımsızlık ihlali ilgili kaydı geçersiz kılar ve yeniden atama gerektirir.

## Doğrulama
- Her karar kaydında manifestte zorunlu metadata alanları dolu ve Record ID benzersiz olmalıdır.
- Proposal sayısı en az üç ve yazarları farklı olmalıdır; critique/rebuttal eşleşmeleri validator kurallarını karşılamalıdır.
- `python scripts/validate_studio.py`, `python -m unittest discover -s tests -v` ve `git diff --check` her teslimde geçmelidir.
- Kaynak ve değişebilirlik etiketleri insan incelemesine sunulmalıdır; validator semantik doğruluğun yerine geçmez.

## Karar noktaları ve açık sorular
1. Öncelikli hedef ülkeler, diller ve yargı alanları hangileridir?
2. Alan adı için hangi TLD’ler ve sosyal kullanıcı adı için hangi platformlar zorunludur?
3. Kısa/uzun ad veya AI vurgusu konusunda araştırmaya yön verecek kurucu kısıtı var mıdır?
4. Ücretli veri tabanı veya dış hukuk danışmanı bütçesi var mıdır? Açık onay olmadan kullanılmayacaktır.
5. Başlangıç ağırlıkları kurulca aynen mi kilitlenecek, yoksa proposal görülmeden önce revize mi edilecektir?
6. Araştırma ve founder review için hedef tarihler nedir?

## Güncel durum
- 2026-08-08: Issue #4 karar talebi alındı; Brief, Research Request ve çalışma planı taslakları oluşturuldu.
- 2026-08-08: Araştırma, aday/proposal üretimi, stop-gate incelemesi, puanlama ve founder review henüz yapılmadı.

## Değişiklik günlüğü
- 2026-08-08: `REC-DEC-STUDIO-NAME-001-PLAN-001` ile ilk plan oluşturuldu.
