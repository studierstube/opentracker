# Microsoft Developer Studio Project File - Name="stbapi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=stbapi - Win32 Release Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "stbapi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "stbapi.mak" CFG="stbapi - Win32 Release Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "stbapi - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "stbapi - Win32 Debug TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "stbapi - Win32 Release TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "stbapi - Win32 Release TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "stbapi - Win32 Debug TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "stbapi - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "stbapi - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "stbapi - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "stbapi - Win32 Debug OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_OSOIV"
# PROP Intermediate_Dir "Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /X /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /I ".." /I "..\..\extras\zlib" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I ".." /I "..\..\extras\zlib" /I "$(OTROOT)\src" /I "$(ACEROOT)" /I "C:\Program Files\Qt\include" /I "$(H323ROOT)\pwlib\include\ptlib\msos" /I "$(H323ROOT)\pwlib\include\pwlib\mswin" /I "$(H323ROOT)\pwlib\include\pwlib" /I "$(H323ROOT)\pwlib\include" /I "$(H323ROOT)\openh323\include" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "SOUND" /D "OPENTRACKER_VIDEO" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "USE_DIV" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib opengl32.lib user32.lib gdi32.lib winmm.lib qtmain.lib qt220.lib soqt1.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../lib/stbapid.dll" /implib:"../../lib/stbapid.lib" /pdbtype:sept /libpath:"C:\OSOIV\Install\Lib" /libpath:"C:\Program Files\Qt\Lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 qtmain.lib qt-mtedu303.lib soqt1d.lib inventord.lib opentrackerd.lib aced.lib glu32.lib opengl32.lib winmm.lib zlibd.lib user32.lib /nologo /dll /debug /machine:I386 /out:"../../lib/stbapid.dll" /implib:"../../lib/stbapid.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(ACEROOT)\ace" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(ACEROOT)/lib" /libpath:"$(H323ROOT)\pwlib\lib" /libpath:"$(H323ROOT)\openh323\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "stbapi - Win32 Debug TGS"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug_TGS"
# PROP BASE Intermediate_Dir "Win32_Debug_TGS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_TGS"
# PROP Intermediate_Dir "Win32_Debug_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /X /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /I ".." /I "..\..\extras\zlib" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(OTROOT)\src" /I ".." /I "..\..\extras\zlib" /I "..\..\extras\intersense" /D "HA" /D "_DEBUG" /D "USE_TGS" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "WIN32" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /X /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib opengl32.lib user32.lib gdi32.lib winmm.lib qtmain.lib qt220.lib soqt1.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../lib/stbapid.dll" /implib:"../../lib/stbapid.lib" /pdbtype:sept /libpath:"C:\OSOIV\Install\Lib" /libpath:"C:\Program Files\Qt\Lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 inv260d.lib wsock32.lib glu32.lib opengl32.lib user32.lib gdi32.lib winmm.lib zlibd.lib /nologo /dll /debug /machine:I386 /out:"../../lib/stbapid.dll" /implib:"../../lib/stbapid.lib" /pdbtype:sept /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "stbapi - Win32 Release TGS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_Release_TGS"
# PROP BASE Intermediate_Dir "Win32_Release_TGS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Release_TGS"
# PROP Intermediate_Dir "Win32_Release_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GR /GX /O2 /X /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /I ".." /I "..\..\extras\zlib" /D "HAVE_TGS" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /O2 /I ".." /I "..\..\extras\zlib" /I "..\..\extras\intersense" /I "$(OTROOT)\src" /D "NDEBUG" /D "USE_TGS" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "WIN32" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /X /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib opengl32.lib user32.lib gdi32.lib winmm.lib /nologo /dll /incremental:yes /machine:I386 /out:"../../lib/stbapi.dll" /implib:"../../lib/stbapi.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 inv260.lib wsock32.lib glu32.lib opengl32.lib user32.lib gdi32.lib winmm.lib zlib.lib /nologo /dll /machine:I386 /out:"../../lib/stbapi.dll" /implib:"../../lib/stbapi.lib" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib"
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "stbapi - Win32 Release TGS with SGI Video"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "stbapi___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "stbapi___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "stbapi___Win32_Release_TGS_with_SGI_Video"
# PROP Intermediate_Dir "stbapi___Win32_Release_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GR /GX /O2 /I ".." /I "..\..\extras\zlib" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FR /FD /c
# SUBTRACT BASE CPP /X /YX
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /O2 /I ".." /I "..\..\extras\zlib" /I "..\..\extras\intersense" /I "$(OTROOT)\src" /D "NDEBUG" /D "USE_TGS" /D "SGI_DMEDIA_SDK" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "WIN32" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /X /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inv260.lib wsock32.lib opengl32.lib user32.lib gdi32.lib winmm.lib /nologo /dll /machine:I386 /out:"../../lib/stbapi.dll" /implib:"../../lib/stbapi.lib"
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 sgidmedia.lib inv260.lib wsock32.lib glu32.lib opengl32.lib user32.lib gdi32.lib winmm.lib zlib.lib /nologo /dll /machine:I386 /out:"../../lib/stbapi.dll" /implib:"../../lib/stbapi.lib" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib"
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "stbapi - Win32 Debug TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "stbapi___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "stbapi___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "stbapi___Win32_Debug_TGS_with_SGI_Video"
# PROP Intermediate_Dir "stbapi___Win32_Debug_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I ".." /I "..\..\extras\zlib" /D "HA" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /X /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I ".." /I "..\..\extras\zlib" /I "..\..\extras\intersense" /I "$(OTROOT)\src" /D "HA" /D "_DEBUG" /D "USE_TGS" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "WIN32" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /X /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inv260d.lib wsock32.lib opengl32.lib user32.lib gdi32.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"../../lib/stbapid.dll" /implib:"../../lib/stbapid.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 inv260d.lib wsock32.lib glu32.lib opengl32.lib user32.lib gdi32.lib winmm.lib zlibd.lib /nologo /dll /debug /machine:I386 /out:"../../lib/stbapid.dll" /implib:"../../lib/stbapid.lib" /pdbtype:sept /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "stbapi - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "stbapi___Win32_Release_OSOIV0"
# PROP BASE Intermediate_Dir "stbapi___Win32_Release_OSOIV0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "stbapi___Win32_Release_OSOIV0"
# PROP Intermediate_Dir "stbapi___Win32_Release_OSOIV0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I ".." /I "..\..\extras\zlib" /I "..\..\extras\intersense" /I "$(OTROOT)\src" /I "$(STBROOT)\extras\firewire" /D "HA" /D "USE_SOQT_LIB" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "WIN32" /D "_MBCS" /D "USE_OPENTRACKER" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /O2 /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I ".." /I "..\..\extras\zlib" /I "$(OTROOT)\src" /I "$(ACEROOT)" /I "$(H323ROOT)\pwlib\include\ptlib\msos" /I "$(H323ROOT)\pwlib\include\pwlib\mswin" /I "$(H323ROOT)\pwlib\include\pwlib" /I "$(H323ROOT)\pwlib\include" /I "$(H323ROOT)\openh323\include" /D "HA" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "OPENTRACKER_VIDEO" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt1.lib inventor.lib glu32.lib wsock32.lib opengl32.lib user32.lib gdi32.lib winmm.lib opentrackerd.lib /nologo /dll /debug /machine:I386 /out:"../../lib/stbapid.dll" /implib:"../../lib/stbapid.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\extras\firewire"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 qtmain.lib qt-mtedu303.lib soqt1.lib inventor.lib opentracker.lib ace.lib glu32.lib opengl32.lib winmm.lib zlib.lib user32.lib /nologo /dll /pdb:"../../lib/stbapi.pdb" /debug /machine:I386 /out:"../../lib/stbapi.dll" /implib:"../../lib/stbapi.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(ACEROOT)\ace" /libpath:"$(STBROOT)\lib" /libpath:"$(OTROOT)\lib" /libpath:"$(ACEROOT)/lib" /libpath:"$(H323ROOT)\pwlib\lib" /libpath:"$(H323ROOT)\openh323\lib"
# SUBTRACT LINK32 /pdb:none /incremental:no /map

