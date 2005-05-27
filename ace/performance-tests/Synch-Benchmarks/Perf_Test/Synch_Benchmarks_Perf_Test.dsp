# Microsoft Developer Studio Project File - Name="Synch_Benchmarks_Perf_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Synch_Benchmarks_Perf_Test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Synch_Benchmarks_Perf_Test.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Synch_Benchmarks_Perf_Test.mak" CFG="Synch_Benchmarks_Perf_Test - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Synch_Benchmarks_Perf_Test - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Synch_Benchmarks_Perf_Test - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Synch_Benchmarks_Perf_Test - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release\Synch_Benchmarks_Perf_Test"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\.." /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\.." /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\Synch_Benchmarks_Perf_Test.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO Synch_Lib.lib ACE.lib /libpath:"..\..\..\lib" /libpath:"..\Synch_Lib" /nologo /version:5.4.2 /subsystem:windows /pdb:".\Synch_Benchmarks_Perf_Test.pdb" /dll  /machine:I386 /out:".\Synch_Benchmarks_Perf_Test.dll"

!ELSEIF  "$(CFG)" == "Synch_Benchmarks_Perf_Test - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\Synch_Benchmarks_Perf_Test"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\.." /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\.." /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\Synch_Benchmarks_Perf_Test.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO Synch_Libd.lib ACEd.lib /libpath:"..\..\..\lib" /libpath:"..\Synch_Lib" /nologo /version:5.4.2 /subsystem:windows /pdb:".\Synch_Benchmarks_Perf_Testd.pdb" /dll /debug /machine:I386 /out:".\Synch_Benchmarks_Perf_Testd.dll"

!ENDIF

# Begin Target

# Name "Synch_Benchmarks_Perf_Test - Win32 Release"
# Name "Synch_Benchmarks_Perf_Test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Adaptive_Lock_Performance_Test_Base.cpp"
# End Source File
# Begin Source File

SOURCE="adaptive_mutex_test.cpp"
# End Source File
# Begin Source File

SOURCE="adaptive_recursive_lock_test.cpp"
# End Source File
# Begin Source File

SOURCE="adaptive_sema_test.cpp"
# End Source File
# Begin Source File

SOURCE="Benchmark_Performance.cpp"
# End Source File
# Begin Source File

SOURCE="condb_test.cpp"
# End Source File
# Begin Source File

SOURCE="conds_test.cpp"
# End Source File
# Begin Source File

SOURCE="context_test.cpp"
# End Source File
# Begin Source File

SOURCE="guard_test.cpp"
# End Source File
# Begin Source File

SOURCE="memory_test.cpp"
# End Source File
# Begin Source File

SOURCE="mutex_test.cpp"
# End Source File
# Begin Source File

SOURCE="Performance_Test.cpp"
# End Source File
# Begin Source File

SOURCE="Performance_Test_Options.cpp"
# End Source File
# Begin Source File

SOURCE="pipe_proc_test.cpp"
# End Source File
# Begin Source File

SOURCE="pipe_thr_test.cpp"
# End Source File
# Begin Source File

SOURCE="recursive_lock_test.cpp"
# End Source File
# Begin Source File

SOURCE="rwrd_test.cpp"
# End Source File
# Begin Source File

SOURCE="rwwr_test.cpp"
# End Source File
# Begin Source File

SOURCE="sema_test.cpp"
# End Source File
# Begin Source File

SOURCE="sysvsema_test.cpp"
# End Source File
# Begin Source File

SOURCE="token_test.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Adaptive_Lock_Performance_Test_Base.h"
# End Source File
# Begin Source File

SOURCE="Benchmark_Performance.h"
# End Source File
# Begin Source File

SOURCE="Performance_Test.h"
# End Source File
# Begin Source File

SOURCE="Performance_Test_Options.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE="Performance_Test_Options.i"
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
