#include <stdio.h>
#include "logtable.h"

void make_table_header(FILE *header_f,uint32_t log_bits, uint32_t lin_bits, uint32_t log_lin_bits) {
    fprintf(header_f,"typedef struct log_table_%d_%d_%d_s {\n",log_bits,lin_bits,log_lin_bits);
    fprintf(header_f,"  uint32_t lineartbl[%d];\n",(1 << lin_bits) + 1);
    fprintf(header_f,"  uint32_t logtbl[%d];\n",(1 << log_bits));
    fprintf(header_f,"} log_table_%d_%d_%d_t;\n\n",log_bits,lin_bits,log_lin_bits);
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

    fprintf(out,"static const log_table_%d_%d_%d_t log_table_%d_%d_%d_i = {\n",log_bits,lin_bits,log_lin_bits,log_bits,lin_bits,log_lin_bits);
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

    fprintf(out,"static const LOG_TABLE log_table_%d_%d_%d = {\n",log_bits,lin_bits,log_lin_bits);
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
        fprintf(stderr,"Error opening log_tables.h");
        return 1;
    }
    fprintf(header_f,"#ifndef LOG_TABLE_H\n");
    fprintf(header_f,"#define LOG_TABLE_H\n\n");
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

    return 0;

}
