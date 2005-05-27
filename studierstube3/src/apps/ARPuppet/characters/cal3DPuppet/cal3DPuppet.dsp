# Microsoft Developer Studio Project File - Name="cal3DPuppet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cal3DPuppet - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cal3DPuppet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cal3DPuppet.mak" CFG="cal3DPuppet - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cal3DPuppet - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cal3DPuppet - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cal3DPuppet - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cal3DPuppet - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cal3DPuppet - Win32 Release OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "Win32_Release_OSOIV"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Release_OSOIV"
# PROP Intermediate_Dir "Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3DPUPPET_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /X /I "$(OSOIVHOME)/include" /I "$(OSOIVHOME)/include/Inventor" /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../../../" /I "$(QTDIR)\INCLUDE" /I "$(CAL3DROOT)/src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3DPUPPET_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 arpuppet.lib cal3d.lib inventor.lib stbapi.lib qt-mtedu303.lib qtmain.lib soqt1.lib opengl32.lib /nologo /dll /machine:I386 /out:"../../../../../bin/apps/arpuppet/characters/cal3DPuppet/cal3DPuppet.dll" /libpath:"$(OSOIVHOME)/lib" /libpath:"$(STBROOT)/lib" /libpath:"$(QTDIR)\lib" /libpath:"$(CAL3DROOT)/bin/Release" /libpath:"$(STBROOT)/src/apps/ARPuppet/characters/lib"

!ELSEIF  "$(CFG)" == "cal3DPuppet - Win32 Debug OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "Win32_Debug_OSOIV"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_OSOIV"
# PROP Intermediate_Dir "Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3DPUPPET_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /X /I "$(OSOIVHOME)/include" /I "$(OSOIVHOME)/include/Inventor" /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../../../" /I "$(QTDIR)\INCLUDE" /I "$(CAL3DROOT)/src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3DPUPPET_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 arpuppetd.lib cal3d_d.lib inventord.lib stbapid.lib qt-mtedu303.lib qtmain.lib soqt1d.lib opengl32.lib /nologo /dll /debug /machine:I386 /out:"../../../../../bin/apps/arpuppet/characters/cal3DPuppet/cal3DPuppetd.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)/lib" /libpath:"$(STBROOT)/lib" /libpath:"$(QTDIR)\lib" /libpath:"$(CAL3DROOT)/bin/Debug" /libpath:"$(STBROOT)/src/apps/ARPuppet/characters/lib"

!ELSEIF  "$(CFG)" == "cal3DPuppet - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "cal3DPuppet___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "cal3DPuppet___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "cal3DPuppet___Win32_Debug_Coin"
# PROP Intermediate_Dir "cal3DPuppet___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../" /I "$(OSOIVHOME)/include" /I "$(OSOIVHOME)/include/Inventor" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3DPUPPET_EXPORTS" /D "USE_QT" /D "USE_DIV" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /X /I "$(COINROOT)/include" /I "$(COINROOT)/include/Inventor" /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../../../" /I "$(QTDIR)\INCLUDE" /I "$(CAL3DROOT)/src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3DPUPPET_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /D "COIN_DLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib stbapid.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/cal3DPuppet/cal3DPuppet.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)/lib" /libpath:"$(STBROOT)/lib"
# ADD LINK32 arpuppetd.lib cal3d_d.lib coin2d.lib stbapid.lib soqt1Coind.lib qt-mtedu303.lib qtmain.lib opengl32.lib /nologo /dll /debug /machine:I386 /out:"../../../../../bin/apps/arpuppet/characters/cal3DPuppet/cal3DPuppetd.dll" /pdbtype:sept /libpath:"$(COINROOT)/lib" /libpath:"$(STBROOT)/lib" /libpath:"$(QTDIR)\lib" /libpath:"$(CAL3DROOT)/bin/Debug" /libpath:"$(STBROOT)/src/apps/ARPuppet/characters/lib"

!ELSEIF  "$(CFG)" == "cal3DPuppet - Win32 Release Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cal3DPuppet___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "cal3DPuppet___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "cal3DPuppet___Win32_Release_Coin"
# PROP Intermediate_Dir "cal3DPuppet___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../" /I "$(OSOIVHOME)/include" /I "$(OSOIVHOME)/include/Inventor" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3DPUPPET_EXPORTS" /D "USE_QT" /D "USE_DIV" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /X /I "$(COINROOT)/include" /I "$(COINROOT)/include/Inventor" /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../../../" /I "$(QTDIR)\INCLUDE" /I "$(CAL3DROOT)/src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3DPUPPET_EXPORTS" /D "USE_SOQT" /D "USE_DIV" /D "COIN_DLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib stbapi.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/cal3DPuppet/cal3DPuppet.dll" /libpath:"$(OSOIVHOME)/lib" /libpath:"$(STBROOT)/lib"
# ADD LINK32 arpuppet.lib cal3d.lib coin2.lib stbapi.lib soqt1Coin.lib qt-mtedu303.lib qtmain.lib opengl32.lib /nologo /dll /machine:I386 /out:"../../../../../bin/apps/arpuppet/characters/cal3DPuppet/cal3DPuppet.dll" /libpath:"$(COINROOT)/lib" /libpath:"$(STBROOT)/lib" /libpath:"$(QTDIR)\lib" /libpath:"$(CAL3DROOT)/bin/Release" /libpath:"$(STBROOT)/src/apps/ARPuppet/characters/lib"

!ENDIF 

# Begin Target

# Name "cal3DPuppet - Win32 Release OSOIV"
# Name "cal3DPuppet - Win32 Debug OSOIV"
# Name "cal3DPuppet - Win32 Debug Coin"
# Name "cal3DPuppet - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cal3DPuppet.cpp
# End Source File
# Begin Source File

SOURCE=.\SoCal3DPuppet.cxx
# End Source File
# Begin Source File

SOURCE=.\SoCal3DPuppeteer.cxx
# End Source File
# Begin Source File

SOURCE=.\tga.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cal3DPuppet.h
# End Source File
# Begin Source File

SOURCE=.\SoCal3DPuppet.h
# End Source File
# Begin Source File

SOURCE=.\SoCal3DPuppeteer.h
# End Source File
# Begin Source File

SOURCE=.\tga.h
# End Source File
# End Group
# End Target
# End Project
