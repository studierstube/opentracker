# Microsoft Developer Studio Project File - Name="Svc_Cfg_IPC_Server_Test_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Svc_Cfg_IPC_Server_Test_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Svc_Cfg_IPC_Server_Test_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Svc_Cfg_IPC_Server_Test_Static.mak" CFG="Svc_Cfg_IPC_Server_Test_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Svc_Cfg_IPC_Server_Test_Static - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Svc_Cfg_IPC_Server_Test_Static - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Svc_Cfg_IPC_Server_Test_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\Svc_Cfg_IPC_Server_Test_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO IPC_Tests_Servers.lib ACEs.lib /libpath:"..\..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"Static_Release\server_test.pdb"  /machine:I386 /out:"Static_Release\server_test.exe"

!ELSEIF  "$(CFG)" == "Svc_Cfg_IPC_Server_Test_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\Svc_Cfg_IPC_Server_Test_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO IPC_Tests_Serversd.lib ACEsd.lib /libpath:"..\..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"Static_Debug\server_test.pdb" /debug /machine:I386 /out:"Static_Debug\server_test.exe"

!ENDIF

# Begin Target

# Name "Svc_Cfg_IPC_Server_Test_Static - Win32 Release"
# Name "Svc_Cfg_IPC_Server_Test_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="server_test.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Handle_Broadcast.h"
# End Source File
# Begin Source File

SOURCE="Handle_L_CODgram.h"
# End Source File
# Begin Source File

SOURCE="Handle_L_Dgram.h"
# End Source File
# Begin Source File

SOURCE="Handle_L_FIFO.h"
# End Source File
# Begin Source File

SOURCE="Handle_L_Pipe.h"
# End Source File
# Begin Source File

SOURCE="Handle_L_SPIPE.h"
# End Source File
# Begin Source File

SOURCE="Handle_L_Stream.h"
# End Source File
# Begin Source File

SOURCE="Handle_R_Dgram.h"
# End Source File
# Begin Source File

SOURCE="Handle_R_Stream.h"
# End Source File
# Begin Source File

SOURCE="Handle_Thr_Stream.h"
# End Source File
# Begin Source File

SOURCE="Handle_Timeout.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE="Handle_Broadcast.i"
# End Source File
# Begin Source File

SOURCE="Handle_L_CODgram.i"
# End Source File
# Begin Source File

SOURCE="Handle_L_Dgram.i"
# End Source File
# Begin Source File

SOURCE="Handle_L_FIFO.i"
# End Source File
# Begin Source File

SOURCE="Handle_L_Pipe.i"
# End Source File
# Begin Source File

SOURCE="Handle_L_SPIPE.i"
# End Source File
# Begin Source File

SOURCE="Handle_L_Stream.i"
# End Source File
# Begin Source File

SOURCE="Handle_R_Dgram.i"
# End Source File
# Begin Source File

SOURCE="Handle_R_Stream.i"
# End Source File
# Begin Source File

SOURCE="Handle_Thr_Stream.i"
# End Source File
# Begin Source File

SOURCE="Handle_Timeout.i"
# End Source File
# End Group
# End Target
# End Project
