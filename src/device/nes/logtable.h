#include "../../normalize.h"

#define LOG_BITS 12
#define LIN_BITS 8
#define LOG_LIN_BITS 30

uint32_t LinearToLog(int32_t l);
int32_t LogToLinear(uint32_t l, uint32_t sft);
void LogTableInitialize(void);
