# Microsoft Developer Studio Project File - Name="netsvcs_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=netsvcs_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "netsvcs_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "netsvcs_Static.mak" CFG="netsvcs_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "netsvcs_Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "netsvcs_Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "netsvcs_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\netsvcs_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\lib\netsvcs.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\lib\netsvcss.lib"

!ELSEIF  "$(CFG)" == "netsvcs_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\netsvcs_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\lib\netsvcs.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\lib\netsvcssd.lib"

!ENDIF

# Begin Target

# Name "netsvcs_Static - Win32 Release"
# Name "netsvcs_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Base_Optimizer.cpp"
# End Source File
# Begin Source File

SOURCE="Client_Logging_Handler.cpp"
# End Source File
# Begin Source File

SOURCE="Log_Message_Receiver.cpp"
# End Source File
# Begin Source File

SOURCE="Name_Handler.cpp"
# End Source File
# Begin Source File

SOURCE="Server_Logging_Handler.cpp"
# End Source File
# Begin Source File

SOURCE="Token_Handler.cpp"
# End Source File
# Begin Source File

SOURCE="TS_Clerk_Handler.cpp"
# End Source File
# Begin Source File

SOURCE="TS_Server_Handler.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Base_Optimizer.h"
# End Source File
# Begin Source File

SOURCE="Client_Logging_Handler.h"
# End Source File
# Begin Source File

SOURCE="Log_Message_Receiver.h"
# End Source File
# Begin Source File

SOURCE="Name_Handler.h"
# End Source File
# Begin Source File

SOURCE="Server_Logging_Handler.h"
# End Source File
# Begin Source File

SOURCE="Server_Logging_Handler_T.h"
# End Source File
# Begin Source File

SOURCE="Token_Handler.h"
# End Source File
# Begin Source File

SOURCE="TS_Clerk_Handler.h"
# End Source File
# Begin Source File

SOURCE="TS_Server_Handler.h"
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="Server_Logging_Handler_T.cpp"
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
