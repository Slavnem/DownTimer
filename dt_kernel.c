// Slavnem @2024-07-08
// DT Çekirdek

// DT Kütüphaneler
#include "dt_init.h"

// Çalışma durumu başlangıçta boş
static dtinite_code __initKernelCode = DTINIT_STAT_FREE;

// main
int main(uint16_s argc, string_s argv[])
{
    // argümanlar ile işlem yapmasını sağlamak
    __initKernelCode = dtinitf_console(argv);

    // çalıştırma koduna göre işlem
    switch(__initKernelCode)
    {
        case DTINIT_MSG_CONSOLEOK:
            return 0; // başarılı, program sonlansın
    }

    // ekranı çalıştırsın
    __initKernelCode = dtinitf_screen();

    // program sonu
    return 0;
}