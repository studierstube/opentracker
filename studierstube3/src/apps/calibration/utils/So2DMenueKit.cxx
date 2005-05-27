 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** So2DMenueKit.h contains the implementation of class So2DMenueKit
  *
  * @author Rainer Splechtna
  *
  * $Id: So2DMenueKit.cxx 4187 2005-04-28 06:28:41Z bornik $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/resource/SoUserManagerKit.h>

#include <stbapi/util/ivio.h>

#ifdef USE_GLEW
#include <GL/glew.h>
#endif
#include <GL/gl.h>

#include "SoSampler.h"

#include "So2DMenueKit.h"

SO_KIT_SOURCE(So2DMenueKit);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
So2DMenueKit::initClass()
{
  SO_KIT_INIT_CLASS(So2DMenueKit, SoBaseKit, "SoBaseKit");
}

//----------------------------------------------------------------------------
/** The constructor of the kit inits the fields and connects the tracker. */

So2DMenueKit::So2DMenueKit()
: activeEntry(-1), callback(NULL), isEnabled(0), b1(NULL), b2(NULL)
{
    SO_KIT_CONSTRUCTOR(So2DMenueKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        onOffSwitch,    SoSwitch,FALSE,this,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        menueRoot,    SoSeparator,FALSE, onOffSwitch,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        textNormal,      SoMessageKit,         FALSE, menueRoot,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        textHighlighted, SoMessageKit,         FALSE, menueRoot,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        control, SoEventCallback,         FALSE, menueRoot,    \0,FALSE );

    // add field
    SO_KIT_ADD_FIELD(menueText, ("")); 
    SO_KIT_ADD_FIELD(color, (1, 0, 0)); 
    SO_KIT_ADD_FIELD(menueSize, (1, 1)); 
    SO_KIT_ADD_FIELD(menuePosition, (0, 0)); 
    SO_KIT_ADD_FIELD(highlightColor, (0, 1, 0)); 
    SO_KIT_ADD_FIELD(fontName, ("Utopia")); 
 
    SO_KIT_INIT_INSTANCE();
    
    //showMenue(TRUE);
}


//----------------------------------------------------------------------------
/** The destructor of the kit does nothing. */

