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
/** source file for SpaceDeviceModule module.
  *
  * @author Michael Woegerbauer, Mathis Csisinko
  *
  * $Id: SpaceDeviceModule.cxx 1271 2006-07-18 19:15:28Z sareika $
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/SpaceDeviceModule.h>
#ifdef USE_SPACEDEVICE
#ifdef WIN32

#include <OpenTracker/input/SpaceDeviceSource.h>

#ifndef SPACEDEVICE_DEPRECATED
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC (0x0001)
#endif
#define LOGITECH_3DX_VID 0x046d

static void* operator new(std::size_t size,std::size_t minimumSize)
{
	return operator new(size > minimumSize ? size: minimumSize);
}
#else
#include <ace/OS.h>

#include <cstdio>
#if defined (WIN32) || defined (GCC3)
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include <Windows.h>
#include <cmath>

/* SpaceWare specific includes and library */
#include <spwmacro.h>  /* Common macros used by SpaceWare functions. */
#include <si.h>        /* Required for any SpaceWare support within an app.*/
#include <siapp.h>     /* Required for siapp.lib symbols */

#pragma comment(lib,"siapp")


#include <opentracker\tool\OT_ACE_Log.h>
#include <opentracker\core\MathUtils.h>
#endif

namespace ot {

	
	OT_MODULE_REGISTER_FUNC(SpaceDeviceModule){
		OT_MODULE_REGISTRATION_DEFAULT(SpaceDeviceModule, "SpaceDeviceConfig");
	}
#ifndef SPACEDEVICE_DEPRECATED
    SpaceDeviceModule::SpaceDeviceModule(): ThreadModule(),NodeFactory(),pSources(),numDevices(0),deviceNames(0),rawInputDeviceList(0),threadId(0)
    {
	}
#else
	HWND		SpaceDeviceModule::hWndSpaceDevice = NULL;
	SiHdl		devHdl;		/* Handle to Spaceball Device */
	SiOpenData	oData;		/* OS Independent data to open ball  */ 

	SpaceDeviceModule::SpaceDeviceModule() : ThreadModule(), NodeFactory(), stop(0)
    {};
#endif


	// Destructor method
	SpaceDeviceModule::~SpaceDeviceModule()
	{
#ifndef SPACEDEVICE_DEPRECATED
#ifdef USE_LIVE
        for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
            delete *it;
#endif
        pSources.clear();
#else
		nodes.clear();
#endif
	}

	// This method is called to construct a new Node.
	Node * SpaceDeviceModule::createNode( const std::string& name, const StringTable& attributes)
	{
		if( name.compare("SpaceDeviceSource") == 0 )
		{
#ifndef SPACEDEVICE_DEPRECATED
			const std::string &deviceName = attributes.get("deviceName");
			if (! deviceName.empty())
				for (UINT device = 0;device < numDevices;device ++)
					if (rawInputDeviceList[device].dwType == RIM_TYPEHID && ! deviceName.compare(deviceNames[device]))
					{
						SpaceDeviceSource* pSource = new SpaceDeviceSource(rawInputDeviceList[device].hDevice);
						pSources.push_back(pSource);

						logPrintI("Build SpaceDeviceSource node\n");
						return pSource;
					}
					else;
			else
				for (UINT device = 0;device < numDevices;device ++)
					if (rawInputDeviceList[device].dwType == RIM_TYPEHID)
					{
						RID_DEVICE_INFO deviceInfo;
						UINT deviceInfoSize = sizeof(deviceInfo);
						deviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
						GetRawInputDeviceInfo(rawInputDeviceList[device].hDevice,RIDI_DEVICEINFO,&deviceInfo,&deviceInfoSize);
						if (deviceInfo.hid.dwVendorId == LOGITECH_3DX_VID && deviceInfo.hid.usUsagePage == HID_USAGE_PAGE_GENERIC && deviceInfo.hid.usUsage == 0x0008)
						{
							SpaceDeviceSource* pSource = new SpaceDeviceSource(rawInputDeviceList[device].hDevice);
							pSources.push_back(pSource);

							logPrintI("Build SpaceDeviceSource node\n");
							return pSource;
						}
					}
#else
			SpaceDeviceSource * source = new SpaceDeviceSource;
			source->event.setConfidence( 1.0f );
			nodes.push_back( source );
			logPrintS("Built SpaceDeviceSource node\n");
			initialized = 1;
			return source;
#endif
		}
		return NULL;
	}

#ifdef SPACEDEVICE_DEPRECATED
	// opens SpaceMouse library
	void SpaceDeviceModule::start()
	{
		if( isInitialized() == 1 && !nodes.empty())
		{
			ThreadModule::start();
		}
	}
#endif

