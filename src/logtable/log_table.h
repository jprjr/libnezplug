#ifndef LOG_TABLE_H
#define LOG_TABLE_H

typedef struct log_table_12_8_30_s {
  uint32_t log_bits;
  uint32_t lin_bits;
  uint32_t log_lin_bits;
  uint32_t lineartbl[257];
  uint32_t logtbl[4096];
} log_table_12_8_30_t;

typedef struct log_table_12_7_30_s {
  uint32_t log_bits;
  uint32_t lin_bits;
  uint32_t log_lin_bits;
  uint32_t lineartbl[129];
  uint32_t logtbl[4096];
} log_table_12_7_30_t;

#endif
