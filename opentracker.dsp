# Microsoft Developer Studio Project File - Name="opentracker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=opentracker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "opentracker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "opentracker.mak" CFG="opentracker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "opentracker - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "opentracker - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "opentracker - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "opentracker___Win32_Release"
# PROP BASE Intermediate_Dir "opentracker___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "opentracker___Win32_Release"
# PROP Intermediate_Dir "opentracker___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "opentracker - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "opentracker___Win32_Debug"
# PROP BASE Intermediate_Dir "opentracker___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "opentracker___Win32_Debug"
# PROP Intermediate_Dir "opentracker___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "c:\projects\ace" /I "c:\projects\xerces\include" /I "c:\projects\artoolkit\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /out:"bin/opentracker.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "opentracker - Win32 Release"
# Name "opentracker - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\input\ARToolKitModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\CommonNodeFactory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\ConfigurationParser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ConsoleModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\Context.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\DOMTreeErrorReporter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\EventGenerator.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\EventQueueImplementation.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\EventQueueNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\InterTraxSource.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\isense.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\MathUtils.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\MergeNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSinkModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSourceModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\NodeFactoryContainer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\OpenTracker.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\TestModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\Transformation.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\TreeNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\VirtualTransformation.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\input\ARToolKitModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\ARToolKitSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\CommonNodeFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\config.h
# End Source File
# Begin Source File

SOURCE=.\src\core\ConfigNode.h
# End Source File
# Begin Source File

SOURCE=.\src\core\ConfigurationParser.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ConsoleModule.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ConsoleSink.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ConsoleSource.h
# End Source File
# Begin Source File

SOURCE=.\src\core\Context.h
# End Source File
# Begin Source File

SOURCE=.\src\core\DOMTreeErrorReporter.h
# End Source File
# Begin Source File

SOURCE=.\src\core\EventGenerator.h
# End Source File
# Begin Source File

SOURCE=.\src\core\EventObserver.h
# End Source File
# Begin Source File

SOURCE=.\src\core\EventQueue.h
# End Source File
# Begin Source File

SOURCE=.\src\core\EventQueueImplementation.h
# End Source File
# Begin Source File

SOURCE=.\src\common\EventQueueNode.h
# End Source File
# Begin Source File

SOURCE=.\src\input\InterTraxModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\InterTraxSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\isense.h
# End Source File
# Begin Source File

SOURCE=.\src\core\MathUtils.h
# End Source File
# Begin Source File

SOURCE=.\src\common\MergeNode.h
# End Source File
# Begin Source File

SOURCE=.\src\core\Module.h
# End Source File
# Begin Source File

SOURCE=.\src\network\Network.h
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSink.h
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSinkModule.h
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSource.h
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSourceModule.h
# End Source File
# Begin Source File

SOURCE=.\src\core\Node.h
# End Source File
# Begin Source File

SOURCE=.\src\core\NodeFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\core\NodeFactoryContainer.h
# End Source File
# Begin Source File

SOURCE=.\src\OpenTracker.h
# End Source File
# Begin Source File

SOURCE=.\src\core\State.h
# End Source File
# Begin Source File

SOURCE=.\src\core\TestModule.h
# End Source File
# Begin Source File

SOURCE=.\src\core\TestSource.h
# End Source File
# Begin Source File

SOURCE=.\src\core\ThreadModule.h
# End Source File
# Begin Source File

SOURCE=.\src\core\TimeDependend.h
# End Source File
# Begin Source File

SOURCE=.\src\common\Transformation.h
# End Source File
# Begin Source File

SOURCE=.\src\core\TreeNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\VirtualTransformation.h
# End Source File
# Begin Source File

SOURCE=.\src\core\WrapperNode.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\ACE\ace\ace.lib
# End Source File
# Begin Source File

SOURCE="..\xerces\lib\xerces-c_1.lib"
# End Source File
# Begin Source File

SOURCE=..\artoolkit\lib\ARvideoWin32.lib
# End Source File
# Begin Source File

SOURCE=..\artoolkit\lib\AR32.lib
# End Source File
# Begin Source File

SOURCE=..\VisSDK\lib\VisCore.lib
# End Source File
# Begin Source File

SOURCE=..\VisSDK\lib\VisImSrc.lib
# End Source File
# Begin Source File

SOURCE=..\artoolkit\lib\ARgsub32.lib
# End Source File
# Begin Source File

SOURCE=..\artoolkit\lib\Strings32.lib
# End Source File
# End Target
# End Project
