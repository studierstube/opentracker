/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** Source file for OpenTracker. It contains global static definitions and
 * global functions etc. 
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <stdlib.h>
#include <string>

#include <ace/ACE.h>

#include "../dllinclude.h"

#include "../core/Event.h"
#include "../core/Context.h"
#include "../common/CommonNodeFactory.h"
#include "../core/TestModule.h"

// the following modules are not supported under Windows CE
#ifndef _WIN32_WCE
#include "../common/ConsoleModule.h"
#ifndef DARWIN
#include "../input/InterSenseModule.h"
#include "../input/ParButtonModule.h"
#endif //DARWIN
#include "../input/XSensModule.h"
#include "../input/DynaSightModule.h"
#include "../input/FastTrakModule.h"
#include "../input/FOBModule.h"
#include "../input/ARTDataTrackerModule.h"
#endif //_WIN32_WCE

#include "../network/NetworkSinkModule.h"
#include "../network/NetworkSourceModule.h"
#include "../common/TimeModule.h"
#include "../common/FileModule.h"
#include "../network/TCPModule.h"

#include "../input/UltraTrakModule.h"
#include "../common/GroupGateModule.h"
#include "../common/InterpolatorModule.h"
#include "../common/ButtonHoldFilterModule.h"
#include "../input/GPSModule.h"
#include "../input/MagicYModule.h"
#include "../common/CallbackModule.h"
#include "../common/LogModule.h"

// these modules depend on compile options
#include "../input/ARToolKitModule.h"
#include "../input/ARToolKitPlusModule.h"
#include "../input/CyberMouseModule.h"
#include "../input/TargusModule.h"
#include "../input/LinmouseModule.h"
#include "../input/WacomGraphireModule.h"
#include "../input/JoystickModule.h"
#include "../input/SpaceMouseModule.h"
#include "../input/SpeechModule.h"
#include "../input/P5GloveModule.h"
#include "../input/MulticastInputModule.h"
#include "../input/UbisenseModule.h"
#include "../input/OpenVideoModule.h"
#include "../network/DwarfModule.h"
#include "../network/VRPNModule.h"

// DLL main function

#ifdef WIN32

#ifndef OPENTRACKER_STATIC
BOOL APIENTRY DllMain( HANDLE hModule, 
		       DWORD  ul_reason_for_call, 
		       LPVOID lpReserved
		       )
{
    switch( ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH:		
            ACE::init();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:		
            break;
        case DLL_PROCESS_DETACH:		
            ACE::fini();
            break;	
    }
    return TRUE;
}
#endif //OPENTRACKER_STATIC

#endif //WIN32

// initializes context

namespace ot {

