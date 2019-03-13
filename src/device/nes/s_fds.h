#ifndef S_FDS_H__
#define S_FDS_H__

#include <nezplug/nezplug.h>
#include "../../normalize.h"

#ifdef __cplusplus
extern "C" {
#endif

PROTECTED void FDSSoundInstall(NEZ_PLAY*);
PROTECTED void FDSSelect(NEZ_PLAY*, unsigned type);

#ifdef __cplusplus
}
#endif
#endif /* S_FDS_H__ */
