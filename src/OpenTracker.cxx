 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Source file for OpenTracker. It contains global static definitions and
  * global functions etc. 
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/Attic/OpenTracker.cxx,v 1.7 2001/03/06 18:09:28 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include <ace/ACE.h>

#include "config.h"

#include "core/State.h"
#include "core/Context.h"
#include "common/CommonNodeFactory.h"
#include "core/TestModule.h"
#include "common/ConsoleModule.h"
#include "network/NetworkSinkModule.h"
#include "network/NetworkSourceModule.h"

// these modules depend on compile options
#include "input/InterTraxModule.h"
#include "input/ARToolKitModule.h"
#include "input/CyberMouseModule.h"
#include "input/WacomGraphireModule.h"

/** a global unique variable null, for typesafe null references as return
 *  values of methods.*/
State State::null;

// initializes context

void initializeContext( Context & context )
{
   // Instance the default modules and add to factory and parser
    CommonNodeFactory * common = new CommonNodeFactory;
    context.addFactory( *common );

    TestModule * test = new TestModule;
    context.addFactory( *test );
    // actually it doesn't have a configuration element
    context.addModule( ( string)"TestConfig", *test );

    NetworkSinkModule * networksink = new NetworkSinkModule;
    context.addFactory( *networksink );
    context.addModule( (string)"NetworkSinkConfig", *networksink );

    ConsoleModule * console = new ConsoleModule ;
    context.addFactory( *console );
    context.addModule( (string)"ConsoleConfig", *console );
    
    NetworkSourceModule * network = new NetworkSourceModule;
    context.addFactory( * network );
    context.addModule( (string)"NetworkSourceConfig", *network );    

#ifdef USE_ARTOOLKIT
    ARToolKitModule * artool = new ARToolKitModule;
    context.addFactory( * artool );
    context.addModule( (string)"ARToolKitConfig", *artool );
#endif

#ifdef USE_INTERTRAX
    InterTraxModule * intertrax = new InterTraxModule;
    context.addFactory( * intertrax );
    context.addModule( (string)"InterTraxConfig", * intertrax );
#endif

#ifdef USE_WACOMGRAPHIRE
    WacomGraphireModule * wacom = new WacomGraphireModule;
    context.addFactory( * wacom );
    context.addModule( (string)"WacomGraphireConfig", * wacom );
#endif

#ifdef USE_CYBERMOUSE
    CyberMouseModule * cmouse = new CyberMouseModule;
    context.addFactory( * cmouse );
    context.addModule( (string)"CyberMouseConfig", * cmouse );
#endif
}
