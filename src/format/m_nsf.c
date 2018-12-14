/* NSF mapper(4KBx8) */

#include <nezplug/nezplug.h>
#include "handler.h"
#include "audiosys.h"
#include "songinfo.h"
#include "nsf6502.h"
#include "../device/nes/s_apu.h"
#include "../device/nes/s_mmc5.h"
#include "../device/nes/s_vrc6.h"
#include "../device/nes/s_vrc7.h"
#include "../device/nes/s_fds.h"
#include "../device/nes/s_n106.h"
#include "../device/nes/s_fme7.h"
#include <stdio.h>

#define NSF_MAPPER_STATIC 0
#include "m_nsf.h"

#define EXTSOUND_VRC6	(1 << 0)
#define EXTSOUND_VRC7	(1 << 1)
#define EXTSOUND_FDS	(1 << 2)
#define EXTSOUND_MMC5	(1 << 3)
#define EXTSOUND_N106	(1 << 4)
#define EXTSOUND_FME7	(1 << 5)
#define EXTSOUND_J86	(1 << 6)	/* JALECO-86 */

struct {
	char* title;
	char* artist;
	char* copyright;
	char detail[1024];
}songinfodata;

/* RAM area */
static uint32_t ReadRam(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->ram[A & 0x07FF];
}
static void WriteRam(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->ram[A & 0x07FF] = (uint8_t)V;
}

/* SRAM area */
static uint32_t ReadStaticArea(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->static_area[A - 0x6000];
}
static void WriteStaticArea(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->static_area[A - 0x6000] = (uint8_t)V;
}

#if !NSF_MAPPER_STATIC
static uint32_t ReadRom8000(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[0][A];
}
static uint32_t ReadRom9000(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[1][A];
}
static uint32_t ReadRomA000(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[2][A];
}
static uint32_t ReadRomB000(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[3][A];
}
static uint32_t ReadRomC000(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[4][A];
}
static uint32_t ReadRomD000(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[5][A];
}
static uint32_t ReadRomE000(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[6][A];
}
static uint32_t ReadRomF000(NEZ_PLAY *pNezPlay, uint32_t A)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[7][A];
}

// ROMに値を書き込むのはできないはずで不正なのだが、
// これをしないとFDS環境のNSFが動作しないのでしょうがない…

static void WriteRom8000(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[0][A] = (uint8_t)V;
}
static void WriteRom9000(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[1][A] = (uint8_t)V;
}
static void WriteRomA000(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[2][A] = (uint8_t)V;
}
static void WriteRomB000(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[3][A] = (uint8_t)V;
}
static void WriteRomC000(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[4][A] = (uint8_t)V;
}
static void WriteRomD000(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[5][A] = (uint8_t)V;
}
static void WriteRomE000(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[6][A] = (uint8_t)V;
}
static void WriteRomF000(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->bank[7][A] = (uint8_t)V;
}
#endif

/* Mapper I/O */
static void WriteMapper(NEZ_PLAY *pNezPlay, uint32_t A, uint32_t V)
{
	uint32_t bank;
	NSFNSF *nsf = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf);
	bank = A - 0x5FF0;
	if (bank < 6 || bank > 15) return;
#if !NSF_MAPPER_STATIC
	if (bank >= 8)
	{
		if (V < nsf->banknum)
		{
			nsf->bank[bank - 8] = &nsf->bankbase[V << 12] - (bank << 12);
		}
		else
			nsf->bank[bank - 8] = nsf->zero_area - (bank << 12);
		return;
	}
#endif
	if (V < nsf->banknum)
	{
		XMEMCPY(&nsf->static_area[(bank - 6) << 12], &nsf->bankbase[V << 12], 0x1000);
	}
	else
		XMEMSET(&nsf->static_area[(bank - 6) << 12], 0x00, 0x1000);
}

static uint32_t Read2000(NEZ_PLAY *pNezPlay, uint32_t A)
{
	NSFNSF *nsf = ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf);
	switch(A & 0x7){
	case 0:
		return 0xff;
	case 1:
		return 0xff;
	case 2:
		return nsf->counter2002++ ? 0x7f : 0xff;
	case 3:
		return 0xff;
	case 4:
		return 0xff;
	case 5:
		return 0xff;
	case 6:
		return 0xff;
	case 7:
		return 0xff;
	}
	return 0xff;
}


