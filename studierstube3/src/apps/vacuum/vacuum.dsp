# Microsoft Developer Studio Project File - Name="vacuum" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=vacuum - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vacuum.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vacuum.mak" CFG="vacuum - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vacuum - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vacuum - Win32 Debug TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vacuum - Win32 Release TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vacuum - Win32 Debug TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vacuum - Win32 Release TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vacuum - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vacuum - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vacuum - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vacuum - Win32 Debug OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vacuum___Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "vacuum___Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_OSOIV"
# PROP Intermediate_Dir "Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../" /I "../../" /I "d:\work\Inventor\install\include" /I "d:\work\Inventor\install\include\inventor" /I "c:\Program Files\Microsoft Visual Studio\vc98\include" /I "c:\Program Files\science\qt\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_SOQT_LIB" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I ".." /I "..\..\extras\zlib" /I "$(OTROOT)\src" /I "$(ACEROOT)" /I "C:\Program Files\Qt\include" /D "_WINDOWS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "SOUND" /D "STB_DISTRIBUTED" /D "_WINDLL" /D "_AFXDLL" /D "NT" /D "ACIS_DLL" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "SPA_NO_AUTO_LINK" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL" /d "NT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept /libpath:"d:\work\inventor\Install\Lib" /libpath:"C:\Program Files\science\Qt\Lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 qtmain.lib qt-mtedu303.lib soqt1d.lib inventord.lib opentrackerd.lib aced.lib glu32.lib opengl32.lib winmm.lib zlibd.lib user32.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\LIB" /libpath:"$(A3DT)\lib\NT_DLLD"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vacuum - Win32 Debug TGS"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vacuum___Win32_Debug_TGS"
# PROP BASE Intermediate_Dir "vacuum___Win32_Debug_TGS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_TGS"
# PROP Intermediate_Dir "Win32_Debug_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../" /I "../../" /I "d:\work\Inventor\install\include" /I "d:\work\Inventor\install\include\inventor" /I "c:\Program Files\Microsoft Visual Studio\vc98\include" /I "c:\Program Files\science\qt\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_SOQT_LIB" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(OTROOT)\src" /I ".." /I "..\..\extras\zlib" /I "..\..\extras\intersense" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_TGS" /D "SOUND" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept /libpath:"d:\work\inventor\Install\Lib" /libpath:"C:\Program Files\science\Qt\Lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vacuum - Win32 Release TGS"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vacuum___Win32_Release_TGS"
# PROP BASE Intermediate_Dir "vacuum___Win32_Release_TGS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Release_TGS"
# PROP Intermediate_Dir "Win32_Release_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "g:\stb\winstb\src\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "$(OTROOT)\src" /I ".." /I "..\..\extras\zlib" /I "..\..\extras\intersense" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_TGS" /D "SOUND" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll"
# ADD LINK32 winmm.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll"

!ELSEIF  "$(CFG)" == "vacuum - Win32 Debug TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vacuum___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "vacuum___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vacuum___Win32_Debug_TGS_with_SGI_Video"
# PROP Intermediate_Dir "vacuum___Win32_Debug_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I ".." /I "..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_TGS" /D "SOUND" /FR /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I ".." /I "..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_TGS" /D "SOUND" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vacuum - Win32 Release TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vacuum___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "vacuum___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vacuum___Win32_Release_TGS_with_SGI_Video"
# PROP Intermediate_Dir "vacuum___Win32_Release_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /I ".." /I "..\.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_TGS" /D "SOUND" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I ".." /I "..\.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_TGS" /D "SOUND" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll"
# ADD LINK32 winmm.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll"

