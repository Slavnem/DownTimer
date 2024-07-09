// Slavnem @2024-07-08
// Downtimer Shutdown
#pragma once

#ifndef DT_SHUTDOWN_H
#define DT_SHUTDOWN_H

// İşletim Sistemine Göre Zamanlayıcılar
// Windows 32 bit ya da 64 bit
#if defined(_WIN32) || defined(_WIN64)
    #define __SHUTDOWN_OFF__ "shutdown /a && shutdown /s /t"
    #define __SHUTDOWN_RESTART__ "shtudown /a && shutdown /r /t"
    #define __SHUTDOWN_SLEEP__ "shutdown /a && rundll32.exe powrprof.dll,SetSuspendState 0,1,0"
    #define __SHUTDOWN_LOCK__ "shutdown /a && rundll32.exe user32.dll,LockWorkStation"
    #define __SHUTDOWN_CANCEL__ "shutdown /a"

    // dakika için
    #define timeConverterForOs(time) (time > 0 ? (time * 60) : 0)
// Linux ise
#elif defined(__linux__)
    #define __SHUTDOWN_OFF__ "shutdown -h"
    #define __SHUTDOWN_RESTART__ "shutdown -r"
    #define __SHUTDOWN_SLEEP__ "systemctl suspend"
    #define __SHUTDOWN_LOCK__ "" // Linux için masaüstü uyumluluğuna göre yapılabilir
    #define __SHUTDOWN_CANCEL__ "shutdown -c"

    // Masaüstü Arayüzleri
    #define __DESKTOP_ENV_GNOME__ "GNOME"
    #define __DESKTOP_ENV_KDE__ "KDE"
    #define __DESKTOP_ENV_XFCE__ "XFCE"
    #define __DESKTOP_ENV_MATE__ "MATE"

    // Masaüstü Arayüzüne Göre
    #define __SHUTDOWN_GNOME_LOCK__ "gnome-screensaver-command -l"
    #define __SHUTDOWN_KDE_LOCK__ "qdbus org.freedesktop.ScreenSaver /ScreenSaver Lock"
    #define __SHUTDOWN_XFCE_LOCK__ "xflock4"
    #define __SHUTDOWN_MATE_LOCK__ "mate-screensaver-command -l"

    // dakika için
    #define timeConverterForOs(time) (time > 0 ? (time) : 0)

    // Masaüstü Ortamı Desteği
    static string_s _linuxDesktopEnvironment(void)
    {
        // kullanılan xdg arayüzünü alsın
        string_a desktop_env = getenv("XDG_CURRENT_DESKTOP");

        // boş değilse eğer, arayüz bulundu
        if(!isNull(desktop_env))
            return desktop_env;

        // arayüz oturumunu alsın
        desktop_env = getenv("DESKTOP_SESSION");

        // boş değilse arayüz bulundu, aksi halde boş
        return (!isNull(desktop_env) ? desktop_env : NULL);
    }
// desteklenmeyen bir işletim sistemi ise
#else
    #error "[DT_SHUTDOWN_H]: Unspported Operating System!\Only Supporting Windows x86/64 and Linux!\n"
#endif

// Konsol/Terminalden Hangi İşlemi Yapacağını Sağlayan Komutlar
// Ana Komutların İlk 2 Harfi Kullanılıyor
#define __SHUTDOWN_COMM_OFF__ "-of"
#define __SHUTDOWN_COMM_RESTART__ "-re"
#define __SHUTDOWN_COMM_SLEEP__ "-sl"
#define __SHUTDOWN_COMM_LOCK__ "-lo"
#define __SHUTDOWN_COMM_CANCEL__ "-ca"

// Ana Komutların Kodları
#define __SHUTDOWN_CODE_OFF__ 10
#define __SHUTDOWN_CODE_RESTART__ 11
#define __SHUTDOWN_CODE_SLEEP__ 12
#define __SHUTDOWN_CODE_LOCK__ 13
#define __SHUTDOWN_CODE_CANCEL__ 14

