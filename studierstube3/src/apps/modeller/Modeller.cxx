// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Modeller.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Demonstrator application for the Gesture Recognition Toolkit
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    bas             Benjamin A. Schmit
// ===========================================================================
//  HISTORY:    see Modeller.h
// ===========================================================================

#include "Modeller.h"

#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFontStyle.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTranslation.h>
#include <stbapi/misc/GRT_defs.h>
#include <stbapi/util/ivio.h>
#include <stbapi/workspace/SoContextKitSub.h>

#ifdef GRT_FENCE
#include <Inventor/nodes/SoTriangleStripSet.h>
#else
#include <Inventor/nodes/SoLineSet.h>
#endif


SO_KIT_SOURCE(Modeller);
CONTEXT_APPLICATION(Modeller);


void Modeller::initClass()
{
    //debug("Initializing Modeller Class.");
    SO_KIT_INIT_CLASS(Modeller, SoContextKit, "SoContextKit");
}


Modeller::Modeller()
{
    //debug("Instantiating Modeller.");
    SO_KIT_CONSTRUCTOR(Modeller);
    SO_KIT_INIT_INSTANCE();
    state= 0;  // will be overwritten by createPipSheet()
    imgr.libNew();
    model= 0;
    activeNode= NULL;
    activeColor= NULL;
    master= false;  // no callbacks set yet
}


SoContextKit* Modeller::factory()
{
    //debug("Factoring Modeller.");
    return new Modeller();
}


Modeller::~Modeller()
{
    //debug("Destroying Modeller.");
    destructor();
}


SoNode* Modeller::createPipSheet(int uid)
{
    // get the sheet:
    pip= readFile(_ModellerPip, comm->workingDirectory);
    pip->ref();

    // let all nodes be found:
    SoSwitch* rootSw= (SoSwitch*) findNode(pip, "RootSw");
    int swc= rootSw->whichChild.getValue();
    rootSw->whichChild.setValue(SO_SWITCH_ALL);

    // initialize sheet components:
    SoGestureKit* gesture= (SoGestureKit*) findNode(pip, "Gesture");
    SbString str= comm->workingDirectory;
    str+= "/";
    str+= _ModellerGestures;
    //debug("Loading gesture file", str.getString());
    gesture->setGestures(str);

    // display initial sheet:
    rootSw->whichChild.setValue(swc);

    // set initial state:
    state= 10*swc;  // get state from file

    // finish:
    pip->unrefNoDelete();
    return pip;
}


SoWindowKit* Modeller::createWindowGeometry(int index)
{
    WindowCreate wc;
    SoWindowKit::defaultWindow(wc);
    SoWindowKit* windowKit= comm->createWindow(NULL, &wc, NULL, NULL);
    windowKit->size= _ModellerWindowSize;
    SoSeparator* wk= windowKit->getClientVolumeSeparator();
    SoDrawStyle* style= new SoDrawStyle();
    wk->addChild(style);
    style->lineWidth.setValue(3);  // this sets default drawing line width
    window= new SoSeparator();
    wk->addChild(window);
    window->setName(_ModellerString);
    return windowKit;
}


