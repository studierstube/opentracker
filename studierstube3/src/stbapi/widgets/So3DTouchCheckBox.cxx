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
/** So3DTouchCheckBox - implementation
  *
  * @author Jan Prikryl
  *
  * $Id: So3DTouchCheckBox.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stbapi/widgets/So3DTouchCheckBox.h>

#define RELEASE_UPON_ENTER

int So3DTouchCheckBox::classInitAttempts = 0;

SO_KIT_SOURCE(So3DTouchCheckBox);

//###################################################################
//## CALLBACK-MANAGEMENT FUNCTIONS
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:        addPressCallback
// Purpose:     This function adds the given pointer funcCB to the
//              list of function-pointer, which are called when
//              the user presses the button down
// Parameters:  funcCB  : pointer to callback-function  
//              userData: void-pointer which is used as a parameter
//                        for the callback-function
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DTouchCheckBox::addPressCallback(So3DTouchCheckBoxCB *funcCB, void *userData)
{
    pressCBList->append((void*)funcCB);
    pressCBList->append(userData);
}

/////////////////////////////////////////////////////////////////////
// Name:        removePressCallback
// Purpose:     This function removes the given pointer funcCB from
//              the list of function-pointer, which are called when
//              the user presses the button down
// Parameters:  funcCB  : pointer to callback-function  
// Returnvalue: TRUE if removing pointer was successful, else FALSE 
/////////////////////////////////////////////////////////////////////
SbBool
So3DTouchCheckBox::removePressCallback(So3DTouchCheckBoxCB *funcCB)
{
    // try to find the pointer in the list
    int i = pressCBList->find((void*)funcCB);  
    // if not found return FALSE
    if (i == -1) return FALSE;
    // remove pointer to callback and to the user data
    // and return TRUE
    pressCBList->remove(i);
    pressCBList->remove(i);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////
// Name:        addReleaseCallback
// Purpose:     This function adds the given pointer funcCB to the
//              list of function-pointer, which are called when
//              the user releases the button
// Parameters:  funcCB  : pointer to callback-function  
//              userData: void-pointer which is used as a parameter
//                        for the callback-function
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DTouchCheckBox::addReleaseCallback(So3DTouchCheckBoxCB *funcCB, void *userData)
{
    releaseCBList->append((void*)funcCB);
    releaseCBList->append(userData);
}

/////////////////////////////////////////////////////////////////////
// Name:        removeReleaseCallback
// Purpose:     This function removes the given pointer funcCB from
//              the list of function-pointer, which are called when
//              the user releases the button
// Parameters:  funcCB  : pointer to callback-function  
// Returnvalue: TRUE if removing pointer was successful, else FALSE 
/////////////////////////////////////////////////////////////////////
SbBool
So3DTouchCheckBox::removeReleaseCallback(So3DTouchCheckBoxCB *funcCB)
{
    // try to find the pointer in the list
    int i = releaseCBList->find((void*)funcCB);
    // if not found return FALSE
    if (i == -1) return FALSE;
    // remove pointers to callback and to the user data 
    // and return TRUE
    releaseCBList->remove(i);
    releaseCBList->remove(i);
    return TRUE;
}

//###################################################################
//## INITIALIZATION FUNCTIONS 
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:        initClass
// Purpose:     sets up type- and file-format name-information for
//              the class
// Parameters:  ---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DTouchCheckBox::initClass()
{
    if (So3DTouchCheckBox::classInitAttempts == 0)
    {
        So3DTouchCheckBox::classInitAttempts = 1;
        SoBase3DButton::initClass();
        
        // use initialization-macro
        SO_KIT_INIT_CLASS(So3DTouchCheckBox, SoBase3DButton, "SoBase3DButton");
    }
}

/////////////////////////////////////////////////////////////////////
// Name:        initFields
// Purpose:     defines the fields used by So3DTouchCheckBox
// Parameters:  ---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DTouchCheckBox::initFields()
{
    SO_KIT_CONSTRUCTOR(So3DTouchCheckBox);
    SO_KIT_INIT_INSTANCE();
    
    pressCBList         = new SbPList;
    releaseCBList       = new SbPList;
    dragging            = FALSE;
    wasPressedUponEnter = FALSE;
    inside              = 0;
}


//###################################################################
//## CONSTRUCTORS/DESTRUCTOR
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:        So3DTouchCheckBox
// Purpose:     Default-Constructor of class So3DTouchCheckBox
// Parameters:  ---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchCheckBox::So3DTouchCheckBox()
{
    // init the fiels of So3DTouchCheckBox
    initFields();
}


/////////////////////////////////////////////////////////////////////
// Name:        So3DTouchCheckBox
// Purpose:     Constructor of class So3DTouchCheckBox that sets the button
//              to a given size
// Parameters:  width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchCheckBox::So3DTouchCheckBox(float width, float height, float depth):
SoBase3DButton(width,height,depth)
{
    // init the fiels of So3DTouchCheckBox
    initFields();
}

/////////////////////////////////////////////////////////////////////
// Name:        So3DTouchCheckBox
// Purpose:     Constructor of class So3DTouchCheckBox that sets the button
//              to a given size and puts a given textture on it
// Parameters:  width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchCheckBox::So3DTouchCheckBox(float width, float height, float depth, 
                                     SoTexture2 *buttonTexture, float xDist,
                                     float yDist):
SoBase3DButton(width,height,depth,buttonTexture,xDist,yDist)
{
    // init the fiels of So3DTouchCheckBox
    initFields();
}

/////////////////////////////////////////////////////////////////////
// Name:        So3DTouchCheckBox
// Purpose:     Constructor of class So3DTouchCheckBox that sets the button
//              to a given size and puts a given textture on it
// Parameters:  width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchCheckBox::So3DTouchCheckBox(float width, float height, float depth, 
                                     char *fontName, int fontSize, char *buttonText, 
                                     float textHeight, SbColor textColor):
SoBase3DButton(width,height,depth,fontName,fontSize,buttonText,
               textHeight, textColor)
               
{
    // init the fiels of So3DTouchCheckBox
    
    initFields();
}

/////////////////////////////////////////////////////////////////////
// Name:        ~So3DTouchCheckBox
// Purpose:     Destructor of class So3DTouchCheckBox
// Parameters:  ---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchCheckBox::~So3DTouchCheckBox()
{
}


//###################################################################
//## 3D-EVENT-HANDLING
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:        isInterested
// Purpose:     returns TRUE if the handle3DEvent-method has to be
//              called ( i.e. the button is interested in the event)
// Parameters:  h3a:    event-handle
// Returnvalue: TRUE if interested, FALSE if not
/////////////////////////////////////////////////////////////////////
SbBool 
So3DTouchCheckBox::isInterested(SoHandle3DEventAction *h3a)
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

    // if position of pen is inside button
    if ( xfbox.intersect(ev->getTranslation())==TRUE)
    {
        // then interested
        h3a->setGrabber(this);
        h3a->setHandled();
        // remember button state upon enter
        wasPressedUponEnter = isPressed();
        return TRUE;
    }
    // else the pen is outside
    
    return FALSE;
}

/////////////////////////////////////////////////////////////////////
// Name:        handle3DEvent
// Purpose:     this method is called whenever the event is
//              interesting for the button
// Parameters:  h3a:    event-handle
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DTouchCheckBox::handle3DEvent(SoHandle3DEventAction *h3a)
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
    
	// if pen is outside ...
	if (xfbox.intersect(ev->getTranslation()) == FALSE )
	{
        // ... check if it has just leaved button's boundingbox
        if (inside)
        {
#ifndef RELEASE_UPON_ENTER
            // ... and if so
            if ( wasPressedUponEnter )
            {
                // ... call all "release" callbacks, if the button has been
                // switched to "released" state during this cycle
                void (* function)(void *userData, So3DTouchCheckBox *button);
                for (int i=0; i<releaseCBList->getLength(); i+=2)
                {
                    function = (void (*)(void *, So3DTouchCheckBox *))(*(releaseCBList))[i];
                    (*function)((*(releaseCBList))[i+1], this);
                }

                // ... and release the button
                setPressed(FALSE);
            }
#endif
            inside = 0;             // indicate the pointer has never been inside and
            setHighlighted(FALSE);	// switch highlighting off and
	        h3a->releaseGrabber();	// release grabber and
		    h3a->setHandled();		// set the event handled
    	}
    }
    else
    {
        // the pen is inside button's boundingbox; it may just now
        // come in, but it might also been in already for longer 
        // time; value of `inside' tells us
        if ( ! inside )
        {
            // check, whether the button was already pressed ...
            setHighlighted(TRUE);
            if (isPressed() == FALSE)
	        {
                // ... if no, call "pressed" callbacks
                void (* function)(void *userData, So3DTouchCheckBox *button);
                for (int i=0; i<pressCBList->getLength(); i+=2)
                {
                    function = (void (*)(void *, So3DTouchCheckBox *))(*(pressCBList))[i];
                    (*function)((*(pressCBList))[i+1], this);
                }
                // ... and "press" the button
                setPressed(TRUE);
            }
#ifdef RELEASE_UPON_ENTER
            else
            {
                // ... if yes, call "release" callbacks
                void (* function)(void *userData, So3DTouchCheckBox *button);
                for (int i=0; i<releaseCBList->getLength(); i+=2)
                {
                    function = (void (*)(void *, So3DTouchCheckBox *))(*(releaseCBList))[i];
                    (*function)((*(releaseCBList))[i+1], this);
                }

                // ... and release the button
                setPressed(FALSE);
            }
#endif
            // ... and register to pointer to be "inside" the button's boundingbox,
            // therefore blocking subsequent calls of the "pressed" callback loop
            // until the pointer leaves this button
            inside = 1;
        }
    }
}
