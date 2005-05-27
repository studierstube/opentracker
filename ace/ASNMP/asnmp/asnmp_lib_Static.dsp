# Microsoft Developer Studio Project File - Name="asnmp_lib_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=asnmp_lib_Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "asnmp_lib_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "asnmp_lib_Static.mak" CFG="asnmp_lib_Static - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "asnmp_lib_Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "asnmp_lib_Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "asnmp_lib_Static - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release\asnmp_lib_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\.." /I "..\..\ASNMP" /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\.." /i "..\..\ASNMP"
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\lib\asnmp.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\lib\asnmps.lib"

!ELSEIF  "$(CFG)" == "asnmp_lib_Static - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug\asnmp_lib_Static"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\.." /I "..\..\ASNMP" /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_AS_STATIC_LIBS /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\.." /i "..\..\ASNMP"
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\lib\asnmp.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"..\..\lib\asnmpsd.lib"

!ENDIF

# Begin Target

# Name "asnmp_lib_Static - Win32 Release"
# Name "asnmp_lib_Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE="address.cpp"
# End Source File
# Begin Source File

SOURCE="asn1.cpp"
# End Source File
# Begin Source File

SOURCE="counter.cpp"
# End Source File
# Begin Source File

SOURCE="ctr64.cpp"
# End Source File
# Begin Source File

SOURCE="gauge.cpp"
# End Source File
# Begin Source File

SOURCE="integer.cpp"
# End Source File
# Begin Source File

SOURCE="octet.cpp"
# End Source File
# Begin Source File

SOURCE="oid.cpp"
# End Source File
# Begin Source File

SOURCE="pdu.cpp"
# End Source File
# Begin Source File

SOURCE="sagent.cpp"
# End Source File
# Begin Source File

SOURCE="snmp.cpp"
# End Source File
# Begin Source File

SOURCE="target.cpp"
# End Source File
# Begin Source File

SOURCE="timetick.cpp"
# End Source File
# Begin Source File

SOURCE="transaction.cpp"
# End Source File
# Begin Source File

SOURCE="vb.cpp"
# End Source File
# Begin Source File

SOURCE="wpdu.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE="address.h"
# End Source File
# Begin Source File

SOURCE="asn1.h"
# End Source File
# Begin Source File

SOURCE="counter.h"
# End Source File
# Begin Source File

SOURCE="ctr64.h"
# End Source File
# Begin Source File

SOURCE="gauge.h"
# End Source File
# Begin Source File

SOURCE="integer.h"
# End Source File
# Begin Source File

SOURCE="octet.h"
# End Source File
# Begin Source File

SOURCE="oid.h"
# End Source File
# Begin Source File

SOURCE="pdu.h"
# End Source File
# Begin Source File

SOURCE="sagent.h"
# End Source File
# Begin Source File

SOURCE="snmp.h"
# End Source File
# Begin Source File

SOURCE="target.h"
# End Source File
# Begin Source File

SOURCE="timetick.h"
# End Source File
# Begin Source File

SOURCE="transaction.h"
# End Source File
# Begin Source File

SOURCE="vb.h"
# End Source File
# Begin Source File

SOURCE="wpdu.h"
# End Source File
# End Group
# End Target
# End Project
