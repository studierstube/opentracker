// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       MySoSketchKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    A sketch pad for the Studierstube system
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    bas		Benjamin A. Schmit
// ===========================================================================
//  HISTORY:	see MySoSketchKit.h
//              2000-10-03: This is a Q&D hack to replace the
//              SoTriangleStripSet with an SoLineSet.
// ===========================================================================
//
// TODO:
// test 3d event handling
// check intersect(), isInside()
// check includes


//#include "GRT_defs.h"
#include "MySoSketchKit.h"

#include <Inventor/SoLists.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>


SO_KIT_SOURCE(MySoSketchKit);
BASE3D_SOURCE(MySoSketchKit);


// static initialization
void MySoSketchKit::initClass()
{
    SO_KIT_INIT_CLASS(MySoSketchKit, SoBaseKit, "BaseKit");
    BASE3D_INIT_CLASS(MySoSketchKit);
}


// initialization macros
void MySoSketchKit::constMySoSketchKit()
{
    // Create this MySoSketchKit:
    SO_KIT_CONSTRUCTOR(MySoSketchKit);
    BASE3D_CONSTRUCTOR;

    // Add the catalog parts:
    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE,
        this, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(surfaceSwitch, SoSwitch, FALSE,
        topSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(surfaceMaterial, SoMaterial, FALSE,
        surfaceSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(surface, SoCube, FALSE,
        surfaceSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(sketchSwitch, SoSwitch, FALSE,
        topSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(sketchMaterial, SoMaterial, FALSE,
        sketchSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(sketchCoordinates, SoCoordinate3, FALSE,
        sketchSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(sketch, SoLineSet, FALSE,
        sketchSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(cursorSwitch, SoSwitch, FALSE,
        topSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(cursorSeparator, SoSeparator, FALSE,
        cursorSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(cursorTranslation, SoTranslation, FALSE,
        cursorSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(cursor, SoNode, FALSE,
        cursorSeparator, \x0, TRUE);

    // Add the nodekit's fields:
    SO_NODE_ADD_FIELD(point, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(numVertices, (0));
    SO_NODE_ADD_FIELD(xSize, (0.2f));
    SO_NODE_ADD_FIELD(ySize, (0.15f));
    SO_NODE_ADD_FIELD(height, (0.01f));
    SO_NODE_ADD_FIELD(threshold, (0.01f));
    SO_NODE_ADD_FIELD(surfaceColor, (0.75f, 0.75f, 0.0f));
    SO_NODE_ADD_FIELD(highlightColor, (1.0f, 1.0f, 0.0f));
    SO_NODE_ADD_FIELD(sketchColor, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(surfaceVisible, (SO_SWITCH_ALL));
    SO_NODE_ADD_FIELD(sketchVisible, (SO_SWITCH_ALL));
    SO_NODE_ADD_FIELD(cursorVisible, (SO_SWITCH_NONE));
    SO_NODE_ADD_FIELD(inside, (FALSE));
    SO_NODE_ADD_FIELD(multiple, (TRUE));

    // Initialize this MySoSketchKit:
    SO_KIT_INIT_INSTANCE();
}


// initializes the parts of the MySoSketchKit
void MySoSketchKit::initMySoSketchKit(
    float myXSize,   // set X size
    float myYSize,   // set Y size
    float myHeight)  // set Z size
{
    // surfaceSwitch:
    {
        SoSwitch* sw= (SoSwitch*) getAnyPart("surfaceSwitch", true);
        sw->whichChild.connectFrom(&surfaceVisible);
    }

    // surfaceMaterial:
    {
        SoMaterial* mat= (SoMaterial*) getAnyPart("surfaceMaterial", true);
        mat->diffuseColor.connectFrom(&surfaceColor);
    }

    // surface:
    {
        SoCube* cub= new SoCube();
        setAnyPart("surface", cub);
        xSize.connectFrom(&(cub->width));
        ySize.connectFrom(&(cub->depth));
        height.connectFrom(&(cub->height));
        cub->width= myXSize;
        cub->depth= myYSize;
        cub->height= myHeight;
        cub->width.connectFrom(&(xSize));
        cub->depth.connectFrom(&(ySize));
        cub->height.connectFrom(&(height));
    }

    // sketchSwitch:
    {
        SoSwitch* sw= (SoSwitch*) getAnyPart("sketchSwitch", true);
        sw->whichChild.connectFrom(&sketchVisible);
    }

    // sketchMaterial:
    {
        SoMaterial* mat= (SoMaterial*) getAnyPart("sketchMaterial", true);
        mat->diffuseColor.connectFrom(&sketchColor);
    }

    // sketchCoordinates:
    {
        SoCoordinate3* coo=
            (SoCoordinate3*) getAnyPart("sketchCoordinates", true);
        coo->point.connectFrom(&point);
        point.connectFrom(&(coo->point));
    }

    // sketch:
    {
        SoLineSet* lin=
            (SoLineSet*) getAnyPart("sketch", true);
        lin->numVertices.connectFrom(&numVertices);
        numVertices.connectFrom(&(lin->numVertices));
        clear();
    }

    // cursorSwitch:
    {
        SoSwitch* sw= (SoSwitch*) getAnyPart("cursorSwitch", true);
        sw->whichChild.connectFrom(&cursorVisible);
    }

    // cursor:
    {
        SoSeparator* cur= (SoSeparator*) getAnyPart("cursor", true);
        // TODO: place instructions for default cursor here!
    }

    // additional state information:
    pressCallback= NULL;
    moveCallback= NULL;
    releaseCallback= NULL;
    pressData= NULL;
    moveData= NULL;
    releaseData= NULL;
    active= false;
    root= NULL;
}


// constructor
MySoSketchKit::MySoSketchKit()
{
    constMySoSketchKit();
    initMySoSketchKit(0.2f, 0.15f, 0.01f);  // set default values
}


// parameterized constructor
MySoSketchKit::MySoSketchKit(
    float myXSize,  // set X size
    float myYSize,  // set Y size
    float height)   // set Z size
{
    constMySoSketchKit();
    initMySoSketchKit(myXSize, myYSize, height);
}


// destructor
MySoSketchKit::~MySoSketchKit() {}


// clears the MySoSketchKit
void MySoSketchKit::clear()
{
    numVertices.deleteValues(0);
    point.deleteValues(0);
}


// TODO: what should that really do?
SoMFVec3f MySoSketchKit::intersect(
    SoMFVec3f drawing)  // drawing to be intersected
{
    // yet untested, may also need to be changed!
    SoMFVec3f result= SoMFVec3f();
    SbVec2f res2;
    SbVec3f res3;
    res3[1]= height.getValue()/2;
    for (int i= 0; i < drawing.getNum(); i++)
    {
        SbVec2f res2= posOnSurface(drawing[i]);
        res3[0]= res2[0];
        res3[2]= res2[1];
        result.set1Value(i, res3);
    }
    return result;
}


// current position (global coordinates)
SbVec3f MySoSketchKit::posInWorld()
{
    return posInWorld(posOnSurface());
}


// local to global coordinates
SbVec3f MySoSketchKit::posInWorld(
    SbVec2f position)  // point to be translated
{
    SbVec3f pos3, result;
    pos3[0]= position[0];
    pos3[1]= height.getValue()/2;
    pos3[2]= position[1];
    SbMatrix mat= getTransformationMatrix();
    mat.multVecMatrix(pos3, result);
    return result;
}


// current position (local coordinates)
SbVec2f MySoSketchKit::posOnSurface()
{
    SbVec2f res;
    SbVec3f trn= ((SoTranslation*) getAnyPart("cursorTranslation", true))
        ->translation.getValue();
        // this is the last known position of the cursor
    res[0]= trn[0];
    res[1]= trn[2];
    return res;
}


// global to local coordinates
SbVec2f MySoSketchKit::posOnSurface(
    SbVec3f position)  // point to be translated
{
    SbVec3f result;
    SbMatrix mat= getTransformationMatrix();
    mat= mat.inverse();
    mat.multVecMatrix(position, result);
    SbVec2f res2;
    res2[0]= result[0];
    res2[1]= result[2];
    return res2;
}


// is this position inside the MySoSketchKit?
bool MySoSketchKit::isInside(
    SbVec2f position)  // local point to be checked
{
    if ((position[0] >= -xSize.getValue()/2)
        && (position[0] <= xSize.getValue()/2)
        && (position[1] >= -ySize.getValue()/2)
        && (position[1] <= ySize.getValue()/2)) return true;
    else return false;
}


// is this position inside the MySoSketchKit?
bool MySoSketchKit::isInside(
    SbVec3f position)  // global point to be checked
{
    // TODO: check for the local z-coordinate as well!
    return isInside(posOnSurface(position));
}


// sets a callback (start line)
void MySoSketchKit::setPressCallback(
    MySoSketchKitCB* cb,     // the callback
    void* userData= NULL)  // data to be passed to the callback
{
    pressCallback= cb;
    pressData= userData;
}


// sets a callback (continue line)
void MySoSketchKit::setMoveCallback(
    MySoSketchKitCB* cb,     // the callback
    void* userData= NULL)  // data to be passed to the callback
{
  moveCallback= cb;
  moveData= userData;
}


// sets a callback (end line)
void MySoSketchKit::setReleaseCallback(
    MySoSketchKitCB* cb,     // the callback
    void* userData= NULL)  // data to be passed to the callback
{
    releaseCallback= cb;
    releaseData= userData;
}


// provide an easy extension mechanism:
void MySoSketchKit::pressImplementation() {}   // stated a sketch
void MySoSketchKit::moveImplementation() {}    // continued a sketch
void MySoSketchKit::releaseImplementation() {} // ended a sketch


// removes a press callback
void MySoSketchKit::removePressCallback()
{
    pressCallback= NULL;
    pressData= NULL;
}


// removes a move callback
void MySoSketchKit::removeMoveCallback()
{
    moveCallback= NULL;
    moveData= NULL;
}


// removes a release callback
void MySoSketchKit::removeReleaseCallback()
{
    releaseCallback= NULL;
    releaseData= NULL;
}


// handles a 3d event
void MySoSketchKit::handle3DEvent(
    SoHandle3DEventAction* h3a)  // the action containing the event
{
    doAction(h3a);
}


// does general event action handling
void MySoSketchKit::doAction(
    SoAction* action)  // the event action to be handled
{
    ///////////////////////////
    // define some variables //
    ///////////////////////////

    enum { INACTIVE=0, SLEEPING, DRAWING, WAITING } state= INACTIVE;
        // current state
    enum { NO_ACTION=0, MOVE_IN, MOVE_OUT, PRESS, RELEASE } trans= NO_ACTION;
        // transitional action
    SbVec2f pos;  // current cursor position (on surface)
    bool move= false;  // check for movement (movement event occured)

    /////////////////////////
    // evaluate transition //
    /////////////////////////

    // check for So3DEvent (3d):
    if (action->isOfType(SoHandle3DEventAction::getClassTypeId()))
    {
        So3DEvent* event= (So3DEvent*) ((SoHandle3DEventAction*) action)
            ->getEvent();

        // button event:
        if (event->getType() == So3DEvent::ET_BUTTON0_EVENT)
        {
            if (event->getButton(So3DEvent::BUTTON0)) trans= PRESS;
            else trans= RELEASE;
        }

        // move event:
        if (event->getType() == So3DEvent::ET_MOVE_EVENT)
        {
            pos= posOnSurface(event->getTranslation());
            if (inside.getValue() && !isInside(pos)) trans= MOVE_OUT;
            if (!inside.getValue() && isInside(pos)) trans= MOVE_IN;
            if (isInside(pos)) move= true;
        }
    }

    ///////////////////////
    // move if necessary //
    ///////////////////////

    if (move) {

        // initialize necessary variables:
        float pos3[3]= { pos[0], height.getValue()/2, pos[1] };
        SoTranslation* ctr= (SoTranslation*) getAnyPart("cursorTranslation",
            true);

        // should movement be detected (compare to threshold)?
        SbVec3f trn= ctr->translation.getValue();
        if ((pow(pos3[0] - trn[0], 2) + pow(pos3[2] - trn[2], 2)) >=
            pow(threshold.getValue(), 2))
        {
            // yes, set current coordinates:
            ctr->translation.setValue(pos3);

            if (active)  // should we draw?
            {
                // yes, draw:
                drawPoint(pos3, false);
            }

            // execute callbacks:
            moveImplementation();  // internal extension mechanism
            if (moveCallback != NULL) moveCallback(moveData, this);
        }
    }

    ///////////////////////
    // state transitions //
    ///////////////////////

    if (trans != NO_ACTION)
    {
        // find out current state:
        // initial value of state: INACTIVE.
        if (inside.getValue()) state= SLEEPING;
        if (active) state= WAITING;
        if (inside.getValue() && active) state= DRAWING;

        // find new state according to table in MySoSketchKit.h:
        switch (trans)
        {
        case NO_ACTION:
            break;
        case MOVE_IN:
            inside= true;  // state transitions
            ((SoMaterial*) getAnyPart("surfaceMaterial", true))->diffuseColor
                .connectFrom(&highlightColor);
            if (state == INACTIVE)
            {
                ((SoHandle3DEventAction*) action)->setGrabber(this);
                ((SoHandle3DEventAction*) action)->setHandled();
            }
            break;
        case MOVE_OUT:
            inside= false;  // state transitions
            ((SoMaterial*) getAnyPart("surfaceMaterial", true))->diffuseColor
                .connectFrom(&surfaceColor);
            if (state == SLEEPING)
                ((SoHandle3DEventAction*) action)->releaseGrabber();
            // other nodes might still want to handle this event, do not call
            // setHandled()
            break;
        case PRESS:
            if (state == SLEEPING) {
                active= true;  // state transition
                if (!multiple.getValue()) clear();
                    // single drawing -> clear existing ones
                SbVec3f pos3= ((SoTranslation*) getAnyPart(
                    "cursorTranslation", true))->translation.getValue();
                pos3[1]= height.getValue()/2;
                drawPoint(pos3, true);
                // execute callback functions:
                pressImplementation();  // internal extension mechanism
                if (pressCallback != NULL) pressCallback(pressData, this);
                ((SoHandle3DEventAction*) action)->setHandled();
            }
            break;
        case RELEASE:
            active= false;  // state transitions
            if (state == DRAWING) {
                // execute callback functions:
                releaseImplementation();  // internal extension mechanism
                if (releaseCallback != NULL) releaseCallback(releaseData,
                    this);
                ((SoHandle3DEventAction*) action)->setHandled();
            } else if (state == WAITING) {
                // execute callback functions:
                releaseImplementation();  // internal extension mechanism
                if (releaseCallback != NULL) releaseCallback(releaseData,
                    this);
                ((SoHandle3DEventAction*) action)->releaseGrabber();
                ((SoHandle3DEventAction*) action)->setHandled();
            }
            break;
        }
    }
}


// returns interest in 3d events
SbBool MySoSketchKit::isInterested(
    SoHandle3DEventAction* h3a)  // the action containing the event
{
    // capture the root path
    root= (SoPath*) ((SoHandle3DEventAction*) h3a)->getPathFromRoot();
        // it isn't sufficient to do this only once, so do it continuously

    // show interest:
    return isInside(h3a->getEvent()->getTranslation());
}


// returns local-to-global matrix
SbMatrix MySoSketchKit::getTransformationMatrix()
{
    // WARNING: this only works if root has been set
    // (by an SoHandle3DEventAction)
    if (root != NULL)
    {
        SoGetMatrixAction* gma= new SoGetMatrixAction(SbViewportRegion());
            // assumes that the SbViewportRegion does not matter here
        gma->apply(root);
        return gma->getMatrix();
    }
    else return SbMatrix::identity();  // fallback
}


// starts or continues a line
void MySoSketchKit::drawPoint(
    SbVec3f pt,      // point to be appended
    SbBool newLine)  // start a new line
{
    /* // This is the old version with the SoTriangleStripSet:
    SbVec3f pt2= pt;
    pt2[1]= pt[1] + height.getValue()/2;
    point.set1Value(point.getNum(), pt);
    point.set1Value(point.getNum(), pt2);
    if (!newLine)
    {
        if (numVertices.getNum() <= 0) numVertices.setValue(2);  // first point
        else numVertices.set1Value(numVertices.getNum() - 1,
            *(numVertices.getValues(numVertices.getNum() - 1)) + 2);
    }
    else numVertices.set1Value(numVertices.getNum(), 2);
    */

    // This is the new version with the SoLineSet:
    point.set1Value(point.getNum(), pt);
    if (!newLine)
    {
        if (numVertices.getNum() <= 0) numVertices.setValue(1);  // first point
        else numVertices.set1Value(numVertices.getNum() - 1,
            *(numVertices.getValues(numVertices.getNum() - 1)) + 1);
    }
    else numVertices.set1Value(numVertices.getNum(), 1);
}
