#include "../../normalize.h"
#include "../kmsnddev.h"
#include "../../format/audiosys.h"
#include "../../format/handler.h"
#include "../../format/nsf6502.h"
#include "logtable.h"
#include "../../format/m_nsf.h"
#include "s_n106.h"

#define NES_BASECYCLES (21477270)
#define CPS_SHIFT (16)
#define PHASE_SHIFT (16+2)
#define  N106SOUND_DATA 0x4800
#define  N106SOUND_ADDR 0xF800

#define REAL_RENDERS 6
#define REAL_OFS_BASE 200000
#define REAL_OFS_COUNT 16
#define RENDERS 4
#define NAMCO106_VOL Namco106_Volume/16
#define CPSF_SHIFT 4
int Namco106_Realmode = 0;
int Namco106_Volume = 16;

typedef struct {
	uint32_t logvol;
	uint32_t cycles;
	uint32_t cycles2;
	uint32_t spd;
	uint32_t phase;
	uint32_t tlen;

	uint8_t update;
	uint8_t freql;
	uint8_t freqm;
	uint8_t freqh;
	uint8_t vreg;
	uint8_t tadr;
	uint8_t nazo;
	uint8_t mute;
	int32_t output;
	uint32_t count;
} N106_WM;

typedef struct {
	uint32_t cps;
	uint32_t cpsf;
	int32_t output;
	int32_t offset;
	uint32_t ofscount;
	uint32_t ofscps;
	uint8_t outputfg;
	uint32_t mastervolume;

	N106_WM ch[8];

	uint8_t addressauto;
	uint8_t address;
	uint8_t chinuse;

	uint32_t tone[0x100];	/* TONE DATA */
	uint8_t data[0x80];
} N106SOUND;

static uint32_t DivFix(uint32_t p1, uint32_t p2, uint32_t fix)
{
	uint32_t ret;
	ret = p1 / p2;
	p1 %= p2;/* p1 = p1 - p2 * ret; */
	//while(p1 >= p2)p1 -= p2;
	while (fix--)
	{
		//p1 += p1;
		//ret += ret;
		p1 <<= 1;
		ret <<= 1;
		if (p1 >= p2)
		{
			p1 -= p2;
			ret++;
		}
	}
	return ret;
}

__inline static void UPDATE(N106_WM *chp)
{
	if (chp->update & 3)
	{
		uint32_t freq;
		freq  = ((int)chp->freql);
		freq += ((int)chp->freqm) << 8;
		freq += ((int)chp->freqh) << 16;
		chp->spd = freq & 0x3ffff;
	}
	if (chp->update & 2)
	{
		uint32_t tlen;
		tlen = (0x100 - (chp->freqh & 0xfc)) << PHASE_SHIFT;
		if (chp->tlen != tlen)
		{
			chp->tlen = tlen;
			chp->phase = 0;
		}
	}
	if (chp->update & 4)
	{
		chp->logvol = LinearToLog((chp->vreg & 0x0f) << 2);
	}
	chp->update = 0;
}

