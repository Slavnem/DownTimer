// Slavnem @2024-07-21
// Değişken Tür Tanımlamaları
#ifndef DT_TYPEDEFINE_H
#define DT_TYPEDEFINE_H

// Bool
#define FALSE 0
#define TRUE 1
typedef char boolean;

// 8 Bit Tamsayılar
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef const int8_t int8_ts;
typedef const uint8_t uint8_s;

// 16 Bit Tamsayılar
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef const int16_t int16_ts;
typedef const uint16_t uint16_s;

// 32 Bit Tamsayılar
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef const int32_t int32_st;
typedef const uint32_t uint32_s;

// Ondalıklar
typedef float double32;
typedef double double64;
typedef long double double128;

typedef const double32 double32_s;
typedef const double64 double64_s;
typedef const double128 double128_s;

// İşaretçiler
typedef void* vptr;
typedef char* string;
typedef const char* string_a;
typedef const char* const string_s;

// 16 Bit Tamsayı İşaretçiler
typedef int16_t* int16ptr_t;
typedef uint16_t* uint16ptr_t;
typedef const int16ptr_t int16ptr_s;
typedef const uint16ptr_t uint16ptr_s;

// 32 Bit Tamsayı İşaretçiler
typedef int32_t* int32ptr_t;
typedef uint32_t* uint32ptr_t;
typedef const int32ptr_t int32ptr_s;
typedef const uint32ptr_t uint32ptr_s;

// Ondalık İşaretçiler
typedef double32* double32ptr;
typedef double64* double64ptr;
typedef double128* double128ptr;

typedef const double32ptr double32ptr_s;
typedef const double64ptr double64ptr_s;
typedef const double128ptr double128ptr_s;

#endif