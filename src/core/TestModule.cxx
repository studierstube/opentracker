 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for TestModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/TestModule.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "TestModule.h"
#include "TestSource.h"

#include <stdio.h>
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

/// Destructor method

TestModule::~TestModule()
{
    nodes.clear();
}

/// This method is called to ruct a new Node.

Node * TestModule::createNode( string& name,
                               StringMap& attributes)
{
    if( name.compare("TestSource") == 0 )
    {
        int frequency;
        int offset;
        int num = sscanf(attributes.find("frequency")->second.c_str(), " %i", &frequency );
        if( num == 0 ){
            frequency = 1;
        }
        num = sscanf(attributes.find("offset")->second.c_str(), " %i", &offset );
        if( num == 0 ){
            offset = 0;
        }
        TestSource * source = new TestSource( frequency, offset );
        nodes.push_back( source );
        cout << "Build source node " << endl;
        initialized = 1;
        return source;
    }
    return NULL;
}

/// pushes events into the tracker tree.

void TestModule::pushState()
{
    for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
    {
        TestSource *source = (TestSource *) *it;
        if((cycle + source->offset) % source->frequency == 0 )
        {
            source->push();
        } 
    }
    cycle++;
}
