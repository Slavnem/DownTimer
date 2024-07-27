// Slavnem @2024-07-21
// Bellek Alanı Ayırıcısı İşlemleri

// Kütüphaneler
#include <stdio.h>
#include <stdlib.h>

#include "../lib/TypeDefine.h"
#include "../lib/Macro.h"
#include "../lib/MemAllocation.h"

// Bellek Alanı Ayırma
/*
    Bellek alanı boş olduğu durumlarda işaretçi
    değişkene istediği boyutta bellek alanı ayırıyp
    o alanın durumunu kontrol ederek kullanıcıya
    alan ayrılmışsa işaretçinin adresini, değilse de
    NULL yani boş döndürüyoruz
*/
extern vptr mem_alloc(vptr addrptr, uintmax_s maxsize)
{
    // bellek alanı boş değilse eğer
    // yeni bellek alanı ayrılamaz
    if(!ISNULL(addrptr))
    {
        // bellek hata ayıklayıcısı aktifse eğer
        #ifdef __DEBUG_MSG_MEMALLOCATION__
            DEBUG_PRINT("Bellek Alani Ayrilamadi, Zaten Dolu");
        #endif
        return NULL; // boş adres
    }

    // bellek alanı tahsis etsin
    addrptr = malloc(maxsize);

    // bellek alanı başarıyla ayrılıp ayrılmamaya göre
    // kontrol sağlayıp başarıyla işlemi sonlandırsın
    #ifdef __DEBUG_MSG_MEMALLOCATION__
        DEBUG_PRINT(ISNULL(addrptr) ?
            "Yeni Bellek Alani Ayrilamadi, Bellek Musait Degil" :
            "Bellek Alani Basariyla Tahsis Edildi"
        );
    #endif
    return addrptr; // boşsa zaten NULL olur aksi halde bellek adresi döner
}

/* Bellek Yeniden Alan Ayırıcısı
	Bellek NULL ise, yeniden alan ayırmak yerine yeni bir alan ayırılmak zorundadır.
	Çünkü bellek alanının yeniden ayrılabilmesi için boş (NULL) olmaması güvenlik açısından önemlidir.
	Aksi halde bellek sızıntılarına ya da çalışma hatalarına neden olabilir.
	Bellek alanı kullanılıyorsa eğer, belirlenen boyuta göre yeniden boyutlandırılır.
	Boyutlandırma başarılı ise işaretçinin adresi döner, değilse boş (NULL) döner
*/
extern vptr mem_realloc(vptr addrptr, uintmax_s resize)
{
    // bellek adresi tahsis edilmemiş yani boş (NULL) ise
	// tekrardan bellek alanı tahsisi yapılamaz
	// yeniden bellek tahsisi sadece önceden tahsis edilmiş
	// bellek adresleri için yapılabilir
	if (ISNULL(addrptr))
    {
        #ifdef __DEBUG_MSG_MEMALLOCATION__
            DEBUG_PRINT("Bos Bir Bellek Adresi Yeniden Boyutlandirilamaz");
            DEBUG_PRINT("\"realloc\" Yerine \"malloc\" Ile Yeni Bir Bellek Tahsisi Yapiniz");
        #endif
        return NULL; // boş adres
    }

    // bellek alanını geçici işaretçiyle yeniden tahsis et
    vptr newPtr = realloc(addrptr, resize);

    // bellek adresi yeniden ayrılabilmiş ise
	// zaten bellek adresi döner yoksa da
	// yeniden tahsis edilemediğinden boş (NULL) döndürülür
	if (ISNULL(newPtr))
    {
        #ifdef __DEBUG_MSG_MEMALLOCATION__
            DEBUG_PRINT("Yeniden Bellek Alanı Tahis Edilemiyor, Bellek Musait Olmayabilir");
        #endif
		return NULL; // bellek tahsis edilemedi, boş adres
    }

    // yeni tahsis edilmiş bellek adresini
    // orijinal bellek adresine atama yap
    addrptr = newPtr;

    // başarıyla atanıp atanmadığını kontrol et
    if(addrptr != newPtr)
    {
        #ifdef __DEBUG_MSG_MEMALLOCATION__
            DEBUG_PRINT("Yeni Ayrilan Bellek Adresi Orijinal Degiskene Atanamadi");
            DEBUG_PRINT("kafkasrevan@gmail.com Bu E-posta Ile Iletisime Geciniz");
        #endif

        // yeniden boyutlandırılan adresi temizle
        mem_free(newPtr);

        // boş adres döndür
        return NULL;
    }

    // bellek adresini döndür
    return addrptr;
}

