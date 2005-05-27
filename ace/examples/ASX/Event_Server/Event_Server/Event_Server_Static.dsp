# Microsoft Developer Studio Project File - Name="Event_Server_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Event_Server_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Event_Server_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Event_Server_Static.mak" CFG="Event_Server_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Event_Server_Static - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Event_Server_Static - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Event_Server_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\Event_Server_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEs.lib /libpath:"..\..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"Static_Release\event_server.pdb"  /machine:I386 /out:"Static_Release\event_server.exe"

!ELSEIF  "$(CFG)" == "Event_Server_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\Event_Server_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEsd.lib /libpath:"..\..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"Static_Debug\event_server.pdb" /debug /machine:I386 /out:"Static_Debug\event_server.exe"

!ENDIF

# Begin Target

# Name "Event_Server_Static - Win32 Release"
# Name "Event_Server_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Consumer_Router.cpp"
# End Source File
# Begin Source File

SOURCE="Event_Analyzer.cpp"
# End Source File
# Begin Source File

SOURCE="event_server.cpp"
# End Source File
# Begin Source File

SOURCE="Options.cpp"
# End Source File
# Begin Source File

SOURCE="Peer_Router.cpp"
# End Source File
# Begin Source File

SOURCE="Supplier_Router.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Consumer_Router.h"
# End Source File
# Begin Source File

SOURCE="Event_Analyzer.h"
# End Source File
# Begin Source File

SOURCE="Options.h"
# End Source File
# Begin Source File

SOURCE="Peer_Router.h"
# End Source File
# Begin Source File

SOURCE="Supplier_Router.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE="Options.i"
# End Source File
# End Group
# End Target
# End Project
