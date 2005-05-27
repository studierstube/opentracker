# Microsoft Developer Studio Project File - Name="JAWS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=JAWS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "JAWS.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "JAWS.mak" CFG="JAWS - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "JAWS - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JAWS - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JAWS - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Release\JAWS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_BUILD_SVC_DLL /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\JAWS.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib /libpath:"..\..\..\lib" /nologo /version:5.4.2 /subsystem:windows /pdb:"..\..\..\lib\JAWS.pdb" /dll  /machine:I386 /out:"..\..\..\lib\JAWS.dll"

!ELSEIF  "$(CFG)" == "JAWS - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Debug\JAWS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_BUILD_SVC_DLL /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\JAWS.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib /libpath:"..\..\..\lib" /nologo /version:5.4.2 /subsystem:windows /pdb:"..\..\..\lib\JAWSd.pdb" /dll /debug /machine:I386 /out:"..\..\..\lib\JAWSd.dll"

!ENDIF

# Begin Target

# Name "JAWS - Win32 Release"
# Name "JAWS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="HTTP_Config.cpp"
# End Source File
# Begin Source File

SOURCE="HTTP_Handler.cpp"
# End Source File
# Begin Source File

SOURCE="HTTP_Helpers.cpp"
# End Source File
# Begin Source File

SOURCE="HTTP_Request.cpp"
# End Source File
# Begin Source File

SOURCE="HTTP_Response.cpp"
# End Source File
# Begin Source File

SOURCE="HTTP_Server.cpp"
# End Source File
# Begin Source File

SOURCE="IO.cpp"
# End Source File
# Begin Source File

SOURCE="JAWS_Concurrency.cpp"
# End Source File
# Begin Source File

SOURCE="JAWS_Pipeline.cpp"
# End Source File
# Begin Source File

SOURCE="Parse_Headers.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="HTTP_Config.h"
# End Source File
# Begin Source File

SOURCE="HTTP_Handler.h"
# End Source File
# Begin Source File

SOURCE="HTTP_Helpers.h"
# End Source File
# Begin Source File

SOURCE="HTTP_Request.h"
# End Source File
# Begin Source File

SOURCE="HTTP_Response.h"
# End Source File
# Begin Source File

SOURCE="HTTP_Server.h"
# End Source File
# Begin Source File

SOURCE="IO.h"
# End Source File
# Begin Source File

SOURCE="JAWS_Concurrency.h"
# End Source File
# Begin Source File

SOURCE="JAWS_Pipeline.h"
# End Source File
# Begin Source File

SOURCE="Parse_Headers.h"
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE="README"
# End Source File
# End Group
# End Target
# End Project
