#include <stdio.h>
#include "logtable.h"
#include "opltable.h"

void make_table_header(FILE *header_f,uint32_t log_bits, uint32_t lin_bits, uint32_t log_lin_bits) {
    fprintf(header_f,"typedef struct log_table_%d_%d_%d_s {\n",log_bits,lin_bits,log_lin_bits);
    fprintf(header_f,"  uint32_t lineartbl[%d];\n",(1 << lin_bits) + 1);
    fprintf(header_f,"  uint32_t logtbl[%d];\n",(1 << log_bits));
    fprintf(header_f,"} log_table_%d_%d_%d_t;\n\n",log_bits,lin_bits,log_lin_bits);
    fprintf(header_f,"PROTECTED_VAR const LOG_TABLE log_table_%d_%d_%d;\n\n",log_bits,lin_bits,log_lin_bits);
}

void make_opltable_header(FILE *header_f) {
    fprintf(header_f,"#define SINTBL_BITS 10\n");
    fprintf(header_f,"#define AMTBL_BITS 8\n");
    fprintf(header_f,"#define PMTBL_BITS 8\n");
    fprintf(header_f,"#define PM_SHIFT 9\n");
    fprintf(header_f,"#define ARTBL_BITS 7\n");
    fprintf(header_f,"#define ARTBL_SHIFT 20\n");
    fprintf(header_f,"#define TLLTBL_BITS 8\n");
    fprintf(header_f,"typedef struct opl_table_s {\n");
    fprintf(header_f,"  uint32_t sin_table[4][%u];\n",(1 << SINTBL_BITS));
    fprintf(header_f,"  uint32_t tll2log_table[%u];\n",(1 << TLLTBL_BITS));
    fprintf(header_f,"  uint32_t ar_tablelog[%u];\n",(1 << ARTBL_BITS));
    fprintf(header_f,"  uint32_t am_table1[%u];\n",(1 << AMTBL_BITS));
    fprintf(header_f,"  uint32_t pm_table1[%u];\n",(1 << PMTBL_BITS));
    fprintf(header_f,"  uint32_t ar_tablepow[%u];\n",(1 << ARTBL_BITS));
    fprintf(header_f,"  uint32_t am_table2[%u];\n",(1 << AMTBL_BITS));
    fprintf(header_f,"  uint32_t pm_table2[%u];\n",(1 << PMTBL_BITS));
    fprintf(header_f,"} opl_table_t;\n\n");
    fprintf(header_f,"PROTECTED_VAR const opl_table_t opl_table_i;\n\n");
}

void make_opltable_unheader(FILE *header_f) {
    fprintf(header_f,"#undef SINTBL_BITS\n");
    fprintf(header_f,"#undef AMTBL_BITS\n");
    fprintf(header_f,"#undef PMTBL_BITS\n");
    fprintf(header_f,"#undef PM_SHIFT\n");
    fprintf(header_f,"#undef ARTBL_BITS\n");
    fprintf(header_f,"#undef ARTBL_SHIFT\n");
    fprintf(header_f,"#undef TLLTBL_BITS\n");
}

void make_opltable(FILE *out) {
    OPL_TABLE tbl = { 0 };
    OplTableInitialize(&tbl);
    int i = 0;
    int j = 0;

    fprintf(out,"#ifndef OPL_TABLE_C\n");
    fprintf(out,"#define OPL_TABLE_C\n\n");
    fprintf(out,"#include \"../normalize.h\"\n");
    fprintf(out,"#include \"opl_table.h\"\n");

    fprintf(out,"PROTECTED_VAR const opl_table_t opl_table_i = {\n");
    fprintf(out,"  .sin_table = {\n");
    for(i=0; i<4; i++) {
        fprintf(out,"    {\n");
        for(j=0;j< 1 << SINTBL_BITS; j++) {
            fprintf(out,"      %u,\n",tbl.sin_table[i][j]);
        }
        fprintf(out,"    },\n");
    }
    fprintf(out,"  },\n");

    fprintf(out,"  .tll2log_table = {\n");
    for(i=0;i< 1 << TLLTBL_BITS; i++) {
        fprintf(out,"    %u,\n",tbl.tll2log_table[i]);
    }
    fprintf(out,"  },\n");

    fprintf(out,"  .ar_tablelog = {\n");
    for(i=0;i< 1 << ARTBL_BITS; i++) {
        fprintf(out,"    %u,\n",tbl.ar_tablelog[i]);
    }
    fprintf(out,"  },\n");

    fprintf(out,"  .am_table1 = {\n");
    for(i=0;i< 1 << AMTBL_BITS; i++) {
        fprintf(out,"    %u,\n",tbl.am_table1[i]);
    }
    fprintf(out,"  },\n");

    fprintf(out,"  .pm_table1 = {\n");
    for(i=0;i< 1 << PMTBL_BITS; i++) {
        fprintf(out,"    %u,\n",tbl.pm_table1[i]);
    }
    fprintf(out,"  },\n");

    fprintf(out,"  .ar_tablepow = {\n");
    for(i=0;i< 1 << ARTBL_BITS; i++) {
        fprintf(out,"    %u,\n",tbl.ar_tablepow[i]);
    }
    fprintf(out,"  },\n");

    fprintf(out,"  .am_table2 = {\n");
    for(i=0;i< 1 << AMTBL_BITS; i++) {
        fprintf(out,"    %u,\n",tbl.am_table2[i]);
    }
    fprintf(out,"  },\n");

    fprintf(out,"  .pm_table2 = {\n");
    for(i=0;i< 1 << PMTBL_BITS; i++) {
        fprintf(out,"    %u,\n",tbl.pm_table2[i]);
    }
    fprintf(out,"  },\n");
    fprintf(out,"};\n\n");

    fprintf(out,"#endif\n");

}

