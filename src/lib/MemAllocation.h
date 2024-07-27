// Slavnem @2024-07-21
// Bellek Alanı Ayırıcısı Tanımlamaları
#ifndef DT_MEMALLOCATION_H
#define DT_MEMALLOCATION_H

// Kütüphaneler
#include "CompileConf.h"
#include "TypeDefine.h"

// Bellek Tahsis Etme Hata Ayıklayıcısı Belirteci
#ifndef __COMPILE_RELEASE_MODE__
    #define __DEBUG_MSG_MEMALLOCATION__
#endif

// Fonksiyon Prototipleri
extern vptr mem_alloc(vptr, uintmax_s);
extern vptr mem_realloc(vptr, uintmax_s);
extern vptr mem_calloc(vptr, uintmax_s, uintmax_s);
extern vptr mem_free(vptr);

#endif