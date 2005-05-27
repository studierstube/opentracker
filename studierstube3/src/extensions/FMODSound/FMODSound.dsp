# Microsoft Developer Studio Project File - Name="FMODSound" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FMODSound - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FMODSound.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FMODSound.mak" CFG="FMODSound - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FMODSound - Win32 Debug Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FMODSound - Win32 Release Coin" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FMODSound - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FMODSound - Win32 Release OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FMODSound - Win32 Debug OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FMODSound___Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "FMODSound___Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FMODSound___Win32_Debug_OSOIV"
# PROP Intermediate_Dir "FMODSound___Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG_FMODSound" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FMODSOUND_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FMODSOUND_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib aced.lib xerces-c_2d.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib"
# ADD LINK32 fmodvc.lib inventord.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/FMODSoundd.dll" /implib:"../../../lib/FMODSoundd.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"../../lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "FMODSound - Win32 Release OSOIV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FMODSound___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "FMODSound___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FMODSound___Win32_Release_OSOIV"
# PROP Intermediate_Dir "FMODSound___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FMODSOUND_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FMODSOUND_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib ace.lib xerces-c_2.lib /nologo /dll /machine:I386 /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib"
# ADD LINK32 fmodvc.lib inventor.lib ace.lib xerces-c_2.lib /nologo /dll /machine:I386 /out:"../../../lib/FMODSound.dll" /implib:"../../../lib/FMODSound.lib" /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(ACEROOT)/ace" /libpath:"$(XERCESCROOT)/lib" /libpath:"../../lib" /implib:"/FMODSound.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "FMODSound - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug Coin"
# PROP BASE Intermediate_Dir "Debug Coin"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Coin"
# PROP Intermediate_Dir "Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FMODSOUND_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /I "$(FMODROOT)\API\INC" /D "_WINDOWS" /D "_USRDLL" /D "FMODSOUND_EXPORTS" /D "HA" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "USE_COIN" /D "COIN_DLL" /FR /FD /GZ  /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 coin2d.lib fmodvc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/FMODSoundd.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(FMODROOT)\api\lib"

!ELSEIF  "$(CFG)" == "FMODSound - Win32 Release Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release Coin"
# PROP BASE Intermediate_Dir "Release Coin"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Coin"
# PROP Intermediate_Dir "Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FMODSOUND_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gi /GX /O2 /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(COINROOT)\INCLUDE\INVENTOR" /I "$(COINROOT)\INCLUDE" /I "$(FMODROOT)\API\INC" /D "_WINDOWS" /D "_USRDLL" /D "FMODSOUND_EXPORTS" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_COIN" /D "COIN_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 coin2.lib fmodvc.lib /nologo /dll /incremental:yes /machine:I386 /out:"../../../lib/FMODSound.dll" /libpath:"$(COINROOT)\LIB" /libpath:"$(FMODROOT)\api\lib"

!ENDIF 

# Begin Target

# Name "FMODSound - Win32 Debug OSOIV"
# Name "FMODSound - Win32 Release OSOIV"
# Name "FMODSound - Win32 Debug Coin"
# Name "FMODSound - Win32 Release Coin"
# Begin Group "source files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FMODSoundExt.cxx
# End Source File
# Begin Source File

SOURCE=.\FMODSoundExt.h
# End Source File
# Begin Source File

SOURCE=.\SoFMODSound.cxx
# End Source File
# Begin Source File

SOURCE=.\SoFMODSound.h
# End Source File
# End Group
# End Target
# End Project
