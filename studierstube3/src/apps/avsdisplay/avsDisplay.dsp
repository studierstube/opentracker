# Microsoft Developer Studio Project File - Name="avsDisplay" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=avsDisplay - Win32 Debug OSOIV
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "avsDisplay.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "avsDisplay.mak" CFG="avsDisplay - Win32 Debug OSOIV"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "avsDisplay - Win32 Release TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "avsDisplay - Win32 Debug TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "avsDisplay - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "avsDisplay - Win32 Debug TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "avsDisplay - Win32 Release TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "avsDisplay - Win32 Release TGS"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\..\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FR /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ws2_32.lib inv260.lib /nologo /dll /incremental:yes /map /machine:I386 /out:"..\..\..\bin\apps\avsdisplay\avsdisplay_stb.dll"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "avsDisplay - Win32 Debug TGS"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\..\\" /D "_WINDOWS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\apps\avsdisplay\avsdisplay_stb.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "avsDisplay - Win32 Debug OSOIV"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "./../.." /I "..\..\\" /I "C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" /I "C:\OSOIV\Install\lnclude" /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /D "_WINDOWS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "NO_STRICT" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "..\.." /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "_WINDOWS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "HA" /D "USE_SOQT_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\apps\avsdisplay\avsdisplay_stb.dll" /pdbtype:sept /libpath:"C:\OSOIV\Install\Lib" /libpath:"C:\Program Files\Qt\Lib"
# ADD LINK32 ws2_32.lib qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\apps\avsdisplay\avsdisplay_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "avsDisplay - Win32 Debug TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "avsDisplay___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "avsDisplay___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "avsDisplay___Win32_Debug_TGS_with_SGI_Video"
# PROP Intermediate_Dir "avsDisplay___Win32_Debug_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\..\\" /D "_WINDOWS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\..\\" /D "_WINDOWS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\apps\avsdisplay\avsdisplay_stb.dll" /pdbtype:sept
# ADD LINK32 ws2_32.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\apps\avsdisplay\avsdisplay_stb.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "avsDisplay - Win32 Release TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "avsDisplay___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "avsDisplay___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "avsDisplay___Win32_Release_TGS_with_SGI_Video"
# PROP Intermediate_Dir "avsDisplay___Win32_Release_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\..\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FR /FD /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\..\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "AVSDISPLAY_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FR /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib inv260.lib /nologo /dll /incremental:yes /map /machine:I386 /out:"..\..\..\bin\apps\avsdisplay\avsdisplay_stb.dll"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 ws2_32.lib inv260.lib /nologo /dll /incremental:yes /map /machine:I386 /out:"..\..\..\bin\apps\avsdisplay\avsdisplay_stb.dll"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "avsDisplay - Win32 Release TGS"
# Name "avsDisplay - Win32 Debug TGS"
# Name "avsDisplay - Win32 Debug OSOIV"
# Name "avsDisplay - Win32 Debug TGS with SGI Video"
# Name "avsDisplay - Win32 Release TGS with SGI Video"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\avsdisplay.cxx

!IF  "$(CFG)" == "avsDisplay - Win32 Release TGS"

!ELSEIF  "$(CFG)" == "avsDisplay - Win32 Debug TGS"

!ELSEIF  "$(CFG)" == "avsDisplay - Win32 Debug OSOIV"

# ADD CPP /I "c:\install\include\inventor"

!ELSEIF  "$(CFG)" == "avsDisplay - Win32 Debug TGS with SGI Video"

!ELSEIF  "$(CFG)" == "avsDisplay - Win32 Release TGS with SGI Video"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Bin2IV.cxx
# End Source File
# Begin Source File

SOURCE=.\boolmodule.cxx
# End Source File
# Begin Source File

SOURCE=.\clientmodule.cxx
# End Source File
# Begin Source File

SOURCE=.\floatmodule.cxx
# End Source File
# Begin Source File

SOURCE=.\geommodule.cxx
# End Source File
# Begin Source File

SOURCE=.\intmodule.cxx
# End Source File
# Begin Source File

SOURCE=.\linemodule.cxx
# End Source File
# Begin Source File

SOURCE=.\moduleman.cxx
# End Source File
# Begin Source File

SOURCE=.\pipgroupmodule.cxx
# End Source File
# Begin Source File

SOURCE=.\pipmodule.cxx
# End Source File
# Begin Source File

SOURCE=.\pointmodule.cxx
# End Source File
# Begin Source File

SOURCE=.\polylinemodule.cxx
# End Source File
# Begin Source File

SOURCE=.\stbcomm.cxx
# End Source File
# Begin Source File

SOURCE=.\windowmodule.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\avsdisplay.h
# End Source File
# Begin Source File

SOURCE=.\Bin2IV.h
# End Source File
# Begin Source File

SOURCE=.\boolmodule.h
# End Source File
# Begin Source File

SOURCE=.\clientmodule.h
# End Source File
# Begin Source File

SOURCE=.\floatmodule.h
# End Source File
# Begin Source File

SOURCE=.\geommodule.h
# End Source File
# Begin Source File

SOURCE=.\intmodule.h
# End Source File
# Begin Source File

SOURCE=.\linemodule.h
# End Source File
# Begin Source File

SOURCE=.\module.h
# End Source File
# Begin Source File

SOURCE=.\moduleman.h
# End Source File
# Begin Source File

SOURCE=.\pipgroupmodule.h
# End Source File
# Begin Source File

SOURCE=.\pipmodule.h
# End Source File
# Begin Source File

SOURCE=.\pointmodule.h
# End Source File
# Begin Source File

SOURCE=.\polylinemodule.h
# End Source File
# Begin Source File

SOURCE=.\windowmodule.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
