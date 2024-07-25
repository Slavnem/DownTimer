// Slavnem @2024-07-21
// Başlangıç İşlemleri

// Kütüphaneler
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/TypeDefine.h"
#include "../lib/Macro.h"
#include "../lib/Time.h"
#include "../lib/FileSteam.h"
#include "../lib/Shutdown.h"
#include "../lib/Init.h"

// zamanlayıcı komutu kullanma metinleri
static string_s _strCommands[] =
{
    "Kapat | Turn Off:",
    "Yeniden Baslat | Restart:",
    "Uyku Modu | Sleep Mode:",
    "Kilitle | Lock Off:",
    "Iptal Et | Cancel:"
};

// zamanlayıcı komutları
static uint8_s _codeCommands[] =
{
    ESH_COMM_OFF, // kapat
    ESH_COMM_RESTART, // yeniden başlat
    ESH_COMM_SLEEP, // uyku
    ESH_COMM_LOCK, // kilitle
    ESH_COMM_CANCEL // iptal et
};

static string_s _strEnterTime = "Sureyi \"?(Saat) ?(Dakika)\" Giriniz | Enter The Time \"?(Hour) ?(Minute)\":";
static string_s _strRandomNumberForTerminate = "* Sonlandirmak Icin Rastgele Numara | Random Number For Terminate *";

static const uint8_s lengthStrArr = SIZEARRAY(_strCommands);
static const uint8_s lengthCodeArr = SIZEARRAY(_codeCommands);

// Fonksiyon Prototipler
static void showCommandList(void);
static const boolean isCommandFound(string_s argStr);

/* Konsol Mesaj Çıktısı
    Kullanıcıdan verilern mesajı belirtilen standarta
    göre çıktı verilmesini sağlamak
*/
static void static_init_message(string_s message)
{
    // mesaj çıktısı
    printf("\n* %s *\n", message);
}

extern void init_message(string_s message)
{
    // yerel fonksiyonu çalıştırsın
    static_init_message(message);
}

/* Konsol Komutları İle Veri Okuyucu
    Konsoldan gelen argümanlar okunabilir ve kullanılabilir
    ise gerekli işlem yapılır ve işlem başarılı ise başarı kodu
    döndürür bu sayede komut ekranı çalışmadan işlemler yapılmış olur
*/
static const EINITCODE static_init_console(uint8_s argc, string_s argvptr[])
{
    // geçici olarak komutu tutacak değişken
    string_a strCommand = NULL;

    // komutu okusun ve bulunmuşsa devam, değilse sonlandırsın
    if(!isCommandFound(argvptr[1]))
    {
        #ifdef __DEBUG_INIT__
            printf("\n%s\n", "* Gecerli Islem Komutu Bulunamadi *");
        #endif

        return EINIT_ERR_CONSOLEARG;
    }
    else
    {
        #ifdef __DEBUG_INIT__
            printf("\n%s\n", "* Komut Basariyla Bulundu *");
        #endif

        strCommand = argvptr[1]; // komutu depolasın
    }

    uint16_t tempHour = 0; // saat
    uint16_t tempMinute = 0; // dakika

    // argüman sayısına göre
    switch(argc)
    {
        // komut ve dakika
        case 3:
            tempHour = 0; // manuel olarak tekrardan saati 0 yapmak
            tempMinute = (!ISNULL(argvptr[2])) ? (int16_t)strtoul(argvptr[2], NULL, 0) : 0;

            #ifdef __DEBUG_INIT__
                printf("\n%s\n", "* Konsoldan Sadece Dakika Bilgisi Okundu *");
            #endif
        break;
        // komut, saat ve dakika
        case 4:
            tempHour = (!ISNULL(argvptr[2])) ? (int16_t)strtoul(argvptr[2], NULL, 0) : 0;
            tempMinute = (!ISNULL(argvptr[3])) ? (int16_t)strtoul(argvptr[3], NULL, 0) : 0;

            #ifdef __DEBUG_INIT__
                printf("\n%s\n", "* Konsoldan Saat ve Dakika Bilgisi Okundu *");
            #endif
        break;
    }

    // zamanlayıcıyı oluştursun ve dönen koda işlem yapılacak
    switch(shutdown_timer(strCommand, TOTALTIME(tempHour, tempMinute), TRUE))
    {
        // zamanlayıcı başarıyla çalıştırıldı
        case ESH_CODE_MSG_TIMERRUNNED:
            #ifdef __DEBUG_INIT__
                printf("\n%s\n", "* Konsol Komutlari Kullanarak Zamanlayiciniz Basariyla Olusturuldu *");
            #endif

            // konsol işlemi başarılı kodu
            return EINIT_MSG_CONSOLEOK;
        // diğer
        default: break;
    }

    // komut var ama zamanlayıcı oluşturma hatası döndür
    #ifdef __DEBUG_INIT__
        printf("\n%s\n", "* Konsole Komutlari Ile Zamanlayici Olusturulamadi *");
    #endif

    // zamanlayıcı hatalı işlem kodu
    return EINIT_ERR_TIMER;
}