void Modeller::setSheetMasterMode(SoNode* pipSheet, SbBool masterMode)
{
    // view all elements so the searches will be successful:
    SoSwitch* sw= (SoSwitch*) findNode(pipSheet, "RootSw");
    int swc= sw->whichChild.getValue();
    sw->whichChild.setValue(SO_SWITCH_ALL);

    // find elements for callbacks:
    int i;
    So3DButton* ok[3];
    ok[0]= (So3DButton*) findNode(pipSheet, "Ok0");
    ok[1]= (So3DButton*) findNode(pipSheet, "Ok1");
    ok[2]= (So3DButton*) findNode(pipSheet, "Ok2");
    So3DButton* cl[3];
    cl[0]= (So3DButton*) findNode(pipSheet, "Cl0");
    cl[1]= (So3DButton*) findNode(pipSheet, "Cl1");
    cl[2]= (So3DButton*) findNode(pipSheet, "Cl2");
    So3DSlider** par= getParameterSliders();
    So3DRadioButton** col= getColorButtons();
    So3DRadioButton** file= getFileButtons();
    So3DRadioButton* buttDraw= (So3DRadioButton*) findNode(pipSheet,
        "ButtDraw");
    So3DRadioButton* buttModel= (So3DRadioButton*) findNode(pipSheet,
        "ButtModel");
    So3DRadioButton* buttText= (So3DRadioButton*) findNode(pipSheet,
        "ButtText");
    So3DRadioButton* buttFile= (So3DRadioButton*) findNode(pipSheet,
        "ButtFile");
    SoSketchKit* sketch= (SoSketchKit*) findNode(pipSheet, "Sketch");
    SoGestureKit* gesture= (SoGestureKit*) findNode(pipSheet, "Gesture");
    SoAlphabetKit* alpha= (SoAlphabetKit*) findNode(pipSheet, "Alpha");
    So3DButton* buttNew= (So3DButton*) findNode(pipSheet, "ButtNew");
    So3DButton* buttLoad= (So3DButton*) findNode(pipSheet, "ButtLoad");
    So3DButton* buttSave= (So3DButton*) findNode(pipSheet, "ButtSave");

    if (masterMode)
    {
        // set callbacks for PIP elements:
        master= true;
        buttDraw->addReleaseCallback(evMenuDraw, this);
        buttModel->addReleaseCallback(evMenuModel, this);
        buttText->addReleaseCallback(evMenuText, this);
        buttFile->addReleaseCallback(evMenuFile, this);
        sketch->setReleaseCallback(evSketchEnter, this);
        alpha->setAlphabetCallback(evTextEnter, this);
        alpha->setGestureCallback(evTextChange, this);
        gesture->setGestureCallback(evModelEnter, this);
        buttNew->addReleaseCallback(evFileNew, this);
        buttLoad->addReleaseCallback(evFileLoad, this);
        buttSave->addReleaseCallback(evFileSave, this);
        ok[0]->addReleaseCallback(evSketchUse, this);
        ok[1]->addReleaseCallback(evModelNew, this);
        ok[2]->addReleaseCallback(evTextUse, this);
        cl[0]->addReleaseCallback(evSketchClear, this);
        cl[1]->addReleaseCallback(evModelClear, this);
        cl[2]->addReleaseCallback(evTextClear, this);
        for (i= 0; i < 2; ++i)
            par[i]->addPositionChangeCallback(evModelSlider, this);
        for (i= 0; i < 8; ++i)
            col[i]->addReleaseCallback(evColorButton, this);
        for (i= 0; i < 6; ++i)
            file[i]->addReleaseCallback(evFileButton, this);

        // set callbacks for window content:
        SoDragKit* drag;
        SbString name;
        for (i= 0; i < window->getNumChildren(); i++)
        {
            drag= (SoDragKit*) window->getChild(i);
            name= SbString(drag->getName().getString());
            if (name.getSubString(0, 3) == "DRAW")
                drag->setPressCallback(evSelectSketch, this);
            else if (name.getSubString(0, 3) == "NODE")
                drag->setPressCallback(evSelectModel, this);
            else if (name.getSubString(0, 3) == "TEXT")
                drag->setPressCallback(evSelectText, this);
            drag->setMoveCallback(evMoveObject, this);
            drag->setReleaseCallback(evMoveObject, this);
        }
    }
    else
    {
        // release callbacks for PIP elements:
        master= false;
        buttDraw->removeReleaseCallback(evMenuDraw);
        buttModel->removeReleaseCallback(evMenuModel);
        buttText->removeReleaseCallback(evMenuText);
        buttFile->removeReleaseCallback(evMenuFile);
        sketch->removeReleaseCallback();
        gesture->removeGestureCallback();
        alpha->removeAlphabetCallback();
        alpha->removeGestureCallback();
        buttNew->removeReleaseCallback(evFileNew);
        buttLoad->removeReleaseCallback(evFileLoad);
        buttSave->removeReleaseCallback(evFileSave);
        ok[0]->removeReleaseCallback(evSketchUse);
        ok[1]->removeReleaseCallback(evModelNew);
        ok[2]->removeReleaseCallback(evTextUse);
        cl[0]->removeReleaseCallback(evSketchClear);
        cl[1]->removeReleaseCallback(evModelClear);
        cl[2]->removeReleaseCallback(evTextClear);
        for (i= 0; i < 2; ++i)
            par[i]->removePositionChangeCallback(evModelSlider);
        for (i= 0; i < 8; ++i)
            col[i]->removeReleaseCallback(evColorButton);
        for (i= 0; i < 6; ++i)
            file[i]->removeReleaseCallback(evFileButton);

        // release callbacks for window content:
        SoDragKit* drag;
        SbString name;
        for (i= 0; i < window->getNumChildren(); i++)
        {
            drag= (SoDragKit*) window->getChild(i);
            name= SbString(drag->getName().getString());
            if (name.getSubString(0, 3) == "DRAW")
                drag->removePressCallback(evSelectSketch, this);
            else if (name.getSubString(0, 3) == "NODE")
                drag->removePressCallback(evSelectModel, this);
            else if (name.getSubString(0, 3) == "TEXT")
                drag->removePressCallback(evSelectText, this);
        }
//        for (i= 0; i < window->getNumChildren(); i++)
//            ((SoDragKit*) window->getChild(i))->
//            removePressCallback();
    }

    // clean up:
    delete par;
    delete col;
    delete file;

    // restore active sheet:
    sw->whichChild.setValue(swc);
}


// get PIP sheet parameter sliders:
So3DSlider** Modeller::getParameterSliders()
{
    So3DSlider** par= new So3DSlider*[2];
    par[0]= (So3DSlider*) findNode(pip, "Par1");
    par[1]= (So3DSlider*) findNode(pip, "Par2");
    return par;
}


