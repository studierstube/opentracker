# Microsoft Developer Studio Project File - Name="sman2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=sman2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sman2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sman2.mak" CFG="sman2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sman2 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "sman2 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sman2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(ACEROOT)" /I "../../src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "STBAPI_STATIC" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib /nologo /subsystem:console /machine:I386 /libpath:"$(ACEROOT)/ace" /libpath:"$(ACEROOT)/lib"

!ELSEIF  "$(CFG)" == "sman2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "$(ACEROOT)" /I "../../src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "STBAPI_STATIC" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib /nologo /subsystem:console /debug /machine:I386 /out:"sman2.exe" /pdbtype:sept /libpath:"$(ACEROOT)/ace" /libpath:"$(ACEROOT)/lib"

!ENDIF 

# Begin Target

# Name "sman2 - Win32 Release"
# Name "sman2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\AddUserMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\Application.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\util\buffer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\ChangeFocusMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\DIVAddressFactory.cxx
# End Source File
# Begin Source File

SOURCE=.\Host.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\JoinLocaleMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\LeaveLocaleMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\Locale.cxx
# End Source File
# Begin Source File

SOURCE=.\main.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\Message.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\MessageFactory.cxx
# End Source File
# Begin Source File

SOURCE=.\MonitorHandler.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\RemoveUserMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\SessionHandler.cxx
# End Source File
# Begin Source File

SOURCE=.\SessionManager.cxx
# End Source File
# Begin Source File

SOURCE=.\SessionVisitorAdaptor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SetApplicationModeMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SetDIVParamMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SetFocusMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SheetBarMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\StartApplicationMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\StopApplicationMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\StreamHandler.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SystemBarMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\TaskBarMessage.cxx
# End Source File
# Begin Source File

SOURCE=.\User.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\UseResourcesMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\UserKitMessage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\WindowBarMessage.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\AddUserMessage.h
# End Source File
# Begin Source File

SOURCE=.\Application.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\util\buffer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\ChangeFocusMessage.h
# End Source File
# Begin Source File

SOURCE=.\DIVAddressFactory.h
# End Source File
# Begin Source File

SOURCE=.\Host.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\JoinLocaleMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\LeaveLocaleMessage.h
# End Source File
# Begin Source File

SOURCE=.\Locale.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\Message.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\MessageFactory.h
# End Source File
# Begin Source File

SOURCE=.\MonitorHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\RemoveUserMessage.h
# End Source File
# Begin Source File

SOURCE=.\SessionHandler.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SessionMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SessionVisitor.h
# End Source File
# Begin Source File

SOURCE=.\SessionVisitorAdaptor.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SetApplicationModeMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SetDIVParamMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SetFocusMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SheetBarMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\StartApplicationMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\StopApplicationMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\StreamHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\SystemBarMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\TaskBarMessage.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\UseResourcesMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\UserKitMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\UserKitMessageVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stbapi\workspace\distrManager\WindowBarMessage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
