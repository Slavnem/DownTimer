// Slavnem @2024-07-25
// OS "İşletim Sistemi" Desteği
#ifndef DT_OS_SUPPORT_H
#define DT_OS_SUPPORT_H

// GCC Kullanılmayan fonksiyon uyarılarını devre dışı bırakmak
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

// Kütüphaneler
#include <stdio.h>
#include <stdlib.h>

#include "TypeDefine.h"
#include "Macro.h"

// Masaüstünü Alan Fonksiyon Prototipleri
static string_s myDesktopEnv(void);

// İşletim Sistemi İsimlendirmesi
#ifdef _WIN32
    #define __OS__ "Windows x86"
#elif defined _WIN64
    #define __OS__ "Windows x64"
#elif defined __linux__
    #ifdef __i386__
        #define __OS__ "Linux x86"
    #elif __x86_64__
        #define __OS__ "Linux x64"
    #endif
#else
    #define __OS__ "Unsupported OS"
    #error "This OS Is Not Supporting!"
#endif

// Linux Desteği
#ifdef __linux__
    // Masaüstü Arayüzleri
    #define __DESKTOP_ENV_GNOME__ "GNOME"
    #define __DESKTOP_ENV_KDE__ "KDE"
    #define __DESKTOP_ENV_XFCE__ "XFCE"
    #define __DESKTOP_ENV_MATE__ "MATE"
#endif

// Masaüstü Ortamı
#define __OS_DESKTOP_ENV__ myDesktopEnv()

// Masaüstünü Alan Fonksiyonları
static string_s myDesktopEnv(void)
{
    // Windows ise "Windows Shell" kullanıldığı varsayılacak
    #if defined(_WIN32) || defined(_WIN64)
        return "WINDOWS SHELL";
    #endif

    // kullanılan xdg arayüzünü alsın
    string_a desktop_env = getenv("XDG_CURRENT_DESKTOP");

    // boş değilse eğer, arayüz bulundu
    if(!ISNULL(desktop_env))
    {
        #ifdef __DEBUG_SHUTDOWN__
            printf("\n%s\n", "* Linux Masaustu Arayuzu Bulundu *");
        #endif

        return desktop_env;
    }

    // arayüz oturumunu alsın
    desktop_env = getenv("DESKTOP_SESSION");

    // boş değilse arayüz bulundu, aksi halde boş
    #ifdef __DEBUG_SHUTDOWN__
        if(ISNULL(desktop_env))
            printf("\n%s\n", "* Linux Masaustu Arayuzu Bulunamadi *");
        else
            printf("\n%s%s%s\n", "* Bulunan Linux Masaustu Arayuzu: ", desktop_env, " *");
    #endif

    // masaüstü arayüzü sonucunu döndür
    return desktop_env;
}

#endif