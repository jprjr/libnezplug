#include "../../normalize.h"
#include "../kmsnddev.h"
#include "../../format/audiosys.h"
#include "../../format/handler.h"
#include "../../format/nsf6502.h"
#include "logtable.h"
#include "../../format/m_nsf.h"
#include "s_vrc7.h"

#define MASTER_CLOCK        (3579545)
#define VRC7_VOL 4/3

#include "../opl/s_opl.h"

typedef struct {
	uint8_t usertone_enable[1];
	uint8_t usertone[1][16 * 19];
	KMIF_SOUND_DEVICE *kmif;
} OPLLSOUND_INTF;

static uint32_t GetDwordLE(uint8_t *p)
{
	return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}
#define GetDwordLEM(p) (uint32_t)((((uint8_t *)p)[0] | (((uint8_t *)p)[1] << 8) | (((uint8_t *)p)[2] << 16) | (((uint8_t *)p)[3] << 24)))

void OPLLSetTone(NEZ_PLAY *pNezPlay, uint8_t *p, uint32_t type)
{
	OPLLSOUND_INTF *sndp = ((NSFNSF*)(pNezPlay)->nsf)->sndp;
	if ((GetDwordLE(p) & 0xf0ffffff) == GetDwordLEM("ILL0"))
		XMEMCPY(sndp->usertone[type], p, 16 * 19);
	else
		XMEMCPY(sndp->usertone[type], p, 8 * 15);
	sndp->usertone_enable[type] = 1;
}

void VRC7SetTone(NEZ_PLAY *pNezPlay, uint8_t *p, uint32_t type)
{
	OPLLSOUND_INTF *sndp = ((NSFNSF*)(pNezPlay)->nsf)->sndp;
	switch (type)
	{
		case 1:
			if ((GetDwordLE(p) & 0xf0ffffff) == GetDwordLEM("ILL0"))
				XMEMCPY(sndp->usertone[0], p, 16 * 19);
			else
				XMEMCPY(sndp->usertone[0], p, 8 * 15);
			sndp->usertone_enable[0] = 1;
			break;
		case 2:
			OPLLSetTone(pNezPlay, p, 0);
			break;
		case 3:
			OPLLSetTone(pNezPlay, p, 1);
			break;
	}
}

static int32_t OPLLSoundRender(NEZ_PLAY *pNezPlay)
{
	OPLLSOUND_INTF *sndp = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->sndp;
	int32_t b[2] = {0, 0};
	sndp->kmif->synth(sndp->kmif->ctx, b);
	return b[0]*VRC7_VOL;
}

const static NEZ_NES_AUDIO_HANDLER s_opll_audio_handler[] = {
	{ 1, OPLLSoundRender, NULL, NULL }, 
	{ 0, 0, 0, NULL }, 
};

static void OPLLSoundVolume(NEZ_PLAY *pNezPlay, uint32_t volume)
{
	OPLLSOUND_INTF *sndp = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->sndp;
	sndp->kmif->volume(sndp->kmif->ctx, volume);
}

const static NEZ_NES_VOLUME_HANDLER s_opll_volume_handler[] = {
	{ OPLLSoundVolume, NULL },
	{ 0, NULL }, 
};

static void VRC7SoundReset(NEZ_PLAY *pNezPlay)
{
	OPLLSOUND_INTF *sndp = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->sndp;
	if (sndp->usertone_enable[0]) sndp->kmif->setinst(sndp->kmif->ctx, 0, sndp->usertone[0], 16 * 19);
	sndp->kmif->reset(sndp->kmif->ctx, MASTER_CLOCK, NESAudioFrequencyGet(pNezPlay));
}

const static NEZ_NES_RESET_HANDLER s_vrc7_reset_handler[] = {
	{ NES_RESET_SYS_NOMAL, VRC7SoundReset, NULL }, 
	{ 0,                   0, NULL }, 
};

static void OPLLSoundTerm(NEZ_PLAY *pNezPlay)
{
	OPLLSOUND_INTF *sndp = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->sndp;
	if (sndp) {
		if (sndp->kmif) {
			sndp->kmif->release(sndp->kmif->ctx);
			sndp->kmif = 0;
		}
		XFREE(sndp);
	}
}

const static NEZ_NES_TERMINATE_HANDLER s_opll_terminate_handler[] = {
	{ OPLLSoundTerm, NULL }, 
	{ 0, NULL }, 
};

static void OPLLSoundWriteAddr(NEZ_PLAY *pNezPlay, uint32_t address, uint32_t value)
{
    (void)address;
	OPLLSOUND_INTF *sndp = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->sndp;
	sndp->kmif->write(sndp->kmif->ctx, 0, value);
}

static void OPLLSoundWriteData(NEZ_PLAY *pNezPlay, uint32_t address, uint32_t value)
{
    (void)address;
	OPLLSOUND_INTF *sndp = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->sndp;
	sndp->kmif->write(sndp->kmif->ctx, 1, value);
}

static NES_WRITE_HANDLER s_vrc7_write_handler[] =
{
	{ 0x9010, 0x9010, OPLLSoundWriteAddr, NULL },
	{ 0x9030, 0x9030, OPLLSoundWriteData, NULL },
	{ 0,      0,      0, NULL },
};


void VRC7SoundInstall(NEZ_PLAY *pNezPlay)
{
	OPLLSOUND_INTF *sndp;
	sndp = XMALLOC(sizeof(OPLLSOUND_INTF));
	if (!sndp) return;
	XMEMSET(sndp, 0, sizeof(OPLLSOUND_INTF));
	((NSFNSF*)pNezPlay->nsf)->sndp = sndp;

	sndp->usertone_enable[0] = 0;
	LogTableInitialize();
	sndp->kmif = OPLSoundAlloc(pNezPlay, OPL_TYPE_VRC7);
	if (sndp->kmif)
	{
		NESAudioHandlerInstall(pNezPlay, s_opll_audio_handler);
		NESVolumeHandlerInstall(pNezPlay, s_opll_volume_handler);
		NESTerminateHandlerInstall(&pNezPlay->nth, s_opll_terminate_handler);

		NESResetHandlerInstall(pNezPlay->nrh, s_vrc7_reset_handler);
		NESWriteHandlerInstall(pNezPlay, s_vrc7_write_handler);
	}
}

