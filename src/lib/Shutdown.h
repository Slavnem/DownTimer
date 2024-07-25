// Slavnem @2024-07-21
// Zamanlayıcı Tanımı
#ifndef DT_SHUTDOWN_H
#define DT_SHUTDOWN_H

// Kütüphaneler
#include "OsSupport.h"
#include "TypeDefine.h"

// Zamanlayıcı Ayarlama Hata Ayıklayıcısı Belirteci
// #define __DEBUG_SHUTDOWN__

// İşletim sistemine göre zamanlı kapatıcı desteği
// Windows 32 bit ve 64 bit desteği
#if defined(_WIN32) || defined(_WIN64)
    #define __SHUTDOWN_OFF__ "shutdown /a && shutdown /s /t"
    #define __SHUTDOWN_RESTART__ "shtudown /a && shutdown /r /t"
    #define __SHUTDOWN_SLEEP__ "shutdown /a && rundll32.exe powrprof.dll,SetSuspendState 0,1,0"
    #define __SHUTDOWN_LOCK__ "shutdown /a && rundll32.exe user32.dll,LockWorkStation"
    #define __SHUTDOWN_CANCEL__ "shutdown /a"

    // İşletim Sistemine Göre Zaman Hesaplayıcı
    #define TIMEOS(time) (time < 1 ? 0 : ((uint16_s)time * 60))
// Linux
#elif defined(__linux__)
    #define __SHUTDOWN_OFF__ "shutdown -h"
    #define __SHUTDOWN_RESTART__ "shutdown -r"
    #define __SHUTDOWN_SLEEP__ "systemctl suspend"
    #define __SHUTDOWN_LOCK__ "xdg-screensaver lock"
    #define __SHUTDOWN_CANCEL__ "shutdown -c"

    // Zaman Hesaplayıcı
    #define TIMEOS(time) (time < 1 ? 0 : (uint32_s)time)
// Bilinmeyen İşletim Sistemi
#else
    #error "Only Linux, Windows x86 And x64's Supporting. Unsupported OS Error!"
#endif

// Konsol/Terminalden Hangi İşlemi Yapacağını Sağlayan Komutlar
// Ana Komutların İlk 2 Harfi Kullanılıyor
#define __SHUTDOWN_COMM_OFF__ "-of"
#define __SHUTDOWN_COMM_RESTART__ "-re"
#define __SHUTDOWN_COMM_SLEEP__ "-sl"
#define __SHUTDOWN_COMM_LOCK__ "-lo"
#define __SHUTDOWN_COMM_CANCEL__ "-ca"

// İşlem Kayıt Dosyası Adı
#define __SHUTDOWN_LOG_FILEPATH__ "downtimer.log"

// Ana Komutların Kodları
typedef enum
{
    // Komutlar
    ESH_COMM_OFF = 10, // kapat
    ESH_COMM_RESTART, // yeniden başlat
    ESH_COMM_SLEEP, // uyku
    ESH_COMM_LOCK, // kilitle
    ESH_COMM_CANCEL // iptal et
} ESHCOMM;

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
} ESHCODE;

// Fonksiyon Prototipleri
extern const ESHCODE shutdown_timer(string_s command, uint32_s time, boolean console);

#endif