#ifndef NESTYPES_H__
#define NESTYPES_H__

#if defined(_MSC_VER)
#define NEVER_REACH __assume(0);
#elif defined(__BORLANDC__)
#define __fastcall __msfastcall
#elif defined(__GNUC__)
#define __inline        __attribute__((always_inline)) inline
#ifndef __fastcall
#define __fastcall
#endif
#else
#define __inline
#define __fastcall
#endif
#ifndef NEVER_REACH
#define NEVER_REACH
#endif

#include "pstdint.h"

typedef int32_t         Int;
typedef uint32_t        Uint;
typedef int8_t          Int8;
typedef uint8_t         Uint8;
typedef int16_t         Int16;
typedef uint16_t        Uint16;
typedef int32_t         Int32;
typedef uint32_t        Uint32;
typedef int64_t         Int64;
typedef uint64_t        Uint64;
typedef char            Char;

#include <stdlib.h>
#include <memory.h>

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#define XSLEEP(t)         _sleep(t)
#else
#include <unistd.h>
#define XSLEEP(t)         sleep(t)
#endif
#define XMALLOC(s)        malloc(s)
#define XREALLOC(p,s)     realloc(p,s)
#define XFREE(p)          free(p)
#define XMEMCPY(d,s,n)    memcpy(d,s,n)
#define XMEMSET(d,c,n)    memset(d,c,n)

#endif /* NESTYPES_H__ */
