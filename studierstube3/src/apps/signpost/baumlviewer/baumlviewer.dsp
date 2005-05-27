# Microsoft Developer Studio Project File - Name="baumlviewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=baumlviewer - Win32 Debug Coin
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "baumlviewer.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "baumlviewer.mak" CFG="baumlviewer - Win32 Debug Coin"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "baumlviewer - Win32 Release" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE "baumlviewer - Win32 Debug" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE "baumlviewer - Win32 Debug Coin" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "baumlviewer - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "$(QTDIR)\INCLUDE" /I "$(XERCESCROOT)\include" /I "../bauml" /I "$(OSOIVHOME)\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "SOQT_DLL" /FR /YX /FD /c
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 inventor.lib xerces-c_2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qtmain.lib qt-mtedu303.lib soqt1.lib /nologo /subsystem:console /machine:I386 /out:"../../../../bin/apps/signpost/baumlviewer.exe" /libpath:"$(XERCESCROOT)\lib" /libpath:"$(OSOIVHOME)\lib" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "baumlviewer - Win32 Debug"

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
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\INCLUDE" /I "$(XERCESCROOT)\include" /I "../bauml" /I "$(OSOIVHOME)\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "USE_SOQT" /D "SOQT_DLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 inventord.lib xerces-c_2D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qtmain.lib qt-mtedu303.lib soqt1d.lib /nologo /subsystem:console /debug /machine:I386 /out:"../../../../bin/apps/signpost/baumlviewer.exe" /pdbtype:sept /libpath:"$(XERCESCROOT)\lib" /libpath:"$(OSOIVHOME)\lib" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "baumlviewer - Win32 Debug Coin"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "baumlviewer___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "baumlviewer___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "baumlviewer___Win32_Debug_Coin"
# PROP Intermediate_Dir "baumlviewer___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\INCLUDE" /I "$(XERCESCROOT)\include" /I "../bauml" /I "$(OSOIVHOME)\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "USE_SOQT" /D "SOQT_DLL" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\INCLUDE" /I "$(XERCESCROOT)\include" /I "../bauml" /I "$(COINROOT)\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "USE_SOQT" /D "SOQT_DLL" /D "COIN_DLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib xerces-c_2D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qtmain.lib qt-mtedu303.lib soqt1d.lib /nologo /subsystem:console /debug /machine:I386 /out:"../../../../bin/apps/signpost/baumlviewer.exe" /pdbtype:sept /libpath:"$(XERCESCROOT)\lib" /libpath:"$(OSOIVHOME)\lib" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 soqt1Coind.lib coin2d.lib xerces-c_2D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qtmain.lib qt-mtedu303.lib /nologo /subsystem:console /debug /machine:I386 /out:"../../../../bin/apps/signpost/baumlviewer.exe" /pdbtype:sept /libpath:"$(XERCESCROOT)\lib" /libpath:"$(COINROOT)\lib" /libpath:"$(QTDIR)\LIB"

!ENDIF 

# Begin Target

# Name "baumlviewer - Win32 Release"
# Name "baumlviewer - Win32 Debug"
# Name "baumlviewer - Win32 Debug Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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
