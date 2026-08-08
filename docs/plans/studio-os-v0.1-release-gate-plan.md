# Studio OS v0.1 Release Gate Plan

## Amaç

Final bağımsız re-audit içindeki `NF-P1-01`, `NF-P2-01` ve `NF-P2-02` bulgularını manifest tabanlı, fail-closed ve regresyon testleriyle doğrulanan repository kontrolleriyle gidermek.

## Kapsam dışı

GitHub ruleset ayarlarını uygulamak veya doğrulanmış göstermek; önceki audit kayıtlarını değiştirmek; bağımsız release-gate onayı vermek; harici Python bağımlılığı eklemek.

## Kilometre taşları

1. Yapısal politika listelerini manifeste taşıma ve manifest şemasını fail-closed doğrulama.
2. Workflow job kimliği ile desteklenen YAML alt kümesini manifest politikasına göre doğrulama.
3. Pozitif, negatif ve izole adversarial regresyonlar.
4. Remediation kanıtı, teknik doğrulama ve küçük commitler.

## Doğrulama

`python scripts/validate_studio.py`, `python -m unittest discover -s tests -v`, `git diff --check` ve talepteki yedi izole repository mutasyonunun non-zero CLI sonucu zorunludur.

## Güncel durum

- 2026-08-08: Plan açıldı; final re-audit, önceki remediation, manifest, validator, testler, workflow ve koruma belgesi okundu.
- 2026-08-08: Manifest şeması, workflow job kimliği ve fail-closed YAML alt kümesi tamamlandı.
- 2026-08-08: 96 test ve yedi izole adversarial CLI mutasyonu PASS; remediation kaydı ve son teknik doğrulama hazırlanıyor.
