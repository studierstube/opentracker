# Microsoft Developer Studio Project File - Name="tabletop" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TABLETOP - WIN32 DEBUG OSOIV
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tabletop.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tabletop.mak" CFG="TABLETOP - WIN32 DEBUG OSOIV"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tabletop - Win32 Release TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tabletop - Win32 Debug TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tabletop - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tabletop - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tabletop - Win32 Release TGS"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TABLETOP_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(STBROOT)\src" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "TABLETOP_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
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
# ADD LINK32 winmm.lib /nologo /dll /machine:I386 /out:"tabletop_stb.dll" /libpath:"$(STBROOT)\lib"

!ELSEIF  "$(CFG)" == "tabletop - Win32 Debug TGS"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TABLETOP_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "$(STBROOT)\src" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "TABLETOP_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
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
# ADD LINK32 winmm.lib /nologo /dll /debug /machine:I386 /out:"tabletop_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib"

!ELSEIF  "$(CFG)" == "tabletop - Win32 Debug OSOIV"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "TABLETOP_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(OTROOT)\src" /I "$(STBROOT)\src" /D "_WINDOWS" /D "_USRDLL" /D "TABLETOP_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /D "USE_OPENTRACKER" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/tabletop/tabletop_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt220.lib soqtd.lib inventord.lib stbapid.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/tabletop/tabletop_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib"

!ELSEIF  "$(CFG)" == "tabletop - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tabletop___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "tabletop___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "tabletop___Win32_Release_OSOIV"
# PROP Intermediate_Dir "tabletop___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "TABLETOP_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(STBROOT)\src" /D "_WINDOWS" /D "_USRDLL" /D "TABLETOP_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/tabletop/tabletop_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib stbapi.lib winmm.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/tabletop/tabletop_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(STBROOT)\lib"

!ENDIF 

# Begin Target

# Name "tabletop - Win32 Release TGS"
# Name "tabletop - Win32 Debug TGS"
# Name "tabletop - Win32 Debug OSOIV"
# Name "tabletop - Win32 Release OSOIV"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Convert.cxx
# End Source File
# Begin Source File

SOURCE=.\ReadConfigFile.cxx
# End Source File
# Begin Source File

SOURCE=.\SoTableTopKit.cxx
# End Source File
# Begin Source File

SOURCE=.\tabletop.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Convert.h
# End Source File
# Begin Source File

SOURCE=.\ReadConfigFile.h
# End Source File
# Begin Source File

SOURCE=.\SoTableTopKit.h
# End Source File
# Begin Source File

SOURCE=".\tabletop.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
