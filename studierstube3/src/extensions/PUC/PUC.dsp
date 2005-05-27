# Microsoft Developer Studio Project File - Name="PUC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PUC - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PUC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PUC.mak" CFG="PUC - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PUC - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PUC - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PUC - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PUC - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PUC - Win32 Release OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PUC___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "PUC___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PUC___Win32_Release_OSOIV"
# PROP Intermediate_Dir "PUC___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLLPUC_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib ace.lib xerces-c_2.lib /nologo /dll /machine:I386 /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib"
# ADD LINK32 inventor.lib ace.lib xerces-c_2.lib PucBasicLib.lib /nologo /dll /machine:I386 /out:"$(STBROOT)/lib/PUC.dll" /implib:"$(STBROOT)/lib/PUC.lib" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib" /implib:"/PUC.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PUC - Win32 Debug OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PUC___Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "PUC___Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PUC___Win32_Debug_OSOIV"
# PROP Intermediate_Dir "PUC___Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_PUC" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_PUC" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_EXPORTS" /FR /YX /FD /GZ /c
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

!ELSEIF  "$(CFG)" == "PUC - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PUC___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "PUC___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PUC___Win32_Debug_Coin"
# PROP Intermediate_Dir "PUC___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_PUC" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /I "../../" /D "HA" /D "WIN32" /D "_DEBUG_PUC" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_EXPORTS" /D "USE_COIN" /D "COIN_DLL" /D "USE_DIV" /FR /YX /FD /GZ /c
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
# ADD LINK32 coin2d.lib aced.lib xerces-c_2d.lib PUCBasicLibd.lib stbapid.lib /nologo /dll /incremental:no /debug /machine:I386 /out:"../../../lib/PUCd.dll" /implib:"../../../lib/PUCd.lib" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PUC - Win32 Release Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PUC___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "PUC___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PUC___Win32_Release_Coin"
# PROP Intermediate_Dir "PUC___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /I "../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PUC_EXPORTS" /D "USE_DIV" /D "USE_COIN" /D "COIN_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib ace.lib xerces-c_2.lib PucBasicLib.lib /nologo /dll /machine:I386 /out:"$(STBROOT)/lib/PUC.dll" /implib:"$(STBROOT)/lib/PUC.lib" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib" /implib:"/PUC.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 coin2.lib ace.lib xerces-c_2.lib PUCBasicLib.lib stbapi.lib /nologo /dll /machine:I386 /out:"../../../lib/PUC.dll" /implib:"../../../lib/PUC.lib" /libpath:"$(COINROOT)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(STBROOT)/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PUC - Win32 Release OSOIV"
# Name "PUC - Win32 Debug OSOIV"
# Name "PUC - Win32 Debug Coin"
# Name "PUC - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "States"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SoPucCommand.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucCommand.h
# End Source File
# Begin Source File

SOURCE=.\SoPucElementBase.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucElementBase.h
# End Source File
# Begin Source File

SOURCE=.\SoPucGroup.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucGroup.h
# End Source File
# Begin Source File

SOURCE=.\SoPucStateBase.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucStateBase.h
# End Source File
# Begin Source File

SOURCE=.\SoPucStateBool.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucStateBool.h
# End Source File
# Begin Source File

SOURCE=.\SoPucStateEnumerated.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucStateEnumerated.h
# End Source File
# Begin Source File

SOURCE=.\SoPucStateFixed.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucStateFixed.h
# End Source File
# Begin Source File

SOURCE=.\SoPucStateFloat.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucStateFloat.h
# End Source File
# Begin Source File

SOURCE=.\SoPucStateInt.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucStateInt.h
# End Source File
# Begin Source File

SOURCE=.\SoPucStateString.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucStateString.h
# End Source File
# End Group
# Begin Group "Server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SoPucDevice.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucDevice.h
# End Source File
# Begin Source File

SOURCE=.\SoPucServer.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucServer.h
# End Source File
# End Group
# Begin Group "Actions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SoGetPUCSpecificationAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoGetPUCSpecificationAction.h
# End Source File
# Begin Source File

SOURCE=.\SoPUCRequestStateAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPUCRequestStateAction.h
# End Source File
# Begin Source File

SOURCE=.\SoPUCSendFullStateAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPUCSendFullStateAction.h
# End Source File
# Begin Source File

SOURCE=.\SoPucTriggerCommandAction.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucTriggerCommandAction.h
# End Source File
# End Group
# Begin Group "ActiveIf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SoPucActiveIfBase.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucActiveIfBase.h
# End Source File
# Begin Source File

SOURCE=.\SoPucActiveIfClause.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucActiveIfClause.h
# End Source File
# Begin Source File

SOURCE=.\SoPucActiveIfNode.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucActiveIfNode.h
# End Source File
# Begin Source File

SOURCE=.\SoPucActiveIfOperator.cxx
# End Source File
# Begin Source File

SOURCE=.\SoPucActiveIfOperator.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\PUC.cxx
# End Source File
# Begin Source File

SOURCE=.\PUC.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