static int32_t N106SoundRenderReal2(void* pNezPlay)
{
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	N106_WM *chp;

	int32_t outputbuf=0,count=0,accum=0,chpn,real2=((1<<REAL_RENDERS) / n106s->chinuse);
	uint32_t cyclesspd = n106s->chinuse << CPS_SHIFT;

	//リアルモード
	/*波形は、1chずつ出力される。
	  波形データ、基準は"8"。volを下げると、8に向かって+-が減衰していく。
	  波形データ8を再生中は高周波ノイズは出ない。8からの差とノイズの大きさは比例する。
	*/
	for (chp = &n106s->ch[0],chpn = 0
		; chp < &n106s->ch[8]; chp++,chpn++)
	{
		accum = 0;
		count = 0;
		if (chp->mute || !chp->logvol) continue;
		if (chp->update) UPDATE(chp);
		chp->cycles2 += n106s->cps << REAL_RENDERS;
		chp->output = LogToLinear(n106s->tone[((chp->phase >> PHASE_SHIFT) + chp->tadr) & 0xff] + chp->logvol + n106s->mastervolume, LOG_LIN_BITS - LIN_BITS - LIN_BITS - 11);
		while (chp->cycles2 >= n106s->cpsf){
			if (((int32_t)chp->count / real2) + 8 - n106s->chinuse == chpn)accum += chp->output;
			count++;
			chp->cycles2 -= n106s->cpsf;
			chp->count++;
			if(chp->count >= (1<<REAL_RENDERS)){
				chp->count = 0;
				chp->cycles += n106s->cpsf;

				chp->phase += chp->spd * (chp->cycles / cyclesspd);
				chp->cycles %= cyclesspd;
				//while(chp->cycles >= cyclesspd)chp->cycles -= cyclesspd;
				chp->phase %= chp->tlen;
				//while(chp->phase >= chp->tlen)chp->phase -= chp->tlen;
				chp->output = LogToLinear(n106s->tone[((chp->phase >> PHASE_SHIFT) + chp->tadr) & 0xff] + chp->logvol + n106s->mastervolume, LOG_LIN_BITS - LIN_BITS - LIN_BITS - 11);
			}
		}
		if (((int32_t)chp->count / real2) == chpn)accum += chp->output;
		count++;
		if (chmask[DEV_N106_CH1+chpn])outputbuf += accum / count;
	}
/*	n106s->ofscount += n106s->ofscps;
	while(n106s->ofscount >= REAL_OFS_COUNT){
		n106s->ofscount -= REAL_OFS_COUNT;
		n106s->offset += (outputbuf - n106s->offset) / 64; 
	}
	return (outputbuf - n106s->offset) * NAMCO106_VOL;
*/	return outputbuf * NAMCO106_VOL;
}

static int32_t N106SoundRenderReal(void* pNezPlay)
{
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	N106_WM *chp;

	int32_t outputbuf=0,count=0,accum=0,chpn;
	uint32_t cyclesspd = n106s->chinuse << CPS_SHIFT;

	//リアルモード
	for (chp = &n106s->ch[8 - n106s->chinuse],chpn = 8 - n106s->chinuse
		; chp < &n106s->ch[8]; chp++,chpn++)
	{
		accum = 0;
		count = 0;
		if (chp->mute || !chp->logvol) continue;
		if (chp->update) UPDATE(chp);
		chp->cycles2 += n106s->cps << REAL_RENDERS;
		chp->output = LogToLinear(n106s->tone[((chp->phase >> PHASE_SHIFT) + chp->tadr) & 0xff] + chp->logvol + n106s->mastervolume, LOG_LIN_BITS - LIN_BITS - LIN_BITS - 9);
		while (chp->cycles2 >= n106s->cpsf){
			accum += chp->output;
			count++;
			chp->cycles2 -= n106s->cpsf;
			chp->count++;
			if(chp->count >= (1<<REAL_RENDERS)){
				chp->count = 0;
				chp->cycles += n106s->cpsf;

				chp->phase += chp->spd * (chp->cycles / cyclesspd);
				chp->cycles %= cyclesspd;
				//while(chp->cycles >= cyclesspd)chp->cycles -= cyclesspd;
				chp->phase %= chp->tlen;
				//while(chp->phase >= chp->tlen)chp->phase -= chp->tlen;
				chp->output = LogToLinear(n106s->tone[((chp->phase >> PHASE_SHIFT) + chp->tadr) & 0xff] + chp->logvol + n106s->mastervolume, LOG_LIN_BITS - LIN_BITS - LIN_BITS - 9);
			}
		}
		accum += chp->output;
		count++;
		if(chmask[DEV_N106_CH1+chpn])outputbuf += accum / count;
	}
	return outputbuf * NAMCO106_VOL;
}


