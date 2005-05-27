/**************************************************************************\
*
*  Copyright (C) 2000 by Vienna University of Technology
*
*  This file is part of the software toolkit distributed for the
*  Virtual Reality exercises.
*
\**************************************************************************/

//for windows dialogs
#include <windows.h>
#include <commctrl.h> 
#include "resource.h"
//--


#include <qobject.h>
#include <qevent.h>
#include <qapplication.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SoDB.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtRenderArea.h>


#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>  

#include "converter.h"


/** Windows specific **/

TCHAR szWindowClass[] = "map_viewer";
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );


// *************************************************************************
HWND mainWnd, dlgWnd;
extern void loadSceneGraph(const char * filename, SoSeparator *, const SbViewportRegion &  );
// *************************************************************************


void mainerror(const char * message)
{
	MessageBox(0,message,"Error",MB_OK);
}



ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//(LPCSTR)IDC_;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);

	return RegisterClassEx(&wcex);
}




/* Windows Resource Dialog */
bool showDialog( HINSTANCE hInstance)
{
   //HWND hWnd;

/*   INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
*/

   MyRegisterClass(hInstance);

   mainWnd = CreateWindow(szWindowClass, "Map Viewer - Options", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 480, 400, NULL, NULL, hInstance, NULL);

   if (!mainWnd) return false;


   ShowWindow( mainWnd, SW_SHOW );

 	// show Dialog/Options window

   dlgWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), mainWnd, (DLGPROC)WndProc);

   ShowWindow(dlgWnd, SW_SHOW);

   //set default options
   SendDlgItemMessage(dlgWnd,IDC_INPUTFILE,WM_SETTEXT,0,(LPARAM) INPUT_FILE);

//	SendDlgItemMessage(cWnd,IDC_USE_COLORS, BM_SETCHECK, (Param.use_range_colors ? BST_CHECKED : BST_UNCHECKED), 0);

   SendDlgItemMessage(dlgWnd,IDC_CONVERT, BM_SETCHECK, BST_CHECKED , 0);
   SendDlgItemMessage(dlgWnd,IDC_SHOWOUTPUT, BM_SETCHECK, BST_CHECKED , 0);
   SendDlgItemMessage(dlgWnd,IDC_MARKER, BM_SETCHECK, BST_CHECKED , 0);
   SendDlgItemMessage(dlgWnd,IDC_MARKERIDS, BM_SETCHECK, BST_CHECKED , 0);
//   SendDlgItemMessage(dlgWnd,IDC_MARKERNAMES, BM_SETCHECK, BST_CHECKED , 0);
	SendDlgItemMessage(dlgWnd,IDC_ZEROPOINTS, BM_SETCHECK, BST_CHECKED , 0);

   UpdateWindow( mainWnd );

	return true;
}




