/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
 /** 
  * Implements the class SoPrintKit, a subclass of SoDropTarget to print geometry.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoPrintKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <memory>
#include <string>

#include <windows.h>

#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/SbColor.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/SbLinear.h>

//#include <Inventor/SoOffscreenRenderer.h>
#include <stbapi/misc/OnScreenRenderer.h>

#include "SoPrintKit.h"

using namespace std;

// OpenInventor macros

SO_KIT_SOURCE(SoPrintKit);
 
// class initialization for OpenInventor

void SoPrintKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoPrintKit, SoDropTarget, "SoDropTarget");
}

// constructor

SoPrintKit::SoPrintKit()
{
    SO_KIT_CONSTRUCTOR(SoPrintKit);

    SO_KIT_INIT_INSTANCE();
}

bool SoPrintKit::handleItem(SoFullPath* pathToItem)
{
    SoDropItem* item = (SoDropItem*)pathToItem->getTail();
    string cmd = item->commands.getValue().getString();

    // are we responsible for this item
    if ((cmd == "*") || (cmd.find("PRINT") > -1))
    {
		SbViewportRegion printView;

		SoSeparator* printRoot = new SoSeparator;
		printRoot->ref();

		SoPerspectiveCamera* printCam = new SoPerspectiveCamera;
		printRoot->addChild(printCam);

		printRoot->addChild(new SoDirectionalLight);

		SoTransform* itemTrans = new SoTransform;
		printRoot->addChild(itemTrans);

        SoGetMatrixAction ma(printView);
        ma.apply(pathToItem);
        SbMatrix transMat = ma.getMatrix();

        SbVec3f t,s; SbRotation rot, so;
        transMat.getTransform( t, rot, s, so );

		itemTrans->rotation.setValue(rot);

		printRoot->addChild(item);

		printCam->viewAll(printRoot, printView);

		// Render the scene 
		SoTexture2* tex = new SoTexture2;
		tex->ref();

		onScreenRenderScene(printRoot, printView, tex, 800, 600, SbColor(1.0, 1.0, 1.0));

		// rendered scene is now in tex
		SbVec2s size;
		int nc;

		const unsigned char* imgData = tex->image.getValue(size, nc);



		//windows printing hack starts here!

		PRINTDLG pd;

		memset( &pd, 0, sizeof( pd ) );

		pd.lStructSize = sizeof( pd );

		/* 
		 * get rid of PD_RETURNDEFAULT on the line below if you'd like to
		 * see the "Printer Settings" dialog!
		 *
		 */
		pd.Flags = PD_RETURNDEFAULT | PD_RETURNDC;

		// try to retrieve the printer DC
		if( !PrintDlg( &pd ) )
		{
			printf("SoPrintKit: Failed to create print dialog... aborting!\n");

			return FALSE;
		}

		DOCINFO di;
		HDC hPrinter = pd.hDC;

		// initialization of the printing!
		memset( &di, 0, sizeof( di ) );
		di.cbSize = sizeof( di );
		StartDoc( hPrinter, &di );

		// start the first and only page
		StartPage( hPrinter );

		TextOut( hPrinter, 40, 50, "Printed by Studierstube:", 24 );

		HDC memDC = CreateCompatibleDC(hPrinter);

		HBITMAP printImg = CreateBitmap(800, 600, nc, 8, imgData);
 
		if (SelectObject(memDC, printImg))
		{
    		StretchBlt(hPrinter, 0, 650, 768, -576, memDC, 0, 0, 800, 600, SRCCOPY);
		}
		else
		{
			printf("SoPrintKit: Failed to select bitmap into printing context!\n");
		}


		// finish the first page
		EndPage( hPrinter );

		// end this document and release the printer's DC
		EndDoc( hPrinter );
		DeleteDC( hPrinter );

		tex->unref();
		printRoot->unref();
		printCam->unref();
        return true;
    }

    return false;

}

