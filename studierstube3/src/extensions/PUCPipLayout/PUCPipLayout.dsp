# Microsoft Developer Studio Project File - Name="PucPipLayout" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PucPipLayout - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PUCPipLayout.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PUCPipLayout.mak" CFG="PucPipLayout - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PucPipLayout - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PucPipLayout - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PucPipLayout - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PucPipLayout - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PucPipLayout - Win32 Release OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PUC_PIP_LAYOUT___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "PUC_PIP_LAYOUT___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PUC_PIP_LAYOUT___Win32_Release_OSOIV"
# PROP Intermediate_Dir "PUC_PIP_LAYOUT___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLLPUC_PIP_LAYOUT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_PIP_LAYOUT_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib ace.lib xerces-c_2.lib /nologo /dll /machine:I386 /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib"
# ADD LINK32 inventor.lib ace.lib xerces-c_2.lib PucPipLayoutBasicLib.lib /nologo /dll /machine:I386 /out:"$(STBROOT)/lib/PucPipLayout.dll" /implib:"$(STBROOT)/lib/PucPipLayout.lib" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib" /implib:"/PucPipLayout.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PucPipLayout - Win32 Debug OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PUC_PIP_LAYOUT___Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "PUC_PIP_LAYOUT___Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PUC_PIP_LAYOUT___Win32_Debug_OSOIV"
# PROP Intermediate_Dir "PUC_PIP_LAYOUT___Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_PUC" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_PIP_LAYOUT_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_PUC" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_PIP_LAYOUT_EXPORTS" /FR /YX /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib aced.lib xerces-c_2d.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib"
# ADD LINK32 inventord.lib aced.lib xerces-c_2d.lib PUCBasicLibd.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/PUCd.dll" /implib:"$(STBROOT)/lib/PUCd.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PucPipLayout - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PUC_PIP_LAYOUT___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "PUC_PIP_LAYOUT___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PUC_PIP_LAYOUT___Win32_Debug_Coin"
# PROP Intermediate_Dir "PUC_PIP_LAYOUT___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_PUC" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_PIP_LAYOUT_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(STBROOT)/src" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /D "HA" /D "WIN32" /D "_DEBUG_PUC" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_PIP_LAYOUT_EXPORTS" /D "USE_COIN" /D "COIN_DLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib aced.lib xerces-c_2d.lib PUCBasicLibd.lib /nologo /dll /debug /machine:I386 /out:"$(STBROOT)/lib/PUCd.dll" /implib:"$(STBROOT)/lib/PUCd.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 coin2d.lib aced.lib xerces-c_2d.lib pucd.lib stbapid.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/PUCPipLayoutd.dll" /implib:"..\..\..\lib\PUCPipLayoutd.lib" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PucPipLayout - Win32 Release Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PUC_PIP_LAYOUT___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "PUC_PIP_LAYOUT___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PUC_PIP_LAYOUT___Win32_Release_Coin"
# PROP Intermediate_Dir "PUC_PIP_LAYOUT___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_PIP_LAYOUT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(STBROOT)/src" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_PIP_LAYOUT_EXPORTS" /D "USE_COIN" /D "COIN_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib ace.lib xerces-c_2.lib PucPipLayoutBasicLib.lib /nologo /dll /machine:I386 /out:"$(STBROOT)/lib/PucPipLayout.dll" /implib:"$(STBROOT)/lib/PucPipLayout.lib" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib" /implib:"/PucPipLayout.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 coin2.lib ace.lib xerces-c_2.lib /nologo /dll /machine:I386 /out:"../../../lib/PUCPipLayout.dll" /libpath:"$(COINROOT)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib" /implib:"/PucPipLayout.lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PucPipLayout - Win32 Release OSOIV"
# Name "PucPipLayout - Win32 Debug OSOIV"
# Name "PucPipLayout - Win32 Debug Coin"
# Name "PucPipLayout - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\PucPipLayout.cxx
# End Source File
# Begin Source File

SOURCE=.\PucPipLayout.h
# End Source File
# Begin Source File

SOURCE=.\SoBuildPanelAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBuildPanelAction.h
# End Source File
# Begin Source File

SOURCE=.\SoBuildPucExtStyleAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBuildPucExtStyleAction.h
# End Source File
# Begin Source File

SOURCE=.\SoBuildPucStyleAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBuildPucStyleAction.h
# End Source File
# Begin Source File

SOURCE=.\SoBuildStbStyleAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoBuildStbStyleAction.h
# End Source File
# Begin Source File

SOURCE=.\SoPanel.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPanel.h
# End Source File
# Begin Source File

SOURCE=.\SoPanelGroup.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPanelGroup.h
# End Source File
# Begin Source File

SOURCE=.\SoPucPipLayout.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucPipLayout.h
# End Source File
# Begin Source File

SOURCE=.\SoWidgetLayoutGroup.cxx
# End Source File
# Begin Source File

SOURCE=.\SoWidgetLayoutGroup.h
# End Source File
# Begin Source File

SOURCE=.\SoWidgetSelectionFromPuc.cxx
# End Source File
# Begin Source File

SOURCE=.\SoWidgetSelectionFromPuc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
