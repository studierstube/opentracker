# Microsoft Developer Studio Project File - Name="ACEXML_Parser_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ACEXML_Parser_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "ACEXML_Parser_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "ACEXML_Parser_Static.mak" CFG="ACEXML_Parser_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "ACEXML_Parser_Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ACEXML_Parser_Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ACEXML_Parser_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\ACEXML_Parser_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\ACEXML_Parser.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\..\lib\ACEXML_Parsers.lib"

!ELSEIF  "$(CFG)" == "ACEXML_Parser_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\ACEXML_Parser_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\ACEXML_Parser.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\..\lib\ACEXML_Parsersd.lib"

!ENDIF

# Begin Target

# Name "ACEXML_Parser_Static - Win32 Release"
# Name "ACEXML_Parser_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Entity_Manager.cpp"
# End Source File
# Begin Source File

SOURCE="Parser.cpp"
# End Source File
# Begin Source File

SOURCE="ParserContext.cpp"
# End Source File
# Begin Source File

SOURCE="ParserInternals.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="Entity_Manager.h"
# End Source File
# Begin Source File

SOURCE="Parser.h"
# End Source File
# Begin Source File

SOURCE="Parser_export.h"
# End Source File
# Begin Source File

SOURCE="ParserContext.h"
# End Source File
# Begin Source File

SOURCE="ParserInternals.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE="Entity_Manager.i"
# End Source File
# Begin Source File

SOURCE="Parser.i"
# End Source File
# Begin Source File

SOURCE="ParserContext.inl"
# End Source File
# End Group
# End Target
# End Project
