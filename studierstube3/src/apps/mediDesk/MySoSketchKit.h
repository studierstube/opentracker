// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       MySoSketchKit.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    A sketch pad for the Studierstube system
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    bas             Benjamin A. Schmit
// ===========================================================================
//  HISTORY:    2000-08-09	first preview version
// ===========================================================================
//
// TODO:
// see definitions below for further information
// also see .cxx file

/*
State Information:

The MySoSketchKit knows a number of states which are listed below:
+-------+----------+------------------+--------------------------------------+
| State | Name     | Flags            | Description                          |
+-------+----------+------------------+--------------------------------------+
|     0 | inactive | !inside, !active | mouse outside MySoSketchKit, no action |
|     1 | sleeping | inside, !active  | mouse inside, no button pressed      |
|     2 | drawing  | inside, active   | mouse inside, button 1 down, drawing |
|     3 | waiting  | !inside, active  | mouse outside, but still drawing     |
+-------+----------+------------------+--------------------------------------+

State sequences and their triggering actions are listed below:
If nothing is set, it means no transition, should not happen.
The sign '+' means grab events, '-' means release.
+-------------------------+----+----+----+----+
| \/ Action   old State > | 0  | 1  | 2  | 3  |
+-------------------------+----+----+----+----+
| mouse move in           | 1+ |    |    | 2  |
| mouse move out          |    | 0- | 3  |    |
| button 1 press          |    | 2  |    |    |
| button 1 release        |    |    | 1  | 0- |
+-------------------------+----+----+----+----+
*/


#ifndef _MySoSketchKit_H_
#define _MySoSketchKit_H_


#include <Inventor/actions/SoAction.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoNode.h>
#include <stbapi/event3d/Base3D.h>


class MySoSketchKit;  // needed for typedef below

typedef void MySoSketchKitCB(   // type for a callback function
    void* userData,           // stores some data for the callback function
    MySoSketchKit* sketchKit);  // the MySoSketchKit that executed the callback


class MySoSketchKit : public SoBaseKit, public Base3D
{
    // import headers:
    SO_KIT_HEADER(MySoSketchKit);
    BASE3D_HEADER;

private:

    // set parts:
    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(surfaceSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(surfaceMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(surface);
    SO_KIT_CATALOG_ENTRY_HEADER(sketchSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(sketchMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(sketchCoordinates);
    SO_KIT_CATALOG_ENTRY_HEADER(sketch);
    SO_KIT_CATALOG_ENTRY_HEADER(cursorSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(cursorSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(cursorTranslation);
    SO_KIT_CATALOG_ENTRY_HEADER(cursor);

    // state data (not in fields):
    MySoSketchKitCB* pressCallback;    // callback functions
    MySoSketchKitCB* moveCallback;
    MySoSketchKitCB* releaseCallback;
    void* pressData;                 // data passed to callback functions
    void* moveData;
    void* releaseData;
    bool active;                     // currently drawing
    SoPath* root;                    // reference to the path of the root node

protected:

    // common initializations for constructors:
    void constMySoSketchKit();  // initialization macros
    void initMySoSketchKit(     // initializes the parts of the MySoSketchKit
        float myXSize,        // set X size
        float myYSize,        // set Y size
        float myHeight);      // set Z size

    // provide an easy extension mechanism:
    virtual void pressImplementation();    // started a sketch
    virtual void moveImplementation();     // continued a sketch
    virtual void releaseImplementation();  // ended a sketch

public:

    // fields:
    SoMFVec3f point;
        // sketch points
    SoMFInt32 numVertices;
        // sketch sequences
    SoSFFloat xSize, ySize, height;
        // size of the MySoSketchKit
    SoSFFloat threshold;
        // detect only movement greater than threshold
    SoSFColor surfaceColor, highlightColor, sketchColor;
        // colors
    SoSFInt32 surfaceVisible, sketchVisible, cursorVisible;
        // switch visibility
    SoSFBool inside;
        // cursor inside MySoSketchKit
    SoSFBool multiple;
        // allow multiple sketches

    // initialization:
    static void initClass();  // static initialization
    MySoSketchKit();            // constructor
    MySoSketchKit(              // parameterized constructor
        float myXSize,        // set X size
        float myYSize,        // set Y size
        float myHeight);      // set Z size
    ~MySoSketchKit();           // destructor

    // methods (interface):
    void clear();                  // clears the MySoSketchKit
    SoMFVec3f intersect(          // TODO: what should that really do?
        SoMFVec3f drawing);       // drawing to be intersected
    SbVec2f posOnSurface();        // current position (local coordinates)
    SbVec2f posOnSurface(          // global to local coordinates
        SbVec3f position);         // point to be translated
    SbVec3f posInWorld();          // current position (global coordinates)
    SbVec3f posInWorld(            // local to global coordinates
        SbVec2f position);         // point to be translated
    bool isInside(                 // is this position inside the MySoSketchKit?
        SbVec2f position);         // local point to be checked
    bool isInside(                 // is this position inside the MySoSketchKit?
        SbVec3f position);         // global point to be checked
        // TODO: check for (local) z-coordinate too
    void setPressCallback(         // sets a callback (start line)
        MySoSketchKitCB* cb,         // the callback
        void* userData);           // data to be passed to the callback
    void setMoveCallback(          // sets a callback (continue line)
        MySoSketchKitCB* cb,         // the callback
        void* userData);           // data to be passed to the callback
    void setReleaseCallback(       // sets a callback (end line)
        MySoSketchKitCB* cb,         // the callback
        void* userData);           // data to be passed to the callback
    void removePressCallback();    // removes a press callback
    void removeMoveCallback();     // removes a move callback
    void removeReleaseCallback();  // removes a release callback

    // methods (event handling):
    void handle3DEvent(               // handles a 3d event
        SoHandle3DEventAction* h3a);  // the action containing the event
    SbBool isInterested(              // returns interest in 3d events
        SoHandle3DEventAction* h3a);  // the action containing the event
    void doAction(                    // does general event action handling
        SoAction* action);            // the event action to be handled

    // methods (internal code reuse):
    SbMatrix getTransformationMatrix();  // returns local-to-global matrix
    void drawPoint(                      // starts or continues a line
    SbVec3f pt,                          // point to be appended
    SbBool newLine);                     // start a new line
};


#endif
