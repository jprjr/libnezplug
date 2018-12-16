#ifndef LOGTABLE_H_
#define LOGTABLE_H_

#include "../../normalize.h"

typedef struct LOG_TABLE_ LOG_TABLE;
struct LOG_TABLE_ {
    uint32_t log_bits;
    uint32_t lin_bits;
    uint32_t log_lin_bits;
    uint32_t *lineartbl;
    uint32_t *logtbl;
};

int32_t LogTableInitialize(LOG_TABLE *tbl);
void LogTableFree(LOG_TABLE *tbl);
int32_t LogToLinear(LOG_TABLE *tbl, uint32_t l, uint32_t sft);
uint32_t LinearToLog(LOG_TABLE *tbl, int32_t l);

#if 0

#define LOG_BITS 12
#define LIN_BITS 8
#define LOG_LIN_BITS 30

uint32_t LinearToLog(int32_t l);
int32_t LogToLinear(uint32_t l, uint32_t sft);
void LogTableInitialize(void);

#endif

#endif