static int32_t N106SoundRenderNormal(void* pNezPlay)
{
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	N106_WM *chp;

	int32_t outputbuf=0,count=0,accum=0,chpn;
	uint32_t cyclesspd = n106s->chinuse << CPS_SHIFT;
	//従来の方法
	for (chp = &n106s->ch[8 - n106s->chinuse],chpn = 8 - n106s->chinuse
		; chp < &n106s->ch[8]; chp++,chpn++)
	{
		if (chp->mute || !chp->logvol) continue;
		accum = 0;
		count = 0;
		if (chp->update) UPDATE(chp);
		chp->cycles += n106s->cps << RENDERS;
		chp->output = LogToLinear(n106s->tone[((chp->phase >> PHASE_SHIFT) + chp->tadr) & 0xff] + chp->logvol + n106s->mastervolume, LOG_LIN_BITS - LIN_BITS - LIN_BITS - 9);
		while (chp->cycles >= cyclesspd)
		{
			accum += chp->output;
			count++;
			chp->cycles -= cyclesspd;
//			chp->count++;
//			if(chp->count >= (1<<RENDERS)){
				chp->count = 0;
				chp->phase += chp->spd >> RENDERS;
				//chp->phase %= chp->tlen;
				if(chp->phase >= chp->tlen)
					do{
						chp->phase -= chp->tlen;
					}while(chp->phase >= chp->tlen);
				chp->output = LogToLinear(n106s->tone[((chp->phase >> PHASE_SHIFT) + chp->tadr) & 0xff] + chp->logvol + n106s->mastervolume, LOG_LIN_BITS - LIN_BITS - LIN_BITS - 9);
//			}
		}
		accum += chp->output;
		count++;
		if(chmask[DEV_N106_CH1+chpn])outputbuf += accum / count;
	}
	return outputbuf * NAMCO106_VOL;
}
static int32_t N106SoundRender(void* pNezPlay)
{
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	switch(Namco106_Realmode){
	case 1:
		return n106s->chinuse < 8 ? N106SoundRenderReal(pNezPlay) : N106SoundRenderReal2(pNezPlay);
	case 2:
		return N106SoundRenderReal(pNezPlay);
	case 3:
		return N106SoundRenderReal2(pNezPlay);
	default:
		return N106SoundRenderNormal(pNezPlay);
	}
}

const static NES_AUDIO_HANDLER s_n106_audio_handler[] = {
	{ 1, N106SoundRender, NULL , NULL }, 
	{ 0, 0, NULL, NULL }, 
};

static void N106SoundVolume(void* pNezPlay, uint32_t volume)
{
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	n106s->mastervolume = (volume << (LOG_BITS - 8)) << 1;
}

const static NES_VOLUME_HANDLER s_n106_volume_handler[] = {
	{ N106SoundVolume, NULL }, 
	{ 0, NULL }, 
};

static void N106SoundWriteAddr(void *pNezPlay, uint32_t address, uint32_t value)
{
    (void)address;
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	n106s->address     = (uint8_t)(value & 0x7f);
	n106s->addressauto = (value & 0x80) ? 1 : 0;
}

static void N106SoundWriteData(void *pNezPlay, uint32_t address, uint32_t value)
{
    (void)address;
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	n106s->data[n106s->address] = (uint8_t)value;
	n106s->tone[n106s->address * 2]     = LinearToLog(((int32_t)(value & 0xf) << 2) - 0x20);
	n106s->tone[n106s->address * 2 + 1] = LinearToLog(((int32_t)(value >>  4) << 2) - 0x20);
	if (n106s->address >= 0x40)
	{
		N106_WM *chp = &n106s->ch[(n106s->address - 0x40) >> 3];
		switch (n106s->address & 7)
		{
/*
$78 0-7：周波数レジスタ加算値（0-7bit）
$79 0-7：周波数レジスタ代入値（0-7bit）
$7a 0-7：周波数レジスタ加算値（8-15bit）
$7b 0-7：周波数レジスタ代入値（8-15bit）
$7c 0-1：周波数レジスタ加算値（16-17bit）
$7c 2-7：サンプル数（256-8*nサンプル）
$7d 0-7：周波数レジスタ代入値（16-24bit）
$7e 0-7：再生波形位置
$7f 0-3：音量
$7f 4-6：使用チャンネル数（$7Fのみ。ほかのchでは意味なし）			
*/
			case 0:
				chp->update |= 1;
				chp->freql = (uint8_t)value;
				break;
			case 1:
				chp->phase &= (0xffff01<<(PHASE_SHIFT-16))-1;
				chp->phase |= value<<(PHASE_SHIFT-16);
				chp->phase %= chp->tlen;
				break;
			case 2:
				chp->update |= 1;
				chp->freqm = (uint8_t)value;
				break;
			case 3:
				chp->phase &= (0xff01<<(PHASE_SHIFT-8))-1;
				chp->phase |= value<<(PHASE_SHIFT-8);
				chp->phase %= chp->tlen;
				break;
			case 4:
				chp->update |= 2;
				chp->freqh = (uint8_t)value;
				break;
			case 5:
				chp->phase = value << PHASE_SHIFT;
				chp->phase %= chp->tlen;
				break;
			case 6:
				chp->tadr = (uint8_t)(value & 0xff);
				break;
			case 7:
				chp->update |= 4;
				chp->vreg = (uint8_t)value;
				chp->nazo = (uint8_t)((value >> 4) & 0x07);
				if (chp == &n106s->ch[7]){
					n106s->chinuse = 1 + chp->nazo;
					n106s->cpsf = DivFix(NES_BASECYCLES, NES_BASECYCLES / n106s->chinuse / CPSF_SHIFT, CPS_SHIFT);
				}
				break;
		}
	}
	if (n106s->addressauto)
	{
		n106s->address = (n106s->address + 1) & 0x7f;
	}
}

