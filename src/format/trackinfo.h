#ifndef TRACKINFO_H
#define TRACKINFO_H

#include "../include/nezplug/nezplug.h"

#ifdef __cplusplus
extern "C" {
#endif

PROTECTED NEZ_TRACKS *TRACKS_New(void);
PROTECTED void TRACKS_Delete(NEZ_TRACKS *);
PROTECTED uint8_t TRACKS_LoadM3U(NEZ_TRACKS *, const uint8_t *data, uint32_t length);


#ifdef __cplusplus
}
#endif



#endif
