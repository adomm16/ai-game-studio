# Agent Çalışma Haritası

## Amaç
Bu repository, marka ve oyun kararlarından bağımsız, denetlenebilir AI Game Studio yönetişim ve üretim altyapısıdır.

## Önce oku
- [Anayasa](docs/studio/constitution.md)
- [Kurucu hakları](docs/studio/founder-rights.md)
- [Yönetişim](docs/studio/governance.md)
- [Karar protokolü](docs/studio/decision-protocol.md)
- [Araştırma politikası](docs/studio/research-policy.md)
- [Güvenlik politikası](docs/studio/security-policy.md)
- [Fikrî mülkiyet politikası](docs/studio/intellectual-property-policy.md)
- [Kalite kapıları](docs/studio/quality-gates.md)
- [Definition of Done](docs/studio/definition-of-done.md)
- [Agent dizini](docs/agents/index.md)

## Yetki
Kurucu korunmuş konularda nihai yetkilidir. Hiçbir agent tek başına önemli karar veremez veya kendi işini nihai onaylayamaz.

## Çalışma ve teslim
- Görevden önce ilgili uzman profilini okuyun.
- Varsayımları etiketleyin; bilinmeyeni uydurmayın.
- Girdileri, çıktıları, riskleri, maliyeti ve doğrulama kanıtını kaydedin.
- Başka bir uzmanlık alanında nihai karar vermeyin; handoff veya escalation yapın.
- Oyun kodu, marka, oyun adı veya görsel üretimi için açık ve onaylı karar kaydı olmadan başlamayın.

## Araştırma
Pazar, hukuk, güvenlik ve teknik iddialar için kaynak, yayın tarihi, erişim tarihi ve güven düzeyi yazılmalıdır. [Araştırma politikası](docs/studio/research-policy.md) ve [kaynak sicili](docs/research/sources-register.md) zorunludur.

## Karar protokolü
Önemli kararlar araştırma, en az üç bağımsız öneri, çapraz eleştiri, rebuttal, çok boyutlu puanlama, bağımsız red-team, tarafsız sentez ve gerekli durumda kurucu onayından geçer. Kayıtlar [karar alanında](docs/decisions/index.md) tutulur.

## Git ve pull request
- Varsayılan dala doğrudan commit veya kurucu onayı olmadan merge yapmayın.
- Ayrı ve açıklayıcı bir branch kullanın.
- Küçük, anlamlı commitler ve doğrulama kanıtı üretin.
- PR; kapsamı, riskleri, testleri, ilgili karar kaydını ve kurucu onayı gereksinimini belirtmelidir.
- Secret, API anahtarı veya kişisel veri commit etmeyin.

## Test
```bash
python scripts/validate_studio.py
python -m unittest discover -s tests -v
```

## Uzun işler
Uzun, çok adımlı veya belirsiz işler için başlamadan önce [PLANS.md](docs/plans/PLANS.md) biçiminde yaşayan plan oluşturun.

## Güvenlik
En az ayrıcalık, secret taraması, veri minimizasyonu ve güvenli varsayılanlar uygulanır. Olayları [escalation politikasına](docs/studio/escalation-policy.md) göre bildirin.
