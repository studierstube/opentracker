# Microsoft Developer Studio Project File - Name="Smart_Pointers_Widget" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Smart_Pointers_Widget - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Smart_Pointers_Widget.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Smart_Pointers_Widget.mak" CFG="Smart_Pointers_Widget - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Smart_Pointers_Widget - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Smart_Pointers_Widget - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Smart_Pointers_Widget - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\Smart_Pointers_Widget"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\.." /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib /libpath:"..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"Release\Widget.pdb"  /machine:I386 /out:"Release\Widget.exe"

!ELSEIF  "$(CFG)" == "Smart_Pointers_Widget - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\Smart_Pointers_Widget"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\.." /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib /libpath:"..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:".\Widget.pdb" /debug /machine:I386 /out:".\Widget.exe"

!ENDIF

# Begin Target

# Name "Smart_Pointers_Widget - Win32 Release"
# Name "Smart_Pointers_Widget - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Widget.cpp"
# End Source File
# Begin Source File

SOURCE="Widget_Factory.cpp"
# End Source File
# Begin Source File

SOURCE="Widget_Impl.cpp"
# End Source File
# Begin Source File

SOURCE="Widget_Part.cpp"
# End Source File
# Begin Source File

SOURCE="Widget_Part_Factory.cpp"
# End Source File
# Begin Source File

SOURCE="Widget_Part_Impl.cpp"
# End Source File
# Begin Source File

SOURCE="widget_test.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Widget.h"
# End Source File
# Begin Source File

SOURCE="Widget_Factory.h"
# End Source File
# Begin Source File

SOURCE="Widget_Impl.h"
# End Source File
# Begin Source File

SOURCE="Widget_Part.h"
# End Source File
# Begin Source File

SOURCE="Widget_Part_Factory.h"
# End Source File
# Begin Source File

SOURCE="Widget_Part_Impl.h"
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
