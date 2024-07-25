// Slavnem @2024-07-21
// Dosya İşleme Yapısı İşlemleri

// Kütüphaneler
#include <stdio.h>

#include "../lib/TypeDefine.h"
#include "../lib/Macro.h"
#include "../lib/MemAllocation.h"
#include "../lib/FileSteam.h"

/* Dosya Açıcı
    Oluşturulan dosya yapısını kullanarak gerekli dosyayı açacak
    ve dosya başarıyla açılırsa değerler kaydedilecek ve başarı
    kodu döndürülecek, aksi halde hata kodu döndürülecek
*/
static const EFSCODE static_file_opener(MyFilePtr myfileptr, string_s filepath, EFSOTYPE opentype)
{
    // eğer gelen yapı boşsa, bellek alanı ayırsın
    if(ISNULL(myfileptr))
    {
        // bellek alanı tahsis etsin
        myfileptr = (MyFilePtr)mem_alloc(myfileptr, sizeof(MyFile));

        // bellek alanı tahsis edilmesine rağmen boşsa
        if(ISNULL(myfileptr))
        {
            #ifdef __DEBUG_FILESTREAM__
                printf("\n%s\n", "* Bos Dosya Degiskeni Icin Bellekten Alan Tahsis Edilemedi, Bellek Musait Degil *");
            #endif

            // yeni obje oluşturulamama hatası
            return EFS_CODE_ERR_NEWOBJECT;
        }

        // değerleri ayarla
        myfileptr->addr = NULL;
        myfileptr->path = filepath;
        myfileptr->otype = opentype;
        
        // bellek alan ayırımı başarılı
        #ifdef __DEBUG_FILESTREAM__
            printf("\n%s\n", "* Bos Dosya Degiskeni Icin Bellek Alani Basariyla Tahsis Edildi *");
            printf("\n%s\n", "* Baslangic Deger Atamalari Basariyla Yapildi *");
        #endif
    }
    else if(!ISNULL(myfileptr->addr))
    {
        #ifdef __DEBUG_FILESTREAM__
            printf("\n%s\n", "* Yeni Bir Dosya Acma Islemi Icin Acik Dosya Olmamasi Zorunludur *");
        #endif

        // dosya işaretçisi boş değil
        return EFS_CODE_ERR_NOTNULL;
    }
    // dosya yolu kontrolü
    else if(ISNULL(filepath))
    {
        #ifdef __DEBUG_FILESTREAM__
            printf("\n%s\n", "* Yeni Bir Dosya Acmak Icin Dosya Yolu Olmak Zorundadir *");
        #endif

        // dosya yolu boş
        return EFS_CODE_ERR_PATHNULL;
    }

    // geçici olarak dosya açma türünü tutacak olan değişken
    string tempOpenType = NULL;

    // dosya açma türleri kontrolü
    switch(myfileptr->otype)
    {
        case EFS_OTYPE_READ: // okuma
            tempOpenType = "r";
            break;
        case EFS_OTYPE_READBIN: // ikilik okuma
            tempOpenType = "rb";
            break;
        case EFS_OTYPE_READWRITE: // okuma yazma
            tempOpenType = "r+";
            break;
        case EFS_OTYPE_READWRITEBIN: // ikilik okuma yazma
            tempOpenType = "rb+";
            break;
        case EFS_OTYPE_WRITE: // yazma
            tempOpenType = "w";
            break;
        case EFS_OTYPE_WRITEBIN: // ikilik yazma
            tempOpenType = "wb";
            break;
        case EFS_OTYPE_WRITEREAD: // yazma okuma
            tempOpenType = "w+";
            break;
        case EFS_OTYPE_WRITEREADBIN: // ikilik yazma okuma
            tempOpenType = "wb+";
            break;
        case EFS_OTYPE_ADD: // sona ekle
            tempOpenType = "a";
            break;
        case EFS_OTYPE_ADDBIN: // ikilik sona ekle
            tempOpenType = "ab";
            break;
        case EFS_OTYPE_ADDREAD: // sona ekle oku
            tempOpenType = "a+";
            break;
        case EFS_OTYPE_ADDREADBIN: // ikilik sona ekle oku
            tempOpenType = "ab+";
            break; // sorunsuz
        // dosya açma türü geçersiz
        default:
            #ifdef __DEBUG_FILESTREAM__
                printf("\n%s\n", "* Gecersiz Dosya Acma Turu Girdiniz *");
            #endif

            return EFS_CODE_ERR_INVALIDOPENTYPE;
    }

    // dosyayı açma işlemi
    myfileptr->addr = fopen(
        filepath, // dosya yolu
        tempOpenType // dosya açma türü
    );

    // eğer dosya adresi boş ise dosya açılamamıştır
    // aksi halde dosya başarıyla açılmıştır
    #ifdef __DEBUG_FILESTREAM__
        if(ISNULL(myfileptr->addr))
            printf("\n%s\n", "* Dosya Acma Islemi Basarisiz Oldu *");
        else
            printf("\n%s\n", "* Dosya Basariyla Acildi *");
    #endif

    // dosya adresi kontrolüne göre kod dönülecek
    return ISNULL(myfileptr->addr) ?
        EFS_CODE_ERR_OPENFAIL // dosya açılamadı
        :
        EFS_CODE_MSG_OPENED; // dosya başarıyla açıldı
}

