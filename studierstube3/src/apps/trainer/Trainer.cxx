// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Trainer.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Training app for the SoGestureKit
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    bas             Benjamin A. Schmit
// ===========================================================================
//  HISTORY:    see Trainer.h
// ===========================================================================

#include "Trainer.h"

#include <Inventor/SbBox.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTransform.h>
#include <stbapi/misc/GRT_defs.h>
#include <stbapi/util/ivio.h>
#include <stbapi/workspace/SoContextKitSub.h>

SO_KIT_SOURCE(Trainer);
CONTEXT_APPLICATION(Trainer);


void Trainer::initClass()
{
    SO_KIT_INIT_CLASS(Trainer, SoContextKit, "SoContextKit");
}


Trainer::Trainer()
{
    SO_KIT_CONSTRUCTOR(Trainer);
    SO_KIT_INIT_INSTANCE();
    state= 0;
    imgr.libNew();
}


SoContextKit* Trainer::factory()
{
    return new Trainer();
}


Trainer::~Trainer()
{
    destructor();
};


SoNode* Trainer::createPipSheet(int uid)
{
    // get the sheet from TrainerSheet.iv:
    SoSeparator* sheet=
        readFile("TrainerSheet.iv", comm->workingDirectory);
    sheet->ref();
    pip= sheet;

    // let all nodes be found:
    SoSwitch* rootSw= (SoSwitch*) findNode(sheet, "RootSw");
    rootSw->whichChild.setValue(SO_SWITCH_ALL);

    // initialize sheet components:
    SoText3* libText= (SoText3*) findNode(sheet, "LibText");
    SoTransform* libTrans= (SoTransform*) findNode(sheet, "LibTrans");
    SoText3* idText= (SoText3*) findNode(sheet, "IdText");
    SoTransform* idTrans= (SoTransform*) findNode(sheet, "IdTrans");

    // adjust text length:
    libTrans->scaleFactor.setValue(scaleDownX(sheet, libText, 7.5f),
        1.0f, 1.0f);
    idTrans->scaleFactor.setValue(scaleDownX(sheet, idText, 2.5f),
        1.0f, 1.0f);

    // display initial sheet:
    rootSw->whichChild.setValue(0);

    // finish:
    sheet->unrefNoDelete();
    return sheet;
}


SoWindowKit* Trainer::createWindowGeometry(int index)
{
    // we don't use a window:
    return NULL;
}


void Trainer::setSheetMasterMode(SoNode* pipSheet, SbBool masterMode)
{
    // view all elements so the searches will be successful:
    SoSwitch* sw= (SoSwitch*) findNode(pipSheet, "RootSw");
    int swc= sw->whichChild.getValue();
    sw->whichChild.setValue(SO_SWITCH_ALL);

    // find elements for callbacks:
    SoSketchKit* sketch= (SoSketchKit*) findNode(pipSheet, "Sketch");
    SoAlphabetKit* alpha= (SoAlphabetKit*) findNode(pipSheet, "Alpha");
    So3DButton* up= (So3DButton*) findNode(pipSheet, "ButtUp");
    So3DButton* down= (So3DButton*) findNode(pipSheet, "ButtDn");
    So3DButton* learn= (So3DButton*) findNode(pipSheet, "ButtOk");
    So3DButton* reject= (So3DButton*) findNode(pipSheet, "ButtCl");
    So3DButton* lib= (So3DButton*) findNode(pipSheet, "LibButt");
    So3DButton* id= (So3DButton*) findNode(pipSheet, "IdButt");
    So3DButton* load= (So3DButton*) findNode(pipSheet, "ButtLoad");
    So3DButton* save= (So3DButton*) findNode(pipSheet, "ButtSave");
    So3DButton* back= (So3DButton*) findNode(pipSheet, "ButtBack");
    So3DButton* set= (So3DButton*) findNode(pipSheet, "SetId");
    So3DButton* rej= (So3DButton*) findNode(pipSheet, "RejId");

    if (masterMode)
    {
        // set callbacks:
        sketch->setPressCallback(evSketchPress, this);
        sketch->setMoveCallback(evSketchMove, this);
        sketch->setReleaseCallback(evSketchRelease, this);
        up->addReleaseCallback(evButt0Up, this);
        down->addReleaseCallback(evButt0Down, this);
        learn->addReleaseCallback(evButt0Learn, this);
        reject->addReleaseCallback(evButt0Reject, this);
        lib->addReleaseCallback(evButt0Library, this);
        id->addReleaseCallback(evButt0ID, this);
        load->addReleaseCallback(evButt1Load, this);
        save->addReleaseCallback(evButt1Save, this);
        back->addReleaseCallback(evButt1Back, this);
        set->addReleaseCallback(evButt2OK, this);
        rej->addReleaseCallback(evButt2Cancel, this);
    }
    else
    {
        // remove callbacks:
        sketch->removePressCallback();
        sketch->removeMoveCallback();
        sketch->removeReleaseCallback();
        up->removeReleaseCallback(evButt0Up);
        down->removeReleaseCallback(evButt0Down);
        learn->removeReleaseCallback(evButt0Learn);
        reject->removeReleaseCallback(evButt0Reject);
        lib->removeReleaseCallback(evButt0Library);
        id->removeReleaseCallback(evButt0ID);
        load->removeReleaseCallback(evButt1Load);
        save->removeReleaseCallback(evButt1Save);
        back->removeReleaseCallback(evButt1Back);
        set->removeReleaseCallback(evButt2OK);
        rej->removeReleaseCallback(evButt2Cancel);
    }

    // restore active sheet:
    sw->whichChild.setValue(swc);
}


