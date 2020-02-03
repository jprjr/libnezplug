# Microsoft Developer Studio Project File - Name="m_nez" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=m_nez - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "m_nez.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "m_nez.mak" CFG="m_nez - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "m_nez - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "m_nez - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "m_nez - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Release"
# PROP BASE Intermediate_Dir "../Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Release/m_nez"
# PROP Intermediate_Dir "../Release/m_nez"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "M_NEZ_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Ox /Ot /Oa /Og /Oi /Op /I "./" /D "M_NEZ_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /machine:I386 /out:"../nezplug.kpi"
# Begin Custom Build
IntDir=.¥../Release/m_nez
WkspDir=.
InputPath=¥develop¥sound¥nezplug¥nezp0946b2¥nezplug.kpi
SOURCE="$(InputPath)"

"$(IntDir)/abkpi.ok" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(WkspDir)¥abkpi.bat "$(InputPath)" "$(IntDir)/abkpi.ok"

# End Custom Build

!ELSEIF  "$(CFG)" == "m_nez - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Debug"
# PROP BASE Intermediate_Dir "../Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Debug/m_nez"
# PROP Intermediate_Dir "../Debug/m_nez"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "M_NEZ_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /I "src/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "M_NEZ_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../Debug/m_nez/nezplug.kpi" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "m_nez - Win32 Release"
# Name "m_nez - Win32 Debug"
# Begin Group "common"

# PROP Default_Filter ""
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.¥common¥zlib¥memzip.c
# End Source File
# Begin Source File

SOURCE=.¥common¥zlib¥memzip.h
# End Source File
# Begin Source File

SOURCE=.¥common¥zlib¥nez.c
# End Source File
# Begin Source File

SOURCE=.¥common¥zlib¥nez.h
# End Source File
# Begin Source File

SOURCE=.¥common¥zlib¥nezuzext.h
# End Source File
# End Group
# Begin Group "nsfsdk"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.¥common¥nsfsdk¥nsfsdk.c
# End Source File
# Begin Source File

SOURCE=.¥common¥nsfsdk¥nsfsdk.h
# End Source File
# End Group
# Begin Group "common_win32"

# PROP Default_Filter ""
# Begin Group "rc"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.¥common¥win32¥csowin32.c
# End Source File
# Begin Source File

SOURCE=.¥common¥win32¥loadvrc7.c
# End Source File
# Begin Source File

SOURCE=.¥common¥win32¥win32dll.c
# End Source File
# Begin Source File

SOURCE=.¥common¥win32¥win32l.h
# End Source File
# End Group
# Begin Source File

SOURCE=.¥common¥cso.h
# End Source File
# End Group
# Begin Group "ui"

# PROP Default_Filter ""
# Begin Group "kmp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.¥ui¥kmp¥kmp_com.c
# End Source File
# Begin Source File

SOURCE=.¥ui¥kmp¥kmp_com.def
# End Source File
# Begin Source File

SOURCE=.¥ui¥kmp¥kmp_com.h
# End Source File
# Begin Source File

SOURCE=.¥ui¥kmp¥kpinez.c
# End Source File
# End Group
# Begin Source File

SOURCE=.¥ui¥version.h
# End Source File
# End Group
# Begin Group "nes"

# PROP Default_Filter ""
# Begin Group "device"

# PROP Default_Filter ""
# Begin Group "ill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.¥nes¥device¥ill¥i_fmpac.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥ill¥i_fmunit.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥ill¥i_vrc7.h
# End Source File
# End Group
# Begin Group "fdsplugin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.¥nes¥device¥fdsplugin¥FDSplugin.h
# End Source File
# End Group
# Begin Source File

SOURCE=.¥nes¥device¥divfix.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥kmsnddev.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥logtable.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥logtable.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_apu.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_apu.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_deltat.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_deltat.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_dmg.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_dmg.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_fds.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_fds.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_fds1.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_fds2.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_fds3.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_fdse.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_fme7.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_fme7.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_hes.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_hes.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_logtbl.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_logtbl.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_mmc5.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_mmc5.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_n106.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_n106.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_opl.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_opl.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_opltbl.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_opltbl.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_psg.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_psg.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_scc.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_scc.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_sng.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_sng.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_vrc6.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_vrc6.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_vrc7.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥device¥s_vrc7.h
# End Source File
# End Group
# Begin Group "km6502"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.¥nes¥km6502¥km6280.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km6280m.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km6502.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km6502cd.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km6502ct.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km6502ex.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km6502ft.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km6502m.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km6502ot.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km65c02.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥km65c02m.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥km6502¥kmconfig.h
# End Source File
# End Group
# Begin Group "kmz80"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmdmg.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmevent.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmevent.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmtypes.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmz80.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmz80.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmz80c.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmz80i.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmz80set.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥kmz80¥kmz80t.c
# End Source File
# End Group
# Begin Source File

SOURCE=.¥nes¥audiosys.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥audiosys.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥handler.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥handler.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_gbr.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_gbr.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_hes.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_hes.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_kss.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_kss.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_nsf.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_nsf.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_zxay.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥m_zxay.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥neserr.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥nestypes.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥nsdout.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥nsdout.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥nsdplay.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥nsdplay.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥nsf6502.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥nsf6502.h
# End Source File
# Begin Source File

SOURCE=.¥nes¥songinfo.c
# End Source File
# Begin Source File

SOURCE=.¥nes¥songinfo.h
# End Source File
# End Group
# End Target
# End Project
