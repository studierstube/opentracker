# Microsoft Developer Studio Project File - Name="opentracker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=opentracker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "opentracker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "opentracker.mak" CFG="opentracker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "opentracker - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "opentracker - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "opentracker - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENTRACKER_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "extras" /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "extras/intersense" /I "$(ARTOOLKITROOT)/include" /I "../wintab/include" /I "$(VRPNROOT)/vrpn" /D "OS_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENTRACKER_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ace.lib xerces-c_2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"bin/opentracker.dll" /implib:"lib/opentracker.lib" /libpath:"$(ACEROOT)/ace" /libpath:"$(ACEROOT)/lib" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(ARTOOLKITROOT)/lib.vc60" /libpath:"../wintab/lib" /libpath:"$(VRPNROOT)/vrpn/pc_win32/Release"
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "opentracker - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENTRACKER_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /Gi /GX /ZI /Od /I "extras" /I "$(ACEROOT)" /I "$(XERCESCROOT)/include" /I "extras/intersense" /I "$(ARTOOLKITROOT)/include" /I "../wintab/include" /I "$(VRPNROOT)/vrpn" /D "OS_WIN32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "OPENTRACKER_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib xerces-c_2d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /debug /machine:I386 /out:"bin/opentrackerd.dll" /implib:"lib/opentrackerd.lib" /libpath:"$(ACEROOT)/ace" /libpath:"$(ACEROOT)/lib" /libpath:"$(XERCESCROOT)/lib" /libpath:"$(ARTOOLKITROOT)/lib.vc60" /libpath:"../wintab/lib" /libpath:"$(VRPNROOT)/vrpn/pc_win32/Debug"

!ENDIF 

# Begin Target

# Name "opentracker - Win32 Release"
# Name "opentracker - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\input\ARTDataTrackerChomp.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\ARTDataTrackerModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\ARToolKitModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ButtonFilterNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ButtonHoldFilterModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ButtonHoldFilterNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ButtonOpNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\CallbackModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\CommonNodeFactory.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ConfidenceFilterNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ConfidenceSelectNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\ConfigurationParser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ConsoleModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\Context.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\CyberMouseModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\DGPSIP_Handler.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\DGPSMirror_Handler.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\DOMTreeErrorReporter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\network\DwarfModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\DynamicTransformation.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\DynaSightModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ElasticFilterNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\EllipsoidTransformNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\EventQueueImplementation.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\EventQueueNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\FastTrakModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\FileModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\FilterNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\FOBModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\GKTransformNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\GPS_Handler.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSDriver.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSParser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\GroupGateModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\GroupGateNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\InterpolatorModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\InterSenseModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\InvertTransformation.cxx
# End Source File
# Begin Source File

SOURCE=.\extras\intersense\isense.c
# End Source File
# Begin Source File

SOURCE=.\src\input\JoystickModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\MagicYModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\MathUtils.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\MatrixTransformation.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\MergeNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\MulticastInputModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSinkModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSourceModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\Node.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\NodeFactoryContainer.cxx
# End Source File
# Begin Source File

SOURCE=.\src\misc\OpenTracker.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\OSUtils.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\P5GloveModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\ParButtonModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\misc\portio.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\PositionFilterNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\RangeFilterNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\rtcm.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\SelectionNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\misc\serialcomm.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\SpaceMouseModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechCore.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechSet.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechSource.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechVoice.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechVoiceModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\State.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\StaticTransformation.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\StringTable.cxx
# End Source File
# Begin Source File

SOURCE=.\src\network\TCPModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\TestModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\core\ThreadModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\ThresholdFilterNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\TimeGateNode.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\TimeModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\Transformation.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\UltraTrakModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\common\VirtualTransformation.cxx
# End Source File
# Begin Source File

SOURCE=.\src\network\VRPNModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\network\VRPNSink.cxx
# End Source File
# Begin Source File

SOURCE=.\src\network\VRPNSource.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\WacomGraphireModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\misc\xml\XMLWriter.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\XSensModule.cxx
# End Source File
# Begin Source File

SOURCE=.\src\input\XSensSource.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\input\ARTDataTrackerChomp.h
# End Source File
# Begin Source File

SOURCE=.\src\input\ARTDataTrackerModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\ARTDataTrackerSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\ARToolKitModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\ARToolKitSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ButtonFilterNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ButtonHoldFilterModule.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ButtonHoldFilterNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ButtonOpNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\CallbackModule.h
# End Source File
# Begin Source File

SOURCE=.\src\common\CallbackNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\CommonNodeFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ConfidenceFilterNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ConfidenceSelectNode.h
# End Source File
# Begin Source File

SOURCE=.\config_win.h
# End Source File
# Begin Source File

SOURCE=.\src\core\ConfigNode.h
# End Source File
# Begin Source File

SOURCE=.\src\core\ConfigurationParser.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ConsoleModule.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ConsoleSink.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ConsoleSource.h
# End Source File
# Begin Source File

SOURCE=.\src\core\Context.h
# End Source File
# Begin Source File

SOURCE=.\src\input\CyberMouseModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\CyberMouseSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\DGPSIP_Handler.h
# End Source File
# Begin Source File

SOURCE=.\src\input\DGPSMirror_Handler.h
# End Source File
# Begin Source File

SOURCE=.\src\dllinclude.h
# End Source File
# Begin Source File

SOURCE=.\src\core\DOMTreeErrorReporter.h
# End Source File
# Begin Source File

SOURCE=.\src\network\DwarfModule.h
# End Source File
# Begin Source File

SOURCE=.\src\network\DwarfSink.h
# End Source File
# Begin Source File

