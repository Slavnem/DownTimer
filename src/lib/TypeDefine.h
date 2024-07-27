// Slavnem @2024-07-21
// Değişken Tür Tanımlamaları
#ifndef DT_TYPEDEFINE_H
#define DT_TYPEDEFINE_H

// Bool
#define FALSE 0
#define TRUE 1
typedef char boolean;
typedef const boolean boolean_s;
typedef const boolean* booleanptr_a;
typedef const boolean* const booleanptr_s;

// 8 Bit Tamsayılar
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef const int8_t int8_s;
typedef const uint8_t uint8_s;

// 16 Bit Tamsayılar
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef const int16_t int16_s;
typedef const uint16_t uint16_s;

// 32 Bit Tamsayılar
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef const int32_t int32_s;
typedef const uint32_t uint32_s;

// 64 Bit Tamsayılar
#ifdef __i386__ // Windows x86 & Linux x86
    typedef signed long int intmax_t;
    typedef unsigned long int uintmax_t;
    typedef const intmax_t intmax_s;
    typedef const uintmax_t uintmax_s;
#elif defined __x86_64__ // Windows x64 & Linux x64
    typedef signed long long int intmax_t;
    typedef unsigned long long int uintmax_t;
    typedef const intmax_t intmax_s;
    typedef const uintmax_t uintmax_s;
#endif

// Ondalıklar
typedef float double32_t;
typedef double double64_t;
typedef long double double128_t;

typedef const double32_t double32_s;
typedef const double64_t double64_s;
typedef const double128_t double128_s;

// İşaretçiler
typedef void* vptr;
typedef char* string;
typedef const char* string_a;
typedef const char* const string_s;

// 16 Bit Tamsayı İşaretçiler
typedef int16_t* int16ptr_t;
typedef uint16_t* uint16ptr_t;
typedef const int16_t* const int16ptr_s;
typedef const uint16_t* const uint16ptr_s;

// 32 Bit Tamsayı İşaretçiler
typedef int32_t* int32ptr_t;
typedef uint32_t* uint32ptr_t;
typedef const int32_t* const int32ptr_s;
typedef const uint32_t* const uint32ptr_s;

// 64 Bit Tamsayı İşaretçiler
typedef intmax_t* intmaxptr_t;
typedef uintmax_t* uintmaxptr_t;
typedef const intmax_t* const intmaxptr_s;
typedef const uintmax_t* const uintmaxptr_s;

// Ondalık İşaretçiler
typedef double32_t* double32ptr;
typedef double64_t* double64ptr;
typedef double128_t* double128ptr;

typedef const double32_t* const double32ptr_s;
typedef const double64_t* const double64ptr_s;
typedef const double128_t* const double128ptr_s;

#endif