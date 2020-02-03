#include <stdlib.h>
#include "common/win32/win32l.h"
#undef NOUSER
#include <windows.h>
#include "../../nezplug.h"
#include "kmp_com.h"
#include "../version.h"

struct KMP_CTX_TAG
{
	NEZ_PLAY* hnsf;
	unsigned srate;
	unsigned nch;
	unsigned bits;
	unsigned filter;
//	unsigned fdstype;
};

static const char *sectionname = "NEZplug";
static char iniFilePath[MAX_PATH] = "";

static UINT GetPrivateProfileIntEx(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName)
{
	UINT ret;
	TCHAR szBuffer[32];
	ret = GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, lpFileName);
	wsprintf(szBuffer, "%d", ret);
	WritePrivateProfileString(lpAppName, lpKeyName, szBuffer, lpFileName);
	return ret;
}

static UINT GetPrivateProfileIntEx2(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName, UINT nMin, UINT nMax)
{
	UINT ret;
	TCHAR szBuffer[32];
	ret = GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, lpFileName);
	if(ret < nMin) ret = nMin;
	if(ret > nMax) ret = nMax;
	wsprintf(szBuffer, "%d", ret);
	WritePrivateProfileString(lpAppName, lpKeyName, szBuffer, lpFileName);
	return ret;
}

static BOOL GetPrivateProfileBoolEx(LPCTSTR lpAppName, LPCTSTR lpKeyName, BOOL bDefault, LPCTSTR lpFileName)
{
	BOOL ret;
	TCHAR szBuffer[32];
	if (GetPrivateProfileString(lpAppName, lpKeyName, bDefault ? "True" : "False", szBuffer, 32, lpFileName))
	{
		/* 先頭だけ確認 */
		ret = szBuffer[0] == 'T' || szBuffer[0] == 't';
	}
	else
		ret = bDefault;
	WritePrivateProfileString(lpAppName, lpKeyName, ret ? "True" : "False", lpFileName);
	return ret;
}

static int RegGetString(const char *lpszSubKey, const char *lpszName, char *lpszBuffer, unsigned uBufferSize)
{
	int ret = 0;
	HKEY hkey;
	DWORD dispo, dwt, dws;
	if (ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, lpszSubKey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispo)) return ret;
	dws = uBufferSize;
	if (ERROR_SUCCESS == RegQueryValueEx(hkey, lpszName,NULL,&dwt,(LPBYTE)lpszBuffer, &dws) && dwt == REG_SZ)
	{
		ret = dws;
	}
	RegCloseKey(hkey);
	return ret;
}

static void Setup(KMP_CTX *pctx)
{
	pctx->hnsf = 0;
	pctx->bits = 16;
	if (pctx->nch != 1) pctx->nch = 2;
	if (pctx->srate == 0) pctx->srate = GetPrivateProfileIntEx(sectionname, "SampleRate", 44100, iniFilePath);

	pctx->filter = GetPrivateProfileIntEx(sectionname, "FilterType", 0, iniFilePath);
//	pctx->fdstype = GetPrivateProfileIntEx(sectionname, "NSFFdsType", 2, iniFilePath);
}

void Close(KMP_CTX *pctx)
{
	if (pctx->hnsf)
	{
		NEZDelete(pctx->hnsf);
	}
	XFREE(pctx);
}

unsigned NEZ_extract(char *lpszSrcFile, void **ppbuf);
//void FDSSelect(NEZ_PLAY *ctx, unsigned type);

KMP_CTX *Open(const char *file, unsigned size, unsigned srate, unsigned nch)
{
	KMP_CTX *pctx = 0;
	do
	{
		pctx = XMALLOC(sizeof(KMP_CTX));
		if (!pctx) break;

		pctx->srate = srate;
		pctx->nch = nch;
		Setup(pctx);

		pctx->hnsf = NEZNew();
		if (!pctx->hnsf) break;
		if (!size)
		{
			unsigned char *buf;
			unsigned int buf_size;
			buf_size = NEZ_extract((char *)file, &buf);
			NEZLoad(pctx->hnsf, (unsigned char*)buf, buf_size);
			XFREE(buf);
		}
		else
		{
			NEZLoad(pctx->hnsf, (unsigned char*)file, size);
		}
//		FDSSelect(pctx->hnsf, pctx->fdstype);
		NEZSetFrequency(pctx->hnsf, pctx->srate);
		NEZSetChannel(pctx->hnsf, pctx->nch);
		NEZSetFilter(pctx->hnsf, pctx->filter);
		NEZReset(pctx->hnsf);
		return pctx;
	} while (0);
	if (pctx) Close(pctx);
	return 0;
}

unsigned Write(KMP_CTX *pctx, void *buf, unsigned smp)
{
	NEZRender(pctx->hnsf, buf, smp >> pctx->nch);
	return smp;
}

