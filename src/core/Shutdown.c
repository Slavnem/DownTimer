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
static const ESHCODE static_shutdown_timer(string_s command, uint32_s time, boolean console)
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

    // geçici olarak kapatma komutu tutucu
    char buffer[256];

    // sistemi kapat
    if((ISNULL(command) && time > 0) || strcmp(command, __SHUTDOWN_COMM_OFF__) == 0)
    {
        // geçici metine kaydet
        snprintf(buffer,
            SAFESIZESTR(buffer), // güvenli şekilde desteklediği boyut
            "%s %u", __SHUTDOWN_OFF__, TIMEOS(time) // sistemi kapat
        );
    }
    // yeniden başlatma
    else if(strcmp(command, __SHUTDOWN_COMM_RESTART__) == 0)
    {
        snprintf(buffer,
            SAFESIZESTR(buffer),
            "%s %u", __SHUTDOWN_RESTART__, TIMEOS(time) // yeniden başlatma
        );
    }
    // uyku
    else if(strcmp(command, __SHUTDOWN_COMM_SLEEP__) == 0)
    {
        snprintf(buffer,
            SAFESIZESTR(buffer),
            "%s", __SHUTDOWN_SLEEP__ // uyku modu
        );
    }
    // kitleme
    else if(strcmp(command, __SHUTDOWN_COMM_LOCK__) == 0)
    {
        snprintf(buffer,
            SAFESIZESTR(buffer),
            "%s", __SHUTDOWN_LOCK__ // kilitleme
        );
    }
    // iptal etme
    else if(strcmp(command, __SHUTDOWN_COMM_CANCEL__) == 0)
    {
        snprintf(buffer,
            SAFESIZESTR(buffer),
            "%s", __SHUTDOWN_CANCEL__ // iptal etme
        );
    }
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
        "env", __OS_DESKTOP_ENV__ // masaüstü ortamı
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

extern ESHCODE shutdown_timer(string_s command, uint32_s time, boolean console)
{
    // yerel fonksiyonu kullan
    return static_shutdown_timer(command, time, console);
}