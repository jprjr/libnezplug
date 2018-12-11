#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>

#ifdef _MSC_VER
#if _MSC_VER >= 1200
#pragma comment(linker, "/OPT:NOWIN98")
/* #pragma comment(linker, "/WS:AGGRESSIVE") */
#endif
#endif

#include "../nsfsdk.h"

#define LEN_OUTBUF 2048

static char *GetExt(char *filename) {
	char *p = filename, *p2 = NULL;
	while (*p)
	{
		if (*p == '/' || *p == '\\' || *p == ':') p2 = NULL;
		if (*p == '.') p2 = p;
#ifdef _MBCS
		if (isleadbyte(p[0]) && p[1]) p++;
#endif
		p++;
	}
	if (p2 != NULL) return p2;
	return p;
}

static SetFourCC(unsigned char *p, char *v)
{
	p[0] = v[0];
	p[1] = v[1];
	p[2] = v[2];
	p[3] = v[3];
}
static SetDwordLE(unsigned char *p, unsigned v)
{
	p[0] =  v        & 255;
	p[1] = (v >>  8) & 255;
	p[2] = (v >> 16) & 255;
	p[3] = (v >> 24) & 255;
}
static SetWordLE(unsigned char *p, unsigned v)
{
	p[0] =  v        & 255;
	p[1] = (v >>  8) & 255;
}

static void nsf2wav(char *ipath, char *opath, unsigned freq, unsigned time, unsigned songno, unsigned filter)
{
	FILE *ifp = NULL, *ofp = NULL;
	unsigned char *nsfbuf = NULL;
	char *pathbuf = NULL;
	HNSF hnsf = NULL;
	setlocale(LC_ALL, "");
	do {
		int worktime, ch;
		clock_t start_clock, end_clock;
		size_t size, len;
		unsigned remain;
		unsigned char wavhead[0x32];
		signed short outbuf[2 * LEN_OUTBUF];
		if (freq == 0) freq = 44100;
		if (time == 0) time = 300;
		remain = freq * time;
		size = 0;
		len = strlen(ipath);
		if (opath != NULL && *opath) size = strlen(opath);
		if (size < len) size = len;
		pathbuf = malloc(size + 8);
		if (pathbuf == NULL)
		{
			fprintf(stderr, "Error: Short of memory\n", ipath);
			break;
		}
		ifp = fopen(ipath, "rb");
		if (ifp == NULL)
		{
			strcpy(pathbuf, ipath);
			strcat(pathbuf, ".nsf");
			ifp = fopen(pathbuf, "rb");
			if (ifp == NULL)
			{
				fprintf(stderr, "Error: File '%s' cannot open\n", ipath);
				break;
			}
		}
		fseek(ifp, 0, SEEK_END);
		size = ftell(ifp);
		nsfbuf = malloc(size + 16);
		if (nsfbuf == NULL)
		{
			fprintf(stderr, "Error: Short of memory\n");
			break;
		}
		fseek(ifp, 0, SEEK_SET);
		fread(nsfbuf, 1, size, ifp);
		fclose(ifp); ifp = NULL;
		hnsf = NSFSDK_Load(nsfbuf, size);
		if (hnsf == NULL)
		{
			fprintf(stderr, "Error: Short of memory\n");
			break;
		}
		if (songno == 0) songno = NSFSDK_GetSongNo(hnsf);
		if (opath != NULL && *opath)
		{
			strcpy(pathbuf, opath);
		}
		else
		{
			strcpy(pathbuf, ipath);
			sprintf(GetExt(pathbuf), "_%d.wav", songno);
		}

		NSFSDK_SetSongNo(hnsf, songno);
		NSFSDK_SetFrequency(hnsf, freq);
		NSFSDK_SetChannel(hnsf, NSFSDK_GetChannel(hnsf));
		NSFSDK_SetNosefartFilter(hnsf, filter);

#if 0
		ofp = fopen(pathbuf, "rb");
		if (ofp != NULL)
		{
			fprintf(stderr, "Error: File '%s' already exists\n", pathbuf);
			break;
		}
#endif
		ofp = fopen(pathbuf, "wb");
		if (ofp == NULL)
		{
			fprintf(stderr, "Error: File '%s' cannot open\n", pathbuf);
			break;
		}
		SetFourCC(wavhead  + 0x00, "RIFF");
		SetDwordLE(wavhead + 0x04, remain * 2 * NSFSDK_GetChannel(hnsf) + 0x26);
		SetFourCC(wavhead  + 0x08, "WAVE");
		SetFourCC(wavhead  + 0x0C, "fmt ");
		SetDwordLE(wavhead + 0x10, 0x12);
		SetWordLE(wavhead  + 0x14, 1/* WAVE_FORMAT_PCM */);
		SetWordLE(wavhead  + 0x16, NSFSDK_GetChannel(hnsf));
		SetDwordLE(wavhead + 0x18, freq);
		SetDwordLE(wavhead + 0x1C, freq * 2 * NSFSDK_GetChannel(hnsf));
		SetWordLE(wavhead  + 0x20, 2 * NSFSDK_GetChannel(hnsf));
		SetWordLE(wavhead  + 0x22, 16);
		SetWordLE(wavhead  + 0x24, 0);
		SetFourCC(wavhead  + 0x26, "data");
		SetDwordLE(wavhead + 0x2a, remain * 2 * NSFSDK_GetChannel(hnsf));
		fwrite(wavhead, 1, 0x2a + 4, ofp);

		start_clock = clock();
		/* start */
		NSFSDK_Reset(hnsf);
		while (remain > LEN_OUTBUF)
		{
			remain -= LEN_OUTBUF;
			NSFSDK_Render(hnsf, outbuf, LEN_OUTBUF);
			fwrite(outbuf, NSFSDK_GetChannel(hnsf) * sizeof(signed short), LEN_OUTBUF, ofp);
		}
		NSFSDK_Render(hnsf, outbuf, remain);
		fwrite(outbuf, NSFSDK_GetChannel(hnsf) * sizeof(signed short), remain, ofp);
		/* end */
		end_clock = clock();
		worktime = (end_clock - start_clock) / CLOCKS_PER_SEC;
		fprintf(stderr, "Finished. %02d:%02d\n", worktime / 60, worktime % 60);
	} while(0);
	if (hnsf != NULL) NSFSDK_Terminate(hnsf);
	if (ofp != NULL) fclose(ofp);
	if (ifp != NULL) fclose(ifp);
	if (nsfbuf != NULL) free(nsfbuf);
	if (pathbuf != NULL) free(pathbuf);
}

