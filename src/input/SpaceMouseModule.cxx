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
/** source file for SpaceMouseModule module.
  *
  * @author Michael Woegerbauer
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpaceMouseModule.cxx,v 1.3 2003/04/03 15:50:59 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

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

/* SpaceWare Specific Includes */
#include "spwmacro.h"  /* Common macros used by SpaceWare functions. */
#include "si.h"        /* Required for any SpaceWare support within an app.*/
#include "siapp.h"     /* Required for siapp.lib symbols */

#include "..\core\MathUtils.h"

using namespace std;

HWND		SpaceMouseModule::hWndSpaceMouse = NULL;
SiHdl		devHdl;			/* Handle to Spaceball Device */
SiOpenData	oData;			/* OS Independent data to open ball  */ 


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
		source->state.confidence = 1.0f;
        nodes.push_back( source );
        cout << "Built SpaceMouseSource node " << endl;
        initialized = 1;
        return source;
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
                cout << "Error Loading SIAPPDLL.DLL." << endl
                     << "SIAPPDLL DLL" << endl;
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
            cout << "Could not register windowclass." << endl
                    << "Fatal Error" << endl;
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
            cout << "Could not create message-only window." << endl
                 << "Fatal Error: " << GetLastError() << endl;
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
            cout << "Could not fetch device handle for SpaceMouse." << endl;
			/* ? do not exit? close dummy window? */
			initialized = 0; 
			exit(1);
		}

		initialized = 1;
		init = 1;

/*		if (SiBeep(devHdl, "ffffffff") == SI_BAD_HANDLE) 
		{
			cout << "could not beep..." << endl;
		}

 */
    }


	while(stop == 0)
    {
        processMessages();
    }
}


void SpaceMouseModule::pushState()
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
                source->state = source->tmpState;
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
	SiSpwEvent     Event;    /* SpaceWare Event */ 
	SiGetEventData EData;    /* SpaceWare Event Data */

	int				num;
	unsigned short	newBut;
	float			newPosX;
	float			newPosY;
	float			newPosZ;
	float			newRotX;
	float			newRotY;
	float			newRotZ;

    if( isInitialized() == 1 )
    {
		/* as long as there are messages waiting */
		WaitMessage();
		while ( PeekMessage( &msg, hWndSpaceMouse, 0, 0, PM_REMOVE ) ) 
		{
			//cout << "successful!" << endl;
			handled = SPW_FALSE;

			/* init Window platform specific data for a call to SiGetEvent */
			SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);
      
			/* check whether msg was a Spaceball event and process it */
			if (SiGetEvent (devHdl, 0, &EData, &Event) == SI_IS_EVENT)
			{
				if (Event.type == SI_MOTION_EVENT)
				{
					
					/* process Spaceball motion event */     
					newPosX = Event.u.spwData.mData[SI_TX] / 330.0;
					newPosY = Event.u.spwData.mData[SI_TY] / 330.0;
					newPosZ = -Event.u.spwData.mData[SI_TZ] / 330.0;
					newRotX = (Event.u.spwData.mData[SI_RX] / 330.0)*SPW_PI/2.0;
					newRotY = (Event.u.spwData.mData[SI_RY] / 330.0)*SPW_PI/2.0;
					newRotZ = -(Event.u.spwData.mData[SI_RZ] / 330.0)*SPW_PI/2.0;
					
					for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
					{
	                    lock();
    					SpaceMouseSource * source = (SpaceMouseSource *)(*it);
						source->tmpState.position[0] = newPosX;
						source->tmpState.position[1] = newPosY;
						source->tmpState.position[2] = newPosZ;
						MathUtils::eulerToQuaternion(newRotX, newRotY, newRotZ, source->tmpState.orientation);
						MathUtils::normalizeQuaternion(source->tmpState.orientation);
						source->changed = 1;
						unlock();
					}
				}
				if (Event.type == SI_ZERO_EVENT)
				{
					// process Spaceball zero event 
					for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
					{
						lock();
						SpaceMouseSource * source = (SpaceMouseSource *)(*it);
						source->tmpState.position[0] = 0.0;
						source->tmpState.position[1] = 0.0;
						source->tmpState.position[2] = 0.0;
						source->tmpState.orientation[0]  = 0.0;
						source->tmpState.orientation[1]  = 0.0;
						source->tmpState.orientation[2]  = 0.0;
						source->tmpState.orientation[3]  = 1.0;
						source->changed = 1;
						unlock();
					}
				}
			
				if (Event.type == SI_BUTTON_EVENT)
				{
					if ((num = SiButtonPressed (&Event)) != SI_NO_BUTTON)	
					{
						/* process Spaceball button event */
						newBut = pow(2, num - 1);
						for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
						{
							lock();
							SpaceMouseSource * source = (SpaceMouseSource *)(*it);
							source->tmpState.button |= newBut;
							source->changed = 1;
							unlock();
						}
					}
					if ((num = SiButtonReleased (&Event)) != SI_NO_BUTTON)	
					{
						/* process Spaceball button release event */
						newBut = pow(2, num - 1);
						for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
						{
							lock();
							SpaceMouseSource * source = (SpaceMouseSource *)(*it);
							source->tmpState.button ^= newBut;
							source->changed = 1;
							unlock();
						}
					}
				}
		        
				handled = SPW_TRUE;              /* spaceball event handled */ 
			}

			/* not a Spaceball event, let windows handle it */
			if (handled == SPW_FALSE)
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
    }
}

#endif
