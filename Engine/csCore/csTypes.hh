#pragma once

#include <string>



namespace cs {


#ifdef CS_WIN32

typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef signed __int8  int8_t;
typedef signed __int16 int16_t;
typedef signed __int32 int32_t;
typedef signed __int64 int64_t;


typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef signed __int8  i8;
typedef signed __int16 i16;
typedef signed __int32 i32;
typedef signed __int64 i64;

#else
    typedef  uint8_t  u8;
    typedef  uint16_t u16;
    typedef  uint32_t u32;
    typedef  uint64_t u64;

    typedef int8_t  i8;
    typedef int16_t  i16;
    typedef int32_t  i32;
    typedef int64_t  i64;
#endif



typedef size_t Size;


}
