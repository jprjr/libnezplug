# Microsoft Developer Studio Project File - Name="nezplug" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=nezplug - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "nezplug.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "nezplug.mak" CFG="nezplug - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "nezplug - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "nezplug - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nezplug - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\Release"
# PROP BASE Intermediate_Dir "..\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\..\Release\nezplug.tlb" /win32
# ADD MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\..\Release\nezplug.tlb" /win32
# ADD BASE CPP /nologo /MT /W3 /GX /Ox /Ot /Gy /I "format" /I "device" /I "zlib" /I "cpu" /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /FR /GF /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "format" /I "device" /I "zlib" /I "cpu" /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /FR /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "nezplug - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\Debug"
# PROP BASE Intermediate_Dir "..\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "..\Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\..\Debug\nezplug.tlb" /win32
# ADD MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\..\Debug\nezplug.tlb" /win32
# ADD BASE CPP /nologo /MTd /W4 /GX /ZI /Od /I "format" /I "device" /I "zlib" /I "cpu" /I "." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /FR /GZ /c
# ADD CPP /nologo /MTd /W4 /GX /ZI /Od /I "format" /I "device" /I "zlib" /I "cpu" /I "." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /FR /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "nezplug - Win32 Release"
# Name "nezplug - Win32 Debug"
# Begin Group "cpu"

# PROP Default_Filter ""
# Begin Group "6502"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpu\km6502\km2a03m.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km6280.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km6280m.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km6502.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km6502cd.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km6502ct.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km6502ex.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km6502ft.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km6502m.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km6502ot.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km65c02.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\km65c02m.h
# End Source File
# Begin Source File

SOURCE=.\cpu\km6502\kmconfig.h
# End Source File
# End Group
# Begin Group "z80"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpu\kmz80\kmdmg.c
DEP_CPP_KMDMG=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\cpu\kmz80\kmz80.h"\
	".\cpu\kmz80\kmz80i.h"\
	
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\kmevent.c
DEP_CPP_KMEVE=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\kmevent.h
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\kmr800.c
DEP_CPP_KMR80=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\cpu\kmz80\kmz80.h"\
	".\cpu\kmz80\kmz80i.h"\
	
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\kmtypes.h
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\kmz80.c
DEP_CPP_KMZ80=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\cpu\kmz80\kmz80.h"\
	".\cpu\kmz80\kmz80i.h"\
	
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\kmz80.h
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\kmz80c.c
DEP_CPP_KMZ80C=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\cpu\kmz80\kmz80.h"\
	".\cpu\kmz80\kmz80i.h"\
	
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\kmz80i.h
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\kmz80t.c
DEP_CPP_KMZ80T=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\cpu\kmz80\kmz80.h"\
	".\cpu\kmz80\kmz80i.h"\
	
# End Source File
# Begin Source File

SOURCE=.\cpu\kmz80\makeft.c
# End Source File
# End Group
# End Group
# Begin Group "device"

# PROP Default_Filter ""
# Begin Group "nes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\device\nes\logtable.c
DEP_CPP_LOGTA=\
	".\device\nes\logtable.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\logtable.h
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_apu.c
DEP_CPP_S_APU=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\logtable.h"\
	".\device\nes\s_apu.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_apu.h
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_fds.c
DEP_CPP_S_FDS=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\s_fds.h"\
	".\format\audiohandler.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_fds.h
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_fds1.c
DEP_CPP_S_FDS1=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\logtable.h"\
	".\device\nes\s_fds.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_fds2.c
DEP_CPP_S_FDS2=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\logtable.h"\
	".\device\nes\s_fds.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_fds3.c
DEP_CPP_S_FDS3=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\logtable.h"\
	".\device\nes\s_fds.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_fme7.c
DEP_CPP_S_FME=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\logtable.h"\
	".\device\nes\s_fme7.h"\
	".\device\s_psg.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_fme7.h
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_mmc5.c
DEP_CPP_S_MMC=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\logtable.h"\
	".\device\nes\s_mmc5.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_mmc5.h
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_n106.c
DEP_CPP_S_N10=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\logtable.h"\
	".\device\nes\s_n106.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_n106.h
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_vrc6.c
DEP_CPP_S_VRC=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\logtable.h"\
	".\device\nes\s_vrc6.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_vrc6.h
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_vrc7.c
DEP_CPP_S_VRC7=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\logtable.h"\
	".\device\nes\s_vrc7.h"\
	".\device\opl\s_opl.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\nes\s_vrc7.h
# End Source File
# End Group
# Begin Group "opll"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\device\opl\s_deltat.c
DEP_CPP_S_DEL=\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\opl\s_deltat.h"\
	".\device\s_logtbl.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\opl\s_deltat.h
