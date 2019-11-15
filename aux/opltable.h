#ifndef OPLTABLE_H__
#define OPLTABLE_H__

#include <stdint.h>

#define SINTBL_BITS 10
#define AMTBL_BITS 8
#define PMTBL_BITS 8
#define PM_SHIFT 9
#define ARTBL_BITS 7
#define ARTBL_SHIFT 20
#define TLLTBL_BITS 8

typedef struct OPL_TABLE_ OPL_TABLE;
struct OPL_TABLE_ {
	uint32_t sin_table[4][1 << SINTBL_BITS];
	uint32_t tll2log_table[1 << TLLTBL_BITS];
	uint32_t ar_tablelog[1 << ARTBL_BITS];
	uint32_t am_table1[1 << AMTBL_BITS];
	uint32_t pm_table1[1 << PMTBL_BITS];
	uint32_t ar_tablepow[1 << ARTBL_BITS];
	uint32_t am_table2[1 << AMTBL_BITS];
	uint32_t pm_table2[1 << PMTBL_BITS];
};


#ifdef __cplusplus
extern "C" {
#endif

void OplTableInitialize(OPL_TABLE *tbl);

#ifdef __cplusplus
}
#endif


#endif /* S_OPLTBL_H__ */
