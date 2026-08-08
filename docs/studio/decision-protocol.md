# Karar Kurulu Protokolü

1. Kurucu veya Studio Orchestrator karar ihtiyacını yazar.
2. Kapsam ve başarı kriterleri belirlenir.
3. Market Research Director doğrulanabilir araştırma paketi hazırlar.
4. En az üç ilgili uzman bağımsız öneri yazar.
5. Uzmanlar önerileri çapraz eleştirir.
6. Öneri sahipleri kısa rebuttal hazırlar.
7. Finance, Technical, Creative, Market ve Legal etkileri puanlanır.
8. Independent Red Team başarısızlık senaryolarını hazırlar.
9. Studio Orchestrator sonuçları çarpıtmadan birleştirir.
10. Yönetim kurulu tavsiye kararı oluşturur.
11. Geri dönüşü zor, maliyetli, hukuki veya marka kararları kurucuya sunulur.
12. Kurucu kabul, revizyon veya ret kararı verir.
13. Sonuç final-decision-record biçiminde kaydedilir.

Eksik araştırma, çıkar çatışması veya kalite kapısı ihlali süreci durdurur.

## Kayıt zinciri ve bağımsızlık
Her aşama Decision ID, benzersiz Record ID, Author Agent ID, Created Date, Source References, Conflict of Interest Declaration, Previous Stage References, Next Stage, Review Target ve Independence Declaration taşır. Aynı Decision ID altında en az üç proposal farklı agentlar tarafından yazılır. Critique yazarı hedef proposal'ın; Red-Team yazarı herhangi bir proposal'ın sahibi olamaz. Final Decision Record önceki bütün Record ID'leri açıkça listeler.

Bu metadata alanları boş bırakılamaz. Bir alan kayıt türü için gerçekten uygulanabilir değilse boş metin, `Yok` veya örtük varsayım yerine açıkça `NOT APPLICABLE` yazılır.

## Puanlama kuralı
Decision Brief'te proposal'lar görülmeden önce toplamı 100 olan ağırlıklar kilitlenir. Finance, Technical, Creative, Market ve Legal boyutları 1-5 arasında puanlanır; her puan ayrı gerekçe ve düşük/orta/yüksek güven düzeyi içerir. Eksik veri varsa puan veya toplam üretilmez ve süreç araştırma aşamasına döner. Legal, Security, Privacy ve IP kontrolleri toplam puandan bağımsız stop-gate'tir; `FAIL` veya `NOT REVIEWED` ilerlemeyi durdurur.
