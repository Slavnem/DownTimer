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
    // Eğer geçersiz süre ise 0 dönsün
    if(time < 1) return 0;

    // Linux için
    #ifdef __linux__
        #ifdef __DEBUG_MSG_SHUTDOWN__
            DEBUG_PRINT_MSG(__DEBUG_MSG_SHUTDOWN_TITLE__, "Linux'e Gore Sure Ayarlandi");
        #endif
        return (intmax_s)(time);
    #endif

    // bilinmeyen işletim sistemi var
    #ifdef __DEBUG_MSG_SHUTDOWN__
        DEBUG_PRINT_ERROR(__DEBUG_MSG_SHUTDOWN_TITLE__, "Desteklenmeyen Isletim Sistemi Kullaniliyor");
    #endif
    return 0;
}

extern intmax_s shutdown_timer_calculate(intmax_s time)
{
    // yerel fonksiyonu çalıştır
    return static_shutdown_timer_calculate(time);
}

/* Kapatma Komutu Çalıştırıcı
    El ile işlem yapmak yerine bir fonksiyona bağlayarak
    işlem yapmak daha iyi bir seçecek olucaktır ve fonksiyonun
    döndürdüğü değere göre daha kolay işlem yapılabilir
*/
static boolean_s static_shutdown_execute(string_s command)
{
    // komut boşsa hata dönsün
    if(ISNULL(command))
    {
        #ifdef __DEBUG_MSG_SHUTDOWN__
            DEBUG_PRINT_ERROR(__DEBUG_MSG_SHUTDOWN_TITLE__, "Komut Yok");
        #endif
        return FALSE;
    }

    #ifdef __DEBUG_MSG_SHUTDOWN__
        DEBUG_PRINT_MSG(__DEBUG_MSG_SHUTDOWN_TITLE__, "Komut Calistiriliyor");
    #endif

    // komutu çalıştırsın
    int32_s result = system(command);

    #ifdef __DEBUG_MSG_SHUTDOWN__
        !result ?
            DEBUG_PRINT_MSG(__DEBUG_MSG_SHUTDOWN_TITLE__, "Komut Basariyla Calisti")
            : DEBUG_PRINT_WARNING(__DEBUG_MSG_SHUTDOWN_TITLE__, "Komut Calistirilamadi");
    #endif

    // komut sonucuna göre değer döndürsün
    // 0 değilse komut çalışması başarısız olmuştur
    return (boolean)!result;
}

