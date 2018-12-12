#include "../../normalize.h"
#include "../kmsnddev.h"
#include "../../format/audiosys.h"
#include "../../format/handler.h"
#include "../../format/nsf6502.h"
#include "logtable.h"
#include "../../format/m_nsf.h"
#include "s_fme7.h"
#include "../s_psg.h"

#define BASECYCLES_ZX  (3579545)/*(1773400)*/
#define BASECYCLES_AMSTRAD  (2000000)
#define BASECYCLES_MSX (3579545)
#define BASECYCLES_NES (21477270)
#define FME7_VOL 4/5

typedef struct {
	KMIF_SOUND_DEVICE *psgp;
	uint8_t adr;
} PSGSOUND;

static int32_t PSGSoundRender(NEZ_PLAY *pNezPlay)
{
	PSGSOUND *psgs = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->psgs;
	int32_t b[2] = {0, 0};
	psgs->psgp->synth(psgs->psgp->ctx, b);
	return b[0]*FME7_VOL;
}

const static NEZ_NES_AUDIO_HANDLER s_psg_audio_handler[] = {
	{ 1, PSGSoundRender, NULL, NULL }, 
	{ 0, 0, 0, NULL }, 
};

static void PSGSoundVolume(NEZ_PLAY *pNezPlay, uint32_t volume)
{
	PSGSOUND *psgs = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->psgs;
	psgs->psgp->volume(psgs->psgp->ctx, volume);
}

const static NEZ_NES_VOLUME_HANDLER s_psg_volume_handler[] = {
	{ PSGSoundVolume, NULL }, 
	{ 0, NULL }, 
};

static void PSGSoundWrireAddr(NEZ_PLAY *pNezPlay, uint32_t address, uint32_t value)
{
    (void)address;
	PSGSOUND *psgs = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->psgs;
	psgs->adr = (uint8_t)value;
	psgs->psgp->write(psgs->psgp->ctx, 0, value);
}
static void PSGSoundWrireData(NEZ_PLAY *pNezPlay, uint32_t address, uint32_t value)
{
    (void)address;
	PSGSOUND *psgs = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->psgs;
	psgs->psgp->write(psgs->psgp->ctx, 1, value);
}

static NES_WRITE_HANDLER s_fme7_write_handler[] =
{
	{ 0xC000, 0xC000, PSGSoundWrireAddr, NULL },
	{ 0xE000, 0xE000, PSGSoundWrireData, NULL },
	{ 0,      0,      0, NULL },
};

static void FME7SoundReset(NEZ_PLAY *pNezPlay)
{
	PSGSOUND *psgs = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->psgs;
	psgs->psgp->reset(psgs->psgp->ctx, BASECYCLES_NES / 12, NESAudioFrequencyGet(pNezPlay));
}

const static NEZ_NES_RESET_HANDLER s_fme7_reset_handler[] = {
	{ NES_RESET_SYS_NOMAL, FME7SoundReset, NULL }, 
	{ 0,                   0, NULL }, 
};

static void PSGSoundTerm(NEZ_PLAY *pNezPlay)
{
	PSGSOUND *psgs = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->psgs;
	if (psgs->psgp)
	{
		psgs->psgp->release(psgs->psgp->ctx);
		psgs->psgp = 0;
	}
	XFREE(psgs);
}

const static NEZ_NES_TERMINATE_HANDLER s_psg_terminate_handler[] = {
	{ PSGSoundTerm, NULL }, 
	{ 0, NULL }, 
};

void FME7SoundInstall(NEZ_PLAY* pNezPlay)
{
	PSGSOUND *psgs;
	psgs = XMALLOC(sizeof(PSGSOUND));
	if (!psgs) return;
	XMEMSET(psgs, 0, sizeof(PSGSOUND));
	((NSFNSF*)pNezPlay->nsf)->psgs = psgs;

	psgs->psgp = PSGSoundAlloc(pNezPlay, PSG_TYPE_YM2149); //エンベロープ31段階あったんでYM2149系でしょう。
	if (!psgs->psgp) return;

	LogTableInitialize();
	NESTerminateHandlerInstall(&pNezPlay->nth, s_psg_terminate_handler);
	NESVolumeHandlerInstall(pNezPlay, s_psg_volume_handler);

	NESAudioHandlerInstall(pNezPlay, s_psg_audio_handler);
	NESWriteHandlerInstall(pNezPlay, s_fme7_write_handler);
	NESResetHandlerInstall(pNezPlay->nrh, s_fme7_reset_handler);
}
