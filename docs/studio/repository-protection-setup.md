# Repository Koruması Kurulumu

Bu belge GitHub yöneticisinin `chatgb` dalına uygulaması gereken ruleset'i tanımlar. Repository içindeki dosyaların varlığı bu yönetici ayarlarının uygulanmış olduğunu kanıtlamaz.

## Hedef ruleset

- Hedef dal: yalnızca `chatgb`.
- Değişiklikler pull request üzerinden gelmeli; doğrudan push kapalı olmalı.
- En az iki onay istenmeli: biri `.github/CODEOWNERS` tarafından belirlenen kurucu/code owner, diğeri öneri sahibinden bağımsız reviewer.
- Yeni commit geldiğinde eski onaylar düşürülmeli ve tüm review konuşmaları çözülmeli.
- Required status check: `studio-validation`; dalın güncel olması zorunlu tutulmalı.
- Force push ve branch deletion yasaklanmalı.
- Bypass listesi varsayılan olarak boş olmalı. Acil bypass gerekiyorsa kurucu onayı, gerekçe, zaman ve sonradan bağımsız review kaydı zorunlu olmalı.
- Code owner review zorunlu tutulmalı; son push'u yapan kişinin tek başına onayı yeterli sayılmamalı.
- Linear history tercihi repository politikasıyla ayrıca kararlaştırılabilir; governance güvencesi olarak sunulmamalı.

## Yönetici doğrulaması

Ruleset etkinleştirildikten sonra GitHub UI veya API üzerinden hedef dal, required check adı, review sayısı, code-owner review, force-push/deletion ve bypass ayarları ekran görüntüsü ya da dışa aktarılmış JSON ile kaydedilmelidir. Bu kanıt bağımsız re-audit'e eklenmelidir.

## Repository içi statik doğrulamanın sınırı

Validator, GitHub hesabının gerçekten var olduğunu veya ruleset'in yönetici tarafından uygulandığını doğrulamaz. `studio.manifest.json` politikasına göre CODEOWNERS satırlarının repository içi sözdizimini, açık kritik pattern kapsamını ve beklenen owner metnini kontrol eder.

Workflow kontrolü tam YAML parser değildir. Standard library ile güvenilir biçimde incelenen alt küme; boşlukla girintilenmiş block mapping/list yapısı, düz veya tırnaklı scalar değerler, tek satırlı `uses` ve `run` alanlarıdır. Tab, anchor/alias, flow mapping, multiline `run` ve expression ile oluşturulan komutlar bu güvenlik kontrolünün desteklenen alt kümesi dışındadır; sessiz PASS yerine anlaşılır hata üretir.
