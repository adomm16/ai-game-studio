# DEC-STUDIO-NAME-001 Kaynak Sicili

## Başlangıç durumu ve kapsam

Bu belge gerçek araştırma sonucu içermeyen boş sicil şablonu ve kayıt kurallarıdır. Aşağıdaki tablo başlık dışında satır içermez. Yöntem belgelerinde anılan resmî bağlantılar bu sicilde gerçek kaynak kaydı oluşturmaz; araştırma yürütmesi başladığında yetkili agent tarafından erişilip doğrulanarak yeni Source ID ile kaydedilir.

Bu görevde aday isim, marka, alan adı, sosyal kullanıcı adı, pazar örneği veya uygunluk sonucu toplanmamıştır.

## Zorunlu alanlar ve kurallar

| Alan | Kural |
|---|---|
| Source ID | `SRC-DEC-STUDIO-NAME-001-NNN` biçiminde benzersiz kimlik |
| Decision ID | Daima `DEC-STUDIO-NAME-001` |
| Research Workstream | Pazar, örneklem, localization, Legal/IP, sınıflandırma veya diğer onaylı iş kolu |
| Agent ID | Kaydı oluşturan sorumlu agent |
| Source Name | Kaynağın görünen adı/başlığı |
| Source Type | Resmî veritabanı, resmî rehber, birincil kayıt veya nitelikli ikincil gibi kaynak türü |
| URL or Official Record Identifier | Doğrudan URL veya yetkili kayıt kimliği; arama özeti değil |
| Publisher/Institution | Yayınlayan kişi/kurum; Source Name'den ayrı tutulur |
| Access Date | ISO `YYYY-MM-DD` |
| Access Time | Saat, dakika, saniye ve saat dilimi; elde edilemiyorsa `NOT AVAILABLE` ve sınırlama |
| Data Date | Kaynağın yayın/değişiklik/veri tarihi; yoksa `NOT FOUND` |
| Country/Jurisdiction | Ülke, yargı alanı ve gerekiyorsa locale |
| Supported Claim | Kaynağın doğrudan desteklediği dar iddia |
| Primary/Secondary Classification | `PRIMARY` veya `SECONDARY`; gerekçe Notes içinde |
| Confidence Level | `LOW`, `MEDIUM` veya `HIGH`; gerekçe Notes içinde |
| Conflict of Interest Declaration | Bilinen ilişki veya `NONE KNOWN` |
| Volatility/Changeability Status | `STABLE`, `PERIODICALLY_CHANGEABLE`, `HIGHLY_CHANGEABLE` veya `UNKNOWN` |
| Recheck Date | ISO tarih veya olay eşiği; uygulanamazsa `NOT APPLICABLE` |
| Evidence Allocation | Kanıt kimliği, birincil ölçüt ve varsa ayrı gerekçeli ikincil etki; gate/puan kullanımı |
| Notes | Sorgu, filtre, resmî veri, normalizasyon ve gerekçeler |
| Limitations | Kapsam dışı, karşı kanıt, belirsizlik ve garanti vermeyen yönler |
| Review Status | `UNREVIEWED`, `REVIEWED`, `NEEDS_RECHECK` veya `REJECTED` |
| Reviewer Agent ID | Reviewer agent; incelenmediyse `NOT ASSIGNED` |

Hiçbir zorunlu alan boş bırakılamaz. Uygulanmayan alan `NOT APPLICABLE`, bulunamayan veri `NOT FOUND`, doğrulanamayan veri `NOT VERIFIED` olarak yazılır; bunlar gerçek değer yerine geçmez. Resmî veri ile araştırmacı çıkarımı Notes içinde ayrı tutulur ve çıkarım `INFERENCE:` etiketi taşır.

## Boş kayıt şablonu

| Source ID | Decision ID | Research Workstream | Agent ID | Source Name | Source Type | URL or Official Record Identifier | Publisher/Institution | Access Date | Access Time | Data Date | Country/Jurisdiction | Supported Claim | Primary/Secondary Classification | Confidence Level | Conflict of Interest Declaration | Volatility/Changeability Status | Recheck Date | Evidence Allocation | Notes | Limitations | Review Status | Reviewer Agent ID |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

## SAMPLE/TEMPLATE kuralı

Bir biçim örneği gerekirse satırın Source ID değeri `SAMPLE` veya `TEMPLATE` ile başlamalıdır. Böyle satır gerçek kaynak kaydı sayılamaz, validator ve araştırma toplamlarından dışlanır, Evidence Allocation'a bağlanamaz ve `REVIEWED` durumu alamaz. Bu belgede örnek satır yoktur.

## Kayıt yaşam döngüsü

- Yeni kayıt başlangıçta `UNREVIEWED` olur.
- Reviewer tüm zorunlu alanları, doğrudan kaynağı ve iddia tahsisini kontrol ederse `REVIEWED` yapabilir.
- Değişebilir veri eskimiş, erişilemez veya çelişkiliyse `NEEDS_RECHECK`; kanıt standardını karşılamıyorsa `REJECTED` olur.
- Değişiklik önceki kaydı sessizce silmez. Yeni Source ID veya sürümlü append-only kayıt kullanılır; eski kayıt Notes ve Evidence Allocation ile bağlanır.
- `HIGHLY_CHANGEABLE` kaynak proposal ve finalist aşamalarında yeniden kontrol edilir. Recheck başarısızsa ilgili iddia `INCOMPLETE` olur ve `PASS` üretmez.
