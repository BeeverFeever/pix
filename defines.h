#pragma once

#include <stdint.h>

// alignment for mat4 and vec4
#define align CGLM_ALIGN_MAT

typedef enum {
    false = 0,
    true = 1,
} bool;

typedef unsigned int uint;

typedef uint8_t byte;

typedef uint8_t        u8;
typedef uint16_t       u16;
typedef uint32_t       u32;
typedef uint64_t       u64;

typedef uint_least8_t  uleast8;
typedef uint_least16_t uleast16;
typedef uint_least32_t uleast32;
typedef uint_least64_t uleast64;

typedef uint_fast8_t   ufast8;
typedef uint_fast16_t  ufast16;
typedef uint_fast32_t  ufast32;
typedef uint_fast64_t  ufast64;

typedef int8_t         i8;
typedef int16_t        i16;
typedef int32_t        i32;
typedef int64_t        i64;

typedef int_least8_t   ileast8;
typedef int_least16_t  ileast16;
typedef int_least32_t  ileast32;
typedef int_least64_t  ileast64;

typedef int_fast8_t    ifast8;
typedef int_fast16_t   ifast16;
typedef int_fast32_t   ifast32;
typedef int_fast64_t   ifast64;

typedef uintptr_t      uintptr;
typedef intptr_t       intptr;

typedef uintmax_t      uintmax;
typedef intmax_t       intmax;
