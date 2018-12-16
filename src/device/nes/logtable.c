#include "../../normalize.h"
#include "logtable.h"
#include <math.h>

#include <stdio.h>

void LogTableFree(LOG_TABLE *tbl) {
    if(tbl->lineartbl) XFREE(tbl->lineartbl);
    if(tbl->logtbl) XFREE(tbl->logtbl);
}

uint32_t LinearToLog(LOG_TABLE *tbl,int32_t l)
{
	return (l < 0) ? (tbl->lineartbl[-l] + 1) : tbl->lineartbl[l];
}

int32_t LogToLinear(LOG_TABLE *tbl, uint32_t l, uint32_t sft)
{
	int32_t ret;
	uint32_t ofs;
	l += sft << (tbl->log_bits + 1);
	sft = l >> (tbl->log_bits + 1);
	if (sft >= tbl->log_lin_bits) return 0;
	ofs = (l >> 1) & ((1 << tbl->log_bits) - 1);
	ret = tbl->logtbl[ofs] >> sft;
	return (l & 1) ? -ret : ret;
}

int32_t LogTableInitialize(LOG_TABLE *tbl)
{
	uint32_t i;
	double a;

    tbl->lineartbl = NULL;
    tbl->logtbl = NULL;

    tbl->lineartbl = XMALLOC((1 << tbl->lin_bits) + 1);
    if(tbl->lineartbl == NULL) {
        return 1;
    }

    tbl->logtbl = XMALLOC(1 << tbl->log_bits);
    if(tbl->logtbl == NULL) {
        return 1;
    }
	for (i = 0; i < (1 << tbl->log_bits); i++)
	{
		a = (1 << tbl->log_lin_bits) / pow(2, i / (double)(1 << tbl->log_bits));
		tbl->logtbl[i] = (uint32_t)a;
	}
	tbl->lineartbl[0] = tbl->log_lin_bits << tbl->log_bits;
	for (i = 1; i < (1 << tbl->lin_bits) + 1; i++)
	{
		uint32_t ua;
		a = i << (tbl->log_lin_bits - tbl->lin_bits);
		ua = (uint32_t)((tbl->log_lin_bits - (log(a) / log(2))) * (1 << tbl->log_bits));
		tbl->lineartbl[i] = ua << 1;
	}
    return 0;
}
