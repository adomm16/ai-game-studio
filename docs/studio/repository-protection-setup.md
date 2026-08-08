# Repository Koruması Kurulumu

Bu belge GitHub yöneticisinin `chatgb` dalına uygulaması gereken ruleset'i tanımlar. Repository içindeki dosyaların varlığı bu yönetici ayarlarının uygulanmış olduğunu kanıtlamaz.

## Hedef ruleset

- workflow file: `.github/workflows/studio-validation.yml`
- workflow name: `studio-validation`
- required job ID: `validate`
- job display name: `studio-validation`
- required status check: `studio-validation`

- Hedef dal: yalnızca `chatgb`.
- Değişiklikler pull request üzerinden gelmeli; doğrudan push kapalı olmalı.
- En az iki onay istenmeli: biri `.github/CODEOWNERS` tarafından belirlenen kurucu/code owner, diğeri öneri sahibinden bağımsız reviewer.
- Yeni commit geldiğinde eski onaylar düşürülmeli ve tüm review konuşmaları çözülmeli.
- GitHub ruleset içinde yukarıdaki required status check seçilmeli; dalın güncel olması zorunlu tutulmalı.
- Force push ve branch deletion yasaklanmalı.
- Bypass listesi varsayılan olarak boş olmalı. Acil bypass gerekiyorsa kurucu onayı, gerekçe, zaman ve sonradan bağımsız review kaydı zorunlu olmalı.
- Code owner review zorunlu tutulmalı; son push'u yapan kişinin tek başına onayı yeterli sayılmamalı.
- Linear history tercihi repository politikasıyla ayrıca kararlaştırılabilir; governance güvencesi olarak sunulmamalı.

## Yönetici doğrulaması

Ruleset etkinleştirildikten sonra GitHub UI veya API üzerinden hedef dal, required check adı, review sayısı, code-owner review, force-push/deletion ve bypass ayarları ekran görüntüsü ya da dışa aktarılmış JSON ile kaydedilmelidir. Bu kanıt bağımsız re-audit'e eklenmelidir.

## Repository içi statik doğrulamanın sınırı

Validator, GitHub hesabının gerçekten var olduğunu veya ruleset'in yönetici tarafından uygulandığını doğrulamaz. `studio.manifest.json` politikasına göre CODEOWNERS satırlarının repository içi sözdizimini, açık kritik pattern kapsamını ve beklenen owner metnini kontrol eder.

Workflow kontrolü tam YAML parser değildir. Standard library ile güvenilir biçimde incelenen alt küme; boşlukla girintilenmiş block mapping/list yapıları, düz veya tırnaklı scalar değerler ve tek satırlı `name`, `uses`, `run` alanlarıdır. Manifest bu sınırı `supported_yaml_subset` alanında yayımlar.

Kritik workflow yapısında tab, flow mapping (`{...}`), flow sequence (`[...]`), anchor (`&name`), alias (`*name`), merge key (`<<:`), multiline `run: |` / `run: >` ve `${{ ... }}` expression kullanımı fail-closed reddedilir. Yorum içindeki örnekler ve komutun ortasında güvenli düz metin olarak kullanılan köşeli parantezler flow collection sayılmaz. Required job veya adımlarında `continue-on-error`, required job üzerinde `needs`/`if`, zorunlu komutlarda manifestteki failure-masking pattern'leri ve aynı display name'i taşıyan ikinci bir job da reddedilir.
