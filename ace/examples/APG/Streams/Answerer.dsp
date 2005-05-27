# Microsoft Developer Studio Project File - Name="Answerer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Answerer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Answerer.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Answerer.mak" CFG="Answerer - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Answerer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Answerer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Answerer - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\Answerer"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib /libpath:"..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"Release\Answerer.pdb"  /machine:I386 /out:"Release\Answerer.exe"

!ELSEIF  "$(CFG)" == "Answerer - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\Answerer"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib /libpath:"..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:".\Answerer.pdb" /debug /machine:I386 /out:".\Answerer.exe"

!ENDIF

# Begin Target

# Name "Answerer - Win32 Release"
# Name "Answerer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Answerer.cpp"
# End Source File
# Begin Source File

SOURCE="CommandModule.cpp"
# End Source File
# Begin Source File

SOURCE="CommandStream.cpp"
# End Source File
# Begin Source File

SOURCE="CommandTask.cpp"
# End Source File
# Begin Source File

SOURCE="CommandTasks.cpp"
# End Source File
# Begin Source File

SOURCE="RecordingDevice_Text.cpp"
# End Source File
# Begin Source File

SOURCE="RecordingDeviceFactory.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="CommandModule.h"
# End Source File
# Begin Source File

SOURCE="CommandStream.h"
# End Source File
# Begin Source File

SOURCE="CommandTask.h"
# End Source File
# Begin Source File

SOURCE="CommandTasks.h"
# End Source File
# Begin Source File

SOURCE="RecordingDevice_Text.h"
# End Source File
# Begin Source File

SOURCE="RecordingDeviceFactory.h"
# End Source File
# End Group
# End Target
# End Project
