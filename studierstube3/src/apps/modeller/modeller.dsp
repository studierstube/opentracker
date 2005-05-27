# Microsoft Developer Studio Project File - Name="modeller" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=modeller - Win32 Debug TGS with SGI Video
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "modeller.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "modeller.mak" CFG="modeller - Win32 Debug TGS with SGI Video"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "modeller - Win32 Release TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "modeller - Win32 Debug TGS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "modeller - Win32 Debug OSOIV" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "modeller - Win32 Debug TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "modeller - Win32 Release TGS with SGI Video" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "modeller - Win32 Release TGS"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MODELLER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "MODELLER_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
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
# ADD LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/modeller/modeller_stb.dll"

!ELSEIF  "$(CFG)" == "modeller - Win32 Debug TGS"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MODELLER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "MODELLER_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
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
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/modeller/modeller_stb.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "modeller - Win32 Debug OSOIV"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /X /I "../../" /I "C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE" /I "C:\OSOIV\Install\lnclude" /I "c:\program files\microsoft visual studio\vc98\include" /I "c:\program files\qt\include" /I "c:\osoiv\install\include\inventor" /I "c:\osoiv\install\include" /D "_WINDOWS" /D "_USRDLL" /D "MODELLER_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /X /I "..\.." /I "$(MSVSHOME)\VC98\INCLUDE" /I "$(QTDIR)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE" /I "$(OSOIVHOME)\INCLUDE\INVENTOR" /D "_WINDOWS" /D "_USRDLL" /D "MODELLER_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "_DEBUG" /D "HA" /D "WIN32" /D "_MBCS" /D "USE_SOQT_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/modeller/modeller_stb.so" /pdbtype:sept /libpath:"C:\OSOIV\Install\Lib" /libpath:"C:\Program Files\Qt\Lib"
# ADD LINK32 qtmain.lib qt220.lib soqt.lib inventor.lib /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/modeller/modeller_stb.dll" /pdbtype:sept /libpath:"$(OSOIVHOME)\LIB" /libpath:"$(QTDIR)\LIB"

!ELSEIF  "$(CFG)" == "modeller - Win32 Debug TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "modeller___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "modeller___Win32_Debug_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "modeller___Win32_Debug_TGS_with_SGI_Video"
# PROP Intermediate_Dir "modeller___Win32_Debug_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "MODELLER_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../" /D "_DEBUG" /D "HA" /D "_WINDOWS" /D "_USRDLL" /D "MODELLER_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /GZ /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/modeller/modeller_stb.dll" /pdbtype:sept
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"../../../bin/apps/modeller/modeller_stb.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "modeller - Win32 Release TGS with SGI Video"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "modeller___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Intermediate_Dir "modeller___Win32_Release_TGS_with_SGI_Video"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "modeller___Win32_Release_TGS_with_SGI_Video"
# PROP Intermediate_Dir "modeller___Win32_Release_TGS_with_SGI_Video"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "MODELLER_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT BASE CPP /X /YX /Yc /Yu
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "../../" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "MODELLER_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_MBCS" /D "USE_TGS" /FD /c
# SUBTRACT CPP /X /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc07 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/modeller/modeller_stb.dll"
# ADD LINK32 /nologo /dll /machine:I386 /out:"../../../bin/apps/modeller/modeller_stb.dll"

!ENDIF 

# Begin Target

# Name "modeller - Win32 Release TGS"
# Name "modeller - Win32 Debug TGS"
# Name "modeller - Win32 Debug OSOIV"
# Name "modeller - Win32 Debug TGS with SGI Video"
# Name "modeller - Win32 Release TGS with SGI Video"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Modeller.cxx
# End Source File
# Begin Source File

SOURCE=.\modeller_win32.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\modeller.h
# End Source File
# Begin Source File

SOURCE=.\modeller_win32.h
# End Source File
# End Group
# Begin Group "Inventor files"

# PROP Default_Filter "iv"
# Begin Group "Saved files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\save\File1.iv
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\save\File2.iv
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\save\File3.iv
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\save\File4.iv
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\save\File5.iv
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\save\File6.iv
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\ModellerSheet.iv
# End Source File
# End Group
# Begin Group "Texture Files"

# PROP Default_Filter "*.jpg"
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\draw_4x2.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\file1_5x3.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\file2_5x3.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\file3_5x3.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\file4_5x3.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\file5_5x3.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\file6_5x3.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\file_4x2.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\load_4x2.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\model_4x2.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\Models.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\new_4x2.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\o_2x4.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\o_4x2.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\save_4x2.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\SoAlphabetKit0.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\SoAlphabetKit1.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\SoAlphabetKit2.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\SoAlphabetKit3.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\text_4x2.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\x_2x4.jpg
# End Source File
# Begin Source File

SOURCE=..\..\..\bin\apps\modeller\textures\x_4x2.jpg
# End Source File
# End Group
# End Target
# End Project
