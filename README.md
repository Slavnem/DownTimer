<!-- Başlık -->
# 🚀 Down Timer

<!-- Son Sürüm Kodu -->
## Version v1.2

<!-- C Standartı -->
## C11 Standard

<!-- İşletim Sistemleri ve Derleme -->
<pre>
Linux -> GCC
    Linux x86 Debug Msg: gcc -m32 src/main.c src/core/*.c -o build/linux/DownTimerLx86_Debug
    Linux x64 Debug Msg: gcc -m64 src/main.c src/core/*.c -o build/linux/DownTimerLx64_Debug

    Linux x86 Release: gcc -m32 src/main.c src/core/*.c -o build/linux/DownTimerLx86_Release -D __COMPILE_RELEASE_MODE__
    Linux x64 Release: gcc -m64 src/main.c src/core/*.c -o build/linux/DownTimerLx64_Release -D __COMPILE_RELEASE_MODE__
</pre>

<!-- Bilgilendirme -->
<pre>
! UYARI !
TR: Linux da "sudo" yönetici olmanız gerekebilir ve bu durumda
yönetici olmadığınızda yetki izni olmazsa eğer, işlemler yapılamayabilir ya da
yapılan işlemler dosyaya kaydedilemeyebilir

! ALERT !
EN: You may need to be a “sudo” administrator in Linux and in this case
if there is no authorization permission when you are not an administrator, operations may not be performed or
operations may not be saved in the file
</pre>

<!-- Komutlar -->
<pre>
Kapat | Off: -of "sudo?"
Yeniden Başlat | Restart: -re "sudo?"
Uyku | Sleep: -sl "sudo?"
Kilitle | Lock: -lo "not sudo" "xdg-screensaver install"
İptal Et | Cancel: -ca "sudo?"
</pre>

## Linux Masaüstü Destek | Linux Desktop Support
<pre>
GNOME [Çok Az Test Edildi | Very Little Tested]
KDE [Test Edilmedi | Not Tested]
XFCE [Test Edilmedi | Not Tested]
MATE [Test Edilmedi | Not Tested]
</pre>

<!-- Açıklama Türkçe -->
# 📝 Özellikler
+ Konsoldan Girilebilecek Komutlar İle Çalıştırma
+ Uygulama İçi Çalıştırma
+ Sistemi Kendi Zamanlayıcı Sistemini Kullanarak Zamanlayıcı Oluşturma
+ Sadece Linux x86/64 Desteğine Ayarlandı
+ Yapılan Zamanlama İşlemleri Sonuçlarının Bilgisini İçeren Kayıt Dosyası

<!-- Comment English -->
# 📝 Features
+ Run with commands that can be entered from the console
+ In-App Run
+ System Creating a Timer Using Own Timer System
+ Set to Support Linux x86/64 Only
+ Record File Containing Information on the Results of Scheduling Operations