// get PIP sheet color buttons:
So3DRadioButton** Modeller::getColorButtons()
{
    So3DRadioButton** col= new So3DRadioButton*[8];
    col[0]= (So3DRadioButton*) findNode(pip, "Col0");
    col[1]= (So3DRadioButton*) findNode(pip, "Col1");
    col[2]= (So3DRadioButton*) findNode(pip, "Col2");
    col[3]= (So3DRadioButton*) findNode(pip, "Col3");
    col[4]= (So3DRadioButton*) findNode(pip, "Col4");
    col[5]= (So3DRadioButton*) findNode(pip, "Col5");
    col[6]= (So3DRadioButton*) findNode(pip, "Col6");
    col[7]= (So3DRadioButton*) findNode(pip, "Col7");
    return col;
}


// get PIP sheet file buttons:
So3DRadioButton** Modeller::getFileButtons()
{
    So3DRadioButton** file= new So3DRadioButton*[6];
    file[0]= (So3DRadioButton*) findNode(pip, "File1");
    file[1]= (So3DRadioButton*) findNode(pip, "File2");
    file[2]= (So3DRadioButton*) findNode(pip, "File3");
    file[3]= (So3DRadioButton*) findNode(pip, "File4");
    file[4]= (So3DRadioButton*) findNode(pip, "File5");
    file[5]= (So3DRadioButton*) findNode(pip, "File6");
    return file;
}


// event 01 - use sketch
void Modeller::evSketchUse(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Use Sketch.");

    SoDragKit* drag;
    SoSeparator* content;
    SoSketchKit* sketchKit= (SoSketchKit*) findNode(that->pip, "Sketch");
    So3DRadioButton** col= that->getColorButtons();

    // create or find sketch object:
    if (that->state == 0)
    {
        that->state= 1;
        drag= new SoDragKit();
        that->window->addChild(drag);
        drag->setName("DRAW");
        if (that->master)
        {
            drag->setPressCallback(evSelectSketch, that);
            drag->setMoveCallback(evMoveObject, that);
            drag->setReleaseCallback(evMoveObject, that);
        }
        content= (SoSeparator*) drag->getPart("content", TRUE);
        that->activeNode= content;
    }
    else if (that->state == 1)
    {
        that->state= 0;
        that->activeColor= NULL;
        content= (SoSeparator*) that->activeNode;
        content->removeAllChildren();
            // first delete, then re-create.
    }

    // set sketch properties:
    that->activeColor= new SoMaterial();
    content->addChild(that->activeColor);
    for (int i= 0; i < 8; ++i)
        if (col[i]->pressed.getValue()) that->activeColor->diffuseColor.
            setValue(col[i]->buttonColor.getValue());
    SoScale* sketchScale= new SoScale();
    content->addChild(sketchScale);
    sketchScale->scaleFactor.setValue(0.02, 0.02, 0.02);
    SoCoordinate3* sketchCoordinate= new SoCoordinate3();
    content->addChild(sketchCoordinate);
    sketchCoordinate->point= sketchKit->point;
#ifdef GRT_FENCE  // depends on the layout of the SoSketchKit...
    SoTriangleStripSet* sketchSet= new SoTriangleStripSet();
#else
    SoLineSet* sketchSet= new SoLineSet();
#endif
    content->addChild(sketchSet);
    sketchSet->numVertices= sketchKit->numVertices;
    SoTranslation* sketchTrn= new SoTranslation();
    content->addChild(sketchTrn);
#ifdef GRT_FENCE  // again...
    sketchTrn->translation.setValue(0.0f, 0.75f, 0.0f);
#else
    sketchTrn->translation.setValue(0.0f, 0.5f, 0.0f);
#endif
    SoMaterial* mat= new SoMaterial();
    content->addChild(mat);
    mat->diffuseColor.setValue(_grt_surface);
    mat->transparency.setValue(_ModellerCarrierTransparency);
    SoCube* cube= new SoCube();
    content->addChild(cube);
    cube->width.setValue(sketchKit->xSize.getValue());
    cube->height.setValue(sketchKit->height.getValue());
    cube->depth.setValue(sketchKit->ySize.getValue());
    if (that->state == 0) sketchKit->clear();

    // clean up:
    delete col;
}


// event 02 - clear sketch
void Modeller::evSketchClear(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Sketch cleared.");

    if (that->state == 0)  // no object selected:
    {
        SoSketchKit* sketch= (SoSketchKit*) findNode(that->pip, "Sketch");
        sketch->clear();
    }
    else if (that->state == 1)  // object selected, delete it:
    {
        // the following complex for/if statement looks through all our objects
        // and compares the pointer to an object with the pointer to activeNode.
        for (int i= 0; i < that->window->getNumChildren(); i++)
            if (((SoSeparator*) ((SoDragKit*) that->window->getChild(i))->
                getPart("content", TRUE)) == that->activeNode)
                that->window->removeChild(i);
        that->state= 0;
    }
}


