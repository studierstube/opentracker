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

#include "TestModule.h"
#include "TestSource.h"

#include "MathUtils.h"

#include <stdio.h>
#include <stdlib.h>
// #include <iostream> FIXME Why was this included ?

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
        attributes.get("position", source->event.getPosition(), 3);
        attributes.get("orientation", source->event.getOrientation(), 4);
        sscanf( attributes.get("button").c_str(), " %hu", &(source->event.getButton()) );
        sscanf( attributes.get("confidence").c_str(), " %f", &(source->event.getConfidence()) );
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

  void TestModule::pushEvent()
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
    static int count;
    if( noise > 0 )
      {
	perturbed.setAttribute("intAttr", count++);
	perturbed.setAttribute("chrAttr", (char)(33 + count % 92));
	perturbed.setAttribute("dblAttr", (double)(count / 7.3));
	perturbed.setAttribute("fltAttr", (float)(count / 7.3));

        int i;
        for( i = 0; i < 3; i++ )
	  {
            perturbed.getPosition()[i] = (float)(event.getPosition()[i] + ((float)rand()/RAND_MAX)*noise - noise / 2.0);
            perturbed.getOrientation()[i] = (float)(event.getOrientation()[i] + (float)(rand()/RAND_MAX)*noise - noise / 2.0);
	  }
        perturbed.getOrientation()[3] = (float)(event.getOrientation()[3] + ((float)rand()/RAND_MAX)*noise - noise / 2.0);
        MathUtils::normalizeQuaternion( perturbed.getOrientation() );
        if( ((float)rand()/RAND_MAX) < noise  )
	  {
            perturbed.getOrientation()[0] = -perturbed.getOrientation()[0];
            perturbed.getOrientation()[1] = -perturbed.getOrientation()[1];
            perturbed.getOrientation()[2] = -perturbed.getOrientation()[2];
            perturbed.getOrientation()[3] = -perturbed.getOrientation()[3];
	  }
	perturbed.setConfidence(static_cast<float>((rand()/RAND_MAX)*noise - noise / 2.0));
	perturbed.setButton(count % 16);

        perturbed.timeStamp();
        updateObservers( perturbed );
      }
    else
      {
        event.timeStamp();
        updateObservers( event );
      }
  }

} //namespace ot