extern const EFSCODE file_opener(MyFilePtr myfileptr, string_s filepath, EFSOTYPE opentype)
{
    // yerel fonksiyonu kullan
    return static_file_opener(myfileptr, filepath, opentype);
}

/* Açık Dosya Kapatıcı
    Oluşturulan dosya yapısını kullanarak gerekli dosya eğer açıksa
    dosyayı kapatacak ve dosyayı bellekten temizleyecek fakat eğer dosya
    zaten kapalıysa ya da kapanmıyorsa, oluşan duruma göre hata ya da
    durum kodu döndürecektir aksi halde zaten kapatma başarılı olmuştur
    ve bu yüzden başarı kodu döndürür
*/
static const EFSCODE static_file_closer(MyFilePtr myfileptr)
{
    // eğer dosya kapalıysa zaten kapatacak bir şey yoktur
    // gerekli ayarlamalar yapılıp sonlandırılır
    if(ISNULL(myfileptr->addr))
    {
        #ifdef __DEBUG_FILESTREAM__
            printf("\n%s\n", "* Dosya Kapatmayi Gerektirecek Aktif Dosya Zaten Yok *");
        #endif

        // dosya boş bildirimi
        return EFS_CODE_STAT_FREE;
    }

    // dosyayı kapat ve adresini boş duruma getir
    fclose(myfileptr->addr);
    myfileptr->addr = NULL;

    // dosya başarıyla kapatılmışsa eğer, dosya işaretçisi boş olur
    // yani NULL olur ve bu sayede dosyanın kapandığını anlarız
    // aksi halde dosya kapanmamıştır
    #ifdef __DEBUG_FILESTREAM__
        if(ISNULL(myfileptr->addr))
            printf("\n%s\n", "* Dosya Basariyla Kapatildi *");
        else
            printf("\n%s\n", "* Dosya Kapatma Islemi Basarisiz Oldu *");
    #endif

    // dosya kapatma durumuna göre kod döndürülsün
    return ISNULL(myfileptr->addr) ?
        EFS_CODE_ERR_CLOSEFAIL // dosya kapatılamadı hatası
        :
        EFS_CODE_MSG_CLOSED; // dosya başarıyla kapandı
}

extern const EFSCODE file_closer(MyFilePtr myfileptr)
{
    // yerel fonksiyonu kullan
    return static_file_closer(myfileptr);
}

/* Dosya Durumu Kontrolcüsü
    Dosya yapısın inceleyerek dosyanın açık ya da kapalı olmasını
    kontrol eder fakat dosya yapısındaki verilere müdahele edemez
*/
static const EFSCODE static_file_status(MyFilePtr_s myfileptr)
{
    // Dosya bellek işaretçisini kontrol et ve
    // boş ya da aktif kodunu döndür
    #ifdef __DEBUG_FILESTREAM__
        if(ISNULL(myfileptr) || ISNULL(myfileptr->addr))
            printf("\n%s\n", "* Dosya Durumu Bos *");
        else
            printf("\n%s\n", "* Dosya Bellekte Mevcut *");
    #endif

    // dosya durumuna göre
    return ISNULL(myfileptr) || ISNULL(myfileptr->addr) ?
        EFS_CODE_STAT_FREE // dosya boş
        :
        EFS_CODE_STAT_ACTIVE; // dosya aktif
}

extern const EFSCODE file_status(MyFilePtr_s myfileptr)
{
    // yerel fonksiyonu kullan
    return static_file_status(myfileptr);
}

/* Dosya Veri Yazıcısı
    Dosya var ve geçerli veri varsa, o dosyaya veriyi yazmasını sağlamak
    ve işlem sonrası dosyayı kapatmadan başarı kodunu döndürmek ya da
    dosya vs. yoksa hata kodunu döndürerek kullanıcıya bildirmek
*/
static const EFSCODE static_file_write(MyFilePtr_s myfileptr, string_s data)
{
    // dosya durumunu kontrol etsin ve eğer boşsa hata dönsün
    if(file_status(myfileptr) == EFS_CODE_STAT_FREE)
    {
        #ifdef __DEBUG_FILESTREAM__
            printf("\n%s\n", "* Dosyaya Veri Yazilabilmesi Icin Dosyanin Acik Olmasi Zorunludur *");
        #endif

        // dosya bulunamadı
        return EFS_CODE_ERR_FILENOTFOUND;
    }

    // dosyaya veriyi yazsın
    if(fprintf(myfileptr->addr, "%s", data) > -1)
    {
        // veri başarıyla dosyaya yazıldı
        #ifdef __DEBUG_FILESTREAM__
            printf("\n%s\n", "* Alinan Veri Dosyaya Basariyla Yazildi *");
        #endif

        // önbellekte tutmadan dosyaya veriyi kaydetsin
        fflush(myfileptr->addr);

        // veri yazıldı başarı kodu
        return EFS_CODE_MSG_DATAWRITTEN;
    }

    // veri yazılamadı hatası
    #ifdef __DEBUG_FILESTREAM__
        printf("\n%s\n", "* Alinan Veri Bir Hatadan Dolayi Dosyaya Yazilamadi *");
    #endif

    // veri yazılamadı hatası
    return EFS_CODE_ERR_UNWRITTEN;
}

extern const EFSCODE file_write(MyFilePtr_s myfileptr, string_s data)
{
    // yerel fonksiyonu çağır
    return static_file_write(myfileptr, data);
}