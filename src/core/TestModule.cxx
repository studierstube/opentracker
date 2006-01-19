  /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for TestModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"
#include <stdlib.h>

#include "TestModule.h"
#include "TestSource.h"

#include "MathUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <ace/Log_Msg.h>

//using namespace std;

// Destructor method

namespace ot {

TestModule::~TestModule()
{
    nodes.clear();
}

// This method is called to construct a new Node.

Node * TestModule::createNode( const std::string& name, StringTable& attributes)
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
        attributes.get("position", source->state.position, 3);
        attributes.get("orientation", source->state.orientation, 4);
        sscanf( attributes.get("button").c_str(), " %hu", &(source->state.button) );
        sscanf( attributes.get("confidence").c_str(), " %f", &(source->state.confidence) );
        if( attributes.containsKey("noise") )
        {
            attributes.get("noise",&source->noise);
        }
        else
        {
            source->noise = -1;
        }
        nodes.push_back( source );
                
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build TestSource node\n")));
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


void TestSource::push(void)
{
    if( noise > 0 )
    {
        perturbed.timeStamp();
        int i;
        for( i = 0; i < 3; i++ )
        {
            perturbed.position[i] = (float)(state.position[i] + ((float)rand()/RAND_MAX)*noise - noise / 2.0);
            perturbed.orientation[i] = (float)(state.orientation[i] + (float)(rand()/RAND_MAX)*noise - noise / 2.0);
        }
        perturbed.orientation[3] = (float)(state.orientation[3] + ((float)rand()/RAND_MAX)*noise - noise / 2.0);
        MathUtils::normalizeQuaternion( perturbed.orientation );
        if( ((float)rand()/RAND_MAX) < noise  )
        {
            perturbed.orientation[0] = -perturbed.orientation[0];
            perturbed.orientation[1] = -perturbed.orientation[1];
            perturbed.orientation[2] = -perturbed.orientation[2];
            perturbed.orientation[3] = -perturbed.orientation[3];
        }
        updateObservers( perturbed );
    }
    else
    {
        state.timeStamp();
        updateObservers( state );
    }
}

} //namespace ot
