/* Project headers */
#include "common/cso.h"

#ifndef _WIN32
/* critical section object (ANSI-C) */
unsigned CriticalSectionEnter(void *a)
{
	volatile long *p = a;
	if (--*(volatile long *)a == 0) return 1;
	++*(volatile long *)a;
	return 0;
}
void CriticalSectionLeave(void *a)
{
	--*(volatile long *)a;
}
void CriticalSectionInitialize(void *a)
{
	*(volatile long *)a = 1;
}
#endif
