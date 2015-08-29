#pragma once
/********************************************
*  contents:   #description
*  usage:
********************************************/
/******* c++ headers *******/
#include <cstdint>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

typedef float     f32;
typedef double    f64;

struct v2
{
   f32 x;
   f32 y;
};

struct v3
{
   f32 x;
   f32 y;
   f32 z;
};


#define core_internal static
#define core_class_scope static
#define core_local_persist static
#define core_global_constant static const
