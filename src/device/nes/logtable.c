#include "../../normalize.h"
#include "logtable.h"
#include <math.h>

static uint32_t lineartbl[(1 << LIN_BITS) + 1];
static uint32_t logtbl[1 << LOG_BITS];
uint32_t LinearToLog(int32_t l)
{
	return (l < 0) ? (lineartbl[-l] + 1) : lineartbl[l];
}

int32_t LogToLinear(uint32_t l, uint32_t sft)
{
	int32_t ret;
	uint32_t ofs;
	l += sft << (LOG_BITS + 1);
	sft = l >> (LOG_BITS + 1);
	if (sft >= LOG_LIN_BITS) return 0;
	ofs = (l >> 1) & ((1 << LOG_BITS) - 1);
	ret = logtbl[ofs] >> sft;
	return (l & 1) ? -ret : ret;
}

void LogTableInitialize(void)
{
	static volatile uint32_t initialized = 0;
	uint32_t i;
	double a;
	if (initialized) return;
	initialized = 1;
	for (i = 0; i < (1 << LOG_BITS); i++)
	{
		a = (1 << LOG_LIN_BITS) / pow(2, i / (double)(1 << LOG_BITS));
		logtbl[i] = (uint32_t)a;
	}
	lineartbl[0] = LOG_LIN_BITS << LOG_BITS;
	for (i = 1; i < (1 << LIN_BITS) + 1; i++)
	{
		uint32_t ua;
		a = i << (LOG_LIN_BITS - LIN_BITS);
		ua = (uint32_t)((LOG_LIN_BITS - (log(a) / log(2))) * (1 << LOG_BITS));
		lineartbl[i] = ua << 1;
	}
}
