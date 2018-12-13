#include <nezplug/nezplug.h>

#include "handler.h"
#include "audiosys.h"
#include "songinfo.h"

#include "../device/kmsnddev.h"
#include "m_hes.h"
#include "m_gbr.h"
#include "m_zxay.h"
#include "m_nsf.h"
#include "m_kss.h"
#include "m_nsd.h"
#include "m_sgc.h"

struct {
	char* title;
	char* artist;
	char* copyright;
	char detail[1024];
}songinfodata;

static uint32_t GetWordLE(uint8_t *p)
{
	return p[0] | (p[1] << 8);
}

static uint32_t GetDwordLE(uint8_t *p)
{
	return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}
#define GetDwordLEM(p) (uint32_t)((((uint8_t *)p)[0] | (((uint8_t *)p)[1] << 8) | (((uint8_t *)p)[2] << 16) | (((uint8_t *)p)[3] << 24)))


NEZ_PLAY* NEZNew()
{
	NEZ_PLAY *pNezPlay = (NEZ_PLAY*)XMALLOC(sizeof(NEZ_PLAY));

	if (pNezPlay != NULL) {
		XMEMSET(pNezPlay, 0, sizeof(NEZ_PLAY));
        XMEMSET(pNezPlay->chmask,1,0x80);
		pNezPlay->song = SONGINFO_New();
		if (!pNezPlay->song) {
			XFREE(pNezPlay);
			return 0;
		}

		pNezPlay->frequency = 48000;
		pNezPlay->channel = 1;

		pNezPlay->naf_type = NES_AUDIO_FILTER_NONE;

        pNezPlay->nes_config.apu_volume = 64;
        pNezPlay->nes_config.n106_volume = 16;
        pNezPlay->nes_config.n106_realmode = 0;
        pNezPlay->nes_config.fds_realmode = 3;
        pNezPlay->nes_config.realdac = 1;
        pNezPlay->nes_config.noise_random_reset = 0;
        pNezPlay->nes_config.nes2A03type = 1;
        pNezPlay->nes_config.fds_debug_option1 = 1;
        pNezPlay->nes_config.fds_debug_option2 = 0;
#if HES_TONE_DEBUG_OPTION_ENABLE
        pNezPlay->hes_config.tone_debug_option = 0;
#endif
        pNezPlay->hes_config.noise_debug_option1 = 9;
        pNezPlay->hes_config.noise_debug_option2 = 10;
        pNezPlay->hes_config.noise_debug_option3 = 3;
        pNezPlay->hes_config.noise_debug_option4 = 508;
		pNezPlay->naf_prev[0] = pNezPlay->naf_prev[1] = 0x8000;
	}


	return pNezPlay;
}

void NEZDelete(NEZ_PLAY *pNezPlay)
{
	if (pNezPlay != NULL) {
		NESTerminate(pNezPlay);
		NESAudioHandlerTerminate(pNezPlay);
		NESVolumeHandlerTerminate(pNezPlay);
		SONGINFO_Delete(pNezPlay->song);
		XFREE(pNezPlay);
	}
}


void NEZSetSongNo(NEZ_PLAY *pNezPlay, uint32_t uSongNo)
{
	if (pNezPlay == 0) return;
	SONGINFO_SetSongNo(pNezPlay->song, uSongNo);
}


void NEZSetFrequency(NEZ_PLAY *pNezPlay, uint32_t freq)
{
	if (pNezPlay == 0) return;
	NESAudioFrequencySet(pNezPlay, freq);
}

void NEZSetChannel(NEZ_PLAY *pNezPlay, uint32_t ch)
{
	if (pNezPlay == 0) return;
	NESAudioChannelSet(pNezPlay, ch);
}

void NEZReset(NEZ_PLAY *pNezPlay)
{
	if (pNezPlay == 0) return;
	NESReset(pNezPlay);
	NESVolume(pNezPlay, pNezPlay->volume);
}

void NEZSetFilter(NEZ_PLAY *pNezPlay, uint32_t filter)
{
	if (pNezPlay == 0) return;
	NESAudioFilterSet(pNezPlay, filter);
}

void NEZVolume(NEZ_PLAY *pNezPlay, uint32_t uVolume)
{
	if (pNezPlay == 0) return;
	pNezPlay->volume = uVolume;
	NESVolume(pNezPlay, pNezPlay->volume);
}

void NEZAPUVolume(NEZ_PLAY *pNezPlay, int32_t uVolume)
{
	if (pNezPlay == 0) return;
	if (pNezPlay->nsf == 0) return;
	((NSFNSF*)pNezPlay->nsf)->apu_volume = uVolume;
}

void NEZDPCMVolume(NEZ_PLAY *pNezPlay, int32_t uVolume)
{
	if (pNezPlay == 0) return;
	if (pNezPlay->nsf == 0) return;
	((NSFNSF*)pNezPlay->nsf)->dpcm_volume = uVolume;
}

void NEZRender(NEZ_PLAY *pNezPlay, void *bufp, uint32_t buflen)
{
	if (pNezPlay == 0) return;
	NESAudioRender(pNezPlay, (int16_t*)bufp, buflen);
}

uint32_t NEZGetSongNo(NEZ_PLAY *pNezPlay)
{
	if (pNezPlay == 0) return 0;
	return SONGINFO_GetSongNo(pNezPlay->song);
}

uint32_t NEZGetSongStart(NEZ_PLAY *pNezPlay)
{
	if (pNezPlay == 0) return 0;
	return SONGINFO_GetStartSongNo(pNezPlay->song);
}