// event 03 - enter sketch
void Modeller::evSketchEnter(
    void* userData,          // receiver
    SoSketchKit* sketchKit)  // sender
{
    // Modeller* that= (Modeller*) userData;  // uncomment if needed
    //debug("Sketch entered.");

    // currently unused.
}


// event 11 - new model
void Modeller::evModelNew(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("New model button pressed.");

    // unselect selected object so that a new object can be created by drawing
    // a model gesture:
    if (that->state == 11)
    {
        that->state= 10;
        ((SoSeparator*) findNode(that->pip, "ModelCheat"))->
            removeAllChildren();
    }
}


// event 12 - clear model
void Modeller::evModelClear(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Clear model button pressed.");

    if (that->state == 11)
    {
        // see event 02 on what this does...
        for (int i= 0; i < that->window->getNumChildren(); i++)
            if (((SoSeparator*) ((SoDragKit*) that->window->getChild(i))->
                getPart("content", TRUE))->getChild(1) == that->activeNode)
                that->window->removeChild(i);
        that->state= 10;
        ((SoSeparator*) findNode(that->pip, "ModelCheat"))->
            removeAllChildren();
    }
}


// event 13 - enter model
void Modeller::evModelEnter(
    void* userData,            // receiver
    int gesture,               // id of the model entered
    SoGestureKit* gestureKit)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Model entered.");

    SoDragKit* drag= NULL;
    SoSeparator* content= NULL;

    So3DSlider** sl= that->getParameterSliders();
    So3DRadioButton** col= that->getColorButtons();
    SoCube* modelCube;
    SoCylinder* modelCylinder;
    SoCone* modelCone;
    SoSphere* modelSphere;

    SoSeparator* modelCheat= (SoSeparator*) findNode(that->pip, "ModelCheat");
    modelCheat->removeAllChildren();

    if (that->state == 10)
    {
        that->state= 11;
        drag= new SoDragKit();
        that->window->addChild(drag);
        if (that->master)
        {
            drag->setPressCallback(evSelectModel, that);
            drag->setMoveCallback(evMoveObject, that);
            drag->setReleaseCallback(evMoveObject, that);
        }
        that->activeColor= new SoMaterial();
        content= (SoSeparator*) drag->getPart("content", TRUE);
        content->addChild(that->activeColor);
        int j= 0;
        for (int i= 0; i < 8; ++i)
            if (col[i]->pressed.getValue()) j= i;
            that->activeColor->diffuseColor.setValue(
                col[j]->buttonColor.getValue());
    }
    else
    {
        // see event 12 (clear model) on what this does...
        for (int i= 0; i < that->window->getNumChildren(); i++)
            if (((SoSeparator*) ((SoDragKit*) that->window->getChild(i))->
                getPart("content", TRUE))->getChild(1) ==
                that->activeNode)
                drag= (SoDragKit*) that->window->getChild(i);
        content= (SoSeparator*) drag->getPart("content",
            TRUE);
        that->activeColor= (SoMaterial*) content->getChild(0);
        content->removeChild(1);
            // the node is first removed, then re-created.
    }

    that->model= gesture;
    SbString str= "NODE";
    str+= gesture;
    drag->setName(str);

    switch(gesture)
    {
    case 1001:  // cube
        modelCheat->addChild(new SoCube());
        modelCube= new SoCube();
        modelCube->width.setValue(0.2*sl[0]->currentValue.getValue());
        modelCube->height.setValue(0.2*sl[1]->currentValue.getValue());
        modelCube->depth.setValue(0.1);
        content->addChild(modelCube);
        that->activeNode= modelCube;
        break;
    case 1002:  // cylinder
        modelCheat->addChild(new SoCylinder());
        modelCylinder= new SoCylinder();
        modelCylinder->height.setValue(
            0.2*sl[0]->currentValue.getValue());
        modelCylinder->radius.setValue(
            0.1*sl[1]->currentValue.getValue());
        content->addChild(modelCylinder);
        that->activeNode= modelCylinder;
        break;
    case 1003:  // cone
        modelCheat->addChild(new SoCone());
        modelCone= new SoCone();
        modelCone->height.setValue(0.2*sl[0]->currentValue.getValue());
        modelCone->bottomRadius.setValue(
            0.1*sl[1]->currentValue.getValue());
        content->addChild(modelCone);
        that->activeNode= modelCone;
        break;
    case 1004:  // sphere
        modelCheat->addChild(new SoSphere());
        modelSphere= new SoSphere();
        modelSphere->radius.setValue(0.1*sl[0]->currentValue.getValue());
        content->addChild(modelSphere);
        that->activeNode= modelSphere;
        break;
    }

    // clean up:
    delete sl;
    delete col;
}


