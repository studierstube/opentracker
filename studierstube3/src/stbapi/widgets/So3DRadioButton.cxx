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
/** So3DRadioButton - implementation
  *
  * @author Hermann Wurnig, Gerd Hesina, Jan Prikryl
  *
  * $Id: So3DRadioButton.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cstdlib>

#include <stbapi/widgets/So3DRadioButton.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <stbapi/workspace/StbWorkspace.h>

SO_KIT_SOURCE(So3DRadioButton);

radioButtonEntry * So3DRadioButton::radioButtonList = NULL;
int 		   So3DRadioButton::classInitAttemptsRadio = 0;


void 
So3DRadioButton::groupNameChangeCB(void *data, SoSensor *)
{
    radioButtonEntry *currentEntry = So3DRadioButton::radioButtonList;
    So3DRadioButton *self = (So3DRadioButton *) data;

    // search list for entry

    while ((currentEntry->next != NULL) && 
           (currentEntry->next->button != self))
        currentEntry = currentEntry->next;

    if (currentEntry->next == NULL)
    {					// no entry exists -> create new one
        currentEntry = (radioButtonEntry *) 
            malloc(sizeof(radioButtonEntry));

        SbString *buttonGroupName = 
            new SbString(self->separatorName.getValue().getString());

        currentEntry->button = self;
        currentEntry->groupName = buttonGroupName;
        currentEntry->next = So3DRadioButton::radioButtonList->next;

        So3DRadioButton::radioButtonList->next = currentEntry;
    }
    else				// entry exists, change group name
    {
        *(currentEntry->next->groupName) =  
            self->separatorName.getValue().getString();
    }
}


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
So3DRadioButton::addPressCallback(So3DRadioButtonCB *funcCB, void *userData)
{
    funcList->append((void*)funcCB);
    funcList->append(userData);
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
So3DRadioButton::removePressCallback(So3DRadioButtonCB *funcCB)
{
    int i;	
    // try to find the pointer in the list
    i=funcList->find((void*)funcCB);
    // if not found return FALSE
    if (i==-1) 
        return FALSE;
    // remove pointer and return TRUE
    funcList->remove(i);
    funcList->remove(i);
    return TRUE;
}

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
So3DRadioButton::addReleaseCallback(So3DRadioButtonCB *funcCB, void *userData)
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
So3DRadioButton::removeReleaseCallback(So3DRadioButtonCB *funcCB)
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
/////////////////////////////////////////////////////////////////////
// Name:		setPressed
// Purpose:		This function sets the field pressed to 1, if 
//				the parameter pressB is TRUE, else to 0
// Parameters:	pressB	: if TRUE -> pressed=1 else pressed=0
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DRadioButton::setPressed(SbBool pressB, SbBool forceUpdate)
{		// only change value when necessary
    radioButtonEntry *currentEntry = So3DRadioButton::radioButtonList;	

    if(!forceUpdate&&getBase3DMode()==Base3D::BASE3D_SLAVEMODE) return;

	if(getBase3DMode()==Base3D::BASE3D_MASTERMODE) ENABLEDIVNODE(this);
	if (pressB == TRUE)	
	{
		
        pressed=1;
        // release all other radio-buttons
        if (dragging==TRUE) return;

        // disable all other radiobuttons of this group
	
        while (currentEntry->next != NULL)
        {

			if ((strcmp(currentEntry->next->groupName->getString(), 
						separatorName.getValue().getString()) == 0)
				&& (currentEntry->next->button != this))
				currentEntry->next->button->setPressed(FALSE);

			currentEntry = currentEntry->next;
		}
	}
	else
		pressed=0;
    if(getBase3DMode()==Base3D::BASE3D_MASTERMODE)
	{
		SETRADIOBUTTONSTATE(this);
		DISABLEDIVNODE(this);
	}
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
So3DRadioButton::initClass()
{
    if (So3DRadioButton::classInitAttemptsRadio == 0)
    {
        So3DRadioButton::classInitAttemptsRadio = 1;

        SoBase3DButton::initClass();

        // setup structures for radio button handling
		
        So3DRadioButton::radioButtonList = (radioButtonEntry *) 
            malloc (sizeof(radioButtonEntry));
	
        So3DRadioButton::radioButtonList->next = NULL;
        So3DRadioButton::radioButtonList->button = NULL;

        // use initialization-macro
        SO_KIT_INIT_CLASS(So3DRadioButton, SoBase3DButton, "SoBase3DButton");

    }
}

/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		defines the fields used by So3DRadioButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DRadioButton::initFields(char *rootSep)
{
    SO_KIT_CONSTRUCTOR(So3DRadioButton);
	
    SO_KIT_ADD_FIELD(separatorName, ("\x0"));

    SO_KIT_INIT_INSTANCE();

    groupNameSensor = new SoFieldSensor(groupNameChangeCB, this);
    groupNameSensor->attach(&separatorName);

    if (rootSep != NULL)
        separatorName.setValue(rootSep);

    funcList=new SbPList;
    funcRList=new SbPList;
}


//###################################################################
//## CONSTRUCTORS/DESTRUCTOR
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		So3DRadioButton
// Purpose:		Default-Constructor of class So3DRadioButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DRadioButton::So3DRadioButton()
{
    // init the fiels of So3DRadioButton
    initFields("");
}


/////////////////////////////////////////////////////////////////////
// Name:		So3DRadioButton
// Purpose:		Constructor of class So3DRadioButton that sets the button
//				to a given size
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DRadioButton::So3DRadioButton(char *rootSep, float width, float height, float depth):
    SoBase3DButton(width,height,depth)
{
    // init the fiels of So3DRadioButton
    initFields(rootSep);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DRadioButton
// Purpose:		Constructor of class So3DRadioButton that sets the button
//				to a given size and puts a given textture on it
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DRadioButton::So3DRadioButton(char *rootSep, float width, float height, float depth, 
                                 SoTexture2 *buttonTexture, float xDist,
                                 float yDist):
    SoBase3DButton(width,height,depth,buttonTexture,xDist,yDist)
{
    // init the fiels of So3DRadioButton
    initFields(rootSep);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DRadioButton
// Purpose:		Constructor of class So3DRadioButton that sets the button
//				to a given size and puts a given textture on it
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DRadioButton::So3DRadioButton(char *rootSep, float width, float height, float depth, 
                                 char *fontName, int fontSize, char *buttonText, 
                                 float textHeight, SbColor textColor):
    SoBase3DButton(width,height,depth,fontName,fontSize,buttonText,
                   textHeight, textColor)

{
    // init the fiels of So3DRadioButton

    initFields(rootSep);
}

/////////////////////////////////////////////////////////////////////
// Name:		~So3DRadioButton
// Purpose:		Destructor of class So3DRadioButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DRadioButton::~So3DRadioButton()
{
    radioButtonEntry *currentEntry = So3DRadioButton::radioButtonList;
    radioButtonEntry *hptr;

    // search list for entry

    while ((currentEntry->next != NULL) && 
           (currentEntry->next->button != this))
        currentEntry = currentEntry->next;

    // and remove it if it can be found

    if (currentEntry->next != NULL)
    {
        hptr = currentEntry->next;
        currentEntry->next = hptr->next;
        free(hptr->groupName);
        free(hptr);
    }
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
So3DRadioButton::isInterested(SoHandle3DEventAction *h3a)
{
    DISABLEDIVNODE(this);
	// get underlying event
	So3DEvent *ev = (So3DEvent*) h3a->getEvent();
		// get path to button

	const SoPath *rootPath = h3a->getPathFromRoot();
	SoPath *currPath = rootPath->copy();
	currPath->ref();
	currPath->append(kitSeparator.getValue());
	currPath->append(topSeparator.getValue());

    static SbViewportRegion vp;

    static SoGetBoundingBoxAction *bba=new SoGetBoundingBoxAction(vp);
    bba->apply((SoFullPath*)currPath);

    currPath->unref();

		// calculate the non-axis alined bounding-box
	SbXfBox3f xfbox=bba->getXfBoundingBox();
		// if position of pen is inside button
	if(xfbox.intersect(ev->getTranslation())==TRUE)
	{		// then interested
		setHighlighted(TRUE);
		h3a->setGrabber(this);
		h3a->setHandled();
		inside=1;
		ENABLEDIVNODE(this);
		return TRUE;
	}
		// else the pen is outside
    ENABLEDIVNODE(this);
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
So3DRadioButton::handle3DEvent(SoHandle3DEventAction *h3a)
{
	DISABLEDIVNODE(this);
	So3DEvent *ev = (So3DEvent*)h3a->getEvent();

    const SoPath *rootPath = h3a->getPathFromRoot();

	SoPath *currPath = rootPath->copy();
	currPath->ref();
	currPath->append(kitSeparator.getValue());
	currPath->append(topSeparator.getValue());
	static SbViewportRegion vp;

	static SoGetBoundingBoxAction *bba=new SoGetBoundingBoxAction(vp);
	bba->apply(currPath);

	currPath->unref();
		// and calculate the body's bounding box
	SbXfBox3f xfbox=bba->getXfBoundingBox();

		// if pen is outside
	if(xfbox.intersect(ev->getTranslation())==FALSE)
	{			
		if ((dragging == TRUE) && (dragStation == ev->getStation()))
		{						
			if (isPressed()==TRUE)
			{
				setPressed(FALSE);	// pressed then change state of checkbox
				inside = 0;
			}
		}
		else						
		{
			setHighlighted(FALSE);	// if station does not drag then
			h3a->releaseGrabber();	// switch highlighting off and
			h3a->setHandled();		// release grabber
            ENABLEDIVNODE(this);
			return;
		}
	}
	else
	if ((dragging == TRUE) && (dragStation == ev->getStation()) && 
            (isPressed()==FALSE)) 
	{
            setPressed(TRUE);	   // if pen moves from outside to inside
            inside = 1;
	}


    int i;
    void (*function)(void *userData, So3DRadioButton *button);

		
    if (ev->getType() == triggerButton.getValue()+1)
    {
        // when button of pen is pressed
        if ((ev->getButton(triggerButton.getValue()) == TRUE) &&
            (dragging == FALSE) && (inside == 1) && (isPressed() == FALSE))
        {
            radioButtonEntry *currentEntry = So3DRadioButton::radioButtonList;
				// search list if any other related RadioButton
				// is currently dragging

            int dragDetected = 0;

			while ((currentEntry = currentEntry->next) != NULL)
			{
				if ((strcmp(currentEntry->groupName->getString(), 
					separatorName.getValue().getString()) == 0) &&
				   (currentEntry->button->dragging == TRUE))
					dragDetected = 1;
			}

			if (dragDetected == 0)
			{
				dragging=TRUE;
				highlighted=2;
				dragStation=ev->getStation();

				setPressed(TRUE);

				for (i=0;i<funcList->getLength();i+=2)
				{		// then execute callbac-funcions
					function = (void (*)(void *, So3DRadioButton *))(*(funcList))[i];
					(*function)((*(funcList))[i+1],this);
				}
			}
		}
		else
		if ((ev->getButton(triggerButton.getValue()) == FALSE) &&
			(dragging == TRUE) && (dragStation==ev->getStation()))
		{
			dragging = FALSE;				// if button released from
			dragStation = -1;				// station that drags then
			highlighted = 1;				// stop dragging and

			if (inside == 1)
			{								// call release callbacks
											// when inside
				for (i=0;i<funcRList->getLength();i+=2)
				{		// then execute callbac-funcions
					function = (void (*)(void *, So3DRadioButton *))(*(funcRList))[i];
					(*function)((*(funcRList))[i+1],this);
				}
				if (isPressed() == TRUE) setPressed(TRUE);
			}
			else
			{
				setHighlighted(FALSE);		// if released when not inside
				h3a->releaseGrabber();		// then switch off highlighting
				h3a->setHandled();			// and call no callbacks
			}
		}
	}
    ENABLEDIVNODE(this);
}

void
So3DRadioButton::updateFromNetwork(SbBool pressedState)
{
	setPressed(pressedState,TRUE);
}

/* ===========================================================================
    End of So3DRadioButton.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */

