#ifndef SONGINFO_H__
#define SONGINFO_H__

#include "../nestypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SONG_INFO_TAG
{
	Uint songno;
	Uint maxsongno;
	Uint startsongno;
	Uint extdevice;
	Uint initaddress;
	Uint playaddress;
	Uint channel;
	Uint initlimit;
} SONG_INFO;

SONG_INFO* SONGINFO_New();
void SONGINFO_Delete(SONG_INFO *info);
Uint SONGINFO_GetSongNo(SONG_INFO*);
void SONGINFO_SetSongNo(SONG_INFO*, Uint v);
Uint SONGINFO_GetStartSongNo(SONG_INFO*);
void SONGINFO_SetStartSongNo(SONG_INFO*, Uint v);
Uint SONGINFO_GetMaxSongNo(SONG_INFO*);
void SONGINFO_SetMaxSongNo(SONG_INFO*, Uint v);
Uint SONGINFO_GetExtendDevice(SONG_INFO*);
void SONGINFO_SetExtendDevice(SONG_INFO*, Uint v);
Uint SONGINFO_GetInitAddress(SONG_INFO*);
void SONGINFO_SetInitAddress(SONG_INFO*, Uint v);
Uint SONGINFO_GetPlayAddress(SONG_INFO*);
void SONGINFO_SetPlayAddress(SONG_INFO*, Uint v);
Uint SONGINFO_GetChannel(SONG_INFO*);
void SONGINFO_SetChannel(SONG_INFO*, Uint v);

#ifdef __cplusplus
}
#endif

#endif /* SONGINFO_H__ */
