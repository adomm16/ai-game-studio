# DEC-STUDIO-NAME-001 Market Verification Partitions

Bu dizin, ana Market Landscape register'ındaki 200 kaydı canlı ve bağımsız kanıt doğrulaması için sekiz çakışmasız çalışma paketine ayırır. Dosyalar doğrulama sonucu değil, başlangıç iş kuyruklarıdır; bütün satırlar `Verification Status = NOT_STARTED` ile başlar.

## Parti haritası

| Parti | Kohort | Kayıt |
|---|---|---:|
| MV-A–MV-F | `STATUS_UNCERTAIN` mevcut kuruluş/marka kayıtları | Her biri 30; toplam 180 |
| MV-G | `HISTORICAL_REBRAND_CASE` | 10 |
| MV-H | `HISTORICAL_NAMING_PROBLEM_CASE` | 10 |

Üyelik, ana register'daki `Counted Sample ID` değerlerinin artan deterministik sırasından türetilir. MV-A–MV-F ardışık 30'luk dilimlerdir; tarihsel kohortlar vaka tipine göre ayrılır. `Existing Source IDs` ve tarihsel paketlerde `Existing Case Source IDs` yalnız mevcut kaynak bağlarını taşır; bunların qualifying veya reviewed olduğunu iddia etmez.

## Reviewer çalışma sınırı

Reviewer yalnızca atanmış kayıtların kaynak erişilebilirliğini, kimliğini, desteklediği iddiayı, publisher/origin bağımsızlığını ve primary-source niteliğini doğrular. Collector ile reviewer aynı agent olamaz. Başarısız, engelli, yeniden kontrol gerektiren veya incelenmemiş kaynak qualifying evidence sayılmaz. İki bağımsız sinyal yoksa `STATUS_UNCERTAIN`; tarihsel neden doğrulanamıyorsa `REASON_UNVERIFIED` korunur.

Reviewer yeni şirket veya kaynak-dışı iddia eklemez; stüdyo/marka adı, alan adı, kullanıcı adı, Brand Strategy, hukuk sonucu veya nihai onay üretmez. Statü değişiklikleri bu başlangıç partition tesliminin parçası değildir ve ayrı, yetkili, satır-seviyesi kanıt yaşam döngüsü gerektirir.

## Yerel doğrulama

```text
python scripts/validate_market_verification_partitions.py
```

Validator ana register ile tam kapsam eşitliğini, parti kotalarını, Counted/Canonical benzersizliğini, kohort sınırlarını, deterministik üyeliği, alan eşleşmelerini ve bütün başlangıç statülerinin `NOT_STARTED` olduğunu fail-closed kontrol eder.
