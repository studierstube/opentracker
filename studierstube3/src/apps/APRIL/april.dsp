# Microsoft Developer Studio Project File - Name="APRIL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=APRIL - Win32 Release Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "april.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "april.mak" CFG="APRIL - Win32 Release Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "APRIL - Win32 Release TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APRIL - Win32 Debug TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APRIL - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APRIL - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APRIL - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APRIL - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "APRIL - Win32 Release TGS"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "APRIL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(STBROOT)\src" /I "$(XERCESCROOT)\include" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/APRIL/april.dll" /libpath:"$(STBROOT)\lib" /libpath:"$(XERCESCROOT)\lib"

!ELSEIF  "$(CFG)" == "APRIL - Win32 Debug TGS"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "APRIL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(STBROOT)\src" /I "$(XERCESCROOT)\include" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/APRIL/april.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib" /libpath:"$(XERCESCROOT)\lib"

!ELSEIF  "$(CFG)" == "APRIL - Win32 Debug OSOIV"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /I "$(XERCESCROOT)\include" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_OPENTRACKER" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/april/april_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 inventord.lib glu32.lib opengl32.lib xerces-c_2D.lib stbapid.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/aprild.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(XERCESCROOT)\lib"
# SUBTRACT LINK32 /profile /pdb:none

!ELSEIF  "$(CFG)" == "APRIL - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "april___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "april___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "april___Win32_Release_OSOIV"
# PROP Intermediate_Dir "april___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /I "$(XERCESCROOT)\include" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "USE_OPENTRACKER" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/april/april_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 inventord.lib glu32.lib opengl32.lib xerces-c_2.lib stbapi.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/april.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(XERCESCROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "APRIL - Win32 Debug Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "APRIL___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "APRIL___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "APRIL___Win32_Debug_Coin"
# PROP Intermediate_Dir "APRIL___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /I "$(XERCESCROOT)\include" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_OPENTRACKER" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /I "$(XERCESCROOT)\include" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "HA" /D "USE_OPENTRACKER" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "USE_COIN" /D "COIN_DLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib glu32.lib opengl32.lib xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/APRIL/april.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(XERCESCROOT)\lib"
# SUBTRACT BASE LINK32 /profile /pdb:none
# ADD LINK32 coin2d.lib glu32.lib opengl32.lib xerces-c_2D.lib stbapid.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/aprild.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(XERCESCROOT)\lib"
# SUBTRACT LINK32 /profile /pdb:none

!ELSEIF  "$(CFG)" == "APRIL - Win32 Release Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "APRIL___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "APRIL___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "APRIL___Win32_Release_Coin"
# PROP Intermediate_Dir "APRIL___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /I "$(XERCESCROOT)\include" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "USE_OPENTRACKER" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /Gi /GX /O2 /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /I "$(XERCESCROOT)\include" /D "_WINDOWS" /D "_USRDLL" /D "APRIL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_OPENTRACKER" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /D "USE_COIN" /D "COIN_DLL" /FD /GZ /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib glu32.lib opengl32.lib xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/APRIL/april.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(XERCESCROOT)\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 coin2.lib glu32.lib opengl32.lib xerces-c_2.lib stbapi.lib /nologo /dll /machine:I386 /out:"../../../lib/april.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(XERCESCROOT)\lib"
# SUBTRACT LINK32 /pdb:none /debug

!ENDIF 

# Begin Target

# Name "APRIL - Win32 Release TGS"
# Name "APRIL - Win32 Debug TGS"
# Name "APRIL - Win32 Debug OSOIV"
# Name "APRIL - Win32 Release OSOIV"
# Name "APRIL - Win32 Debug Coin"
# Name "APRIL - Win32 Release Coin"
# Begin Group "State Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SAXReadHandler.cxx
# End Source File
# Begin Source File

SOURCE=.\SAXReadHandler.h
# End Source File
# Begin Source File

SOURCE=.\SoStateEngine.cxx
# End Source File
# Begin Source File

SOURCE=.\SoStateEngine.h
# End Source File
# Begin Source File

SOURCE=.\StateEngine.cxx
# End Source File
# Begin Source File

SOURCE=.\StateEngine.h
# End Source File
# End Group
# Begin Group "Light Projector"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SoLightProjector.cxx
# End Source File
# Begin Source File

SOURCE=.\SoLightProjector.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\april.cxx
# End Source File
# Begin Source File

SOURCE=".\april.h"
# End Source File
# End Target
# End Project