// Standart Kütüphaneler
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// DT Kütüphaneler
#include "dt_typedef.h"
#include "dt_macro.h"
#include "dt_filestream.h"

// DTSH Kodları
typedef enum
{
    // DTSH Hataları
    DTSH_ERR_TIMENULL = 0, // geçerli süre yok
    DTSH_ERR_COMMNULL, // işlem komutu boş
    DTSH_ERR_UNKNOWN, // bilinmeyen
    DTSH_ERR_INVALID, // geçersiz
    DTSH_ERR_UNKNOWNCOMM, // bilinmeyen komut

    // DTSH Argüman Değerleri
    DTSH_VAL_OFF = 1000, // bilgisayarı kapat
    DTSH_VAL_RESTART, // yeniden başlat
    DTSH_VAL_SLEEP, // uyku modu
    DTSH_VAL_LOCK, // kilitle
    DTSH_VAL_NULL, // boş

    // DTSH Başarılı Durum Mesajları
    DTSH_MSG_TIMERCREATED = 2000, // zamanlayıcı oluşturuldu
    DTSH_MSG_TIMEREND, // zamanlayıcı bitti

    // DTSH Durumları
    DTSH_STAT_RUNNING = 3000, // zamanlayıcı çalışıyor
    DTSH_STAT_WAITING, // zamanlayıcı bekleniyor
    DTSH_STAT_FREE, // zamanlayıcı boş
    DTSH_STAT_ENDED, // zamanlayıcı bitti
    DTSH_STAT_TERMINATED // zamanlayıcı elle sonlandırıldı
} dtshe_code;

// MY DTSH (Benim DT Kapatıcım) Yapısı
typedef struct
{
    uint32_t time; // toplam süre
    string_a command; // kapatma... komutu
} mydtsh;

typedef mydtsh* mydtshptr;
typedef const mydtshptr const mydtshptr_s;

//////////////////////////////////
// Fonksiyon Prototipleri
//////////////////////////////////
extern dtshe_code dtshf_timer(mydtshptr_s argMydtshptr);