extern const EINITCODE init_console(uint8_s argc, string_s argvptr[])
{
    // yerel kodu çalıştır
    return static_init_console(argc, argvptr);
}

/* Ekrandan Veri Okuyucu
    Kullanıcı konsol argümanları ile işlem yapmadığında
    bir kod döndürür ve o koda göre ekranın gösterilip
    gösterilmeyeceği belirtilir, aksi halde konsol çalışır
    ve sonlanır ve ekran çalıştırılmaz
*/
static const EINITCODE static_init_screen(void)
{
    // girilecek komut, saat ve dakika tutulacak
    int16_t tempCommand; // komut
    int16_t tempHour = 0; // saat
    int16_t tempMinute = 0; // dakika
    
    // komut metni
    string_a strCommand = NULL;

    // komut listesini göstersin
    showCommandList();
    puts(_strRandomNumberForTerminate);

    // kullanıcıdan değeri alsın
    scanf("%hd", &tempCommand);

    // girilen koda göre işlem
    switch(tempCommand)
    {
        // kapat
        case ESH_COMM_OFF:
            strCommand = __SHUTDOWN_COMM_OFF__;
        break;
        // yeniden başlat
        case ESH_COMM_RESTART:
            strCommand = __SHUTDOWN_COMM_RESTART__;
            break;
        // uyku
        case ESH_COMM_SLEEP:
            strCommand = __SHUTDOWN_COMM_SLEEP__;
            goto screentimer;
        break;
        // kilitle
        case ESH_COMM_LOCK:
            strCommand = __SHUTDOWN_COMM_LOCK__;
            goto screentimer;
        break;
        // iptal et
        case ESH_COMM_CANCEL:
            strCommand = __SHUTDOWN_COMM_CANCEL__;
            goto screentimer;
        break;
        // geçersiz, sonlandırıldı
        default:
            #ifdef __DEBUG_INIT__
                printf("\n%s%d%s\n", "* Gecersiz Islem Komutu (", tempCommand, ") Girildi *");
            #endif

            // sonlandırma komutu
            return EINIT_STAT_TERMINATED;
    }

    // süreyi girmesini istemek
    printf("%s ", _strEnterTime);
    scanf("%hd %hd", &tempHour, &tempMinute);

    // zamanlayıcı ile birlikte çalıştırsın
    goto screentimer;

    // ekran zamanlayıcısı
    screentimer:
        // zamanlayıcıyı oluştursun ve dönen koda işlem yapılacak
        switch(shutdown_timer(strCommand, TOTALTIME(tempHour, tempMinute), FALSE))
        {
            // zamanlayıcı başarıyla çalıştırıldı
            case ESH_CODE_MSG_TIMERRUNNED:
                #ifdef __DEBUG_INIT__
                    printf("\n%s\n", "* Ekran Menusu Kullanarak Zamanlayiciniz Basariyla Olusturuldu *");
                #endif

                // ekran işlemi başarılı kodu
                return EINIT_MSG_SCREENOK;
            // komut var ama zamanlayıcı oluşturma hatası döndür
            default:
                #ifdef __DEBUG_INIT__
                    printf("\n%s\n", "* Ekran Menusu Ile Zamanlayici Olusturulamadi *");
                #endif

                // zamanlayıcı hatalı işlem kodu
                return EINIT_ERR_TIMER;
        }

    // ekran işlemi başarılı kodu
    return EINIT_MSG_SCREENOK;
}

extern const EINITCODE init_screen(void)
{
    // yerel kodu çalıştır
    return static_init_screen();
}

/* Ekranda komutları çıktı versin
    Ekrana kullanılabilecek komutları
    çıktı veren ve herhangi bir veri almayan
    basit bir fonksiyon
*/
static void showCommandList(void)
{
    uint8_t counter = 0; // sayaç

    // döngüyle birlikte çıktı versin
    while(counter < lengthStrArr && counter < lengthCodeArr)
    {
        // çıktı ver ve sayacı arttır
        printf("%s %u\n", _strCommands[counter], _codeCommands[counter]);
        ++counter;
    }
}

/* Komut Metini Kontrolcüsü
    Verilen metinin geçerli komutlardan biri olup
    olmadığını kontrol edip ona göre evet "true" ya da
    hayır "false" cevabı döndürülür
*/
static const boolean isCommandFound(string_s argStr)
{
    // metin boşsa zaten yoktur
    if(ISNULL(argStr))
        return FALSE;

    // sırayla kontrol etsin
    if(strcmp(argStr, __SHUTDOWN_COMM_OFF__) == 0) // kapatma
        return TRUE;
    else if(strcmp(argStr, __SHUTDOWN_COMM_RESTART__) == 0) // yeniden başlatma
        return TRUE;
    else if(strcmp(argStr, __SHUTDOWN_COMM_SLEEP__) == 0) // uyku modu
        return TRUE;
    else if(strcmp(argStr, __SHUTDOWN_COMM_LOCK__) == 0) // oturumu kilitleme
        return TRUE;
    else if(strcmp(argStr, __SHUTDOWN_COMM_CANCEL__) == 0) // iptal etme
        return TRUE;

    // komut bulunamadı
    return FALSE;
}