!ELSEIF  "$(CFG)" == "stbapi - Win32 Debug Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "stbapi___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "stbapi___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "stbapi___Win32_Debug_Coin"
# PROP Intermediate_Dir "stbapi___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I ".." /I "..\..\extras\zlib" /I "$(OTROOT)\src" /I "$(ACEROOT)" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "SOUND" /D "OPENTRACKER_VIDEO" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "USE_DIV" /FR /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /Zi /Od /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I ".." /I "..\..\extras\zlib" /I "$(OTROOT)\src" /I "$(ACEROOT)" /I "$(H323ROOT)\pwlib\include\ptlib\msos" /I "$(H323ROOT)\pwlib\include\pwlib\mswin" /I "$(H323ROOT)\pwlib\include\pwlib" /I "$(H323ROOT)\pwlib\include" /I "$(H323ROOT)\openh323\include" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "SOUND" /D "OPENTRACKER_VIDEO" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "USE_COIN" /D "COIN_DLL" /D "USE_SOWIN" /D "USE_DIV" /FR /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt-mtedu303.lib soqt1d.lib inventord.lib glu32.lib opengl32.lib user32.lib gdi32.lib winmm.lib opentrackerd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"../../lib/stbapid.dll" /implib:"../../lib/stbapid.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\lib" /libpath:"$(ACEROOT)\ace"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 qt-mtedu303.lib qtmain.lib sowin1Coind.lib coin2d.lib opentrackerd.lib aced.lib glu32.lib opengl32.lib user32.lib winmm.lib zlibd.lib qt-mtedu303.lib qtmain.lib soqt1Coind.lib coin2d.lib opentrackerd.lib aced.lib glu32.lib opengl32.lib user32.lib winmm.lib zlibd.lib /nologo /dll /incremental:no /debug /machine:I386 /out:"../../lib/stbapid.dll" /implib:"../../lib/stbapid.lib" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(ACEROOT)\ace" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(ACEROOT)/lib" /libpath:"$(H323ROOT)\pwlib\lib" /libpath:"$(H323ROOT)\openh323\lib"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "stbapi - Win32 Release Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "stbapi___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "stbapi___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "stbapi___Win32_Release_Coin"
# PROP Intermediate_Dir "stbapi___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I ".." /I "..\..\extras\zlib" /I "$(OTROOT)\src" /I "$(ACEROOT)" /D "HA" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "USE_OPENTRACKER" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /Ob0 /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I ".." /I "..\..\extras\zlib" /I "$(OTROOT)\src" /I "$(ACEROOT)" /I "$(H323ROOT)\pwlib\include\ptlib\msos" /I "$(H323ROOT)\pwlib\include\pwlib\mswin" /I "$(H323ROOT)\pwlib\include\pwlib" /I "$(H323ROOT)\pwlib\include" /I "$(H323ROOT)\openh323\include" /D "HA" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "STBAPI_EXPORTS" /D "STB_VIENNA_AR" /D "SOUND" /D "USE_OPENTRACKER" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /D "USE_COIN" /D "COIN_DLL" /D "USE_DIV" /FR /FD /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt1.lib inventor.lib glu32.lib wsock32.lib opengl32.lib user32.lib gdi32.lib winmm.lib opentracker.lib ace.lib /nologo /dll /machine:I386 /out:"../../lib/stbapi.dll" /implib:"../../lib/stbapi.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\extras\firewire" /libpath:"$(ACEROOT)\ace"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 qtmain.lib qt-mtedu303.lib soqt1Coin.lib coin2.lib opentracker.lib ace.lib glu32.lib opengl32.lib zlib.lib user32.lib winmm.lib /nologo /dll /machine:I386 /out:"../../lib/stbapi.dll" /implib:"../../lib/stbapi.lib" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(ACEROOT)\ace" /libpath:"$(OTROOT)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(ACEROOT)/lib" /libpath:"$(H323ROOT)\pwlib\lib" /libpath:"$(H323ROOT)\openh323\lib"
# SUBTRACT LINK32 /pdb:none /debug