static NES_WRITE_HANDLER s_n106_write_handler[] =
{
	{ N106SOUND_DATA, N106SOUND_DATA, N106SoundWriteData, NULL },
	{ N106SOUND_ADDR, N106SOUND_ADDR, N106SoundWriteAddr, NULL },
	{ 0,              0,              0, NULL },
};

static uint32_t N106SoundReadData(void *pNezPlay, uint32_t address)
{
    (void)address;
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	uint32_t ret = n106s->data[n106s->address];
	if (n106s->addressauto)
	{
		n106s->address = (n106s->address + 1) & 0x7f;
	}
	return ret;
}

static NES_READ_HANDLER s_n106_read_handler[] =
{
	{ N106SOUND_DATA, N106SOUND_DATA, N106SoundReadData, NULL },
	{ 0,              0,              0, NULL },
};

static void N106SoundReset(void* pNezPlay)
{
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	int i,j;
	XMEMSET(n106s, 0, sizeof(N106SOUND));

	//波形データの初期化
	for (j = 0; j < 0xff; j++)
		n106s->tone[j] =  LinearToLog((0) - 0x20);

	for (i = 0; i < 8; i++)
	{
		n106s->ch[i].tlen = 0x10 << PHASE_SHIFT;
		n106s->ch[i].logvol = LinearToLog(0);
	}
	n106s->addressauto = 1;
	n106s->chinuse = 8;
	n106s->cps = DivFix(NES_BASECYCLES, 45 * NESAudioFrequencyGet(pNezPlay), CPS_SHIFT);
	n106s->cpsf = DivFix(NES_BASECYCLES, NES_BASECYCLES / n106s->chinuse / CPSF_SHIFT, CPS_SHIFT);
	n106s->output = 0;
	n106s->outputfg = 0;

	n106s->ofscps = REAL_OFS_BASE * REAL_OFS_COUNT / NESAudioFrequencyGet(pNezPlay);
}

const static NES_RESET_HANDLER s_n106_reset_handler[] = {
	{ NES_RESET_SYS_NOMAL, N106SoundReset, NULL }, 
	{ 0,                   0, NULL }, 
};


static void N106SoundTerm(void* pNezPlay)
{
	N106SOUND *n106s = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->n106s;
	if (n106s)
		XFREE(n106s);
}

const static NES_TERMINATE_HANDLER s_n106_terminate_handler[] = {
	{ N106SoundTerm, NULL }, 
	{ 0, NULL }, 
};

//ここからレジスタビュアー設定
uint8_t *n106_regdata;
uint32_t (*ioview_ioread_DEV_N106)(uint32_t a);
static uint32_t ioview_ioread_bf(uint32_t a){
	if(a<=0x7f)return n106_regdata[a];else return 0x100;
}
//ここまでレジスタビュアー設定

void N106SoundInstall(NEZ_PLAY *pNezPlay)
{
	N106SOUND *n106s;
	n106s = XMALLOC(sizeof(N106SOUND));
	if (!n106s) return;
	XMEMSET(n106s, 0, sizeof(N106SOUND));
	((NSFNSF*)pNezPlay->nsf)->n106s = n106s;

	LogTableInitialize();
	NESAudioHandlerInstall(pNezPlay, s_n106_audio_handler);
	NESVolumeHandlerInstall(pNezPlay, s_n106_volume_handler);
	NESTerminateHandlerInstall(&pNezPlay->nth, s_n106_terminate_handler);
	NESReadHandlerInstall(pNezPlay, s_n106_read_handler);
	NESWriteHandlerInstall(pNezPlay, s_n106_write_handler);
	NESResetHandlerInstall(pNezPlay->nrh, s_n106_reset_handler);

	//ここからレジスタビュアー設定
	n106_regdata = n106s->data;
	ioview_ioread_DEV_N106 = ioview_ioread_bf;
	//ここまでレジスタビュアー設定

}
