// Slavnem @2024-07-21
// Zamanlayıcı İşlemleri

// Kütüphaneler
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/TypeDefine.h"
#include "../lib/Macro.h"
#include "../lib/Time.h"
#include "../lib/OsSupport.h"
#include "../lib/Shutdown.h"
#include "../lib/FileSteam.h"

/* İşletim Sistemi Zamanlayıcısı
    İşletim sistemine göre zamanlayıcı miktarları farklı olabiliryor.
    Windows da saniye cinsinden süre geçerliyken Linux de dakika
    cinsinden kullanılıyor bu yüzden ikisinde de aynı süreyi kullanamayız,
    bunun içinde uyumluluk sağlayan fonksiyon ile işletim sistemine göre
    özel olarak hesaplamakta
*/
static intmax_s static_shutdown_timer_calculate(intmax_s time)
{
    // Eğer geçersiz süre iseoşsa
    if(time < 1) return 0;

    // Windows için
    #if _WIN32 || _WIN64
        return (intmax_s)(time * 60);
    // Linux için
    #elif __linux__
        return (intmax_s)(time);
    #endif

    // bilinmeyen
    return 0;
}

extern intmax_s shutdown_timer_calculate(intmax_s time)
{
    // yerel fonksiyonu çalıştır
    return static_shutdown_timer_calculate(time);
}

/* Zamanlayıcı
    Hazırlanmış verileri kullanarak yeni bir zamanlayıcı oluşturmak
    fakat değerleri ve komutu kontrol ettikten sonra ancak işlem
    yapılabilir ve önceden zamanlayıcı olup olmadığını bilmediğimiz için
    otomatik olarak zamanlayıcı iptal etme çalışır ve sonrasında gerçek
    zamanlayıcı kodu çalışır, zaten zamanlayıcı hali hazırda yoksa eğer,
    boş bir kod döndürücektir iptal işleminde ve bu bize sorun oluşturmuyor
    eğer kod girilmeden sadece sayı girilmişse, biz bunu bilgisayar kapatma
    olarak varsayacağız ve süreyi kullanarak işlem yapacağız
*/
static const ESHCODE static_shutdown_timer(string_s command, intmax_s time, boolean console)
{
    // geçerli komut ve zaman boşsa
    if(ISNULL(command) && time < 1)
    {
        #ifdef __DEBUG_SHUTDOWN__
            printf("\n%s\n", "* Zamanlayici Icin Girdiginiz Degerler Gecersiz *");
        #endif

        // geçersiz komut hatası
        return ESH_CODE_ERR_INVALID;
    }
    
    #ifdef __DEBUG_SHUTDOWN__
        printf("\n%s\n", "* Zamanlayici Icin Girdiginiz Degerler Gecerli *");
    #endif

    intmax_s calcTime = shutdown_timer_calculate(time); // işletim sistemine göre süre
    char buffer[256]; // geçici olarak kapatma komutu tutucu

    // sistemi kapat
    if((ISNULL(command) && time > 0) || strcmp(command, __SHUTDOWN_COMM_OFF__) == 0)
        // sistemi kapat
        snprintf(buffer, SAFESIZESTR(buffer), "%s " __INTMAX_FORMAT__, __SHUTDOWN_OFF__, calcTime);
    // yeniden başlat
    else if(strcmp(command, __SHUTDOWN_COMM_RESTART__) == 0)
        // yeniden başlat
        snprintf(buffer, SAFESIZESTR(buffer), "%s " __INTMAX_FORMAT__, __SHUTDOWN_RESTART__, calcTime);
    // uyku
    else if(strcmp(command, __SHUTDOWN_COMM_SLEEP__) == 0)
        snprintf(buffer, SAFESIZESTR(buffer), "%s", __SHUTDOWN_SLEEP__);
    // kilitle
    else if(strcmp(command, __SHUTDOWN_COMM_LOCK__) == 0)
        snprintf(buffer, SAFESIZESTR(buffer), "%s", __SHUTDOWN_LOCK__);
    // iptal et
    else if(strcmp(command, __SHUTDOWN_COMM_CANCEL__) == 0)
        snprintf(buffer, SAFESIZESTR(buffer), "%s", __SHUTDOWN_CANCEL__);
    else
    {
        #ifdef __DEBUG_SHUTDOWN__
            printf("\n%s\n", "* Bilinmeyen Zamanlama Komutu Girildi *");
        #endif

        // geçersiz, bilinmeyen komut
        return ESH_CODE_ERR_UNKNOWNCOMM;
    }

    #ifdef __DEBUG_SHUTDOWN__
        printf("\n%s\n", "* Gecerli Komut Algilandi *");
    #endif

    // zamanlayıcı komutunu çalıştırsın ve tutsun
    system(buffer);

    #ifdef __DEBUG_SHUTDOWN__
        printf("\n%s\n", "* Zamanlayici Komutu Calistirildi *");
    #endif

    // dosyaya veri kaydetmek için dosya yapısı işaretçisi
    MyFile logFile = {
        .addr = NULL,
        .path = __SHUTDOWN_LOG_FILEPATH__,
        .otype = EFS_OTYPE_ADD
    };

    // dosya açma kontrolü
    file_opener(&logFile, logFile.path, logFile.otype);

    // dosya açma durumu kontrolü
    #ifdef __DEBUG_SHUTDOWN__
        if(file_status(&logFile) == EFS_CODE_STAT_FREE)
            printf("\n%s\n", "* Dosya Acilamadigi Icin Dosyaya Zamanlayici Islem Sonucu Kayit Edilmeyecek *");
        else
            printf("\n%s\n", "* Dosya Basariyla Acildi *");
    #endif

    // dosya açma başarısız ise direk kod dönsün
    if(file_status(&logFile) == EFS_CODE_STAT_FREE)
        return ESH_CODE_MSG_TIMERRUNNED; // zamanlayıcı çalıştırıldı

    // metini tutacak geçici değişken
    char tempText[512];

    // kaydedilecek metini ayarlamak
    snprintf(tempText,
        SAFESIZESTR(tempText),
        "[ %s ][%s: %s][%s: %0u%s %02u%s %02u%s][%s: %s][%s: %s]\n",
        (console ? "console" : "screen"), // konsoldan mı yoksa ekrandan mı çalıştı?
        "command", command, // komut metini
        "time", // süre
            MINUTETODAY(time), "D", // gün
            CLOCKHOUR(MINUTETOHOUR(time)), "Hr", // saat
            CLOCKMINUTE(time), "Min", // dakika
        "os", __OS__, // işletim sistemi
        "env", myDesktopEnv() // masaüstü ortamı
    );

    // dosyaya veri yazdırmak
    file_write(&logFile, tempText);

    // dosyayı kapatsın
    file_closer(&logFile);

    #ifdef __DEBUG_SHUTDOWN__
        printf("\n%s\n", "* Dosyaya Zamanlayici İslem Sonucu Kaydedildi *");
    #endif

    // zamanlayıcı çalıştırıldı başarı kodu
    return ESH_CODE_MSG_TIMERRUNNED;
}

extern const ESHCODE shutdown_timer(string_s command, intmax_s time, boolean console)
{
    // yerel fonksiyonu kullan
    return static_shutdown_timer(command, time, console);
}