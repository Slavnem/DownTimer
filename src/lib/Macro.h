// Slavnem @2024-07-21
// Makro Tanımlamarı
#ifndef DT_MACRO_H
#define DT_MACRO_H

// Kütüphaneler
#include <stdio.h>

// Null mu?
/*
    Genel amaç olarak işaretçi değişkenin
    NULL yani boş olup olmadığını kontrol etmek için
*/
#define ISNULL(ptr) ((ptr) == NULL)

// Dizi mi?
/*
    Bir değişkenin birden çok değişkeni içeren
    bir dizi mi yoksa sadece tek bir değişken mi
    olduğunu anlamamızı sağlayan basit bir yöntem
*/
#define ISARRAY(var) (sizeof(var) != sizeof(void*))

// Dizi Boyutu
/*
    Önceden tanımladığımız dizi kontrolü makrosunu kullanarak
    bir dizi ise eğer, boyutunu hesaplayacağız fakat
    aksi halde dizi olmadığı için 0 olucaktır
*/
#define SIZEARRAY(arr) (ISARRAY(arr) ? (sizeof(arr) / sizeof(arr[0])) :  0)

// Güvenli Metin Dizi Boyutu
/*
    Metinler yani karakter dizilerinin alabileceği bir karakter sınırı var
    fakat bu sınırın sonu belirtilmesse diğer bellek adreslerine
    taşabilir ve bu istenmeyen bir hatadır, bu yüzden karakter dizininin
    desteklediği en fazla boyutun 1 düşüğü güvenli boyut miktarıdır
    aksi halde bir dizi değilse 0 dönülecektir
*/
#define SAFESIZESTR(arr) (ISARRAY(arr) ? (sizeof(arr) - 1) : 0)

// Metin Uzunluğu
/*
    Bir metinin sonu varsa bu metinin boyutu güvenli şekilde hesaplanabilir
    biz de bunu yaparak metinin boyutunu hesaplayacağız fakat eğer
    karakter dizininin sonu belirtilmemişse, bu yüzden en fazla desteklediği
    boyutu aşıp dizi sonu işaretini görene kadar saymaya devam edebilir
    bu yüzden karakter dizisi aslında güvenli bir karakter dizisi olmalıdır
*/
#define STRLENGTH(str) ({ \
    size_t count = 0; \
    while(str[count] != '\0') \
        ++count; \
    count; \
})

// Hata Derleyicisi Çıktısı
/*
    Hata derleyici aktif olduğunda kullanılacak
    basitçe hata çıktısı anlaşılacak şekilde
    gelen metini çıktı verebilen bir makro
*/
#define DEBUG_PRINT_MSG(title, text) ({ \
    printf("\n[MSG] %s: %s\n", \
        !ISNULL(title) ? title : "DEBUG", \
        !ISNULL(text) ? text : "Debug" \
    ); \
})

#define DEBUG_PRINT_ERROR(title, text) ({ \
    printf("\n[ERROR] %s: %s\n", \
        !ISNULL(title) ? title : "DEBUG ERROR", \
        !ISNULL(text) ? text : "Debug" \
    ); \
})

#define DEBUG_PRINT_WARNING(title, text) ({ \
    printf("\n[WARN] %s: %s\n", \
        !ISNULL(title) ? title : "DEBUG WARNING", \
        !ISNULL(text) ? text : "Debug" \
    ); \
})

#endif