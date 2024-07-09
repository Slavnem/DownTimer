// Slavnem @2024-07-08
// DT Başlatıcı

#ifndef DT_INIT_H
#define DT_INIT_H

// DT Kütüphaneler
#include "tools/dt_typedef.h"
#include "tools/dt_macro.h"
#include "tools/dt_allocation.h"
#include "tools/dt_filestream.h"
#include "tools/dt_shutdown.h"

// INIT Komutları
#define __INIT_EXIT__ -1

// DTINIT Kodları
typedef enum
{
    // DTINIT Hataları
    DTINIT_ERR_CONSOLEARG = 0, // konsoldan argüman okuma hatası
    DTINIT_ERR_DEFAULTSCREEN, // normal ekrandan çalıştırma hatası
    DTINIT_ERR_TIMER, // komut var ama zamanlayıcı hatası var

    // DTINIT Başarı Durum Mesajları
    DTINIT_MSG_CONSOLEOK = 2000, // konsoldan başarıyla okundu
    DTINIT_MSG_SCREENOK, // normal ekrandan başaryıla okundu

    // DTINIT Durumları
    DTINIT_STAT_FREE = 3000, // boş
    DTINIT_STAT_UNKNOWN, // bilinmeyen durum
    DTINIT_STAT_TERMINATED, // elle sonlandırıldı
    DTINIT_STAT_CONSOLEARGNULL, // konsol argümanı boş
    DTINIT_STAT_SCREENARGNULL // normal ekran argümanı boş
} dtinite_code;

// Komutları Yazdırmak İçin Diziler
static string_s _strCommands[] =
{
    "Kapat | Turn Off:",
    "Yeniden Baslat | Restart:",
    "Uyku Modu | Sleep Mode:",
    "Kilitle | Lock Off:",
    "Iptal Et | Cancel:"
};

static uint8_s _codeCommands[] =
{
    __SHUTDOWN_CODE_OFF__,
    __SHUTDOWN_CODE_RESTART__,
    __SHUTDOWN_CODE_SLEEP__,
    __SHUTDOWN_CODE_LOCK__,
    __SHUTDOWN_CODE_CANCEL__
};

static string_s _strEnterTime = "Sureyi \"Dakika\" Giriniz | Enter The Time \"Minute\":";
static string_s _strRandomNumberForTerminate = "* Sonlandirmak Icin Rastgele Numara | Random Number For Terminate *";

static uint8_s lengthStrArr = arrSize(_strCommands);
static uint8_s lengthCodeArr = arrSize(_codeCommands);

//////////////////////////////////
// Fonksiyon Prototipleri
//////////////////////////////////
extern dtinite_code dtinitf_console(string_s argvptr[]);
extern dtinite_code dtinitf_screen(void);

static boolean isCommandFound(string_s argStr);
static void showCommandList(void);

//////////////////////////////////
// Fonksiyonlar
//////////////////////////////////
// Konsoldan Veri Okuyucu
/*
    Konsoldan gelen argümanlar okunabilir ve kullanılabilir
    ise gerekli işlem yapılır ve işlem başarılı ise başarı kodu
    döndürür bu sayede komut ekranı çalışmadan işlemler yapılmış olur
*/
extern dtinite_code dtinitf_console(string_s argvptr[])
{
    string_a tempCommand = NULL; // geçici olarak komutu tutmak
    uint16_t tempTime = 0; // geçici olarak süreyi tutmak
    uint8_t counter; // sayaç

    // sayaç ile komutu depolasın
    for(counter = 0; !isNull(argvptr[counter]); counter++)
    {   
        // komut bulunmadığı için sonraki tura geçsin
        if(!isCommandFound(argvptr[counter]))
            continue;

        // metini geçici olarak tutsun
        tempCommand = argvptr[counter];

        // döngüyü kırsın
        break;
    }

    // sayaç ile bu seferde dakikayı bulsun
    for(counter = 0; !isNull(argvptr[counter]); counter++)
    {
        // metini 10'luk sistem alsın ve 0 ya da pozitif metini alsın
        tempTime = strtoul((argvptr[counter]), NULL, 10);

        // geçerli bi süre varsa döngüyü sonlandırsın
        if(tempTime > 0)
            break;
    }

    // eğer komut yoksa ve süre de yoksa hata döndürsün
    if(isNull(tempCommand) && tempTime < 1)
        return DTINIT_ERR_CONSOLEARG;

    // zamanlayıcı için yapı oluştursun
    mydtsh newTimer =
    {
        .time = tempTime, // süre
        .command = __SHUTDOWN_COMM_OFF__ // komut
    };

    // eğer kapatma dışında bir komut girilmişse
    // komutu değiştirsin
    if(!isNull(tempCommand) || tempCommand != __SHUTDOWN_COMM_OFF__ )
        newTimer.command = tempCommand;

    // zamanlayıcıyı oluştursun ve dönen koda işlem yapılacak
    switch(dtshf_timer(&newTimer))
    {
        // zamanlayıcı başarıyla oluşturuldu
        case DTSH_MSG_TIMERCREATED:
            return DTINIT_MSG_CONSOLEOK;
        // komut var ama zamanlayıcı oluşturma hatası döndür
        default:
            return DTINIT_ERR_TIMER;
    }

    // hata vermemesi için konuldu
    return DTINIT_MSG_CONSOLEOK;
}

