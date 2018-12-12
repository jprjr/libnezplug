#include "../../normalize.h"
#include "../kmsnddev.h"
#include "../../format/audiosys.h"
#include "../../format/nsf6502.h"
#include "logtable.h"
#include "../../format/m_nsf.h"
#include "s_fds.h"
//#include <math.h>
#define FDS_DYNAMIC_BIAS 1
int FDS_RealMode = 3;

#define FM_DEPTH 0 /* 0,1,2 */
#define NES_BASECYCLES (21477270)
#define PGCPS_BITS (32-16-6)
#define EGCPS_BITS (12)
#define VOL_BITS 12
#define RENDERS 4

typedef struct {
	uint8_t spd;
	uint8_t cnt;
	uint8_t mode;
	uint8_t volume;
} FDS_EG;
typedef struct {
	uint32_t spdbase;
	uint32_t spd;
	uint32_t freq;
} FDS_PG;
typedef struct {
	uint32_t phase;
	uint32_t phase2;
	int8_t wave[0x40];
	uint8_t wavereg[0x40];
	uint8_t wavptr;
	uint32_t pt;
	int8_t output;
	int32_t output32;
	int32_t output32bf;
	uint8_t disable;
	uint8_t disable2;
} FDS_WG;
typedef struct {
	FDS_EG eg;
	FDS_PG pg;
	FDS_WG wg;
	int32_t bias;
	uint8_t wavebase;
	uint8_t d[2];
} FDS_OP;

typedef struct FDSSOUND_tag {
	FDS_OP op[2];
	uint32_t phasecps;
	uint32_t envcnt;
	uint32_t envspd;
	uint32_t envcps;
	uint8_t envdisable;
	uint8_t d[3];
	uint32_t lvl;
	int32_t mastervolumel[4];
	uint32_t mastervolume;
	uint32_t srate;
	uint8_t reg[0x10];
	uint32_t count;
	int32_t realout[0x40];
	int32_t lowpass;
	int32_t outbf;
} FDSSOUND;

#if (((-1) >> 1) == -1)
/* RIGHT SHIFT IS SIGNED */
#define SSR(x, y) (((int32_t)x) >> (y))
#else
/* RIGHT SHIFT IS UNSIGNED */
#define SSR(x, y) (((x) >= 0) ? ((x) >> (y)) : (-((-(x) - 1) >> (y)) - 1))
#endif


static void FDSSoundPhaseStep(FDS_OP *op , uint32_t spd)
{
	if(op->wg.disable) return;
	op->wg.pt += spd;
	while (op->wg.pt >= (1 << (PGCPS_BITS+16)))
	{
		op->wg.pt -= (1 << (PGCPS_BITS+16));
		op->bias = (op->bias + op->wg.wave[(op->wg.phase) & 0x3f]) & 0x7f;
		if((uint8_t)op->wg.wave[(op->wg.phase) & 0x3f] == 64) op->bias = 0;
		op->wg.phase++;
	}
}

static void FDSSoundEGStep(FDS_EG *peg)
{
	if (peg->mode & 0x80) return;
	if (++peg->cnt <= peg->spd) return;
	peg->cnt = 0;
	if (peg->mode & 0x40)
		peg->volume += (peg->volume < 0x1f);
	else
		peg->volume -= (peg->volume > 0);

}


