#include <nezplug/nezplug.h>
#include "handler.h"
#include "audiosys.h"
#include "nsf6502.h"
#include "songinfo.h"

#include "../device/nes/s_apu.h"

/* ------------ */
/*  km6502 I/F  */
/* ------------ */

#define USE_DIRECT_ZEROPAGE 0
#define USE_CALLBACK	1
#define USE_INLINEMMC	12
#define USE_USERPOINTER	1
#define External __inline static

#include "m_nsf.h"
#include "../cpu/km6502/km2a03m.h"

static void NES6502BreakPoint(NEZ_PLAY *pNezPlay, uint32_t A)
{
	((NSFNSF*)pNezPlay->nsf)->work6502_BP = A;
}

void NES6502Irq(NEZ_PLAY *pNezPlay)
{
	((NSFNSF*)pNezPlay->nsf)->work6502.iRequest |= K6502_INT;
}

/*
//DPCMのIRQ予約用。サンプリングレートの影響を受けずに、
//狂いなくDPCM-IRQを入れるために作成。 0で割り込み無し
void NES6502SetIrqCount(NEZ_PLAY *pNezPlay, int32_t A)
{
	((NSFNSF*)pNezPlay->nsf)->dpcmirq_ct = A;
}
*/

#if 0
static void NES6502Nmi(NEZ_PLAY *pNezPlay)
{
	((NSFNSF*)pNezPlay->nsf)->work6502.iRequest |= K6502_NMI;
}
#endif
uint32_t NES6502Read(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)pNezPlay->nsf)->work6502.ReadByte[A >> USE_INLINEMMC](pNezPlay, A);
}

uint32_t NES6502ReadDma(NEZ_PLAY *pNezPlay, uint32_t A)
{
	((NSFNSF*)pNezPlay->nsf)->work6502.clock++;	/* DMA cycle */
	if(((NSFNSF*)pNezPlay->nsf)->dpcmirq_ct >= 0){
		((NSFNSF*)pNezPlay->nsf)->dpcmirq_ct--;
	}
	return ((NSFNSF*)pNezPlay->nsf)->work6502.ReadByte[A >> USE_INLINEMMC](pNezPlay, A);
}

void NES6502Write(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)pNezPlay->nsf)->work6502.WriteByte[A >> USE_INLINEMMC](pNezPlay, A, V);
}

uint32_t NES6502GetCycles(NEZ_PLAY* pNezPlay)
{
	return ((NSFNSF*)pNezPlay->nsf)->work6502.clock + ((NSFNSF*)pNezPlay->nsf)->work6502_start_cycles;
}

static uint32_t NES6502Execute(NEZ_PLAY *pNezPlay, uint32_t start_cycles, uint32_t total_cycles)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	//int32_t clb = nsf->work6502.clock;
	nsf->work6502_start_cycles = start_cycles;

	while (nsf->work6502.clock < total_cycles)
	{
		//clb = nsf->work6502.clock;
		K6502_Exec(&nsf->work6502);
		/*
		//DPCM用 IRQカウンタ
		if(nsf->dpcmirq_ct>-65536){
			clb = nsf->work6502.clock - clb;
			nsf->dpcmirq_ct -= clb;
			clb = nsf->work6502.clock;
			if (nsf->dpcmirq_ct <= 0){
				nsf->dpcmirq_ct = -65536;
				nsf->work6502.iRequest |= K6502_INT;
				((APUSOUND*)nsf->apu)->dpcm.irq_report = 0x80;//すっげぇイレギュラーだが、こうするしかない 
				((APUSOUND*)nsf->apu)->dpcm.key = 0; 
				((APUSOUND*)nsf->apu)->dpcm.length = 0; 
			}
		}
		if (nsf->work6502.PC == nsf->work6502_BP)
		{
			if(nsf->dpcmirq_ct<=-65536 && !(nsf->work6502.iRequest & K6502_INT)){
				//nsf->work6502.clock = 0;
				//return 1;
			}
		}
		*/
		if (nsf->work6502.PC == nsf->work6502_BP)
		{
			nsf->work6502.clock = 0;
			return 1;
		}
	}
	nsf->work6502.clock -= total_cycles;
	/*
	if (nsf->work6502.PC == nsf->work6502_BP)
	{
		return 1;
	}
	*/
	return 0;
}

/* ----------------------- */
/*  Memory Access Handler  */
/* ----------------------- */

static void NES6502ReadHandlerSet(NEZ_PLAY *pNezPlay, uint32_t bank, READHANDLER rh)
{
	((NSFNSF*)pNezPlay->nsf)->work6502.ReadByte[bank] = rh;
}

