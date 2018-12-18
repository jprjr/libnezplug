#include "../../normalize.h"
#include "../kmsnddev.h"
#include "../../format/audiosys.h"
#include "../../format/handler.h"
#include "../../format/nsf6502.h"
#include "logtable.h"
#include "../../format/m_nsf.h"
#include "s_mmc5.h"
#include "../../common/divfix.h"

#define NES_BASECYCLES (21477270)

/* 31 - log2(NES_BASECYCLES/(12*MIN_FREQ)) > CPS_BITS  */
/* MIN_FREQ:11025 23.6 > CPS_BITS */
/* 32-12(max spd) > CPS_BITS */
#define CPS_BITS 17

typedef struct {
	uint32_t cps;
	int32_t cycles;
	int32_t envphase;

	uint32_t spd;
	uint32_t envspd;

	uint32_t length;
	uint32_t freq;
	uint32_t mastervolume;
	int32_t output;

	uint8_t regs[4];
	uint8_t update;
	uint8_t key;
	uint8_t adr;
	uint8_t envadr;
	uint8_t duty;
	uint8_t mute;
	uint8_t ct;
} MMC5_SQUARE;
typedef struct {
	int32_t output;
	int32_t linearvolume;
	uint8_t key;
	uint8_t mute;
} MMC5_DA;

typedef struct {
	MMC5_SQUARE square[2];
	MMC5_DA da;
	uint8_t mmc5multiplier[2];
	uint8_t mmc5exram[0x400];
	uint8_t regs[0x20];
    LOG_TABLE logtable;
} MMC5SOUND;

/* ----------------- */
/*  MMC5 EXTEND RAM  */
/* ----------------- */

static uint32_t mmc5exram_read(NEZ_PLAY *pNezPlay, uint32_t address)
{
	return ((MMC5SOUND*)((NSFNSF*)pNezPlay->nsf)->mmc5)->mmc5exram[address & 0x03FF];
}

static void mmc5exram_write(NEZ_PLAY *pNezPlay, uint32_t address, uint32_t value)
{
	((MMC5SOUND*)((NSFNSF*)pNezPlay->nsf)->mmc5)->mmc5exram[address & 0x03FF] = (uint8_t)value;
}

static NES_READ_HANDLER mmc5exram_read_handler[] =
{
	{ 0x5C00, 0x5FEF, mmc5exram_read, NULL },
	{ 0,      0,      0, NULL },
};
static NES_WRITE_HANDLER mmc5exram_write_handler[] =
{
	{ 0x5C00, 0x5FEF, mmc5exram_write, NULL },
	{ 0,      0,      0, NULL },
};

void MMC5ExtendRamInstall(NEZ_PLAY *pNezPlay)
{
	NESReadHandlerInstall(pNezPlay, mmc5exram_read_handler);
	NESWriteHandlerInstall(pNezPlay, mmc5exram_write_handler);
}



/* ----------------- */
/*  MMC5 MULTIPLIER  */
/* ----------------- */

static uint32_t mmc5mul_read(NEZ_PLAY *pNezPlay, uint32_t address)
{
	MMC5SOUND *mmc5 = ((NSFNSF*)pNezPlay->nsf)->mmc5;
	uint32_t mul;
	address = address - 0x5205;
	/* if (address > 1) return; */
	mul = mmc5->mmc5multiplier[0] * mmc5->mmc5multiplier[1];
	return address ? (uint8_t)(mul >> 8) & 0xff : (uint8_t)(mul & 0xff);
}

static void mmc5mul_write(NEZ_PLAY *pNezPlay, uint32_t address, uint32_t value)
{
	MMC5SOUND *mmc5 = ((NSFNSF*)pNezPlay->nsf)->mmc5;
	address = address - 0x5205;
	/* if (address > 1) return; */
	mmc5->mmc5multiplier[address] = (uint8_t)value;
}

static NES_READ_HANDLER mmc5mul_read_handler[] =
{
	{ 0x5205, 0x5206, mmc5mul_read, NULL },
	{ 0,      0,      0, NULL },
};
static NES_WRITE_HANDLER mmc5mul_write_handler[] =
{
	{ 0x5205, 0x5206, mmc5mul_write, NULL },
	{ 0,      0,      0, NULL },
};

