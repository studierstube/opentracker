# Microsoft Developer Studio Project File - Name="DsRenderer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DsRenderer - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DsRenderer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DsRenderer.mak" CFG="DsRenderer - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DsRenderer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DsRenderer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DsRenderer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release.vc60"
# PROP Intermediate_Dir "Release.vc60"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSRENDERER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /Zi /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSRENDERER_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 winmm.lib ../lib.vc60/dx9vc6/strmbase.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x1d200000" /dll /machine:I386 /out:"../bin.vc60/DsRenderer.ax" /implib:"../lib.vc60/DsRenderer.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DsRenderer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug.vc60"
# PROP Intermediate_Dir "Debug.vc60"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSRENDERER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSRENDERER_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../lib.vc60/dx9vc6/strmbasd.lib winmm.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x1d200000" /dll /debug /machine:I386 /out:"../bin.vc60/DsRendererd.ax" /implib:"../lib.vc60/DsRendererd.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DsRenderer - Win32 Release"
# Name "DsRenderer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CPU.cpp
# End Source File
# Begin Source File

SOURCE=.\DsRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\DsRenderer.def
# End Source File
# Begin Source File

SOURCE=.\MemCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\PixelFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\PixelFormatTypes.h
# End Source File
# Begin Source File

SOURCE=.\VFxBase.cpp
# End Source File
# Begin Source File

SOURCE=.\VFxDeinterlace.cpp
# End Source File
# Begin Source File

SOURCE=.\VFxFlip.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CPU.h
# End Source File
# Begin Source File

SOURCE=.\DsRenderer.h
# End Source File
# Begin Source File

SOURCE=.\IRendererParam.h
# End Source File
# Begin Source File

SOURCE=.\IRendererVFx.h
# End Source File
# Begin Source File

SOURCE=.\IRenderTarget.h
# End Source File
# Begin Source File

SOURCE=.\IRenderTargetDirect3D.h
# End Source File
# Begin Source File

SOURCE=.\IRenderTargetMemoryBuffer.h
# End Source File
# Begin Source File

SOURCE=.\IRenderTargetOpenGL.h
# End Source File
# Begin Source File

SOURCE=.\IRenderTargetUserCallback.h
# End Source File
# Begin Source File

SOURCE=.\MemCopy.h
# End Source File
# Begin Source File

SOURCE=.\PixelFormat.h
# End Source File
# Begin Source File

SOURCE=.\VFxBase.h
# End Source File
# Begin Source File

SOURCE=.\VFxBaseParam.h
# End Source File
# Begin Source File

SOURCE=.\VFxDeinterlace.h
# End Source File
# Begin Source File

SOURCE=.\VFxDeinterlaceParam.h
# End Source File
# Begin Source File

SOURCE=.\VFxFlip.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\DsRenderer.RC
# End Source File
# End Group
# Begin Source File

SOURCE=..\LICENSE.txt
# End Source File
# Begin Source File

SOURCE=..\README.txt
# End Source File
# End Target
# End Project