!ENDIF 

# Begin Target

# Name "stbapi - Win32 Debug OSOIV"
# Name "stbapi - Win32 Debug TGS"
# Name "stbapi - Win32 Release TGS"
# Name "stbapi - Win32 Release TGS with SGI Video"
# Name "stbapi - Win32 Debug TGS with SGI Video"
# Name "stbapi - Win32 Release OSOIV"
# Name "stbapi - Win32 Debug Coin"
# Name "stbapi - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\workspace\distrManager\AddUserMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\event3d\Base3D.cxx
# End Source File
# Begin Source File

SOURCE=.\util\buffer.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\ChangeFocusMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\ConnectionHandler.cxx
# End Source File
# Begin Source File

SOURCE=.\div\ControlModule.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\ConvertToSFPlane.cxx
# End Source File
# Begin Source File

SOURCE=.\div\divmain.cxx
# End Source File
# Begin Source File

SOURCE=.\window3d\EventAutomaton.cxx
# End Source File
# Begin Source File

SOURCE=.\util\fileBrowser.cxx
# End Source File
# Begin Source File

SOURCE=.\util\GLtools.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\GLTweak.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\H323Utils.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\IMGR.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\JoinLocaleMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\LeaveLocaleMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\LocalDistribution.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\Message.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\MessageFactory.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\MultRotRot.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\OnScreenRenderer.cxx
# End Source File
# Begin Source File