unsigned WriteSkip(KMP_CTX *pctx, unsigned smp)
{
	NEZRender(pctx->hnsf, 0, smp >> pctx->nch);
	return smp;
}

unsigned SetPosition(KMP_CTX *pctx, unsigned ms)
{
	if (ms) NEZSetSongNo(pctx->hnsf, ms / 1000);
	NEZReset(pctx->hnsf);
	return 0;
}

int GetLoopFlag(KMP_CTX *pctx)
{
	return 1;
}

int GetSeekableFlag(KMP_CTX *pctx)
{
	return 1;
}

int GetMultisongFlag(KMP_CTX *pctx)
{
	return 1;
}

unsigned GetFrequency(KMP_CTX *pctx)
{
	return pctx->srate;
}
unsigned GetChannels(KMP_CTX *pctx)
{
	return pctx->nch;
}
unsigned GetSamplebits(KMP_CTX *pctx)
{
	return pctx->bits;
}
unsigned GetLength(KMP_CTX *pctx)
{
	return NEZGetSongMax(pctx->hnsf) * 1000;
}
unsigned GetUnitSamples(KMP_CTX *pctx)
{
	return 44100 * 60 / 1000;	/* 60msec */
}

void Init(void)
{
}
void Deinit(void)
{
}

const char **GetPluginInfo(void)
{
	static char flags[4] = { KPI_VERSION, 1, 1, 0 };
	static char *infos[] = {
		flags,
		KPI_NAME,
		" - ",
		".nez",".gbr",".gbs",".ay",".cpc",".nsz",
		".nsd",
		".nsf",
		".hes",
		".kss",
		".sgc",
		0
	};
	char *extp, *fntop;
	extern LPTSTR GetDLLArgv0(void);

	GetFullPathName(GetDLLArgv0(), MAX_PATH, iniFilePath, &fntop);
	for (extp = 0;*fntop;fntop++) if (*fntop == '.') extp = fntop;
	if (!extp) extp = fntop;
	lstrcpy(extp, ".ini");

	if (!GetPrivateProfileBoolEx(sectionname, "EnableNSF", TRUE, iniFilePath)) *infos[3 + 7] = 0;
	if (!GetPrivateProfileBoolEx(sectionname, "EnableHES", TRUE, iniFilePath)) *infos[3 + 8] = 0;
	if (!GetPrivateProfileBoolEx(sectionname, "EnableKSS", TRUE, iniFilePath)) *infos[3 + 9] = 0;
	if (!GetPrivateProfileBoolEx(sectionname, "EnableSGC", TRUE, iniFilePath)) *infos[3 +10] = 0;
	{
		extern int NSF_noise_random_reset;
		if (!GetPrivateProfileBoolEx(sectionname, "NSFNoiseRandomReset", TRUE, iniFilePath))
			NSF_noise_random_reset = 0;
		else
			NSF_noise_random_reset = 1;
	}
	{
		extern int NSF_2A03Type;
		NSF_2A03Type = GetPrivateProfileIntEx2(sectionname, "2A03Type", 1, iniFilePath, 0, 3);
	}
	{
		extern int Namco106_Realmode;
		Namco106_Realmode = GetPrivateProfileIntEx2(sectionname, "Namco106RealMode", 1, iniFilePath, 0, 3);
	}
	{
		extern int Namco106_Volume;
		Namco106_Volume = GetPrivateProfileIntEx2(sectionname, "Namco106Volume", 16, iniFilePath, 0, 64);
	}
	{
		extern int GBAMode;
		if (!GetPrivateProfileBoolEx(sectionname, "GBAMode", FALSE, iniFilePath))
			GBAMode = 0;
		else
			GBAMode = 1;
	}
	{
		extern int FDS_RealMode;
		FDS_RealMode = GetPrivateProfileIntEx2(sectionname, "FDSRealMode", 3, iniFilePath, 0, 3);
	}
	{
		extern int LowPassFilterLevel;
		LowPassFilterLevel = GetPrivateProfileIntEx2(sectionname, "LowPassFilterLevel", 16, iniFilePath, 0, 32);
	}
	{
		extern int NESAPUVolume;
		NESAPUVolume = GetPrivateProfileIntEx2(sectionname, "NESAPUVolume", 64, iniFilePath, 0, 255);
	}
	{
		extern int NESRealDAC;
		if (!GetPrivateProfileBoolEx(sectionname, "NESRealDAC", TRUE, iniFilePath))
			NESRealDAC = 0;
		else
			NESRealDAC = 1;
	}
	{
		extern char ColecoBIOSFilePath[0x200];
		GetPrivateProfileString(sectionname, "ColecoBIOSFilePath", "", ColecoBIOSFilePath, sizeof(ColecoBIOSFilePath), iniFilePath);
		WritePrivateProfileString(sectionname, "ColecoBIOSFilePath", ColecoBIOSFilePath, iniFilePath);
	}
	return infos;
}
