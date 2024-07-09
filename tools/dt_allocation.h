// Slavnem @2024-06-29
// Downtimer Allocation
#pragma once

#ifndef DT_ALLOCATION_H
#define DT_ALLOCATION_H

// Standart Kütüphaneler
#include <stdlib.h>

// DT Kütüphaneler
#include "dt_typedef.h"
#include "dt_macro.h"

//////////////////////////////////
// Fonksiyon Prototipleri
//////////////////////////////////
extern vptr dtaf_mallocation(vptr addrptr, uint8_s addrsize);
extern vptr dtaf_reallocation(vptr addrptr, uint8_s addrsize);
extern vptr dtaf_calloc(vptr addrptr, uint8_s addrsize, uint8_s arrsize);
extern vptr dtaf_memfree(vptr addrptr);

//////////////////////////////////
// Fonksiyonlar
//////////////////////////////////
// Bellek Alan Ayırıcısı
/*
	Bellek NULL değilse zaten belirli bir adres tarafından
	kullanılıyordur. Bellek alanı ayırabilmemiz için boş olması yani NULL olması
	gerek. Ancak bu sayede yeni bir bellek alan ayırma yapabiliriz.
	Alan ayırma işlemi sonrası boş değilse (NULL), yeni bellek alanı başarıyla ayrılmıştır
	bu sayede ayrılan alanın adresini döndürürüz, bellek alanı ayrılamamışsa
	değişkenin adresini döndürdüğümüz için zaten boş (NULL) döndürür
*/
extern vptr dtaf_mallocation(vptr addrptr, uint8_s addrsize)
{
	// yeni bellek alanı tahsis edilebilmesi için
	// bellek alanını hali hazırda boş (NULL) olması lazım
	// aksi halde tanımlı bir adrese yeniden tahsis yapılamaz
	if (isNull(addrptr))
		return NULL;

	// yeni bellek alanı tahsis etsin
	addrptr = malloc(addrsize);

	// bellek alanı tahsis edilmiş ise bellek adresi,
	// değilse boş (NULL) döner
	return addrptr;
}

// Bellek Yeniden Alan Ayırıcısı
/*
	Bellek NULL ise, yeniden alan ayırmak yerine yeni bir alan ayırılmak zorundadır.
	Çünkü bellek alanının yeniden ayrılabilmesi için boş (NULL) olmaması güvenlik açısından önemlidir.
	Aksi halde bellek sızıntılarına ya da çalışma hatalarına neden olabilir.
	Bellek alanı kullanılıyorsa eğer, belirlenen boyuta göre yeniden boyutlandırılır.
	Boyutlandırma başarılı ise işaretçinin adresi döner, değilse boş (NULL) döner
*/
extern vptr dtaf_reallocation(vptr addrptr, uint8_s addrsize)
{
	// bellek adresi tahsis edilmemiş yani boş (NULL) ise
	// tekrardan bellek alanı tahsisi yapılamaz
	// yeniden bellek tahsisi sadece önceden tahsis edilmiş
	// bellek adresleri için yapılabilir
	if (isNull(addrptr))
		return NULL;

	// bellek alanını geçici işaretçiyle yeniden tahsis et
	vptr tempPtr = realloc(addrptr, addrsize);

	// bellek adresi yeniden ayrılabilmiş ise
	// zaten bellek adresi döner yoksa da
	// yeniden tahsis edilemediğinden boş (NULL) döndürülür
	if (isNull(tempPtr))
		return NULL;

	// yeniden tahsis edilmiş bellek adresini kaydet
	addrptr = tempPtr;

	// bellek adresini döndür
	return addrptr;
}

// Bellek Alanı Ayırıp Değeri Sıfırlamak
/*
	Belirli bir boyutta belirtilen sayı kadar bellek alanı tahsis edilip
	değeri sıfırlanır, fakat bu bellek alanının boş olması güvenlik açısından önemlidir.
	Zaten dolu bellek alanına bir şey yapılmaz ve bununla ilgili hata kodu döndürülür.
	Eğer boş bir bellek alanı verilmiş ise, kendisi yeni bir bellek alanı tahsis eder
	ve bu bellek alanındaki değerleri sıfırlar. Bellek alanı boş değilse adresini
	döndürür ama eğer boş ise NULL döndürür
*/
extern vptr dtaf_calloc(vptr addrptr, uint8_s addrsize, uint8_s arrsize)
{
	// bellek adresi boş değilse calloc kullanılamaz
	// çünkü calloc yeni bir bellek alanı açar fakat diziler için daha uygundur
	if (!isNull(addrptr))
		return NULL;

	// bellek alanı ayır
	addrptr = calloc(arrsize, addrsize);

	// bellek alanı ayrılmışsa zaten işlem başarılıdır
	// aksi halde otomatik boş (NULL) olucaktır
	return addrptr;
}

// Bellek Temizleyici
/*
	Bellek adresleri kullanıldıktan sonra bir işe yaramayan belleklerin dolu
	olması gelecekteki işlemler için sorundur ve bellek sızıntılarına neden olabilir.
	Bunları önlemek adına bellek alanı boş (NULL) değilse temizleriz ve adresini boş (NULL)
	yaparız, bu sayede güvenli şekilde bellek adresi temizlenmiş ve sisteme tekrar katılmış olur.
	Eğer bellek alanı zaten boş (NULL) ise boş (NULL) döndürülür ve işlem sonlandırılır.
	Bellek alanı temizlenip sisteme katılmadan adresini boş (NULL) yapmak bellek sızıntısına neden olur.
	Bu sayede güvenlik sorunları yaşanmış olur ve sistem bellek alanını geri kullanamaz
*/
extern vptr dtaf_memfree(vptr addrptr)
{
	// bellek adresi boş ise zaten tekrar boşaltılamaz
	if (isNull(addrptr))
		return NULL;

	// belleği temizlesin boşaltsın ve boş olarak ayarlasın
	free(addrptr);
	addrptr = NULL;

	// bellek adresini boş yaptık fakat yine de döndürüyoruz
	// çünkü eğer boş (NULL) dönerse temizlenme başarıyla tamamlanmıştır
	// aksi halde hala bir bellek adresi döndürüyorsa
	// bellek tam olarak temizlenememiştir
	return addrptr;
}

#endif

#ifndef DT_ALLOCATION_H
#error "DT_ALLOCATION_H!"
#endif