SOURCE=.\util\option.cxx
# End Source File
# Begin Source File

SOURCE=.\util\os.cxx
# End Source File
# Begin Source File

SOURCE=.\util\ReactorTask.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\RemoteDistribution.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\RemoveUserMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Ack_Worker.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Fork.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Fragment.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_IO_UDP.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Membership.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Module.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Module_Factory.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Pack.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Partial_Message.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Proxy.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Reassembly.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Receiver_Module.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Reliable_Factory.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Reordering.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Resend_Handler.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Resend_Worker.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Retransmission.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Role_Fork.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Sequencer.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Singleton_Factory.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Transceiver_Factory.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_UDP_Event_Handler.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_UDP_Proxy.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_UDP_Reliable_Transceiver.cxx
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Unpack.cxx
# End Source File
# Begin Source File

SOURCE=.\event3d\Sb3DEventHistory.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SetApplicationModeMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SetDIVParamMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SetFocusMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SheetBarMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoAbortGroup.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoAlphabetKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoAlphaMaskKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoAnaglyphDisplayMode.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\SoApplicationKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoArtifactKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoAudio.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoAxisRotateConstraint.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoBase3DButton.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoBase3DDial.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoBase3DSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoBase3DTouchSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoBoxConstraint.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoButtonPressGesture.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoButtonReleaseGesture.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoCameraControlMode.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\SoClassLoader.cxx
# End Source File
# Begin Source File

SOURCE=.\seams\SoClipCube.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoConditionalTrigger.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoConstraint.cxx
# End Source File
# Begin Source File

SOURCE=.\context\SoContext.cxx
# End Source File
# Begin Source File

SOURCE=.\context\SoContextElement.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\SoContextKit.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\SoContextManagerKit.cxx
# End Source File
# Begin Source File

SOURCE=.\context\SoContextMultiSwitch.cxx
# End Source File
# Begin Source File

SOURCE=.\context\SoContextReport.cxx
# End Source File
# Begin Source File

SOURCE=.\context\SoContextSeparator.cxx
# End Source File
# Begin Source File

SOURCE=.\context\SoContextSwitch.cxx
# End Source File
# Begin Source File

SOURCE=.\seams\SoCubeSeamKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoDesktopControlMode.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoDisplayKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoDisplayMode.cxx
# End Source File
# Begin Source File

SOURCE=.\div\SoDivGroup.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoDragKit.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoDragSeparator.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoDualChannelDisplayMode.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoEaseIn.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoEngineWrapper.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoEnterGesture.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoExitGesture.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoExtCubeKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoFactorRotation.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoFanIn.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoFieldSequentialDisplayMode.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoFileSubgraph.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoFishnet.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoFrameBuffer.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoGesture.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoGestureKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoGLFrameBufferElement.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoGLStencilBufferElement.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoGroupSwitch.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoGroupSwitchElement.cxx
# End Source File
# Begin Source File

SOURCE=.\event3d\SoHandle3DEventAction.cxx
# End Source File
# Begin Source File

SOURCE=.\event3d\SoKeyToggle.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoLabelObjectKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoLineConstraint.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoLineSequentialDisplayMode.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\SoLinkApp.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\SoLocaleKit.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\SoLocaleManagerKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoMD2NodeKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoMonoDisplayMode.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoMoveGesture.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoMultiSwitch.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoMultiSwitchElement.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoMultMatrix.cxx
# End Source File
# Begin Source File

