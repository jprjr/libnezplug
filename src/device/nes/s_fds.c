#include "s_fds.h"
#include "../../format/m_nsf.h"

extern void FDSSoundInstall1(NEZ_PLAY*);
extern void FDSSoundInstall2(NEZ_PLAY*);
extern void FDSSoundInstall3(NEZ_PLAY*);

void FDSSoundInstall(NEZ_PLAY *pNezPlay)
{
	switch (((NSFNSF*)pNezPlay->nsf)->fds_type)
	{
	case 1:
		FDSSoundInstall1(pNezPlay);
		break;
	case 3:
		FDSSoundInstall2(pNezPlay);
		break;
	default:
	case 2:
		FDSSoundInstall3(pNezPlay);
		break;
	}
}

void FDSSelect(NEZ_PLAY *pNezPlay, unsigned type)
{
	if ((NSFNSF*)pNezPlay->nsf)
		((NSFNSF*)pNezPlay->nsf)->fds_type = type;
}
