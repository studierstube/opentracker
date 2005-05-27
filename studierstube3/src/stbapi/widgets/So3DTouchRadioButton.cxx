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
/** So3DTouchRadioButton - implementation
  *
  * @author Jan Prikryl
  *
  * $Id: So3DTouchRadioButton.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cstdlib>
#include <stbapi/widgets/So3DTouchRadioButton.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <cstring>

SO_KIT_SOURCE(So3DTouchRadioButton);

touchRadioButtonEntry *	So3DTouchRadioButton::touchRadioButtonList = NULL;
int                     So3DTouchRadioButton::classInitAttemptsTouchRadio = 0;


void 
So3DTouchRadioButton::groupNameChangeCB(void *data, SoSensor *)
{
  touchRadioButtonEntry * currentEntry = So3DTouchRadioButton::touchRadioButtonList;
  So3DTouchRadioButton  * self = (So3DTouchRadioButton *) data;

  // search list for entry

  while ((currentEntry->next != NULL) && 
	 (currentEntry->next->button != self))
    currentEntry = currentEntry->next;

  if (currentEntry->next == NULL)
    {					// no entry exists -> create new one
      currentEntry = (touchRadioButtonEntry *) 
	malloc(sizeof(touchRadioButtonEntry));

      SbString *buttonGroupName = 
	new SbString(self->separatorName.getValue().getString());

      currentEntry->button = self;
      currentEntry->groupName = buttonGroupName;
      currentEntry->next = So3DTouchRadioButton::touchRadioButtonList->next;

      So3DTouchRadioButton::touchRadioButtonList->next = currentEntry;
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
So3DTouchRadioButton::addPressCallback(So3DTouchRadioButtonCB *funcCB, void *userData)
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
So3DTouchRadioButton::removePressCallback(So3DTouchRadioButtonCB *funcCB)
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
So3DTouchRadioButton::addReleaseCallback(So3DTouchRadioButtonCB *funcCB, void *userData)
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
So3DTouchRadioButton::removeReleaseCallback(So3DTouchRadioButtonCB *funcCB)
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
So3DTouchRadioButton::setPressed(SbBool pressB)
{		// only change value when necessary
  touchRadioButtonEntry *currentEntry = So3DTouchRadioButton::touchRadioButtonList;	


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
So3DTouchRadioButton::initClass()
{
  if (So3DTouchRadioButton::classInitAttemptsTouchRadio == 0)
    {
      So3DTouchRadioButton::classInitAttemptsTouchRadio = 1;

      SoBase3DButton::initClass();

      // setup structures for radio button handling
		
      So3DTouchRadioButton::touchRadioButtonList = (touchRadioButtonEntry *) 
	malloc (sizeof(touchRadioButtonEntry));
	
      So3DTouchRadioButton::touchRadioButtonList->next = NULL;
      So3DTouchRadioButton::touchRadioButtonList->button = NULL;

      // use initialization-macro
      SO_KIT_INIT_CLASS(So3DTouchRadioButton, SoBase3DButton, "SoBase3DButton");

    }
}

/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		defines the fields used by So3DTouchRadioButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DTouchRadioButton::initFields(char *rootSep)
{
  SO_KIT_CONSTRUCTOR(So3DTouchRadioButton);
	
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
// Name:		So3DTouchRadioButton
// Purpose:		Default-Constructor of class So3DTouchRadioButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchRadioButton::So3DTouchRadioButton()
{
  // init the fiels of So3DTouchRadioButton
  initFields("");
}


/////////////////////////////////////////////////////////////////////
// Name:		So3DTouchRadioButton
// Purpose:		Constructor of class So3DTouchRadioButton that sets the button
//				to a given size
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchRadioButton::So3DTouchRadioButton(char *rootSep, float width, float height, float depth):
  SoBase3DButton(width,height,depth)
{
  // init the fiels of So3DTouchRadioButton
  initFields(rootSep);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DTouchRadioButton
// Purpose:		Constructor of class So3DTouchRadioButton that sets the button
//				to a given size and puts a given textture on it
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchRadioButton::So3DTouchRadioButton(char *rootSep, float width, float height, float depth, 
					   SoTexture2 *buttonTexture, float xDist,
					   float yDist):
  SoBase3DButton(width,height,depth,buttonTexture,xDist,yDist)
{
  // init the fiels of So3DTouchRadioButton
  initFields(rootSep);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DTouchRadioButton
// Purpose:		Constructor of class So3DTouchRadioButton that sets the button
//				to a given size and puts a given textture on it
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchRadioButton::So3DTouchRadioButton(char *rootSep, float width, float height, float depth, 
					   char *fontName, int fontSize, char *buttonText, 
					   float textHeight, SbColor textColor):
  SoBase3DButton(width,height,depth,fontName,fontSize,buttonText,
		 textHeight, textColor)

{
  // init the fiels of So3DTouchRadioButton

  initFields(rootSep);
}

/////////////////////////////////////////////////////////////////////
// Name:		~So3DTouchRadioButton
// Purpose:		Destructor of class So3DTouchRadioButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchRadioButton::~So3DTouchRadioButton()
{
  touchRadioButtonEntry *currentEntry = So3DTouchRadioButton::touchRadioButtonList;
  touchRadioButtonEntry *hptr;

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
So3DTouchRadioButton::isInterested(SoHandle3DEventAction *h3a)
{
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

  // if pen is inside button
  if (xfbox.intersect(ev->getTranslation()) == TRUE)
    {
      // then interested
      h3a->setGrabber(this);
      h3a->setHandled();
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
So3DTouchRadioButton::handle3DEvent(SoHandle3DEventAction *h3a)
{
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
  if (xfbox.intersect(ev->getTranslation()) == FALSE )
    {
      if ( inside )
        {
	  void (* function)(void *userData, So3DTouchRadioButton *button);
	  int i;

	  //printf("calling release callbacks\n");

	  // pen leaves the button, call "release" callbacks
	  for (i=0; i<funcRList->getLength(); i+=2)
            {
	      function = (void (*)(void *, So3DTouchRadioButton *))(*(funcRList))[i];
	      (*function)((*(funcRList))[i+1], this);
	    }

	  inside = 0;             // indicate the pointer has never been in and
    	}

      setHighlighted(FALSE);	// switch highlighting off and
      h3a->releaseGrabber();	// release grabber and
      h3a->setHandled();		// set the event handled
    }
  else
    {
      if (isPressed() == FALSE)
	{
	  // emulate "press"
	  // touchRadioButtonEntry * currentEntry = So3DTouchRadioButton::touchRadioButtonList;

	  // look for any other pressed button in the radio list, 
	  // skipping the first item of the entry list
	  /*
            while ((currentEntry = currentEntry->next) != NULL)
	    {
	    if (! strcmp(currentEntry->groupName->getString(),
	    separatorName.getValue().getString()))
	    {
	    if (currentEntry->button->isPressed()) break;
	    }
            }

            // if we found a previously pressed button, release it now
            if (currentEntry != NULL)
            {
	    currentEntry->button->setPressed(FALSE);
            }
	  */

	  // ... and set this button as pressed
	  setPressed(TRUE);
        }

      // if the button is already in "pressed" state (might be an already
      // pressed button as well), but the pointer has not yet been registered
      // as being inside, execute any "pressed" callbacks
      if ( !inside && isPressed() == TRUE )
        {
	  void (* function)(void *userData, So3DTouchRadioButton *button);
	  for (int i=0; i<funcList->getLength(); i+=2)
            {
	      function = (void (*)(void *, So3DTouchRadioButton *))(*(funcList))[i];
	      (*function)((*(funcList))[i+1], this);
	    }
	  // the current state of the radio button is "pointer inside"
	  inside = 1;
	  setHighlighted(TRUE);
	}
    }
}