SOURCE=.\context\SoNodeContext.cxx
# End Source File
# Begin Source File

SOURCE=.\context\SoNodeContextReport.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoNodeToName.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoOffAxisCamera.cxx
# End Source File
# Begin Source File

SOURCE=.\tracker\SoPartitionKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\SoPenKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\SoPipKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\SoPipSheetKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\SoPullDownKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoRayPicker.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoRefEngine.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoRenderSync.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoRenderSyncUDP.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoRotateFromTo.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoRotateVector.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoScreenCapture.cxx
# End Source File
# Begin Source File

SOURCE=.\seams\SoSeamKit.cxx
# End Source File
# Begin Source File

SOURCE=.\window3d\SoSizeShapeKit.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoSketchKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoSocketModule.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoSpeechGesture.cxx
# End Source File
# Begin Source File

SOURCE=.\tracker\SoStationKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoStencilBuffer.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoStereoCameraKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoStreamLine.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoStringCalculator.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoStringMap.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoStroke.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoStudierstubeViewer.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoSwitchSelect.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\SoTabKit.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoTextBox.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\SoTrackedArtifactKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoTrackedDisplayControlMode.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoTrackedViewpointControlMode.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoTrackedViewpointMobileDisplayControlMode.cxx
# End Source File
# Begin Source File

SOURCE=.\tracker\SoTrakEngine.cxx
# End Source File
# Begin Source File

SOURCE=.\util\sound.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoUse.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\SoUserKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\SoUserManagerKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoVariable.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackground.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundH323.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundIEEE1394.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundOT.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundSGI.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundUSB.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoVisibleSwitchEngine.cxx
# End Source File
# Begin Source File

SOURCE=.\window3d\SoWindowKit.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\StartApplicationMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\event3d\Stb3DEventGenerator.cxx
# End Source File
# Begin Source File

SOURCE=.\util\StbBuffer.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\StbDistrManager.cxx
# End Source File
# Begin Source File

SOURCE=.\div\stbdiv.cxx
# End Source File
# Begin Source File

SOURCE=.\event3d\StbEventInspector.cxx
# End Source File
# Begin Source File

SOURCE=.\tracker\StbInputModule.cxx
# End Source File
# Begin Source File

SOURCE=.\tracker\StbSink.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\StbSkinLoader.cxx
# End Source File
# Begin Source File

SOURCE=.\tracker\StbSource.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\StbWorkspace.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\StopApplicationMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\context\StudierstubeContext.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SystemBarMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\TaskBarMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\TCPServerClient.cxx
# End Source File
# Begin Source File

SOURCE=.\util\timerutility.cxx
# End Source File
# Begin Source File

SOURCE=.\tracker\TrackerOT.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\UseResourcesMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\UserKitMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\WindowBarMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\workspace\workspace.cxx
# End Source File
# Begin Source File

SOURCE=.\util\zpthread.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\workspace\distrManager\AddUserMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\appdef.h
# End Source File
# Begin Source File

SOURCE=.\event3d\Base3D.h
# End Source File
# Begin Source File

SOURCE=.\util\buffer.h
# End Source File
# Begin Source File

SOURCE=.\tracker\ButtonFilter.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\ChangeFocusMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\ConnectionHandler.h
# End Source File
# Begin Source File

SOURCE=.\div\ControlModule.h
# End Source File
# Begin Source File

SOURCE=.\misc\ConvertToSFPlane.h
# End Source File
# Begin Source File

SOURCE=.\workspace\debug.h
# End Source File
# Begin Source File

SOURCE=.\workspace\defines.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\DistributionStrategy.h
# End Source File
# Begin Source File

SOURCE=.\div\divmain.h
# End Source File
# Begin Source File

SOURCE=.\window3d\EventAutomaton.h
# End Source File
# Begin Source File

SOURCE=.\util\fileBrowser.h
# End Source File
# Begin Source File

SOURCE=.\util\GLtools.h
# End Source File
# Begin Source File

SOURCE=.\misc\GLTweak.h
# End Source File
# Begin Source File

SOURCE=.\misc\GRT_defs.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\H323Utils.h
# End Source File
# Begin Source File

SOURCE=.\misc\IMGR.h
# End Source File
# Begin Source File

