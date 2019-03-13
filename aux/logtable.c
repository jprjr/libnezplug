#include "logtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void LogTableFree(LOG_TABLE *tbl) {
    if(tbl->lineartbl) free(tbl->lineartbl);
    if(tbl->logtbl) free(tbl->logtbl);
}

int32_t LogTableInitialize(LOG_TABLE *tbl)
{
	uint32_t i;
	double a;

    tbl->lineartbl = NULL;
    tbl->logtbl = NULL;

    tbl->lineartbl = malloc(sizeof(uint32_t) * ((1 << tbl->lin_bits) + 1));
    if(tbl->lineartbl == NULL) {
        return 1;
    }

    tbl->logtbl = malloc(sizeof(uint32_t) * (1 << tbl->log_bits));
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
