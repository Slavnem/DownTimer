// Slavnem @2024-07-21
// Başlangıç Tanımlamaları
#ifndef DT_INIT_H
#define DT_INIT_H

// Kütüphaneler
#include "CompileConf.h"
#include "TypeDefine.h"

// Başlangıç Hata Ayıklayıcısı Belirteci
#ifndef __COMPILE_RELEASE_MODE__
    #define __DEBUG_MSG_INIT__
#endif

// DTINIT Kodları
typedef enum
{
    // Hatalar
    EINIT_ERR_CONSOLEARG = 0, // konsoldan argüman okuma hatası
    EINIT_ERR_DEFAULTSCREEN, // normal ekrandan çalıştırma hatası
    EINIT_ERR_TIMER, // komut var ama zamanlayıcı hatası var

    // Başarı Durum Mesajlar
    EINIT_MSG_CONSOLEOK = 2000, // konsoldan başarıyla okundu
    EINIT_MSG_SCREENOK, // normal ekrandan başaryıla okundu

    // Durumlar
    EINIT_STAT_FREE = 3000, // boş
    EINIT_STAT_UNKNOWN, // bilinmeyen durum
    EINIT_STAT_TERMINATED, // elle sonlandırıldı
    EINIT_STAT_CONSOLEARGNULL, // konsol argümanı boş
    EINIT_STAT_SCREENARGNULL // normal ekran argümanı boş
} einitcode;

typedef const einitcode einitcode_s;

extern void init_message(string_s message);
extern einitcode_s init_console(int argc, string_s argvptr[]);
extern einitcode_s init_screen(void);

#endif