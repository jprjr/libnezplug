#include "hspnez.as"

	onexit *S_Quit
	screen 0 , 64*7 , 24
	HSPNEZVersion
	vMajor = stat/100
	vMinor = stat\100
	vPlay = 1
	vSong = 1
	vVolume = 0
        sFilename = ""
	sCaption = "NEZ PLAYER - npnez.dll v"+vMajor+"."+vMinor
	title sCaption
	pos 0*64 , 0: button "OPEN" , *S_NSFOpen
	pos 1*64 , 0: button "PLAY" , *S_NSFStart
	pos 2*64 , 0: button "STOP" , *S_NSFStop
	pos 3*64 , 0: button "PREV" , *S_NSFPrev
	pos 4*64 , 0: button "NEXT" , *S_NSFNext
	pos 5*64 , 0: button "VOL-" , *S_NSFVolDown
	pos 6*64 , 0: button "VOL+" , *S_NSFVolUp
	stop

*S_NSFVolUp
	vVolume -= 64
	if vVolume < 0 : vVolume = 0
	HSPNSFVolume vVolume
	stop

*S_NSFVolDown
        vVolume += 64
	HSPNSFVolume vVolume
	stop

*S_NSFSongNo
	HSPNSFSongNo 0
	if stat < 1 : return
	vSong = stat
*S_NSFCaption
	title sCaption+" - "+vSong+" - "+sFilename
	return

*S_NSFPlay
	HSPNSFPlay
	gosub S_NSFSongNo
	return

*S_NSFOpen
	dialog "nsf;*.nsz;*.nez;*.nsd;*.kss;*.zip" , 16 , "NSF,KSS file"
	if stat != 1 : stop
	sFilename = refstr
	;packable start
	;exist sFilename
	;vBufsize = strsize
	;if vBufsize < 1 : stop
	;alloc mBuf,vBufsize
	;bload sFilename,mBuf
	;HSPNSFOpenMemory mBuf,vBufsize
	;packable end
	HSPNSFOpen sFilename
	if stat != 0 : stop
	vSong = 1
	HSPNSFVolume vVolume
	gosub S_NSFSongNo
	;if vPlay = 0 : stop
*S_NSFStart
	vPlay = 1
	gosub S_NSFPlay
	stop

*S_NSFStop
	vPlay = 0;
	HSPNSFStop
	stop

*S_NSFNext
	vSong = vSong + 1;
	HSPNSFSongNo vSong
	if vPlay != 0 : gosub S_NSFPlay
	gosub S_NSFCaption
	stop

*S_NSFPrev
	if vSong > 1 : vSong = vSong - 1;
	HSPNSFSongNo vSong
	if vPlay != 0 : gosub S_NSFPlay
	gosub S_NSFCaption
	stop

*S_Quit
	end