static int32_t FDSSoundRender(NEZ_PLAY *pNezPlay)
{
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;
	int32_t output;
	int32_t outputbuf=0,count=0;

	/* Frequency Modulator */
	fdssound->op[1].pg.spd = fdssound->op[1].pg.spdbase;
//	if (fdssound->op[1].wg.disable){
//		fdssound->op[0].pg.spd = fdssound->op[0].pg.spdbase;
//	}
//	else
	{
		int32_t v1,v2;
#if FDS_DYNAMIC_BIAS
		/* この式を変に書き換えると、ナゾラーランド第３号の爆走トモちゃんのBGMのFDS音源のピッチが
		   １オクターブ下がる恐れが非常に大きい。 */
/*
	value_1 = Sweep envelope出力値 * Sweep Bias;	// (1)
	value_2 = value_1 / 16;				// (2)

	if( (value_1 % 16) != 0 ) {			// (3)
		if( Sweep Bias >= 0 ) {
			value_2 = value_2 + 2;
		} else {
			value_2 = value_2 - 1;
		}
	}

	if( value_2 > 193 )				// (4)
		value_2 = value_2 - 258;

	Freq = Main Freq * value_2 / 64;		// (5)
	Freq = Main Freq + Freq;			// (6)
	if( Freq < 0 )
		Freq = (Main Freq * 4) + Freq;		// (7)
*/
		v1 = (int32_t)((fdssound->op[1].bias & 0x40) ? (fdssound->op[1].bias - 128) : fdssound->op[1].bias) * ((int32_t)(fdssound->op[1].eg.volume));
		v2 = v1 / 16;
//		v1 = ((int32_t)fdssound->op[1].eg.volume) * (((int32_t)(((uint8_t)fdssound->op[1].wg.output) & 127)) - 64);
#else
		v1 = 0x10000 + ((int32_t)fdssound->op[1].eg.volume) * (((int32_t)((((uint8_t)fdssound->op[1].wg.output)                      ) & 255)) - 64);
#endif
		if(v1&15){
			if(fdssound->op[1].bias & 0x40){
				v2-=1;
			}else{
				v2+=2;
			}
		}
		if(v2>193)v2-=258;

//		v1 = (((4096 + 1024 + (int32_t)v1) & 0xfff)+8)/16 - 64 + (((int32_t)v1 & 0xf) ? ((v1 < 0) ? -1 : 2) : 0);
//		v1 = v1<0 ? SSR(v1-8,4) : v1>0 ? SSR(v1+8,4) : 0; //doubleの無い四捨五入
		v1 = ((int32_t)(fdssound->op[0].pg.freq * v2) / 64);
		v1 = v1 + (int32_t)fdssound->op[0].pg.freq;
		if( v1 < 0 )
			v1 = (fdssound->op[0].pg.freq * 4) + v1;
		fdssound->op[0].pg.spd = v1 * fdssound->phasecps;
	}

	/* Accumulator */
//	output = fdssound->op[0].eg.volume;
//	if (output > 0x20) output = 0x20;
//	output = (fdssound->op[0].wg.output * output * fdssound->mastervolumel[fdssound->lvl]) >> (VOL_BITS - 4);

	/* Envelope Generator */
	if (!fdssound->envdisable && fdssound->envspd)
	{
		fdssound->envcnt += fdssound->envcps;
		while (fdssound->envcnt >= fdssound->envspd)
		{
			fdssound->envcnt -= fdssound->envspd;
			FDSSoundEGStep(&fdssound->op[1].eg);
			FDSSoundEGStep(&fdssound->op[0].eg);
		}
	}
	/* Phase Generator */
	FDSSoundPhaseStep(&fdssound->op[1] , fdssound->op[1].pg.spd);

	/* Wave Generator */
	fdssound->op[0].wg.phase2 += fdssound->op[0].pg.spd;

	output = fdssound->op[0].eg.volume;
	if (output > 0x20) output = 0x20;
	outputbuf += (fdssound->op[0].wg.output32 * output * (fdssound->mastervolumel[fdssound->lvl] >> (VOL_BITS - 2)));
	count++;

	if (fdssound->op[0].wg.disable || fdssound->op[0].wg.disable2);
	else{
		while(fdssound->op[0].wg.phase2 > (1<<(PGCPS_BITS+12))){
			fdssound->op[0].wg.phase2 -= 1<<(PGCPS_BITS+12);
			fdssound->op[0].wg.phase += 1<<(PGCPS_BITS+12);
			fdssound->op[0].wg.output32
				= fdssound->realout[fdssound->op[0].wg.wavereg[(fdssound->op[0].wg.phase >> (PGCPS_BITS+16)) & 0x3f]];
			if (output > 0x20) output = 0x20;
			outputbuf += (fdssound->op[0].wg.output32 * output * (fdssound->mastervolumel[fdssound->lvl] >> (VOL_BITS - 2)));
			count++;
		}
	}

	outputbuf = (fdssound->op[0].pg.freq != 0) ? outputbuf / 3 : 0;
	if(FDS_RealMode & 1)
	{
		fdssound->outbf += (outputbuf/count - fdssound->outbf) *4 / fdssound->lowpass;
		outputbuf = fdssound->outbf;
	}else{
		outputbuf /= count;
	}
	if(!pNezPlay->chmask[DEV_FDS_CH1]) return 0;
	return outputbuf;
}

const static NEZ_NES_AUDIO_HANDLER s_fds_audio_handler[] =
{
	{ 1, FDSSoundRender, NULL, NULL }, 
	{ 0, 0, NULL, NULL }, 
};