static NES_READ_HANDLER nsf_mapper_read_handler[] = {
	{ 0x0000,0x0FFF,ReadRam, NULL },
	{ 0x1000,0x1FFF,ReadRam, NULL },
	{ 0x2000,0x2007,Read2000, NULL },
	{ 0x6000,0x6FFF,ReadStaticArea, NULL },
	{ 0x7000,0x7FFF,ReadStaticArea, NULL },
#if !NSF_MAPPER_STATIC
	{ 0x8000,0x8FFF,ReadRom8000, NULL },
	{ 0x9000,0x9FFF,ReadRom9000, NULL },
	{ 0xA000,0xAFFF,ReadRomA000, NULL },
	{ 0xB000,0xBFFF,ReadRomB000, NULL },
	{ 0xC000,0xCFFF,ReadRomC000, NULL },
	{ 0xD000,0xDFFF,ReadRomD000, NULL },
	{ 0xE000,0xEFFF,ReadRomE000, NULL },
	{ 0xF000,0xFFFF,ReadRomF000, NULL },
#else
	{ 0x8000,0x8FFF,ReadStaticArea, NULL },
	{ 0x9000,0x9FFF,ReadStaticArea, NULL },
	{ 0xA000,0xAFFF,ReadStaticArea, NULL },
	{ 0xB000,0xBFFF,ReadStaticArea, NULL },
	{ 0xC000,0xCFFF,ReadStaticArea, NULL },
	{ 0xD000,0xDFFF,ReadStaticArea, NULL },
	{ 0xE000,0xEFFF,ReadStaticArea, NULL },
	{ 0xF000,0xFFFF,ReadStaticArea, NULL },
#endif
	{ 0     ,0     ,0, NULL },
};
static NES_WRITE_HANDLER nsf_mapper_write_handler[] = {
	{ 0x0000,0x0FFF,WriteRam, NULL },
	{ 0x1000,0x1FFF,WriteRam, NULL },
	{ 0x6000,0x6FFF,WriteStaticArea, NULL },
	{ 0x7000,0x7FFF,WriteStaticArea, NULL },
	{ 0     ,0     ,0, NULL },
};
static NES_WRITE_HANDLER nsf_mapper_write_handler_fds[] = {
	{ 0x0000,0x0FFF,WriteRam, NULL },
	{ 0x1000,0x1FFF,WriteRam, NULL },
	{ 0x6000,0x6FFF,WriteStaticArea, NULL },
	{ 0x7000,0x7FFF,WriteStaticArea, NULL },
	{ 0x8000,0x8FFF,WriteRom8000, NULL },
	{ 0x9000,0x9FFF,WriteRom9000, NULL },
	{ 0xA000,0xAFFF,WriteRomA000, NULL },
	{ 0xB000,0xBFFF,WriteRomB000, NULL },
	{ 0xC000,0xCFFF,WriteRomC000, NULL },
	{ 0xD000,0xDFFF,WriteRomD000, NULL },
	{ 0xE000,0xEFFF,WriteRomE000, NULL },
	{ 0xF000,0xFFFF,WriteRomF000, NULL },
	{ 0     ,0     ,0, NULL },
};
static NES_WRITE_HANDLER nsf_mapper_write_handler2[] = {
	{ 0x5FF6,0x5FFF,WriteMapper, NULL },
	{ 0     ,0     ,0, NULL },
};

