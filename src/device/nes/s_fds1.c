#include "../../normalize.h"
#include "../kmsnddev.h"
#include "../../format/audiosys.h"
#include "../../format/nsf6502.h"
#include "logtable.h"
#include "../../format/m_nsf.h"
#include "s_fds.h"

#define NES_BASECYCLES (21477270)
#define CPS_SHIFT (23)
#define PHASE_SHIFT (23)
#define FADEOUT_SHIFT 11/*(11)*/
#define XXX_SHIFT 1 /* 3 */

typedef struct {
	uint32_t wave[0x40];
	uint32_t envspd;
	int32_t envphase;
	uint32_t envout;
	uint32_t outlvl;

	uint32_t phase;
	uint32_t spd;
	uint32_t volume;
	int32_t sweep;

	uint8_t enable;
	uint8_t envmode;
	uint8_t xxxxx;
	uint8_t xxxxx2;
} FDS_FMOP;

typedef struct FDSSOUND {
	uint32_t cps;
	int32_t cycles;
	uint32_t mastervolume;
	int32_t output;

	FDS_FMOP op[2];

	uint32_t waveaddr;
	uint8_t mute;
	uint8_t key;
	uint8_t reg[0x10];
} FDSSOUND;

static int32_t FDSSoundRender(NEZ_PLAY *pNezPlay)
{
	FDS_FMOP *pop;
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;

	for (pop = &fdssound->op[0]; pop < &fdssound->op[2]; pop++)
	{
		uint32_t vol;
		if (pop->envmode)
		{
			pop->envphase -= fdssound->cps >> (FADEOUT_SHIFT - XXX_SHIFT);
			if (pop->envmode & 0x40)
				while (pop->envphase < 0)
				{
					pop->envphase += pop->envspd;
					pop->volume += (pop->volume < 0x1f);
				}
			else
				while (pop->envphase < 0)
				{
					pop->envphase += pop->envspd;
					pop->volume -= (pop->volume > 0x00);
				}
		}
		vol = pop->volume;
		if (vol)
		{
			vol += pop->sweep;
			if (vol < 0)
				vol = 0;
			else if (vol > 0x3f)
				vol = 0x3f;
		}
		pop->envout = LinearToLog(vol);
	}
	fdssound->op[1].envout += fdssound->mastervolume;

	fdssound->cycles -= fdssound->cps;
	while (fdssound->cycles < 0)
	{
		fdssound->cycles += 1 << CPS_SHIFT;
		fdssound->output = 0;
		for (pop = &fdssound->op[0]; pop < &fdssound->op[2]; pop++)
		{
			if (!pop->spd || !pop->enable)
			{
				fdssound->output = 0;
				continue;
			}
			pop->phase += pop->spd + fdssound->output;
			fdssound->output = LogToLinear(pop->envout + pop->wave[(pop->phase >> (PHASE_SHIFT - XXX_SHIFT)) & 0x3f], pop->outlvl);
		}
	}
	if (fdssound->mute) return 0;
	return fdssound->output;
}

const static NEZ_NES_AUDIO_HANDLER s_fds_audio_handler[] =
{
	{ 1, FDSSoundRender, NULL , NULL}, 
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
		fdssound->op[1].wave[address - 0x4040] = LinearToLog(((int32_t)value & 0x3f) - 0x20);
		/* fdssound->op[1].wave[address - 0x4040] = LinearToLog(((int32_t)value & 0x7f) - 0x40); */
	}
	else if (0x4080 <= address && address <= 0x408F)
	{
		int ch = (address < 0x4084);
		FDS_FMOP *pop = &fdssound->op[ch];
		fdssound->reg[address - 0x4080] = (uint8_t)value;
		switch (address & 15)
		{
			case 0:	case 4:
				if (value & 0x80)
				{
					pop->volume = (value & 0x3f);
					pop->envmode = 0;
				}
				else
				{
					pop->envspd = ((value & 0x3f) + 1) << CPS_SHIFT;
					pop->envmode = (uint8_t)(0x80 | value);
				}
				break;
			case 1:	case 5:
				if (!value) break;
				if ((value & 0x7f) < 0x60)
					pop->sweep = value & 0x7f;
				else
					pop->sweep = ((int32_t)value & 0x7f) - 0x80;
				break;
			case 2:	case 6:
				pop->spd &= 0x00000F00 << 7;
				pop->spd |= (value & 0xFF) << 7;
				break;
			case 3:	case 7:
				pop->spd &= 0x000000FF << 7;
				pop->spd |= (value & 0x0F) << (7 + 8);
				pop->enable = !(value & 0x80);
				break;
			case 8:
				{
					static int8_t lfotbl[8] = { 0,1,2,3,-4,-3,-2,-1 };
					uint32_t v = LinearToLog(lfotbl[value & 7]);
					fdssound->op[0].wave[fdssound->waveaddr++] = v;
					fdssound->op[0].wave[fdssound->waveaddr++] = v;
					if (fdssound->waveaddr == 0x40)
					{
						fdssound->waveaddr = 0;
					}
				}
				break;
			case 9:
				fdssound->op[0].outlvl = LOG_LIN_BITS - LIN_BITS - LIN_BITS - 10 - (value & 3);
				break;
			case 10:
				fdssound->op[1].outlvl = LOG_LIN_BITS - LIN_BITS - LIN_BITS - 10 - (value & 3);
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
	int32_t i;
	FDS_FMOP *pop;
	XMEMSET(fdssound, 0, sizeof(FDSSOUND));
	fdssound->cps = DivFix(NES_BASECYCLES, 12 * (1 << XXX_SHIFT) * NESAudioFrequencyGet(pNezPlay), CPS_SHIFT);
	for (pop = &fdssound->op[0]; pop < &fdssound->op[2]; pop++)
	{
		pop->enable = 1;
	}
	fdssound->op[0].outlvl = LOG_LIN_BITS - LIN_BITS - LIN_BITS - 10;
	fdssound->op[1].outlvl = LOG_LIN_BITS - LIN_BITS - LIN_BITS - 10;
	for (i = 0; i < 0x40; i++)
	{
		fdssound->op[1].wave[i] = LinearToLog((i < 0x20)?0x1f:-0x20);
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

void FDSSoundInstall1(NEZ_PLAY *pNezPlay)
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