SOURCE=.\misc\ivmacros.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\JoinLocaleMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\LeaveLocaleMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\LocalDistribution.h
# End Source File
# Begin Source File

SOURCE=.\util\math2.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\Message.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\MessageFactory.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\MultRotRot.h
# End Source File
# Begin Source File

SOURCE=.\misc\OnScreenRenderer.h
# End Source File
# Begin Source File

SOURCE=.\util\option.h
# End Source File
# Begin Source File

SOURCE=.\util\os.h
# End Source File
# Begin Source File

SOURCE=.\util\ReactorTask.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\RemoteDistribution.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\RemoveUserMessage.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Ack_Worker.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Copy_On_Write.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Fork.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Fragment.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_IO_UDP.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Membership.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Module.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Module_Factory.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Pack.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Partial_Message.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Reassembly.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Receiver_Module.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Reliable_Factory.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Reordering.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Resend_Handler.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Resend_Worker.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Retransmission.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Role_Fork.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Sequencer.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Singleton_Factory.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Transceiver_Factory.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_UDP_Event_Handler.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_UDP_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_UDP_Reliable_Transceiver.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Unpack.h
# End Source File
# Begin Source File

SOURCE=.\div\RMCast_Worker.h
# End Source File
# Begin Source File

SOURCE=.\event3d\Sb3DEventHistory.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SessionMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SessionVisitor.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SetApplicationModeMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SetDIVParamMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SetFocusMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SheetBarMessage.h
# End Source File
# Begin Source File

SOURCE=.\tracker\SingleFilter.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoAbortGroup.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoAlphabetKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoAlphaMaskKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoAnaglyphDisplayMode.h
# End Source File
# Begin Source File

SOURCE=.\workspace\SoApplicationKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoArtifactKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoAudio.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoAxisRotateConstraint.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoBoxConstraint.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoButtonPressGesture.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoButtonReleaseGesture.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoCameraControlMode.h
# End Source File
# Begin Source File

SOURCE=.\workspace\SoClassLoader.h
# End Source File
# Begin Source File

SOURCE=.\window3d\SoClientVolumeKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoConditionalTrigger.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoConstraint.h
# End Source File
# Begin Source File

SOURCE=.\context\SoContext.h
# End Source File
# Begin Source File

SOURCE=.\context\SoContextElement.h
# End Source File
# Begin Source File

SOURCE=.\workspace\SoContextKit.h
# End Source File
# Begin Source File

SOURCE=.\workspace\SoContextManagerKit.h
# End Source File
# Begin Source File

SOURCE=.\context\SoContextMultiSwitch.h
# End Source File
# Begin Source File

SOURCE=.\context\SoContextReport.h
# End Source File
# Begin Source File

SOURCE=.\context\SoContextSeparator.h
# End Source File
# Begin Source File

SOURCE=.\context\SoContextSwitch.h
# End Source File
# Begin Source File

SOURCE=.\seams\SoCubeSeamKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoDesktopControlMode.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoDial.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoDisplayKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoDisplayMode.h
# End Source File
# Begin Source File

SOURCE=.\div\SoDivGroup.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoDragKit.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoDragSeparator.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoDualChannelDisplayMode.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoEaseIn.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoEngineWrapper.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoEnterGesture.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoExitGesture.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoExtCubeKit.h
# End Source File
# Begin Source File

SOURCE=.\context\SoEyeContextElement.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoFactorRotation.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoFanIn.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoFieldSequentialDisplayMode.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoFileSubgraph.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoFishnet.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoFrameBuffer.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoGesture.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoGestureKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoGLFrameBufferElement.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoGLStencilBufferElement.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoGroupSwitch.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoGroupSwitchElement.h
# End Source File
# Begin Source File

SOURCE=.\event3d\SoHandle3DEventAction.h
# End Source File
# Begin Source File

SOURCE=.\event3d\SoKeyToggle.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoLabelObjectKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoLineConstraint.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoLineSequentialDisplayMode.h
# End Source File
# Begin Source File

SOURCE=.\workspace\SoLinkApp.h
# End Source File
# Begin Source File

SOURCE=.\workspace\SoLocaleKit.h
# End Source File
# Begin Source File