//////////////////////////////////
// Fonksiyonlar
//////////////////////////////////
// Zamanlayıcı
/*
    Hazırlanmış verileri kullanarak yeni bir zamanlayıcı oluşturmak
    fakat değerleri ve komutu kontrol ettikten sonra ancak işlem
    yapılabilir ve önceden zamanlayıcı olup olmadığını bilmediğimiz için
    otomatik olarak zamanlayıcı iptal etme çalışır ve sonrasında gerçek
    zamanlayıcı kodu çalışır, zaten zamanlayıcı hali hazırda yoksa eğer,
    boş bir kod döndürücektir iptal işleminde ve bu bize sorun oluşturmuyor
    eğer kod girilmeden sadece sayı girilmişse, biz bunu bilgisayar kapatma
    olarak varsayacağız ve süreyi kullanarak işlem yapacağız
*/
extern dtshe_code dtshf_timer(mydtshptr_s argMydtshptr)
{
    // ikiside boşsa hata döndürsün
    if(isNull(argMydtshptr->command) && (argMydtshptr->time) < 1)
        return DTSH_ERR_INVALID;

    // geçici metin tutucu
    char buffer[128];

    // eğer metin boş ve süre de varsa, bilgisayar kapatmaya ayarla
    if(isNull(argMydtshptr->command) && (argMydtshptr)->time > 0)
    {
        // geçici metine kaydet
        snprintf(
            buffer, // geçici depolayıcı
            strArrMax(buffer), // güvenli şekilde desteklediği boyut
            "%s %zu", // değişken türleri
            __SHUTDOWN_OFF__, // sistemi kapat
            timeConverterForOs(argMydtshptr->time) // süre (dakika)
        );
    }
    // eşleştiği komuta göre ayarla
    // sistemi kapatma
    else if(strcmp(argMydtshptr->command, __SHUTDOWN_COMM_OFF__) == 0)
    {
        snprintf(
            buffer,
            strArrMax(buffer),
            "%s %zu",
            __SHUTDOWN_OFF__,
            timeConverterForOs(argMydtshptr->time)
        );
    }
    // yeniden başlatma
    else if(strcmp(argMydtshptr->command, __SHUTDOWN_COMM_RESTART__) == 0)
    {
        snprintf(
            buffer,
            strArrMax(buffer),
            "%s %zu",
            __SHUTDOWN_RESTART__,
            timeConverterForOs(argMydtshptr->time)
        );
    }
    // uyku
    else if(strcmp(argMydtshptr->command, __SHUTDOWN_COMM_SLEEP__) == 0)
    {
        snprintf(
            buffer,
            strArrMax(buffer),
            "%s",
            __SHUTDOWN_SLEEP__
        );
    }
    // kitleme
    else if(strcmp(argMydtshptr->command, __SHUTDOWN_COMM_LOCK__) == 0)
    {
        // otomatik tanımlı kod
        string_a osSupportedCommand = __SHUTDOWN_LOCK__;

        // Linux için destek
        #if defined(__linux__)
            // linux masaüstünü ortamı bilgisini alsın
            string_s linuxDesktopEnv = _linuxDesktopEnvironment();

            // uyuşan masaüstü varsa ona göre işlem yapsın
            // GNOME
            if(strcmp(linuxDesktopEnv, __DESKTOP_ENV_GNOME__) == 0)
                osSupportedCommand = __SHUTDOWN_GNOME_LOCK__;
            // KDE
            else if(strcmp(linuxDesktopEnv, __DESKTOP_ENV_KDE__) == 0)
                osSupportedCommand = __SHUTDOWN_KDE_LOCK__;
            // XFCE
            else if(strcmp(linuxDesktopEnv, __DESKTOP_ENV_XFCE__) == 0)
                osSupportedCommand = __SHUTDOWN_XFCE_LOCK__;
            // MATE
            else if(strcmp(linuxDesktopEnv, __DESKTOP_ENV_MATE__) == 0)
                osSupportedCommand = __SHUTDOWN_MATE_LOCK__;
            // Bilinmeyen
            else
                osSupportedCommand = __SHUTDOWN_LOCK__;
        #endif

        snprintf(
            buffer,
            strArrMax(buffer),
            "%s",
            osSupportedCommand
        );
    }
    // iptal etme
    else if(strcmp(argMydtshptr->command, __SHUTDOWN_COMM_CANCEL__) == 0)
    {
        snprintf(
            buffer,
            strArrMax(buffer),
            "%s",
            __SHUTDOWN_CANCEL__
        );
    }
    // geçersiz, bilinmeyen komut
    else
        return DTSH_ERR_UNKNOWNCOMM;

    system(buffer); // komutu çalıştır

    // yeni bir dosya işlem yapısı oluştursun
    mydtfs logfile;

    // dosya ayarlamalarını yapsın
    logfile.file_addr = NULL;
    logfile.file_path = __DTFS_LOG__; // kayıt dosyası
    logfile.open_type = __DTFS_OTYPE_ADD__; // sonuna veri ekle
    
    // dosya açma kontrolü
    switch(dtfsf_fileopener(&logfile))
    {
        // başarıyla açıldı, sorun yok
        case DTFS_MSG_OPENED:
            break;
        // başarısız
        default:
            // komut çalıştırma başarı kodunu döndürsün
            return DTSH_MSG_TIMERCREATED;
    }

    // dosyaya veriyi eklesin
    fprintf(
        logfile.file_addr, // dosya işaretçisi
        "%s %s | %s %zu\n", // eklenecek metin
        "command -->", argMydtshptr->command, // komut metini
        "minute -->", argMydtshptr->time // süre metini
    );

    // dosyayı kapatsın
    dtfsf_filecloser(&logfile);

    // başarı kodunu döndür
    return DTSH_MSG_TIMERCREATED;
}

#endif

#ifndef DT_SHUTDOWN_H
#error "DT_SHUTDOWN_H!"
#endif