void MMC5MutiplierInstall(NEZ_PLAY *pNezPlay)
{
	NESReadHandlerInstall(pNezPlay, mmc5mul_read_handler);
	NESWriteHandlerInstall(pNezPlay, mmc5mul_write_handler);
}



/* ------------ */
/*  MMC5 SOUND  */
/* ------------ */

/* 31 - 7 - log2(MAX_FREQ/60) > SPF_BITS */
/* MAX_FREQ:96000 13.3 > SPF_BITS */
#define SPF_BITS 12
#define ENV_BITS	(31 - CPS_BITS - 5/* bitsof env(1-16) */)
#define KEY_ON		1
#define KEY_RELEASE 2

#define V(x) (x*64/60)
static const uint32_t vbl_length[32] =
{
	V(0x05), V(0x7F), V(0x0A), V(0x01), V(0x14), V(0x02), V(0x28), V(0x03),
	V(0x50), V(0x04), V(0x1E), V(0x05), V(0x07), V(0x06), V(0x0E), V(0x07),
	V(0x06), V(0x08), V(0x0C), V(0x09), V(0x18), V(0x0A), V(0x30), V(0x0B),
	V(0x60), V(0x0C), V(0x24), V(0x0D), V(0x08), V(0x0E), V(0x10), V(0x0F),
};
#undef V

static const uint8_t square_duty_table[4][8] = 
	{ {0,0,0,1,0,0,0,0} , {0,0,0,1,1,0,0,0} , {0,0,0,1,1,1,1,0} , {1,0,0,1,1,1,1,1} };

static int32_t MMC5SoundSquareRender(NEZ_PLAY *pNezPlay, MMC5_SQUARE *ch)
{
	MMC5SOUND *mmc5 = ((NSFNSF*)pNezPlay->nsf)->mmc5;
	int32_t outputbuf=0,count=0;
	if (ch->update)
	{
		if (ch->update & 1)
		{
			ch->duty = (ch->regs[0] >> 6) & 0x03;
			//if (ch->duty == 0) ch->duty = 2;
			ch->envspd = ((ch->regs[0] & 0x0F) + 1) << (CPS_BITS + ENV_BITS);
		}
		if (ch->update & (4 | 8))
		{
			ch->spd = (((ch->regs[3] & 7) << 8) + ch->regs[2] + 1) << CPS_BITS;
		}
		if ((ch->update & 8) && ch->key)
		{
			ch->length = (vbl_length[ch->regs[3] >> 3] * ch->freq) >> 6;
			ch->envadr = 0;
			ch->adr = 0;
			ch->ct = 0x30;
		}
		ch->update = 0;
	}

	if (ch->key == 0) return 0;

	ch->envphase -= ch->cps >> (13 - ENV_BITS);
	if (ch->regs[0] & 0x20)
	{
		while (ch->envphase < 0)
		{
			ch->envphase += ch->envspd;
			ch->envadr++;
		}
		ch->envadr &= 0x0F;
	}
	else
	{
		while (ch->envphase < 0)
		{
			ch->envphase += ch->envspd;
			ch->envadr += (ch->envadr < 15);
		}
	}

	if (ch->length)
	{
		if (!(ch->regs[0] & 0x20)) ch->length--;
	}
	else
	{
//		ch->key = 0;
	}

	if (ch->spd < (8 << CPS_BITS)) return 0;
/*	if (!(ch->regs[1] & 8))
	{
		if (ch->spd > spd_limit[ch->regs[1] & 7]) return 0;
	}
*/
	if (ch->regs[0] & 0x10) /* fixed volume */
		ch->output = ch->regs[0] & 0x0F;
	else
		ch->output = 15 - ch->envadr;

	ch->output = LinearToLog(&mmc5->logtable,ch->output) + ch->mastervolume;
	ch->output += square_duty_table[ch->duty][ch->adr];
	ch->output = LogToLinear(&mmc5->logtable,ch->output, mmc5->logtable.log_lin_bits - mmc5->logtable.lin_bits - 16);
	
	ch->cycles -= ch->cps << 6;
	while (ch->cycles < 0)
	{
		outputbuf += ch->output;
		count++;

		ch->cycles += ch->spd;
		ch->ct++;
		if(ch->ct >= (1<<7)){
			ch->ct = 0;

			ch->adr++;
			ch->adr &= 0x07;

			if (ch->regs[0] & 0x10) /* fixed volume */
				ch->output = ch->regs[0] & 0x0F;
			else
				ch->output = 15 - ch->envadr;

			ch->output = LinearToLog(&mmc5->logtable,ch->output) + ch->mastervolume;
			ch->output += square_duty_table[ch->duty][ch->adr];
			ch->output = LogToLinear(&mmc5->logtable,ch->output, mmc5->logtable.log_lin_bits - mmc5->logtable.lin_bits - 16);
		}

	}

	if (ch->mute) return 0;

	outputbuf += ch->output;
	count++;

	return outputbuf /count;

}