SOURCE=.\workspace\SoLocaleManagerKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoMD2NodeKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoMonoDisplayMode.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoMoveGesture.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoMultiSwitch.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoMultiSwitchElement.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoMultMatrix.h
# End Source File
# Begin Source File

SOURCE=.\context\SoNodeContext.h
# End Source File
# Begin Source File

SOURCE=.\context\SoNodeContextReport.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoNodeToName.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoOffAxisCamera.h
# End Source File
# Begin Source File

SOURCE=.\tracker\SoPartitionKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\SoPenKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\SoPipKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\SoPipSheetKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\SoPullDownKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoRayPicker.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoRefEngine.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoRenderSync.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoRenderSyncUDP.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoRotateFromTo.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoRotateVector.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoScreenCapture.h
# End Source File
# Begin Source File

SOURCE=.\seams\SoSeamKit.h
# End Source File
# Begin Source File

SOURCE=.\window3d\SoSizeShapeKit.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoSketchKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoSocketModule.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoSpeechGesture.h
# End Source File
# Begin Source File

SOURCE=.\tracker\SoStationKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoStencilBuffer.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoStereoCameraKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoStreamLine.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoStringCalculator.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoStringMap.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoStroke.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoStudierstubeViewer.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoSubBehavior.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoSwitchSelect.h
# End Source File
# Begin Source File

SOURCE=.\resource\SoTabKit.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoTextBox.h
# End Source File
# Begin Source File

SOURCE=.\resource\SoTrackedArtifactKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoTrackedDisplayControlMode.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoTrackedViewpointControlMode.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\SoTrackedViewpointMobileDisplayControlMode.h
# End Source File
# Begin Source File

SOURCE=.\tracker\SoTrakEngine.h
# End Source File
# Begin Source File

SOURCE=.\util\sound.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoUse.h
# End Source File
# Begin Source File

SOURCE=.\resource\SoUserKit.h
# End Source File
# Begin Source File

SOURCE=.\resource\SoUserManagerKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoVariable.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackground.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundH323.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundIEEE1394.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundOT.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundSGI.h
# End Source File
# Begin Source File

SOURCE=.\resource\viewer\video\SoVideoBackgroundUSB.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoVisibleSwitchEngine.h
# End Source File
# Begin Source File

SOURCE=.\window3d\SoWindowKit.h
# End Source File
# Begin Source File

SOURCE=.\util\stack.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\StartApplicationMessage.h
# End Source File
# Begin Source File

SOURCE=.\event3d\Stb3DEventGenerator.h
# End Source File
# Begin Source File

SOURCE=.\stbapi.h
# End Source File
# Begin Source File

SOURCE=.\util\StbBuffer.h
# End Source File
# Begin Source File

SOURCE=.\viewer\StbCameraControlMode.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\StbDistrManager.h
# End Source File
# Begin Source File

SOURCE=.\div\stbdiv.h
# End Source File
# Begin Source File

SOURCE=.\event3d\StbEventInspector.h
# End Source File
# Begin Source File

SOURCE=.\tracker\StbInputModule.h
# End Source File
# Begin Source File

SOURCE=.\tracker\StbKeyboardSource.h
# End Source File
# Begin Source File

SOURCE=.\tracker\StbMouseSource.h
# End Source File
# Begin Source File

SOURCE=.\workspace\StbResManager.h
# End Source File
# Begin Source File

SOURCE=.\tracker\STBSink.h
# End Source File
# Begin Source File

SOURCE=.\resource\StbSkinLoader.h
# End Source File
# Begin Source File

SOURCE=.\tracker\StbSource.h
# End Source File
# Begin Source File

SOURCE=.\workspace\StbWorkspace.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\StopApplicationMessage.h
# End Source File
# Begin Source File

SOURCE=.\context\StudierstubeContext.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\SystemBarMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\TaskBarMessage.h
# End Source File
# Begin Source File

SOURCE=.\misc\TCPServerClient.h
# End Source File
# Begin Source File

SOURCE=.\util\timerutility.h
# End Source File
# Begin Source File

SOURCE=.\tracker\TrackerOT.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\UseResourcesMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\UserKitMessage.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\UserKitMessageVisitor.h
# End Source File
# Begin Source File

SOURCE=.\workspace\version.h
# End Source File
# Begin Source File

