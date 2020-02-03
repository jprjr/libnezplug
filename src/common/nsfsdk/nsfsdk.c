/*
	NSF SDK API - Win32
	Author:   Mamiya (mamiya@proc.org.tohoku.ac.jp)
	Language: ANSI-C
	License:  PDS
	Required DirectX3 or later vresion
*/

/* #define NSFSDKAPI __declspec(dllexport) */

#include "nsfsdk.h"
#include "nezplug.h"
#include "ui/nezplug/Dialog.h"

typedef struct NSFSDK_TAG
{
	unsigned volume;
	NEZ_PLAY *ctx;
}  NSFSDK_TAG;

HNSF NSFSDKAPI NSFSDK_Load(void *pData, unsigned uSize)
{
	NSFSDK_TAG *d = XMALLOC(sizeof(NSFSDK_TAG));
	if (d)
	{
		d->ctx = NEZNew();
		if (d->ctx)
		{
			d->volume = 0;
			if (!NEZLoad(d->ctx, pData, uSize)) return d;
		}
	}

	if (d)
	{
		if (d->ctx)
		  NEZDelete(d->ctx);
		XFREE(d);
	}
	return 0;
}

void NSFSDKAPI NSFSDK_SetSongNo(HNSF hnsf, unsigned uSongNo)
{
	if (hnsf == 0) return;
	NEZSetSongNo(hnsf->ctx, uSongNo);
}

void NSFSDKAPI NSFSDK_SetFrequency(HNSF hnsf, unsigned freq)
{
	if (hnsf == 0) return;
	NEZSetFrequency(hnsf->ctx, freq);
}

void NSFSDKAPI NSFSDK_SetNosefartFilter(HNSF hnsf, unsigned filter)
{
	if (hnsf == 0) return;
	NEZSetFilter(hnsf->ctx, filter);
}


void NSFSDKAPI NSFSDK_SetChannel(HNSF hnsf, unsigned ch)
{
	if (hnsf == 0) return;
	NEZSetChannel(hnsf->ctx, ch);
}

void NSFSDKAPI NSFSDK_Reset(HNSF hnsf)
{
	if (hnsf == 0) return;
	NEZReset(hnsf->ctx);
	NEZVolume(hnsf->ctx, hnsf->volume);
}

void NSFSDKAPI NSFSDK_Volume(HNSF hnsf, unsigned uVolume)
{
	if (hnsf == 0) return;
	hnsf->volume = uVolume;
	NEZVolume(hnsf->ctx, hnsf->volume);
}

void NSFSDKAPI NSFSDK_Render(HNSF hnsf, void *bufp, unsigned buflen)
{
	if (hnsf == 0) return;
	NEZRender(hnsf->ctx, bufp, buflen);
}

void NSFSDKAPI NSFSDK_Terminate(HNSF hnsf)
{
	if (hnsf == 0) return;
	NEZDelete(hnsf->ctx);
	XFREE(hnsf);
}

unsigned NSFSDKAPI NSFSDK_GetSongNo(HNSF hnsf)
{
	if (hnsf == 0) return 0;
	return NEZGetSongNo(hnsf->ctx);
}

unsigned NSFSDKAPI NSFSDK_GetSongStart(HNSF hnsf)
{
	if (hnsf == 0) return 0;
	return NEZGetSongStart(hnsf->ctx);
}

unsigned NSFSDKAPI NSFSDK_GetSongMax(HNSF hnsf)
{
	unsigned ret;
	if (hnsf == 0) return 0;
	ret = NEZGetSongMax(hnsf->ctx);
	if (!ret) return 256;
	return ret;
}

unsigned NSFSDKAPI NSFSDK_GetChannel(HNSF hnsf)
{
	if (hnsf == 0) return 1;
	return NEZGetChannel(hnsf->ctx);
}

unsigned NSFSDKAPI NSFSDK_GetFrequency(HNSF hnsf)
{
	if (hnsf == 0) return 1;
	return NEZGetFrequency(hnsf->ctx);
}

void NSFSDKAPI NSFSDK_GetFileInfo(char **p1, char **p2, char **p3, char **p4)
{
	NEZGetFileInfo(p1,p2,p3,p4);
}

void NSFSDKAPI NSFSDK_OpenFileInfoDlg(HINSTANCE p1, HWND p2)
{
	NEZFileInfoDlg(p1, p2);
}

void NSFSDKAPI NSFSDK_OpenMemViewDlg(HINSTANCE p1, HWND p2)
{
	NEZMemViewDlg(p1, p2);
}

void NSFSDKAPI NSFSDK_OpenChMaskDlg(HINSTANCE p1, HWND p2)
{
	NEZChMaskDlg(p1, p2);
}

void NSFSDKAPI NSFSDK_OpenIOViewDlg(HINSTANCE p1, HWND p2)
{
	NEZIOViewDlg(p1, p2);
}

void NSFSDKAPI NSFSDK_OpenDumpDlg(HINSTANCE p1, HWND p2)
{
	NEZDumpDlg(p1, p2);
}