	// closes SpaceMouse library
	void SpaceDeviceModule::close()
	{
#ifndef SPACEDEVICE_DEPRECATED
        PostThreadMessage(threadId,WM_QUIT,0,0x00000000);
        ThreadModule::close();
        if (deviceNames)
            {
                for (UINT device = 0;device < numDevices;device ++)
                    if (deviceNames[device])
                        delete[] deviceNames[device];
                delete[] deviceNames;
            }
        if (rawInputDeviceList)
            delete[] rawInputDeviceList;
#else
		// stop thread
		lockLoop();
		stop = 1;
		unlockLoop();

		if( (isInitialized() == 1) && (devHdl != NULL)) 
		{
			/* should maybe be called even if devHdl == NULL */
			SiTerminate();  /* called to shut down the SpaceWare input library */
		}
#endif
	}


	void SpaceDeviceModule::pushEvent()
	{
#ifndef SPACEDEVICE_DEPRECATED
        for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
            {
                SpaceDeviceSource* pSource = static_cast<SpaceDeviceSource*>((Node*)*it);
                lockLoop();
                if (pSource->calcEvent())
                    pSource->updateObservers(pSource->getEvent());
                unlockLoop();
            }
#else
		SpaceDeviceSource *source;

		if( isInitialized() == 1 )
		{   
			for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
			{
				source = (SpaceDeviceSource *) ((Node*)*it);

				lockLoop();            
				if (source->changed == 1)
				{
					source->event = source->tmpEvent;

					source->changed = 0;
					unlockLoop();        
					source->push();
				}
				else
					unlockLoop();
			}
		}
#endif
	}


#ifndef SPACEDEVICE_DEPRECATED
    void SpaceDeviceModule::init(StringTable &attributes,ConfigNode* pLocalTree)
    {
		Module::init(attributes,pLocalTree);
		GetRawInputDeviceList(NULL,&numDevices,sizeof(RAWINPUTDEVICELIST));
        rawInputDeviceList = new RAWINPUTDEVICELIST[numDevices];
        GetRawInputDeviceList(rawInputDeviceList,&numDevices,sizeof(RAWINPUTDEVICELIST));
        deviceNames = new char*[numDevices];
        logPrintI("Known raw input device names:\n");
        for (UINT device = 0;device < numDevices;device ++)
            {
                UINT deviceNameSize;
                GetRawInputDeviceInfo(rawInputDeviceList[device].hDevice,RIDI_DEVICENAME,NULL,&deviceNameSize);
                deviceNames[device] = new char[deviceNameSize + 1];
                GetRawInputDeviceInfo(rawInputDeviceList[device].hDevice,RIDI_DEVICENAME,deviceNames[device],&deviceNameSize);
                logPrintI("%s\n",deviceNames[device]);
            }
    }
#endif

