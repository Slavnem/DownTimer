// Slavnem @2024-07-26
// OS "İşletim Sistemi" Desteği İşlemleri

// Kütüphaneler
#include <stdio.h>
#include <stdlib.h>

#include "../lib/TypeDefine.h"
#include "../lib/Macro.h"
#include "../lib/OsSupport.h"

// Fonksiyonlar
static string_s static_myDesktopEnv(void)
{
    // Linux değils eğer bilinmeyen bir masaüstü dür
    #ifndef __linux__
    	#if __DEBUG_MSG_OS_SUPPORT__
    		DEBUG_PRINT("Desteklenmeyen Isletim Sistemi Tespit Edildi");
    	#endif
        return NULL;
    #endif

    // kullanılan xdg arayüzünü alsın
    string_a desktop_env = getenv("XDG_CURRENT_DESKTOP");

    // boş değilse eğer, arayüz bulundu
    if(!ISNULL(desktop_env))
    {
        #ifdef __DEBUG_MSG_OS_SUPPORT__
            DEBUG_PRINT("Linux Masaustu Arayuzu Bulundu");
        #endif
        return desktop_env; // masaüstü ortamını döndürsün
    }

    // arayüz oturumunu alsın
    desktop_env = getenv("DESKTOP_SESSION");

    // boş değilse arayüz bulundu, aksi halde boş
    #ifdef __DEBUG_MSG_OS_SUPPORT__
        DEBUG_PRINT(ISNULL(desktop_env) ?
            "Linux Masaustu Arayuzu Bulunamadi" :
            desktop_env
        );
    #endif
    return desktop_env; // masaüstü arayüzü sonucunu döndür
}

extern string_s myDesktopEnv(void)
{
    // yerel fonksiyonu çalıştır
    return static_myDesktopEnv();
}
