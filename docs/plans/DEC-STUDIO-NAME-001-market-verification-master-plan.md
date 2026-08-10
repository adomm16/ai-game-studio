# DEC-STUDIO-NAME-001 Market Verification Master Plan

## Amaç

Market Landscape paketindeki 200 sayım birimini canlı ve bağımsız kaynak doğrulaması için sekiz çakışmasız, yeniden üretilebilir çalışma paketine ayırmak. Bu plan kaynak doğrulaması yapmaz ve mevcut araştırma statülerini değiştirmez.

## Kapsam ve kapsam dışı

Kapsam; mevcut ana register'ın deterministik bölümlenmesi, reviewer kabul ölçütleri, parti CSV'leri, otomatik partition doğrulaması ve testlerdir. Canlı kaynak erişimi, `Review Status` veya `STATUS_UNCERTAIN` değişikliği, yeni şirket ekleme, stüdyo/marka adı üretme, alan adı veya kullanıcı adı araştırma, Brand Strategy, hukuki clearance ve nihai onay kapsam dışıdır.

## Roller ve görev ayrılığı

- Program/collector sahibi: `market-research-director`. Mevcut kayıt ve Source ID bağlamını reviewer'a teslim eder; kendi kaynağını `REVIEWED` yapamaz.
- Satır reviewer'ı: `quality-assurance-director`. Erişilebilirlik, kaynak kimliği, claim support, publisher/origin bağımsızlığı ve birincil kaynak ölçütlerini satır seviyesinde doğrular; bulguyu araştırma sahibine değiştirtmez.
- Bağımsız program re-auditi: `independent-red-team-auditor` veya önceki toplama/remediation/review işinden bağımsız atanmış reviewer. Parti tamamlanmasını ve kontrol atlatma risklerini denetler; kendi audit'ini nihai onaylamaz.
- `Collector Agent ID` ile `Reviewer Agent ID` her kaynak incelemesinde farklı olmalıdır. Çıkar çatışmasında reviewer çekilir ve `FOUNDER` hattında bağımsız atama istenir.

## Deterministik partition yöntemi

Ana kaynak `docs/research/DEC-STUDIO-NAME-001-market-sample-register.csv` dosyasıdır. `Counted Sample ID` değerleri artan sözlüksel sırada (sabit sıfır dolgulu kimlik biçimi nedeniyle sayısal sırayla aynı) sıralanır.

- `STATUS_UNCERTAIN` olan 180 kayıt ardışık 30'luk dilimler hâlinde MV-A, MV-B, MV-C, MV-D, MV-E ve MV-F'ye atanır.
- `HISTORICAL_REBRAND_CASE` olan 10 kayıt MV-G'ye atanır.
- `HISTORICAL_NAMING_PROBLEM_CASE` olan 10 kayıt MV-H'ye atanır.
- Bir Counted Sample ID ve bir Canonical Entity ID yalnızca bir partide bulunabilir. Parti üyeliği elle dengelenmez veya yeniden yorumlanmaz.

## Aktif kayıt kabul ölçütleri (MV-A–MV-F)

Bir kayıt ancak en az iki erişilebilir kaynak, iki farklı Publisher/Institution, iki farklı Source Origin Group, en az bir resmî/birincil kaynak ve aynı kurumsal kökten gelmeyen iki kanıtla ilerleyebilir. Reviewer; accessibility, source identity ve claim support sonuçlarını ayrı kaydeder. `BROKEN_LINK`, `ACCESS_BLOCKED`, `NEEDS_RECHECK` ve `UNREVIEWED` qualifying evidence değildir. Yalnız bütün kontrolleri geçen kaynak `REVIEWED` olabilir. İki bağımsız sinyal oluşmazsa kayıt `STATUS_UNCERTAIN` kalır.

## Tarihsel kayıt kabul ölçütleri (MV-G–MV-H)

Reviewer; olayın gerçekleştiğini destekleyen resmî/birincil kaynak, bağımsız destekleyici kaynak, eski ad, yeni ad/çözüm, olay tarihi, doğrulanmış neden, Reviewer Agent ID, confidence ve claim-support sonucunu kaydeder. Neden doğrulanamazsa `REASON_UNVERIFIED` korunur; tahminle doldurulmaz.

## Kilometre taşları ve karar noktaları

1. **Partition kurulumu:** 200 satır ve sekiz CSV, `Verification Status = NOT_STARTED`.
2. **Parti çalışması:** Her reviewer yalnız atanmış partide append-only inceleme kanıtı üretir. Bu repository teslimi bu aşamayı çalıştırmaz.
3. **Parti QA:** Satır ölçütleri ve collector/reviewer ayrılığı doğrulanır. Başarısız kayıt açık statüde kalır.
4. **Program re-auditi:** Sekiz partinin kapsamı, benzersizliği ve kaynak yaşam döngüsü bağımsız olarak denetlenir.
5. **Handoff kararı:** Açık kanıt eksikliği varsa downstream proposal/Brand Strategy kapısı kapalı kalır; gerekli durumda `FOUNDER` hattına escalation yapılır.

## Bağımlılıklar, riskler ve azaltımlar

- Değişebilir kaynaklar ve erişim engelleri: zaman damgalı yeniden kontrol; başarısız erişimi qualifying saymama.
- Aynı kurumsal kökten sahte bağımsızlık: Publisher/Institution ve Source Origin Group ikisini birlikte doğrulama.
- Statü şişirmesi: iki bağımsız sinyal yoksa fail-closed `STATUS_UNCERTAIN`.
- Çift sayım veya elle parti kaydırma: partition validator ile tam ana-register eşitliği ve beklenen sıralı üyelik kontrolü.
- Tarihsel neden uydurma: kanıt yoksa `REASON_UNVERIFIED`.

## Doğrulama ve kabul

`python scripts/validate_market_verification_partitions.py`, tam 200 kayıt, 180/20 kohort ayrımı, 30/10 parti kotaları, benzersiz kimlikler, eksiksiz ana-register kapsamı, doğru kohort yerleşimi, başlangıç statüsü ve deterministik üyeliği doğrulamalıdır. Ayrıca Studio validator, tüm unit testler ve `git diff --check` geçmelidir. Bu plan kendi uygulamasını veya ilerideki kaynak doğrulamasını nihai onaylamaz.

## Güncel durum ve değişiklik günlüğü

- 2026-08-10: Zorunlu taban commit'i doğrulandı; sekiz partition ve fail-closed doğrulama tasarlandı. Canlı kaynak doğrulaması yapılmadı.
