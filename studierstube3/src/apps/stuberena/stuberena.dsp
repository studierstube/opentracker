# Microsoft Developer Studio Project File - Name="stuberena" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=stuberena - Win32 Debug OSOIV
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "stuberena.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "stuberena.mak" CFG="stuberena - Win32 Debug OSOIV"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "stuberena - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "stuberena - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "stuberena - Win32 Debug OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_OSOIV"
# PROP Intermediate_Dir "Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STUBERENA_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STUBERENA_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/stuberena/stuberena_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt-mtedu303.lib soqt2d.lib inventord.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/stuberena/stuberena_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "stuberena - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Release_OSOIV"
# PROP BASE Intermediate_Dir "Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release_OSOIV"
# PROP Intermediate_Dir "Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "stuberena_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STUBERENA_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/stuberena/stuberena_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt-mtedu303.lib soqt2.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/stuberena/stuberena_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"

!ENDIF 

# Begin Target

# Name "stuberena - Win32 Debug OSOIV"
# Name "stuberena - Win32 Release OSOIV"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SoStuberenaKit.cxx
# End Source File
# Begin Source File

SOURCE=.\stuberena.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SoStuberenaKit.h
# End Source File
# Begin Source File

SOURCE=".\stuberena.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
