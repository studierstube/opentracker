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
/** source file for CyberMouseModule module.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "CyberMouseSource.h"
#include "CyberMouseModule.h"

#ifdef USE_CYBERMOUSE

#include <stdio.h>
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

#include "freeddll.h"

#include <ace/Log_Msg.h>

//using namespace std;

#define SZ_Get_FREED_Info   "Get_FREED_Info"
#define SZ_OpenFREED        "OpenFREED"
#define SZ_CloseFREED       "CloseFREED"

namespace ot {

typedef ULONG (FAR PASCAL *PFNGet_FREED_Info)(FREED_info *Info);
extern PFNGet_FREED_Info lpfnGet_FREED_Info;

typedef ULONG (FAR PASCAL *PFNOpenFREED)(DWORD,HWND);
extern PFNOpenFREED lpfnOpenFREED;

typedef ULONG (FAR PASCAL *PFNCloseFREED)();
extern PFNCloseFREED lpfnCloseFREED;

PFNGet_FREED_Info lpfnGet_FREED_Info;
PFNOpenFREED lpfnOpenFREED;
PFNCloseFREED lpfnCloseFREED;
FREED_info FREED_Info;
WORD FREED_Opened;

HANDLE hLibrary;
HWND hWndCyber;


long FAR PASCAL WndProc (HWND hWndCyber, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWndCyber, uMsg, wParam, lParam);
}

// Destructor method
CyberMouseModule::~CyberMouseModule()
{
    nodes.clear();
}

// This method is called to construct a new Node.
Node * CyberMouseModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("CyberMouseSource") == 0 )
    {       
        CyberMouseSource * source = new CyberMouseSource;
		source->state.confidence = 1.0f;
        nodes.push_back( source );
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Build CyberMouseSource node\n")));
        initialized = 1;
        return source;
    }
    return NULL;
}

// opens CyberMouse library
void CyberMouseModule::start()
{
    if( isInitialized() == 1 )
	{
        WNDCLASS  wndclass ;

        // register FREED window class
        wndclass.style =         0 ;
        wndclass.lpfnWndProc =   WndProc ;
        wndclass.cbClsExtra =    0 ;
        wndclass.cbWndExtra =    0; 
        wndclass.hInstance =     NULL ;
        wndclass.hIcon =         NULL;
        wndclass.hCursor =       NULL;
        wndclass.hbrBackground = 0;
        wndclass.lpszMenuName =  NULL ;
        wndclass.lpszClassName = "dummyClassCyberMouseModule";

        RegisterClass(&wndclass) ;

        hWndCyber = CreateWindow(
		    "dummyClassCyberMouseModule",
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

        if (!hWndCyber)
        {
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Could not create message-only window.\nFatal Error\n")));
                exit(1);
        }

        if ((hLibrary = LoadLibrary("FREEDDLL.DLL")) == NULL) {
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error Loading FREEDDLL.DLL.\nFatal Error\n")));
                exit(1);
        }

        if ((lpfnGet_FREED_Info = (PFNGet_FREED_Info)GetProcAddress((HMODULE)hLibrary, SZ_Get_FREED_Info)) == NULL)
        exit(1);
        if ((lpfnOpenFREED = (PFNOpenFREED)GetProcAddress((HMODULE)hLibrary,SZ_OpenFREED)) == NULL)
        exit(1);
        if ((lpfnCloseFREED = (PFNCloseFREED)GetProcAddress((HMODULE)hLibrary,SZ_CloseFREED)) == NULL)
        exit(1);

        if (lpfnOpenFREED != NULL) {
            lpfnOpenFREED(0,hWndCyber);
            FREED_Opened = TRUE;
        }

        // get initial FREED information
        if (lpfnGet_FREED_Info != NULL) lpfnGet_FREED_Info(&FREED_Info);
    }
}

// closes CyberMouse library
void CyberMouseModule::close()
{
    if( isInitialized() == 1 && lpfnCloseFREED != NULL) {
        FREED_Opened = FALSE;
        lpfnCloseFREED();
    }
    if (hLibrary != NULL) FreeLibrary((HMODULE)hLibrary);
    PostQuitMessage(0);
}

// pushes events into the tracker tree.
void CyberMouseModule::pushState()
{
    if( isInitialized() == 1 )
    {
        lpfnGet_FREED_Info(&FREED_Info);
        if( ! (FREED_Info.Status & (NewPosition | NewButton_Status)))
            return;
    }
	for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
    {
        CyberMouseSource * source = (CyberMouseSource *)(*it);
        source->state.position[0] = FREED_Info.AbsX;
        source->state.position[1] = FREED_Info.AbsY;
        source->state.position[2] = FREED_Info.AbsZ;
        //middle button = 1, second button = 2, both = 3
        source->state.button = (FREED_Info.Status & 0x3);
        source->state.timeStamp();
        source->updateObservers( source->state );
    }
}

} // namespace ot

#endif
