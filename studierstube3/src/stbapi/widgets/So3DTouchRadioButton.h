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
/** So3DTouchRadioButton header file
 *
 *
 * @author Jan Prikryl
 *
 * $Id: So3DTouchRadioButton.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __So3DTouchRadioButton_H__
#define __So3DTouchRadioButton_H__

#include <stbapi/widgets/SoBase3DButton.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DTouchRadioButton;

typedef struct _touchRadioButtonEntry
{
	SbString                      * groupName;
	So3DTouchRadioButton          *	button;
	struct _touchRadioButtonEntry *	next;
} touchRadioButtonEntry;

typedef void So3DTouchRadioButtonCB(void *userData, So3DTouchRadioButton *cbox);

////////////////////////////////////////////////////////////////////
//
//  Class: So3DTouchRadioButton
//
//  Contains all functions for handling a 3D-Button. Acts like
//  a SoSeparator-Node. 
//
////////////////////////////////////////////////////////////////////
/**
 * @ingroup oldwidgets
 * @ingroup deprecated
 */
class STBAPI_API So3DTouchRadioButton : public SoBase3DButton {

	SO_KIT_HEADER(So3DTouchRadioButton);

  public:

	SoSFString separatorName;
		// member-variables
		// member-methods
	static void initClass();	
		   void initFields(char *rootSep);	

				So3DTouchRadioButton();				
				So3DTouchRadioButton(char *rootSep, float width, float height, float depth);
				So3DTouchRadioButton(char *rootSep, float width, float height, float depth, 
						SoTexture2 *buttonTexture, float xDist = 0,
						float yDist = 0);
				So3DTouchRadioButton(char *rootSep, float width, float height, float depth, 
						char *fontName, int fontSize, char *buttonText,
						float textHeight, SbColor textColor);

	virtual SbBool isInterested(SoHandle3DEventAction *h3a);
	virtual void handle3DEvent(SoHandle3DEventAction *h3a);
				
	void		addPressCallback(So3DTouchRadioButtonCB *funcCB, void *userData=NULL);
	void		addReleaseCallback(So3DTouchRadioButtonCB *funcCB, void *userData=NULL);
	SbBool		removePressCallback(So3DTouchRadioButtonCB *funcCB);
	SbBool		removeReleaseCallback(So3DTouchRadioButtonCB *funcCB);

	void		setPressed(SbBool pressB);

  protected:
		// member-variables
	SbPList                       * funcList;		// list of callback-function *
	SbPList                       * funcRList;		// list of callback-function * for release
    static touchRadioButtonEntry  * touchRadioButtonList;
	static int                      classInitAttemptsTouchRadio;
  	SoFieldSensor                 * groupNameSensor;

	static void groupNameChangeCB(void *data, SoSensor *);

  private:
		// member-variables


		// member-methods

    virtual	   ~So3DTouchRadioButton();	

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