// event 01 - sketch press
void Trainer::evSketchPress(
    void* userData,          // receiver
    SoSketchKit* sketchKit)  // sender
{
    Trainer* that= (Trainer*) userData;

    switch (that->state)
    {
    case 1:
        {
            // start a gesture:
            SbVec2f vec= sketchKit->posOnSurface();
            float factor= (float) sqrt(_grt_area*sketchKit->xSize.getValue()/
                sketchKit->ySize.getValue())/sketchKit->xSize.getValue();
            that->imgr.gstNew(vec[0], vec[1], factor);
            that->state= 2;
        }
        break;
    case 3:
        that->state= 2;
        break;
    }
}


// event 02 - sketch move:
void Trainer::evSketchMove(
    void* userData,          // receiver
    SoSketchKit* sketchKit)  // sender
{
    Trainer* that= (Trainer*) userData;

    if (that->state == 2)
    {
        // continue the gesture:
        SbVec2f vec= sketchKit->posOnSurface();
        that->imgr.gstCont(vec[0], vec[1]);
    }
}


// event 03 - sketch release
void Trainer::evSketchRelease(
    void* userData,          // receiver
    SoSketchKit* sketchKit)  // sender
{
    Trainer* that= (Trainer*) userData;

    if (that->state == 2)
    {
        // continue the gesture:
        SbVec2f vec= sketchKit->posOnSurface();
        that->imgr.gstCont(vec[0], vec[1]);
        that->state= 3;
    }
}


// event 04 - button up
void Trainer::evButt0Up(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if ((that->state == 1) || (that->state == 3))
    {
        // increase gesture id:
        SoText3* idText= (SoText3*) findNode(that->pip, "IdText");
        SoTransform* idTrans=
            (SoTransform*) findNode(that->pip, "IdTrans");
        idTrans->scaleFactor.setValue(1.0f, 1.0f, 1.0f);
        idText->string.setValue(SbString(
            atoi(idText->string[0].getString()) + 1));
        idTrans->scaleFactor.setValue(
            scaleDownX(that->pip, idText, 2.5f), 1.0f, 1.0f);
        that->state= 1;
    }
}


// event 05 - button down
void Trainer::evButt0Down(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if ((that->state == 1) || (that->state == 3))
    {
        // decrease gesture id:
        SoText3* idText= (SoText3*) findNode(that->pip, "IdText");
        SoTransform* idTrans=
            (SoTransform*) findNode(that->pip, "IdTrans");
        idTrans->scaleFactor.setValue(1.0f, 1.0f, 1.0f);
        idText->string.setValue(SbString(
            atoi(idText->string[0].getString()) - 1));
        idTrans->scaleFactor.setValue(
            scaleDownX(that->pip, idText, 2.5f), 1.0f, 1.0f);
        that->state = 1;
    }
}


// event 06 - button learn
void Trainer::evButt0Learn(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if (that->state == 3)
    {
        // learn the gesture:
        that->imgr.gstLearn(atoi(((SoText3*) findNode(that->pip, "IdText"))->
            string[0].getString()));
        that->state= 1;
    }
}


// event 07 - button reject
void Trainer::evButt0Reject(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if (that->state  == 3) that->state= 1;
}


// event 08 - button library
void Trainer::evButt0Library(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if ((that->state == 0) || (that->state == 1) || (that->state == 3))
    {
        SoAlphabetKit* alpha= (SoAlphabetKit*) findNode(that->pip, "Alpha");
        alpha->clear();  // delete leftover sketches
        if (that->state != 0)
            alpha->text.setValue(
            ((SoText3*) findNode(that->pip, "LibText"))->string[0]);
            // initialize library string if library is loaded
        that->state0= that->state;  // backup state
        that->state= 21;
        ((SoSwitch*) findNode(that->pip, "RootSw"))->whichChild.setValue(1);
    }
}


