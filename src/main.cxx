// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      main.cxx
//  TYPE:      c++ source file
//  PROJECT:   @INSERT_PROJECTNAME
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

#include <iostream.h>

/*** 
 * main
 */
int main(int argc, char **argv)
{
    if( argc != 2 ){
	    cout << "Usage : " << argv[0] << " configfile" << endl;
    	return 1;
    }

    // important parts of the system
    NodeFactoryContainer factory;
    CommonNodeFactory common;
    VideoSource video;
    NetworkDriver driver;
    
    cout << "Initialized system." << endl;

    // add factories to our main FactoryContainer
    factory.addFactory( common );
    factory.addFactory( video );
    factory.addFactory( driver );

    cout << "Added factories to container." << endl;

    // configuration file parser
    ConfigurationParser parser( &factory );

    cout << "Initialized parser." << endl;

    // add modules to the parser
    parser.addModule( "Network", driver );
    parser.addModule( "Video", video );

    cout << "Added modules to parser." << endl;

    NodeVector * vector = parser.parseConfigurationFile( argv[1] );

    cout << "Got " << vector->size() << " root nodes." << endl;
    cout << "Parsing complete." << endl;
    return 0;
}
