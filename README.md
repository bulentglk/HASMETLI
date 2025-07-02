HAŞMETLİ 2D PLATFORM OYUNU
Bu proje, temel olarak bir 2D platform oyunu geliştirme denemesi için hazırlandı. SDL2 kütüphanesi ve C++ dili kullanılarak baştan sona elle yazıldı. Oyunun amacı, menüden başlayarak bir karakteri yönlendirmek, platform üzerinde sağa-sola hareket ettirmek, zıplatmak, puan toplamak ve engellerden (çivilerden) kaçmaktır.

Proje aşama aşama geliştirildi. İlk olarak menü ekranı, ardından karakter kontrolleri, zıplama mekaniği, çarpışma kontrolleri, kamera ve parallax arkaplan özellikleri eklendi. Son aşamada ise ESC tuşu ile pause (durdurma) sistemi entegre edildi.

✨ Özellikler
✅ Başlangıç menüsü (oyun başlığı, talimatlar)
✅ Boşluk tuşu ile oyuna geçiş
✅ ESC ile oyunu istediğiniz anda durdurma ve ekrana “OYUN DURDURULDU” yazdırma
✅ Sağ/sol ok tuşları ile yatay hareket
✅ Yukarı ok tuşu ile zıplama
✅ Çivili zeminlere basınca can azaltma ve can bitince öldün ekranı gösterme
✅ Skor takibi (her sağ tuşla ilerlemede +1)
✅ FPS için basit deltaTime yönetimi
✅ Parallax arka plan katmanları (bina, sis, props vs.)
✅ Kamera hareketi (oyuncu ekranın merkezine sabitleniyor)
✅ SDL_ttf ile yazı yazdırma desteği
✅ SDL_image ile PNG desteği

🎮 Kontroller
Sağ / Sol ok tuşları → hareket

Yukarı ok tuşu → zıplama

ESC → oyunu durdur

Boşluk (SPACE) → menüden oyuna başla

🧩 Kod Dosyaları
main.cpp
SDL başlatma ve pencere oluşturma

font yükleme (pixel_font.ttf veya OpenSans.ttf)

menü döngüsü:

menüde başlık ve alt yazılar

SPACE tuşu dinleniyor

oyun döngüsü:

olay kontrolü (klavye eventleri)

kamera takibi

FPS hesaplama (deltaTime)

çarpışma kontrolleri

yerçekimi yönetimi

oyuncunun animasyon frameleri

puan yazdırma

can sayısını ekranda yazdırma

pause sistemi (ESC basınca OYUN DURDURULDU yazısı)

cleanup (SDL_Destroy komutları)

assets/
backgrounds → Base_Color, Buildings, Props, Mid_Fog, Frontal_Fog katmanları

fonts → pixel_font.ttf, OpenSans.ttf

player.png → karakter sprite

spike.png → çivi sprite

öldün.png → öldüğünde gösterilen ekran

stone.png → yer döşemesi

🛠️ Teknolojiler
C++

SDL2 (grafikler)

SDL_ttf (yazı tipleri)

SDL_image (PNG desteği)

🗓️ Geliştirme Durumu
Bu proje öğrenme ve ödev amacıyla yazılmıştır. Kodlar şuan basit bir platform mantığını göstermek üzerine kurgulanmıştır, ancak ileride:

yeni bölümler

farklı düşman tipleri

skor tablosu

müzik ve ses efektleri

ayar menüsü

checkpoint / save sistemi

eklenerek genişletilecektir. Yani proje aktif olarak geliştirilmeye devam edilecektir.

📌 Lisans
Bu proje özel bir çalışma olduğu için, izinsiz paylaşılması veya dağıtılması yasaktır. Kodlar sadece proje sahibi (ben) tarafından kullanılabilir.

## Katkıda Bulunma

Proje şu anda ders projesi olarak geliştiriliyor.  
Katkılara açık değildir.


## Kurulum

1. Projeyi klonlayın:
2. Visual Studio 2022 ile açın
3. `main.cpp` derleyin ve çalıştırın

## Ekran Görüntüsü
![image](https://github.com/user-attachments/assets/f27b4f9f-b0a8-4c3f-837c-5cfbc9c124a8)
![image](https://github.com/user-attachments/assets/bb922209-26d6-460a-8e8c-eb60fd23515a)

## Lisans

Tüm hakları saklıdır. Bu proje sadece proje sahibi tarafından kullanılabilir. İzinsiz kopyalanamaz, dağıtılamaz veya değiştirilemez.