	// This is the method executed in its own thread. It polls the spacedevices.
	void SpaceDeviceModule::run()
	{
#ifndef SPACEDEVICE_DEPRECATED
		threadId = GetCurrentThreadId();
        if (pSources.size() > 0)
            {
                WNDCLASSEX wndClassEx;
                wndClassEx.cbSize = sizeof(wndClassEx);
                wndClassEx.style = 0x00000000;
                wndClassEx.lpfnWndProc = WndProc;
                wndClassEx.cbClsExtra = 0;
                wndClassEx.cbWndExtra = 0;
                wndClassEx.hInstance = NULL;
                wndClassEx.hIcon = NULL;
                wndClassEx.hCursor = NULL;
                wndClassEx.hbrBackground = NULL;
                wndClassEx.lpszMenuName = NULL;
                wndClassEx.lpszClassName = "SpaceDeviceInputDummyWindow";
                wndClassEx.hIconSm = NULL;
                if (RegisterClassEx(&wndClassEx))
                    {
                        pSpaceDeviceModule = this;
                        HWND hWnd = CreateWindowEx(0x00000000,wndClassEx.lpszClassName,NULL,WS_OVERLAPPED,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,NULL,0);
                        if (hWnd)
                            {
                                RAWINPUTDEVICE rawInputDevices[1];
                                rawInputDevices[0].usUsage = 0x0008;
                                rawInputDevices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
                                rawInputDevices[0].hwndTarget = hWnd;
                                rawInputDevices[0].dwFlags = RIDEV_INPUTSINK;
                                if (RegisterRawInputDevices(rawInputDevices,sizeof(rawInputDevices) / sizeof(RAWINPUTDEVICE),sizeof(RAWINPUTDEVICE)))
                                    {
                                        MSG msg;
                                        BOOL result;
                                        for(;;)
                                            {
                                                result = GetMessage(&msg,NULL,0x00000000,0x00000000);
                                                if (result == 0 || result == -1)
                                                    break;
                                                else
                                                    DispatchMessage(&msg);
                                            }
                                    }
                                DestroyWindow(hWnd);
                            }
                        pSpaceDeviceModule = 0;
						UnregisterClass(wndClassEx.lpszClassName,NULL);
                    }
            }
#else
		WNDCLASS  wndclass ;
		static int init = 0;

		if( init == 0 )
		{
			/*init the SpaceWare input library */
			if (SiInitialize() == SPW_DLL_LOAD_ERROR)
			{
				logPrintE("SpaceDeviceModule Error Loading SIAPPDLL.DLL.\n");
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
			wndclass.lpszClassName = "dummyClassSpaceDeviceModule";
	 
			/* Register display window class */
			if ( RegisterClass(&wndclass) == 0) 
			{
				logPrintE("SpaceDeviceModule Could not register windowclass.\n");
				exit(1);
			}

			hWndSpaceDevice = CreateWindow(
				"dummyClassSpaceDeviceModule",
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
			
			if (hWndSpaceDevice == NULL)
			{
				logPrintE("SpaceDeviceModule Could not create message-only window.\n");
				exit(1);
			}

			SiOpenWinInit (&oData, hWndSpaceDevice);  /* init Win. platform specific data  */
			SiSetUiMode(devHdl, SI_UI_ALL_CONTROLS); /* Config SoftButton Win Display */

			/* open data, which will check for device type and return the device handle
			   to be used by this function */ 
			if ( (devHdl = SiOpen ("ot_spacedevice", SI_ANY_DEVICE, SI_NO_MASK,  
							 SI_EVENT, &oData)) == NULL)	
			{
				// could not open spacemouse device handle
				SiTerminate();  /* called to shut down the SpaceWare input library */
				logPrintE("Could not fetch device handle for SpaceMouse.\n");
				initialized = 0; 
			}

			initialized = 1;
			init = 1;
		}
		while(stop == 0)
		{
			processMessages();
		}
#endif
	}


#ifndef SPACEDEVICE_DEPRECATED
    LRESULT CALLBACK SpaceDeviceModule::WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
    {
        switch (message)
            {
		case WM_INPUT:
                    if (pSpaceDeviceModule)
                        pSpaceDeviceModule->processRawInput(hWnd,wParam,reinterpret_cast<HRAWINPUT>(lParam));
		default:
                    return DefWindowProc(hWnd,message,wParam,lParam);
            }
    }

	void SpaceDeviceModule::processRawInput(HWND,WPARAM,HRAWINPUT hRawInput)
    {
        RAWINPUTHEADER rawInputHeader;
        UINT rawInputSize = sizeof(rawInputHeader);
        if (GetRawInputData(hRawInput,RID_HEADER,&rawInputHeader,&rawInputSize,sizeof(RAWINPUTHEADER)) != -1)
		{
			for (NodeVector::iterator it = pSources.begin();it != pSources.end();it ++)
				{
					SpaceDeviceSource* pSource = static_cast<SpaceDeviceSource*>((Node*)*it);
					if (pSource->getDevice() == rawInputHeader.hDevice && rawInputHeader.dwType == RIM_TYPEHID && GetRawInputData(hRawInput,RID_INPUT,NULL,&rawInputSize,sizeof(RAWINPUTHEADER)) != -1)
						{
							LPRAWINPUT pRawInput = new(rawInputSize) RAWINPUT;
							if (GetRawInputData(hRawInput,RID_INPUT,pRawInput,&rawInputSize,sizeof(RAWINPUTHEADER)) != -1)
                                {
                                    lockLoop();
									switch(pRawInput->data.hid.bRawData[0])
									{
									case 1:
										pSource->setPosition(*reinterpret_cast<signed short*>(&pRawInput->data.hid.bRawData[1]) / 500.f,*reinterpret_cast<signed short*>(&pRawInput->data.hid.bRawData[3]) / 500.f,*reinterpret_cast<signed short*>(&pRawInput->data.hid.bRawData[5]) / 500.f);
										break;
									case 2:
										pSource->setOrientation(*reinterpret_cast<signed short*>(&pRawInput->data.hid.bRawData[1]) / 500.f * MathUtils::Pi / 2.f,*reinterpret_cast<signed short*>(&pRawInput->data.hid.bRawData[3]) / 500.f * MathUtils::Pi / 2.f,*reinterpret_cast<signed short*>(&pRawInput->data.hid.bRawData[5]) / 500.f * MathUtils::Pi / 2.f);
										break;
									case 3:
                                        pSource->setButtonEvent(*reinterpret_cast<unsigned short*>(&pRawInput->data.hid.bRawData[1]));
									}
									unlockLoop();
								}
							else
								if (context != NULL && context->doSynchronization())
									{
										context->dataSignal();
										context->consumedWait();
									}      
							delete pRawInput;
						}

				}
		}
    }

    SpaceDeviceModule* SpaceDeviceModule::pSpaceDeviceModule = 0;
#else
	// pushes events into the tracker tree.
	void SpaceDeviceModule::processMessages()
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
			while ( PeekMessage( &msg, hWndSpaceDevice, 0, 0, PM_REMOVE ) ) 
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
						newPosX = (SpaceEvent.u.spwData.mData[SI_TX] / 330.0f);
						newPosY = (SpaceEvent.u.spwData.mData[SI_TY] / 330.0f);
						newPosZ = (-SpaceEvent.u.spwData.mData[SI_TZ] / 330.0f);

						newRotX = ((SpaceEvent.u.spwData.mData[SI_RX] / 330.0f)*SPW_PI/2.0f);
						newRotY = ((SpaceEvent.u.spwData.mData[SI_RY] / 330.0f)*SPW_PI/2.0f);
						newRotZ = ((SpaceEvent.u.spwData.mData[SI_RZ] / 330.0f)*SPW_PI/2.0f);

						for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
						{
							lockLoop();
    						SpaceDeviceSource * source = (SpaceDeviceSource *)((Node*)*it);

							source->tmpEvent.getPosition()[0] = newPosX;
							source->tmpEvent.getPosition()[1] = newPosY;
							source->tmpEvent.getPosition()[2] = newPosZ;
							MathUtils::eulerToQuaternion (newRotX, newRotY, newRotZ, q);
							source->tmpEvent.setOrientation(q);

							/*source->tmpEvent.getOrientation()[0];
							source->tmpEvent.getOrientation()[1];
							source->tmpEvent.getOrientation()[2];
							source->tmpEvent.getOrientation()[3];*/

							source->changed = 1;
							unlockLoop();
						}
					}
					if (SpaceEvent.type == SI_ZERO_EVENT)
					{
						// process SpaceDevice zero event 
						// Zero Event does nothing
					}
				
					if (SpaceEvent.type == SI_BUTTON_EVENT)
					{
						if ((num = SiButtonPressed (&SpaceEvent)) != SI_NO_BUTTON)	
						{
							/* process SpaceDevice button event */
							newBut = (unsigned short)pow(2.f, num - 1.f);
							if(newBut>265) 
							{
								newBut =0;
								continue;
							}
							for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
							{
								lockLoop();
								SpaceDeviceSource * source = (SpaceDeviceSource *)((Node*)*it);
								source->tmpEvent.getButton() |= newBut;
								source->changed = 1;
								unlockLoop();
							}
						}
						if ((num = SiButtonReleased (&SpaceEvent)) != SI_NO_BUTTON)	
						{
							/* process SpaceDevice button release event */
							newBut = (unsigned short)pow(2.f, num - 1.f);
							for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
							{
								lockLoop();
								SpaceDeviceSource * source = (SpaceDeviceSource *)((Node*)*it);
								source->tmpEvent.getButton() ^= newBut;
								source->changed = 1;
								unlockLoop();
							}
						}
					}
			        
					handled = SPW_TRUE;              /* spacedevice event handled */                 
					if (context != NULL)
					  {
					    if (context->doSynchronization())
					    {
					      context->dataSignal();
					      context->consumedBroadcast();
					    }
					  }

				}

				/* not a Spacedevice event, let windows handle it */
				if (handled == SPW_FALSE)
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}
			ACE_OS::sleep(ACE_Time_Value(0, 500));
		}
	}

	LRESULT FAR PASCAL WndSpaceMouseProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
#endif


} // namespace ot

#endif
#endif