extern const boolean shudown_execute(string_s command)
{
    // yerel fonksiyonu çalıştır
    return static_shutdown_execute(command);
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
static eshcode_s static_shutdown_timer(string_s command, intmax_s time, boolean console)
{
    // geçerli komut ve zaman boşsa
    if(ISNULL(command) && time < 1)
    {
        #ifdef __DEBUG_MSG_SHUTDOWN__
            DEBUG_PRINT_ERROR(__DEBUG_MSG_SHUTDOWN_TITLE__, "Zamanlayici Icin Girdiginiz Degerler Gecersiz");
        #endif
        return ESH_CODE_ERR_INVALID; // geçersiz komut hatası
    }
    
    #ifdef __DEBUG_MSG_SHUTDOWN__
        DEBUG_PRINT_MSG(__DEBUG_MSG_SHUTDOWN_TITLE__, "Zamanlayici Icin Girdiginiz Degerler Gecerli");
    #endif
    
    // hesaplanmış zaman
    uintmax_s calcTimer = static_shutdown_timer_calculate(time);

    // geçici olarak kapatma komutu tutucu
    char sysCommand[128];

    // sistemi kapat
    if(strcmp(command, __SHUTDOWN_COMM_OFF__) == 0)
        snprintf(sysCommand, SAFESIZESTR(sysCommand), "%s %lu", __SHUTDOWN_OFF__, calcTimer);
    // yeniden başlat
    else if(strcmp(command, __SHUTDOWN_COMM_RESTART__) == 0)
        snprintf(sysCommand, SAFESIZESTR(sysCommand), "%s %lu", __SHUTDOWN_RESTART__, calcTimer);
    // uyku
    else if(strcmp(command, __SHUTDOWN_COMM_SLEEP__) == 0)
        snprintf(sysCommand, SAFESIZESTR(sysCommand), "%s", __SHUTDOWN_SLEEP__);
    // kilitle
    else if(strcmp(command, __SHUTDOWN_COMM_LOCK__) == 0)
        snprintf(sysCommand, SAFESIZESTR(sysCommand), "%s", __SHUTDOWN_LOCK__);
    // iptal et
    else if(strcmp(command, __SHUTDOWN_COMM_CANCEL__) == 0)
        snprintf(sysCommand, SAFESIZESTR(sysCommand), "%s", __SHUTDOWN_CANCEL__);
    else
    {
        #ifdef __DEBUG_MSG_SHUTDOWN__
            DEBUG_PRINT_ERROR(__DEBUG_MSG_SHUTDOWN_TITLE__, "Bilinmeyen Zamanlama Komutu Girildi");
        #endif
        return ESH_CODE_ERR_UNKNOWNCOMM; // geçersiz, bilinmeyen komut
    }

    #ifdef __DEBUG_MSG_SHUTDOWN__
        DEBUG_PRINT_MSG(__DEBUG_MSG_SHUTDOWN_TITLE__, "Gecerli Komut Algilandi");
        DEBUG_PRINT_MSG(__DEBUG_MSG_SHUTDOWN_TITLE__, sysCommand);
    #endif

    // komutu çalıştırsın ve tutsun
    const boolean resultCommand = shudown_execute(sysCommand);

    // dosyaya veri kaydetmek için dosya yapısı işaretçisi
    MyFile logFile = {
        .addr = NULL,
        .path = __SHUTDOWN_LOG_FILEPATH__,
        .otype = EFS_OTYPE_ADD
    };

    // dosya açma kontrolü
    file_opener(&logFile, logFile.path, logFile.otype);

    // dosya açma durumu kontrolü
    #ifdef __DEBUG_MSG_SHUTDOWN__
        file_status(&logFile) == EFS_CODE_STAT_FREE ?
            DEBUG_PRINT_WARNING(__DEBUG_MSG_SHUTDOWN_TITLE__, "Dosya Acilamadigi Icin Dosyaya Zamanlayici Islem Sonucu Kayit Edilmeyecek")
            : DEBUG_PRINT_MSG(__DEBUG_MSG_SHUTDOWN_TITLE__, "Dosya Basariyla Acildi");
    #endif

    // dosya açma başarısız veya çalıştırılma başarılı ise direk kod dönsün
    if(file_status(&logFile) == EFS_CODE_STAT_FREE && resultCommand) return ESH_CODE_MSG_TIMERRUNNED; // zamanlayıcı çalıştırıldı
    else if(!resultCommand) return ESH_CODE_ERR_COMMRUNFAIL; // zamanlayıcı çalışamadı

    // metini tutacak geçici değişken
    char tempText[512];

    // kaydedilecek metini ayarlamak
    snprintf(tempText, SAFESIZESTR(tempText),
        "[ %s ][%s: %s][%s: %0u%s %02u%s %02u%s][%s: %s][%s: %s][%s: %s][%s: %s]\n",
        (console ? "console" : "screen"), // konsoldan mı yoksa ekrandan mı çalıştı?
        "command", command, // komut metini
        "time", // süre
            MINUTETODAY(time), "D", // gün
            CLOCKHOUR(MINUTETOHOUR(time)), "Hr", // saat
            CLOCKMINUTE(time), "Min", // dakika
        "success", (resultCommand ? "true" : "false"),
        "arch", __OS_ARCH_NAME__, // işletim sistemi derleme türü
        "env", myDesktopEnv(), // masaüstü ortamı
        "root", (__ISROOT__ ? "true" : "false")
    );

    // dosyaya veri yazdırmak
    file_write(&logFile, tempText);

    // dosyayı kapatsın
    file_closer(&logFile);

    #ifdef __DEBUG_MSG_SHUTDOWN__
        DEBUG_PRINT_MSG(__DEBUG_MSG_SHUTDOWN_TITLE__, "Dosyaya Zamanlayici İslem Sonucu Kaydedildi");
    #endif
    return ESH_CODE_MSG_TIMERRUNNED; // zamanlayıcı çalıştırıldı başarı kodu
}

extern eshcode_s shutdown_timer(string_s command, intmax_s time, boolean console)
{
    // yerel fonksiyonu kullan
    return static_shutdown_timer(command, time, console);
}
