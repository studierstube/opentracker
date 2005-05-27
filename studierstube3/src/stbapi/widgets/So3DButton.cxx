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
/** So3DButton - implementation
  *
  * @author Hermann Wurnig, Gerd Hesina
  *
  * $Id: So3DButton.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/StbWorkspace.h>

int So3DButton::classInitAttempts = 0;

SO_KIT_SOURCE(So3DButton);


//###################################################################
//## CALLBACK-MANAGEMENT FUNCTIONS
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		addPressCallback
// Purpose:		This function adds the given pointer funcCB to the
//				list of function-pointer, which are called when
//				the user presses the button down
// Parameters:	funcCB	: pointer to callback-function	
//				userData: void-pointer which is used as a parameter
//						  for the callback-function
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DButton::addPressCallback(So3DButtonCB *funcCB, void *userData)
{
	funcPList->append((void*)funcCB);
	funcPList->append(userData);
}

/////////////////////////////////////////////////////////////////////
// Name:		addReleaseCallback
// Purpose:		This function adds the given pointer funcCB to the
//				list of function-pointer, which are called when
//				the user releases the button
// Parameters:	funcCB	: pointer to callback-function	
//				userData: void-pointer which is used as a parameter
//						  for the callback-function
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DButton::addReleaseCallback(So3DButtonCB *funcCB, void *userData)
{
	funcRList->append((void*)funcCB);
	funcRList->append(userData);
}

/////////////////////////////////////////////////////////////////////
// Name:		removePressCallback
// Purpose:		This function removes the given pointer funcCB from
//				the list of function-pointer, which are called when
//				the user presses the button down
// Parameters:	funcCB	: pointer to callback-function	
// Returnvalue: TRUE if removing pointer was successful, else FALSE 
/////////////////////////////////////////////////////////////////////
SbBool
So3DButton::removePressCallback(So3DButtonCB *funcCB)
{
	int i;	
		// try to find the pointer in the list
	i=funcPList->find((void*)funcCB);
		// if not found return FALSE
	if (i==-1) 
		return FALSE;
		// remove pointer and return TRUE
	funcPList->remove(i);
	funcPList->remove(i);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// Name:		removeReleaseCallback
// Purpose:		This function removes the given pointer funcCB from
//				the list of function-pointer, which are called when
//				the user releases the button 
// Parameters:	funcCB	: pointer to callback-function	
// Returnvalue: TRUE if removing pointer was successful, else FALSE 
/////////////////////////////////////////////////////////////////////
SbBool
So3DButton::removeReleaseCallback(So3DButtonCB *funcCB)
{
	int i;
		// try to find the pointer in the list
	i=funcRList->find((void*)funcCB);
		// if not found return FALSE
	if (i==-1) 
		return FALSE;
		// remove pointer and return TRUE
	funcRList->remove(i);
	funcRList->remove(i);
	return TRUE;
}


//###################################################################
//## INITIALIZATION FUNCTIONS 
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		initClass
// Purpose:		sets up type- and file-format name-information for
//				the class
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DButton::initClass()
{
	if (So3DButton::classInitAttempts == 0)
	{
		So3DButton::classInitAttempts = 1;
		SoBase3DButton::initClass();

			// use initialization-macro
		SO_KIT_INIT_CLASS(So3DButton, SoBase3DButton, "SoBase3DButton");
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		defines the fields used by So3DButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DButton::initFields()
{
	SO_KIT_CONSTRUCTOR(So3DButton);
	
	SO_KIT_INIT_INSTANCE();

	funcPList=new SbPList;
	funcRList=new SbPList;
}


//###################################################################
//## CONSTRUCTORS/DESTRUCTOR
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		So3DButton
// Purpose:		Default-Constructor of class So3DButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DButton::So3DButton()
{
		// init the fiels of So3DButton
	initFields();
}


/////////////////////////////////////////////////////////////////////
// Name:		So3DButton
// Purpose:		Constructor of class So3DButton that sets the button
//				to a given size
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DButton::So3DButton(float width, float height, float depth):
	SoBase3DButton(width,height,depth)
{
		// init the fiels of So3DButton
	initFields();
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DButton
// Purpose:		Constructor of class So3DButton that sets the button
//				to a given size and puts a given textture on it
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DButton::So3DButton(float width, float height, float depth, 
					   SoTexture2 *buttonTexture, float xDist,
					   float yDist):
		SoBase3DButton(width,height,depth,buttonTexture,xDist,yDist)

{
		// init the fiels of So3DButton
	initFields();
 }

/////////////////////////////////////////////////////////////////////
// Name:		So3DButton
// Purpose:		Constructor of class So3DButton that sets the button
//				to a given size and puts a given textture on it
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DButton::So3DButton(float width, float height, float depth, 
		char *fontName, int fontSize, char *buttonText, 
		float textHeight, SbColor textColor):
	SoBase3DButton(width,height,depth,fontName,fontSize,buttonText,
				   textHeight, textColor)

{
		// init the fiels of So3DButton

	initFields();
}

/////////////////////////////////////////////////////////////////////
// Name:		~So3DButton
// Purpose:		Destructor of class So3DButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DButton::~So3DButton()
{
}


//###################################################################
//## 3D-EVENT-HANDLING
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		isInterested
// Purpose:		returns TRUE if the handle3DEvent-method has to be
//				called ( i.e. the button is interested in the event)
// Parameters:	h3a:	event-handle
// Returnvalue: TRUE if interested, FALSE if not
/////////////////////////////////////////////////////////////////////
SbBool 
So3DButton::isInterested(SoHandle3DEventAction *h3a)
{
//printf("button %x received event\n",this);
		// get underlying event
	So3DEvent *ev = (So3DEvent*) h3a->getEvent();
		// get path to button

	const SoPath *rootPath = h3a->getPathFromRoot();
	SoPath *currPath = rootPath->copy();
	currPath->ref();

	currPath->append(kitSeparator.getValue());
	currPath->append(topSeparator.getValue());

	SbViewportRegion vp;

	SoGetBoundingBoxAction bba(vp);
	bba.apply((SoFullPath*)currPath);

	currPath->unref();

		// calculate the non-axis alined bounding-box
	SbXfBox3f xfbox=bba.getXfBoundingBox();
		// if position of pen is inside button
	if(xfbox.intersect(ev->getTranslation())==TRUE)
	{		// then interested
		setHighlighted(TRUE);
		h3a->setGrabber(this);
		h3a->setHandled();
		inside=1;
		return TRUE;
	}
		// else the pen is outside

	return FALSE;
}
	
/////////////////////////////////////////////////////////////////////
// Name:		handle3DEvent
// Purpose:		this method is called whenever the event is
//				interesting for the button
// Parameters:	h3a:	event-handle
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DButton::handle3DEvent(SoHandle3DEventAction *h3a)
{
	So3DEvent *ev = (So3DEvent*)h3a->getEvent();

	const SoPath *rootPath = h3a->getPathFromRoot();
	SoPath *currPath = rootPath->copy();
	currPath->ref();

	currPath->append(kitSeparator.getValue());
	currPath->append(topSeparator.getValue());

	SbViewportRegion vp;
	SoGetBoundingBoxAction bba(vp);
	bba.apply((SoFullPath *)currPath);

	currPath->unref();
		// and calculate the body's bounding box
	SbXfBox3f xfbox=bba.getXfBoundingBox();

		// if pen is outside
	if(xfbox.intersect(ev->getTranslation())==FALSE)
	{			
		if ((dragging == TRUE) && (dragStation == ev->getStation()))
		{						
			setPressed(FALSE);		// if pen button still pressed then
			inside=0;				// change to unpressed geometry
		}
		else
		{
			setHighlighted(FALSE);	// if station does not drag then
			h3a->releaseGrabber();	// switch highlighting off and
			h3a->setHandled();		// release grabber
			return;
		}
	}
	else
	if ((dragging == TRUE) && (dragStation == ev->getStation())) 
	{								// if pen inside geometry and
		setPressed(TRUE);			// pen button still pressed then
		inside=1;					// change to pressed geometry
	}


	int i;
	void (*function)(void *userData, So3DButton *button);

		
	if (ev->getType() == triggerButton.getValue()+1)
	{		// when button of pen is pressed
		if ((ev->getButton(triggerButton.getValue()) == TRUE) &&
			(dragging == FALSE) && (inside == 1))
		{
			setPressed(TRUE);				// station starts dragging
            DISABLEDIVFIELD(highlighted);
			highlighted=2;					// when no one else drags
            ENABLEDIVFIELD(highlighted);
			dragging=TRUE;
			dragStation=ev->getStation();
											// execute callbacks
			for (i=0;i<funcPList->getLength();i+=2)
			{		// then execute callbac-funcions
				function = (void (*)(void *, So3DButton *))(*(funcPList))[i];
				(*function)((*(funcPList))[i+1],this);
			}
		}
		else
		if ((ev->getButton(triggerButton.getValue()) == FALSE) &&
			(dragging == TRUE) && (dragStation==ev->getStation()))
		{	
			dragging = FALSE;				// if button released from
			dragStation = -1;				// station that drags then
			setPressed(FALSE);				// stop dragging and
			DISABLEDIVFIELD(highlighted);
			highlighted=1;
			ENABLEDIVFIELD(highlighted);

			if (inside == 1)
			{								// call release callbacks
											// when inside
				for (i=0;i<funcRList->getLength();i+=2)
				{		// execute the release-callbac-functions
					function = (void (*)(void *, So3DButton *))(*(funcRList))[i];
					(*function)((*(funcRList))[i+1],this);
				}
			}
			else
			{
				setHighlighted(FALSE);		// if released when not inside
				h3a->releaseGrabber();		// then switch off highlighting
				h3a->setHandled();			// and call no callbacks
			}
		}
	}
}
