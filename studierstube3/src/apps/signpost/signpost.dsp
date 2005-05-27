# Microsoft Developer Studio Project File - Name="signpost" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=signpost - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "signpost.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "signpost.mak" CFG="signpost - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "signpost - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "signpost - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "signpost - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "signpost - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "signpost - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SIGNPOST_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\include" /I "$(STBROOT)\src" /I "$(OTROOT)\src" /I "$(BOOST)" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SIGNPOST_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib inventor.lib stbapi.lib opentracker.lib qt-mtedu303.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/signpost/signpost.dll" /implib:"signpost.lib" /libpath:"$(OSOIVHOME)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(QTDIR)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "signpost - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SIGNPOST_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\include" /I "$(STBROOT)\src" /I "$(OTROOT)\src" /I "$(BOOST)" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SIGNPOST_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib inventord.lib stbapid.lib opentrackerd.lib qt-mtedu303.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/signpost/signpost.dll" /implib:"signpost.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(QTDIR)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "signpost - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "signpost___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "signpost___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "signpost___Win32_Debug_Coin"
# PROP Intermediate_Dir "signpost___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\include" /I "$(STBROOT)\src" /I "$(OTROOT)\src" /I "$(BOOST)" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SIGNPOST_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\include" /I "$(STBROOT)\src" /I "$(OTROOT)\src" /I "$(BOOST)" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SIGNPOST_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /D "COIN_DLL" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib inventord.lib stbapid.lib opentrackerd.lib qt-mtedu303.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/signpost/signpost.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(QTDIR)\lib"
# ADD LINK32 coin2d.lib stbapid.lib opentrackerd.lib qt-mtedu303.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/signpost/signpost.dll" /implib:"signpost.lib" /pdbtype:sept /libpath:"$(COINROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(QTDIR)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "signpost - Win32 Release Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "signpost___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "signpost___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "signpost___Win32_Release_Coin"
# PROP Intermediate_Dir "signpost___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /I "$(OSOIVHOME)\include" /I "$(STBROOT)\src" /I "$(BOOST)" /I "$(QTDIR)\include" /I "$(OTROOT)\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SIGNPOST_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "$(COINROOT)\include" /I "$(STBROOT)\src" /I "$(BOOST)" /I "$(QTDIR)\include" /I "$(OTROOT)\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SIGNPOST_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /D "COIN_DLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib inventor.lib stbapi.lib opentracker.lib qt-mtedu303.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/signpost/signpost.dll" /libpath:"$(OSOIVHOME)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(QTDIR)\lib"
# ADD LINK32 coin2.lib stbapi.lib opentracker.lib qt-mtedu303.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/signpost/signpost.dll" /implib:"signpost.lib" /libpath:"$(COINROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(QTDIR)\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "signpost - Win32 Release"
# Name "signpost - Win32 Debug"
# Name "signpost - Win32 Debug Coin"
# Name "signpost - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\signpost.cpp
# End Source File
# Begin Source File

SOURCE=.\SoBAUBuilding.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBAUKit.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBAUOKit.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBAURoom.cxx
# End Source File
# Begin Source File

SOURCE=.\SoInfoKit.cxx
# End Source File
# Begin Source File

SOURCE=.\SoNavigationKit.cxx
# End Source File
# Begin Source File

SOURCE=.\SoWIMKit.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\signpost.h
# End Source File
# Begin Source File

SOURCE=.\SoBAUBuilding.h
# End Source File
# Begin Source File

SOURCE=.\SoBAUKit.h
# End Source File
# Begin Source File

SOURCE=.\SoBAUOKit.h
# End Source File
# Begin Source File

SOURCE=.\SoBAURoom.h
# End Source File
# Begin Source File

SOURCE=.\SoInfoKit.h
# End Source File
# Begin Source File

SOURCE=.\SoNavigationKit.h
# End Source File
# Begin Source File

SOURCE=.\SoWIMKit.h
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