So2DMenueKit::~So2DMenueKit()
{
    if (b1)
        delete(b1);
    if (b2)
        delete(b2);
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::keyEventCB(void *data, SoEventCallback *myEvent) 
{
    const SoEvent *event = myEvent->getEvent();
    SoKeyboardEvent::Key pressed;
 
    So2DMenueKit *actual = (So2DMenueKit*)data;

    if (SoKeyboardEvent::isKeyPressEvent(event, SoKeyboardEvent::ANY)) {
        pressed = ((SoKeyboardEvent *)event)->getKey(); 
        switch (pressed)
        {
            case SoKeyboardEvent::A:
                actual->selectPrevious();
                break;
            case SoKeyboardEvent::Y:
                actual->selectNext();
                break;
            case SoKeyboardEvent::RETURN:
                actual->choose();
                break;
            default: break;
        }
    }

    myEvent->setHandled();
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::connectAndSetFields()
{
    getText()->color = color.getValue();
    getHighlightedText()->color = highlightColor.getValue();
    getText()->fontName = fontName.getValue();
    getHighlightedText()->fontName = fontName.getValue();
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::setEntries(SoMFString &entries)
{
    menueText = entries;
    if (menueText.getNum() > 0)
        activeEntry = 1;
    else
        activeEntry = -1;

    setNewMenueText();
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::insertEntry(SbString newEntry, int index)
{
    menueText.set1Value(index, newEntry);
    setNewMenueText();
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::removeEntry(int index)
{
    menueText.deleteValues(index, 1);
    setNewMenueText();
}

//----------------------------------------------------------------------------
void 
So2DMenueKit::setCallback(So2DMenueKitCB *callback, void *data)
{
    this->callback = callback;
    userData = data;
}

//----------------------------------------------------------------------------
void 
So2DMenueKit::enableMenue(SbBool on)
{
    if (on && !isEnabled)
    {
        connectAndSetFields();
        showMenue(TRUE);
        SO_GET_ANY_PART(this,"control",SoEventCallback)->addEventCallback(
            SoKeyboardEvent::getClassTypeId(), keyEventCB, this);              
        
        setNewMenueText();
        isEnabled = 1;
    }
    else if (!on && isEnabled)
    {
        showMenue(FALSE);
        SO_GET_ANY_PART(this,"control",SoEventCallback)->removeEventCallback(
            SoKeyboardEvent::getClassTypeId(), keyEventCB, this);              
        isEnabled = 0;
    }
}

//----------------------------------------------------------------------------

void
So2DMenueKit::button1Triggered(void *data)
{
    So2DMenueKit *actual = (So2DMenueKit*)data;    
    actual->menueControl(0);  
}

//----------------------------------------------------------------------------

void
So2DMenueKit::button2Triggered(void *data)
{
    So2DMenueKit *actual = (So2DMenueKit*)data;    
    actual->menueControl(1);  
}

//----------------------------------------------------------------------------

void
So2DMenueKit::menueControl(int button)
{
    int a = activeEntry;
    switch (button)
    {
        case 0: choose();
                break;
        case 1: selectNext();
                if (a == activeEntry)
                    select(0);
                break;
        default: break;
    }
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::enableButtonControl(SbBool on, int userID)
{
    if (on)    
    {
        int penStation = SoUserManagerKit::getInstance()->getUserKit(userID)
            ->getPen()->station.getValue();
        if (!b1)
        {
            b1 = new SoSampler;
            b1->initButtonTriggering(penStation, 
                SoUserManagerKit::getInstance()->getUserKit(userID)
                ->getPen()->primaryButton.getValue(),
                FALSE, FALSE, button1Triggered,this);
        }
        if (!b2)
        {
            b2 = new SoSampler;
            b2->initButtonTriggering(penStation, 
                SoUserManagerKit::getInstance()->getUserKit(userID)
                ->getPen()->secondaryButton.getValue(),
                FALSE, FALSE, button2Triggered,this);
        }

        b1->startButtonTriggering();
        b2->startButtonTriggering();
    }
    else 
    {  
        if (b1)
            b1->stopButtonTriggering();
        if (b2)
            b2->stopButtonTriggering();
    }
}



//----------------------------------------------------------------------------
void 
So2DMenueKit::showMenue(SbBool on)
{
    SO_GET_ANY_PART(this,"onOffSwitch",SoSwitch)->whichChild = on ? 0 : -1;
}

//----------------------------------------------------------------------------

int 
So2DMenueKit::getSelectedEntry()
{
    return activeEntry;
}

//----------------------------------------------------------------------------
void
So2DMenueKit::setNewMenueText()
{
    getText()->message = "";
    getHighlightedText()->message = "";
    if (menueText.getNum() == 0)
    {
        activeEntry = -1;
        return;
    }
    else 
        activeEntry = 0;
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    float newAspectRatio = ((float)vp[2]/(float)vp[3]);
    int longestEntry = 0;
    for (int i=0; i<menueText.getNum(); i++)
        if (menueText[i].getLength() > longestEntry)
            longestEntry = menueText[i].getLength();
    float xMax = vp[2]*menueSize.getValue()[0]*1.5/(float)longestEntry;
    float yMax = vp[3]*menueSize.getValue()[1]/(float)(2*menueText.getNum()-1);
    float fontSize = xMax < yMax ? xMax : yMax;
    getText()->message = menueText;
    getText()->fontSize = fontSize;
    getHighlightedText()->fontSize = fontSize;
    
    float yOffset = (menueText.getNum()*0.66 - 1) * fontSize/(float)vp[3];
    SbVec2f position(menuePosition.getValue()[0],menuePosition.getValue()[1]+yOffset);
    getText()->position = position;
    getHighlightedText()->position = position;

    select(0);
}

//----------------------------------------------------------------------------

SoMessageKit *
So2DMenueKit::getText()
{
    return SO_GET_ANY_PART(this,"textNormal",SoMessageKit);
}

//----------------------------------------------------------------------------

SoMessageKit *
So2DMenueKit::getHighlightedText()
{
    return SO_GET_ANY_PART(this,"textHighlighted",SoMessageKit);
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::selectNext()
{
    select(activeEntry+1);
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::selectPrevious()
{
    select(activeEntry-1);
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::select(int index)
{
    if (activeEntry == -1)
        return;

    if (index < 0)
        index = 0;
    else if (index > menueText.getNum()-1)
        index = menueText.getNum()-1;
    if (menueText[index].getLength() == 0)
    {
        if ((index < activeEntry) && (index > 0))
            select(index-1);
        else if (index < menueText.getNum()-1)
            select(index+1);
        return;
    }

    for (int i=0; i<menueText.getNum(); i++)
        if (i == index)
            getHighlightedText()->message.set1Value(i, menueText[i]);
        else
            getHighlightedText()->message.set1Value(i, "");

    activeEntry = index;
}

//----------------------------------------------------------------------------

void 
So2DMenueKit::choose()
{
    if (callback)
        callback(userData, this);
}

//----------------------------------------------------------------------------
//eof

