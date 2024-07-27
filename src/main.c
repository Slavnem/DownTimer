// Slavnem @2024-07-21
// Ana Dosya

// Geçersiz İşletim Sistemi İçin Kontrol
#ifndef __linux__
    #error "Only Linux x86_64 Supporting. Unsupported OS Error!"
#endif

// Kütüphaneler
#include "lib/Init.h"

// çalışma durumu başlangıç (boş)
static einitcode __initKernelCode = EINIT_STAT_FREE;

// main
int main(int argc, string_s argv[])
{
    // argümanlar ile işlem yapmasını sağlamak
    __initKernelCode = init_console(argc, argv);

    // çalıştırma koduna göre işlem
    switch(__initKernelCode)
    {
        // zamanlayıcı hatası
        case EINIT_ERR_TIMER:
            init_message("Konsol Zamanlayici Hatasi | Console Timer Error");
            return 1;
        // başarıyla çalıştı
        case EINIT_MSG_CONSOLEOK:
            return 0; // başarılı, program sonlansın
    }

    // ekranı çalıştırsın
    __initKernelCode = init_screen();

    // çalıştırma koduna göre
    switch(__initKernelCode)
    {
        // geçersiz komut
        case EINIT_STAT_TERMINATED:
            init_message("Program Sonlandiriliyor | App Is Terminating");
            return 1;
        // zamanlayıcı hatası
        case EINIT_ERR_TIMER:
            init_message("Ekran Zamanlayici Hatasi | Screen Timer Error");
            return 1;
        // başarıyla çalıştı
        case EINIT_MSG_SCREENOK:
            return 0; // başarılı, program sonlansın
    }

    // program sonu
    return 0;
}