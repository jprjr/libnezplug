#ifndef S_VRC7_H__
#define S_VRC7_H__

#include "../../normalize.h"
#include "../kmsnddev.h"

#ifdef __cplusplus
extern "C" {
#endif

void VRC7SoundInstall(NEZ_PLAY *);
void VRC7SetTone(NEZ_PLAY *, uint8_t *p, uint32_t type);

#ifdef __cplusplus
}
#endif

#endif /* S_VRC7_H__ */
