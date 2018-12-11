#include "songinfo.h"
#include "../normalize.h"

int32_t Always_stereo = 0;

SONG_INFO* SONGINFO_New()
{
	SONG_INFO *info = (SONG_INFO*)XMALLOC(sizeof(SONG_INFO));

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

uint32_t SONGINFO_GetSongNo(SONG_INFO *info)
{
	return info->songno;
}
void SONGINFO_SetSongNo(SONG_INFO *info, uint32_t v)
{
	if (info->maxsongno && v > info->maxsongno) v = info->maxsongno;
	if (v == 0) v++;
	info->songno = v;
}
uint32_t SONGINFO_GetStartSongNo(SONG_INFO *info)
{
	return info->startsongno;
}
void SONGINFO_SetStartSongNo(SONG_INFO *info, uint32_t v)
{
	info->startsongno = v;
}
uint32_t SONGINFO_GetMaxSongNo(SONG_INFO *info)
{
	return info->maxsongno;
}
void SONGINFO_SetMaxSongNo(SONG_INFO* info, uint32_t v)
{
	info->maxsongno = v;
}
uint32_t SONGINFO_GetExtendDevice(SONG_INFO *info)
{
	return info->extdevice;
}
void SONGINFO_SetExtendDevice(SONG_INFO *info, uint32_t v)
{
	info->extdevice = v;
}
uint32_t SONGINFO_GetInitAddress(SONG_INFO *info)
{
	return info->initaddress;
}
void SONGINFO_SetInitAddress(SONG_INFO *info, uint32_t v)
{
	info->initaddress = v;
}
uint32_t SONGINFO_GetPlayAddress(SONG_INFO *info)
{
	return info->playaddress;
}
void SONGINFO_SetPlayAddress(SONG_INFO *info, uint32_t v)
{
	info->playaddress = v;
}
uint32_t SONGINFO_GetChannel(SONG_INFO *info)
{
	return info->channel;
}
void SONGINFO_SetChannel(SONG_INFO *info, uint32_t v)
{

	if(Always_stereo)
		info->channel = 2;
	else
		info->channel = v;

}
