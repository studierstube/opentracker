# Microsoft Developer Studio Project File - Name="animal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=animal - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "animal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "animal.mak" CFG="animal - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "animal - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animal - Win32 Debug TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animal - Win32 Release TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animal - Win32 Release TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animal - Win32 Debug TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animal - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animal - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animal - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "animal - Win32 Debug OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "animal___Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "animal___Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_OSOIV"
# PROP Intermediate_Dir "Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /X /I "../../" /I "C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" /I "C:\OSOIV\Install\lnclude" /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /pdbtype:sept /libpath:"C:\OSOIV\Install\Lib" /libpath:"C:\Program Files\Qt\Lib"
# ADD LINK32 qtmain.lib qt-mtedu303.lib soqt1d.lib inventord.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "animal - Win32 Debug TGS"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "animal___Win32_Debug_TGS"
# PROP BASE Intermediate_Dir "animal___Win32_Debug_TGS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_TGS"
# PROP Intermediate_Dir "Win32_Debug_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /X /I "../../" /I "C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" /I "C:\OSOIV\Install\lnclude" /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt1.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /pdbtype:sept /libpath:"C:\OSOIV\Install\Lib" /libpath:"C:\Program Files\Qt\Lib"
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "animal - Win32 Release TGS"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "animal___Win32_Release_TGS"
# PROP BASE Intermediate_Dir "animal___Win32_Release_TGS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Release_TGS"
# PROP Intermediate_Dir "Win32_Release_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /X /I "../../" /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /I "C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" /I "C:\OSOIV\Install\lnclude" /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll"
# ADD LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll"

!ELSEIF  "$(CFG)" == "animal - Win32 Release TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "animal___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "animal___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "animal___Win32_Release_TGS_with_SGI_Video"
# PROP Intermediate_Dir "animal___Win32_Release_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll"
# ADD LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll"

!ELSEIF  "$(CFG)" == "animal - Win32 Debug TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "animal___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "animal___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "animal___Win32_Debug_TGS_with_SGI_Video"
# PROP Intermediate_Dir "animal___Win32_Debug_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /pdbtype:sept
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "animal - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "animal___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "animal___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "animal___Win32_Release_OSOIV"
# PROP Intermediate_Dir "animal___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /I "../../" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll"
# ADD LINK32 inventor.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "animal - Win32 Debug Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "animal___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "animal___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "animal___Win32_Debug_Coin"
# PROP Intermediate_Dir "animal___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "COIN_DLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt-mtedu303.lib soqt2d.lib inventord.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qt-mtedu303.lib coin2d.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "animal - Win32 Release Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "animal___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "animal___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "animal___Win32_Release_Coin"
# PROP Intermediate_Dir "animal___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /I "../../" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ANIMAL_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /D "COIN_DLL" /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qt-mtedu303.lib coin2.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/animal/animal_stb.dll" /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB"

!ENDIF 

# Begin Target

# Name "animal - Win32 Debug OSOIV"
# Name "animal - Win32 Debug TGS"
# Name "animal - Win32 Release TGS"
# Name "animal - Win32 Release TGS with SGI Video"
# Name "animal - Win32 Debug TGS with SGI Video"
# Name "animal - Win32 Release OSOIV"
# Name "animal - Win32 Debug Coin"
# Name "animal - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\animal.cpp
# End Source File
# Begin Source File

SOURCE=.\SoAnimalKit.cxx
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\animal.h
# End Source File
# Begin Source File

SOURCE=.\SoAnimalKit.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
