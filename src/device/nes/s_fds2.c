#include "../../normalize.h"
#include "../kmsnddev.h"
#include "../../format/audiosys.h"
#include "../../format/nsf6502.h"
#include "logtable.h"
#include "../../format/m_nsf.h"
#include "s_fds.h"

#define NES_BASECYCLES (21477270)
#define CPS_BITS (16)
#define CPF_BITS (12)

typedef struct {
	uint32_t phase;
	uint32_t count;
	uint8_t spd;			/* fader rate */
	uint8_t disable;		/* fader disable */
	uint8_t direction;	/* fader direction */
	int8_t volume;		/* volume */
} FADER;

typedef struct {
	uint32_t cps;		/* cycles per sample */
	uint32_t cpf;		/* cycles per frame */
	uint32_t phase;	/* wave phase */
	uint32_t spd;		/* wave speed */
	uint32_t pt;		/* programmable timer */
	int32_t ofs1;
	int32_t ofs2;
	int32_t ofs3;
	int32_t input;
	FADER fd;
	uint8_t fp;		/* frame position; */
	uint8_t lvl;
	uint8_t disable;
	uint8_t disable2;
	int8_t wave[0x40];
} FDS_FMOP;

typedef struct FDSSOUND {
	uint32_t mastervolume;
	FDS_FMOP op[2];
	uint8_t mute;
	uint8_t reg[0x10];
	uint8_t waveaddr;
} FDSSOUND;

static int32_t FDSSoundOperatorRender(FDS_FMOP *op)
{
	int32_t spd;
	if (op->disable) return 0;

	if (!op->disable2 && !op->fd.disable)
	{
		uint32_t fdspd;
		op->fd.count += op->cpf;
		fdspd = ((uint32_t)op->fd.spd + 1) << (CPF_BITS + 11);
		if (op->fd.direction)
		{
			while (op->fd.count >= fdspd)
			{
				op->fd.count -= fdspd;
				op->fd.volume += (op->fd.volume < 0x3f);
			}
		}
		else
		{
			while (op->fd.count >= fdspd)
			{
				op->fd.count -= fdspd;
				op->fd.volume -= (op->fd.volume > 0);
			}
		}
	}

	op->pt += op->cps;
	if (op->spd <= 0x20) return 0;
	spd = op->spd + op->input + op->ofs1;
	/* if (spd > (1 << 24)) spd = (1 << 24); */
	op->phase += spd * (op->pt >> CPS_BITS);
	op->pt &= ((1 << CPS_BITS) - 1);

	return op->ofs3 + ((int32_t)op->wave[(op->phase >> 24) & 0x3f] + op->ofs2) * (int32_t)op->fd.volume;
}

static int32_t FDSSoundRender(NEZ_PLAY *pNezPlay)
{
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;
	fdssound->op[1].input = 0;
	fdssound->op[0].input = FDSSoundOperatorRender(&fdssound->op[1]) << (11 - fdssound->op[1].lvl);
	return (FDSSoundOperatorRender(&fdssound->op[0]) << (9 + 2 - fdssound->op[0].lvl));
}

const static NEZ_NES_AUDIO_HANDLER s_fds_audio_handler[] =
{
	{ 1, FDSSoundRender, NULL, NULL }, 
	{ 0, 0, NULL, NULL }, 
};

static void FDSSoundVolume(NEZ_PLAY *pNezPlay, uint32_t volume)
{
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;
	fdssound->mastervolume = (volume << (LOG_BITS - 8)) << 1;
}

const static NEZ_NES_VOLUME_HANDLER s_fds_volume_handler[] = {
	{ FDSSoundVolume, NULL }, 
	{ 0, NULL }, 
};

