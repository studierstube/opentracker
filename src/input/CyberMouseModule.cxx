 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for CyberMouseModule module.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/CyberMouseModule.cxx,v 1.1 2001/03/05 17:53:05 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "CyberMouseSource.h"
#include "CyberMouseModule.h"

#ifdef USE_CYBERMOUSE

#include <stdio.h>
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

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
Node * CyberMouseModule::createNode( string& name, StringMap& attributes)
{
    if( name.compare("CyberMouseSource") == 0 )
    {       
        CyberMouseSource * source = new CyberMouseSource;
		source->state.confidence = 1.0f;
        nodes.push_back( source );
        cout << "Build CyberMouseSource node " << endl;
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
                cout << "Could not create message-only window." << endl
                     << "Fatal Error" << endl;
                exit(1);
        }

        if ((hLibrary = LoadLibrary("FREEDDLL.DLL")) == NULL) {
                cout << "Error Loading FREEDDLL.DLL." << endl
                     << "FREED DLL" << endl;
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
        source->push();
    }
}

#endif