void make_table(FILE *out,uint32_t log_bits, uint32_t lin_bits, uint32_t log_lin_bits) {
    LOG_TABLE tbl = { 0 };
    tbl.log_bits = log_bits;
    tbl.lin_bits = lin_bits;
    tbl.log_lin_bits = log_lin_bits;
    int i = 0;

    LogTableInitialize(&tbl);
    fprintf(out,"#ifndef LOG_TABLE_%d_%d_%d_C\n",log_bits,lin_bits,log_lin_bits);
    fprintf(out,"#define LOG_TABLE_%d_%d_%d_C\n\n",log_bits,lin_bits,log_lin_bits);
    fprintf(out,"#include \"../normalize.h\"\n");
    fprintf(out,"#include \"log_table.h\"\n");

    fprintf(out,"PROTECTED_VAR const log_table_%d_%d_%d_t log_table_%d_%d_%d_i = {\n",log_bits,lin_bits,log_lin_bits,log_bits,lin_bits,log_lin_bits);
    fprintf(out,"  .lineartbl = {\n");
    for(i=0; i< (1 << lin_bits) +1; i++) {
        fprintf(out,"    %d,\n",tbl.lineartbl[i]);
    }
    fprintf(out,"  },\n");
    fprintf(out,"  .logtbl = {\n");
    for(i=0;i<(1<<log_bits); i++) {
        fprintf(out,"    %d,\n",tbl.logtbl[i]);
    }
    fprintf(out,"  }\n");
    fprintf(out,"};\n\n");

    fprintf(out,"PROTECTED_VAR const LOG_TABLE log_table_%d_%d_%d = {\n",log_bits,lin_bits,log_lin_bits);
    fprintf(out,"  .log_bits = %d,\n",log_bits);
    fprintf(out,"  .lin_bits = %d,\n",lin_bits);
    fprintf(out,"  .log_lin_bits = %d,\n",log_lin_bits);
    fprintf(out,"  .lineartbl = log_table_%d_%d_%d_i.lineartbl,\n",log_bits,lin_bits,log_lin_bits);
    fprintf(out,"  .logtbl    = log_table_%d_%d_%d_i.logtbl,\n",log_bits,lin_bits,log_lin_bits);
    fprintf(out,"};\n\n");
    fprintf(out,"#endif\n");
    LogTableFree(&tbl);

}

int main(void) {
    FILE *header_f = fopen("../src/logtable/log_table.h","w");
    if(header_f == NULL) {
        fprintf(stderr,"Error opening log_table.h");
        return 1;
    }
    fprintf(header_f,"#ifndef LOG_TABLE_H\n");
    fprintf(header_f,"#define LOG_TABLE_H\n\n");
    fprintf(header_f,"#include \"../device/logtable.h\"\n\n");
    make_table_header(header_f,12,8,30);
    make_table_header(header_f,12,7,30);
    fprintf(header_f,"#endif\n");
    fclose(header_f);

    FILE *out = fopen("../src/logtable/log_table_12_8_30.c","w");
    if(out == NULL) {
        fprintf(stderr,"Error opening log_table_12_8_30.c");
        return 1;
    }
    make_table(out,12,8,30);
    fclose(out);

    out = fopen("../src/logtable/log_table_12_7_30.c","w");
    if(out == NULL) {
        fprintf(stderr,"Error opening log_table_12_8_30.c");
        return 1;
    }
    make_table(out,12,7,30);
    fclose(out);

    header_f = fopen("../src/opltable/opl_table.h","w");
    if(header_f == NULL) {
        fprintf(stderr,"Error opening opl_table.h");
        return 1;
    }
    fprintf(header_f,"#ifndef OPL_TABLE_H\n");
    fprintf(header_f,"#define OPL_TABLE_H\n");
    make_opltable_header(header_f);
    fprintf(header_f,"#endif\n");
    fclose(header_f);

    header_f = fopen("../src/opltable/opl_tableu.h","w");
    if(header_f == NULL) {
        fprintf(stderr,"Error opening opl_tableu.h");
        return 1;
    }
    make_opltable_unheader(header_f);
    fclose(header_f);

    out = fopen("../src/opltable/opl_table.c","w");
    if(out == NULL) {
        fprintf(stderr,"Error opening opl_table.c");
        return 1;
    }
    make_opltable(out);
    fclose(out);

    return 0;

}
