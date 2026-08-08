# DEC-STUDIO-NAME-001 Örneklem Çerçevesi

## Amaç, evren ve değişmez sayım kuralı

Bu çerçeve isim üretmez ve gerçek pazar örneklemini başlatmaz. Proposal'lardan önce kurulacak karşılaştırma evreni tam olarak 200 benzersiz sayım birimidir: 150 oyun stüdyosu/yayıncı ve 50 yakın marka. Her `Sample ID` yalnız bir birincil katmana ve yalnız bir kota hücresine sayılır. İkincil etiketler analize yardımcı olur fakat hiçbir toplamı artırmaz.

## Sayısal dağılım

| Birincil katman | Hedef | Üst grup |
|---|---:|---|
| Büyük uluslararası yayıncılar | 30 | Oyun stüdyosu/yayıncı |
| Bağımsız stüdyolar | 45 | Oyun stüdyosu/yayıncı |
| Mobil oyun şirketleri | 25 | Oyun stüdyosu/yayıncı |
| PC/konsol stüdyoları | 25 | Oyun stüdyosu/yayıncı |
| Strateji/MMO üreticileri | 25 | Oyun stüdyosu/yayıncı |
| Teknoloji/yaratıcı üretim markaları | 30 | Yakın marka |
| Başarılı rebrand örnekleri | 10 | Yakın marka — tarihsel vaka |
| Marka sorunu örnekleri | 10 | Yakın marka — tarihsel vaka |
| **Toplam** | **200** | **150 oyun stüdyosu/yayıncı + 50 yakın marka** |

Hedeflerin tamamı bağlayıcıdır. Önceki `%80` toleransı kaldırılmıştır; eksik kota tamamlanmış örneklem olarak sunulamaz.

## Birincil katman ataması

Bir marka birden fazla katmana uyuyorsa birincil katman sırasıyla şu kanıtlarla belirlenir:

1. ana gelir ve ürün faaliyeti;
2. son üç yıldaki yayın portföyü;
3. resmî şirket tanımı;
4. belirsizlik sürüyorsa `market-research-director` escalation kararı.

Atama gerekçesi ve kullanılan Source ID'ler kaydedilir. İkincil faaliyet, platform, tür, rebrand veya sahiplik etiketleri ayrı tutulabilir; kota toplamına ikinci kez eklenemez. Aynı tüzel kişi, marka veya tarihsel olay için oluşturulan ilişkili kayıtlar `Related Sample/Event ID` ile bağlanır ve tek bir `Counted Sample ID` gösterir.

## Seçim prosedürü ve aday havuzu

Her katman için araştırma başlamadan resmî şirket listeleri, doğrudan platform/yayıncı dizinleri ve yetkili sektör kaynaklarından bir aday havuzu oluşturulur. Havuzdaki her kayda kaynak, bölge, ölçek, durum ve uygunluk gerekçesi eklenir. Kolay bulunan ya da tanınmış kayıtları elle seçmek yerine:

1. uygun kayıtlar bölge ve ölçek hücrelerine ayrılır;
2. her hücrede kaynak kalitesi ve aktiflik kanıtı yeterli kayıtlar listelenir;
3. seçim sırası önceden kaydedilmiş deterministik `Candidate Pool Order` ile uygulanır;
4. dışlama ve ikame gerekçeleri append-only seçim günlüğünde korunur;
5. pilot sonrası dağılım sapması ölçülmeden ana örnekleme geçilmez.

Bu amaçlı ve katmanlı örneklemdir; rastgele veya eksiksiz piyasa sayımı olarak sunulamaz.

## Bölgesel dengeleme

Her birincil katmanda en az şu gruplar izlenir: Kuzey Amerika; Avrupa; Türkiye ve yakın bölge; Doğu Asya; Güney ve Güneydoğu Asya; diğer bölgeler.

Kesin bölge kotası kaynak verisi görülmeden uydurulmaz. Pilot sonunda `market-research-director`, elde edilebilir aday havuzuna göre gerekçeli uygulanabilir dağılım önerir. Başlangıç yoğunlaşma kontrolü olarak tek bir bölgenin toplam 200 kaydın `%40`ından fazlasına çıkması veya temsil edilebilir bir bölgenin sıfır kalması inceleme tetikler. Bu sınır kota değildir; görünürlük yanlılığını yakalayan bir alarm eşiğidir. Aşım, kaynak erişimi ve pazar kapsamıyla gerekçelendirilir; `quality-assurance-director` ya da bağımsız reviewer onayı olmadan kabul edilmez.

## Ölçek dengelemesi

`BÜYÜK`, `ORTA`, `KÜÇÜK/BAĞIMSIZ` ayrı izlenir. Ölçek sınıfı çalışan sayısı, yayın hacmi, finansal büyüklük veya resmî şirket tanımından en az birine bağlanır; ölçüt, tarih ve Source ID yazılır. Veri yoksa `SCALE_UNKNOWN` kullanılır, tahmin gerçek gibi sunulmaz. Büyük/tanınmış marka yoğunlaşması pilot QA'da ayrıca incelenir; küçük/bağımsız havuz yalnız erişim kolaylığı nedeniyle azaltılamaz.

## Seçim yanlılığı sicili

Araştırma başlamadan ve pilot sonrasında şu riskler ayrı satırlarda tutulur: tanınmış marka yanlılığı; İngilizce kaynak yanlılığı; hayatta kalma yanlılığı; platform yanlılığı; bölgesel yanlılık; yalnız başarılı örnekleri seçme yanlılığı; güncel veri erişimi yanlılığı.