static void FDSSoundVolume(NEZ_PLAY *pNezPlay, uint32_t volume)
{
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;
	volume += 196;
	fdssound->mastervolume = (volume << (LOG_BITS - 15 + LIN_BITS)) << 1;
	fdssound->mastervolumel[0] = LogToLinear(fdssound->mastervolume, LOG_LIN_BITS - LIN_BITS - VOL_BITS) * 2;
	fdssound->mastervolumel[1] = LogToLinear(fdssound->mastervolume, LOG_LIN_BITS - LIN_BITS - VOL_BITS) * 4 / 3;
	fdssound->mastervolumel[2] = LogToLinear(fdssound->mastervolume, LOG_LIN_BITS - LIN_BITS - VOL_BITS) * 2 / 2;
	fdssound->mastervolumel[3] = LogToLinear(fdssound->mastervolume, LOG_LIN_BITS - LIN_BITS - VOL_BITS) * 8 / 10;
}

const static NEZ_NES_VOLUME_HANDLER s_fds_volume_handler[] = {
	{ FDSSoundVolume, NULL }, 
	{ 0, NULL }, 
};

static const uint8_t wave_delta_table[8] = {
	0,(1 << FM_DEPTH),(2 << FM_DEPTH),(4 << FM_DEPTH),
	64,256 - (4 << FM_DEPTH),256 - (2 << FM_DEPTH),256 - (1 << FM_DEPTH),
};

