# Microsoft Developer Studio Project File - Name="Kokyu" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Kokyu - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Kokyu.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Kokyu.mak" CFG="Kokyu - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Kokyu - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Kokyu - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Kokyu - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Release\Kokyu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /D KOKYU_BUILD_DLL /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\lib\Kokyu.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib /libpath:"..\lib" /nologo /version:5.4.2 /subsystem:windows /pdb:"..\lib\Kokyu.pdb" /dll  /machine:I386 /out:"..\lib\Kokyu.dll"

!ELSEIF  "$(CFG)" == "Kokyu - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Debug\Kokyu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /D KOKYU_BUILD_DLL /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\lib\Kokyu.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib /libpath:"..\lib" /nologo /version:5.4.2 /subsystem:windows /pdb:"..\lib\Kokyud.pdb" /dll /debug /machine:I386 /out:"..\lib\Kokyud.dll"

!ENDIF

# Begin Target

# Name "Kokyu - Win32 Release"
# Name "Kokyu - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Default_Dispatcher_Impl.cpp"
# End Source File
# Begin Source File

SOURCE="Dispatcher_Impl.cpp"
# End Source File
# Begin Source File

SOURCE="Dispatcher_Task.cpp"
# End Source File
# Begin Source File

SOURCE="Kokyu.cpp"
# End Source File
# Begin Source File

SOURCE="Kokyu_defs.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Default_Dispatcher_Impl.h"
# End Source File
# Begin Source File

SOURCE="Dispatcher_Impl.h"
# End Source File
# Begin Source File

SOURCE="Dispatcher_Task.h"
# End Source File
# Begin Source File

SOURCE="DSRT_Direct_Dispatcher_Impl_T.h"
# End Source File
# Begin Source File

SOURCE="DSRT_Dispatch_Item_T.h"
# End Source File
# Begin Source File

SOURCE="DSRT_Dispatcher_Impl_T.h"
# End Source File
# Begin Source File

SOURCE="DSRT_Sched_Queue_T.h"
# End Source File
# Begin Source File

SOURCE="Kokyu.h"
# End Source File
# Begin Source File

SOURCE="Kokyu_defs.h"
# End Source File
# Begin Source File

SOURCE="Kokyu_dsrt.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE="Default_Dispatcher_Impl.i"
# End Source File
# Begin Source File

SOURCE="Dispatcher_Impl.i"
# End Source File
# Begin Source File

SOURCE="Dispatcher_Task.i"
# End Source File
# Begin Source File

SOURCE="DSRT_Dispatch_Item_T.i"
# End Source File
# Begin Source File

SOURCE="DSRT_Dispatcher_Impl_T.i"
# End Source File
# Begin Source File

SOURCE="Kokyu.i"
# End Source File
# Begin Source File

SOURCE="Kokyu_defs.i"
# End Source File
# Begin Source File

SOURCE="Kokyu_dsrt.i"
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="DSRT_Direct_Dispatcher_Impl_T.cpp"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE="DSRT_Dispatch_Item_T.cpp"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE="DSRT_Dispatcher_Impl_T.cpp"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE="DSRT_Sched_Queue_T.cpp"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE="Kokyu_dsrt.cpp"
# PROP Exclude_From_Build 1
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
