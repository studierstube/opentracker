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
/** source file containing the main function and test code 
  * for the test program use.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/misc/test.cxx,v 1.3 2001/07/16 21:43:52 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include <OpenTracker.h>

#ifdef WIN32
#include <iostream>    // new IOStream for VisualC++
#else
#include <iostream.h>
#endif

using namespace std;

#include <common/CallbackModule.h>

CallbackFunction testCB;

/**
 * The main function for the test program. It expects a
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

    cout << "OpenTracker Test program ...\n";

    // important parts of the system
    // get a context, the default modules and factories are
    // added allready.
    Context context( 1 );
    // initializeContext( context );
    CallbackModule  * cbModule = new CallbackModule;
    context.addFactory( *cbModule );
    context.addModule( "CallbackConfig", *cbModule );
    
    cout << "Context established." << endl;

    // parse the configuration file, builds the tracker tree
    context.parseConfiguration( argv[1] );
  //  cbModule->setCallback( "test1", &testCB );
    
    cout << "Parsing complete." << endl << endl << "Press return to start mainloop !" << endl;    
     
    cin.get();
    
    // initializes the modules and starts the tracker main loop
    context.start();
    for( int i = 0; i < 10; i++ ){
        context.pushStates();
        context.pullStates();
        context.stop();
    }
    cout << "Doing some operations" << endl;

    Node * myNode = context.findNode( "Test" );
    if( myNode != NULL ){
        cout << "Found node " << myNode->getName() << " of type " << myNode->getType() << endl;
        StringTable table;
        table.put( "translation", "1 1 1" );
        table.put( "scale", "0.5 0.5 0.5" );
        table.put( "rotationtype", "quaternion" );
        table.put( "rotation",  "0 1 0 0" );
        Node * newNode = context.createNode( "EventTransform", table );
        if( newNode != NULL ){
            cout << "Created new Node of Type " << newNode->getType() << endl;
            Node * parent = myNode->getParent();
            if( parent != NULL ){
                cout << "Parent of myNode is " << parent->getType() << endl;
                parent->removeChild( *myNode );
                parent->addChild( *newNode );
                newNode->addChild( *myNode );
                cout << "Inserted newNode between parent and myNode !\n";
            }
        }
    }
    cout << "Starting context again !\n";
    while( !context.stop()){
        context.pushStates();
        context.pullStates();
    }
    context.close();
    return 0;
}

void testCB( const State & event ){
    cout << "Got event !\n";
}
