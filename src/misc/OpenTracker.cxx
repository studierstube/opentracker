 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Source file for OpenTracker. It contains global static definitions and
  * global functions etc. 
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/misc/OpenTracker.cxx,v 1.20 2002/10/31 20:02:56 splechtna Exp $
  * @file                                                                   */
 /* ======================================================================= */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <string>

#include <ace/ACE.h>

#include "../dllinclude.h"

#include "../core/State.h"
#include "../core/Context.h"
#include "../common/CommonNodeFactory.h"
#include "../core/TestModule.h"
#include "../common/ConsoleModule.h"
#include "../network/NetworkSinkModule.h"
#include "../network/NetworkSourceModule.h"
#include "../input/InterSenseModule.h"
#include "../common/TimeModule.h"
#include "../common/FileModule.h"
#include "../input/ParButtonModule.h"
#include "../input/FOBModule.h"
#include "../network/TCPModule.h"
#include "../input/FastTrakModule.h"
#include "../input/ARTDataTrackerModule.h"
#include "../input/UltratrakModule.h"

// these modules depend on compile options
#include "../input/ARToolKitModule.h"
#include "../input/CyberMouseModule.h"
#include "../input/WacomGraphireModule.h"
#include "../input/JoystickModule.h"
#include "../input/SpaceMouseModule.h"

// DLL main function

#ifdef WIN32

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
#endif

// initializes context

void OPENTRACKER_API initializeContext( Context & context )
{
    // Instance the default modules and add to factory and parser
    CommonNodeFactory * common = new CommonNodeFactory;
    context.addFactory( *common );

    TestModule * test = new TestModule;
    context.addFactory( *test );
    // actually it doesn't have a configuration element
    context.addModule( "TestConfig", *test );

    NetworkSinkModule * networksink = new NetworkSinkModule;
    context.addFactory( *networksink );
    context.addModule( "NetworkSinkConfig", *networksink );
	
    ConsoleModule * console = new ConsoleModule ;
    context.addFactory( *console );
    context.addModule( "ConsoleConfig", *console );

    NetworkSourceModule * network = new NetworkSourceModule;
    context.addFactory( * network );
    context.addModule( "NetworkSourceConfig", *network );    

#ifdef USE_ARTOOLKIT
    ARToolKitModule * artool = new ARToolKitModule;
    context.addFactory( * artool );
    context.addModule( "ARToolKitConfig", *artool );
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

    InterSenseModule * intersense = new InterSenseModule;
    context.addFactory( * intersense );
    context.addModule( "InterSenseConfig", * intersense );
    
    TimeModule * time = new TimeModule();
    context.addModule( "TimeConfig", * time );
	
    FileModule * file = new FileModule();
    context.addFactory( * file );
    context.addModule( "FileConfig", * file );
    
    ParButtonModule * parbutton = new ParButtonModule();
    context.addFactory( *parbutton );
    context.addModule( "ParButtonConfig", *parbutton );

    TCPModule * tcp = new TCPModule();
    context.addFactory( *tcp );
    context.addModule( "TCPConfig", *tcp );

    FOBModule * fob = new FOBModule();
    context.addFactory( *fob );
    context.addModule( "FOBConfig", *fob );

    FastTrakModule * ftrak = new FastTrakModule();
    context.addFactory( *ftrak );
    context.addModule( "FastTrakConfig", *ftrak );

    ARTDataTrackerModule * dtrak = new ARTDataTrackerModule();
    context.addFactory( *dtrak );
    context.addModule( "ARTDataTrackerConfig", *dtrak );

    UltraTrakModule * ultra = new UltraTrakModule;
    context.addFactory( * ultra );
    context.addModule( "UltraTrakConfig", *ultra );    
}
