 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file containing the main function for standalone use.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/misc/main.cxx,v 1.1 2001/03/24 23:50:02 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "../OpenTracker.h"

#ifdef WIN32
#include <iostream>    // new IOStream for VisualC++
#else
#include <iostream.h>
#endif

/**
 * The main function for the standalone program. It expects a
 * filename as argument, tries to parse the configuration file
 * and starts the main loop, if successful
 */
int main(int argc, char **argv)
{
    if( argc != 2 )
    {
        cout << "Usage : " << argv[0] << " configfile" << endl;
        return 1;
    }

    // important parts of the system
    // get a context, the default modules and factories are
    // added allready.
    Context context;
    initializeContext( context );
    cout << "Context established." << endl;

    // parse the configuration file, builds the tracker tree
    context.parseConfiguration( string(argv[1]) );
    cout << "Parsing complete." << endl << endl << "Starting mainloop !" << endl;

    // initializes the modules and starts the tracker main loop
    context.run();
    return 0;
}