SOURCE=.\src\network\DwarfSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\DynamicTransformation.h
# End Source File
# Begin Source File

SOURCE=.\src\input\DynaSightModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\DynaSightSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ElasticFilterNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\EllipsoidTransformNode.h
# End Source File
# Begin Source File

SOURCE=.\src\core\EventQueueImplementation.h
# End Source File
# Begin Source File

SOURCE=.\src\common\EventQueueNode.h
# End Source File
# Begin Source File

SOURCE=.\src\input\FastTrakModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\FastTrakSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\File.h
# End Source File
# Begin Source File

SOURCE=.\src\common\FileModule.h
# End Source File
# Begin Source File

SOURCE=.\src\common\FileSink.h
# End Source File
# Begin Source File

SOURCE=.\src\common\FileSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\FilterNode.h
# End Source File
# Begin Source File

SOURCE=.\src\input\FOBModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\FOBSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\GKTransformNode.h
# End Source File
# Begin Source File

SOURCE=.\src\input\GPS_Handler.h
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSDirectionSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSDriver.h
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSGarminAltitude.h
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSGarminCompass.h
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSInfoSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSParser.h
# End Source File
# Begin Source File

SOURCE=.\src\input\GPSSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\GroupGateModule.h
# End Source File
# Begin Source File

SOURCE=.\src\common\GroupGateNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\InterpolatorModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\InterSenseModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\InterSenseSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\InvertTransformation.h
# End Source File
# Begin Source File

SOURCE=.\extras\intersense\isense.h
# End Source File
# Begin Source File

SOURCE=.\src\input\JoystickModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\JoystickSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\MagicYModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\MagicYSource.h
# End Source File
# Begin Source File

SOURCE=.\src\core\MathUtils.h
# End Source File
# Begin Source File

SOURCE=.\src\common\MatrixTransformation.h
# End Source File
# Begin Source File

SOURCE=.\src\common\MergeNode.h
# End Source File
# Begin Source File

SOURCE=.\src\core\Module.h
# End Source File
# Begin Source File

SOURCE=.\src\input\MulticastInputModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\MulticastInputSource.h
# End Source File
# Begin Source File

SOURCE=.\src\network\Network.h
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSink.h
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSinkModule.h
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSource.h
# End Source File
# Begin Source File

SOURCE=.\src\network\NetworkSourceModule.h
# End Source File
# Begin Source File

SOURCE=.\src\core\Node.h
# End Source File
# Begin Source File

SOURCE=.\src\core\NodeFactory.h
# End Source File
# Begin Source File

SOURCE=.\src\core\NodeFactoryContainer.h
# End Source File
# Begin Source File

SOURCE=.\src\core\NodePort.h
# End Source File
# Begin Source File

SOURCE=.\src\OpenTracker.h
# End Source File
# Begin Source File

SOURCE=.\src\core\OSUtils.h
# End Source File
# Begin Source File

SOURCE=.\src\input\P5GloveModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\P5GloveSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\ParButtonModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\ParButtonSource.h
# End Source File
# Begin Source File

SOURCE=.\src\misc\portio.h
# End Source File
# Begin Source File

SOURCE=.\src\common\PositionFilterNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\RangeFilterNode.h
# End Source File
# Begin Source File

SOURCE=.\src\core\RefNode.h
# End Source File
# Begin Source File

SOURCE=.\src\input\rtcm.h
# End Source File
# Begin Source File

SOURCE=.\src\common\SelectionNode.h
# End Source File
# Begin Source File

SOURCE=.\src\misc\serialcomm.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpaceMouseModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpaceMouseSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechCore.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechDef.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechInc.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechSet.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechVoice.h
# End Source File
# Begin Source File

SOURCE=.\src\input\SpeechVoiceModule.h
# End Source File
# Begin Source File

SOURCE=.\src\core\State.h
# End Source File
# Begin Source File

SOURCE=.\src\common\StaticTransformation.h
# End Source File
# Begin Source File

SOURCE=.\src\core\StringTable.h
# End Source File
# Begin Source File

SOURCE=.\src\network\TCPModule.h
# End Source File
# Begin Source File

SOURCE=.\src\network\TCPSink.h
# End Source File
# Begin Source File

SOURCE=.\src\core\TestModule.h
# End Source File
# Begin Source File

SOURCE=.\src\core\TestSource.h
# End Source File
# Begin Source File

SOURCE=.\src\core\ThreadModule.h
# End Source File
# Begin Source File

SOURCE=.\src\common\ThresholdFilterNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\TimeGateNode.h
# End Source File
# Begin Source File

SOURCE=.\src\common\TimeModule.h
# End Source File
# Begin Source File

SOURCE=.\src\common\Transformation.h
# End Source File
# Begin Source File

SOURCE=.\extras\intersense\types.h
# End Source File
# Begin Source File

SOURCE=.\src\input\UltraTrakModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\UltraTrakSource.h
# End Source File
# Begin Source File

SOURCE=.\src\common\VirtualTransformation.h
# End Source File
# Begin Source File

SOURCE=.\src\network\VRPNModule.h
# End Source File
# Begin Source File

SOURCE=.\src\network\VRPNSink.h
# End Source File
# Begin Source File

SOURCE=.\src\network\VRPNSource.h
# End Source File
# Begin Source File

SOURCE=.\src\input\WacomGraphireModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\WacomGraphireSource.h
# End Source File
# Begin Source File

SOURCE=.\src\misc\xml\XMLWriter.h
# End Source File
# Begin Source File

SOURCE=.\src\input\XSensModule.h
# End Source File
# Begin Source File

SOURCE=.\src\input\XSensSource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
