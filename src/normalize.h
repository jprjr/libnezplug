#ifndef NORMALIZE_H__
#define NORMALIZE_H__

#include "include/nezplug/pstdint.h"

#if defined(_MSC_VER)
#define NEVER_REACH __assume(0);
#elif defined(__GNUC__)
#define __inline        __attribute__((always_inline)) inline
#else
#define __inline
#endif

#ifndef NEVER_REACH
#define NEVER_REACH
#endif

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

#ifdef DO_AMALGATION
#define PROTECTED static
#else
#define PROTECTED
#endif

#endif /* NORMALIZE_H__ */
