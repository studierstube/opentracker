# Microsoft Developer Studio Project File - Name="JAWS3_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=JAWS3_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "JAWS3_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "JAWS3_Static.mak" CFG="JAWS3_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "JAWS3_Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "JAWS3_Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JAWS3_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\JAWS3_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\.." /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\.." /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\JAWS3.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\..\lib\JAWS3s.lib"

!ELSEIF  "$(CFG)" == "JAWS3_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\JAWS3_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\.." /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\.." /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\JAWS3.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\..\lib\JAWS3sd.lib"

!ENDIF

# Begin Target

# Name "JAWS3_Static - Win32 Release"
# Name "JAWS3_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Asynch_IO.cpp"
# End Source File
# Begin Source File

SOURCE="Concurrency.cpp"
# End Source File
# Begin Source File

SOURCE="Config_File.cpp"
# End Source File
# Begin Source File

SOURCE="Datagram.cpp"
# End Source File
# Begin Source File

SOURCE="Event_Completer.cpp"
# End Source File
# Begin Source File

SOURCE="Event_Dispatcher.cpp"
# End Source File
# Begin Source File

SOURCE="FILE.cpp"
# End Source File
# Begin Source File

SOURCE="IO.cpp"
# End Source File
# Begin Source File

SOURCE="Options.cpp"
# End Source File
# Begin Source File

SOURCE="Protocol_Handler.cpp"
# End Source File
# Begin Source File

SOURCE="Reactive_IO.cpp"
# End Source File
# Begin Source File

SOURCE="Signal_Task.cpp"
# End Source File
# Begin Source File

SOURCE="Symbol_Table.cpp"
# End Source File
# Begin Source File

SOURCE="Synch_IO.cpp"
# End Source File
# Begin Source File

SOURCE="Task_Timer.cpp"
# End Source File
# Begin Source File

SOURCE="Templates.cpp"
# End Source File
# Begin Source File

SOURCE="THYBRID_Concurrency.cpp"
# End Source File
# Begin Source File

SOURCE="Timer.cpp"
# End Source File
# Begin Source File

SOURCE="Timer_Helpers.cpp"
# End Source File
# Begin Source File

SOURCE="TPOOL_Concurrency.cpp"
# End Source File
# Begin Source File

SOURCE="TPR_Concurrency.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Asynch_IO.h"
# End Source File
# Begin Source File

SOURCE="Asynch_IO_Helpers.h"
# End Source File
# Begin Source File

SOURCE="Cached_Allocator_T.h"
# End Source File
# Begin Source File

SOURCE="Concurrency.h"
# End Source File
# Begin Source File

SOURCE="Concurrency_T.h"
# End Source File
# Begin Source File

SOURCE="Config_File.h"
# End Source File
# Begin Source File

SOURCE="Datagram.h"
# End Source File
# Begin Source File

SOURCE="Event_Completer.h"
# End Source File
# Begin Source File

SOURCE="Event_Dispatcher.h"
# End Source File
# Begin Source File

SOURCE="Event_Result.h"
# End Source File
# Begin Source File

SOURCE="Export.h"
# End Source File
# Begin Source File

SOURCE="FILE.h"
# End Source File
# Begin Source File

SOURCE="IO.h"
# End Source File
# Begin Source File

SOURCE="Options.h"
# End Source File
# Begin Source File

SOURCE="Protocol_Handler.h"
# End Source File
# Begin Source File

SOURCE="Reactive_IO.h"
# End Source File
# Begin Source File

SOURCE="Reactive_IO_Helpers.h"
# End Source File
# Begin Source File

SOURCE="Signal_Task.h"
# End Source File
# Begin Source File

SOURCE="Symbol_Table.h"
# End Source File
# Begin Source File

SOURCE="Synch_IO.h"
# End Source File
# Begin Source File

SOURCE="Task_Timer.h"
# End Source File
# Begin Source File

SOURCE="THYBRID_Concurrency.h"
# End Source File
# Begin Source File

SOURCE="Timer.h"
# End Source File
# Begin Source File

SOURCE="Timer_Helpers.h"
# End Source File
# Begin Source File

SOURCE="TPOOL_Concurrency.h"
# End Source File
# Begin Source File

SOURCE="TPR_Concurrency.h"
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Cached_Allocator_T.cpp"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE="Concurrency_T.cpp"
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