uint8_t *NSFGetHeader(NEZ_PLAY *pNezPlay)
{
	return ((NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf)->head;
}

static uint32_t GetWordLE(uint8_t *p)
{
	return p[0] | (p[1] << 8);
}

static void ResetBank(NEZ_PLAY *pNezPlay)
{
	uint32_t i, startbank;
	NSFNSF *nsf = (NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf;
	XMEMSET(nsf->ram, 0, 0x0800);
	startbank = GetWordLE(nsf->head + 8) >> 12;
	for (i = 0; i < 16; i++)
		WriteMapper(pNezPlay, 0x5FF0 + i, 0x10000/* off */);
	if (nsf->banksw)
	{
		for (i = 0; (startbank + i) < 8; i++)
			WriteMapper(pNezPlay, 0x5FF0 + (startbank + i) , i);
		if (nsf->banksw & 2)
		{
			WriteMapper(pNezPlay, 0x5FF6, nsf->head[0x70 + 6]);
			WriteMapper(pNezPlay, 0x5FF7, nsf->head[0x70 + 7]);
		}
		for (i = 8; i < 16; i++)
			WriteMapper(pNezPlay, 0x5FF0 + i, nsf->head[0x70 + i - 8]);
	}
	else
	{
		for (i = startbank; i < 16; i++)
			WriteMapper(pNezPlay, 0x5FF0 + i , i - startbank);
	}
}

const static NEZ_NES_RESET_HANDLER nsf_mapper_reset_handler[] = {
	{ NES_RESET_SYS_FIRST, ResetBank, NULL },
	{ 0,                   0, NULL },
};

static void Terminate(NEZ_PLAY *pNezPlay)
{
	NSFNSF *nsf = (NSFNSF*)((NEZ_PLAY*)pNezPlay)->nsf;
	if (nsf)
	{
		if (nsf->bankbase)
		{
			XFREE(nsf->bankbase);
			nsf->bankbase = 0;
		}
		XFREE(nsf);
		((NEZ_PLAY*)pNezPlay)->nsf = 0;
	}
}

const static NEZ_NES_TERMINATE_HANDLER nsf_mapper_terminate_handler[] = {
	{ Terminate, NULL },
    { 0, NULL },
};

static uint32_t NSFMapperInitialize(NEZ_PLAY *pNezPlay, uint8_t *pData, uint32_t uSize)
{
	uint32_t size = uSize;
	uint8_t *data = pData;
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;

	size += GetWordLE(nsf->head + 8) & 0x0FFF;
	size  = (size + 0x0FFF) & ~0x0FFF;

	nsf->bankbase = XMALLOC(size + 8);
	if (!nsf->bankbase) return NEZ_NESERR_SHORTOFMEMORY;

	nsf->banknum = size >> 12;
	XMEMSET(nsf->bankbase, 0, size);
	XMEMCPY(nsf->bankbase + (GetWordLE(nsf->head + 8) & 0x0FFF), data, uSize);

	nsf->banksw = nsf->head[0x70] || nsf->head[0x71] || nsf->head[0x72] || nsf->head[0x73] || nsf->head[0x74] || nsf->head[0x75] || nsf->head[0x76] || nsf->head[0x77];
	if (SONGINFO_GetExtendDevice(pNezPlay->song) & EXTSOUND_FDS) nsf->banksw <<= 1;
	return NEZ_NESERR_NOERROR;
}

uint32_t NSFDeviceInitialize(NEZ_PLAY *pNezPlay)
{
	NSFNSF *nsf = (NSFNSF*)pNezPlay->nsf;
	NESResetHandlerInstall(pNezPlay->nrh, nsf_mapper_reset_handler);
	NESTerminateHandlerInstall(&pNezPlay->nth, nsf_mapper_terminate_handler);
	NESReadHandlerInstall(pNezPlay, nsf_mapper_read_handler);
	//FDSのみをサポートしている場合のみ8000-DFFFも書き込み可能-
	if(pNezPlay->song->extdevice == 4){
		NESWriteHandlerInstall(pNezPlay, nsf_mapper_write_handler_fds);
	}else{
		NESWriteHandlerInstall(pNezPlay, nsf_mapper_write_handler);
	}

	if (nsf->banksw) NESWriteHandlerInstall(pNezPlay, nsf_mapper_write_handler2);

	XMEMSET(nsf->ram, 0, 0x0800);
	XMEMSET(nsf->static_area, 0, sizeof(nsf->static_area));
#if !NSF_MAPPER_STATIC
	XMEMSET(nsf->zero_area, 0, sizeof(nsf->zero_area));
#endif

	APUSoundInstall(pNezPlay);
	if (SONGINFO_GetExtendDevice(pNezPlay->song) & EXTSOUND_VRC6) VRC6SoundInstall(pNezPlay);
	if (SONGINFO_GetExtendDevice(pNezPlay->song) & EXTSOUND_VRC7) VRC7SoundInstall(pNezPlay);
	if (SONGINFO_GetExtendDevice(pNezPlay->song) & EXTSOUND_FDS)  FDSSoundInstall(pNezPlay);
	if (SONGINFO_GetExtendDevice(pNezPlay->song) & EXTSOUND_MMC5)
	{
		MMC5SoundInstall(pNezPlay);
		MMC5MutiplierInstall(pNezPlay);
		MMC5ExtendRamInstall(pNezPlay);
	}
	if (SONGINFO_GetExtendDevice(pNezPlay->song) & EXTSOUND_N106) N106SoundInstall(pNezPlay);
	if (SONGINFO_GetExtendDevice(pNezPlay->song) & EXTSOUND_FME7) FME7SoundInstall(pNezPlay);

	nsf->counter2002 = 0;	//暫定

	return NEZ_NESERR_NOERROR;
}


static void NSFMapperSetInfo(NEZ_PLAY *pNezPlay, uint8_t *pData)
{
    uint8_t titlebuffer[0x21];
    uint8_t artistbuffer[0x21];
    uint8_t copyrightbuffer[0x21];

	XMEMCPY(((NSFNSF*)pNezPlay->nsf)->head, pData, 0x80);
	SONGINFO_SetStartSongNo(pNezPlay->song, pData[0x07]);
	SONGINFO_SetMaxSongNo(pNezPlay->song, pData[0x06]);
	SONGINFO_SetExtendDevice(pNezPlay->song, pData[0x7B]);
	SONGINFO_SetInitAddress(pNezPlay->song, GetWordLE(pData + 0x0A));
	SONGINFO_SetPlayAddress(pNezPlay->song, GetWordLE(pData + 0x0C));
	SONGINFO_SetChannel(pNezPlay->song, 1);

	XMEMSET(titlebuffer, 0, 0x21);
	XMEMCPY(titlebuffer, pData + 0x000e, 0x20);
	songinfodata.title=(char *)titlebuffer;

	XMEMSET(artistbuffer, 0, 0x21);
	XMEMCPY(artistbuffer, pData + 0x002e, 0x20);
	songinfodata.artist=(char *)artistbuffer;

	XMEMSET(copyrightbuffer, 0, 0x21);
	XMEMCPY(copyrightbuffer, pData + 0x004e, 0x20);
	songinfodata.copyright=(char *)copyrightbuffer;

	sprintf(songinfodata.detail,
"Type          : NSF\r\n\
Song Max      : %d\r\n\
Start Song    : %d\r\n\
Load Address  : %04XH\r\n\
Init Address  : %04XH\r\n\
Play Address  : %04XH\r\n\
NTSC/PAL Mode : %s\r\n\
NTSC Speed    : %04XH (%4.0fHz)\r\n\
PAL Speed     : %04XH (%4.0fHz)\r\n\
Extend Device : %s%s%s%s%s%s%s\r\n\
\r\n\
Set First ROM Bank                    : %d\r\n\
First ROM Bank(8000-8FFF)             : %02XH\r\n\
First ROM Bank(9000-9FFF)             : %02XH\r\n\
First ROM Bank(A000-AFFF)             : %02XH\r\n\
First ROM Bank(B000-BFFF)             : %02XH\r\n\
First ROM Bank(C000-CFFF)             : %02XH\r\n\
First ROM Bank(D000-DFFF)             : %02XH\r\n\
First ROM Bank(E000-EFFF or 6000-6FFF): %02XH\r\n\
First ROM Bank(F000-FFFF or 7000-7FFF): %02XH"
		,pData[0x06],pData[0x07],GetWordLE(pData + 0x08),GetWordLE(pData + 0x0A),GetWordLE(pData + 0x0C)
		,pData[0x7A]&0x02 ? "NTSC + PAL" : pData[0x7A]&0x01 ? "PAL" : "NTSC"
		,GetWordLE(pData + 0x6E),GetWordLE(pData + 0x6E) ? 1000000.0/GetWordLE(pData + 0x6E) : 0
		,GetWordLE(pData + 0x78),GetWordLE(pData + 0x78) ? 1000000.0/GetWordLE(pData + 0x78) : 0
		,pData[0x7B]&0x01 ? "VRC6 " : ""
		,pData[0x7B]&0x02 ? "VRC7 " : ""
		,pData[0x7B]&0x04 ? "2C33 " : ""
		,pData[0x7B]&0x08 ? "MMC5 " : ""
		,pData[0x7B]&0x10 ? "Namco1xx " : ""
		,pData[0x7B]&0x20 ? "Sunsoft5B " : ""
		,pData[0x7B] ? "" : "None"
		,pData[0x70]|pData[0x71]|pData[0x72]|pData[0x73]|pData[0x74]|pData[0x75]|pData[0x76]|pData[0x77] ? 1 : 0
		,pData[0x70],pData[0x71],pData[0x72],pData[0x73],pData[0x74],pData[0x75],pData[0x76],pData[0x77]
	);
}

uint32_t NSFLoad(NEZ_PLAY *pNezPlay, uint8_t *pData, uint32_t uSize)
{
	uint32_t ret;
	NSFNSF *THIS_ = (NSFNSF *)XMALLOC(sizeof(NSFNSF));
	if (!THIS_) return NEZ_NESERR_SHORTOFMEMORY;
	XMEMSET(THIS_, 0, sizeof(NSFNSF));
	THIS_->fds_type = 2;
	pNezPlay->nsf = THIS_;
	NESMemoryHandlerInitialize(pNezPlay);
	NSFMapperSetInfo(pNezPlay, pData);
	ret = NSF6502Install(pNezPlay);
	if (ret) return ret;
	ret = NSFMapperInitialize(pNezPlay, pData + 0x80, uSize - 0x80);
	if (ret) return ret;
	ret = NSFDeviceInitialize(pNezPlay);
	if (ret) return ret;
	SONGINFO_SetSongNo(pNezPlay->song, SONGINFO_GetStartSongNo(pNezPlay->song));
	return NEZ_NESERR_NOERROR;
}

