#ifndef NEZPLUG_H__
#define NEZPLUG_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	NEZ_NESERR_NOERROR,
	NEZ_NESERR_SHORTOFMEMORY,
	NEZ_NESERR_FORMAT,
	NEZ_NESERR_PARAMETER
} NEZ_NESERR_CODE;

//チャンネルマスク用
typedef enum{//順番を変えたら恐ろしいことになる
	NEZ_DEV_2A03_SQ1,
	NEZ_DEV_2A03_SQ2,
	NEZ_DEV_2A03_TR,
	NEZ_DEV_2A03_NOISE,
	NEZ_DEV_2A03_DPCM,

	NEZ_DEV_FDS_CH1,

	NEZ_DEV_MMC5_SQ1,
	NEZ_DEV_MMC5_SQ2,
	NEZ_DEV_MMC5_DA,

	NEZ_DEV_VRC6_SQ1,
	NEZ_DEV_VRC6_SQ2,
	NEZ_DEV_VRC6_SAW,

	NEZ_DEV_N106_CH1,
	NEZ_DEV_N106_CH2,
	NEZ_DEV_N106_CH3,
	NEZ_DEV_N106_CH4,
	NEZ_DEV_N106_CH5,
	NEZ_DEV_N106_CH6,
	NEZ_DEV_N106_CH7,
	NEZ_DEV_N106_CH8,

	NEZ_DEV_DMG_SQ1,
	NEZ_DEV_DMG_SQ2,
	NEZ_DEV_DMG_WM,
	NEZ_DEV_DMG_NOISE,

	NEZ_DEV_HUC6230_CH1,
	NEZ_DEV_HUC6230_CH2,
	NEZ_DEV_HUC6230_CH3,
	NEZ_DEV_HUC6230_CH4,
	NEZ_DEV_HUC6230_CH5,
	NEZ_DEV_HUC6230_CH6,

	NEZ_DEV_AY8910_CH1,
	NEZ_DEV_AY8910_CH2,
	NEZ_DEV_AY8910_CH3,

	NEZ_DEV_SN76489_SQ1,
	NEZ_DEV_SN76489_SQ2,
	NEZ_DEV_SN76489_SQ3,
	NEZ_DEV_SN76489_NOISE,

	NEZ_DEV_SCC_CH1,
	NEZ_DEV_SCC_CH2,
	NEZ_DEV_SCC_CH3,
	NEZ_DEV_SCC_CH4,
	NEZ_DEV_SCC_CH5,

	NEZ_DEV_YM2413_CH1,
	NEZ_DEV_YM2413_CH2,
	NEZ_DEV_YM2413_CH3,
	NEZ_DEV_YM2413_CH4,
	NEZ_DEV_YM2413_CH5,
	NEZ_DEV_YM2413_CH6,
	NEZ_DEV_YM2413_CH7,
	NEZ_DEV_YM2413_CH8,
	NEZ_DEV_YM2413_CH9,
	NEZ_DEV_YM2413_BD,
	NEZ_DEV_YM2413_HH,
	NEZ_DEV_YM2413_SD,
	NEZ_DEV_YM2413_TOM,
	NEZ_DEV_YM2413_TCY,

	NEZ_DEV_ADPCM_CH1,

	NEZ_DEV_MSX_DA,

	NEZ_DEV_MAX,
} NEZ_CHANNEL_ID;

/* ensure int32_t etc exist */
#include "pstdint.h"

typedef struct NEZ_SONG_INFO_ NEZ_SONG_INFO;
typedef struct NEZ_NES_AUDIO_HANDLER_ NEZ_NES_AUDIO_HANDLER;
typedef struct NEZ_NES_VOLUME_HANDLER_  NEZ_NES_VOLUME_HANDLER;
typedef struct NEZ_NES_RESET_HANDLER_ NEZ_NES_RESET_HANDLER;
typedef struct NEZ_NES_TERMINATE_HANDLER_ NEZ_NES_TERMINATE_HANDLER;
typedef struct NEZ_PLAY_ NEZ_PLAY;

/* audiohandler function pointers */
typedef int32_t (*NEZ_AUDIOHANDLER)(NEZ_PLAY *);
typedef void (*NEZ_AUDIOHANDLER2)(NEZ_PLAY *, int32_t *p);
typedef void (*NEZ_VOLUMEHANDLER)(NEZ_PLAY *, uint32_t volume);
typedef void (*NEZ_RESETHANDLER)(NEZ_PLAY *);
typedef void (*NEZ_TERMINATEHANDLER)(NEZ_PLAY *);

struct NEZ_SONG_INFO_
{
	uint32_t songno;
	uint32_t maxsongno;
	uint32_t startsongno;
	uint32_t extdevice;
	uint32_t initaddress;
	uint32_t playaddress;
	uint32_t channel;
	uint32_t initlimit;
};

struct NEZ_NES_AUDIO_HANDLER_ {
	uint32_t fMode;
	NEZ_AUDIOHANDLER Proc;
	NEZ_AUDIOHANDLER2 Proc2;
	struct NEZ_NES_AUDIO_HANDLER_ *next;
};

struct NEZ_NES_VOLUME_HANDLER_ {
	NEZ_VOLUMEHANDLER Proc;
	struct NEZ_NES_VOLUME_HANDLER_ *next;
};

struct NEZ_NES_RESET_HANDLER_ {
	uint32_t priority;	/* 0(first) - 15(last) */
	NEZ_RESETHANDLER Proc;
	struct NEZ_NES_RESET_HANDLER_ *next;
};

struct NEZ_NES_TERMINATE_HANDLER_ {
	NEZ_TERMINATEHANDLER Proc;
	struct NEZ_NES_TERMINATE_HANDLER_ *next;
};

struct NEZ_PLAY_ {
	NEZ_SONG_INFO *song;
	uint32_t volume;
	uint32_t frequency;
	uint32_t channel;
	NEZ_NES_AUDIO_HANDLER *nah;
	NEZ_NES_VOLUME_HANDLER *nvh;
	NEZ_NES_RESET_HANDLER *(nrh[0x10]);
	NEZ_NES_TERMINATE_HANDLER *nth;
	uint32_t naf_type;
	uint32_t naf_prev[2];
    uint8_t chmask[0x80];
    struct {
        uint8_t gbamode;
    } gb_config;
    struct {
        int32_t apu_volume;
        int32_t realdac;
        int32_t noise_random_reset;
        int32_t nes2A03type;
    } nes_config;
	void *nsf;
	void *gbrdmg;
	void *heshes;
	void *zxay;
	void *kssseq;
	void *sgcseq;
	void *nsdp;
};

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

void NEZMuteChannel(NEZ_PLAY *, int32_t chan);
void NEZUnmuteChannel(NEZ_PLAY *, int32_t chan);

void NEZGBAMode(NEZ_PLAY *, uint8_t m);

void NEZGetFileInfo(char **p1, char **p2, char **p3, char **p4);

#ifdef __cplusplus
}
#endif

#endif /* NEZPLUG_H__ */