static void NES6502WriteHandlerSet(NEZ_PLAY *pNezPlay, uint32_t bank, WRITEHANDLER wh)
{
	((NSFNSF*)pNezPlay->nsf)->work6502.WriteByte[bank] = wh;
}

#define EXTREADWRITE(p) \
static uint32_t ExtRd##p (NEZ_PLAY *pNezPlay, uint32_t A) \
{ \
	NES_READ_HANDLER *ph = ((NSFNSF*)pNezPlay->nsf)->nprh[0x##p ]; \
	do \
	{ \
		if (ph->min <= A && A <= ph->max) \
		{ \
			return ph->Proc(pNezPlay, A); \
		} \
	} while ((ph = ph->next) != 0); \
	return 0; \
} \
static void ExtWr##p (NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V) \
{ \
	NES_WRITE_HANDLER *ph = ((NSFNSF*)pNezPlay->nsf)->npwh[0x##p ]; \
	do \
	{ \
		if (ph->min <= A && A <= ph->max) \
		{ \
			ph->Proc(pNezPlay, A, V); \
			return; \
		} \
	} while ((ph = ph->next) != 0); \
}
EXTREADWRITE(0)
EXTREADWRITE(1)
EXTREADWRITE(2)
EXTREADWRITE(3)
EXTREADWRITE(4)
EXTREADWRITE(5)
EXTREADWRITE(6)
EXTREADWRITE(7)
EXTREADWRITE(8)
EXTREADWRITE(9)
EXTREADWRITE(A)
EXTREADWRITE(B)
EXTREADWRITE(C)
EXTREADWRITE(D)
EXTREADWRITE(E)
EXTREADWRITE(F)
static const READHANDLER ExtRdTbl[0x10] = {
	ExtRd0,ExtRd1,ExtRd2,ExtRd3,
	ExtRd4,ExtRd5,ExtRd6,ExtRd7,
	ExtRd8,ExtRd9,ExtRdA,ExtRdB,
	ExtRdC,ExtRdD,ExtRdE,ExtRdF,
};
static const WRITEHANDLER ExtWrTbl[0x10] = {
	ExtWr0,ExtWr1,ExtWr2,ExtWr3,
	ExtWr4,ExtWr5,ExtWr6,ExtWr7,
	ExtWr8,ExtWr9,ExtWrA,ExtWrB,
	ExtWrC,ExtWrD,ExtWrE,ExtWrF,
};
static uint32_t NullRead(NEZ_PLAY *pNezPlay, uint32_t A)
{
    (void)pNezPlay;
    (void)A;
	return 0;
}
static void NullWrite(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
    (void)pNezPlay;
    (void)A;
    (void)V;
}


//ここからメモリービュアー設定
uint32_t (*memview_memread)(uint32_t a);
NEZ_PLAY* memview_context;
int MEM_MAX,MEM_IO,MEM_RAM,MEM_ROM;
uint32_t memview_memread_nes(uint32_t a){
	if(((NSFNSF*)memview_context->nsf)->nprh[(a>>12) & 0xF]!=NULL)
		return ExtRdTbl[(a>>12) & 0xF](memview_context,a);
	else return 0xFF;
}
//ここまでメモリービュアー設定



static void NES6502Reset(NEZ_PLAY *pNezPlay)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	nsf->work6502.clock = 0;
	nsf->work6502.iRequest = K6502_INIT;
	nsf->work6502.PC = nsf->work6502_BP = 0xFFFF;
	NES6502Execute(pNezPlay, 0, nsf->work6502.clock + 1);

	//ここからメモリービュアー設定
	memview_context = pNezPlay;
	MEM_MAX=0xffff;
	MEM_IO =0x4000;
	MEM_RAM=0x0000;
	MEM_ROM=0x8000;
	memview_memread = memview_memread_nes;
	//ここまでメモリービュアー設定

}


static void InstallPageReadHandler(NEZ_PLAY *pNezPlay, NES_READ_HANDLER *ph)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	uint32_t page = (ph->min >> 12) & 0xF;
	if (nsf->nprh[page])
		NES6502ReadHandlerSet(pNezPlay, page, ExtRdTbl[page]);
	else
		NES6502ReadHandlerSet(pNezPlay, page, ph->Proc);
	/* Add to head of list*/
	ph->next = nsf->nprh[page];
	nsf->nprh[page] = ph;
}
static void InstallPageWriteHandler(NEZ_PLAY *pNezPlay, NES_WRITE_HANDLER *ph)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	uint32_t page = (ph->min >> 12) & 0xF;
	if (nsf->npwh[page])
		NES6502WriteHandlerSet(pNezPlay, page, ExtWrTbl[page]);
	else
		NES6502WriteHandlerSet(pNezPlay, page, ph->Proc);
	/* Add to head of list*/
	ph->next = nsf->npwh[page];
	nsf->npwh[page] = ph;
}
void NESReadHandlerInstall(NEZ_PLAY *pNezPlay, NES_READ_HANDLER *ph)
{
	for (; ph->Proc; ph++) InstallPageReadHandler(pNezPlay, ph);
}