static void MMC5SoundSquareReset(NEZ_PLAY *pNezPlay, MMC5_SQUARE *ch)
{
	XMEMSET(ch, 0, sizeof(MMC5_SQUARE));
	ch->freq = NESAudioFrequencyGet(pNezPlay);
	ch->cps = DivFix(NES_BASECYCLES, 12 * ch->freq, CPS_BITS);
}



static int32_t MMC5SoundRender(NEZ_PLAY *pNezPlay)
{
	MMC5SOUND *mmc5 = ((NSFNSF*)pNezPlay->nsf)->mmc5;
	int32_t accum = 0;
	accum += MMC5SoundSquareRender(pNezPlay, &mmc5->square[0]) * pNezPlay->chmask[NEZ_DEV_MMC5_SQ1];
	accum += MMC5SoundSquareRender(pNezPlay, &mmc5->square[1]) * pNezPlay->chmask[NEZ_DEV_MMC5_SQ2];
	if(pNezPlay->chmask[NEZ_DEV_MMC5_DA])
		if (!mmc5->da.key && !mmc5->da.mute) accum += mmc5->da.output * mmc5->da.linearvolume;
	return accum;
}

static const NEZ_NES_AUDIO_HANDLER s_mmc5_audio_handler[] = {
	{ 1, MMC5SoundRender, NULL, NULL }, 
	{ 0, 0, NULL, NULL }, 
};

static void MMC5SoundVolume(NEZ_PLAY *pNezPlay, uint32_t volume)
{
	MMC5SOUND *mmc5 = ((NSFNSF*)pNezPlay->nsf)->mmc5;
	volume = (volume << (mmc5->logtable.log_bits - 8)) << 1;
	mmc5->square[0].mastervolume = volume;
	mmc5->square[1].mastervolume = volume;
	mmc5->da.linearvolume = LogToLinear(&mmc5->logtable,volume, mmc5->logtable.log_lin_bits - 16);
}

static const NEZ_NES_VOLUME_HANDLER s_mmc5_volume_handler[] = {
	{ MMC5SoundVolume, NULL },
	{ 0, NULL }, 
};

static void MMC5SoundWrite(NEZ_PLAY *pNezPlay, uint32_t address, uint32_t value)
{
	if (0x5000 <= address && address <= 0x5015)
	{
		MMC5SOUND *mmc5 = ((NSFNSF*)pNezPlay->nsf)->mmc5;
		mmc5->regs[address-0x5000] = value;
		switch (address)
		{
			case 0x5000: case 0x5002: case 0x5003:
			case 0x5004: case 0x5006: case 0x5007:
				{
					int ch = address >= 0x5004;
					int port = address & 3;
					mmc5->square[ch].regs[port] = (uint8_t)value;
					mmc5->square[ch].update |= 1 << port; 
				}
				break;
			case 0x5011:
				mmc5->da.output = ((int32_t)(value & 0xff)) - 0x80;
				break;
			case 0x5010:
				mmc5->da.key = (uint8_t)(value & 0x01);
				break;
			case 0x5015:
				if (value & 1)
					mmc5->square[0].key = 1;
				else
				{
					mmc5->square[0].key = 0;
					mmc5->square[0].length = 0;
				}
				if (value & 2)
					mmc5->square[1].key = 1;
				else
				{
					mmc5->square[1].key = 0;
					mmc5->square[1].length = 0;
				}
				break;
		}
	}
}

