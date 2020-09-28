#pragma once

#include <string>


#ifdef CE_WIN32

typedef unsigned __int8  ceUInt8;
typedef unsigned __int16 ceUInt16;
typedef unsigned __int32 ceUInt32;
typedef unsigned __int64 ceUInt64;

typedef signed __int8  ceInt8;
typedef signed __int16 ceInt16;
typedef signed __int32 ceInt32;
typedef signed __int64 ceInt64;

#endif


typedef unsigned long ceID;
#define ceID_Undefined ~0x00


typedef size_t csSize;

