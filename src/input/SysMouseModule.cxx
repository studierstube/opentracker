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
 /** header file for SysMouseSink Node.
  *
  * @author Markus Sareika
  * 
  * $Id: SysMouseModule.h
  * @file                                                                   */
 /* ======================================================================= */


#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/SysMouseSink.h>
#include <OpenTracker/input/SysMouseModule.h>


#ifdef USE_SYSMOUSE


#include <opentracker\tool\OT_ACE_Log.h>

namespace ot {
	OT_MODULE_REGISTER_FUNC(SysMouseModule){
		OT_MODULE_REGISTRATION_DEFAULT(SysMouseModule, "SysMouseConfig");
	}

	SysMouseModule::SysMouseModule() : ThreadModule(), 
									NodeFactory(), 
									stop(0),
									buttonInput(0), 
									lastButtonInput(0),
									mouseX(0), mouseY(0)
	{
#ifdef WIN32 
		inputPtr = new ::INPUT;
		mouseInputPtr = new ::MOUSEINPUT;
#endif		
	};

	// Destructor method
	SysMouseModule::~SysMouseModule()
	{
		nodes.clear();
#ifdef WIN32 
		delete inputPtr;
#endif
	}

	// This method is called to construct a new Node.
	Node * SysMouseModule::createNode( const std::string& name, StringTable& attributes)
	{
		if( name.compare("SysMouseSink") == 0 )
		{       
			SysMouseSink * sink = new SysMouseSink( this );
			//sink->event.setConfidence( 1.0f );
			nodes.push_back( sink );
			logPrintS("Built SysMouseSink node\n");
			initialized = 1;
			return sink;
		}
		if( (name.compare("AbsoluteInput") == 0) || (name.compare("RelativeInput") == 0) ) 
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
	void SysMouseModule::start()
	{
		if( isInitialized() == 1 && !nodes.empty())
		{
			ThreadModule::start();
		}
	}

	// stops the thread
	void SysMouseModule::close()
	{
		// stop thread
		lock();
		stop = 1;
		unlock();

		if( isInitialized() == 1 && !nodes.empty()) 
		{
			SysMouseSink * sink;
			for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
			{
				sink = (SysMouseSink *) *it;
				//source->deleteSth;
			}
		}
	}


	void SysMouseModule::run()
	{
		static int init = 0;

		if( init == 0 )
		{
			initialized = 1;
			init = 1;
		}
		while(stop == 0)
		{
			processLoop();
		}
	}


	void SysMouseModule::processLoop()
	{
		if( isInitialized() == 1 )
		{
			SysMouseSink *sink;
			for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
			{
				sink = (SysMouseSink *) *it;
				
				if( sink->changedRelative )
				{
					buttonInput = sink->relativeEvent.getButton();
					mouseX = (int)(sink->relativeEvent.getPosition()[0]*10);
					mouseY = (int)(sink->relativeEvent.getPosition()[1]*10);
					#ifdef WIN32
					mouseFlags = MOUSEEVENTF_MOVE;
					#endif
					sink->changedRelative = 0;
				}
				if( sink->changedAbsolute )
				{
					buttonInput = sink->absoluteEvent.getButton();
					mouseX = (int)(sink->absoluteEvent.getPosition()[0]*65535);
					mouseY = (int)(sink->absoluteEvent.getPosition()[1]*65535);
					#ifdef WIN32
					mouseFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
					#endif
					sink->changedAbsolute = 0;
					// comment out for overriding the systems mouse
					sink->changedRelative = 1; 
				}				
#ifdef WIN32
				mouseInputPtr->dx = mouseX;
				mouseInputPtr->dy = mouseY;
				mouseInputPtr->mouseData = 0;
				mouseInputPtr->dwFlags = mouseFlags;
				if( lastButtonInput != buttonInput )
				{
					if( buttonInput == 1 )
					{
						mouseInputPtr->dwFlags = mouseInputPtr->dwFlags | MOUSEEVENTF_LEFTDOWN;
						buttonPressed = 1;
					}
					if( buttonInput == 2 )
					{
						mouseInputPtr->dwFlags = mouseInputPtr->dwFlags | MOUSEEVENTF_RIGHTDOWN;
						buttonPressed = 2;
					}
					if( buttonInput == 0 )
					{
						if( buttonPressed == 1 ) mouseInputPtr->dwFlags |=  MOUSEEVENTF_LEFTUP;
						if( buttonPressed == 2 ) mouseInputPtr->dwFlags |=  MOUSEEVENTF_RIGHTUP;
						buttonPressed = 0;
					}
				}
				lastButtonInput = buttonInput;
				mouseInputPtr->time = 0;
				mouseInputPtr->dwExtraInfo = 0;

				inputPtr->type = INPUT_MOUSE;
				inputPtr->mi = *mouseInputPtr;
				int send = ::SendInput( 1, inputPtr, sizeof(*inputPtr) );
#endif
			}
		}
		// wait 10msec for now - this sets the update intervall to the system
		ACE_OS::sleep( ACE_Time_Value(0, 10000) );
	}
} // namespace ot

#endif //USE_SYSMOUSE