// event 14 - adjust sliders
void Modeller::evModelSlider(
    void* userData,      // receiver
    So3DSlider* slider)  // sender
{
    Modeller* that= (Modeller*) userData;

    if (that->state == 11)
    {
        So3DSlider** sl= that->getParameterSliders();
        SoCube* modelCube;
        SoCylinder* modelCylinder;
        SoCone* modelCone;
        SoSphere* modelSphere;

        // rasterize sliders:
        sl[0]->currentValue.setValue(
            ((int) (16*sl[0]->currentValue.getValue()))/16.0f);
        sl[1]->currentValue.setValue(
            ((int) (16*sl[1]->currentValue.getValue()))/16.0f);

        // sliders must not be 0:
        // (else, the result would look ugly and could not be grabbed again)
        if (sl[0]->currentValue.getValue() < 0.0625)
            sl[0]->currentValue.setValue(0.0625);
        if (sl[1]->currentValue.getValue() < 0.0625)
            sl[1]->currentValue.setValue(0.0625);

        // adjust it:
        switch(that->model)
        {
        case 1001:  // cube
            modelCube= (SoCube*) that->activeNode;
            modelCube->width.setValue(0.2*sl[0]->currentValue.getValue());
            modelCube->height.setValue(0.2*sl[1]->currentValue.getValue());
            break;
        case 1002:  // cylinder
            modelCylinder= (SoCylinder*) that->activeNode;
            modelCylinder->height.setValue(
                0.2*sl[0]->currentValue.getValue());
            modelCylinder->radius.setValue(
                0.1*sl[1]->currentValue.getValue());
            break;
        case 1003:  // cone
            modelCone= (SoCone*) that->activeNode;
            modelCone->height.setValue(0.2*sl[0]->currentValue.getValue());
            modelCone->bottomRadius.setValue(
                0.1*sl[1]->currentValue.getValue());
            break;
        case 1004:  // sphere
            modelSphere= (SoSphere*) that->activeNode;
            modelSphere->radius.setValue(0.1*sl[0]->currentValue.getValue());
            // 2nd slider has no function with SoSphere
            break;
        }

        // clean up:
        delete sl;
    }
}


// event 21 - use text
void Modeller::evTextUse(
    void* userData,      // reveiver
    So3DButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Use text.");

    SoDragKit* drag;
    SoSeparator* content;
    SoAlphabetKit* alpha= (SoAlphabetKit*) findNode(that->pip, "Alpha");
    So3DRadioButton** col= that->getColorButtons();

    if (that->state == 20)
    {
        that->state= 21;
        drag= new SoDragKit();
        that->window->addChild(drag);
        drag->setName("TEXT");
        if (that->master)
        {
            drag->setPressCallback(evSelectText, that);
            drag->setMoveCallback(evMoveObject, that);
            drag->setReleaseCallback(evMoveObject, that);
        }
        content= (SoSeparator*) drag->getPart("content", TRUE);
        that->activeNode= content;
    }
    else if (that->state == 21)
    {
        that->state= 20;
        that->activeColor= NULL;
        content= (SoSeparator*) that->activeNode;
        content->removeAllChildren();
    }

    that->activeColor= new SoMaterial();
    content->addChild(that->activeColor);
    for (int i= 0; i < 8; i++)
        if (col[i]->pressed.getValue()) that->activeColor->diffuseColor.
            setValue(col[i]->buttonColor.getValue());
    SoFontStyle* style= new SoFontStyle();
    content->addChild(style);
    style->size.setValue(0.04);
    SoText3* text= new SoText3();
    content->addChild(text);
    text->string.setValue(alpha->text.getValue());
    text->justification= SoText3::CENTER;
    SoTranslation* sketchTrn= new SoTranslation();
    content->addChild(sketchTrn);
    sketchTrn->translation.setValue(0.0f, 0.01f, 0.0f);
    SoMaterial* mat= new SoMaterial();
    content->addChild(mat);
    mat->diffuseColor.setValue(_grt_surface);
    mat->transparency.setValue(_ModellerCarrierTransparency);
    SoCube* cube= new SoCube();
    content->addChild(cube);
    cube->width.setValue(0.24f);
    cube->height.setValue(0.08f);
    cube->depth.setValue(0.02f);
    if (that->state == 20) alpha->text.setValue("");

    // clean up:
    delete col;
}


// event 22 - clear text
void Modeller::evTextClear(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Clear text.");

    if (that->state == 20)
    {
        SoAlphabetKit* alpha= (SoAlphabetKit*) findNode(that->pip, "Alpha");
        alpha->text.setValue("");
    }
    else if (that->state == 21)
    {
        // see event 02 on what this does...
        for (int i= 0; i < that->window->getNumChildren(); ++i)
            if (((SoSeparator*) ((SoDragKit*) that->window->getChild(i))->
                getPart("content", TRUE)) == that->activeNode)
                that->window->removeChild(i);
        that->state= 20;
    }
}


// event 23 - enter text
void Modeller::evTextEnter(
    void* userData,        // receiver
    SoAlphabetKit* alpha)  // sender
{
    // Modeller* that= (Modeller*) userData;  // uncomment if needed
    //debug("Enter text.");

    // currently unused.
}


// event 24 - change text
void Modeller::evTextChange(
    void* userData,            // receiver
    int,                       // unimportant here
    SoGestureKit* gestureKit)  // sender
{
    Modeller* that= (Modeller*) userData;  // uncomment if needed
    //debug("Change text.");

    ((SoSwitch*) findNode(that->pip, "AlphaCheat"))->whichChild.setValue(
        ((SoAlphabetKit*) gestureKit)->getMode());
}


