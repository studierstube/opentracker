# Microsoft Developer Studio Project File - Name="aeki" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=aeki - Win32 Release Coin
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "aeki.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "aeki.mak" CFG="aeki - Win32 Release Coin"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "aeki - Win32 Release TGS" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "aeki - Win32 Debug TGS" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "aeki - Win32 Debug OSOIV" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "aeki - Win32 Release OSOIV" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "aeki - Win32 Debug Coin" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "aeki - Win32 Release Coin" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "aeki - Win32 Release TGS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_Release_TGS"
# PROP BASE Intermediate_Dir "Win32_Release_TGS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_Release_TGS"
# PROP Intermediate_Dir "Win32_Release_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AEKI_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(MSVSHOME)\VC98\INCLUDE" /I "../../" /I "$(XERCESCROOT)/include" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 xerces-c_1.lib /nologo /dll /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /libpath:"$(XERCESCROOT)/lib"

!ELSEIF  "$(CFG)" == "aeki - Win32 Debug TGS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug_TGS"
# PROP BASE Intermediate_Dir "Win32_Debug_TGS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_TGS"
# PROP Intermediate_Dir "Win32_Debug_TGS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AEKI_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../" /I "$(XERCESCROOT)/include" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_1D.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(XERCESCROOT)/lib"

!ELSEIF  "$(CFG)" == "aeki - Win32 Debug OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Debug_OSOIV"
# PROP BASE Intermediate_Dir "Win32_Debug_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_Debug_OSOIV"
# PROP Intermediate_Dir "Win32_Debug_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(XERCESCROOT)\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../../" /I "$(XERCESCROOT)/include" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 inventord.lib xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(XERCESCROOT)\LIB"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "aeki - Win32 Release OSOIV"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "aeki___Win32_Release_OSOIV"
# PROP BASE Intermediate_Dir "aeki___Win32_Release_OSOIV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "aeki___Win32_Release_OSOIV"
# PROP Intermediate_Dir "aeki___Win32_Release_OSOIV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "../../" /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /G6 /MD /W3 /Gm /Gi /GR /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../../" /I "$(XERCESCROOT)/include" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_DEBUG" /D "HA" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"
# ADD LINK32 inventor.lib xerces-c_2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(XERCESCROOT)/lib"

!ELSEIF  "$(CFG)" == "aeki - Win32 Debug Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "aeki___Win32_Debug_Coin"
# PROP BASE Intermediate_Dir "aeki___Win32_Debug_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "aeki___Win32_Debug_Coin"
# PROP Intermediate_Dir "aeki___Win32_Debug_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(XERCESCROOT)\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../../" /I "$(XERCESCROOT)/include" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(XERCESCROOT)\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(COINROOT)\INCLUDE" /I "../../" /I "$(XERCESCROOT)/include" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_SOQT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "COIN_DLL" /FR /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventord.lib xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(XERCESCROOT)\LIB"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Coin2d.lib xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /pdbtype:sept /libpath:"$(COINROOT)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(XERCESCROOT)\LIB"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "aeki - Win32 Release Coin"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "aeki___Win32_Release_Coin"
# PROP BASE Intermediate_Dir "aeki___Win32_Release_Coin"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "aeki___Win32_Release_Coin"
# PROP Intermediate_Dir "aeki___Win32_Release_Coin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../../" /I "$(XERCESCROOT)/include" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /I "$(OSOIVHOME)\INCLUDE" /I "../../" /I "$(XERCESCROOT)/include" /D "_WINDOWS" /D "_USRDLL" /D "AEKI_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "USE_SOQT" /D "WIN32" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 inventor.lib xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(XERCESCROOT)/lib"
# ADD LINK32 inventor.lib xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/aeki/aeki_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB" /libpath:"$(XERCESCROOT)/lib"

!ENDIF 

# Begin Target

# Name "aeki - Win32 Release TGS"
# Name "aeki - Win32 Debug TGS"
# Name "aeki - Win32 Debug OSOIV"
# Name "aeki - Win32 Release OSOIV"
# Name "aeki - Win32 Debug Coin"
# Name "aeki - Win32 Release Coin"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\aeki.cxx
# End Source File
# Begin Source File

SOURCE=.\FurnitureFactory.cxx
# End Source File
# Begin Source File

SOURCE=.\SoAekiFurnitureKit.cxx
# End Source File
# Begin Source File

SOURCE=.\SoAekiKit.cxx
# End Source File
# Begin Source File

SOURCE=.\XmlHandler.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\aeki.h"
# End Source File
# Begin Source File

SOURCE=.\AssemblyStep.h
# End Source File
# Begin Source File

SOURCE=.\ComponentInstance.h
# End Source File
# Begin Source File

SOURCE=.\FurnitureFactory.h
# End Source File
# Begin Source File

SOURCE=.\SoAekiFurnitureKit.h
# End Source File
# Begin Source File

SOURCE=.\SoAekiKit.h
# End Source File
# Begin Source File

SOURCE=.\XmlHandler.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\aeki_stb.iv
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\sheet.iv
# End Source File
# End Group
# Begin Group "Textures"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\button_exit.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\button_fwd.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\button_next.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\button_prev.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\button_rev.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\button_start.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\wood_1.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\wood_2.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\wood_3.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\wood_4.gif
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\aeki\textures\wood_5.gif
# End Source File
# End Group
# End Target
# End Project
