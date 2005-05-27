// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Trainer.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Training app for the SoGestureKit
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    bas             Benjamin A. Schmit
// ===========================================================================
//  HISTORY:    2000-09-22      first preview version
// ===========================================================================

#ifndef _TRAINER_H_
#define _TRAINER_H_

#include "trainer_win32.h"
#include <Inventor/SbBasic.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/SoAlphabetKit.h>
#include <stbapi/widgets/SoSketchKit.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/workspace/SoContextKit.h>


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class TRAINER_API Trainer : public SoContextKit
{
    SO_KIT_HEADER(Trainer);

public:
    static void initClass();
    Trainer();
    SoContextKit* factory();
    ~Trainer();
    SoNode* createPipSheet(
        int uid);
    SoWindowKit* createWindowGeometry(
        int index);
    virtual void setSheetMasterMode(
        SoNode* pipSheet,
        SbBool masterMode);
    static void evSketchPress(          // event 01 - sketch press
        void* userData,                 // receiver
        SoSketchKit* sketchKit);        // sender
    static void evSketchMove(           // event 02 - sketch move
        void* userData,                 // receiver
        SoSketchKit* sketchKit);        // sender
    static void evSketchRelease(        // event 03 - sketch release
        void* userData,                 // receiver
        SoSketchKit* sketchKit);        // sender
    static void evButt0Up(              // event 04 - button up
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt0Down(            // event 05 - button down
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt0Learn(           // event 06 - button learn
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt0Reject(          // event 07 - button reject
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt0Library(         // event 08 - button library
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt0ID(              // event 09 - button id
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt1Load(            // event 21 - button load
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt1Save(            // event 22 - button save
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt1Back(            // event 23 - button back
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt2OK(              // event 41 - button ok
        void* userData,                 // receiver
        So3DButton* button);            // sender
    static void evButt2Cancel(          // event 42 - button cancel
        void* userData,                 // receiver
        So3DButton* button);            // sender

    /** This scales down the x size of an object.
      * Note that it does not scale up, so the final x size might be less.
      * @param scene The scene graph containing the object.
      * @param node The object to be scaled down.
      * @param size The maximum x size the object should have.
      * @return Factor to scale by.
      */
    static float scaleDownX(SoNode* scene, SoNode* node, float size);

    /** This returns the bounding box of an object.
      * @param scene The scene graph containing the object.
      * @param node The object to be used.
      * @return The bounding box.
      */
    static SbBox3f getBoundingBox(SoNode* scene, SoNode* node);

private:
    SoSeparator* pip;               // the PIP scene graph
    IMGR imgr;                      // access to IMGR functions
    int state;                      // the state of the Trainer
    int state0;                     // backup state on sheet 0
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif


/*
 * Sheets:
 * 0 - train
 * 1 - load/save
 * 2 - id
 */

/*
 * States:
 * 00 - initial, no library loaded
 * 01 - ready
 * 02 - drawing
 * 03 - learn
 * 21 - load/save (sheet 1)
 * 41 - enter id (sheet 2)
 */


/*
 * Events (sheet 0):
 * 01 - PENDN pen button 1 pressed
 * 02 - PENMV pen moved
 * 03 - PENUP pen button 1 released
 * 04 - UP    button "up" pressed
 * 05 - DOWN  button "down" pressed
 * 06 - LEARN button "learn" pressed
 * 07 - REJCT button "reject" pressed
 * 08 - LIB   button "library" pressed
 * 09 - ID    button "id" pressed
 *
 * Transitions:
 * state PENDN PENMV PENUP UP    DOWN  LEARN REJCT LIB   ID
 * 00    *     *     *     *     *     *     *     21    *
 * 01    02s   *     *     01+   01-   *     *     21    41
 * 02    *     *c    03c   *     *     *     *     *     *
 * 03    02r   *     *     01r+  01r-  01l   01r   21r   01i
 *
 * Notes:
 * * keep state
 * + increase gesture id
 * - decrease gesture id
 * s start sketch
 * c continue sketch
 * r reject sketch
 * l learn sketch as active gesture
 * i identify sketch as gesture
 */
 
/*
 * Events (sheet 1):
 * 21 - LOAD button "load" pressed
 * 22 - SAVE button "save" pressed
 * 23 - BACK button "cancel" pressed
 *
 * Transitions:
 * state LOAD  SAVE  BACK  TAB
 * 21    01lBB 01sBB 01bBB 21t
 *
 * Notes:
 * lxx load selected library, if not found -> state xx
 * sxx save selected library, if not found -> state xx
 * bxx no library loaded -> state xx
 * t   try to complete the name of the library entered
 * BB  state that was active on sheet 0 before sheet switch
 */

/*
 * Events (sheet 2):
 * 41 - SET  button "set" pressed
 * 42 - REJ  button "reject" pressed
 *
 * Transitions:
 * state SET   REJ
 * 41    01s41 01r
 *
 * Notes:
 * s   set gesture id
 * r   reject gesture id
 */


#endif
