# Microsoft Developer Studio Project File - Name="Web_Crawler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Web_Crawler - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Web_Crawler.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Web_Crawler.mak" CFG="Web_Crawler - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Web_Crawler - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Web_Crawler - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Web_Crawler - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\Web_Crawler"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\.." /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib /libpath:"..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:"Release\main.pdb"  /machine:I386 /out:"Release\main.exe"

!ELSEIF  "$(CFG)" == "Web_Crawler - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\Web_Crawler"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\.." /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib /libpath:"..\..\lib" /nologo /version:5.4.2 /subsystem:console /pdb:".\main.pdb" /debug /machine:I386 /out:".\main.exe"

!ENDIF

# Begin Target

# Name "Web_Crawler - Win32 Release"
# Name "Web_Crawler - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Command_Processor.cpp"
# End Source File
# Begin Source File

SOURCE="HTTP_URL.cpp"
# End Source File
# Begin Source File

SOURCE="Iterators.cpp"
# End Source File
# Begin Source File

SOURCE="main.cpp"
# End Source File
# Begin Source File

SOURCE="Mem_Map_Stream.cpp"
# End Source File
# Begin Source File

SOURCE="Options.cpp"
# End Source File
# Begin Source File

SOURCE="URL.cpp"
# End Source File
# Begin Source File

SOURCE="URL_Addr.cpp"
# End Source File
# Begin Source File

SOURCE="URL_Status.cpp"
# End Source File
# Begin Source File

SOURCE="URL_Visitor.cpp"
# End Source File
# Begin Source File

SOURCE="URL_Visitor_Factory.cpp"
# End Source File
# Begin Source File

SOURCE="Web_Crawler.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Command_Processor.h"
# End Source File
# Begin Source File

SOURCE="HTTP_URL.h"
# End Source File
# Begin Source File

SOURCE="Iterators.h"
# End Source File
# Begin Source File

SOURCE="Mem_Map_Stream.h"
# End Source File
# Begin Source File

SOURCE="Options.h"
# End Source File
# Begin Source File

SOURCE="URL.h"
# End Source File
# Begin Source File

SOURCE="URL_Addr.h"
# End Source File
# Begin Source File

SOURCE="URL_Status.h"
# End Source File
# Begin Source File

SOURCE="URL_Visitor.h"
# End Source File
# Begin Source File

SOURCE="URL_Visitor_Factory.h"
# End Source File
# Begin Source File

SOURCE="Web_Crawler.h"
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