static NES_WRITE_HANDLER s_mmc5_write_handler[] =
{
	{ 0x5000, 0x5015, MMC5SoundWrite, NULL },
	{ 0,      0,      0, NULL },
};

static void MMC5SoundDaReset(MMC5_DA *ch)
{
	XMEMSET(ch, 0, sizeof(MMC5_DA));
}

static void MMC5SoundReset(NEZ_PLAY *pNezPlay)
{
	MMC5SOUND *mmc5 = ((NSFNSF*)pNezPlay->nsf)->mmc5;
    uint32_t *lineartbl = mmc5->logtable.lineartbl;
    uint32_t *logtbl = mmc5->logtable.logtbl;
	MMC5SoundSquareReset(pNezPlay, &mmc5->square[0]);
	MMC5SoundSquareReset(pNezPlay, &mmc5->square[1]);
	MMC5SoundDaReset(&mmc5->da);

	MMC5SoundWrite(pNezPlay, 0x5000, 0x00);
	MMC5SoundWrite(pNezPlay, 0x5002, 0x00);
	MMC5SoundWrite(pNezPlay, 0x5003, 0x00);
	MMC5SoundWrite(pNezPlay, 0x5004, 0x00);
	MMC5SoundWrite(pNezPlay, 0x5006, 0x00);
	MMC5SoundWrite(pNezPlay, 0x5007, 0x00);
	MMC5SoundWrite(pNezPlay, 0x5010, 0x00);
	MMC5SoundWrite(pNezPlay, 0x5011, 0x80);
    mmc5->logtable.log_bits = 12;
    mmc5->logtable.lin_bits = 8;
    mmc5->logtable.log_lin_bits = 30;
    mmc5->logtable.lineartbl = lineartbl;
    mmc5->logtable.logtbl = logtbl;
}

static const NEZ_NES_RESET_HANDLER s_mmc5_reset_handler[] = {
	{ NES_RESET_SYS_NOMAL, MMC5SoundReset, NULL }, 
	{ 0,                   0, NULL }, 
};

static void MMC5SoundTerm(NEZ_PLAY *pNezPlay)
{
	MMC5SOUND *mmc5 = ((NSFNSF*)pNezPlay->nsf)->mmc5;
	if (mmc5) {
        LogTableFree(&mmc5->logtable);
		XFREE(mmc5);
    }
}

static const NEZ_NES_TERMINATE_HANDLER s_mmc5_terminate_handler[] = {
	{ MMC5SoundTerm, NULL }, 
	{ 0, NULL }, 
};

void MMC5SoundInstall(NEZ_PLAY *pNezPlay)
{
	MMC5SOUND *mmc5;
	mmc5 = XMALLOC(sizeof(MMC5SOUND));
	if (!mmc5) return;
	XMEMSET(mmc5, 0, sizeof(MMC5SOUND));
	((NSFNSF*)pNezPlay->nsf)->mmc5 = mmc5;

    mmc5->logtable.log_bits = 12;
    mmc5->logtable.lin_bits = 8;
    mmc5->logtable.log_lin_bits = 30;
	if(LogTableInitialize(&mmc5->logtable) != 0) return;
	NESAudioHandlerInstall(pNezPlay, s_mmc5_audio_handler);
	NESVolumeHandlerInstall(pNezPlay, s_mmc5_volume_handler);
	NESTerminateHandlerInstall(&pNezPlay->nth, s_mmc5_terminate_handler);
	NESWriteHandlerInstall(pNezPlay, s_mmc5_write_handler);
	NESResetHandlerInstall(pNezPlay->nrh, s_mmc5_reset_handler);

}

#undef SPF_BITS
#undef ENV_BITS
#undef KEY_ON
#undef KEY_RELEASE

#undef NES_BASECYCLES
#undef CPS_BITS
