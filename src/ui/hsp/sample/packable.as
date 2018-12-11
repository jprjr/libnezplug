#include "hspnez.as"

	onexit *S_Quit
	HSPNEZVersion
	vMajor = stat/100
	vMinor = stat\100
	title "TEST.NSF PLAYER - npnez.dll v"+vMajor+"."+vMinor

	exist "test.nsf"
	vBufsize = strsize
	if vBufsize < 1 : stop
	dim mBuf,vBufsize/4+2
	bload "test.nsf",mBuf
	HSPNSFOpenMemory mBuf,vBufsize
	vRet = stat
	dim mBuf,1
	if vRet != 0 : stop
	HSPNSFPlay
	stop

*S_Quit
	end