int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{

	
	if (!showDialog(hInstance)) return -1;

	MSG msg;
	//HACCEL hAccelTable;

		// Hauptnachrichtenschleife:
	while( GetMessage(&msg, NULL, 0, 0) ) 
	{
	//	if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return msg.wParam;
}


void getOptions(options * Opt)
{
	Opt->convert = (SendDlgItemMessage(dlgWnd,IDC_CONVERT, BM_GETCHECK, 0, 0)
					== BST_CHECKED);
	if (Opt->convert)
	{
		char * temp = new char[256];
		GetWindowText(GetDlgItem(dlgWnd,IDC_INPUTFILE),temp,255);
		Opt->inputfile = temp;
	}
	else
		Opt->inputfile = NULL;

	Opt->markers = (SendDlgItemMessage(dlgWnd,IDC_MARKER, BM_GETCHECK, 0, 0)
					== BST_CHECKED);

	Opt->marker_ids = (SendDlgItemMessage(dlgWnd,IDC_MARKERIDS, BM_GETCHECK, 0, 0)
					== BST_CHECKED);

	Opt->marker_names = (SendDlgItemMessage(dlgWnd,IDC_MARKERNAMES, BM_GETCHECK, 0, 0)
					== BST_CHECKED);

	Opt->zeropoints = (SendDlgItemMessage(dlgWnd,IDC_ZEROPOINTS, BM_GETCHECK, 0, 0)
					== BST_CHECKED);

	Opt->outputfile = NULL; //unused
}



class QuitFilter : public QObject
{
public:
    QuitFilter::QuitFilter( QObject *parent );
protected:
    bool  eventFilter( QObject *, QEvent * );
};

QuitFilter::QuitFilter( QObject *parent )
: QObject()
{
    if ( parent )                           // has a parent widget
        parent->installEventFilter( this ); // then install filter
}

bool QuitFilter::eventFilter( QObject *o, QEvent *e )
{
    if ( (e->type() == QEvent::KeyPress )    // some kind of user input
     //     || ( e->type() == QEvent::MouseButtonPress )
        //* ||  (e->type() == QEvent::MouseMove ) 
        ) 
    {   
		QKeyEvent * ke = (QKeyEvent *) e;
		if ((ke->ascii() == 'q') || (ke->ascii() == 'x')
								 || (ke->ascii() == 27)  )
			SoQt::exitMainLoop();                 // quit
        return TRUE;                        
    }
    return QObject::eventFilter( o, e );    // standard event processing
}




int Startup()
{
	options * Options = new options;
	getOptions(Options);
	Options->outputfile = TEMP_FILE;
/*
	char tmp[200];
	sprintf(tmp,"cv: %i m: %i mi: %i mn: %i\r\n%s",
		Options->convert,Options->markers,	Options->marker_ids,Options->marker_names,Options->inputfile);
	MessageBox(0,tmp,"getOptions",MB_OK);
*/

	//kill main dlg window
	//DestroyWindow(mainWnd);
	ShowWindow(mainWnd,SW_HIDE);

	//convert iv file!
	if (Options->convert)
		if (!convert_file(Options))
			return -1;
	
	
	// if not CHECK 2 exit

    if (SendDlgItemMessage(dlgWnd,IDC_SHOWOUTPUT, BM_GETCHECK, 0, 0)
					!= BST_CHECKED) return 0;


	// setup Qt and SoQT
    // This initialises inventor and qt
    QWidget * window = SoQt::init( "Fullscreen" );
    if ( ! window )
        exit( 1 );
    //window->setMinimumSize(700,300);

  //  SoQt::setDefaultVisual(QGL::DoubleBuffer|QGL::DepthBuffer|QGL::Rgba|QGL::StencilBuffer|
	//	                   QGL::DirectRendering|QGL::AlphaChannel|QGL::AccumBuffer);

    // doesn't work right now :(
    QuitFilter quitter( SoQt::getApplication());
    // the common root node
    SoSeparator * root = new SoSeparator;
    root->ref();    
         
    // Create a renderArea in which to see our scene graph.
    // The render area will appear within the main window.
    SoQtRenderArea *myRenderArea = new SoQtRenderArea( window );

	// set up the scenegraph
	loadSceneGraph(Options->outputfile, root, myRenderArea->getViewportRegion() ); 

	// Put our scene in myRenderArea, change the title
    myRenderArea->setSceneGraph(root);
    myRenderArea->setTitle("Fullscreen");
    myRenderArea->show();
    


	// ALTERNATIVE: Examiner Viewer
	SoQtExaminerViewer *myViewer = new SoQtExaminerViewer(window);
	//SoQtPlaneViewer *myViewer = new SoQtPlaneViewer(window);

	myViewer->setSceneGraph(root);
	myViewer->setTitle("Map Viewer");
	myViewer->setHeadlight(FALSE);
	myViewer->show();



    // show and set to full screen
    SoQt::show( window );
    // comment this out for debugging
    //window->showFullScreen();
    window->showMaximized();

    // Start mainLoop
    SoQt::mainLoop();
    root->unref();

    //delete viewer;
    delete myRenderArea;
	delete Options;
    return 0;
} // main()



bool FileDialog()
{
	char filename[512];
	OPENFILENAME ofn;

	memset(filename,0,512);
	memset(&ofn,0,sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = dlgWnd;
	ofn.lpstrTitle = "Select input file (to be converted)";
	ofn.lpstrFilter = "Inventor files (*.iv)\0*.iv\0Any file (*.*)\0*.*\0\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 511;
    ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
	{
		return false;
	}

	//set file name
	SendDlgItemMessage(dlgWnd,IDC_INPUTFILE,WM_SETTEXT,0,(LPARAM) filename);
	return true;
}


// *************************************************************************


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
//	int idCtrl;

	wmId    = LOWORD(wParam); 
	wmEvent = HIWORD(wParam); 


	switch( message ) 
	{
        case WM_NOTIFY:            

			MessageBox(0,"notify","test",MB_OK);
			break;
		case WM_COMMAND:
			//MessageBox(0,"command","test",MB_OK);
			switch( wmId ) 
			{
			case IDC_BROWSE:
				FileDialog();
				break;
			case IDOK:
				{
				//start ExaminerViewer
				int ret = Startup();
				//returns from ExaminerViewer on exit
				PostQuitMessage( ret );
				break;
				}
			case IDCANCEL:
				PostQuitMessage( 0 );	//EXIT
				break;
			}
			
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;		
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
   }
   return 0;

}