// event 31 - new file
void Modeller::evFileNew(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("New file button pressed.");

    So3DRadioButton** file= that->getFileButtons();
    for (int i= 0; i < 6; ++i)
        file[i]->pressed.setValue(false);
    that->window->removeAllChildren();
    that->window->setName(_ModellerString);
    if (that->state == 31) that->state= 30;

    // clean up:
    delete file;
}


// event 32 - load file
void Modeller::evFileLoad(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Load file button pressed.");

    if (that->state == 31)  // a file is selected
    {
        So3DRadioButton** file= that->getFileButtons();
        int j= 1;
        int i;
        for (i= 0; i < 6; ++i)
            if (file[i]->pressed.getValue()) j= i + 1;
        SbString str= that->comm->workingDirectory;
        str+= "/";
        str+= _ModellerSavePath;
        str+= "/File";
        str+= j;
        str+= ".iv";
        //debug("Loading file", str.getString());
        that->window->removeAllChildren();
        SoSeparator* content= readFile(str.getString(),
            that->comm->workingDirectory);
        content->ref();
        bool valid= (content->getName() == _ModellerString);
        that->window->setName(_ModellerString);
        SoDragKit* drag;
        for (i= 0; i < content->getNumChildren(); i++)
        {
            drag= NULL;
            if (content->getChild(i)->getTypeId() ==
                SoDragKit::getClassTypeId())
                drag= ((SoDragKit*) content->getChild(i));
            if (drag && that->master)
            {
                if (valid)
                {
                    SbString name= SbString(drag->getName().getString());
                    if (name.getSubString(0, 3) == "DRAW")
                        drag->setPressCallback(evSelectSketch, that);
                    else if (name.getSubString(0, 3) == "NODE")
                        drag->setPressCallback(evSelectModel, that);
                    else if (name.getSubString(0, 3) == "TEXT")
                        drag->setPressCallback(evSelectText, that);
                    else drag->setName("");
                }
                else drag->setName("");
                drag->setMoveCallback(evMoveObject, that);
                drag->setReleaseCallback(evMoveObject, that);
                that->window->addChild(content->getChild(i));
            }
        }

        // clean up:
        delete file;
        content->unref();
    }
}


// event 33 - save file
void Modeller::evFileSave(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Save file button pressed.");

    if (that->state == 31)  // a file is selected
    {
        So3DRadioButton** file= that->getFileButtons();
        int j= 1;
        for (int i= 0; i < 6; i++)
            if (file[i]->pressed.getValue()) j= i + 1;
        SbString str= that->comm->workingDirectory;
        str+= "/";
        str+= _ModellerSavePath;
        str+= "/File";
        str+= j;
        str+= ".iv";
        //debug("Saving file", str.getString());
        writeFile(str.getString(), that->window);

        // clean up:
        delete file;
    }
}


// event 34 - select file
void Modeller::evFileButton(
    void* userData,           // receiver
    So3DRadioButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("File button pressed.");

    if (that->state == 30) that->state= 31;
}


// event 91 - draw
void Modeller::evMenuDraw(
    void* userData,           // receiver
    So3DRadioButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Button draw pressed.");

    that->state= 0;
    ((SoSwitch*) findNode(that->pip, "RootSw"))->whichChild.setValue(0);
}


// event 92 - model
void Modeller::evMenuModel(
    void* userData,           // receiver
    So3DRadioButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Button model pressed.");

    that->state= 10;
    ((SoSeparator*) findNode(that->pip, "ModelCheat"))->
        removeAllChildren();
    ((SoSwitch*) findNode(that->pip, "RootSw"))->whichChild.setValue(1);
}


// event 93 - text
void Modeller::evMenuText(
    void* userData,           // receiver
    So3DRadioButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Button text pressed.");

    that->state= 20;
    ((SoSwitch*) findNode(that->pip, "RootSw"))->whichChild.setValue(2);
}


// event 94 - file
void Modeller::evMenuFile(
    void* userData,           // receiver
    So3DRadioButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Button file pressed.");

    // physical state change:
    ((SoSwitch*) findNode(that->pip, "RootSw"))->whichChild.setValue(3);

    // logical state change:
    So3DRadioButton** file= that->getFileButtons();
    bool sel= false;  // is a file selected?
    for (int i= 0; i < 6; i++)
        if (file[i]->pressed.getValue()) sel= true;
    if (sel) that->state= 31;
    else that->state= 30;

    // clean up:
    delete file;
}