# End Source File
# Begin Source File

SOURCE=.\device\opl\s_opl.c
DEP_CPP_S_OPL=\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\opl\ill\i_fmpac.h"\
	".\device\opl\ill\i_fmunit.h"\
	".\device\opl\ill\i_vrc7.h"\
	".\device\opl\s_deltat.h"\
	".\device\opl\s_opl.h"\
	".\device\opl\s_opltbl.h"\
	".\device\s_logtbl.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\opl\s_opl.h
# End Source File
# Begin Source File

SOURCE=.\device\opl\s_opltbl.c
DEP_CPP_S_OPLT=\
	".\device\opl\s_opltbl.h"\
	".\device\s_logtbl.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\opl\s_opltbl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\device\divfix.h
# End Source File
# Begin Source File

SOURCE=.\device\kmsnddev.h
# End Source File
# Begin Source File

SOURCE=.\device\s_dmg.c
DEP_CPP_S_DMG=\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\s_dmg.h"\
	".\device\s_logtbl.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\s_dmg.h
# End Source File
# Begin Source File

SOURCE=.\device\s_hes.c
DEP_CPP_S_HES=\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\s_hes.h"\
	".\device\s_logtbl.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\s_hes.h
# End Source File
# Begin Source File

SOURCE=.\device\s_hesad.c
DEP_CPP_S_HESA=\
	".\device\kmsnddev.h"\
	".\device\opl\s_deltat.h"\
	".\device\s_hesad.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\s_hesad.h
# End Source File
# Begin Source File

SOURCE=.\device\s_logtbl.c
DEP_CPP_S_LOG=\
	".\device\s_logtbl.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\s_logtbl.h
# End Source File
# Begin Source File

SOURCE=.\device\s_psg.c
DEP_CPP_S_PSG=\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\s_logtbl.h"\
	".\device\s_psg.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\s_psg.h
# End Source File
# Begin Source File

SOURCE=.\device\s_scc.c
DEP_CPP_S_SCC=\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\s_logtbl.h"\
	".\device\s_scc.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\s_scc.h
# End Source File
# Begin Source File

SOURCE=.\device\s_sng.c
DEP_CPP_S_SNG=\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\s_logtbl.h"\
	".\device\s_sng.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\device\s_sng.h
# End Source File
# End Group
# Begin Group "format"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\format\audiohandler.h
# End Source File
# Begin Source File

SOURCE=.\format\audiosys.c
DEP_CPP_AUDIO=\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\audiosys.h
# End Source File
# Begin Source File

SOURCE=.\format\handler.c
DEP_CPP_HANDL=\
	".\format\audiohandler.h"\
	".\format\handler.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\handler.h
# End Source File
# Begin Source File

SOURCE=.\format\m_gbr.c
DEP_CPP_M_GBR=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\cpu\kmz80\kmz80.h"\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\s_dmg.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_gbr.h"\
	".\format\songinfo.h"\
	".\neserr.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\m_gbr.h
# End Source File
# Begin Source File

SOURCE=.\format\m_hes.c
DEP_CPP_M_HES=\
	".\cpu\km6502\km6280.h"\
	".\cpu\km6502\km6280m.h"\
	".\cpu\km6502\km6502cd.h"\
	".\cpu\km6502\km6502ct.h"\
	".\cpu\km6502\km6502ex.h"\
	".\cpu\km6502\km6502ft.h"\
	".\cpu\km6502\km6502ot.h"\
	".\cpu\km6502\kmconfig.h"\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\s_hes.h"\
	".\device\s_hesad.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_hes.h"\
	".\format\songinfo.h"\
	".\neserr.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\m_hes.h
# End Source File
# Begin Source File

SOURCE=.\format\m_kss.c
DEP_CPP_M_KSS=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\cpu\kmz80\kmz80.h"\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\opl\s_opl.h"\
	".\device\s_psg.h"\
	".\device\s_scc.h"\
	".\device\s_sng.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_kss.h"\
	".\format\songinfo.h"\
	".\neserr.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\m_kss.h
# End Source File
# Begin Source File

SOURCE=.\format\m_nsd.c
DEP_CPP_M_NSD=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsd.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\neserr.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\m_nsd.h
# End Source File
# Begin Source File

SOURCE=.\format\m_nsf.c
DEP_CPP_M_NSF=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\device\nes\s_apu.h"\
	".\device\nes\s_fds.h"\
	".\device\nes\s_fme7.h"\
	".\device\nes\s_mmc5.h"\
	".\device\nes\s_n106.h"\
	".\device\nes\s_vrc6.h"\
	".\device\nes\s_vrc7.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\neserr.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\m_nsf.h
