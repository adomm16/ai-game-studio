# DEC-STUDIO-NAME-001 Kaynak Sicili

## Kapsam

Bu sicil şu anda yalnız araştırma yöntemini kurmak için kullanılan resmî kaynakları içerir. Herhangi bir aday isim, marka uygunluğu sonucu, alan adı veya kullanıcı adı sorgusu içermez. Yeni kayıtlar append-only mantığıyla eklenir; değişiklik eski satırı silmez.

## Zorunlu kayıt şeması

| Alan | Kural |
|---|---|
| Source ID | `SRC-DEC-STUDIO-NAME-001-NNN` biçiminde benzersiz kimlik |
| Kurum/yazar | Kaynağın sorumlu kurumu veya yazarı |
| Başlık | Kaynaktaki başlık |
| URL/belge kimliği | Doğrudan kaynak; arama sonucu sayfası değil |
| Yayın/değişiklik tarihi | Kaynakta yoksa `BULUNAMADI` |
| Erişim | ISO tarih-saat ve saat dilimi |
| Tür | Resmî sınıflandırma, resmî veritabanı, resmî rehber, birincil kayıt veya nitelikli ikincil |
| Yargı/locale | Kapsanan ülke, kurum ve dil |
| Sorgu/seçim yöntemi | Yeniden üretilebilir sorgu, filtre veya belge bölümü; uygulanmazsa `NOT APPLICABLE` |
| Desteklenen iddia | Kaynağın doğrudan desteklediği dar iddia |
| Resmî veri | Kaynaktan aktarılan alan; yorum eklenmez |
| Araştırmacı çıkarımı | Varsa `INFERENCE:` etiketi ve gerekçe |
| Karşı kanıt/sınırlama | Kapsam dışı ve garanti vermeyen yönler |
| Çıkar çatışması | Bilinen ilişki veya `BİLİNEN YOK` |
| Güven | DÜŞÜK/ORTA/YÜKSEK ve kısa gerekçe |
| Yeniden kontrol | Tarih/olay eşiği |
| Durum | CURRENT, STALE, SUPERSEDED veya UNVERIFIED |

## Kurulum kaynakları

| Source ID | Kurum/yazar | Başlık | URL/belge kimliği | Yayın/değişiklik tarihi | Erişim | Tür | Yargı/locale | Sorgu/seçim yöntemi | Desteklenen iddia | Resmî veri | Araştırmacı çıkarımı | Karşı kanıt/sınırlama | Çıkar çatışması | Güven | Yeniden kontrol | Durum |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| SRC-DEC-STUDIO-NAME-001-001 | WIPO | Nice Classification | https://www.wipo.int/en/web/classification-nice/index | BULUNAMADI | 2026-08-08T17:40:50+03:00 | Resmî sınıflandırma ana sayfası | Uluslararası / EN | Ana sayfa ve yürürlükteki NCL edition/version | NCL'nin marka tescili için mal ve hizmet sınıflandırması olduğu ve düzenli sürümlendiği | NCL'nin amacı ve sürüm erişimi | INFERENCE: yöntem otoritesi olarak kullanılmalı | Belirli faaliyet için sınıf veya tescil sonucu vermez | BİLİNEN YOK | YÜKSEK — birincil kurum | Her sınıf çalışması öncesi edition/version | CURRENT |
| SRC-DEC-STUDIO-NAME-001-002 | WIPO | Frequently Asked Questions: Nice Classification | https://www.wipo.int/en/web/classification-nice/faq | BULUNAMADI | 2026-08-08T17:40:50+03:00 | Resmî rehber | Uluslararası / EN | NCL yapısı bölümü | NCL'nin sınıf listesi, açıklayıcı notlar ve alfabetik listeden oluştuğu | Sınıflandırmanın resmî bileşenleri | INFERENCE: başlık tek başına yeterli değildir | Rehber belirli faaliyete kesin sınıf atamaz | BİLİNEN YOK | YÜKSEK — birincil kurum | WIPO sürüm değişikliğinde | CURRENT |
| SRC-DEC-STUDIO-NAME-001-003 | CIPO / ISED Canada | Search intellectual property databases | https://www.ised-isde.canada.ca/site/canadian-intellectual-property-office/en/search-intellectual-property-databases | 2022-05-09 | 2026-08-08T17:40:50+03:00 | Resmî rehber | Kanada / EN | Trademarks bölümü ve scope/limitations | Canadian trademarks search'ün resmî başlangıç yüzeyi olduğu ve ön aramanın sınırlı olduğu | Veritabanı bağlantısı ve CIPO sınırlamaları | INFERENCE: Kanada ön taramasının ana otoritesi | Kayıt dışı/common-law veya Quebec civil-law haklarını dışlamaz; profesyonel doğrulama önerir | BİLİNEN YOK | YÜKSEK — resmî kurum | Her Kanada yöntem çalışması öncesi | CURRENT |
| SRC-DEC-STUDIO-NAME-001-004 | CIPO / ISED Canada | Canadian Trademarks Database — Search | https://ised-isde.canada.ca/cipo/trademark-search/srch?lang=eng | Veritabanı güncellemesi dinamik | 2026-08-08T17:40:50+03:00 | Resmî veritabanı | Kanada / EN | Arama alanları ve filtreler gözden geçirildi; aday sorgusu yapılmadı | Marka türü, durum, Nice sınıfı ve tarih filtresiyle resmî ön tarama yapılabildiği | Arama alanları, durum ve sınıf filtreleri | INFERENCE: sorgular yeniden üretilebilir parametrelerle kaydedilmeli | Sonuç yokluğu hukuki temizlik garantisi değildir; veri değişebilir | BİLİNEN YOK | YÜKSEK — resmî veritabanı | Her sorguda güncelleme tarihi ve aynı iş günü | CURRENT |
| SRC-DEC-STUDIO-NAME-001-005 | CIPO / ISED Canada | Goods and Services Manual | https://www.ised-isde.canada.ca/site/canadian-intellectual-property-office/en/trademarks/goods-and-services-manual | BULUNAMADI | 2026-08-08T17:40:50+03:00 | Resmî mal/hizmet rehberi | Kanada / EN | Arama seçenekleri ve Nice edition bilgisi | Kanada için kabul edilebilir mal/hizmet ifadeleri ve güncel Nice sürümü bağlamı sağladığı | CIPO mal/hizmet terimleri ve sürüm bilgisi | INFERENCE: WIPO eşleşmesi Kanada uygulamasıyla çaprazlanmalı | İçerik değişebilir; başvuru veya tescil kabulü hakkında genel garanti vermez | BİLİNEN YOK | YÜKSEK — resmî kurum | Nice edition veya CIPO içerik değişikliğinde | CURRENT |

## Çıkarım ve değişiklik kayıtları

Henüz araştırmacı çıkarımı, aday sorgusu veya örneklem kaydı yoktur. İleride her çıkarım kaynak satırından ayrı bir evidence-allocation kaydıyla bağlanır. Resmî durum değişirse eski kayıt `SUPERSEDED`, yeni kayıt yeni Source ID ile `CURRENT` olur.