int main(int argc, char **argv)
{
	unsigned count = 0, freq = 0, time = 0, songno = 0, filter = 0;
	char *opath = NULL;
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strncmp(argv[i],"-f",2) == 0)
		{
			char *p = argv[i] + 2;
			if (*p == '\0')
			{
				if (argv[i + 1] == NULL) break;
				p = argv[++i];
			}
			freq = atoi(p);
			continue;
		}
		if (strncmp(argv[i],"-t",2) == 0)
		{
			char *p = argv[i] + 2;
			if (*p == '\0')
			{
				if (argv[i + 1] == NULL) break;
				p = argv[++i];
			}
			time = atoi(p);
			continue;
		}
		if (strncmp(argv[i],"-n",2) == 0)
		{
			char *p = argv[i] + 2;
			if (*p == '\0')
			{
				if (argv[i + 1] == NULL) break;
				p = argv[++i];
			}
			songno = atoi(p);
			continue;
		}
		if (strncmp(argv[i],"-o",2) == 0)
		{
			char *p = argv[i] + 2;
			if (*p == '\0')
			{
				if (argv[i + 1] == NULL) break;
				p = argv[++i];
			}
			opath = p;
			continue;
		}
		if (strncmp(argv[i],"-i",2) == 0)
		{
			char *p = argv[i] + 2;
			if (*p == '\0')
			{
				if (argv[i + 1] == NULL) break;
				p = argv[++i];
			}
			filter = atoi(p);
			continue;
		}
		nsf2wav(argv[i], opath, freq, time, songno, filter);
		count++;
		opath = NULL;
		break;
	}
	if (count == 0)
	{
		fprintf(stderr, "Usage: %s [options]... file...\n", argv[0]);
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "  -f<frequency>    set output frequency in Hz\n");
		fprintf(stderr, "  -t<time>         set output time in sec\n");
		fprintf(stderr, "  -n<songno>       set output song no\n");
		fprintf(stderr, "  -o<file>         place output into the <file>\n");
	}
	return 0;
}