static void GetSettingString(LPCSTR lpKey, LPCSTR lpDefault, LPSTR lpBuf, DWORD nSize, char *file)
{
	GetPrivateProfileString("NEZplug", lpKey, lpDefault, lpBuf, nSize, file);
}
static void SetSettingString(LPCSTR lpKey, LPCSTR lpStr, char *file)
{
	WritePrivateProfileString("NEZplug", lpKey, lpStr, file);
}

static int GetSettingInt(LPCSTR lpKey, int iDefault, char *file)
{
	char buf[128], dbuf[128];
	wsprintf(dbuf, "%d", iDefault);
	GetPrivateProfileString("NEZplug", lpKey, dbuf, buf, sizeof(buf), file);
	return atoi(buf);
}
static void SetSettingInt(LPCSTR lpKey, int iInt, char *file)
{
	char buf[128];
	wsprintf(buf, "%d", iInt);
	WritePrivateProfileString("NEZplug", lpKey, buf, file);
}

void NSFSDKAPI NSFSDK_LoadSetting(HNSF hnsf, char *file)
{
	if (hnsf == 0) return;
	if (file == 0) return;

	{
		extern int NSF_noise_random_reset;
		NSF_noise_random_reset  = GetSettingInt("NSFNoiseRandomReset", 0, file);
		SetSettingInt("NSFNoiseRandomReset", NSF_noise_random_reset, file);
	}
	{
		extern int Namco106_Realmode;
		Namco106_Realmode = GetSettingInt("Namco106RealMode", 1, file);
		SetSettingInt("Namco106RealMode", Namco106_Realmode, file);
	}
	{
		extern int Namco106_Volume;
		Namco106_Volume = GetSettingInt("Namco106Volume", 16, file);
		SetSettingInt("Namco106Volume", Namco106_Volume, file);
	}
	{
		extern int NSF_2A03Type;
		NSF_2A03Type  = GetSettingInt("2A03Type", 1, file);
		SetSettingInt("2A03Type", NSF_2A03Type, file);
	}
	{
		extern int FDS_RealMode;
		FDS_RealMode = GetSettingInt("FDSRealMode", 3, file);
		SetSettingInt("FDSRealMode", FDS_RealMode, file);
	}
	{
		extern int GBAMode;
		GBAMode = GetSettingInt("GBAMode", 0, file);
		SetSettingInt("GBAMode", GBAMode, file);
	}
	{
		extern int LowPassFilterLevel;
		LowPassFilterLevel = GetSettingInt("LowPassFilterLevel", 16, file);
		if (LowPassFilterLevel <  0) LowPassFilterLevel =  0;
		if (LowPassFilterLevel > 32) LowPassFilterLevel = 32;
		SetSettingInt("LowPassFilterLevel", LowPassFilterLevel, file);
	}
	{
		extern int NESAPUVolume;
		NESAPUVolume = GetSettingInt("NESAPUVolume", 64, file);
		if (NESAPUVolume < 0) NESAPUVolume =  0;
		if (NESAPUVolume > 255) NESAPUVolume = 255;
		SetSettingInt("NESAPUVolume", NESAPUVolume, file);
	}
	{
		extern int NESRealDAC;
		NESRealDAC = GetSettingInt("NESRealDAC", 1, file);
		SetSettingInt("NESRealDAC", NESRealDAC, file);
	}
	{
		extern int MSXPSGType;
		MSXPSGType = GetSettingInt("MSXPSGType", 1, file);
		SetSettingInt("MSXPSGType", MSXPSGType, file);
	}
	{
		extern int MSXPSGVolume;
		MSXPSGVolume = GetSettingInt("MSXPSGVolume", 64, file);
		if (MSXPSGVolume < 0) MSXPSGVolume =  0;
		if (MSXPSGVolume > 255) MSXPSGVolume = 255;
		SetSettingInt("MSXPSGVolume", MSXPSGVolume, file);
	}
	{
		extern char ColecoBIOSFilePath[0x200];
		GetSettingString("ColecoBIOSFilePath", "", ColecoBIOSFilePath, sizeof(ColecoBIOSFilePath), file);
		SetSettingString("ColecoBIOSFilePath", ColecoBIOSFilePath, file);
	}
	{
		int frequency;
		frequency = GetSettingInt("Frequency", 44100, file);
		if (frequency < 8000) frequency = 44100;
		if (frequency > 192000) frequency = 44100;
		SetSettingInt("Frequency", frequency, file);
		NEZSetFrequency(hnsf->ctx, frequency);
	}
/*	{
		int channel;
		channel = GetSettingInt("Channel", 2, file);
		if (channel < 1) channel = 1;
		if (channel > 2) channel = 2;
		SetSettingInt("Channel", channel, file);
		NEZSetChannel(hnsf->ctx, channel);
	}
*/	{
		int filtertype;
		filtertype = GetSettingInt("FilterType", 0, file);
		SetSettingInt("FilterType", filtertype, file);
		NEZSetFilter(hnsf->ctx, filtertype);
	}

}

