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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/Attic/OpenTracker.cxx,v 1.2 2001/01/28 16:49:11 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "core/State.h"
#include "core/Context.h"
#include "core/CommonNodeFactory.h"
#include "core/TestModule.h"
#include "output/ConsoleModule.h"
#include "network/NetworkSinkModule.h"
#include "network/NetworkSourceModule.h"
#ifdef USER_INTERTRAX
#include "input/InterTraxModule.h"
#endif
#ifdef USE_ARTOOLKIT
#include "input/ARToolKitModule.h"
#endif

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
}
