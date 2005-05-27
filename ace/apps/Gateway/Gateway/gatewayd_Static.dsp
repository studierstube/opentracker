# Microsoft Developer Studio Project File - Name="gatewayd_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=gatewayd_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "gatewayd_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "gatewayd_Static.mak" CFG="gatewayd_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "gatewayd_Static - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "gatewayd_Static - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gatewayd_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\gatewayd_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO Gateways.lib ACEs.lib /libpath:"..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"Static_Release\gatewayd.pdb"  /machine:I386 /out:"Static_Release\gatewayd.exe"

!ELSEIF  "$(CFG)" == "gatewayd_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\gatewayd_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO Gatewaysd.lib ACEsd.lib /libpath:"..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"Static_Debug\gatewayd.pdb" /debug /machine:I386 /out:"Static_Debug\gatewayd.exe"

!ENDIF

# Begin Target

# Name "gatewayd_Static - Win32 Release"
# Name "gatewayd_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="gatewayd.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Concrete_Connection_Handlers.h"
# End Source File
# Begin Source File

SOURCE="Config_Files.h"
# End Source File
# Begin Source File

SOURCE="Connection_Handler.h"
# End Source File
# Begin Source File

SOURCE="Connection_Handler_Acceptor.h"
# End Source File
# Begin Source File

SOURCE="Connection_Handler_Connector.h"
# End Source File
# Begin Source File

SOURCE="Consumer_Dispatch_Set.h"
# End Source File
# Begin Source File

SOURCE="Event.h"
# End Source File
# Begin Source File

SOURCE="Event_Channel.h"
# End Source File
# Begin Source File

SOURCE="Event_Forwarding_Discriminator.h"
# End Source File
# Begin Source File

SOURCE="File_Parser.h"
# End Source File
# Begin Source File

SOURCE="Gateway.h"
# End Source File
# Begin Source File

SOURCE="Options.h"
# End Source File
# End Group
# End Target
# End Project
