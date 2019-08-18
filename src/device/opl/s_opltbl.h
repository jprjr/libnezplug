#ifndef S_OPLTBL_H__
#define S_OPLTBL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SINTBL_BITS 10
#define AMTBL_BITS 8
#define PMTBL_BITS 8
#define PM_SHIFT 9
#define ARTBL_BITS 7
#define ARTBL_SHIFT 20
#define TLLTBL_BITS 8
typedef struct
{
	void *ctx;
	void (*release)(void *ctx);
	uint32_t sin_table[4][1 << SINTBL_BITS];
	uint32_t tll2log_table[1 << TLLTBL_BITS];
	uint32_t ar_tablelog[1 << ARTBL_BITS];
	uint32_t am_table1[1 << AMTBL_BITS];
	uint32_t pm_table1[1 << PMTBL_BITS];
	uint32_t ar_tablepow[1 << ARTBL_BITS];
	uint32_t am_table2[1 << AMTBL_BITS];
	uint32_t pm_table2[1 << PMTBL_BITS];
} KMIF_OPLTABLE;

PROTECTED KMIF_OPLTABLE *OplTableAddRef(void);

#ifdef __cplusplus
}
#endif

#endif /* S_OPLTBL_H__ */
