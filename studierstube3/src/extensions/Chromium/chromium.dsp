# Microsoft Developer Studio Project File - Name="chromium" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=chromium - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "chromium.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "chromium.mak" CFG="chromium - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "chromium - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "chromium - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "chromium - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CHROMIUM_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "$(OSOIVHOME)\INCLUDE" /I "e:\cr\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CHROMIUM_EXPORTS" /D "WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib inventor.lib spuload.lib crunpacker.lib crstate.lib crmothership.lib crutil.lib opengl32.lib /nologo /dll /machine:I386 /out:"../../../lib/chromium.dll" /implib:"../../../lib/chromium.lib" /libpath:"$(OSOIVHOME)\lib" /libpath:"e:\cr\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "chromium - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CHROMIUM_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "$(OSOIVHOME)\INCLUDE" /I "e:\cr\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CHROMIUM_EXPORTS" /D "WINDOWS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib inventord.lib spuload.lib crunpacker.lib crstate.lib crmothership.lib crutil.lib opengl32.lib /nologo /dll /debug /machine:I386 /out:"../../../lib/chromiumd.dll" /implib:"../../../lib/chromiumd.lib" /pdbtype:sept /libpath:"$(OSOIVHOME)\lib" /libpath:"e:\cr\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "chromium - Win32 Release"
# Name "chromium - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\chromiumext.cxx
# End Source File
# Begin Source File

SOURCE=.\crserver\server_boundsinfo.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_clear.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_clip.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_config.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_context.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_dispatch.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_gentextures.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_get.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_getmap.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_getpixelmap.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_getpointer.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_getstring.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_getteximage.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_main.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_misc.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_papi.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_projmatrix.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_readpixels.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_retval.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_simpleget.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_stb.cxx
# End Source File
# Begin Source File

SOURCE=.\crserver\server_stream.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_viewport.c
# End Source File
# Begin Source File

SOURCE=.\crserver\server_writeback.c
# End Source File
# Begin Source File

SOURCE=.\SoChromium.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\chromiumext.h
# End Source File
# Begin Source File

SOURCE=.\crserver\server.h
# End Source File
# Begin Source File

SOURCE=.\crserver\server_dispatch.h
# End Source File
# Begin Source File

SOURCE=.\crserver\server_stb.h
# End Source File
# Begin Source File

SOURCE=.\SoChromium.h
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
