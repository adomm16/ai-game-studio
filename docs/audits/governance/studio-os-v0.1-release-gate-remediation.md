# Studio OS v0.1 Release Gate Remediation

- Remediation dalı: `fix/studio-os-v0.1-release-gate`
- Tarih: 2026-08-08
- Kapsam: final bağımsız re-audit içindeki `NF-P1-01`, `NF-P2-01` ve `NF-P2-02`
- Onay sınırı: Bu kayıt bağımsız release-gate onayı değildir; bağımsız re-audit ve GitHub yönetici kanıtı gerekir.

| Finding ID | Yapılan düzeltme | Kod kanıtı | Test kanıtı | Durum |
|---|---|---|---|---|
| NF-P1-01 | Manifest workflow dosyası, adı, exact `validate` job ID'si, display name, status check, trigger, branch, action ve komut kimliğini tanımlar. Validator gerekli job sınırını ayırır; boş/yeniden adlandırılmış job, başka job'a taşınan kontroller, sahte aynı isimli job, `needs`/`if` ve `continue-on-error` kullanımını reddeder. Koruma belgesi kimliği manifestle eşleşmelidir. | `studio.manifest.json`; `scripts/validate_studio.py`; `.github/workflows/studio-validation.yml`; `docs/studio/repository-protection-setup.md` | Exact job ID/display name, komut taşıma, sahte job, job dependency ve yalnız-manifest policy testleri; izole job rename/command move CLI mutasyonları exit 1 | FIXED |
| NF-P2-01 | Standard library tabanlı kontrolün desteklediği block-YAML alt kümesi manifestte ve koruma belgesinde yayımlandı. Tab, flow mapping/sequence, anchor/alias, merge key, multiline run ve dynamic expression kritik workflow yapısında fail-closed reddedilir; yorum ve güvenli düz metin negatif kontrolleri yanlış alarmı önler. | `studio.manifest.json`; `scripts/validate_studio.py`; `docs/studio/repository-protection-setup.md` | Flow mapping/sequence, anchor/alias/merge, multiline, expression, failure masking ve güvenli yorum/düz metin testleri; `env: {AUDIT_MODE: strict}` ile anchor/alias izole mutasyonları exit 1 | FIXED |
| NF-P2-02 | Agent başlıkları, 14 kurucu yetkisi, AGENTS linkleri, karar metadata/aşama politikası, bağımsız roller, zorunlu/yasak ilişkiler, kritik dosyalar, CODEOWNERS, workflow ve profil tekrar allowlist'i manifestte tutulur. Validator liste değerlerini yalnız manifestten okur; tip, boşluk, tekrar, bilinmeyen rol ve ilişki çelişkileri hard-fail'dir. | `studio.manifest.json`; `scripts/validate_studio.py` | Manifest-only kaynak testleri; eksik başlık/kurucu hakkı belge çelişkileri; tip/boş/tekrar/bilinmeyen rol/çelişkili ilişki testleri; üç izole policy-removal mutasyonu exit 1 | FIXED |

## Bilinen sınırlar ve manuel işlemler

- Validator tam YAML parser değildir; yalnız belgelenmiş alt kümeyi kabul eder ve kritik destek dışı yapıları fail-closed reddeder.
- Kurucu yetkisi doğal dil eşdeğerleri ve Markdown heading anchor doğruluğu insan incelemesi gerektiren non-blocking warning olarak kalır.
- `chatgb` ruleset, PR/review koşulları, required check seçimi, force-push/deletion ve bypass ayarları repository içinden doğrulanamaz; GitHub yöneticisi UI/API kanıtı sağlamalıdır.
- `@adomm16` hesabının gerçek kurucu/code-owner kimliği manuel doğrulanmalıdır.
- Değişiklikler bağımsız release-gate re-audit'inden geçmeden APPROVED sayılmamalıdır.
