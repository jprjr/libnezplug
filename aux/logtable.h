#ifndef LOGTABLE_H_
#define LOGTABLE_H_
#include <stdint.h>

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

#endif