// event 09 - button id
void Trainer::evButt0ID(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    switch (that->state)
    {
    case 1:
        {
            SoAlphabetKit* number=
                (SoAlphabetKit*) findNode(that->pip, "Number");
            number->clear();
            number->text.setValue("");
            that->state0= that->state;  // backup state
            that->state= 41;
            ((SoSwitch*) findNode(that->pip, "RootSw"))->
                whichChild.setValue(2);
        }
        break;

    case 3:
        {
            // identify the gesture:
            SoText3* idText= (SoText3*) findNode(that->pip, "IdText");
            SoTransform* idTrans=
                (SoTransform*) findNode(that->pip, "IdTrans");
            idTrans->scaleFactor.setValue(1.0f, 1.0f, 1.0f);
            idText->string.setValue(SbString(that->imgr.gstRecog()));
            idTrans->scaleFactor.setValue(
                scaleDownX(that->pip, idText, 2.5f), 1.0f, 1.0f);
            that->state= 1;
        }
        break;
    }
}


// event 21 - button load
void Trainer::evButt1Load(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if (that->state == 21)
    {
        SbString full= SbString(that->comm->workingDirectory);
        full+= "/gestures/";
        SbString file= ((SoAlphabetKit*) findNode(that->pip, "Alpha"))->
            text.getValue();
        full+= file;
        if (that->imgr.libLoad(full.getString()))
        {
            ((SoSwitch*) findNode(that->pip, "RootSw"))->
                whichChild.setValue(0);
            SoText3* libText= (SoText3*) findNode(that->pip, "LibText");
            SoTransform* libTrans=
                (SoTransform*) findNode(that->pip, "LibTrans");
            libTrans->scaleFactor.setValue(1.0f, 1.0f, 1.0f);
            libText->string.setValue(file);
            libTrans->scaleFactor.setValue(
                scaleDownX(that->pip, libText, 7.5f), 1.0f, 1.0f);
            that->state= 1;
        }
    }
}


// event 22 - button save
void Trainer::evButt1Save(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if (that->state == 21)
    {
        SbString full= that->comm->workingDirectory;
        full+= "/gestures/";
        SbString file= ((SoAlphabetKit*) findNode(that->pip, "Alpha"))->
            text.getValue();
        full+= file;
        if ((that->state0 != 0) && that->imgr.libSave(full.getString()))
        {
            ((SoSwitch*) findNode(that->pip, "RootSw"))->
                whichChild.setValue(0);
            SoText3* libText= (SoText3*) findNode(that->pip, "LibText");
            SoTransform* libTrans=
                (SoTransform*) findNode(that->pip, "LibTrans");
            libTrans->scaleFactor.setValue(1.0f, 1.0f, 1.0f);
            libText->string.setValue(file);
            libTrans->scaleFactor.setValue(
                scaleDownX(that->pip, libText, 7.5f), 1.0f, 1.0f);
            that->state= that->state0;
        }
    }
}


// event 23 - button back
void Trainer::evButt1Back(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if (that->state == 21)
    {
        that->state= that->state0;
        ((SoSwitch*) findNode(that->pip, "RootSw"))->whichChild.setValue(0);
    }
}


// event 41 - button ok
void Trainer::evButt2OK(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if (that->state == 41)
    {
        ((SoSwitch*) findNode(that->pip, "RootSw"))->whichChild.setValue(0);
        SoText3* idText= (SoText3*) findNode(that->pip, "IdText");
        SoTransform* idTrans=
            (SoTransform*) findNode(that->pip, "IdTrans");
        idTrans->scaleFactor.setValue(1.0f, 1.0f, 1.0f);
        idText->string.setValue(SbString(atoi(((SoAlphabetKit*)
            findNode(that->pip, "Number"))->text.getValue().getString())));
            // copy numeric part of gesture id
        idTrans->scaleFactor.setValue(
            scaleDownX(that->pip, idText, 2.5f), 1.0f, 1.0f);
        that->state= 1;
    }
}


// event 42 - button cancel
void Trainer::evButt2Cancel(
    void* userData,      // receiver
    So3DButton* button)  // sender
{
    Trainer* that= (Trainer*) userData;

    if (that->state == 41)
    {
        that->state= 1;
        ((SoSwitch*) findNode(that->pip, "RootSw"))->whichChild.setValue(0);
    }
}


/// This scales down the x size of an object.
float Trainer::scaleDownX(SoNode* scene, SoNode* node, float size)
{
    SbBox3f bbox= getBoundingBox(scene, node);
    if (bbox.getMax()[0] == bbox.getMin()[0]) return size;
    float scale= size/(bbox.getMax()[0] - bbox.getMin()[0]);
    if (scale < 1.0f) return scale;
    else return 1.0f;
}


/// This returns the bounding box of an object.
SbBox3f Trainer::getBoundingBox(SoNode* scene, SoNode* node)
{
    SoSearchAction sa= SoSearchAction();
    sa.setNode(node);
    sa.apply(scene);
    SoPath* path= sa.getPath();
    SoGetBoundingBoxAction bba= SoGetBoundingBoxAction(SbViewportRegion());
    bba.apply(path);
    SbBox3f bbox= bba.getBoundingBox();
    return bbox;
}