static void FDSSoundWrite(NEZ_PLAY *pNezPlay, uint32_t address, uint32_t value)
{
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;
	if (0x4040 <= address && address <= 0x407F)
	{
		fdssound->op[0].wave[address - 0x4040] = ((int32_t)(value & 0x3f)) - 0x20;
	}
	else if (0x4080 <= address && address <= 0x408F)
	{
		int ch = (address >= 0x4084);
		FDS_FMOP *pop = &fdssound->op[ch];
		fdssound->reg[address - 0x4080] = (uint8_t)value;
		switch (address & 15)
		{
			case 0:	case 4:
				pop->fd.disable = (uint8_t)(value & 0x80);
				if (pop->fd.disable)
				{
					pop->fd.volume = (value & 0x3f);
				}
				else
				{
					pop->fd.direction = (uint8_t)(value & 0x40);
					pop->fd.spd = (uint8_t)(value & 0x3f);
				}
				break;
			case 5:
				{
					int32_t dat;
					if ((value & 0x7f) < 0x60)
						dat = value & 0x7f;
					else
						dat = ((int32_t)(value & 0x7f)) - 0x80;
					switch (pNezPlay->nes_config.fds_debug_option1)
					{
						default:
						case 1: pop->ofs1 = dat << pNezPlay->nes_config.fds_debug_option2; break;
						case 2: pop->ofs2 = dat >> pNezPlay->nes_config.fds_debug_option2; break;
						case 3: pop->ofs3 = dat << pNezPlay->nes_config.fds_debug_option2; break;
					}
				}
				break;
			case 2:	case 6:
				pop->spd &= 0x000FF0000;
				pop->spd |= (value & 0xFF) << 8;
				break;
			case 3:
				pop->spd &= 0x0000FF00;
				pop->spd |= (value & 0xF) << 16;
				pop->disable = (uint8_t)(value & 0x80);
				break;
			case 7:
				pop->spd &= 0x0000FF00;
				pop->spd |= (value & 0x7F) << 16;
				pop->disable = (uint8_t)(value & 0x80);
				break;
			case 8:
				{
					static int8_t lfotbl[8] = { 0,2,4,6,-8,-6,-4,-2 };
					int8_t v = lfotbl[value & 7];
					fdssound->op[1].wave[fdssound->waveaddr++] = v;
					fdssound->op[1].wave[fdssound->waveaddr++] = v;
					if (fdssound->waveaddr == 0x40) fdssound->waveaddr = 0;
				}
				break;
			case 9:
				fdssound->op[0].lvl = (uint8_t)(value & 3);
				fdssound->op[0].disable2 = (uint8_t)(value & 0x80);
				break;
			case 10:
				fdssound->op[1].lvl = (uint8_t)(value & 3);
				fdssound->op[1].disable2 = (uint8_t)(value & 0x80);
				break;
		}
	}
}

static NES_WRITE_HANDLER s_fds_write_handler[] =
{
	{ 0x4040, 0x408F, FDSSoundWrite, NULL },
	{ 0,      0,      0, NULL },
};

static uint32_t FDSSoundRead(NEZ_PLAY *pNezPlay, uint32_t address)
{
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;
	if (0x4090 <= address && address <= 0x409F)
	{
		return fdssound->reg[address - 0x4090];
	}
	return 0;
}

static NES_READ_HANDLER s_fds_read_handler[] =
{
	{ 0x4090, 0x409F, FDSSoundRead, NULL },
	{ 0,      0,      0, NULL },
};

static uint32_t DivFix(uint32_t p1, uint32_t p2, uint32_t fix)
{
	uint32_t ret;
	ret = p1 / p2;
	p1  = p1 % p2;/* p1 = p1 - p2 * ret; */
	while (fix--)
	{
		p1 += p1;
		ret += ret;
		if (p1 >= p2)
		{
			p1 -= p2;
			ret++;
		}
	}
	return ret;
}

static void FDSSoundReset(NEZ_PLAY *pNezPlay)
{
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;
	uint32_t i, cps, cpf;
	XMEMSET(fdssound, 0, sizeof(FDSSOUND));
	cps = DivFix(NES_BASECYCLES, 12 * NESAudioFrequencyGet(pNezPlay), CPS_BITS);
	cpf = DivFix(NES_BASECYCLES, 12 * NESAudioFrequencyGet(pNezPlay), CPF_BITS);
	fdssound->op[0].cps = fdssound->op[1].cps = cps;
	fdssound->op[0].cpf = fdssound->op[1].cpf = cpf;
	fdssound->op[0].lvl = fdssound->op[1].lvl = 0;
#if 0
	fdssound->op[0].fd.disable = fdssound->op[1].fd.disable = 1;
	fdssound->op[0].disable = fdssound->op[1].disable = 1;
#endif

	for (i = 0; i < 0x40; i++)
	{
		fdssound->op[0].wave[i] = (i < 0x20) ? 0x1f : -0x20;
		fdssound->op[1].wave[i] = 0;
	}
}

const static NEZ_NES_RESET_HANDLER s_fds_reset_handler[] =
{
	{ NES_RESET_SYS_NOMAL, FDSSoundReset, NULL }, 
	{ 0,                   0, NULL }, 
};

static void FDSSoundTerm(NEZ_PLAY *pNezPlay)
{
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;
	if (fdssound)
		XFREE(fdssound);
}

const static NEZ_NES_TERMINATE_HANDLER s_fds_terminate_handler[] = {
	{ FDSSoundTerm, NULL }, 
	{ 0, NULL }, 
};

void FDSSoundInstall2(NEZ_PLAY *pNezPlay)
{
	FDSSOUND *fdssound;
	fdssound = XMALLOC(sizeof(FDSSOUND));
	if (!fdssound) return;
	XMEMSET(fdssound, 0, sizeof(FDSSOUND));
	((NSFNSF*)pNezPlay->nsf)->fdssound = fdssound;

	LogTableInitialize();
	NESAudioHandlerInstall(pNezPlay, s_fds_audio_handler);
	NESVolumeHandlerInstall(pNezPlay, s_fds_volume_handler);
	NESTerminateHandlerInstall(&pNezPlay->nth, s_fds_terminate_handler);
	NESReadHandlerInstall(pNezPlay, s_fds_read_handler);
	NESWriteHandlerInstall(pNezPlay, s_fds_write_handler);
	NESResetHandlerInstall(pNezPlay->nrh, s_fds_reset_handler);
}