static void FDSSoundWrite(NEZ_PLAY *pNezPlay, uint32_t address, uint32_t value)
{
	FDSSOUND *fdssound = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->fdssound;
	if (0x4040 <= address && address <= 0x407F)
	{
		fdssound->op[0].wg.wavereg[address - 0x4040] = value & 0x3f;
		fdssound->op[0].wg.wave[address - 0x4040] = ((int)(value & 0x3f)) - 0x20;
	}
	else if (0x4080 <= address && address <= 0x408F)
	{
		FDS_OP *pop = &fdssound->op[(address & 4) >> 2];
		fdssound->reg[address - 0x4080] = (uint8_t)value;
		switch (address & 0xf)
		{
			case 0:
			case 4:
				pop->eg.mode = (uint8_t)(value & 0xc0);
				if (pop->eg.mode & 0x80)
				{
					pop->eg.volume = (uint8_t)(value & 0x3f);
				}
				else
				{
					pop->eg.spd = (uint8_t)(value & 0x3f);
				}
				break;
			case 5:
#if 1
				fdssound->op[1].bias = (uint8_t)(value & 0x7f);
#else
				fdssound->op[1].bias = (((value & 0x7f) ^ 0x40) - 0x40) & 255;
#endif
#if 0
				fdssound->op[1].wg.phase = 0;
#endif
				pop->wg.pt = 0;
				pop->wg.phase = 0;
				pop->wavebase = 0;
				break;
			case 2:	case 6:
				pop->pg.freq &= 0x00000F00;
				pop->pg.freq |= (value & 0xFF) << 0;
				pop->pg.spdbase = pop->pg.freq * fdssound->phasecps;
				if(pop->pg.freq==0){
					//pop->wg.phase = 0;
				}
				break;
			case 3:
				fdssound->envdisable = (uint8_t)(value & 0x40);
//				pop->pg.spdbase = pop->pg.freq * fdssound->phasecps;
				if (value & 0x80){
					pop->wg.phase = 0;
//					pop->wg.wavptr = 0;
				}
			case 7:
				pop->pg.freq &= 0x000000FF;
				pop->pg.freq |= (value & 0x0F) << 8;
				pop->pg.spdbase = pop->pg.freq * fdssound->phasecps;
				pop->wg.disable = (uint8_t)(value & 0x80);
				if (fdssound->op[1].wg.disable){
					//fdssound->op[1].bias = 0;
				}
				break;
			case 8:
				if (fdssound->op[1].wg.disable)
				{
					int32_t idx = value & 7;
					fdssound->op[1].wg.wavereg[fdssound->op[1].wg.wavptr + 0] = idx;
					fdssound->op[1].wg.wavereg[fdssound->op[1].wg.wavptr + 1] = idx;
#if FDS_DYNAMIC_BIAS
					fdssound->op[1].wg.wave[fdssound->op[1].wg.wavptr + 0] = wave_delta_table[idx];
					fdssound->op[1].wg.wave[fdssound->op[1].wg.wavptr + 1] = wave_delta_table[idx];
					fdssound->op[1].wg.wavptr = (fdssound->op[1].wg.wavptr + 2) & 0x3f;
#else
					fdssound->op[1].wavebase += wave_delta_table[idx];
					fdssound->op[1].wg.wave[fdssound->op[1].wg.wavptr + 0] = (fdssound->op[1].wavebase + fdssound->op[1].bias + 64) & 255;
					fdssound->op[1].wavebase += wave_delta_table[idx];
					fdssound->op[1].wg.wave[fdssound->op[1].wg.wavptr + 1] = (fdssound->op[1].wavebase + fdssound->op[1].bias + 64) & 255;
					fdssound->op[1].wg.wavptr = (fdssound->op[1].wg.wavptr + 2) & 0x3f;
#endif
				}
				break;
			case 9:
				fdssound->lvl = (value & 3);
				fdssound->op[0].wg.disable2 = (uint8_t)(value & 0x80);
				break;
			case 10:
				fdssound->envspd = value << EGCPS_BITS;
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
	if (0x4040 <= address && address <= 0x407f)
	{
		return fdssound->op[0].wg.wave[address & 0x3f] + 0x20;
	}
	if (0x4080 <= address && address <= 0x408f)
	{
		return fdssound->reg[address & 0xf];
	}
	if (0x4090 == address)
		return fdssound->op[0].eg.volume | 0x40;
	if (0x4092 == address) /* 4094? */
		return fdssound->op[1].eg.volume | 0x40;
//	if (0x4094 == address) /* 4094? */
//		return fdssound->op[1].wg.freq;
	return 0;
}

static NES_READ_HANDLER s_fds_read_handler[] =
{
	{ 0x4040, 0x409F, FDSSoundRead, NULL },
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
	XMEMSET(fdssound, 0, sizeof(FDSSOUND));
	fdssound->srate = NESAudioFrequencyGet(pNezPlay);
	fdssound->envcps = DivFix(NES_BASECYCLES, 12 * fdssound->srate, EGCPS_BITS + 5 - 9 + 1);
	fdssound->envspd = 0xe8 << EGCPS_BITS;
	fdssound->envdisable = 1;
	fdssound->phasecps = DivFix(NES_BASECYCLES, 12 * fdssound->srate, PGCPS_BITS);
	for (i = 0; i < 0x40; i++)
	{
		fdssound->op[0].wg.wave[i] = 0;
//		fdssound->op[0].wg.wave[i] = (i < 0x20) ? 0x1f : -0x20;
		fdssound->op[1].wg.wave[i] = 64;
	}
	fdssound->op[1].wg.pt = 0;

	//リアル出力計算
#define BIT(x) ((i&(1<<x))>>x)
	for (i = 0; i < 0x40; i++)
	{
		if(FDS_RealMode & 2)
			/* FDS音源出力の際、NOT回路のIC（BU4069UB）によるローパスフィルタをかけているので、波形が上下逆になる。 */
			fdssound->realout[i]=-(i*4+(BIT(0)+BIT(1)+BIT(2)+BIT(3)+BIT(4)+BIT(5))*(0+(i*3)/0x5) - 239);
		else
			fdssound->realout[i]=i*7                                               - 239;
	}
	//ローパス計算
//	fdssound->lowpass = sqrt(fdssound->srate / 500.0);
	fdssound->lowpass = (int32_t)(fdssound->srate / 11025.0 *4);
	if(fdssound->lowpass<4)fdssound->lowpass=4;
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

//ここからレジスタビュアー設定
uint8_t *fds_regdata;
uint8_t *fds_regdata2;
uint8_t *fds_regdata3;
uint32_t (*ioview_ioread_DEV_FDS)(uint32_t a);
static uint32_t ioview_ioread_bf(uint32_t a){
	if(         a<=0x0f)return fds_regdata[a];
	if(a>=0x20&&a<=0x5f)return fds_regdata2[a-0x20];
	if(a>=0x70&&a<=0x8f)return fds_regdata3[(a-0x70)*2];
	else return 0x100;

}
//ここまでレジスタビュアー設定

void FDSSoundInstall3(NEZ_PLAY *pNezPlay)
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

	//ここからレジスタビュアー設定
	fds_regdata = fdssound->reg;
	fds_regdata2 = fdssound->op[0].wg.wavereg;
	fds_regdata3 = fdssound->op[1].wg.wavereg;
	ioview_ioread_DEV_FDS = ioview_ioread_bf;
	//ここまでレジスタビュアー設定
}
