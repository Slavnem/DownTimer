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
    // Windows ise "Windows Shell" kullanıldığı varsayılacak
    #if _WIN32 || _WIN64
    	#if __DEBUG_OS_SUPPORT__
    		printf("\n%s\n", "* Windows Isletim Sistemi Tespit Edildi *");
    	#endif
    	
        return "WINDOWS SHELL";
    #endif

    // kullanılan xdg arayüzünü alsın
    string_a desktop_env = getenv("XDG_CURRENT_DESKTOP");

    // boş değilse eğer, arayüz bulundu
    if(!ISNULL(desktop_env))
    {
        #ifdef __DEBUG_OS_SUPPORT__
            printf("\n%s\n", "* Linux Masaustu Arayuzu Bulundu *");
        #endif

        return desktop_env;
    }

    // arayüz oturumunu alsın
    desktop_env = getenv("DESKTOP_SESSION");

    // boş değilse arayüz bulundu, aksi halde boş
    #ifdef __DEBUG_OS_SUPPORT__
        if(ISNULL(desktop_env))
            printf("\n%s\n", "* Linux Masaustu Arayuzu Bulunamadi *");
        else
            printf("\n%s%s%s\n", "* Bulunan Linux Masaustu Arayuzu: ", desktop_env, " *");
    #endif

    // masaüstü arayüzü sonucunu döndür
    return desktop_env;
}

extern string_s myDesktopEnv(void)
{
    // yerel fonksiyonu çalıştır
    return static_myDesktopEnv();
}
