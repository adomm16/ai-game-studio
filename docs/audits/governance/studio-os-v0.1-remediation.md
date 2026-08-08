# Studio OS v0.1 Governance Remediation

- Remediation dalı: `fix/studio-os-v0.1-audit`
- Tarih: 2026-08-08
- Kapsam: `studio-os-v0.1-audit.md` bulguları
- Onay durumu: Bu rapor APPROVED ilanı değildir; bağımsız re-audit gerektirir.

| Finding ID | Seviye | Yapılan düzeltme | Kanıt dosyaları | Test | Durum |
| ---------- | ------ | ---------------- | --------------- | ---- | ----- |
| P1-01 Repository koruması | P1 | Gerçek CODEOWNERS, PR/push doğrulama workflow'u ve `chatgb` ruleset kurulum tarifi eklendi. GitHub yönetici ruleset'i uygulanmadı. | `.github/CODEOWNERS`; `.github/workflows/studio-validation.yml`; `docs/studio/repository-protection-setup.md` | Eksik CODEOWNERS/workflow negatif testleri; validator; workflow statik inceleme | REQUIRES MANUAL ADMIN ACTION |
| P1-02 Agent görev ayrımı | P1 | Ortak kurallar tek politikaya taşındı; 29 profil role özgü uzmanlık, karar yaklaşımı, girdi/çıktı, araştırma, QC, başarı, red/escalation, handoff ve işbirliği sözleşmeleriyle yeniden yazıldı. | `docs/agents/shared-professional-policy.md`; `docs/agents/*.md` | 29 rol, zorunlu başlık, ID ve UTF-8 testleri | FIXED |
| P1-03 Orchestrator bağımsızlığı | P1 | “Tüm uzman roller” kaldırıldı. Orchestrator görev yönlendirme, koordinasyon, kayıt, takvim/bağımlılık, tarafsız sentez ve eksik teslim escalation'ıyla sınırlandı. Beş güvence rolü doğrudan kurucu hattına alındı. | `docs/agents/studio-orchestrator.md`; beş bağımsız profil; `docs/studio/governance.md`; `docs/studio/organization-chart.md` | Yasak supervisor, bağımsız reporting ve “Tüm uzman roller” negatif testleri | FIXED |
| P1-04 Validator kapsamı | P1 | Manifest zorunlu dosya/dizin/rol kaynağı oldu; envanter, profiller, raporlama/denetim, bağımsızlık, kurucu yetkisi, CODEOWNERS/CI, şablonlar, kritik içerik ve Markdown link kontrolleri eklendi. Güvenilmez semantik iddialar warning üretir. | `studio.manifest.json`; `scripts/validate_studio.py` | 32 unit test ve CLI testi | FIXED |
| P2-01 Karar zinciri | P2 | On zorunlu metadata alanı bütün şablonlara eklendi; üç farklı proposal sahibi, critique/red-team yazar ayrılığı ve final geçmiş referansları doğrulanıyor. | `docs/decisions/templates/*.md`; `scripts/validate_studio.py` | Tam zincir pozitif; dört bağımsızlık/referans negatif testi | FIXED |
| P2-02 Puanlama protokolü | P2 | 1–5 ölçeği, proposal öncesi kilitli ağırlık, puan gerekçesi, güven düzeyi, eksik veri dönüşü ve Legal/Security/Privacy/IP stop-gate tanımlandı. | `docs/studio/decision-protocol.md`; `docs/decisions/templates/decision-brief.md`; `docs/decisions/templates/scorecard.md` | Zorunlu template/alan envanter testleri | FIXED |
| P2-03 Test kapsamı | P2 | Mutlu yolun yanında manifest, başlık, template, bağlantı, boş/placeholder, rol ilişkisi, yetki devri, CODEOWNERS, workflow, CLI, çoklu hata ve Türkçe UTF-8 testleri eklendi. | `tests/test_validate_studio.py` | `python -m unittest discover -s tests -v` | FIXED |
| P3-01 AGENTS bağlantıları | P3 | IP politikası ve kalite kapıları başlangıç bağlantılarına eklendi ve validator ile zorunlu kılındı. | `AGENTS.md`; `scripts/validate_studio.py` | İki ayrı eksik bağlantı negatif testi | FIXED |
| P3-02 Markdown bağlantıları | P3 | Inline ve reference-style yerel hedef kontrolü eklendi; heading anchor doğruluğu bilinçli warning olarak bırakıldı. | `scripts/validate_studio.py` | Inline/reference-style bozuk bağlantı testi | PARTIALLY FIXED |

## Manuel kalan işlemler

1. GitHub yöneticisi `docs/studio/repository-protection-setup.md` içindeki `chatgb` ruleset'ini uygulamalı ve dışa aktarılmış kanıtı saklamalıdır.
2. Kurucu/code-owner `@adomm16` kimliğinin uygun kişi/team olduğunu doğrulamalıdır.
3. Required check adı GitHub UI'da `studio-validation` olarak seçilmelidir.
4. Bu değişiklikler öneri sahibi dışında Independent Red-Team Auditor ve kurucu tarafından atanmış reviewer ile bağımsız re-audit'ten geçirilmelidir.

## Bilinen sınırlar

Metin tabanlı kurucu yetkisi taraması bütün olası Türkçe/İngilizce semantik devri kanıtlayamaz. Markdown heading anchor doğruluğu parser kullanılmadan kesinleştirilmez. Validator bu iki alanı warning olarak gösterir; yeşil sonuç bağımsız insan incelemesinin yerine geçmez.