/* Bellek Alanı Ayırıp Değeri Sıfırlamak
	Belirli bir boyutta belirtilen sayı kadar bellek alanı tahsis edilip
	değeri sıfırlanır, fakat bu bellek alanının boş olması güvenlik açısından önemlidir.
	Zaten dolu bellek alanına bir şey yapılmaz ve bununla ilgili hata kodu döndürülür.
	Eğer boş bir bellek alanı verilmiş ise, kendisi yeni bir bellek alanı tahsis eder
	ve bu bellek alanındaki değerleri sıfırlar. Bellek alanı boş değilse adresini
	döndürür ama eğer boş ise NULL döndürür
*/
extern vptr mem_calloc(vptr addrptr, uintmax_s arrsize, uintmax_s varsize)
{
    // bellek adresi boş değilse calloc kullanılamaz
	// çünkü calloc yeni bir bellek alanı açar fakat diziler için daha uygundur
	if (!ISNULL(addrptr))
    {
        #ifdef __DEBUG_MSG_MEMALLOCATION__
            DEBUG_PRINT("Aktif Olarak Kullanilan Bellek Adresine Islem Yapilamaz");
            DEBUG_PRINT("\"calloc\" Kullanimi Icin Bellek Alani Bos Olmalidir");
        #endif
		return NULL; // boş adres
    }

    // bellek alanını ayır
    addrptr = calloc(arrsize, varsize);

    // bellek alanı ayrılmışsa zaten işlem başarılıdır
	// aksi halde otomatik boş (NULL) olucaktır
    #ifdef __DEBUG_MSG_MEMALLOCATION__
        if(ISNULL(addrptr)) DEBUG_PRINT("Bellek Alani Istenilen Dizi Ve Degisken Boyutunda Olusturulamadi");
    #endif
    return addrptr; // bellek adresini döndür
}

/* Bellek Temizleyici
	Bellek adresleri kullanıldıktan sonra bir işe yaramayan belleklerin dolu
	olması gelecekteki işlemler için sorundur ve bellek sızıntılarına neden olabilir.
	Bunları önlemek adına bellek alanı boş (NULL) değilse temizleriz ve adresini boş (NULL)
	yaparız, bu sayede güvenli şekilde bellek adresi temizlenmiş ve sisteme tekrar katılmış olur.
	Eğer bellek alanı zaten boş (NULL) ise boş (NULL) döndürülür ve işlem sonlandırılır.
	Bellek alanı temizlenip sisteme katılmadan adresini boş (NULL) yapmak bellek sızıntısına neden olur.
	Bu sayede güvenlik sorunları yaşanmış olur ve sistem bellek alanını geri kullanamaz
*/
extern vptr mem_free(vptr addrptr)
{
    // bellek adresi boş ise zaten tekrar boşaltılamaz
    if(ISNULL(addrptr))
    {
        #ifdef __DEBUG_MSG_MEMALLOCATION__
            DEBUG_PRINT("Bos Bir Bellek Adresi Tekrar Bosaltilamaz");
        #endif
        return NULL; // boş adres
    }

    // belleği temizlesin boşaltsın ve boş olarak ayarlasın
	free(addrptr);
	addrptr = NULL;

    // bellek adresini boş yaptık fakat yine de kontrol ediyoruz
    #ifdef __DEBUG_MSG_MEMALLOCATION__
        DEBUG_PRINT(ISNULL(addrptr) ?
            "Bellek Adresi Basariyla Temizlendi" :
            "Bellek Adresi Temizlenemedi, Adres Geri Donduruluyor"
        );
    #endif
    return addrptr; // bellek adresi temizlenmişse NULL, değilse bellek adresi döner
}