# Microsoft Developer Studio Project File - Name="sample" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=sample - Win32 Release COIN
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sample.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sample.mak" CFG="sample - Win32 Release COIN"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sample - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "sample - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "sample - Win32 Debug COIN" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "sample - Win32 Release COIN" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sample - Win32 Debug OSOIV"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "SAMPLE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /D "_WINDOWS" /D "_USRDLL" /D "SAMPLE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/sample/sample_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt220.lib soqtd.lib inventord.lib stbapid.lib /nologo /dll /debug /machine:I386 /out:"sample_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib"

!ELSEIF  "$(CFG)" == "sample - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "sample___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "sample___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "sample___Win32_Release_OSOIV"
# PROP Intermediate_Dir "sample___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "SAMPLE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(STBROOT)\src" /D "_WINDOWS" /D "_USRDLL" /D "SAMPLE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/sample/sample_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib stbapi.lib /nologo /dll /debug /machine:I386 /out:"sample_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib"

!ELSEIF  "$(CFG)" == "sample - Win32 Debug COIN"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "sample___Win32_Debug_COIN"
# PROP BASE Intermediate_Dir "sample___Win32_Debug_COIN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "sample___Win32_Debug_COIN"
# PROP Intermediate_Dir "sample___Win32_Debug_COIN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /D "_WINDOWS" /D "_USRDLL" /D "SAMPLE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /D "_WINDOWS" /D "_USRDLL" /D "SAMPLE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT" /D "COIN_DLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqtd.lib inventord.lib stbapid.lib /nologo /dll /debug /machine:I386 /out:"sample_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib"
# ADD LINK32 coin2d.lib stbapid.lib /nologo /dll /debug /machine:I386 /out:"sample_stb.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib"

!ELSEIF  "$(CFG)" == "sample - Win32 Release COIN"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "sample___Win32_Release_COIN"
# PROP BASE Intermediate_Dir "sample___Win32_Release_COIN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "sample___Win32_Release_COIN"
# PROP Intermediate_Dir "sample___Win32_Release_COIN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(STBROOT)\src" /D "_WINDOWS" /D "_USRDLL" /D "SAMPLE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /I "$(STBROOT)\src" /D "_WINDOWS" /D "_USRDLL" /D "SAMPLE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT" /D "COIN_DLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib stbapi.lib /nologo /dll /debug /machine:I386 /out:"sample_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib"
# ADD LINK32 coin2.lib stbapi.lib /nologo /dll /debug /machine:I386 /out:"sample_stb.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib"

!ENDIF 

# Begin Target

# Name "sample - Win32 Debug OSOIV"
# Name "sample - Win32 Release OSOIV"
# Name "sample - Win32 Debug COIN"
# Name "sample - Win32 Release COIN"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\sample.cxx
# End Source File
# Begin Source File

SOURCE=.\SoSampleKit.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\sample.h"
# End Source File
# Begin Source File

SOURCE=.\SoSampleKit.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