!ELSEIF  "$(CFG)" == "vacuum - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vacuum___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "vacuum___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vacuum___Win32_Release_OSOIV"
# PROP Intermediate_Dir "vacuum___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /I "../" /I "../../" /I "d:\work\Inventor\install\include" /I "d:\work\Inventor\install\include\inventor" /I "c:\Program Files\Microsoft Visual Studio\vc98\include" /I "c:\Program Files\science\qt\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "USE_SOQT_LIB" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /O2 /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I ".." /I "..\..\extras\zlib" /I "$(OTROOT)\src" /I "$(ACEROOT)" /I "C:\Program Files\Qt\include" /D "_WINDOWS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "SOUND" /D "STB_DISTRIBUTED" /D "_DEBUG" /D "NT" /D "ACIS_DLL" /D "SPA_NO_AUTO_LINK" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept /libpath:"d:\work\inventor\Install\Lib" /libpath:"C:\Program Files\science\Qt\Lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 qtmain.lib qt-mtedu303.lib soqt1.lib inventor.lib opentracker.lib ace.lib glu32.lib opengl32.lib winmm.lib zlib.lib user32.lib /nologo /dll /incremental:yes /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\LIB" /libpath:"$(A3DT)\lib\NT_DLL"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vacuum - Win32 Debug Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vacuum___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "vacuum___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vacuum___Win32_Debug_Coin"
# PROP Intermediate_Dir "vacuum___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I ".." /I "..\.." /I "$(OTROOT)\SRC" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(MSVSHOME)\VC98\BIN\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(A3DT)\CSTR" /I "$(A3DT)\INTR" /I "$(A3DT)\KERN" /I "$(A3DT)\blnd" /I "$(A3DT)\fct" /I "$(A3DT)\BOOL" /I "$(A3DT)\abl" /I "$(A3DT)" /I "$(A3DT)\BASE" /D "$(A3DT)\lib\$(ARCH)" /D "_WINDOWS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "SOUND" /D "STB_DISTRIBUTED" /D "_WINDLL" /D "_AFXDLL" /D "NT" /D "ACIS_DLL" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# ADD CPP /nologo /G6 /MTd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../" /I "$(COINROOT)/include" /I "$(COINROOT)/include/Inventor" /I "$(QTDIR)\INCLUDE" /I "$(SIMVOLEONROOT)" /D "_WINDOWS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "SOUND" /D "USE_DIV" /D "NT" /D "ACIS_DLL" /D "USE_SOQT" /D "SPA_NO_AUTO_LINK" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "USE_COIN" /D "COIN_DLL" /D "SIMVOLEON_DLL" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL" /d "NT"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "NT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 baseutil.lib faceter.lib qtmain.lib qt-mtedu303.lib soqt2d.lib opentrackerd.lib inventord.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\LIB" /libpath:"$(A3DT)\lib\$(ARCH)"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 simvoleon1.lib coin2d.lib soqt1Coind.lib qt-mtedu303.lib qtmain.lib stbapid.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(ACEROOT)\ace" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(SIMVOLEONROOT)\VolumeViz"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "vacuum - Win32 Release Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vacuum___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "vacuum___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vacuum___Win32_Release_Coin"
# PROP Intermediate_Dir "vacuum___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /X /I ".." /I "..\.." /I "$(OTROOT)\SRC" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(MSVSHOME)\VC98\BIN\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(A3DT)\CSTR" /I "$(A3DT)\INTR" /I "$(A3DT)\KERN" /I "$(A3DT)\blnd" /I "$(A3DT)\fct" /I "$(A3DT)\BOOL" /I "$(A3DT)\abl" /I "$(A3DT)" /I "$(A3DT)\BASE" /D "$(A3DT)\lib\$(ARCH)" /D "_WINDOWS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "SOUND" /D "STB_DISTRIBUTED" /D "_DEBUG" /D "_WINDLL" /D "_AFXDLL" /D "NT" /D "ACIS_DLL" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /Zi /O2 /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../" /I "$(COINROOT)/include" /I "$(COINROOT)/include/Inventor" /I "$(QTDIR)\INCLUDE" /I "$(SIMVOLEONROOT)" /D "_WINDOWS" /D "_USRDLL" /D "VACUUM_EXPORTS" /D "SOUND" /D "SPA_NO_AUTO_LINK" /D "STB_DISTRIBUTED" /D "_DEBUG" /D "NT" /D "ACIS_DLL" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /D "USE_COIN" /D "COIN_DLL" /D "SIMVOLEON_DLL" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 baseutil.lib faceter.lib qtmain.lib qt220.lib soqt.lib opentrackerd.lib inventor.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\LIB" /libpath:"$(A3DT)\lib\$(ARCH)"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 simvoleon1.lib coin2.lib soqt1Coin.lib qt-mtedu303.lib qtmain.lib opentracker.lib ace.lib glu32.lib opengl32.lib user32.lib winmm.lib zlib.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/vacuum/vacuum_stb.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(ACEROOT)\ace" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(SIMVOLEONROOT)\VolumeViz"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "vacuum - Win32 Debug OSOIV"
# Name "vacuum - Win32 Debug TGS"
# Name "vacuum - Win32 Release TGS"
# Name "vacuum - Win32 Debug TGS with SGI Video"
# Name "vacuum - Win32 Release TGS with SGI Video"
# Name "vacuum - Win32 Release OSOIV"
# Name "vacuum - Win32 Debug Coin"
# Name "vacuum - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\settings.cpp
# End Source File
# Begin Source File

SOURCE=.\settings_moc.cpp
# End Source File
# Begin Source File

SOURCE=.\SoVacuumKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vacuum.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\settings.h
# End Source File
# Begin Source File

SOURCE=.\settings.ui.h
# End Source File
# Begin Source File

SOURCE=.\SoVacuumKit.h
# End Source File
# Begin Source File

SOURCE=.\vacuum.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\settings.ui

!IF  "$(CFG)" == "vacuum - Win32 Debug OSOIV"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "vacuum - Win32 Debug TGS"

!ELSEIF  "$(CFG)" == "vacuum - Win32 Release TGS"

!ELSEIF  "$(CFG)" == "vacuum - Win32 Debug TGS with SGI Video"

!ELSEIF  "$(CFG)" == "vacuum - Win32 Release TGS with SGI Video"

!ELSEIF  "$(CFG)" == "vacuum - Win32 Release OSOIV"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "vacuum - Win32 Debug Coin"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "vacuum - Win32 Release Coin"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
