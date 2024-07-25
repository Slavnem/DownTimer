// Slavnem @2024-07-21
// Bellek Alanı Ayırıcısı Tanımlamaları
#ifndef DT_MEMALLOCATION_H
#define DT_MEMALLOCATION_H

// Bellek Tahsis Etme Hata Ayıklayıcısı Belirteci
// #define __DEBUG_MEMALLOCATION__

// Kütüphaneler
#include "TypeDefine.h"

// Fonksiyon Prototipleri
extern vptr mem_alloc(vptr, uint32_s);
extern vptr mem_realloc(vptr, uint32_s);
extern vptr mem_calloc(vptr, uint32_s, uint32_s);
extern vptr mem_free(vptr);

#endif