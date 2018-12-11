# Microsoft Developer Studio Project File - Name="kb_nez" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=kb_nez - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "kb_nez.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "kb_nez.mak" CFG="kb_nez - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "kb_nez - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "kb_nez - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kb_nez - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\Release\kb_nez"
# PROP BASE Intermediate_Dir "..\Release\kb_nez"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release\kb_nez"
# PROP Intermediate_Dir "..\Release\kb_nez"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Ox /Ot /I "format" /I "device" /I "cpu" /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /GF /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "format" /I "device" /I "cpu" /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /FR /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Release\nezplug.lib /nologo /subsystem:windows /dll /machine:IX86 /def:"ui\kmp\kmp_com.def" /out:"..\Release\kb_nez\$(ProjectName).kpi" /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Release\nezplug.lib /nologo /subsystem:windows /dll /machine:IX86 /def:"ui\kmp\kmp_com.def" /out:"..\Release\kb_nez\kb_nez.kpi" /pdbtype:sept /opt:ref /opt:icf
# Begin Special Build Tool
TargetPath=\CPP Folder\My Project\NEZPlug\Release\kb_nez\kb_nez.kpi
SOURCE="$(InputPath)"
PostBuild_Cmds=@rem 環境に合わせて！ copy "$(TargetPath)" "C:\Program Files\_\kbmed241\Plugins\OffGao\"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "kb_nez - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\Debug\kb_nez"
# PROP BASE Intermediate_Dir "..\Debug\kb_nez"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug\kb_nez"
# PROP Intermediate_Dir "..\Debug\kb_nez"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Debug\nezplug.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /def:"ui\kmp\kmp_com.def" /out:"..\Debug\kb_nez\$(ProjectName).kpi" /pdbtype:sept /opt:noref
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\Debug\nezplug.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /def:"ui\kmp\kmp_com.def" /out:"..\Debug\kb_nez\$(ProjectName).kpi" /pdbtype:sept /opt:noref

!ENDIF 

# Begin Target

# Name "kb_nez - Win32 Release"
# Name "kb_nez - Win32 Debug"
# Begin Group "ui"

# PROP Default_Filter ""
# Begin Group "kmp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ui\kmp\kmp_com.c
DEP_CPP_KMP_C=\
	".\common\win32\win32l.h"\
	".\ui\kmp\kmp_com.h"\
	".\ui\kmp\kmp_pi.h"\
	
# End Source File
# Begin Source File

SOURCE=.\ui\kmp\kmp_com.def
# End Source File
# Begin Source File

SOURCE=.\ui\kmp\kmp_com.h
# End Source File
# Begin Source File

SOURCE=.\ui\kmp\kmp_pi.h
# End Source File
# Begin Source File

SOURCE=.\ui\kmp\kpinez.c
DEP_CPP_KPINE=\
	".\common\win32\win32l.h"\
	".\format\audiohandler.h"\
	".\format\handler.h"\
	".\format\songinfo.h"\
	".\nestypes.h"\
	".\nezplug.h"\
	".\ui\kmp\kmp_com.h"\
	".\ui\version.h"\
	
# End Source File
# End Group
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
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
