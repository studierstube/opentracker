 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for WacomGraphireModule module.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header:
  * @file                                                                   */
 /* ======================================================================= */

#include "WacomGraphireSource.h"
#include "WacomGraphireModule.h"

#ifdef USE_WACOMGRAPHIRE

#include <stdio.h>
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include "wintab.h"
/* What Wintab packet data items we want.  PK_CURSOR identifies which cursor 
  (0-5) generated the packet.  PACKETDATA must be defined before including
  pktdef.h 
*/

#define	PACKETDATA	( PK_X | PK_Y | PK_Z | PK_CURSOR | PK_BUTTONS | PK_NORMAL_PRESSURE )
#include "pktdef.h"

HANDLE    hInst;   // Handle for instance
PACKET    pkt;    // Packet
HCTX    hTab;    // Handle for Tablet Context
POINT    ptNew;    // XY value storage
UINT    prsNew;    // Pressure value storage
UINT    curNew;    // Cursor number storage
UINT    butNew;    // Tilt value storage
HWND    hWndWacom;

// Destructor method
WacomGraphireModule::~WacomGraphireModule()
{
    nodes.clear();
}

// This method is called to construct a new Node.
Node * WacomGraphireModule::createNode( string& name, StringMap& attributes)
{
    if( name.compare("WacomGraphireSource") == 0 )
    {       
        int device;
        int num = sscanf(attributes.find("device")->second.c_str(), " %i", &device );
        if (num == 0) device = 1;
        WacomGraphireSource * source = new WacomGraphireSource(device);
        nodes.push_back( source );
        cout << "Build WacomGraphireSource node " << endl;
        initialized = 1;
        return source;
    }
    return NULL;
}

LRESULT FAR PASCAL RuleAppWndProc (HWND hWndWacom, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
        return DefWindowProc(hWndWacom, wMsg, wParam, lParam);
}


// opens WacomGraphire library
void WacomGraphireModule::start()
{
    LOGCONTEXT  lcMine; // The context of the tablet 
    AXIS    TabletX, TabletY;   // The maximum tablet size
    char    WName[50];  // String to hold window name 
    WNDCLASS    wc;
	
    if( isInitialized() == 1 )
    {
        // check if WinTab available.
        if (!WTInfo(0, 0, NULL)) 
        {
		    cout << "WinTab Services Not Available." << endl
                 << "Fatal Error" << endl;
            return;
	    }

	    // check if WACOM available.
        WTInfo(WTI_DEVICES, DVC_NAME, WName);
        if (strncmp(WName,"WACOM",5)) 
        {
		    cout << "Wacom Tablet Not Installed." << endl
                 << "Fatal Error" << endl;
            return;
        }
	
	    // Fill in window class structure with parameters that describe the message-only window.
        wc.style = 0;
        wc.lpfnWndProc = RuleAppWndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = NULL;
        wc.hIcon = NULL;
        wc.hCursor = NULL;
        wc.hbrBackground = 0;
        wc.lpszMenuName =  NULL;
        wc.lpszClassName = "dummyClassWacomGraphireModule";

        //Register the window class and return success failure code.
        RegisterClass(&wc);

        hWndWacom = CreateWindow(
            "dummyClassWacomGraphireModule",
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

        // If window could not be created, return "failure" 
        if (!hWndWacom) 
        {
                cout << "Could not create message-only window." << endl
                     << "Fatal Error" << endl;
                exit(1);
        }

        // get default region */
        WTInfo(WTI_DEFCONTEXT, 0, &lcMine);

        // modify the digitizing region 
        lcMine.lcOptions |= CXO_MESSAGES;
        lcMine.lcPktData = PACKETDATA;
        lcMine.lcPktMode = 0;
        lcMine.lcMoveMask = PACKETDATA;
        lcMine.lcBtnUpMask = lcMine.lcBtnDnMask;

	    // Set the entire tablet as active
        WTInfo(WTI_DEVICES,DVC_X,&TabletX);
        WTInfo(WTI_DEVICES,DVC_Y,&TabletY);
        lcMine.lcInOrgX = 0;
        lcMine.lcInOrgY = 0;
        lcMine.lcInExtX = TabletX.axMax;
        lcMine.lcInExtY = TabletY.axMax;

        // output the data in screen coords
        lcMine.lcOutOrgX = lcMine.lcOutOrgY = 0;
        lcMine.lcOutExtX = 127.6*40;//GetSystemMetrics(SM_CXSCREEN); //127.6
        // move origin to upper left 
        lcMine.lcOutExtY = -92.8*40;//-GetSystemMetrics(SM_CYSCREEN); //92.8

        // open the region 
        if ((hTab = WTOpen(hWndWacom, &lcMine, TRUE)) == NULL)
        {
            cout << "Failed to open WacomGraphire library !" << endl;
            initialized = 0;
        }
    }
}

// closes WacomGraphire library
void WacomGraphireModule::close()
{
    if( isInitialized() == 1 && hTab)
    {
        WTClose(hTab);
    }
}

// pushes events into the tracker tree.
void WacomGraphireModule::pushState()
{
    if( isInitialized() == 1 )
    {
        unsigned int newest, oldest;
        if (!WTQueuePacketsEx(hTab, &oldest, &newest)) return;
        if (WTPacket(hTab, newest, &pkt))
        {
            // save new co-ordinates
            ptNew.x = (UINT)pkt.pkX;
            ptNew.y = (UINT)pkt.pkY;
            curNew = pkt.pkCursor; // penTip = 1, eraser = 2, puck = 0
            butNew = (UINT)pkt.pkButtons; // pen & eraser = 1, hi = 4, low= 2; 
            prsNew = pkt.pkNormalPressure;	// pressure					
        }
        for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
        {	
            WacomGraphireSource * source = (WacomGraphireSource *)(*it);
            if (curNew==source->device)
            {
                source->state.position[0] = ptNew.x/40.0/1000;
                source->state.position[1] = ptNew.y/40.0/1000;
                source->state.position[2] = prsNew; 
                source->state.button = butNew;
                source->state.timeStamp();
                source->push();
            }
        }
    }
}

#endif