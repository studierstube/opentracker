 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** So2DMenueKit.h contains the definition of class So2DMenueKit
  *
  * @author Rainer Splechtna
  *
  * $Id: So2DMenueKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SO2DMENUEKIT_H_
#define _SO2DMENUEKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/nodes/SoEventCallback.h>

#include "../utils/SoMessageKit.h"

class SoSampler;

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//----------------------------------------------------------------------------
class So2DMenueKit;

typedef void So2DMenueKitCB(void *data, So2DMenueKit *menue);

/** So2DMenueKit is a nodekit.

@author Rainer Splechtna
 */
class So2DMenueKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(So2DMenueKit);
    SO_KIT_CATALOG_ENTRY_HEADER(menueRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(onOffSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(textNormal);
    SO_KIT_CATALOG_ENTRY_HEADER(textHighlighted);
    SO_KIT_CATALOG_ENTRY_HEADER(control);

public:

    SoSFColor color;
    SoSFColor highlightColor;
    SoSFVec2f menueSize;
    SoSFVec2f menuePosition;
    SoSFName  fontName;

    /** The constructor of the kit inits the fields.
      */
    So2DMenueKit(
        );
    /** The destructor of the kit does nothing. */
    ~So2DMenueKit(             
        );
    /** Inventor class initialization */
    static void initClass(             
        );

    void setEntries(SoMFString &entries);
    void insertEntry(SbString newEntry, int index = -1);
    void removeEntry(int index);
    //void setStereo(SbBool on);
    void setCallback(So2DMenueKitCB *callback, void *data = NULL);
    //void removeCallback(So2DMenueKitCB *callback, void *data = NULL);
    void enableButtonControl(SbBool on, int userID);
    void enableMenue(SbBool on);
    void showMenue(SbBool on);
    int getSelectedEntry();

private:
    static void keyEventCB(void *data, SoEventCallback *myEvent);
    static void button1Triggered(void *data);
    static void button2Triggered(void *data);
    void menueControl(int button);

    void selectNext();
    void selectPrevious();
    void select(int index);
    void choose();
    SoMessageKit *getText();
    SoMessageKit *getHighlightedText();

    void connectAndSetFields();
    void setNewMenueText();

    SoMFString menueText; 
    So2DMenueKitCB *callback;
    void *userData;
    int activeEntry;
    int isEnabled;
    SoSampler *b1, *b2;
};

//----------------------------------------------------------------------------
#endif//_SO2DMENUKIT_H_