// event 95 - select sketch
void Modeller::evSelectSketch(
    void* userData,      // receiver
    SoDragKit* dragKit)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Sketch selected.");

    SoSketchKit* sketch= (SoSketchKit*) findNode(that->pip, "Sketch");
    So3DRadioButton** col= that->getColorButtons();
    So3DRadioButton* menu[4];
    menu[0]= (So3DRadioButton*) findNode(that->pip, "ButtDraw");
    menu[1]= (So3DRadioButton*) findNode(that->pip, "ButtModel");
    menu[2]= (So3DRadioButton*) findNode(that->pip, "ButtText");
    menu[3]= (So3DRadioButton*) findNode(that->pip, "ButtFile");

    that->currentRotation= dragKit->rotation.getValue();
    SoSeparator* content= (SoSeparator*) dragKit->getPart("content", TRUE);
    that->activeNode= content;
    SbString str= SbString(dragKit->getName().getString());
    //debug("Name is", str.getString());
    if (str == "DRAW") {
        that->activeColor= (SoMaterial*) content->getChild(0);
        for (int i= 0; i < 8; ++i)
            col[i]->pressed.setValue(col[i]->buttonColor.getValue() ==
                that->activeColor->diffuseColor[0]);
        sketch->point= ((SoCoordinate3*) content->getChild(2))->point;
#ifdef GRT_FENCE  // depends on the layout of the SoSketchKit...
        sketch->numVertices=
            ((SoTriangleStripSet*) content->getChild(3))->numVertices;
#else
        sketch->numVertices= ((SoLineSet*) content->getChild(3))->numVertices;
#endif
        that->state= 1;
    }

    // switch sheets:
    SoSwitch* sw= (SoSwitch*) findNode(that->pip, "RootSw");
    //debug("Switching to sheet", (int) that->state/10);
    sw->whichChild.setValue((int) that->state/10);
    for (int i= 0; i < 4; ++i)
        menu[i]->pressed.setValue(i == (int) that->state/10);

    // clean up:
    delete col;
}


// event 96 - select model
void Modeller::evSelectModel(
    void* userData,      // receiver
    SoDragKit* dragKit)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Model selected.");

    SoCube* modelCube;
    SoCylinder* modelCylinder;
    SoCone* modelCone;
    SoSphere* modelSphere;
    So3DSlider** sl= that->getParameterSliders();
    So3DRadioButton** col= that->getColorButtons();
    So3DRadioButton* menu[4];  // for the main menu
    menu[0]= (So3DRadioButton*) findNode(that->pip, "ButtDraw");
    menu[1]= (So3DRadioButton*) findNode(that->pip, "ButtModel");
    menu[2]= (So3DRadioButton*) findNode(that->pip, "ButtText");
    menu[3]= (So3DRadioButton*) findNode(that->pip, "ButtFile");

    that->currentRotation= dragKit->rotation.getValue();
    SoSeparator* content= (SoSeparator*) dragKit->getPart("content", TRUE);
    SbString str= SbString(dragKit->getName().getString());
    //debug("Name is", str.getString());
    SoSeparator* modelCheat= (SoSeparator*) findNode(that->pip, "ModelCheat");
    modelCheat->removeAllChildren();
    if (str.getSubString(0, 3) == "NODE")
    {
        that->activeColor= (SoMaterial*) content->getChild(0);
        that->activeNode= content->getChild(1);
        for (int i= 0; i < 8; i++)
            if (col[i]->buttonColor.getValue()
                == that->activeColor->diffuseColor[0])
                col[i]->pressed.setValue(true);
            else col[i]->pressed.setValue(false);
        that->model= atoi(str.getSubString(4, 7).getString());
        switch(that->model)
        {
        case 1001:
            //debug("Cube selected.");
            modelCheat->addChild(new SoCube());
            modelCube= (SoCube*) that->activeNode;
            sl[0]->currentValue.setValue(modelCube->width.getValue()/0.2);
            sl[1]->currentValue.setValue(modelCube->height.getValue()/0.2);
            break;
        case 1002:
            //debug("Cylinder selected.");
            modelCheat->addChild(new SoCylinder());
            modelCylinder= (SoCylinder*) that->activeNode;
            sl[0]->currentValue.setValue(
                modelCylinder->height.getValue()/0.2);
            sl[1]->currentValue.setValue(
                modelCylinder->radius.getValue()/0.1);
            break;
        case 1003:
            //debug("Cone selected.");
            modelCheat->addChild(new SoCone());
            modelCone= (SoCone*) that->activeNode;
            sl[0]->currentValue.setValue(modelCone->height.getValue()/0.2);
            sl[1]->currentValue.setValue(
                modelCone->bottomRadius.getValue()/0.1);
            break;
        case 1004:
            //debug("Sphere selected.");
            modelCheat->addChild(new SoSphere());
            modelSphere= (SoSphere*) that->activeNode;
            sl[0]->currentValue.setValue(modelSphere->radius.getValue()/0.1);
            break;
        }
        that->state= 11;
    }

    // switch sheets:
    SoSwitch* sw= (SoSwitch*) findNode(that->pip, "RootSw");
    //debug("Switching to sheet", (int) that->state/10);
    sw->whichChild.setValue((int) that->state/10);
    for (int i= 0; i < 4; i++)
        menu[i]->pressed.setValue(i == (int) that->state/10);

    // clean up:
    delete sl;
    delete col;
}


