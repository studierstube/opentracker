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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/Attic/OpenTracker.cxx,v 1.1 2001/01/03 14:46:36 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "network/NetworkSinkModule.h"
#include "core/State.h"
#include "core/Context.h"
#include "core/CommonNodeFactory.h"
#include "core/TestModule.h"
#include "output/ConsoleModule.h"
#include "network/NetworkSourceModule.h"
#include "input/InterTraxModule.h"
#include "input/ARToolKitModule.h"

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

    /*
    VideoModule * video = new VideoModule;
    addFactory( *video );
    addModule( "Video", *video );
    */

    NetworkSinkModule * networksink = new NetworkSinkModule;
    context.addFactory( *networksink );
    context.addModule( (string)"NetworkSinkConfig", *networksink );

    ConsoleModule * console = new ConsoleModule ;
    context.addFactory( *console );
    context.addModule( (string)"ConsoleConfig", *console );
    
    NetworkSourceModule * network = new NetworkSourceModule;
    context.addFactory( * network );
    context.addModule( (string)"NetworkSourceConfig", *network );    

    ARToolKitModule * artool = new ARToolKitModule;
    context.addFactory( * artool );
    context.addModule( (string)"ARToolKitConfig", *artool );
}