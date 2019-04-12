#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#include <stddef.h>
#include <cstdint>


//Standard Types

typedef char char8;
typedef unsigned char uchar8;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef float float32;
typedef double float64;


#define Kilobytes(bytes) (bytes * 1024LL)
#define Megabytes(kBytes) (Kilobytes(kBytes) * 1024LL)
#define Gigabytes(mBytes) (Megabytes(mBytes) * 1024LL)
#define Terabytes(gBytes) (Gigabytes(gBytes) * 1024LL)



#endif //TYPE_INFO_H
