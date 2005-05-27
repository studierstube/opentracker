# Microsoft Developer Studio Project File - Name="ACEXML_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ACEXML_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "ACEXML_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "ACEXML_Static.mak" CFG="ACEXML_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "ACEXML_Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ACEXML_Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ACEXML_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\ACEXML_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\lib\ACEXML.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\lib\ACEXMLs.lib"

!ELSEIF  "$(CFG)" == "ACEXML_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\ACEXML_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\lib\ACEXML.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\lib\ACEXMLsd.lib"

!ENDIF

# Begin Target

# Name "ACEXML_Static - Win32 Release"
# Name "ACEXML_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="Attributes_Def_Builder.cpp"
# End Source File
# Begin Source File

SOURCE="AttributesImpl.cpp"
# End Source File
# Begin Source File

SOURCE="CharStream.cpp"
# End Source File
# Begin Source File

SOURCE="DefaultHandler.cpp"
# End Source File
# Begin Source File

SOURCE="DTD_Manager.cpp"
# End Source File
# Begin Source File

SOURCE="Element_Def_Builder.cpp"
# End Source File
# Begin Source File

SOURCE="Encoding.cpp"
# End Source File
# Begin Source File

SOURCE="Env.cpp"
# End Source File
# Begin Source File

SOURCE="Exception.cpp"
# End Source File
# Begin Source File

SOURCE="FileCharStream.cpp"
# End Source File
# Begin Source File

SOURCE="HttpCharStream.cpp"
# End Source File
# Begin Source File

SOURCE="InputSource.cpp"
# End Source File
# Begin Source File

SOURCE="LocatorImpl.cpp"
# End Source File
# Begin Source File

SOURCE="Mem_Map_Stream.cpp"
# End Source File
# Begin Source File

SOURCE="NamespaceSupport.cpp"
# End Source File
# Begin Source File

SOURCE="SAXExceptions.cpp"
# End Source File
# Begin Source File

SOURCE="StrCharStream.cpp"
# End Source File
# Begin Source File

SOURCE="StreamFactory.cpp"
# End Source File
# Begin Source File

SOURCE="Transcode.cpp"
# End Source File
# Begin Source File

SOURCE="URL_Addr.cpp"
# End Source File
# Begin Source File

SOURCE="Validator.cpp"
# End Source File
# Begin Source File

SOURCE="XML_Codecs.cpp"
# End Source File
# Begin Source File

SOURCE="XMLFilterImpl.cpp"
# End Source File
# Begin Source File

SOURCE="ZipCharStream.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="ACEXML_Export.h"
# End Source File
# Begin Source File

SOURCE="Attributes.h"
# End Source File
# Begin Source File

SOURCE="Attributes_Def_Builder.h"
# End Source File
# Begin Source File

SOURCE="AttributesImpl.h"
# End Source File
# Begin Source File

SOURCE="CharStream.h"
# End Source File
# Begin Source File

SOURCE="ContentHandler.h"
# End Source File
# Begin Source File

SOURCE="DefaultHandler.h"
# End Source File
# Begin Source File

SOURCE="DTD_Manager.h"
# End Source File
# Begin Source File

SOURCE="DTDHandler.h"
# End Source File
# Begin Source File

SOURCE="Element_Def_Builder.h"
# End Source File
# Begin Source File

SOURCE="Encoding.h"
# End Source File
# Begin Source File

SOURCE="EntityResolver.h"
# End Source File
# Begin Source File

SOURCE="Env.h"
# End Source File
# Begin Source File

SOURCE="ErrorHandler.h"
# End Source File
# Begin Source File

SOURCE="Exception.h"
# End Source File
# Begin Source File

SOURCE="FileCharStream.h"
# End Source File
# Begin Source File

SOURCE="HttpCharStream.h"
# End Source File
# Begin Source File

SOURCE="InputSource.h"
# End Source File
# Begin Source File

SOURCE="Locator.h"
# End Source File
# Begin Source File

SOURCE="LocatorImpl.h"
# End Source File
# Begin Source File

SOURCE="Mem_Map_Stream.h"
# End Source File
# Begin Source File

SOURCE="NamespaceSupport.h"
# End Source File
# Begin Source File

SOURCE="SAXExceptions.h"
# End Source File
# Begin Source File

SOURCE="StrCharStream.h"
# End Source File
# Begin Source File

SOURCE="StreamFactory.h"
# End Source File
# Begin Source File

SOURCE="Transcode.h"
# End Source File
# Begin Source File

SOURCE="URL_Addr.h"
# End Source File
# Begin Source File

SOURCE="Validator.h"
# End Source File
# Begin Source File

SOURCE="XML_Codecs.h"
# End Source File
# Begin Source File

SOURCE="XML_Macros.h"
# End Source File
# Begin Source File

SOURCE="XML_Types.h"
# End Source File
# Begin Source File

SOURCE="XMLFilter.h"
# End Source File
# Begin Source File

SOURCE="XMLFilterImpl.h"
# End Source File
# Begin Source File

SOURCE="XMLReader.h"
# End Source File
# Begin Source File

SOURCE="ZipCharStream.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE="AttributesImpl.i"
# End Source File
# Begin Source File

SOURCE="DefaultHandler.i"
# End Source File
# Begin Source File

SOURCE="Env.i"
# End Source File
# Begin Source File

SOURCE="Exception.i"
# End Source File
# Begin Source File

SOURCE="LocatorImpl.i"
# End Source File
# Begin Source File

SOURCE="NamespaceSupport.i"
# End Source File
# Begin Source File

SOURCE="SAXExceptions.i"
# End Source File
# Begin Source File

SOURCE="Transcode.i"
# End Source File
# Begin Source File

SOURCE="URL_Addr.inl"
# End Source File
# Begin Source File

SOURCE="XMLFilterImpl.i"
# End Source File
# End Group
# End Target
# End Project