void NESWriteHandlerInstall(NEZ_PLAY *pNezPlay, NES_WRITE_HANDLER *ph)
{
	for (; ph->Proc; ph++) InstallPageWriteHandler(pNezPlay, ph);
}

void NESMemoryHandlerInitialize(NEZ_PLAY *pNezPlay)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	uint32_t i;
	for (i = 0; i < 0x10;  i++)
	{
		NES6502ReadHandlerSet(pNezPlay, i, NullRead);
		NES6502WriteHandlerSet(pNezPlay, i, NullWrite);
		nsf->nprh[i] = 0;
		nsf->npwh[i] = 0;
	}
}

/* ------------- */
/*  nsf6502 I/F  */
/* ------------- */

#define SHIFT_CPS 24
#define NES_BASECYCLES (21477270)

static void NSFRomInit(NEZ_PLAY *pNezPlay, uint32_t A)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	nsf->nsf6502.rom[0] = 0x20;	/* jsr */
	nsf->nsf6502.rom[1] = (A & 0xff);		/* init */
	nsf->nsf6502.rom[2] = ((A >> 8) & 0xff);
	nsf->nsf6502.rom[3] = 0x4C;	/* JMP nnnn */
	nsf->nsf6502.rom[4] = 0x03;
	nsf->nsf6502.rom[5] = 0x41;
}

static uint32_t GetWordLE(uint8_t *p)
{
	return p[0] | (p[1] << 8);
}

static void NSF6502PlaySetup(NEZ_PLAY *pNezPlay)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	if (nsf->nsf6502.breaked)
	{
		nsf->nsf6502.breaked = 0;
		NES6502BreakPoint(pNezPlay, 0x4103);
		NSFRomInit(pNezPlay, SONGINFO_GetPlayAddress(pNezPlay->song));	/* PLAY */
		nsf->work6502.PC = 0x4100;
		nsf->work6502.A = 0x00;
		nsf->work6502.S = 0xFF;
		nsf->work6502.P = 0x26							/* IRZ */;
	}
}

static int32_t Execute6502(NEZ_PLAY *pNezPlay)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	uint32_t cycles;
	nsf->nsf6502.cleft += nsf->nsf6502.cps;
	cycles = nsf->nsf6502.cleft >> SHIFT_CPS;
	if (/*nsf->dpcmirq_ct>-65536 || */!nsf->nsf6502.breaked)
	{
		nsf->nsf6502.breaked = NES6502Execute(pNezPlay, nsf->nsf6502.total_cycles, cycles);
	}else
	if (nsf->work6502.iRequest & IRQ_INT || nsf->work6502.PC != 0x4103)
	{
		NES6502Execute(pNezPlay, nsf->nsf6502.total_cycles, cycles);
	}
	nsf->nsf6502.cleft &= (1 << SHIFT_CPS) - 1;
	nsf->nsf6502.cycles += cycles * 12;
	if (nsf->nsf6502.cycles > nsf->nsf6502.cpf[nsf->nsf6502.iframe])
	{
		nsf->vsyncirq_fg = 0x40;
		nsf->nsf6502.cycles -= nsf->nsf6502.cpf[nsf->nsf6502.iframe];
		if (nsf->nsf6502.breaked)
		{
			nsf->nsf6502.iframe ^= 1;
			NSF6502PlaySetup(pNezPlay);
		}
	}
	/*
	if (nsf->nsf6502.cycles >= nsf->nsf6502.cpf[nsf->nsf6502.iframe] * 2)
	{
		nsf->nsf6502.cycles = nsf->nsf6502.cpf[nsf->nsf6502.iframe] * 2;
	}
	*/
	nsf->nsf6502.total_cycles += cycles;
	return 0;
}

const static NEZ_NES_AUDIO_HANDLER nsf6502_audio_handler[] = {
	{ 0, Execute6502, NULL, NULL },
	{ 0, 0, NULL, NULL },
};