Her satırın zorunlu alanları: `Bias ID`, `Risk`, `Etkilenen katman`, `Azaltma yöntemi`, `Kalan risk`, `Güven düzeyi`, `Owner`, `Review Date`, `Reviewer`. Açık bias kaydı olmadan pilot `PASS` alamaz.

## Durum taksonomisi ve kanıt

Her kuruluş/marka aşağıdaki tek güncel durum değerini taşır:

- `ACTIVE`: bağımsız faaliyetini sürdüren ve aktiflik eşiğini karşılayan marka.
- `ACTIVE_REBRANDED`: rebrand sonrasında yeni kimlikle aktif olan marka.
- `ACQUIRED_ACTIVE`: satın alınmış fakat faaliyetini sürdüren marka.
- `DORMANT`: doğrulanmış güncel faaliyet sinyali yetersiz, resmî kapanış yok.
- `CLOSED`: kapanışı veya faaliyet sonu resmî/birincil kaynakla doğrulanmış.
- `DEFUNCT`: tüzel/marka varlığının sona erdiği yetkili kaynakla doğrulanmış.
- `HISTORICAL_REBRAND_CASE`: tarihsel rebrand olay kaydı; aktiflik statüsü değildir.
- `HISTORICAL_NAMING_PROBLEM_CASE`: tarihsel isim/marka sorunu olay kaydı; aktiflik statüsü değildir.
- `STATUS_UNCERTAIN`: çelişkili veya yetersiz kanıt; escalation zorunlu.

Her durum en az bir resmî/birincil kanıt, varsa ikinci bağımsız sinyal, doğrulama tarihi, karar gerekçesi ve reviewer taşır. `ACTIVE`, `ACTIVE_REBRANDED` ve `ACQUIRED_ACTIVE` ana aktif katmanlara girebilir; son ikisi ayrıca sahiplik/rebrand etiketi taşır. `DORMANT`, `CLOSED`, `DEFUNCT` ve `STATUS_UNCERTAIN` aktif kotaya giremez.

## Aktif ve tarihsel kohortların sayımı

Ana 200 toplamı sekiz birincil katmanın toplamıdır. İlk altı katmandaki 180 kayıt kuruluş/marka sayım birimidir; son iki katmandaki 20 kayıt ayrı tarihsel olay sayım birimidir. Tarihsel olayın öznesi bugün aktif olabilir veya olmayabilir; olayın dahil edilmesi güncel aktiflik eşiğine bağlı değildir.

Aynı marka hem aktif kuruluş analizinde hem tarihsel vaka analizinde yer alırsa iki farklı kimlik kullanılır: `Sample ID` kuruluşu, `Event ID` olayı gösterir. Ancak 200 kotasında yalnız bir `Counted Sample ID` seçilir. Aktif katmanda sayılan kuruluşun tarihsel olayı ikincil analiz etiketi olarak bağlanır ve tarihsel 10'luk kotada tekrar sayılmaz; tarihsel kotada sayılan olayın güncel kuruluş kaydı bağlamsal kayıt olur ve aktif kotada tekrar sayılmaz. Böylece analitik bağ korunur, çift sayım engellenir.

## Aktiflik eşiği

Aktif kotaya girecek kayıt son doğrulama tarihinde en az iki bağımsız faaliyet sinyali taşır ve bunlardan en az biri resmî/birincil kaynaktır: doğrulanmış resmî web sitesi; 2021-01-01–2026-08-08 döneminde ürün, büyük güncelleme veya yayın faaliyeti; doğrudan resmî mağaza/platform profili; yetkili şirket/marka kaydı. Tek sosyal medya hesabı yeterli değildir.

## Kota escalation

Bir kota tamamlanamazsa:

1. hedef, bulunan sayı ve eksik sayı kaydedilir;
2. kaynak/veri eksikliği ve yapılan aramalar belgelenir;
3. komşu katmandan sessiz aktarım yapılmaz;
4. ayrı alternatif aday havuzu oluşturulur;
5. `market-research-director` seçenekleri ve yanlılık etkisini içeren gerekçeli öneri hazırlar;
6. `quality-assurance-director` veya yöntem paketinin yazarı olmayan bağımsız reviewer yazılı onay vermeden kota değişmez;
7. kabul/ret ve gerekçe Decision ID'ye bağlı değişiklik kaydına işlenir.

Çözülmemiş eksik kota `INCOMPLETE` veya `ESCALATED` olur; araştırma paketi 200 tamamlanmış gibi sunulamaz ve proposal'a geçemez.

## Kayıt başına zorunlu alanlar

`Sample ID`, varsa `Event ID`, `Counted Sample ID`, kaynakta aynen ad, birincil katman, ikincil etiketler, durum, durum kanıtı, durum doğrulama tarihi, ülke/bölge, ölçek ve ölçütü, sahiplik, faaliyet türü, aktiflik sinyalleri, Source ID'ler, dahil/dışla/escalate sonucu, atama gerekçesi, güven, reviewer ve `Related Sample/Event ID`.

## Kalite kontrolü

`market-research-director` seçim paketini üretir. Brand Strategy yalnız kabul edilmiş örneklemi yorumlar; Localization ve Legal/IP ayrı içerik hatlarında kalır. QA; toplamları, tek birincil sayımı, durum ayrımını, bias sicilini, bölge/ölçek sapmasını ve kota escalation kayıtlarını bağımsız re-audit öncesinde doğrular. Bu çerçeve kendi çalışmasını `APPROVED` ilan etmez.
