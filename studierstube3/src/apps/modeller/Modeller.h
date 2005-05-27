// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Modeller.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Demonstrator application for the Gesture Recognition Toolkit
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    bas             Benjamin A. Schmit
// ===========================================================================
//  HISTORY:    2000-11-13      first preview version
// ===========================================================================

#ifndef _MODELLER_H_
#define _MODELLER_H_

#include "modeller_win32.h"
#include <Inventor/SbBasic.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <stbapi/interaction/SoDragKit.h>
#include <stbapi/misc/IMGR.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/So3DRadioButton.h>
#include <stbapi/widgets/So3DSlider.h>
#include <stbapi/widgets/SoAlphabetKit.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/workspace/SoContextKit.h>


//////////////////////////////////////////////////////////////////////////////
// Constants:

// Relative location of pip sheet file:
static const char* _ModellerPip= "ModellerSheet.iv";

// Relative location of gesture file:
static const char* _ModellerGestures= "gestures/model.gst";

// Relative location of stored drawings (.../File1.iv - .../File6.iv):
static const char* _ModellerSavePath= "save";

// Version information string for saved objects:
static const char* _ModellerString= "ModellerV1-0";

// Size of the initial drawing window:
static const SbVec3f _ModellerWindowSize= SbVec3f(0.5f, 0.5f, 0.5f);

// Transparency of carrier SoCubes:
static const float _ModellerCarrierTransparency= 0.9f;

// End Constants.
//////////////////////////////////////////////////////////////////////////////


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class MODELLER_API Modeller : public SoContextKit
{
    SO_KIT_HEADER(Modeller);

public:
    static void initClass();
    Modeller();
    SoContextKit* factory();
    ~Modeller();
    SoNode* createPipSheet(
        int uid);
    SoWindowKit* createWindowGeometry(
        int index);
    virtual void setSheetMasterMode(
        SoNode* pipSheet,
        SbBool masterMode);
    static void evSketchUse(            // event 01 - use sketch
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evSketchClear(          // event 02 - clear sketch
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evSketchEnter(          // event 03 - enter sketch
        void* userData,                 // receiver
        SoSketchKit* sketchKit);        // sender
    static void evModelNew(             // event 11 - new model
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evModelClear(           // event 12 - clear model
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evModelEnter(           // event 13 - enter model
        void* userData,                 // receiver
        int gesture,                    // id of the model entered
        SoGestureKit* gestureKit);      // sender
    static void evModelSlider(          // event 14 - adjust sliders
        void* userData,                 // receiver
        So3DSlider* slider);            // sender
    static void evTextUse(              // event 21 - use text
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evTextClear(            // event 22 - clear text
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evTextEnter(            // event 23 - enter text
        void* userData,                 // receiver
        SoAlphabetKit* alpha);          // sender
    static void evTextChange(           // event 24 - change text
        void* userData,                 // receiver
        int,                            // unimportant here
        SoGestureKit* gestureKit);      // sender
    static void evFileNew(              // event 31 - new file
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evFileLoad(             // event 32 - load file
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evFileSave(             // event 33 - save file
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evFileButton(           // event 34 - select file
        void* userData,                 // receiver
        So3DRadioButton* button);       // sender
    static void evMenuDraw(             // event 91 - draw
        void* userData,                 // receiver
        So3DRadioButton* button);       // sender
    static void evMenuModel(            // event 92 - model
        void* userData,                 // receiver
        So3DRadioButton* button);       // sender
    static void evMenuText(             // event 93 - text
        void* userData,                 // receiver
        So3DRadioButton* button);       // sender
    static void evMenuFile(             // event 94 - file
        void* userData,                 // receiver
        So3DRadioButton* button);       // sender
    static void evSelectSketch(         // event 95 - select sketch
        void* userData,                 // receiver
        SoDragKit* dragKit);            // sender
    static void evSelectModel(          // event 96 - select model
        void* userData,                 // receiver
        SoDragKit* dragKit);            // sender
    static void evSelectText(           // event 97 - select text
        void* userData,                 // receiver
        SoDragKit* dragKit);            // sender
    static void evColorButton(          // event 98 - color buttons
        void* userData,                 // receiver
        So3DRadioButton* button);       // sender
    static void evMoveObject(           // event 99 - move / release object
        void* userData,                 // receiver
        SoDragKit* dragKit);            // sender

private:
    So3DSlider** getParameterSliders();   // get PIP sheet parameter sliders
    So3DRadioButton** getColorButtons();  // get PIP sheet color buttons
    So3DRadioButton** getFileButtons();   // get PIP sheet file buttons

    SoSeparator* pip;                     // content of pip
    SoSeparator* window;                  // content of window
    IMGR imgr;                            // object for IMGR processing
    int state;                            // state variable
    int model;                            // active model (sheet 2)
    SoNode* activeNode;                   // active node
    SoMaterial* activeColor;              // active color
    bool master;                          // are we master?
    SbRotation currentRotation;           // accepted rotation value

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

/*
 * Sheets:
 * 0 - draw
 * 1 - model
 * 2 - text
 * 3 - file
 */

/*
 * States:
 * 00 - sheet 0, no current sketch
 * 01 - sheet 0, sketch drawn
 * 10 - sheet 1, no current model
 * 11 - sheet 1, model selected
 * 20 - sheet 2, no current text
 * 21 - sheet 2, text entered
 * 30 - sheet 3, no file selected
 * 31 - sheet 3, file selected
 */

/*
 * Events:
 * 01 - use sketch
 * 02 - clear sketch
 * 03 - enter sketch
 * 11 - new model
 * 12 - clear model
 * 13 - enter model
 * 14 - adjust sliders
 * 21 - use text
 * 22 - clear text
 * 23 - enter text
 * 24 - change text
 * 31 - new file
 * 32 - load file
 * 33 - save file
 * 34 - select file
 * 91 - draw
 * 92 - model
 * 93 - text
 * 94 - file
 * 95 - select sketch
 * 96 - select model
 * 97 - select text
 * 98 - color buttons
 * 99 - move / release object
 *
 * Transitions:
 * state 01 02 03 11 12 13 21 22 23 31 34 91 92 93 94 95 96 97
 *    00 00 00 01 -- -- -- -- -- -- -- -- 00 10 20 3x 01 11 21
 *    01 00 00 01 -- -- -- -- -- -- -- -- 01 10 20 3x 01 11 21
 *    10 -- -- -- 10 10 11 -- -- -- -- -- 00 10 20 3x 01 11 21
 *    11 -- -- -- 10 10 11 -- -- -- -- -- 00 11 20 3x 01 11 21
 *    20 -- -- -- -- -- -- 20 20 21 -- -- 00 10 20 3x 01 11 21
 *    21 -- -- -- -- -- -- 20 20 21 -- -- 00 10 21 3x 01 11 21
 *    30 -- -- -- -- -- -- -- -- -- 30 31 00 10 20 30 01 11 21
 *    31 -- -- -- -- -- -- -- -- -- 30 31 00 10 20 31 01 11 21
 *
 * Notes:
 * --    - not possible (wrong sheet).
 * 3x - depending on last state (i.e. go to 30 or 31).
 * Events that are not listed cause no transitions.
 * Note that x0 and x1 are very similar in their transitions.
 */

#endif
