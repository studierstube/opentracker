# Microsoft Developer Studio Project File - Name="baumlconverter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=baumlconverter - Win32 Debug Coin
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "baumlconverter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "baumlconverter.mak" CFG="baumlconverter - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "baumlconverter - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "baumlconverter - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "baumlconverter - Win32 Debug Coin" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "baumlconverter - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\bauml" /I "..\bau" /I "$(XERCESCROOT)\include" /I "$(QTDIR)\include" /I "$(STBROOT)\src" /I "$(OSOIVHOME)\include" /I "..\markerallocation" /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "SOQT_DLL" /YX /FD /c
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 inventor.lib xerces-c_2.lib stbapi.lib signpost.lib /nologo /subsystem:console /machine:I386 /out:"../../../../lib/baumlconverter.exe" /libpath:".." /libpath:"$(XERCESCROOT)\lib" /libpath:"$(QTDIR)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(OSOIVHOME)\lib" /libpath:"..\bauml"

!ELSEIF  "$(CFG)" == "baumlconverter - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I ".." /I "..\bauml" /I "..\bau" /I "$(XERCESCROOT)\include" /I "$(QTDIR)\include" /I "$(STBROOT)\src" /I "$(OSOIVHOME)\include" /I "..\markerallocation" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "SOQT_DLL" /FD /GZ /c
# SUBTRACT CPP /X /YX
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 inventord.lib xerces-c_2D.lib stbapid.lib signpost.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\..\bin\apps\signpost\baumlconverter.exe" /pdbtype:sept /libpath:"$(XERCESCROOT)\lib" /libpath:"$(QTDIR)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(OSOIVHOME)\lib" /libpath:"..\bauml" /libpath:".."

!ELSEIF  "$(CFG)" == "baumlconverter - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "baumlconverter___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "baumlconverter___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "baumlconverter___Win32_Debug_Coin"
# PROP Intermediate_Dir "baumlconverter___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I ".." /I "..\bauml" /I "..\bau" /I "$(XERCESCROOT)\include" /I "$(QTDIR)\include" /I "$(STBROOT)\src" /I "$(OSOIVHOME)\include" /I "..\markerallocation" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "SOQT_DLL" /FD /GZ /c
# SUBTRACT BASE CPP /X /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I ".." /I "..\bauml" /I "..\bau" /I "$(XERCESCROOT)\include" /I "$(QTDIR)\include" /I "$(STBROOT)\src" /I "$(COINROOT)\include" /I "..\markerallocation" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "SOQT_DLL" /D "COIN_DLL" /FD /GZ /c
# SUBTRACT CPP /X /YX
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qtmain.lib qt-mtedu303.lib soqt1d.lib inventord.lib xerces-c_2D.lib stbapid.lib signpost.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\..\bin\apps\signpost\baumlconverter.exe" /pdbtype:sept /libpath:"$(XERCESCROOT)\lib" /libpath:"$(QTDIR)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(OSOIVHOME)\lib" /libpath:"..\bauml" /libpath:"..\Debug"
# ADD LINK32 coin2d.lib xerces-c_2D.lib stbapid.lib signpost.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\..\bin\apps\signpost\baumlconverter.exe" /pdbtype:sept /libpath:"$(XERCESCROOT)\lib" /libpath:"$(QTDIR)\lib" /libpath:"$(STBROOT)\lib" /libpath:"$(COINROOT)\lib" /libpath:"..\bauml" /libpath:".."

!ENDIF 

# Begin Target

# Name "baumlconverter - Win32 Release"
# Name "baumlconverter - Win32 Debug"
# Name "baumlconverter - Win32 Debug Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\bauml\arbuilding.cxx
# End Source File
# Begin Source File

SOURCE=..\bauml\arroom.cxx
# End Source File
# Begin Source File

SOURCE=..\bauml\bauml.cxx
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\makeiv.cpp
# End Source File
# Begin Source File

SOURCE=.\makeot.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\bauml\arbuilding.h
# End Source File
# Begin Source File

SOURCE=..\bauml\arroom.h
# End Source File
# Begin Source File

SOURCE=..\bauml\bauml.h
# End Source File
# Begin Source File

SOURCE=..\markerallocation\box.h
# End Source File
# Begin Source File

SOURCE=..\markerallocation\cell.h
# End Source File
# Begin Source File

SOURCE=..\markerallocation\cells.h
# End Source File
# Begin Source File

SOURCE=..\markerallocation\types.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\app\baumlconverter.bat
# End Source File
# End Target
# End Project
