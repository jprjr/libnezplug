#ifndef NEZPLUG_H__
#define NEZPLUG_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	NEZPLUG_NESERR_NOERROR,
	NEZPLUG_NESERR_SHORTOFMEMORY,
	NEZPLUG_NESERR_FORMAT,
	NEZPLUG_NESERR_PARAMETER
} NEZPLUG_NESERR_CODE;

/* ensure int32_t etc exist */
#include "pstdint.h"

/* audiohandler function pointers */
typedef int32_t (*AUDIOHANDLER)(void *);
typedef void (*AUDIOHANDLER2)(void *, int32_t *p);
typedef void (*VOLUMEHANDLER)(void*, uint32_t volume);
typedef void (*RESETHANDLER)(void*);
typedef void (*TERMINATEHANDLER)(void*);

typedef struct SONG_INFO_TAG
{
	uint32_t songno;
	uint32_t maxsongno;
	uint32_t startsongno;
	uint32_t extdevice;
	uint32_t initaddress;
	uint32_t playaddress;
	uint32_t channel;
	uint32_t initlimit;
} SONG_INFO;

typedef struct NES_AUDIO_HANDLER_TAG {
	uint32_t fMode;
	AUDIOHANDLER Proc;
	AUDIOHANDLER2 Proc2;
	struct NES_AUDIO_HANDLER_TAG *next;
} NES_AUDIO_HANDLER;

typedef struct NES_VOLUME_HANDLER_TAG {
	VOLUMEHANDLER Proc;
	struct NES_VOLUME_HANDLER_TAG *next;
} NES_VOLUME_HANDLER;

typedef struct NES_RESET_HANDLER_TAG {
	uint32_t priority;	/* 0(first) - 15(last) */
	RESETHANDLER Proc;
	struct NES_RESET_HANDLER_TAG *next;
} NES_RESET_HANDLER;

typedef struct NES_TERMINATE_HANDLER_TAG {
	TERMINATEHANDLER Proc;
	struct NES_TERMINATE_HANDLER_TAG *next;
} NES_TERMINATE_HANDLER;

typedef struct NEZPLAY_TAG {
	SONG_INFO *song;
	uint32_t volume;
	uint32_t frequency;
	uint32_t channel;
	NES_AUDIO_HANDLER *nah;
	NES_VOLUME_HANDLER *nvh;
	NES_RESET_HANDLER *(nrh[0x10]);
	NES_TERMINATE_HANDLER *nth;
	uint32_t naf_type;
	uint32_t naf_prev[2];
	void *nsf;
	void *gbrdmg;
	void *heshes;
	void *zxay;
	void *kssseq;
	void *sgcseq;
	void *nsdp;
} NEZ_PLAY;

/* NEZ player */
NEZ_PLAY* NEZNew();
void NEZDelete(NEZ_PLAY*);

uint32_t NEZLoad(NEZ_PLAY*, uint8_t*, uint32_t);
void NEZSetSongNo(NEZ_PLAY*, uint32_t uSongNo);
void NEZSetFrequency(NEZ_PLAY*, uint32_t freq);
void NEZSetChannel(NEZ_PLAY*, uint32_t ch);
void NEZReset(NEZ_PLAY*);
void NEZSetFilter(NEZ_PLAY *, uint32_t filter);
void NEZVolume(NEZ_PLAY*, uint32_t uVolume);
void NEZAPUVolume(NEZ_PLAY*, int32_t uVolume);
void NEZDPCMVolume(NEZ_PLAY*, int32_t uVolume);
void NEZRender(NEZ_PLAY*, void *bufp, uint32_t buflen);

uint32_t NEZGetSongNo(NEZ_PLAY*);
uint32_t NEZGetSongStart(NEZ_PLAY*);
uint32_t NEZGetSongMax(NEZ_PLAY*);
uint32_t NEZGetChannel(NEZ_PLAY*);
uint32_t NEZGetFrequency(NEZ_PLAY*);
void NEZGetFileInfo(char **p1, char **p2, char **p3, char **p4);

#ifdef __cplusplus
}
#endif

#endif /* NEZPLUG_H__ */
