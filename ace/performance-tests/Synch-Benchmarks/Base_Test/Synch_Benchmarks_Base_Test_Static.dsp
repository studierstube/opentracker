# Microsoft Developer Studio Project File - Name="Synch_Benchmarks_Base_Test_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Synch_Benchmarks_Base_Test_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Synch_Benchmarks_Base_Test_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Synch_Benchmarks_Base_Test_Static.mak" CFG="Synch_Benchmarks_Base_Test_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Synch_Benchmarks_Base_Test_Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Synch_Benchmarks_Base_Test_Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Synch_Benchmarks_Base_Test_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\Synch_Benchmarks_Base_Test_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\.." /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\.." /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\Synch_Benchmarks_Base_Test_Static.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:".\Synch_Benchmarks_Base_Test_Statics.lib"

!ELSEIF  "$(CFG)" == "Synch_Benchmarks_Base_Test_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\Synch_Benchmarks_Base_Test_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\.." /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\.." /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\Synch_Benchmarks_Base_Test_Static.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:".\Synch_Benchmarks_Base_Test_Staticsd.lib"

!ENDIF

# Begin Target

# Name "Synch_Benchmarks_Base_Test_Static - Win32 Release"
# Name "Synch_Benchmarks_Base_Test_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="base_test.cpp"
# End Source File
# Begin Source File

SOURCE="Baseline_Test.cpp"
# End Source File
# Begin Source File

SOURCE="mutex_test.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Baseline_Test.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE="Baseline_Test.i"
# End Source File
# End Group
# End Target
# End Project