    void OPENTRACKER_API initializeContext( Context & context )
    {
        // register the creator functions and generic names of all known types
        Event::registerAllKnownTypes();

        // Instance the default modules and add to factory and parser
        CommonNodeFactory * common = new CommonNodeFactory;
        context.addFactory( *common );

        LogModule * log = new LogModule;
        context.addModule( "LogConfig", *log );

#ifndef OT_NO_TESTMODULE_SUPPORT
        TestModule * test = new TestModule;
        context.addFactory( *test );
        // actually it doesn't have a configuration element
        context.addModule( "TestConfig", *test );
#endif

#ifndef OT_NO_NETWORK_SUPPORT
        NetworkSinkModule * networksink = new NetworkSinkModule;
        context.addFactory( *networksink );
        context.addModule( "NetworkSinkConfig", *networksink );
#endif


#ifndef _WIN32_WCE
#ifndef OT_NO_CONSOLE_SUPPORT
        ConsoleModule * console = new ConsoleModule ;
        context.addFactory( *console );
        context.addModule( "ConsoleConfig", *console );
#endif

#ifndef OT_NO_NETWORK_SUPPORT
        NetworkSourceModule * network = new NetworkSourceModule;
        context.addFactory( * network );
        context.addModule( "NetworkSourceConfig", *network );    
#endif
#endif //_WIN32_WCE

#ifdef USE_ARTOOLKIT
        ARToolKitModule * artool = new ARToolKitModule;
        context.addFactory( * artool );
        context.addModule( "ARToolKitConfig", *artool );
#endif

#ifdef USE_ARTOOLKITPLUS
        // Create an ARToolKitPlusModule instance
        ARToolKitPlusModule *artoolplus = new ARToolKitPlusModule;
        context.addFactory( * artoolplus );
        context.addModule( "ARToolKitPlusConfig", *artoolplus );
        context.registerVideoUser(artoolplus);
#endif

#ifdef USE_OPENVIDEO
        OpenVideoModule * ovModule = new OpenVideoModule(&context);
        context.addModule( "OpenVideoConfig", *ovModule );
#endif

#ifdef USE_WACOMGRAPHIRE
        WacomGraphireModule * wacom = new WacomGraphireModule;
        context.addFactory( * wacom );
        context.addModule( "WacomGraphireConfig", * wacom );
#endif

#ifdef USE_CYBERMOUSE
        CyberMouseModule * cmouse = new CyberMouseModule;
        context.addFactory( * cmouse );
        context.addModule( "CyberMouseConfig", * cmouse );
#endif	

#ifdef USE_LINMOUSEMODULE
        LinmouseModule * linmousemodule = new LinmouseModule;
        context.addFactory( * linmousemodule );
        context.addModule( "LinmouseConfig", * linmousemodule );
#endif	

#ifdef USE_TARGUSMODULE
        TargusModule * targusmodule = new TargusModule;
        context.addFactory( * targusmodule );
        context.addModule( "TargusConfig", * targusmodule );
#endif	

#ifdef USE_JOYSTICK
        JoystickModule * joy = new JoystickModule();
        context.addFactory( *joy );
        context.addModule( "JoystickConfig", *joy );
#endif

#ifdef USE_SPACEMOUSE
        SpaceMouseModule * smouse = new SpaceMouseModule;
        context.addFactory( * smouse );
        context.addModule( "SpaceMouseConfig", * smouse );
#endif	

#ifndef OT_NO_NETWORK_SUPPORT
        MulticastInputModule *mcinput = new MulticastInputModule;
        context.addFactory( * mcinput );
        context.addModule( "MulticastInputConfig", * mcinput );
#endif

#ifndef _WIN32_WCE
#ifndef DARWIN
#ifndef OT_NO_INTERSENSE_SUPPORT
        InterSenseModule * intersense = new InterSenseModule;
        context.addFactory( * intersense );
        context.addModule( "InterSenseConfig", * intersense );
#endif
#endif DARWIN
#endif //_WIN32_WCE

#ifndef OT_NO_TIMEMODULE_SUPPORT
        TimeModule * time = new TimeModule();
        context.addModule( "TimeConfig", * time );
#endif

#ifndef OT_NO_FILEMODULE_SUPPORT
        FileModule * file = new FileModule();
        context.addFactory( * file );
        context.addModule( "FileConfig", * file );
#endif

#ifndef _WIN32_WCE
#ifndef DARWIN
#ifndef OT_NO_PARBUTTON_SUPPORT
        ParButtonModule * parbutton = new ParButtonModule();
        context.addFactory( *parbutton );
        context.addModule( "ParButtonConfig", *parbutton );
#endif
#endif //DARWIN
#endif //_WIN32_WCE

#ifndef OT_NO_NETWORK_SUPPORT
        TCPModule * tcp = new TCPModule();
        context.addFactory( *tcp );
        context.addModule( "TCPConfig", *tcp );
#endif

#ifndef _WIN32_WCE
#ifndef OT_NO_FOB_SUPPORT
        FOBModule * fob = new FOBModule();
        context.addFactory( *fob );
        context.addModule( "FOBConfig", *fob );
#endif
#endif //_WIN32_WCE

#ifndef _WIN32_WCE
#ifndef OT_NO_FASTTRACK_SUPPORT
        FastTrakModule * ftrak = new FastTrakModule();
        context.addFactory( *ftrak );
        context.addModule( "FastTrakConfig", *ftrak );
#endif
#endif //_WIN32_WCE

#ifndef _WIN32_WCE
#ifndef OT_NO_ARTDATATRACKER_SUPPORT
        ARTDataTrackerModule * dtrak = new ARTDataTrackerModule();
        context.addFactory( *dtrak );
        context.addModule( "ARTDataTrackerConfig", *dtrak );
#endif
#endif //_WIN32_WCE

#ifndef OT_NO_ULTRATRACK_SUPPORT
        UltraTrakModule * ultra = new UltraTrakModule;
        context.addFactory( * ultra );
        context.addModule( "UltraTrakConfig", *ultra );
#endif

#ifndef OT_NO_GROUPGATE_SUPPORT
        GroupGateModule * groupgate = new GroupGateModule();
        context.addFactory( * groupgate );
        context.addModule( "GroupGateConfig", *groupgate ); 
#endif
    
#ifndef OT_NO_SPEECH_SUPPORT
        SpeechModule *speechmodule = new SpeechModule;
        context.addFactory( *speechmodule );
        context.addModule( "SpeechRecoConfig", *speechmodule );
#endif

#ifndef OT_NO_INTERPORLATORMODULE_SUPPORT
        InterpolatorModule * ipol = new InterpolatorModule;
        context.addFactory( *ipol );
        // actually it doesn't have a configuration element
        context.addModule( "InterpolatorConfig", *ipol );
#endif

#ifndef OT_NO_BUTTON_SUPPORT
        ButtonHoldFilterModule * buttonHoldFiler = new ButtonHoldFilterModule;
        context.addFactory( *buttonHoldFiler );
        // actually it doesn't have a configuration element
        context.addModule( "ButtonHoldFilterConfig", *buttonHoldFiler );
#endif

#ifdef USE_P5GLOVE
        P5GloveModule *p5glovemodule = new P5GloveModule;
        context.addFactory( *p5glovemodule );
        context.addModule( "P5GloveConfig", *p5glovemodule );
#endif

#ifndef OT_NO_GPS_SUPPORT
        GPSModule * gps = new GPSModule;
        context.addFactory( * gps );
        context.addModule( "GPSConfig", * gps );
#endif

#ifndef _WIN32_WCE
#ifndef OT_NO_DYNASIGHT_SUPPORT
        DynaSightModule * dynasight = new DynaSightModule;
        context.addFactory( * dynasight );
        context.addModule( "DynaSightConfig", *dynasight );
#endif
#endif //_WIN32_WCE

#ifndef OT_NO_MAGICY_SUPPORT
        MagicYModule * magicY = new MagicYModule;
        context.addFactory( * magicY );
        context.addModule( "MagicYConfig", *magicY );
#endif
    
#ifndef OT_NO_CALLBACKMODULE_SUPPORT
	CallbackModule  * cbModule = new CallbackModule;
        context.addFactory( *cbModule );
        context.addModule( "CallbackConfig", *cbModule );
#endif

#ifdef USE_DWARF
        DwarfModule * dwarf = new DwarfModule;
        context.addFactory( *dwarf);
        context.addModule( "DwarfConfig", *dwarf );
#endif

#ifdef USE_VRPN
        VRPNModule * vrpn = new VRPNModule;
        context.addFactory( *vrpn );
        context.addModule( "VRPNConfig", *vrpn );
#endif

#ifndef _WIN32_WCE
#ifndef OT_NO_XSENS_SUPPORT
        XSensModule * xsens = new XSensModule;
        context.addFactory( *xsens );
        context.addModule( "XSensConfig", *xsens );
#endif
#endif //_WIN32_WCE

#ifdef USE_UBISENSE
        UbisenseModule * ubisense = new UbisenseModule;
        context.addFactory( * ubisense );
        context.addModule( "UbisenseConfig", *ubisense );
#endif
    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of OpenTracker.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
