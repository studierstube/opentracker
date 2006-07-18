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
 /** header file for PanTiltUnitSinkSource Node.
  *
  * @author Markus Sareika
  * 
  * $Id: PanTiltUnitModule.h
  * @file                                                                   */
 /* ======================================================================= */


#include "../tool/disable4786.h"

#include "PanTiltUnitSinkSource.h"
#include "PanTiltUnitModule.h"

#ifdef USE_PANTILTUNIT

#include <stdio.h>
#if defined (WIN32) || defined (GCC3)
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include <Windows.h>
//#include <math.h>

#include "..\tool\OT_ACE_Log.h"


namespace ot {

	// Destructor method
	PanTiltUnitModule::~PanTiltUnitModule()
	{
		nodes.clear();
	}

	// This method is called to construct a new Node.
	Node * PanTiltUnitModule::createNode( const std::string& name, StringTable& attributes)
	{
		if( name.compare("PanTiltUnitSinkSource") == 0 )
		{       
			PanTiltUnitSinkSource * source = new PanTiltUnitSinkSource;
			source->event.addAttribute("float", "focalDistance", "0");
			// read values from xml config file and initialize PTU
			if ( !attributes.get("comPort").empty() ) {
				int comPort = (int)atof(attributes.get("comPort").c_str());
				if (!source->initComPort(comPort)) 
					std::cerr << "PTU Serial Port: "<<comPort<<" setup error.\n" << std::endl;
				else std::cerr << "PTU Serial Port: "<<comPort<<" initialized" << std::endl;
			}
			if ( !attributes.get("frequency").empty() ) 
				source->frequency = (int)atof(attributes.get("frequency").c_str());
			
			source->event.setConfidence( 1.0f );
			nodes.push_back( source );
			LOG_ACE_INFO("ot:Built PanTiltUnitSinkSource node\n");
			initialized = 1;
			return source;
		}
		if( (name.compare("PtuLocation") == 0) ||(name.compare("PtuMoveTo") == 0) || 
			(name.compare("RelativeInput") == 0) || (name.compare("TopOffset") == 0)) 
		{
			// create just a pass-through node
			NodePort *np = new NodePort();
			np->name = name;
			return np;
		}

		// no node configured 
		return NULL;
	}

	// starts the ptu thread
	void PanTiltUnitModule::start()
	{
		if( isInitialized() == 1 && !nodes.empty())
		{
			ThreadModule::start();
		}
	}

	// stops the ptu thread and closes all COM port streams
	void PanTiltUnitModule::close()
	{
		// stop thread
		lock();
		stop = true;
		unlock();

		if( isInitialized() == 1 && !nodes.empty()) 
		{
			PanTiltUnitSinkSource * source;
			for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
			{
				source = (PanTiltUnitSinkSource *) *it;
				source->closeComPort();
			}
		}
	}


	// This is the method executed in its own thread the "ptu thread". 
	void PanTiltUnitModule::run()
	{
		//static bool init = false;
		//if( !init ) // initialize ptu thread
		//{
		//	initialized = 1;
		//	init = true;
		//}
		while( !stop )
		{
			processMovements();
		}
	}

	// 
	void PanTiltUnitModule::pushEvent()
	{
		PanTiltUnitSinkSource *source;

		if( isInitialized() == 1 )
		{   
			for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
			{
				source = (PanTiltUnitSinkSource *) *it;     
				if (source->publishEvent)
				{
					//if((cycle + source->offset) % source->frequency == 0 )
					//{
					//	source->push();
					//}
					//lock();  
					//source->event = source->newEvent;

					source->publishEvent = false;
					//unlock();        
					source->push();
				}
			}
		}
	}

	// pushes events into the tracker tree.
	void PanTiltUnitModule::processMovements()
	{
		// do something with best effort in each ptu 
		for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
		{
			PanTiltUnitSinkSource * source = (PanTiltUnitSinkSource *) *it;     
			//... do some time dependent stuff ...
			
		}
	}
} // namespace ot

#endif //USE_PANTILTUNIT
