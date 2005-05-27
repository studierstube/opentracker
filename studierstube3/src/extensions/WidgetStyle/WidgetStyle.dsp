# Microsoft Developer Studio Project File - Name="WidgetStyle" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=WidgetStyle - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WidgetStyle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WidgetStyle.mak" CFG="WidgetStyle - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WidgetStyle - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WidgetStyle - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WidgetStyle - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WidgetStyle - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WidgetStyle - Win32 Release OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WidgetStyle___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "WidgetStyle___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WidgetStyle___Win32_Release_OSOIV"
# PROP Intermediate_Dir "WidgetStyle___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLLWidgetStyle_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /I "../../" /D "NDEBUG" /D "WidgetStyle_EXPORTS" /D "WIDGETSTYLE_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib ace.lib xerces-c_2.lib /nologo /dll /machine:I386 /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib"
# ADD LINK32 inventor.lib ace.lib xerces-c_2.lib WidgetStyleBasicLib.lib /nologo /dll /machine:I386 /out:"$(STBROOT)/lib/WidgetStyle.dll" /implib:"$(STBROOT)/lib/WidgetStyle.lib" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib" /implib:"/WidgetStyle.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "WidgetStyle - Win32 Debug OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WidgetStyle___Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "WidgetStyle___Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WidgetStyle___Win32_Debug_OSOIV"
# PROP Intermediate_Dir "WidgetStyle___Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_WidgetStyle" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WidgetStyle_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /I "../../" /D "_DEBUG_WidgetStyle" /D "_DEBUG" /D "WidgetStyle_EXPORTS" /D "WIDGETSTYLE_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /GZ /c
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
# ADD LINK32 inventord.lib aced.lib xerces-c_2d.lib WidgetStyleBasicLibd.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/WidgetStyled.dll" /implib:"$(STBROOT)/lib/WidgetStyled.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "WidgetStyle - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WidgetStyle___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "WidgetStyle___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WidgetStyle___Win32_Debug_Coin"
# PROP Intermediate_Dir "WidgetStyle___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_WidgetStyle" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WidgetStyle_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../../" /I "../" /D "HA" /D "_DEBUG_WIDGETSTYLE" /D "_DEBUG" /D "USE_COIN" /D "COIN_DLL" /D "USE_DIV" /D "WIDGETSTYLE_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib aced.lib xerces-c_2d.lib WidgetStyleBasicLibd.lib /nologo /dll /debug /machine:I386 /out:"$(STBROOT)/lib/WidgetStyled.dll" /implib:"$(STBROOT)/lib/WidgetStyled.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 coin2d.lib stbapid.lib /nologo /dll /incremental:no /debug /machine:I386 /out:"../../../lib/WidgetStyled.dll" /implib:"../../../lib/WidgetStyled.lib" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "WidgetStyle - Win32 Release Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WidgetStyle___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "WidgetStyle___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WidgetStyle___Win32_Release_Coin"
# PROP Intermediate_Dir "WidgetStyle___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WidgetStyle_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../../" /I "../" /D "NDEBUG" /D "WIDGETSTYLE_EXPORTS" /D "USE_DIV" /D "USE_COIN" /D "COIN_DLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib ace.lib xerces-c_2.lib WidgetStyleBasicLib.lib /nologo /dll /machine:I386 /out:"$(STBROOT)/lib/WidgetStyle.dll" /implib:"$(STBROOT)/lib/WidgetStyle.lib" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib" /implib:"/WidgetStyle.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 coin2.lib stbapi.lib /nologo /dll /machine:I386 /out:"../../../lib/WidgetStyle.dll" /implib:"../../../lib/WidgetStyle.lib" /libpath:"$(COINROOT)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "WidgetStyle - Win32 Release OSOIV"
# Name "WidgetStyle - Win32 Debug OSOIV"
# Name "WidgetStyle - Win32 Debug Coin"
# Name "WidgetStyle - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SoApplyWidgetStyleAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoApplyWidgetStyleAction.h
# End Source File
# Begin Source File

SOURCE=.\SoReApplyWidgetStyleAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoReApplyWidgetStyleAction.h
# End Source File
# Begin Source File

SOURCE=.\SoStyleSwitch.cxx
# End Source File
# Begin Source File

SOURCE=.\SoStyleSwitch.h
# End Source File
# Begin Source File

SOURCE=.\SoWidgetStyleApply.cxx
# End Source File
# Begin Source File

SOURCE=.\SoWidgetStyleApply.h
# End Source File
# Begin Source File

SOURCE=.\SoWidgetStyleGroup.cxx
# End Source File
# Begin Source File

SOURCE=.\SoWidgetStyleGroup.h
# End Source File
# Begin Source File

SOURCE=.\WidgetStyle.cxx
# End Source File
# Begin Source File

SOURCE=.\WidgetStyle.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
