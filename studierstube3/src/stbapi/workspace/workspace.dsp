# Microsoft Developer Studio Project File - Name="workspace" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=workspace - Win32 Release Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "workspace.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "workspace.mak" CFG="workspace - Win32 Release Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "workspace - Win32 Debug OSOIV" (based on "Win32 (x86) Console Application")
!MESSAGE "workspace - Win32 Debug TGS" (based on "Win32 (x86) Console Application")
!MESSAGE "workspace - Win32 Release TGS" (based on "Win32 (x86) Console Application")
!MESSAGE "workspace - Win32 Debug TGS with SGI Video" (based on "Win32 (x86) Console Application")
!MESSAGE "workspace - Win32 Release TGS with SGI Video" (based on "Win32 (x86) Console Application")
!MESSAGE "workspace - Win32 Release OSOIV" (based on "Win32 (x86) Console Application")
!MESSAGE "workspace - Win32 Debug Coin" (based on "Win32 (x86) Console Application")
!MESSAGE "workspace - Win32 Release Coin" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "workspace - Win32 Debug OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_OSOIV"
# PROP Intermediate_Dir "Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /X /I "C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" /I "C:\OSOIV\Install\lnclude" /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /D "_CONSOLE" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "_CONSOLE" /D "HA" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspace.exe" /pdbtype:sept /libpath:"C:\OSOIV\Install\Lib" /libpath:"C:\Program Files\Qt\Lib"
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspaced.exe" /pdbtype:sept
# SUBTRACT LINK32 /verbose

!ELSEIF  "$(CFG)" == "workspace - Win32 Debug TGS"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug_TGS"
# PROP BASE Intermediate_Dir "Win32_Debug_TGS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_TGS"
# PROP Intermediate_Dir "Win32_Debug_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /X /I "C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" /I "C:\OSOIV\Install\lnclude" /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /D "_CONSOLE" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /D "_DEBUG" /D "HA" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FR /FD /GZ /c
# SUBTRACT CPP /X /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspace.exe" /pdbtype:sept /libpath:"C:\OSOIV\Install\Lib" /libpath:"C:\Program Files\Qt\Lib"
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspace.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "workspace - Win32 Release TGS"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_Release_TGS"
# PROP BASE Intermediate_Dir "Win32_Release_TGS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Release_TGS"
# PROP Intermediate_Dir "Win32_Release_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /X /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /I "C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" /I "C:\OSOIV\Install\lnclude" /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /D "NDEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /D "NDEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT CPP /X /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /machine:I386 /out:"../../../bin/workspace/workspace.exe"
# ADD LINK32 /nologo /subsystem:console /machine:I386 /out:"../../../bin/workspace/workspace.exe"

!ELSEIF  "$(CFG)" == "workspace - Win32 Debug TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "workspace___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "workspace___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "workspace___Win32_Debug_TGS_with_SGI_Video"
# PROP Intermediate_Dir "workspace___Win32_Debug_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /D "_DEBUG" /D "HA" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /X /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /D "_DEBUG" /D "HA" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FR /FD /GZ /c
# SUBTRACT CPP /X /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspace.exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspace.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "workspace - Win32 Release TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "workspace___Win32_Release_TGS_with_SGI_Video0"
# PROP BASE Intermediate_Dir "workspace___Win32_Release_TGS_with_SGI_Video0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "workspace___Win32_Release_TGS_with_SGI_Video0"
# PROP Intermediate_Dir "workspace___Win32_Release_TGS_with_SGI_Video0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /D "NDEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT BASE CPP /X /YX
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft Visual Studio\VC98\Include" /D "NDEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT CPP /X /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /machine:I386 /out:"../../../bin/workspace/workspace.exe"
# ADD LINK32 /nologo /subsystem:console /machine:I386 /out:"../../../bin/workspace/workspace.exe"

!ELSEIF  "$(CFG)" == "workspace - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "workspace___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "workspace___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "workspace___Win32_Release_OSOIV"
# PROP Intermediate_Dir "workspace___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "_CONSOLE" /D "_AFXDLL" /D "HA" /D "USE_SOQT_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "_CONSOLE" /D "HA" /D "NDEBUG" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspace.exe" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# SUBTRACT BASE LINK32 /verbose
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspace.exe" /pdbtype:sept
# SUBTRACT LINK32 /verbose

!ELSEIF  "$(CFG)" == "workspace - Win32 Debug Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "workspace___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "workspace___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "workspace___Win32_Debug_Coin"
# PROP Intermediate_Dir "workspace___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "_CONSOLE" /D "_AFXDLL" /D "HA" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /D "_CONSOLE" /D "HA" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /X /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspaced.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /verbose
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspaced.exe" /pdbtype:sept
# SUBTRACT LINK32 /verbose

!ELSEIF  "$(CFG)" == "workspace - Win32 Release Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "workspace___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "workspace___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "workspace___Win32_Release_Coin"
# PROP Intermediate_Dir "workspace___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "_CONSOLE" /D "_AFXDLL" /D "HA" /D "NDEBUG" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /D "_CONSOLE" /D "HA" /D "NDEBUG" /D "WIN32" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspace.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /verbose
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"../../../bin/workspace/workspace.exe" /pdbtype:sept
# SUBTRACT LINK32 /verbose

!ENDIF 

# Begin Target

# Name "workspace - Win32 Debug OSOIV"
# Name "workspace - Win32 Debug TGS"
# Name "workspace - Win32 Release TGS"
# Name "workspace - Win32 Debug TGS with SGI Video"
# Name "workspace - Win32 Release TGS with SGI Video"
# Name "workspace - Win32 Release OSOIV"
# Name "workspace - Win32 Debug Coin"
# Name "workspace - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\main.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
