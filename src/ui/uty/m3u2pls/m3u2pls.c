#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#ifdef __BORLAND_C__
#define _strcmpi stricmp
#endif

static int overwrite = 0;
static void m3u2pls(char *ipath)
{
	FILE *ifp, *ofp = NULL;
	int num = 0;
	do
	{
		char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
		char opath[_MAX_PATH];
		char linebuf[1024];
		ifp = fopen(ipath, "r");
		if (ifp == NULL)
		{
			printf("File '%s' cannot open.\n", ipath);
			break;
		}
		_splitpath(ipath, drive, dir, fname, ext);
		if (strcmpi(ext, ".m3u") == 0)
			strcpy(ext, ".pls");
		else
			strcat(ext, ".pls");
		_makepath(opath, drive, dir, fname, ext);
		if (!overwrite)
		{
			ofp = fopen(opath, "r");
			if (ofp != NULL)
			{
				int c;
				fclose(ofp);	ofp = NULL;
				do {
					printf("File '%s' already exists.\n", opath);
					printf("Overwrite?(Yes/No/All)");
					fflush(stdout);
					c = getch();
					printf("%c\n", c);
					switch (c)
					{
						case 'Y': case 'y':
						case 'N': case 'n':
							break;
						case 'A': case 'a':
							overwrite = 1;
							break;
						default:
							c = 0;
							break;
					}
				} while(c == 0);
				if (c == 'N' || c == 'n') break;
			}
		}
		ofp = fopen(opath, "w");
		if (ofp == NULL)
		{
			printf("File '%s' cannot open(w).\n", opath);
			break;
		}
		while (!feof(ifp))
		{
			if (NULL == fgets(linebuf, sizeof(linebuf), ifp)) break;
			if (linebuf[0] == '#' || linebuf[0] == '\n')
			{
				fputs(linebuf, ofp);
				continue;
			}
			if (++num == 1)
			{
				fputs("[playlist]\n", ofp);
			}
			fprintf(ofp, "File%d=%s", num, linebuf);
		}
	} while(0);
	if (num)
	{
		fprintf(ofp, "NumberOfEntries=%d\n", num);
		fprintf(ofp, "Version=%d\n", 2);
	}
	if (ofp != NULL) fclose(ofp);
	if (ifp != NULL) fclose(ifp);
	return;
}

static void extarg(char *arg)
{
	WIN32_FIND_DATA w32fd;
	HANDLE hFind;
	hFind = FindFirstFile(arg, &w32fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			m3u2pls(w32fd.cFileName);
		} while (FindNextFile(hFind, &w32fd));
		FindClose(hFind);
	}
}

int main(int argc, char **argv)
{
	int i;
	if (argc == 1)
		extarg("*.m3u");
	else
		for (i = 1; i < argc; i++)
		{
			char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
			char ipath[_MAX_PATH];
			_splitpath(argv[i], drive, dir, fname, ext);
			if (strcmpi(ext, ".m3u") != 0) lstrcat(ext, ".m3u");
			_makepath(ipath, drive, dir, fname, ext);
			extarg(ipath);
		}
	return 0;
}