uint32_t NEZGetSongMax(NEZ_PLAY *pNezPlay)
{
	if (pNezPlay == 0) return 0;
	return SONGINFO_GetMaxSongNo(pNezPlay->song);
}

uint32_t NEZGetChannel(NEZ_PLAY *pNezPlay)
{
	if (pNezPlay == 0) return 1;
	return SONGINFO_GetChannel(pNezPlay->song);
}

uint32_t NEZGetFrequency(NEZ_PLAY *pNezPlay)
{
	if (pNezPlay == 0) return 1;
	return NESAudioFrequencyGet(pNezPlay);
}

uint32_t NEZLoad(NEZ_PLAY *pNezPlay, uint8_t *pData, uint32_t uSize)
{
	uint32_t ret = NEZ_NESERR_NOERROR;

	songinfodata.title=NULL;
	songinfodata.artist=NULL;
	songinfodata.copyright=NULL;
	songinfodata.detail[0]=0;

	while (1)
	{
		if (!pNezPlay || !pData) {
			ret = NEZ_NESERR_PARAMETER;
			break;
		}
		NESTerminate(pNezPlay);
		NESHandlerInitialize(pNezPlay->nrh, pNezPlay->nth);
		NESAudioHandlerInitialize(pNezPlay);
		if (uSize < 8)
		{
			ret = NEZ_NESERR_FORMAT;
			break;
		}
		else if (GetDwordLE(pData + 0) == GetDwordLEM("KSCC"))
		{
			/* KSS */
			ret =  KSSLoad(pNezPlay, pData, uSize);
			if (ret) break;
		}
		else if (GetDwordLE(pData + 0) == GetDwordLEM("KSSX"))
		{
			/* KSS */
			ret =  KSSLoad(pNezPlay, pData, uSize);
			if (ret) break;
		}
		else if (GetDwordLE(pData + 0) == GetDwordLEM("HESM"))
		{
			/* HES */
			ret =  HESLoad(pNezPlay, pData, uSize);
			if (ret) break;
		}
		else if (uSize > 0x220 && GetDwordLE(pData + 0x200) == GetDwordLEM("HESM"))
		{
			/* HES(+512byte header) */
			ret =  HESLoad(pNezPlay, pData + 0x200, uSize - 0x200);
			if (ret) break;
		}
		else if (GetDwordLE(pData + 0) == GetDwordLEM("NESM") && pData[4] == 0x1A)
		{
			/* NSF */
			ret = NSFLoad(pNezPlay, pData, uSize);
			if (ret) break;
		}
		else if (GetDwordLE(pData + 0) == GetDwordLEM("ZXAY") && GetDwordLE(pData + 4) == GetDwordLEM("EMUL"))
		{
			/* ZXAY */
			ret =  ZXAYLoad(pNezPlay, pData, uSize);
			if (ret) break;
		}
		else if (GetDwordLE(pData + 0) == GetDwordLEM("GBRF"))
		{
			/* GBR */
			ret =  GBRLoad(pNezPlay, pData, uSize);
			if (ret) break;
		}
		else if ((GetDwordLE(pData + 0) & 0x00ffffff) == GetDwordLEM("GBS\x0"))
		{
			/* GBS */
			ret =  GBRLoad(pNezPlay, pData, uSize);
			if (ret) break;
		}
		else if (pData[0] == 0xc3 && uSize > GetWordLE(pData + 1) + 4 && GetWordLE(pData + 1) > 0x70 && (GetDwordLE(pData + GetWordLE(pData + 1) - 0x70) & 0x00ffffff) == GetDwordLEM("GBS\x0"))
		{
			/* GB(GBS player) */
			ret =  GBRLoad(pNezPlay, pData + GetWordLE(pData + 1) - 0x70, uSize - (GetWordLE(pData + 1) - 0x70));
			if (ret) break;
		}
		else if (GetDwordLE(pData + 0) == GetDwordLEM("NESL") && pData[4] == 0x1A)
		{
			/* NSD */
			ret = NSDLoad(pNezPlay, pData, uSize);
			if (ret) break;
		}
		else if ((GetDwordLE(pData + 0) & 0x00ffffff) == GetDwordLEM("SGC"))
		{
			/* SGC */
			ret = SGCLoad(pNezPlay, pData, uSize);
			if (ret) break;
		}
		else
		{
			ret = NEZ_NESERR_FORMAT;
			break;
		}
		return NEZ_NESERR_NOERROR;
	}
	NESTerminate(pNezPlay);
	return ret;
}


void NEZGetFileInfo(char **p1, char **p2, char **p3, char **p4)
{
	*p1 = songinfodata.title;
	*p2 = songinfodata.artist;
	*p3 = songinfodata.copyright;
	*p4 = songinfodata.detail;
}

void NEZMuteChannel(NEZ_PLAY *pNezPlay, int32_t chan)
{
    if(chan < 0) {
        XMEMSET(pNezPlay->chmask,0,sizeof(pNezPlay->chmask));
    } else if ((size_t)chan < sizeof(pNezPlay->chmask)) {
        pNezPlay->chmask[chan] = 0;
    }
}

void NEZUnmuteChannel(NEZ_PLAY *pNezPlay, int32_t chan)
{
    if(chan < 0) {
        XMEMSET(pNezPlay->chmask,1,sizeof(pNezPlay->chmask));
    } else if ((size_t)chan < sizeof(pNezPlay->chmask)) {
        pNezPlay->chmask[chan] = 1;
    }
}

void NEZGBAMode(NEZ_PLAY *pNezPlay, uint8_t m) {
    pNezPlay->gb_config.gbamode = m;
}

