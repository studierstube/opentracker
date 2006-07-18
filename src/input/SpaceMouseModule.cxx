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
/** source file for SpaceMouseModule module.
  *
  * @author Michael Woegerbauer
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "SpaceMouseSource.h"
#include "SpaceMouseModule.h"

#ifdef USE_SPACEMOUSE

#include <stdio.h>
#if defined (WIN32) || defined (GCC3)
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include <Windows.h>
#include <math.h>

/* SpaceWare specific includes and library */
#include <spwmacro.h>  /* Common macros used by SpaceWare functions. */
#include <si.h>        /* Required for any SpaceWare support within an app.*/
#include <siapp.h>     /* Required for siapp.lib symbols */

#pragma comment(lib,"siapp")


#include "..\tool\OT_ACE_Log.h"
#include "..\core\MathUtils.h"

namespace ot {

	HWND		SpaceMouseModule::hWndSpaceMouse = NULL;
	SiHdl		devHdl;		/* Handle to Spaceball Device */
	SiOpenData	oData;		/* OS Independent data to open ball  */ 


	LRESULT FAR PASCAL WndSpaceMouseProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	};

	// Destructor method
	SpaceMouseModule::~SpaceMouseModule()
	{
		nodes.clear();
	}

	// This method is called to construct a new Node.
	Node * SpaceMouseModule::createNode( const std::string& name, StringTable& attributes)
	{
		if( name.compare("SpaceMouseSource") == 0 )
		{       
			SpaceMouseSource * source = new SpaceMouseSource;
			// add extra attributes
			source->event.addAttribute("float", "scalingFactor", "1");
			source->event.addAttribute("float", "cursorDistance", "1");

			source->tmpEvent.addAttribute("float", "scalingFactor", "1");
			source->tmpEvent.addAttribute("float", "cursorDistance", "1");

			/*if (attributes.get("type").compare("ext") == 0) {
				source->extNode = true;
			} else {
				source->extNode = false;
			}*/

			// read default values from ot config 
			if (!attributes.get("translationWeight").empty()) {
				float tw = (float)atof(attributes.get("translationWeight").c_str());
				source->tWeightDefault = tw;
				source->tWeight = tw;
				printf ("default translation weight: %.2f\n", tw);
			}
			if (!attributes.get("rotationWeight").empty()) {
				float rw = (float)atof(attributes.get("rotationWeight").c_str());
				source->rWeightDefault = rw;
				source->rWeight = rw;
				printf ("default rotation weight: %.2f\n", rw);
			}
			source->event.setConfidence( 1.0f );
			nodes.push_back( source );
			LOG_ACE_INFO("ot:Built SpaceMouseSource node\n");
			initialized = 1;
			return source;
		}
		if( 
			(name.compare("ExtSpaceMouseConfig") == 0) 
			|| (name.compare("SpaceMouseKitData") == 0)
			) {
				// create just a pass-through node
				NodePort *np = new NodePort();
				np->name = name;
				return np;
		}

		return NULL;
	}

	// opens SpaceMouse library
	void SpaceMouseModule::start()
	{
		if( isInitialized() == 1 && !nodes.empty())
		{
			ThreadModule::start();
		}
	}

	// closes SpaceMouse library
	void SpaceMouseModule::close()
	{
		// stop thread
		lock();
		stop = 1;
		unlock();

		if( (isInitialized() == 1) && (devHdl != NULL)) 
		{
			/* should maybe be called even if devHdl == NULL */
			SiTerminate();  /* called to shut down the SpaceWare input library */
		}
	}


	// This is the method executed in its own thread. It polls the joysticks.
	void SpaceMouseModule::run()
	{
		WNDCLASS  wndclass ;
		static int init = 0;

		if( init == 0 )
		{
			/*init the SpaceWare input library */
			if (SiInitialize() == SPW_DLL_LOAD_ERROR)
			{
				LOG_ACE_ERROR("ot:SpaceMouseModule Error Loading SIAPPDLL.DLL.\n");
				exit(1);
			}

			// Fill in window class structure with parameters that describe the message-only window.
			wndclass.style =         0;
			wndclass.lpfnWndProc =   WndSpaceMouseProc ;
			wndclass.cbClsExtra =    0;
			wndclass.cbWndExtra =    0; 
			wndclass.hInstance =     NULL;
			wndclass.hIcon =         NULL;
			wndclass.hCursor =       NULL;
			wndclass.hbrBackground = 0;
			wndclass.lpszMenuName =  NULL ;
			wndclass.lpszClassName = "dummyClassSpaceMouseModule";
	 
			/* Register display window class */
			if ( RegisterClass(&wndclass) == 0) 
			{
				LOG_ACE_ERROR("ot:SpaceMouseModule Could not register windowclass.\n");
				exit(1);
			}

			hWndSpaceMouse = CreateWindow(
				"dummyClassSpaceMouseModule",
				NULL,
				NULL,
				0,
				0,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				NULL,
				NULL,
				NULL,
				NULL
			);
			
			if (hWndSpaceMouse == NULL)
			{
				LOG_ACE_ERROR("ot:SpaceMouseModule Could not create message-only window.\n");
				exit(1);
			}

			SiOpenWinInit (&oData, hWndSpaceMouse);  /* init Win. platform specific data  */
			SiSetUiMode(devHdl, SI_UI_ALL_CONTROLS); /* Config SoftButton Win Display */

			/* open data, which will check for device type and return the device handle
			   to be used by this function */ 
			if ( (devHdl = SiOpen ("ot_spacemouse", SI_ANY_DEVICE, SI_NO_MASK,  
							 SI_EVENT, &oData)) == NULL)	
			{
				// could not open spacemouse device handle
				SiTerminate();  /* called to shut down the SpaceWare input library */
				LOG_ACE_INFO("ot:Could not fetch device handle for SpaceMouse.\n");
				/* ? do not exit? close dummy window? */
				initialized = 0; 
				exit(1);
			}

			initialized = 1;
			init = 1;
		}
		while(stop == 0)
		{
			processMessages();
		}
	}


	void SpaceMouseModule::pushEvent()
	{
		SpaceMouseSource *source;

		if( isInitialized() == 1 )
		{   
			for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
			{
				source = (SpaceMouseSource *) *it;

				lock();            
				if (source->changed == 1)
				{
					//if (source->extNode) {
					//	// push extended data
					//	source->event.getPosition()[0] = source->scale;
					//	source->event.getPosition()[1] = source->cursorDistance;

					//} else {
					//	source->event = source->tmpEvent;
					//}
					source->event = source->tmpEvent;

					source->changed = 0;
					unlock();        
					source->push();
				}
				else
					unlock();
			}
		}
	}


	// pushes events into the tracker tree.
	void SpaceMouseModule::processMessages()
	{
		MSG            msg;      /* incoming message to be evaluated */
		BOOL           handled;  /* is message handled yet */ 
		SiSpwEvent     SpaceEvent;    /* SpaceWare SpaceEvent */ 
		SiGetEventData EData;    /* SpaceWare Event Data */

		int				num;
		unsigned short	newBut;

		float			newPosX=0;
		float			newPosY=0;
		float			newPosZ=0;
		float			newRotX=0;
		float			newRotY=0;
		float			newRotZ=0;

		if( isInitialized() == 1 )
		{
			/* as long as there are messages waiting */
			WaitMessage();
			while ( PeekMessage( &msg, hWndSpaceMouse, 0, 0, PM_REMOVE ) ) 
			{
				//ACE_LOG_INFO("successful!\n");
				handled = SPW_FALSE;

				/* init Window platform specific data for a call to SiGetEvent */
				SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);
	      
				/* check whether msg was a Spaceball event and process it */
				if (SiGetEvent (devHdl, 0, &EData, &SpaceEvent) == SI_IS_EVENT)
				{
					if (SpaceEvent.type == SI_MOTION_EVENT)
					{
						float q[4];
						/* process Spaceball motion event */     
						newPosX = (-SpaceEvent.u.spwData.mData[SI_TX] / 330.0f);
						newPosY = (SpaceEvent.u.spwData.mData[SI_TY] / 330.0f);
						newPosZ = (SpaceEvent.u.spwData.mData[SI_TZ] / 330.0f);

						newRotX = ((-SpaceEvent.u.spwData.mData[SI_RX] / 330.0f)*SPW_PI/2.0f);
						newRotY = ((SpaceEvent.u.spwData.mData[SI_RY] / 330.0f)*SPW_PI/2.0f);
						newRotZ = ((SpaceEvent.u.spwData.mData[SI_RZ] / 330.0f)*SPW_PI/2.0f);

						for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
						{
							lock();
    						SpaceMouseSource * source = (SpaceMouseSource *)(*it);

							float tmp[3],corr[3],tq[4];
			
							tmp[0] = newPosX*source->tWeight;// - source->usrx;
							tmp[1] = newPosY*source->tWeight;// - source->usry;
							tmp[2] = newPosZ*source->tWeight;// - source->usrz;

							#define DIST(x, y) sqrt(((x[2]-y[2])*(x[2]-y[2])+(x[1]-y[1])*(x[1]-y[1])+(x[0]-y[0])*(x[0]-y[0])))

							
							// debug output
							/*#define prvec3(n) printf("%.2f %.2f %.2f", n[0], n[1], n[2]);
							printf ("\nCursor Pos:"); prvec3(source->tmpEvent.getPosition());
							printf ("\nCamera Pos:"); prvec3 (source->usrpos.getPosition());
							printf ("\n");*/
							

							// calculate cursor distance
							float cursorDistance = DIST(source->tmpEvent.getPosition(), source->usrpos.getPosition());
							source->tmpEvent.setAttribute<float>("cursorDistance", cursorDistance);
							
							// GoGo factor
							float gogofact = 1.0f;
							
							if (cursorDistance > source->nlDistance) {
								gogofact = 1.0f+(source->gogok*(1.0f - source->nlDistance/cursorDistance)*(1.0f - source->nlDistance/cursorDistance));
							}

							// Scaling factor
							if (cursorDistance > 25.0f) {
								//source->scale = 1.0f + (source->cursorDistance-25.0f)/25.0f;
								source->tmpEvent.setAttribute<float>("scalingFactor", (float)(1.0f+(cursorDistance-25.0f)/25.0f));
							} else {
								//source->scale = 1.0f;
								source->tmpEvent.setAttribute<float>("scalingFactor", (float)1.0);
							}

							//printf ("distance: %.2f", dist);
							float userOrientation [] = {
								source->usrpos.getOrientation()[0],
								source->usrpos.getOrientation()[1],
								source->usrpos.getOrientation()[2],
								source->usrpos.getOrientation()[3]};
							MathUtils::rotateVector(userOrientation, tmp, corr);
							
							source->tmpEvent.getPosition()[0] += corr[0]*gogofact;
							source->tmpEvent.getPosition()[1] += corr[1]*gogofact;
							source->tmpEvent.getPosition()[2] += corr[2]*gogofact;

							MathUtils::eulerToQuaternion (newRotX*source->rWeight, newRotY*source->rWeight, newRotZ*source->rWeight, q);

							tq[0]=source->tmpEvent.getOrientation()[0];
							tq[1]=source->tmpEvent.getOrientation()[1];
							tq[2]=source->tmpEvent.getOrientation()[2];
							tq[3]=source->tmpEvent.getOrientation()[3];

							float rq [4];
							if (!source->useAbsRotation) {		
								// Rotate in Object Space
								MathUtils::multiplyQuaternion(tq, q, rq);
							} else {
								// Use Absolute Rotation (Camera Space)
								// Transfer identity quaternion from Object into Camera Space, rotate, and transform back
								float irotC[4],irotO[4];
								MathUtils::invertQuaternion(userOrientation, irotC);
								MathUtils::invertQuaternion(tq, irotO);
								float init[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
								float st[4],q1[4],q2[4],q3[4],q4[4],q5[4];
								MathUtils::axisAngleToQuaternion(init, st);
								MathUtils::multiplyQuaternion(init, irotO, q1);
								MathUtils::multiplyQuaternion(q1, userOrientation, q2);
								MathUtils::multiplyQuaternion(q2,q,q3);
								MathUtils::multiplyQuaternion(q3, irotC, q4);
								MathUtils::multiplyQuaternion(q4, tq, q5);
								MathUtils::multiplyQuaternion(tq, q5, rq);
							}
							source->tmpEvent.setOrientation(rq);

							MathUtils::normalizeQuaternion(source->tmpEvent.getOrientation());

							source->changed = 1;
							unlock();
						}
					}
					if (SpaceEvent.type == SI_ZERO_EVENT)
					{
						// process SpaceMouse zero event 
						// Zero Event does nothing
					}
				
					if (SpaceEvent.type == SI_BUTTON_EVENT)
					{
						if ((num = SiButtonPressed (&SpaceEvent)) != SI_NO_BUTTON)	
						{
							/* process Spaceball button event */
							newBut = (unsigned short)pow(2.f, num - 1.f);
							for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
							{
								lock();
								SpaceMouseSource * source = (SpaceMouseSource *)(*it);
								source->tmpEvent.getButton() |= newBut;
								source->changed = 1;
								// Predefined Button mechanics
								switch(num) {
									case 5:
										// Center Mouse Cursor on Screen
										{
										// Position Mouse Cursor  units into the direction of the camera view
	/*
										float v1[3],v2[3];
										v1[0]=0.0f;v1[1]=0.0f;v1[2]=5.0f;
										MathUtils::rotateVector(source->usrpos.orientation, v1, v2);

										source->tmpEvent.position[0] = source->usrpos.position[0]+v2[0];
										source->tmpEvent.position[1] = source->usrpos.position[1]+v2[1];
										source->tmpEvent.position[2] = source->usrpos.position[2]+v2[2];
	*/
										}
									break;
									case 6: 
										{
											/*
											// Set orientation of the cursor to match that of the camera
											float np[4], cq[4];
											np[0]=0.0f; 
											np[1]=1.0f; 
											np[2]=0.0f; 
											np[3]=0.0f;
											MathUtils::axisAngleToQuaternion(np, cq);
											MathUtils::multiplyQuaternion(cq, source->usrpos.orientation, source->tmpEvent.orientation);
											*/
										}
										break;
									case 7:
										/*
										printf ("\nAbsolute rotation ");
										source->useAbsRotation = !source->useAbsRotation;
										printf (source->useAbsRotation ? "activated\n" : "deactivated\n");
										*/
										break;
								}

								unlock();
							}
						}
						if ((num = SiButtonReleased (&SpaceEvent)) != SI_NO_BUTTON)	
						{
							/* process Spaceball button release event */
							newBut = (unsigned short)pow(2.f, num - 1.f);
							for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
							{
								lock();
								SpaceMouseSource * source = (SpaceMouseSource *)(*it);
								source->tmpEvent.getButton() ^= newBut;
								source->changed = 1;
								unlock();
							}
						}
					}
			        
					handled = SPW_TRUE;              /* spacedevice event handled */ 
				}

				/* not a Spacedevice event, let windows handle it */
				if (handled == SPW_FALSE)
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}
		}
	}



} // namespace ot

#endif
