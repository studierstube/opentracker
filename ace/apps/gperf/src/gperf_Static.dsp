# Microsoft Developer Studio Project File - Name="gperf_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=gperf_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "gperf_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "gperf_Static.mak" CFG="gperf_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "gperf_Static - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "gperf_Static - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gperf_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\gperf_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEs.lib /libpath:"..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"..\..\..\bin\gperf.pdb"  /machine:I386 /out:"..\..\..\bin\gperf.exe"

!ELSEIF  "$(CFG)" == "gperf_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\gperf_Static"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEsd.lib /libpath:"..\..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"..\..\..\bin\gperf.pdb" /debug /machine:I386 /out:"..\..\..\bin\gperf.exe"

!ENDIF

# Begin Target

# Name "gperf_Static - Win32 Release"
# Name "gperf_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Bool_Array.cpp"
# End Source File
# Begin Source File

SOURCE="Gen_Perf.cpp"
# End Source File
# Begin Source File

SOURCE="gperf.cpp"
# End Source File
# Begin Source File

SOURCE="Hash_Table.cpp"
# End Source File
# Begin Source File

SOURCE="Iterator.cpp"
# End Source File
# Begin Source File

SOURCE="Key_List.cpp"
# End Source File
# Begin Source File

SOURCE="List_Node.cpp"
# End Source File
# Begin Source File

SOURCE="Options.cpp"
# End Source File
# Begin Source File

SOURCE="Vectors.cpp"
# End Source File
# Begin Source File

SOURCE="Version.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Bool_Array.h"
# End Source File
# Begin Source File

SOURCE="Gen_Perf.h"
# End Source File
# Begin Source File

SOURCE="Hash_Table.h"
# End Source File
# Begin Source File

SOURCE="Iterator.h"
# End Source File
# Begin Source File

SOURCE="Key_List.h"
# End Source File
# Begin Source File

SOURCE="List_Node.h"
# End Source File
# Begin Source File

SOURCE="Options.h"
# End Source File
# Begin Source File

SOURCE="Vectors.h"
# End Source File
# End Group
# End Target
# End Project