#ifdef _WIN32
extern int __stdcall MulDiv(int nNumber,int nNumerator,int nDenominator);
static uint32_t __forceinline muldiv(uint32_t m, uint32_t n, uint32_t d)
{
	return MulDiv(m,n,d);
}
#else
static uint32_t muldiv(uint32_t m, uint32_t n, uint32_t d)
{
	return ((double)m) * n / d;
}
#endif

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

static void NSF6502Reset(NEZ_PLAY *pNezPlay)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	uint8_t *nsfhead = NSFGetHeader(pNezPlay);
	uint32_t freq = NESAudioFrequencyGet(pNezPlay);
	uint32_t speed = 0;

	nsf->nsf6502.palntsc = nsfhead[0x7a] & 1;
	if (!nsf->nsf6502.palntsc && GetWordLE(nsfhead + 0x6E))
		speed = GetWordLE(nsfhead + 0x6E);	/* NTSC tune */
	else if (nsf->nsf6502.palntsc && GetWordLE(nsfhead + 0x78))
		speed = GetWordLE(nsfhead + 0x78);	/* PAL  tune */

	if (speed == 0)
		speed = nsf->nsf6502.palntsc ? 0x4e20 : 0x411A;

	nsf->nsf6502.cleft = 0;
	nsf->nsf6502.cps = DivFix(NES_BASECYCLES, 12 * freq, SHIFT_CPS);

	nsf->nsf6502.cycles = 0;
	if (nsf->nsf6502.palntsc)
	{
		nsf->nsf6502.cpf[0] = muldiv(speed, 4 * 341 * 313    , 0x4e20);
		nsf->nsf6502.cpf[1] = muldiv(speed, 4 * 341 * 313 - 4, 0x4e20);
	}
	else
	{
		nsf->nsf6502.cpf[0] = muldiv(speed, 4 * 341 * 262    , 0x411a);
		nsf->nsf6502.cpf[1] = muldiv(speed, 4 * 341 * 262 - 4, 0x411a);
	}
	nsf->nsf6502.iframe = 0;

	NES6502Reset(pNezPlay);
	NES6502BreakPoint(pNezPlay, 0x4103);
	NSFRomInit(pNezPlay, SONGINFO_GetInitAddress(pNezPlay->song));
	nsf->work6502.PC = 0x4100;
	nsf->work6502.A = SONGINFO_GetSongNo(pNezPlay->song) - 1;
	nsf->work6502.X = nsf->nsf6502.palntsc;
	nsf->work6502.Y = 0;
	nsf->work6502.S = 0xFF;
	nsf->work6502.P = 0x26;							/* IRZ */
	nsf->nsf6502.total_cycles = 0;
	//nsf->dpcmirq_ct = -65536;
	nsf->work6502.user = pNezPlay;
	nsf->vsyncirq_fg = 0x40;

#define LIMIT_INIT (2 * 60)	/* 2sec */
#if LIMIT_INIT
	{
		uint32_t sec;
		for (sec = 0; sec < LIMIT_INIT; sec++)
		{
			nsf->nsf6502.breaked = NES6502Execute(pNezPlay, nsf->nsf6502.total_cycles, nsf->nsf6502.cpf[0]);
			if (nsf->nsf6502.breaked) break;
		}
	}
#else
	while (1)
	{
		nsf->nsf6502.breaked = NES6502Execute(pNezPlay, nsf->nsf6502.total_cycles, ~0);
		if (nsf->nsf6502.breaked) break;
	}
#endif

	NSF6502PlaySetup(pNezPlay);
}

const static NEZ_NES_RESET_HANDLER nsf6502_reset_handler[] = {
	{ NES_RESET_SYS_LAST, NSF6502Reset, NULL },
	{ 0,                  0, NULL },
};


/* Nosefart-ROM area */
static uint32_t ReadNosefartRom(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)pNezPlay->nsf)->nsf6502.rom[A & 0x000F];
}


static NES_READ_HANDLER nsf6502_read_handler[] = {
	{ 0x4100,0x410F,ReadNosefartRom, NULL },
	{ 0     ,0     ,0, NULL },
};

uint32_t NSF6502Install(NEZ_PLAY *pNezPlay)
{
	NESReadHandlerInstall(pNezPlay, nsf6502_read_handler);
	NESAudioHandlerInstall(pNezPlay, nsf6502_audio_handler);
	NESResetHandlerInstall(pNezPlay->nrh, nsf6502_reset_handler);
	return NEZ_NESERR_NOERROR;
}

