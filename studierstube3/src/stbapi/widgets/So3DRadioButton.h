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
/** So3DRadioButton header file
 *
 *
 * @author Hermann Wurnig, Gerd Hesina
 *
 * $Id: So3DRadioButton.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SO3DRADIOBUTTON_H__
#define __SO3DRADIOBUTTON_H__

#include <stbapi/widgets/SoBase3DButton.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DRadioButton;

typedef struct _radioButtonEntry
{
	SbString *					groupName;
	So3DRadioButton *			button;
	struct _radioButtonEntry *	next;
} radioButtonEntry;

typedef void So3DRadioButtonCB(void *userData, So3DRadioButton *cbox);

////////////////////////////////////////////////////////////////////
//
//  Class: So3DRadioButton
//
//  Contains all functions for handling a 3D-Button. Acts like
//  a SoSeparator-Node. 
//
////////////////////////////////////////////////////////////////////
/**
 * @ingroup oldwidgets
 * @ingroup deprecated
 */
class STBAPI_API So3DRadioButton : public SoBase3DButton {

	SO_KIT_HEADER(So3DRadioButton);

  public:

	SoSFString separatorName;
		// member-variables
		// member-methods
	static void initClass();	
		   void initFields(char *rootSep);	

				So3DRadioButton();				
				So3DRadioButton(char *rootSep, float width, float height, float depth);
				So3DRadioButton(char *rootSep, float width, float height, float depth, 
						SoTexture2 *buttonTexture, float xDist = 0,
						float yDist = 0);
				So3DRadioButton(char *rootSep, float width, float height, float depth, 
						char *fontName, int fontSize, char *buttonText,
						float textHeight, SbColor textColor);

	virtual SbBool isInterested(SoHandle3DEventAction *h3a);
	virtual void handle3DEvent(SoHandle3DEventAction *h3a);
				
	void		addPressCallback(So3DRadioButtonCB *funcCB, void *userData=NULL);
	void		addReleaseCallback(So3DRadioButtonCB *funcCB, void *userData=NULL);
	SbBool		removePressCallback(So3DRadioButtonCB *funcCB);
	SbBool		removeReleaseCallback(So3DRadioButtonCB *funcCB);

	void		setPressed(SbBool pressB, SbBool forceUpdate=FALSE);
    void        updateFromNetwork(SbBool pressedState);
  protected:
		// member-variables
	SbPList    *			 funcList;		// list of callback-function *
	SbPList    *			 funcRList;		// list of callback-function * for release
    static radioButtonEntry *radioButtonList;
	static int 				 classInitAttemptsRadio;
  	SoFieldSensor *			 groupNameSensor;


	static void groupNameChangeCB(void *data, SoSensor *);

  private:
		// member-variables


		// member-methods

    virtual	   ~So3DRadioButton();	

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
