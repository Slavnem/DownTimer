// Slavnem @2024-07-07
// Downtimer Macro
#pragma once

#ifndef DT_MACRO_H
#define DT_MACRO_H

// Null mu?
#define isNull(ptr) (ptr == NULL)

// Dizi mi?
#define isArray(arr) (!(sizeof(arr) == sizeof(void*)))

// Yüksek mi?
#define isHigher(val1, val2) (val1 > val2)

// Düşük mü?
#define isLower(val1, val2) (val1 < val2)

// Dizi Boyutu
#define arrSize(arr) (isArray(arr) ? (sizeof(arr) / sizeof(arr[0])) : 1)

// Metin Dizisi Güvenli Boyutu
#define strArrMax(str) (isArray(str) ? ((sizeof(str) / sizeof(str[0])) - 1) : 0)

// Metin Uzunluğu
#define strLength(str) ({ \
    size_t count = 0; \
    while(str[count] != '\0') \
        ++count; \
    count; \
})

#endif

#ifndef DT_MACRO_H
#error "DT_MACRO_H!"
#endif