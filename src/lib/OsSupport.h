// Slavnem @2024-07-25
// OS "İşletim Sistemi" Desteği
#ifndef DT_OS_SUPPORT_H
#define DT_OS_SUPPORT_H

// Kütüphaneler
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // geteuid fonksiyonu için gerekli kütüphane
#include "TypeDefine.h"
#include "Macro.h"

// İşletim Sistemi Desteği Hata Ayıklayıcısı Belirteci
#ifndef __COMPILE_RELEASE_MODE__
    #define __DEBUG_MSG_OS_SUPPORT__
    #define __DEBUG_MSG_OS_SUPPORT_TITLE__ "OS SUPPORT"
#endif

// Linux Desteği
#ifdef __linux__
    // Masaüstü Arayüzleri
    #define __DESKTOP_ENV_GNOME__ "GNOME"
    #define __DESKTOP_ENV_KDE__ "KDE"
    #define __DESKTOP_ENV_XFCE__ "XFCE"
    #define __DESKTOP_ENV_MATE__ "MATE"
#endif

// Kök Yönetici Bilgisi
#define __ISROOT__ (geteuid() == 0 ? TRUE : FALSE) // Eğer 0'sa Kök Kullanıcıdır

// Bit Mimarisine Göre Destek
// Linux x64
#ifdef __x86_64__
    #define __OS_ARCH_NAME__ "Linux x64" // 64 Bit
    #define __OS_ARCH_x64__ // İşletim Sistemi 32 Bit Mimarisi
    #define __INTMAX_FORMAT__ "%lu" /* en büyük int sayı printf desteği */
// Linux x86
#elif defined __i386__
    #define __OS_ARCH_NAME__ "Linux x86" // 32 Bit
    #define __OS_ARCH_x86__ // İşletim Sistemi 64 Bit Mimarisi
    #define __INTMAX_FORMAT__ "%llu" /* en büyük int sayı printf desteği */
#endif

// Masaüstünü Alan Fonksiyonlar Prototip
extern string_s myDesktopEnv(void);

#endif
