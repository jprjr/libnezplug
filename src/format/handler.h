#ifndef HANDLER_H__
#define HANDLER_H__

#include <nezplug/nezplug.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NES_RESET_SYS_FIRST 4
#define NES_RESET_SYS_NOMAL 8
#define NES_RESET_SYS_LAST 12

void NESReset(NEZ_PLAY *);
void NESResetHandlerInstall(NEZ_NES_RESET_HANDLER**, const NEZ_NES_RESET_HANDLER *ph);
void NESTerminate(NEZ_PLAY *);
void NESTerminateHandlerInstall(NEZ_NES_TERMINATE_HANDLER**, const NEZ_NES_TERMINATE_HANDLER *ph);
void NESHandlerInitialize(NEZ_NES_RESET_HANDLER**, NEZ_NES_TERMINATE_HANDLER *);
void NESHandlerTerminate(NEZ_NES_RESET_HANDLER**, NEZ_NES_TERMINATE_HANDLER *);

#ifdef __cplusplus
}
#endif

#endif /* HANDLER_H__ */