SOURCE=.\workspace\windef.h
# End Source File
# Begin Source File

SOURCE=.\workspace\distrManager\WindowBarMessage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "ivutil"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\util\ivio.cxx
# End Source File
# Begin Source File

SOURCE=.\util\ivio.h
# End Source File
# Begin Source File

SOURCE=.\util\ivutil.cxx
# End Source File
# Begin Source File

SOURCE=.\util\ivutil.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoRoute.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoRoute.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoRouteDB.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoRouteDB.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoRouter.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoRouter.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoSFFieldContainer.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoSFFieldContainer.h
# End Source File
# End Group
# Begin Group "widgets"

# PROP Default_Filter ""
# Begin Group "Old"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\widgets\So3DButton.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DButton.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DCheckBox.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DDial.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DDial.h
# End Source File
# Begin Source File

SOURCE=.\event3d\So3DEvent.cxx
# End Source File
# Begin Source File

SOURCE=.\event3d\So3DEvent.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DExpSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DExpSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DIntDial.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DIntDial.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DIntSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DIntSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DLogSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DLogSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DRadioButton.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DRadioButton.h
# End Source File
# Begin Source File

SOURCE=.\interaction\So3DSensor.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\So3DSensor.h
# End Source File
# Begin Source File

SOURCE=.\interaction\So3DSeparator.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\So3DSeparator.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DTouchButton.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DTouchButton.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DTouchCheckBox.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DTouchCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DTouchRadioButton.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DTouchRadioButton.h
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DTouchSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\So3DTouchSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoBase3DButton.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoBase3DDial.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoBase3DSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoBase3DTouchSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoListBox.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoListBox.h
# End Source File
# Begin Source File

SOURCE=.\resource\SoTabButtonKit.cxx
# End Source File
# Begin Source File

SOURCE=.\resource\SoTabButtonKit.h
# End Source File
# End Group
# Begin Group "Buttons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\widgets\SoButtonKit.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoButtonKit.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoPushButton.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoPushButton.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoRadioButton.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoRadioButton.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoRadioButtonGroup.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoRadioButtonGroup.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoToggleButton.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoToggleButton.h
# End Source File
# End Group
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\interaction\SoBehaviorKit.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoBehaviorKit.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoLabelKit.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoLabelKit.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoLayoutKit.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoLayoutKit.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoTooltipKit.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoTooltipKit.h
# End Source File
# End Group
# Begin Group "Slider"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\interaction\SoDraggerKit.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoDraggerKit.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoIncrementalSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoIncrementalSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoSimpleSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoSimpleSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoSlider.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoSpeechSlider.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoSpeechSlider.h
# End Source File
# End Group
# Begin Group "ListBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\widgets\SoLabelListBox.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoLabelListBox.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoTextListBox.cxx
# End Source File
# Begin Source File

SOURCE=.\widgets\SoTextListBox.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\misc\SoBillboardKit.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoBillboardKit.h
# End Source File
# Begin Source File

SOURCE=.\widgets\SoDial.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoTouchButtonDragKit.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoTouchButtonDragKit.h
# End Source File
# Begin Source File

SOURCE=.\interaction\SoTouchDragKit.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoTouchDragKit.h
# End Source File
# End Group
# Begin Group "interaction"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\interaction\SoFitInKit.cxx
# End Source File
# Begin Source File

SOURCE=.\interaction\SoFitInKit.h
# End Source File
# Begin Source File

SOURCE=.\misc\SoRayPickerTracker.cxx
# End Source File
# Begin Source File

SOURCE=.\misc\SoRayPickerTracker.h
# End Source File
# End Group
# Begin Group "Virtual Cockpit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vc\globaldef.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCBarKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCBarKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCCalibrationWidgetKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCCalibrationWidgetKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCDialWidgetKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCDialWidgetKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCDraggerKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCDraggerKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCMainPanelKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCMainPanelKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCManager.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCManager.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCPushWidgetKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCPushWidgetKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCStatusBarKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCStatusBarKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCSubPanelKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCSubPanelKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCToggleWidgetKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCToggleWidgetKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCWidgetKit.cxx
# End Source File
# Begin Source File

SOURCE=.\vc\SoVCWidgetKit.h
# End Source File
# Begin Source File

SOURCE=.\vc\vcmainpage.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
