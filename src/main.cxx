// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      main.cxx
//  TYPE:      c++ source file
//  PROJECT:   maxwell tracker server
//  CONTENT:   Implementation main method
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

// Master include file
#include "TrackerServer.h"

#ifdef WIN32
#include <iostream>    // VisualC++ has two incompatible iostreams libraries !
#else
#include <iostream.h>
#endif

/*** 
 * main
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
    cout << "Context established." << endl;
    
    // parse the configuration file, this initializes the modules
    // and builds the tracker tree.
    context.parseConfiguration( argv[1] );
    cout << "Parsing complete." << endl << endl << "Starting mainloop !" << endl;
    
    // start the tracker main loop
    context.run();
    return 0;
}
