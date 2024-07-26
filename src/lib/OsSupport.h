// Slavnem @2024-07-25
// OS "İşletim Sistemi" Desteği
#ifndef DT_OS_SUPPORT_H
#define DT_OS_SUPPORT_H

// İşletim Sistemi Desteği Hata Ayıklayıcısı Belirteci
// #define __DEBUG_OS_SUPPORT__

// Kütüphaneler
#include <stdio.h>
#include <stdlib.h>

#include "TypeDefine.h"
#include "Macro.h"

// Geçersiz İşletim Sistemi Kontrolü
#if !_WIN32 && !_WIN64 && !__linux__
    #error "Only Linux, Windows x86 And x64's Supporting. Unsupported OS Error!"
#endif

// Windows Desteği
#if _WIN32 || _WIN64
    #if _WIN32 /* 32 Bit Desteği */
        #define __OS__ "Windows x86"
    #elif _WIN64 /* 64 Bit Desteği */
        #define __OS__ "Windows x64"
    #endif
#endif

// Linux Desteği
#if __linux__
    // Masaüstü Arayüzleri
    #define __DESKTOP_ENV_GNOME__ "GNOME"
    #define __DESKTOP_ENV_KDE__ "KDE"
    #define __DESKTOP_ENV_XFCE__ "XFCE"
    #define __DESKTOP_ENV_MATE__ "MATE"

    #ifdef __i386__ /* 32 Bit Desteği */
        #define __OS__ "Linux x86"
    #elif __x86_64__ /* 64 Bit Desteği */
        #define __OS__ "Linux x64"
    #endif
#endif

// Bit Mimarisine Göre Destek
// Windows x64 & Linux x64
#if _WIN64 || __x86_64__
    #define __OS_ARCH_x86__ // İşletim Sistemi 32 Bit Mimarisi
    #define __INTMAX_FORMAT__ "%lu" /* en büyük int sayı printf desteği */
// Windows x86 & Linux x86
#elif _WIN32 || __i386__
    #define __OS_ARCH_x64__ // İşletim Sistemi 64 Bit Mimarisi
    #define __INTMAX_FORMAT__ "%llu" /* en büyük int sayı printf desteği */
#endif

// Masaüstünü Alan Fonksiyonlar Prototip
extern string_s myDesktopEnv(void);

#endif
