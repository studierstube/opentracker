/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
//  PROJECT:    Studierstube
//  CONTENT:    So3DButton - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw              Hermann Wurnig
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:28  gh      last modification
// ===========================================================================
#ifndef __SO3DBUTTON_H__
#define __SO3DBUTTON_H__

#include <stbapi/widgets/SoBase3DButton.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DButton;

typedef void So3DButtonCB(void *userData, So3DButton *button);

/**
 * So3DButton contains all functions for handling a 3D-Button. Documented at
 * http://www.studierstube.org/stbapi/So3DButton.html
 * @ingroup oldwidgets
 * @ingroup deprecated
 */
class STBAPI_API So3DButton : public SoBase3DButton {

	SO_KIT_HEADER(So3DButton);

  public:
		// member-variables
		// member-methods
	static void initClass();	
		   void initFields();	

				So3DButton();				
				So3DButton(float width, float height, float depth);
				So3DButton(float width, float height, float depth, 
						SoTexture2 *buttonTexture, float xDist = 0,
						float yDist = 0);
				So3DButton(float width, float height, float depth, 
						char *fontName, int fontSize, char *buttonText,
						float textHeight, SbColor textColor);

	virtual SbBool isInterested(SoHandle3DEventAction *h3a);
	virtual void handle3DEvent(SoHandle3DEventAction *h3a);
				
	void		addPressCallback(So3DButtonCB *funcCB, void *userData=NULL);
	void		addReleaseCallback(So3DButtonCB *funcCB, void *userData=NULL);
	SbBool		removePressCallback(So3DButtonCB *funcCB);
	SbBool		removeReleaseCallback(So3DButtonCB *funcCB);


  protected:
	SbPList    *funcPList,		// list of callback-function *
			   *funcRList;		// for press/release event
	static int 				 classInitAttempts;
		// member-variables
  private:
		// member-methods
	virtual	   ~So3DButton();	

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

