#ifndef NESTYPES_H__
#define NESTYPES_H__

#if defined(_MSC_VER)
#define NEVER_REACH __assume(0);
#elif defined(__BORLANDC__)
#define __fastcall __msfastcall
#elif defined(__GNUC__)
#define __inline		__inline__
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

typedef int				Int;
typedef unsigned int	Uint;
typedef signed int		Int32;
typedef signed long		Int64;
typedef unsigned long	Uint64;
#ifndef Uint32
#define Uint32 unsigned int
#endif
#ifndef Uint8
#define Uint8 unsigned char
#endif
typedef signed short	Int16;
typedef unsigned short	Uint16;
typedef signed char		Int8;
typedef char			Char;

#include <malloc.h>
#include <memory.h>

#ifdef WIN32
#include <stdlib.h>
#define XSLEEP(t)		_sleep(t)
#else
#include <windows.h>
#include <stdlib.h>
#define XSLEEP(t)		sleep(t)
#endif
#define XMALLOC(s)		malloc(s)
#define XREALLOC(p,s)	realloc(p,s)
#define XFREE(p)		free(p)
#define XMEMCPY(d,s,n)	memcpy(d,s,n)
#define XMEMSET(d,c,n)	memset(d,c,n)

#endif /* NESTYPES_H__ */
