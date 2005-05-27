# Microsoft Developer Studio Project File - Name="StbSpeech" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=StbSpeech - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StbSpeech.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StbSpeech.mak" CFG="StbSpeech - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StbSpeech - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StbSpeech - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StbSpeech - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StbSpeech - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StbSpeech - Win32 Release OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "StbSpeech___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "StbSpeech___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "StbSpeech___Win32_Release_OSOIV"
# PROP Intermediate_Dir "StbSpeech___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STBSPEECH_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STBSPEECH_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib ace.lib xerces-c_2.lib /nologo /dll /machine:I386 /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib"
# ADD LINK32 inventor.lib ace.lib xerces-c_2.lib /nologo /dll /machine:I386 /out:"../../../lib/StbSpeech.dll" /implib:"../../../lib/StbSpeech.lib" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"../../lib" /implib:"/StbSpeech.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "StbSpeech - Win32 Debug OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "StbSpeech___Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "StbSpeech___Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "StbSpeech___Win32_Debug_OSOIV"
# PROP Intermediate_Dir "StbSpeech___Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_StbSpeech" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STBSPEECH_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STBSPEECH_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib aced.lib xerces-c_2d.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib"
# ADD LINK32 inventord.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/StbSpeechd.dll" /implib:"../../../lib/StbSpeechd.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "StbSpeech - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "StbSpeech___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "StbSpeech___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "StbSpeech___Win32_Debug_Coin"
# PROP Intermediate_Dir "StbSpeech___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_StbSpeech" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STBSPEECH_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(COINROOT)\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "../" /D "HA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STBSPEECH_EXPORTS" /D "USE_COIN" /D "COIN_DLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib aced.lib xerces-c_2d.lib StbSpeechBasicLibd.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/StbSpeechd.dll" /implib:"../../../lib/StbSpeechd.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"../../lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 coin2d.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/StbSpeechd.dll" /implib:"../../../lib/StbSpeechd.lib" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "StbSpeech - Win32 Release Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "StbSpeech___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "StbSpeech___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "StbSpeech___Win32_Release_Coin"
# PROP Intermediate_Dir "StbSpeech___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STBSPEECH_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STBSPEECH_EXPORTS" /D "USE_COIN" /D "COIN_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib ace.lib xerces-c_2.lib PucBasicLib.lib /nologo /dll /machine:I386 /out:"../../../lib/StbSpeech.dll" /implib:"../../../lib/StbSpeech.lib" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"../../lib" /implib:"/StbSpeech.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 coin2.lib /nologo /dll /machine:I386 /out:"../../../lib/StbSpeech.dll" /implib:"../../../lib/StbSpeech.lib" /libpath:"$(COINROOT)\LIB" /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "StbSpeech - Win32 Release OSOIV"
# Name "StbSpeech - Win32 Debug OSOIV"
# Name "StbSpeech - Win32 Debug Coin"
# Name "StbSpeech - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EmptySpeechImpl.cxx
# End Source File
# Begin Source File

SOURCE=.\EmptySpeechImpl.h
# End Source File
# Begin Source File

SOURCE=.\SAPISpeechImpl.cxx
# End Source File
# Begin Source File

SOURCE=.\SAPISpeechImpl.h
# End Source File
# Begin Source File

SOURCE=.\SoEmotionaliseSpeechText.cxx
# End Source File
# Begin Source File

SOURCE=.\SoEmotionaliseSpeechText.h
# End Source File
# Begin Source File

SOURCE=.\SoSpeechOutput.cxx
# End Source File
# Begin Source File

SOURCE=.\SoSpeechOutput.h
# End Source File
# Begin Source File

SOURCE=.\StbSpeech.cxx
# End Source File
# Begin Source File

SOURCE=.\StbSpeech.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
