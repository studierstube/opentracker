# Microsoft Developer Studio Project File - Name="styluscalibration" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=styluscalibration - Win32 Release Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "styluscalibration.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "styluscalibration.mak" CFG="styluscalibration - Win32 Release Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "styluscalibration - Win32 Release TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "styluscalibration - Win32 Debug TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "styluscalibration - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "styluscalibration - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "styluscalibration - Win32 Release TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "styluscalibration - Win32 Debug TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "styluscalibration - Win32 Debug OSOIV with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "styluscalibration - Win32 Release OSOIV with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "styluscalibration - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "styluscalibration - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "styluscalibration - Win32 Release TGS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_Release_TGS"
# PROP BASE Intermediate_Dir "Win32_Release_TGS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Release_TGS"
# PROP Intermediate_Dir "Win32_Release_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll"

!ELSEIF  "$(CFG)" == "styluscalibration - Win32 Debug TGS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug_TGS"
# PROP BASE Intermediate_Dir "Win32_Debug_TGS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_TGS"
# PROP Intermediate_Dir "Win32_Debug_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../../" /I "$(OTROOT)\src" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /D "USE_OPENTRACKER" /FD /GZ /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opentrackerd.lib opengl32.lib /nologo /dll /debug /machine:I386 /out:"../../../../bin/apps/calibration/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OTROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "styluscalibration - Win32 Debug OSOIV"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../../" /I "$(OTROOT)\src" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /D "USE_OPENTRACKER" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt220.lib soqtd.lib inventord.lib opentrackerd.lib opengl32.lib /nologo /dll /debug /machine:I386 /out:"../../../../bin/apps/calibration/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\lib"

!ELSEIF  "$(CFG)" == "styluscalibration - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "styluscalibration___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "styluscalibration___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "styluscalibration___Win32_Release_OSOIV"
# PROP Intermediate_Dir "styluscalibration___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../../" /I "$(OTROOT)\src" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib opentracker.lib opengl32.lib /nologo /dll /debug /machine:I386 /out:"../../../../bin/apps/calibration/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\lib"

!ELSEIF  "$(CFG)" == "styluscalibration - Win32 Release TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "styluscalibration___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "styluscalibration___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "styluscalibration___Win32_Release_TGS_with_SGI_Video"
# PROP Intermediate_Dir "styluscalibration___Win32_Release_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll"
# ADD LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll"

!ELSEIF  "$(CFG)" == "styluscalibration - Win32 Debug TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "styluscalibration___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "styluscalibration___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "styluscalibration___Win32_Debug_TGS_with_SGI_Video"
# PROP Intermediate_Dir "styluscalibration___Win32_Debug_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "styluscalibration - Win32 Debug OSOIV with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "styluscalibration___Win32_Debug_OSOIV_with_SGI_Video"
# PROP BASE Intermediate_Dir "styluscalibration___Win32_Debug_OSOIV_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "styluscalibration___Win32_Debug_OSOIV_with_SGI_Video"
# PROP Intermediate_Dir "styluscalibration___Win32_Debug_OSOIV_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "styluscalibration - Win32 Release OSOIV with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "styluscalibration___Win32_Release_OSOIV_with_SGI_Video"
# PROP BASE Intermediate_Dir "styluscalibration___Win32_Release_OSOIV_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "styluscalibration___Win32_Release_OSOIV_with_SGI_Video"
# PROP Intermediate_Dir "styluscalibration___Win32_Release_OSOIV_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "styluscalibration - Win32 Debug Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "styluscalibration___Win32_Debug_Coin0"
# PROP BASE Intermediate_Dir "styluscalibration___Win32_Debug_Coin0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "styluscalibration___Win32_Debug_Coin0"
# PROP Intermediate_Dir "styluscalibration___Win32_Debug_Coin0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../../" /I "$(OTROOT)\src" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "STYLUSCALIBRATION_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /D "USE_OPENTRACKER" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "../../../" /I "$(OTROOT)\src" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /D "STYLUSCALIBRATION_EXPORTS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "USE_COIN" /D "COIN_DLL" /D "USE_SOQT" /D "USE_DIV" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqtd.lib inventord.lib opentrackerd.lib opengl32.lib /nologo /dll /debug /machine:I386 /out:"../../../../bin/apps/calibration/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\lib"
# ADD LINK32 qt-mtedu303.lib qtmain.lib soqt1Coind.lib coin2d.lib stbapid.lib powelld.lib opentrackerd.lib /nologo /dll /debug /machine:I386 /out:"../../../../bin/apps/calibration/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(COINROOT)/lib" /libpath:"$(STBROOT)/lib" /libpath:"$(QTDIR)/LIB" /libpath:"$(OTROOT)\lib"

!ELSEIF  "$(CFG)" == "styluscalibration - Win32 Release Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "styluscalibration___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "styluscalibration___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "styluscalibration___Win32_Release_Coin"
# PROP Intermediate_Dir "styluscalibration___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "../../../" /I "$(OTROOT)\src" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /D "STYLUSCALIBRATION_EXPORTS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "USE_COIN" /D "COIN_DLL" /D "USE_SOQT" /D "USE_DIV" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /X /I "../../../" /I "$(OTROOT)\src" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /D "STYLUSCALIBRATION_EXPORTS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "USE_COIN" /D "COIN_DLL" /D "USE_SOQT" /D "USE_DIV" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qt-mtedu303.lib qtmain.lib soqt1Coind.lib coin2d.lib stbapid.lib powelld.lib opentrackerd.lib /nologo /dll /debug /machine:I386 /out:"../../../../bin/apps/calibration/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(COINROOT)/lib" /libpath:"$(STBROOT)/lib" /libpath:"$(QTDIR)/LIB" /libpath:"$(OTROOT)\lib"
# ADD LINK32 qtmain.lib qt-mtedu303.lib soqt1Coin.lib coin2.lib stbapi.lib powell.lib opentracker.lib /nologo /dll /incremental:no /machine:I386 /out:"../../../../bin/apps/calibration/styluscalibration/styluscalibration_stb.dll" /pdbtype:sept /libpath:"$(COINROOT)/lib" /libpath:"$(STBROOT)/lib" /libpath:"$(QTDIR)/LIB" /libpath:"$(OTROOT)\lib" /libpath:"Release"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "styluscalibration - Win32 Release TGS"
# Name "styluscalibration - Win32 Debug TGS"
# Name "styluscalibration - Win32 Debug OSOIV"
# Name "styluscalibration - Win32 Release OSOIV"
# Name "styluscalibration - Win32 Release TGS with SGI Video"
# Name "styluscalibration - Win32 Debug TGS with SGI Video"
# Name "styluscalibration - Win32 Debug OSOIV with SGI Video"
# Name "styluscalibration - Win32 Release OSOIV with SGI Video"
# Name "styluscalibration - Win32 Debug Coin"
# Name "styluscalibration - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\utils\SoMessageKit.cxx
# End Source File
# Begin Source File

SOURCE=..\utils\SoSampler.cxx
# End Source File
# Begin Source File

SOURCE=.\SoStylusCalibrationKit.cxx
# End Source File
# Begin Source File

SOURCE=".\styluscalibration.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\utils\SoMessageKit.h
# End Source File
# Begin Source File

SOURCE=..\utils\SoSampler.h
# End Source File
# Begin Source File

SOURCE=.\SoStylusCalibrationKit.h
# End Source File
# Begin Source File

SOURCE=".\styluscalibration.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