// Normal Veri Okuyucu
/*
    Kullanıcı konsol argümanları ile işlem yapmadığında
    bir kod döndürür ve o koda göre ekranın gösterilip
    gösterilmeyeceği belirtilir, aksi halde konsol çalışır
    ve sonlanır ve ekran çalıştırılmaz
*/
extern dtinite_code dtinitf_screen(void)
{
    // girilecek değerleri (komut ve süre) tutacak
    int32_t tempNumber;
    int32_t tempTime;

    // zamanlayıcı için yapı oluştursun
    mydtsh newTimer =
    {
        .time = 0, // süre
        .command = NULL // komut
    };

    // komut listesini göstersin
    showCommandList();
    puts(_strRandomNumberForTerminate);

    // kullanıcıdan değeri alsın
    scanf("%d", &tempNumber);

    // girilen koda göre işlem
    switch(tempNumber)
    {
        // kapat
        case __SHUTDOWN_CODE_OFF__:
            newTimer.command = __SHUTDOWN_COMM_OFF__;
        break;
        // yeniden başlat
        case __SHUTDOWN_CODE_RESTART__:
            newTimer.command = __SHUTDOWN_COMM_RESTART__;
            break;
        // uyku
        case __SHUTDOWN_CODE_SLEEP__:
            newTimer.command = __SHUTDOWN_COMM_SLEEP__;
            goto screentimer;
        break;
        // kilitle
        case __SHUTDOWN_CODE_LOCK__:
            newTimer.command = __SHUTDOWN_COMM_LOCK__;
            goto screentimer;
        break;
        // iptal et
        case __SHUTDOWN_CODE_CANCEL__:
            newTimer.command = __SHUTDOWN_COMM_CANCEL__;
            goto screentimer;
        break;
        // geçersiz, sonlandırıldı
        default:
            return DTINIT_STAT_TERMINATED;
    }

    // süreyi girmesini istemek
    printf("%s ", _strEnterTime);
    scanf("%d", &tempTime);

    // süreyi kontrol edip ayarlasın
    newTimer.time = (tempTime > 0 ? tempTime : 0);

    // zamanlayıcı ile birlikte çalıştırsın
    goto screentimer;

    // ekran zamanlayıcısı
    screentimer:
        // zamanlayıcıyı oluştursun ve dönen koda işlem yapılacak
        switch(dtshf_timer(&newTimer))
        {
            // zamanlayıcı başarıyla oluşturuldu
            case DTSH_MSG_TIMERCREATED:
                return DTINIT_MSG_SCREENOK;
            // komut var ama zamanlayıcı oluşturma hatası döndür
            default:
                return DTINIT_ERR_TIMER;
        }

    // hata vermemesi için konuldu
    return DTINIT_MSG_SCREENOK;
}

// ekranda komutları çıktı versin
/*
    Ekrana kullanılabilecek komutları
    çıktı veren ve herhangi bir veri almayan
    basit bir fonksiyon
*/
static void showCommandList(void)
{
    // sayaç
    uint8_t counter;

    // döngüyle birlikte çıktı versin
    for(counter = 0; counter < lengthStrArr && counter < lengthCodeArr; counter++)
    {
        printf("%s %u\n", _strCommands[counter], _codeCommands[counter]);
    }
}

// Komut Metini Kontrolcüsü
/*
    Verilen metinin geçerli komutlardan biri olup
    olmadığını kontrol edip ona göre evet "true" ya da
    hayır "false" cevabı döndürülür
*/
static boolean isCommandFound(string_s argStr)
{
    // metin boşsa zaten yoktur
    if(isNull(argStr))
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

#endif

#ifndef DT_INIT_H
#error "DT_INIT_H!"
#endif