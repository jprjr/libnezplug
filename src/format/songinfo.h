#ifndef SONGINFO_H__
#define SONGINFO_H__

#include <nezplug/nezplug.h>

#ifdef __cplusplus
extern "C" {
#endif

SONG_INFO* SONGINFO_New();
void SONGINFO_Delete(SONG_INFO *info);
uint32_t SONGINFO_GetSongNo(SONG_INFO*);
void SONGINFO_SetSongNo(SONG_INFO*, uint32_t v);
uint32_t SONGINFO_GetStartSongNo(SONG_INFO*);
void SONGINFO_SetStartSongNo(SONG_INFO*, uint32_t v);
uint32_t SONGINFO_GetMaxSongNo(SONG_INFO*);
void SONGINFO_SetMaxSongNo(SONG_INFO*, uint32_t v);
uint32_t SONGINFO_GetExtendDevice(SONG_INFO*);
void SONGINFO_SetExtendDevice(SONG_INFO*, uint32_t v);
uint32_t SONGINFO_GetInitAddress(SONG_INFO*);
void SONGINFO_SetInitAddress(SONG_INFO*, uint32_t v);
uint32_t SONGINFO_GetPlayAddress(SONG_INFO*);
void SONGINFO_SetPlayAddress(SONG_INFO*, uint32_t v);
uint32_t SONGINFO_GetChannel(SONG_INFO*);
void SONGINFO_SetChannel(SONG_INFO*, uint32_t v);

#ifdef __cplusplus
}
#endif

#endif /* SONGINFO_H__ */
