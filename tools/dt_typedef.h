// Slavnem @2024-07-07
// Downtimer Type Definer
#pragma once

#ifndef DT_TYPEDEF_H
#define DT_TYPEDEF_H

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
typedef uint16_t* int16ptr_u;
typedef const int16ptr_t const int16ptr_st;
typedef const int16ptr_u const int16ptr_su;

// 32 Bit Tamsayı İşaretçiler
typedef int32_t* int32ptr_t;
typedef uint32_t* int32ptr_u;
typedef const int32ptr_t const int32ptr_st;
typedef const int32ptr_u const int32ptr_su;

// Ondalık İşaretçiler
typedef double32* double32ptr;
typedef double64* double64ptr;
typedef double128* double128ptr;

typedef const double32ptr const double32ptr_s;
typedef const double64ptr const double64ptr_s;
typedef const double128ptr const double128ptr_s;

#endif

#ifndef DT_TYPEDEF_H
#error "DT_TYPEDEF_H!"
#endif