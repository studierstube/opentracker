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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for TestModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/TestModule.cxx,v 1.4 2001/07/16 21:43:52 reitmayr Exp $
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

using namespace std;

// Destructor method

TestModule::~TestModule()
{
    nodes.clear();
}

// This method is called to construct a new Node.

Node * TestModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("TestSource") == 0 )
    {
        int frequency;
        int offset;
        int num = sscanf(attributes.get("frequency").c_str(), " %i", &frequency );
        if( num == 0 ){
            frequency = 1;
        }
        num = sscanf(attributes.get("offset").c_str(), " %i", &offset );
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

// pushes events into the tracker tree.

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
