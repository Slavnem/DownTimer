// Slavnem @2024-07-21
// Bellek Alanı Ayırıcısı Tanımlamaları
#ifndef DT_MEMALLOCATION_H
#define DT_MEMALLOCATION_H

// Bellek Tahsis Etme Hata Ayıklayıcısı Belirteci
// #define __DEBUG_MEMALLOCATION__

// Kütüphaneler
#include "TypeDefine.h"

// Fonksiyon Prototipleri
extern vptr mem_alloc(vptr, uintmax_s);
extern vptr mem_realloc(vptr, uintmax_s);
extern vptr mem_calloc(vptr, uintmax_s, uintmax_s);
extern vptr mem_free(vptr);

#endif