// event 97 - select text
void Modeller::evSelectText(
    void* userData,      // receiver
    SoDragKit* dragKit)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Model selected.");

    SoAlphabetKit* alpha= (SoAlphabetKit*) findNode(that->pip, "Alpha");
    So3DRadioButton** col= that->getColorButtons();
    So3DRadioButton* menu[4];
    menu[0]= (So3DRadioButton*) findNode(that->pip, "ButtDraw");
    menu[1]= (So3DRadioButton*) findNode(that->pip, "ButtModel");
    menu[2]= (So3DRadioButton*) findNode(that->pip, "ButtText");
    menu[3]= (So3DRadioButton*) findNode(that->pip, "ButtFile");

    that->currentRotation= dragKit->rotation.getValue();
    SoSeparator* content= (SoSeparator*) dragKit->getPart("content", TRUE);
    that->activeNode= content;
    SbString str= SbString(dragKit->getName().getString());
    //debug("Name is", str.getString());
    if (str == "TEXT") {
        that->activeColor= (SoMaterial*) content->getChild(0);
        for (int i= 0; i < 8; i++)
            col[i]->pressed.setValue(col[i]->buttonColor.getValue() ==
                that->activeColor->diffuseColor[0]);
        alpha->text.setValue(
            ((SoText3*) content->getChild(2))->string[0]);
        that->state= 21;
    }

    // switch sheets:
    SoSwitch* sw= (SoSwitch*) findNode(that->pip, "RootSw");
    //debug("Switching to sheet", (int) that->state/10);
    sw->whichChild.setValue((int) that->state/10);
    for (int i= 0; i < 4; i++)
        menu[i]->pressed.setValue(i == (int) that->state/10);

    // clean up:
    delete col;
}


// event 98 - color buttons
void Modeller::evColorButton(
    void* userData,           // receiver
    So3DRadioButton* button)  // sender
{
    Modeller* that= (Modeller*) userData;
    //debug("Color button pressed.");

    SbColor col= button->buttonColor.getValue();
    SoSketchKit* sketch= (SoSketchKit*) findNode(that->pip, "Sketch");
    sketch->sketchColor.setValue(col);
    if ((that->state == 1) ||
        (that->state == 11) ||
        (that->state == 21))  // object selected
        that->activeColor->diffuseColor.setValue(col);
}


// event 99 - move / release object
void Modeller::evMoveObject(
    void* userData,      // receiver
    SoDragKit* dragKit)  // sender
{
    Modeller* that= (Modeller*) userData;  // uncomment if needed
    int i;

    // rasterize translation to 0.01 raster:
    SbVec3f trn= dragKit->translation.getValue();
    for (i= 0; i < 3; ++i) trn[i]= ((int) (100.0f*trn[i]))/100.0f;
    dragKit->translation.setValue(trn);

    // get rotation minus currentRotation:
    SbMatrix rotM;
    that->currentRotation.getValue(rotM);
    rotM= rotM.inverse();
    SbRotation rot= SbRotation(dragKit->rotation.getValue()*rotM);
    SbVec3f axis;
    float angle= 0.0f;  // must be initialized...
    rot.getValue(axis, angle);

    // normalize values:
    if (angle < 0)
    {
        angle*= -1;
        for (int i= 0; i < 3; ++i)
            axis[i]*= -1;
    }

    // should we rotate?
    if (angle >= 0.523598776)  // 30°
    {
        float x= axis[0] >= 0 ? axis[0] : -axis[0];
        float y= axis[1] >= 0 ? axis[1] : -axis[1];
        float z= axis[2] >= 0 ? axis[2] : -axis[2];

        // calculate rotation in grid:
        if ((x > y) && (x > z))
            if (axis[0] > 0) rot= SbRotation(
                SbVec3f(0.0f, 1.0f, 0.0f), SbVec3f(0.0f, 0.0f, 1.0f));  // +x
            else rot= SbRotation(
                SbVec3f(0.0f, 0.0f, 1.0f), SbVec3f(0.0f, 1.0f, 0.0f));  // -x
        if ((x <= y) && (y > z))
            if (axis[1] > 0) rot= SbRotation(
                SbVec3f(0.0f, 0.0f, 1.0f), SbVec3f(1.0f, 0.0f, 0.0f));  // +y
            else rot= SbRotation(
                SbVec3f(1.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 1.0f));  // -y
        if ((x <= z) && (y <= z))
            if (axis[2] > 0) rot= SbRotation(
                SbVec3f(1.0f, 0.0f, 0.0f), SbVec3f(0.0f, 1.0f, 0.0f));  // +z
            else rot= SbRotation(
                SbVec3f(0.0f, 1.0f, 0.0f), SbVec3f(1.0f, 0.0f, 0.0f));  // -z

        // apply rotation:
        dragKit->rotation.setValue(rot*that->currentRotation);

    }
    else  // do not detect rotation
        dragKit->rotation.setValue(that->currentRotation);
}
