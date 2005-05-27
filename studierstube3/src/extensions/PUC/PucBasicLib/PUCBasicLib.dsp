# Microsoft Developer Studio Project File - Name="PUCBasicLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PUCBasicLib - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PUCBasicLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PUCBasicLib.mak" CFG="PUCBasicLib - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PUCBasicLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PUCBasicLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "PUCBasicLib - Win32 Debug Coin" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PUCBasicLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "../" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../lib/PUCBasicLib.lib"

!ELSEIF  "$(CFG)" == "PUCBasicLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm- /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "../" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_DEBUG_PUC" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../lib/PUCBasicLibd.lib"

!ELSEIF  "$(CFG)" == "PUCBasicLib - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PUCBasicLib___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "PUCBasicLib___Win32_Debug_Coin"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PUCBasicLib___Win32_Debug_Coin"
# PROP Intermediate_Dir "PUCBasicLib___Win32_Debug_Coin"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm- /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "../" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_DEBUG_PUC" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm- /GX /ZI /Od /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "../" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_DEBUG_PUC" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../../../lib/PUCBasicLibd.lib"
# ADD LIB32 /nologo /out:"../../../../lib/PUCBasicLibd.lib"

!ENDIF 

# Begin Target

# Name "PUCBasicLib - Win32 Release"
# Name "PUCBasicLib - Win32 Debug"
# Name "PUCBasicLib - Win32 Debug Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DOMTreeErrorReporter.cxx
# End Source File
# Begin Source File

SOURCE=.\PUCDeviceServer.cpp
# End Source File
# Begin Source File

SOURCE=.\PUCInformationServer.cpp
# End Source File
# Begin Source File

SOURCE=.\PUCMessages.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DOMTreeErrorReporter.h
# End Source File
# Begin Source File

SOURCE=.\PUCDeviceServer.h
# End Source File
# Begin Source File

SOURCE=.\PUCInformationServer.h
# End Source File
# Begin Source File

SOURCE=.\PUCInterface.h
# End Source File
# Begin Source File

SOURCE=.\PUCMessages.h
# End Source File
# End Group
# End Target
# End Project
