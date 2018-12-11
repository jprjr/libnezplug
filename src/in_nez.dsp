# Microsoft Developer Studio Project File - Name="in_nez" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=in_nez - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "in_nez.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "in_nez.mak" CFG="in_nez - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "in_nez - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "in_nez - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "in_nez - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\Release\in_nez"
# PROP BASE Intermediate_Dir "..\Release\in_nez"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release\in_nez"
# PROP Intermediate_Dir "..\Release\in_nez"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Ox /Ot /Gy /I "format" /I "device" /I "cpu" /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /GF /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "format" /I "device" /I "cpu" /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /FR /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Release\nezplug.lib /nologo /subsystem:windows /dll /machine:IX86 /def:"ui\winamp\in_nez.def" /out:"..\Release\in_nez\$(ProjectName).dll" /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Release\nezplug.lib /nologo /subsystem:windows /dll /machine:IX86 /def:"ui\winamp\in_nez.def" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "C:\My Projects\CPP Folder\My Project\nezplug\Release\in_nez\in_nez.dll" "C:\Program Files (x86)\Winamp\Plugins\"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "in_nez - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\Debug\in_nez"
# PROP BASE Intermediate_Dir "..\Debug\in_nez"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug\in_nez"
# PROP Intermediate_Dir "..\Debug\in_nez"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /ZI /Od /I "format" /I "device" /I "cpu" /I "." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /FR /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "format" /I "device" /I "cpu" /I "." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /FR /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Debug\nezplug.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /def:"ui\winamp\in_nez.def" /out:"..\Debug\in_nez\$(ProjectName).dll" /pdbtype:sept /opt:noref
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Debug\nezplug.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /def:"ui\winamp\in_nez.def" /out:"..\Debug\in_nez\$(ProjectName).dll" /pdbtype:sept /opt:noref

!ENDIF 

# Begin Target

# Name "in_nez - Win32 Release"
# Name "in_nez - Win32 Debug"
# Begin Group "ui"

# PROP Default_Filter ""
# Begin Group "winamp"

# PROP Default_Filter ""
# Begin Group "minisdk"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ui\winamp\minisdk\In2.h
# End Source File
# Begin Source File

SOURCE=.\ui\winamp\minisdk\Out.h
# End Source File
# Begin Source File

SOURCE=.\ui\winamp\minisdk\wafe.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ui\winamp\con_nez.c
DEP_CPP_CON_N=\
	".\common\cso.h"\
	".\common\win32\rc\nezplug.rh"\
	".\ui\version.h"\
	".\ui\winamp\con_nez.h"\
	".\ui\winamp\in_nez.h"\
	".\ui\winamp\minisdk\In2.h"\
	".\ui\winamp\minisdk\Out.h"\
	".\ui\winamp\minisdk\wafe.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\winamp\con_nez.h
# End Source File
# Begin Source File

SOURCE=.\ui\winamp\in_nez.c
DEP_CPP_IN_NE=\
	".\ui\version.h"\
	".\ui\winamp\con_nez.h"\
	".\ui\winamp\in_nez.h"\
	".\ui\winamp\in_xxx.h"\
	".\ui\winamp\minisdk\In2.h"\
	".\ui\winamp\minisdk\Out.h"\
	".\ui\winamp\sequencer.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\winamp\in_nez.def
# End Source File
# Begin Source File

SOURCE=.\ui\winamp\in_nez.h
# End Source File
# Begin Source File

SOURCE=.\ui\winamp\in_xxx.h
# End Source File
# Begin Source File

SOURCE=.\ui\winamp\seq_nez.c
DEP_CPP_SEQ_N=\
	".\common\win32\rc\nezplug.rh"\
	".\format\audiohandler.h"\
	".\format\handler.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	".\ui\nezplug\Dialog.h"\
	".\ui\version.h"\
	".\ui\winamp\con_nez.h"\
	".\ui\winamp\in_nez.h"\
	".\ui\winamp\sequencer.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\winamp\sequencer.h
# End Source File
# End Group
# Begin Group "nezplug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ui\nezplug\ChMask.c
DEP_CPP_CHMAS=\
	".\ui\nezplug\ChMask.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\ChMask.h
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\Dialog.c
DEP_CPP_DIALO=\
	".\ui\nezplug\ChMask.h"\
	".\ui\nezplug\Dialog.h"\
	".\ui\nezplug\Dump.h"\
	".\ui\nezplug\FileInfo.h"\
	".\ui\nezplug\IOView.h"\
	".\ui\nezplug\MemView.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\Dialog.h
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\Dump.c
DEP_CPP_DUMP_=\
	".\ui\nezplug\Dump.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\Dump.h
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\FileInfo.c
DEP_CPP_FILEI=\
	".\ui\nezplug\ChMask.h"\
	".\ui\nezplug\Dump.h"\
	".\ui\nezplug\FileInfo.h"\
	".\ui\nezplug\IOView.h"\
	".\ui\nezplug\MemView.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\FileInfo.h
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\IOView.c
DEP_CPP_IOVIE=\
	".\ui\nezplug\IOView.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\IOView.h
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\MemView.c
DEP_CPP_MEMVI=\
	".\ui\nezplug\MemView.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\MemView.h
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\nezplug.rc
# End Source File
# Begin Source File

SOURCE=.\ui\nezplug\resource.h
# End Source File
# End Group
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common\cso.h
# End Source File
# Begin Source File

SOURCE=.\common\win32\csowin32.c
DEP_CPP_CSOWI=\
	".\common\cso.h"\
	".\common\win32\win32l.h"\
	
# End Source File
# Begin Source File

SOURCE=.\common\win32\win32dll.c
DEP_CPP_WIN32=\
	".\common\win32\win32l.h"\
	
# End Source File
# Begin Source File

SOURCE=.\common\win32\win32l.h
# End Source File
# End Group
# End Target
# End Project
