# Microsoft Developer Studio Project File - Name="maxwell" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=maxwell - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "maxwell.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "maxwell.mak" CFG="maxwell - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "maxwell - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "maxwell - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "maxwell - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winsock32.lib xerces-c_1 ar32 arvideowin32 strings32 /nologo /subsystem:console /machine:I386 /nodefaultlib:"LIBCP.LIB LIBCPMT.LIB MSVCPRT.LIB" /libpath:"..\artool\lib" /libpath:"..\xerces\lib"

!ELSEIF  "$(CFG)" == "maxwell - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /Gm /GX /ZI /Od /I "../artool/include" /I "../xerces/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FD /GZ /c
# SUBTRACT CPP /X /Fr /YX
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib xerces-c_1D.lib ar32.lib arvideowin32.lib strings32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /pdbtype:sept /libpath:"..\artool\lib" /libpath:"..\xerces\lib" /libpath:"..\vissdk\lib"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "maxwell - Win32 Release"
# Name "maxwell - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\CommonNodeFactory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ConfigurationParser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Context.cxx
# End Source File
# Begin Source File

SOURCE=.\src\DOMTreeErrorReporter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\main.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Marker.cxx
# End Source File
# Begin Source File

SOURCE=.\src\MathUtils.cxx
# End Source File
# Begin Source File

SOURCE=.\src\MaxConfidenceFilter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Module.cxx
# End Source File
# Begin Source File

SOURCE=.\src\multicast.cxx
# End Source File
# Begin Source File

SOURCE=.\src\NetworkModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\NodeFactory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\NodeFactoryContainer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\State.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Station.cxx
# End Source File
# Begin Source File

SOURCE=.\src\TestModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\TestSource.cxx
# End Source File
# Begin Source File

SOURCE=.\src\Transformation.cxx
# End Source File
# Begin Source File

SOURCE=.\src\TreeNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\VideoModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\VirtualTransformation.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\CommonNodeFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\ConfigurationParser.h
# End Source File
# Begin Source File

SOURCE=.\src\Context.h
# End Source File
# Begin Source File

SOURCE=.\src\DOMTreeErrorReporter.h
# End Source File
# Begin Source File

SOURCE=.\src\Marker.h
# End Source File
# Begin Source File

SOURCE=.\src\MathUtils.h
# End Source File
# Begin Source File

SOURCE=.\src\MaxConfidenceFilter.h
# End Source File
# Begin Source File

SOURCE=.\src\Module.h
# End Source File
# Begin Source File

SOURCE=.\src\multicast.h
# End Source File
# Begin Source File

SOURCE=.\src\NetworkModule.h
# End Source File
# Begin Source File

SOURCE=.\src\NodeFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\NodeFactoryContainer.h
# End Source File
# Begin Source File

SOURCE=.\src\State.h
# End Source File
# Begin Source File

SOURCE=.\src\Station.h
# End Source File
# Begin Source File

SOURCE=.\src\TestModule.h
# End Source File
# Begin Source File

SOURCE=.\src\TestSource.h
# End Source File
# Begin Source File

SOURCE=.\src\TrackerServer.h
# End Source File
# Begin Source File

SOURCE=.\src\Transformation.h
# End Source File
# Begin Source File

SOURCE=.\src\TreeNode.h
# End Source File
# Begin Source File

SOURCE=.\src\VideoModule.h
# End Source File
# Begin Source File

SOURCE=.\src\VirtualTransformation.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "data files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\data\camera_para.dat
# End Source File
# Begin Source File

SOURCE=.\data\maxwell.dtd
# End Source File
# Begin Source File

SOURCE=.\data\pen.tag
# End Source File
# Begin Source File

SOURCE=.\data\pip.tag
# End Source File
# Begin Source File

SOURCE=.\data\pip2.tag
# End Source File
# Begin Source File

SOURCE=.\data\test.xml
# End Source File
# End Group
# End Target
# End Project
