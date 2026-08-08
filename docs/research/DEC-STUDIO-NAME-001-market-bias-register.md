# DEC-STUDIO-NAME-001 Market Bias Register

## Register

| Bias ID | Risk | Etkilenen katman | Azaltma yöntemi | Kalan risk | Güven düzeyi | Owner | Review Date | Reviewer |
|---|---|---|---|---|---|---|---|---|
| BIAS-001 | Tanınmış marka yanlılığı | Tüm aktif katmanlar | Bağımsız ve bölgesel kayıtlar için ayrı kotalar; 45 bağımsız kayıt | Büyük resmî siteler daha kolay doğrulanıyor | MEDIUM | market-research-director | 2026-08-08 | NOT ASSIGNED — independent audit required |
| BIAS-002 | İngilizce kaynak yanlılığı | Doğu Asya; Güney/Güneydoğu Asya; Türkiye/yakın bölge | Resmî yerel/İngilizce sayfaları birlikte kabul et; localization handoff | Kaynak sicili büyük ölçüde İngilizce URL kullanıyor | HIGH | market-research-director | 2026-08-08 | NOT ASSIGNED — independent audit required |
| BIAS-003 | Hayatta kalma yanlılığı | İlk altı aktif katman | Ayrı 20 tarihsel olay kotası; kapanan kuruluşları aktif saymama | Aktif kota tanımı gereği başarısız/kapanmış kuruluşlar az | MEDIUM | market-research-director | 2026-08-08 | NOT ASSIGNED — independent audit required |
| BIAS-004 | Platform yanlılığı | Oyun katmanları | Kurumsal ve ürün sayfalarını birlikte kullan; tek mağazaya bağlı kalmama | PC/console resmi web varlığı mobil/yerel mağazalardan daha görünür | MEDIUM | market-research-director | 2026-08-08 | NOT ASSIGNED — independent audit required |
| BIAS-005 | Bölgesel yanlılık | Tüm katmanlar | Altı bölgeyi raporla; sıfır bölge bırakma; %40 alarmını izle | Avrupa ve Kuzey Amerika toplamı yüksektir | HIGH | market-research-director | 2026-08-08 | NOT ASSIGNED — independent audit required |
| BIAS-006 | Yalnız başarılı örnekleri seçme yanlılığı | Rebrand ve sorun vakaları | 10 isim sorunu vakasını ayrı kota olarak koru | Belgelenmiş vakalar yayınlanmayan sorunları temsil etmeyebilir | MEDIUM | market-research-director | 2026-08-08 | NOT ASSIGNED — independent audit required |
| BIAS-007 | Güncel veri erişimi yanlılığı | Tüm aktif katmanlar | İki sinyal, zaman damgası, recheck ve fail-closed kuralı | Dinamik/engelli sayfalar bağımsız audit sırasında düşebilir | HIGH | market-research-director | 2026-08-08 | NOT ASSIGNED — independent audit required |

## Regional access notes

| Region | Count | Main source types | Access limitations | Remaining bias risk |
|---|---:|---|---|---|
| Avrupa | 77 | Official corporate and product/platform pages | Dynamic pages, redirects, language/locale variation; no paid databases | MEDIUM |
| Diğer bölgeler | 6 | Official corporate and product/platform pages | Dynamic pages, redirects, language/locale variation; no paid databases | HIGH |
| Doğu Asya | 31 | Official corporate and product/platform pages | Dynamic pages, redirects, language/locale variation; no paid databases | HIGH |
| Güney ve Güneydoğu Asya | 3 | Official corporate and product/platform pages | Dynamic pages, redirects, language/locale variation; no paid databases | HIGH |
| Kuzey Amerika | 71 | Official corporate and product/platform pages | Dynamic pages, redirects, language/locale variation; no paid databases | MEDIUM |
| Türkiye ve yakın bölge | 12 | Official corporate and product/platform pages | Dynamic pages, redirects, language/locale variation; no paid databases | HIGH |

The achieved distribution is reported as observed; no exact regional quota is invented. Independent QA must decide whether the residual regional concentration is acceptable or requires controlled replacements.
