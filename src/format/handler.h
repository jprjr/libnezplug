#ifndef HANDLER_H__
#define HANDLER_H__

#include <nezplug/nezplug.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NES_RESET_SYS_FIRST 4
#define NES_RESET_SYS_NOMAL 8
#define NES_RESET_SYS_LAST 12

void NESReset(void*);
void NESResetHandlerInstall(NES_RESET_HANDLER**, const NES_RESET_HANDLER *ph);
void NESTerminate(void*);
void NESTerminateHandlerInstall(NES_TERMINATE_HANDLER**, const NES_TERMINATE_HANDLER *ph);
void NESHandlerInitialize(NES_RESET_HANDLER**, NES_TERMINATE_HANDLER *);
void NESHandlerTerminate(NES_RESET_HANDLER**, NES_TERMINATE_HANDLER *);

#ifdef __cplusplus
}
#endif

#endif /* HANDLER_H__ */
