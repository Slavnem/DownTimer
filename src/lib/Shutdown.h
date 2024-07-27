// Slavnem @2024-07-21
// Zamanlayıcı Tanımı
#ifndef DT_SHUTDOWN_H
#define DT_SHUTDOWN_H

// Kütüphaneler
#include "CompileConf.h"
#include "OsSupport.h"
#include "TypeDefine.h"

// Zamanlayıcı Ayarlama Hata Ayıklayıcısı Belirteci
#ifndef __COMPILE_RELEASE_MODE__
    #define __DEBUG_MSG_SHUTDOWN__
#endif

// İşletim sistemine göre zamanlı kapatıcı desteği
#ifdef __linux__
    #define __SHUTDOWN_OFF__ "shutdown -c && shutdown -h"
    #define __SHUTDOWN_RESTART__ "shutdown -c && shutdown -r"
    #define __SHUTDOWN_SLEEP__ "systemctl suspend"
    #define __SHUTDOWN_LOCK__ "xdg-screensaver lock"
    #define __SHUTDOWN_CANCEL__ "shutdown -c"
#endif

// Konsol/Terminalden Hangi İşlemi Yapacağını Sağlayan Komutlar
// Ana Komutların İlk 2 Harfi Kullanılıyor
#define __SHUTDOWN_COMM_OFF__ "-of"
#define __SHUTDOWN_COMM_RESTART__ "-re"
#define __SHUTDOWN_COMM_SLEEP__ "-sl"
#define __SHUTDOWN_COMM_LOCK__ "-lo"
#define __SHUTDOWN_COMM_CANCEL__ "-ca"

// İşlem Kayıt Dosyası Adı
#define __SHUTDOWN_LOG_FILEPATH__ "DownTimer.log"

// Ana Komutların Kodları
typedef enum
{
    // Komutlar
    ESH_COMM_OFF = 10, // kapat
    ESH_COMM_RESTART, // yeniden başlat
    ESH_COMM_SLEEP, // uyku
    ESH_COMM_LOCK, // kilitle
    ESH_COMM_CANCEL // iptal et
} eshcomm;

typedef const eshcomm eshcomm_s;

// Zamanlayıcı İşlem Kodları
typedef enum
{
    // Hatalar
    ESH_CODE_ERR_TIMENULL = 0, // geçerli süre yok
    ESH_CODE_ERR_COMMNULL, // işlem komutu boş
    ESH_CODE_ERR_UNKNOWN, // bilinmeyen
    ESH_CODE_ERR_INVALID, // geçersiz
    ESH_CODE_ERR_UNKNOWNCOMM, // bilinmeyen komut
    ESH_CODE_ERR_COMMRUNFAIL, // komut çalışma hatası
    ESH_CODE_ERR_NEWOBJECT, // yeni obje oluşturma hatası

    // Argüman Değerler
    ESH_CODE_VAL_OFF = 1000, // bilgisayarı kapat
    ESH_CODE_VAL_RESTART, // yeniden başlat
    ESH_CODE_VAL_SLEEP, // uyku modu
    ESH_CODE_VAL_LOCK, // kilitle
    ESH_CODE_VAL_NULL, // boş

    // Başarılı Durum Mesajlar
    ESH_CODE_MSG_TIMERCREATED = 2000, // zamanlayıcı oluşturuldu
    ESH_CODE_MSG_TIMEREND, // zamanlayıcı bitti
    ESH_CODE_MSG_TIMERRUNNED, // zamanlayıcı çalıştı

    // Durumlar
    ESH_CODE_STAT_RUNNING = 3000, // zamanlayıcı çalışıyor
    ESH_CODE_STAT_WAITING, // zamanlayıcı bekleniyor
    ESH_CODE_STAT_FREE, // zamanlayıcı boş
    ESH_CODE_STAT_ENDED, // zamanlayıcı bitti
    ESH_CODE_STAT_TERMINATED // zamanlayıcı elle sonlandırıldı
} eshcode;

typedef const eshcode eshcode_s;

// Fonksiyon Prototipleri
extern intmax_s shutdown_timer_calculate(intmax_s time);
extern boolean_s shudown_execute(string_s command);
extern eshcode_s shutdown_timer(string_s command, intmax_s time, boolean console);

#endif