# End Source File
# Begin Source File

SOURCE=.\format\m_sgc.c
DEP_CPP_M_SGC=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\cpu\kmz80\kmz80.h"\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\opl\s_opl.h"\
	".\device\s_sng.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_sgc.h"\
	".\format\songinfo.h"\
	".\neserr.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\m_sgc.h
# End Source File
# Begin Source File

SOURCE=.\format\m_zxay.c
DEP_CPP_M_ZXA=\
	".\cpu\kmz80\kmevent.h"\
	".\cpu\kmz80\kmtypes.h"\
	".\cpu\kmz80\kmz80.h"\
	".\device\divfix.h"\
	".\device\kmsnddev.h"\
	".\device\s_psg.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_zxay.h"\
	".\format\songinfo.h"\
	".\neserr.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\m_zxay.h
# End Source File
# Begin Source File

SOURCE=.\format\nezplug.c
DEP_CPP_NEZPL=\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_gbr.h"\
	".\format\m_hes.h"\
	".\format\m_kss.h"\
	".\format\m_nsd.h"\
	".\format\m_nsf.h"\
	".\format\m_sgc.h"\
	".\format\m_zxay.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\neserr.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\nsf6502.c
DEP_CPP_NSF65=\
	".\cpu\km6502\km2a03m.h"\
	".\cpu\km6502\km6502.h"\
	".\cpu\km6502\km6502cd.h"\
	".\cpu\km6502\km6502ct.h"\
	".\cpu\km6502\km6502ex.h"\
	".\cpu\km6502\km6502ft.h"\
	".\cpu\km6502\km6502ot.h"\
	".\cpu\km6502\kmconfig.h"\
	".\device\kmsnddev.h"\
	".\format\audiohandler.h"\
	".\format\audiosys.h"\
	".\format\handler.h"\
	".\format\m_nsf.h"\
	".\format\nsf6502.h"\
	".\format\songinfo.h"\
	".\neserr.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\nsf6502.h
# End Source File
# Begin Source File

SOURCE=.\format\songinfo.c
DEP_CPP_SONGI=\
	".\format\songinfo.h"\
	".\nestypes.h"\
	
# End Source File
# Begin Source File

SOURCE=.\format\songinfo.h
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\adler32.c
DEP_CPP_ADLER=\
	".\zlib\zconf.h"\
	".\zlib\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\zlib\crc32.c
DEP_CPP_CRC32=\
	".\zlib\crc32.h"\
	".\zlib\zconf.h"\
	".\zlib\zlib.h"\
	".\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=.\zlib\crc32.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.c
DEP_CPP_INFFA=\
	".\zlib\inffast.h"\
	".\zlib\inflate.h"\
	".\zlib\inftrees.h"\
	".\zlib\zconf.h"\
	".\zlib\zlib.h"\
	".\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.c
DEP_CPP_INFLA=\
	".\zlib\inffast.h"\
	".\zlib\inffixed.h"\
	".\zlib\inflate.h"\
	".\zlib\inftrees.h"\
	".\zlib\zconf.h"\
	".\zlib\zlib.h"\
	".\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.c
DEP_CPP_INFTR=\
	".\zlib\inftrees.h"\
	".\zlib\zconf.h"\
	".\zlib\zlib.h"\
	".\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\memzip.c
DEP_CPP_MEMZI=\
	".\zlib\memzip.h"\
	".\zlib\zconf.h"\
	".\zlib\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\zlib\memzip.h
# End Source File
# Begin Source File

SOURCE=.\zlib\nez.c
DEP_CPP_NEZ_C=\
	".\zlib\memzip.h"\
	".\zlib\nez.h"\
	".\zlib\nezuzext.h"\
	
# End Source File
# Begin Source File

SOURCE=.\zlib\nez.h
# End Source File
# Begin Source File

SOURCE=.\zlib\nezuzext.h
# End Source File
# Begin Source File

SOURCE=.\zlib\uncompr.c
DEP_CPP_UNCOM=\
	".\zlib\zconf.h"\
	".\zlib\zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.c
DEP_CPP_ZUTIL=\
	".\zlib\zconf.h"\
	".\zlib\zlib.h"\
	".\zlib\zutil.h"\
	
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\neserr.h
# End Source File
# Begin Source File

SOURCE=.\nestypes.h
# End Source File
# Begin Source File

SOURCE=.\nezplug.h
# End Source File
# Begin Source File

SOURCE=.\ui\version.h
# End Source File
# End Target
# End Project
