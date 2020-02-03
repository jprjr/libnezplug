#include "songinfo.h"

Int32 Always_stereo = 0;

SONG_INFO* SONGINFO_New()
{
	SONG_INFO *info = (SONG_INFO*)malloc(sizeof(SONG_INFO));

	if (info != NULL) {
		info->songno = 1;
		info->maxsongno = info->startsongno = 0;
		info->extdevice = 0;
		info->initaddress = info->playaddress = 0;
		info->channel = 1;
		info->initlimit = 0;
	}

	return info;
}

void SONGINFO_Delete(SONG_INFO *info)
{
	free (info);
}

Uint SONGINFO_GetSongNo(SONG_INFO *info)
{
	return info->songno;
}
void SONGINFO_SetSongNo(SONG_INFO *info, Uint v)
{
	if (info->maxsongno && v > info->maxsongno) v = info->maxsongno;
	if (v == 0) v++;
	info->songno = v;
}
Uint SONGINFO_GetStartSongNo(SONG_INFO *info)
{
	return info->startsongno;
}
void SONGINFO_SetStartSongNo(SONG_INFO *info, Uint v)
{
	info->startsongno = v;
}
Uint SONGINFO_GetMaxSongNo(SONG_INFO *info)
{
	return info->maxsongno;
}
void SONGINFO_SetMaxSongNo(SONG_INFO* info, Uint v)
{
	info->maxsongno = v;
}
Uint SONGINFO_GetExtendDevice(SONG_INFO *info)
{
	return info->extdevice;
}
void SONGINFO_SetExtendDevice(SONG_INFO *info, Uint v)
{
	info->extdevice = v;
}
Uint SONGINFO_GetInitAddress(SONG_INFO *info)
{
	return info->initaddress;
}
void SONGINFO_SetInitAddress(SONG_INFO *info, Uint v)
{
	info->initaddress = v;
}
Uint SONGINFO_GetPlayAddress(SONG_INFO *info)
{
	return info->playaddress;
}
void SONGINFO_SetPlayAddress(SONG_INFO *info, Uint v)
{
	info->playaddress = v;
}
Uint SONGINFO_GetChannel(SONG_INFO *info)
{
	return info->channel;
}
void SONGINFO_SetChannel(SONG_INFO *info, Uint v)
{

	if(Always_stereo)
		info->channel = 2;
	else
		info->channel = v;

}
