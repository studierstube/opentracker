/* ===========================================================================
    (C) 2000 Vienna University of Technology
   ===========================================================================
    NAME:       snapshot
    TYPE:       c++ code
    PROJECT:    Studierstube
    CONTENT:    Implementation snapshot/seam file system module
    VERSION:    2.2
   ===========================================================================
    AUTHORS:    ds      Dieter Schmalstieg
                gh      Gerd Hesina
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    08-Nov-00 22:57:56  jp      last modification
    08-Nov-00 22:56:42  jp      modifications for gcc 2.96 strong ISO
    17-sep-98 21:47:52  ds      created
=========================================================================== */


#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <Inventor/SbPList.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <stbapi/widgets/So3DCheckBox.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/seams/SoSeamKit.h>
#include <stbapi/misc/SoFishnet.h>
#include <stbapi/misc/SoMultMatrix.h>
#include <stbapi/util/ivio.h>

#include "piputil.h"
#include "snapshot.h"
#include <stbapi/misc/wstudierstube.h>

//-----------------------------------------------------------------------------

struct SeamFileData             //stores information associated with one seam
{
    SoSeamKit*       seam;      //seam node itself
    SoMultMatrix*    multEng;   //engine which is disconnected to freeze
	SoMultMatrix*    invEng;    //engine which is left dangling after freeze
	SoComposeMatrix* compEng;   //engine which is disconnected when stored
	SoTransform*     seamTrafo; //transformation of the seam
    SoSeparator*     seamFrame; //pointer to frame separator
};

SeamFileData*    sfdCurrent;       //pointer to current seam file structure
SbPList          sfdlist;          //all other seam files

SoSeparator*     sceneRoot;        //root of the scene graph
SoSeparator*     worldSep;         //separator of the world scene
SoSeparator*     currentSeamSep;   //separator holding current seam on pip
SoSeparator*     storedSeamSep;    //separator holding stored seams
SoSeparator*     seamFrameSep;     //separator holding seam frames
SoSeparator*     seamFrameProto;   //seam frame to copy from
SoTrakEngine*    trakSource;       //trak engine for pip
SoFishnet*       seamFishnetNode;  //fishnet node for picking up seams
SoCoordinate3*   seamPipCoord;     //pip coordinates

// buttons
So3DCheckBox*    seamLockButton;   //button to lock (=freeze) semam on pip
So3DButton*      seamStoreButton;  //button to disconnect seam from pip
So3DButton*      seamPickupButton; //button to pick up a seam with pip
So3DButton*      seamCopyButton;   //button to copy a seam
So3DButton*      seamMaxButton;    //button to transfer stored scene to world


//-----------------------------------------------------------------------------
// connect (pickup) given seam file to pip
// sfd: seam file data for seam to connect

void
connectSeamToPip(SeamFileData* sfd)
{
    sfd->seamTrafo->translation.connectFrom(&trakSource->translation);
    sfd->seamTrafo->rotation.connectFrom(&trakSource->rotation);
    sfd->multEng->matrix2.connectFrom(&sfd->compEng->matrix); //reconnect
}

//-----------------------------------------------------------------------------
// disconnect (store) given seam from pip
// sfd: seam file data for seam to disconnect

void
disconnectSeamFromPip(SeamFileData* sfd)
{
    sfd->seamTrafo->translation.disconnect(); 
    sfd->seamTrafo->rotation.disconnect();
    sfd->multEng->matrix2.disconnect();
}

//-----------------------------------------------------------------------------
// lock (freeze) sema on pip
// sfd: seam file data for seam to lock

void
lockSeamOnPip(SeamFileData* sfd)
{
    sfd->multEng->matrix1.disconnect(); //break the connection in this place
}

//-----------------------------------------------------------------------------
// unlock seam on pip
// sfd: seam file data for seam to unlock

void
unlockSeamOnPip(SeamFileData* sfd)
{
    sfd->multEng->matrix1.connectFrom(&sfd->invEng->inverse); //reconnect
}

//-----------------------------------------------------------------------------
// make a new seam + seam file on the pip
// returns: new seam file

SeamFileData*
newSeamOnPip()
{
//--create new seam
    SeamFileData* sfd = new SeamFileData;
    NEWNODE(seam,SoSeamKit);
    sfd->seam = seam;

    assert(seam->setPart("seamGeometry",seamPipCoord));
    //must be false because stereo viewer does not have camera
    //this is a flaw but we don't use nested seams anyhow
    seam->checkVisibility = FALSE; 
    assert(seam->setPart("secondaryWorld",worldSep)); //seam sees normal world

//--create various engines etc.
    sfd->seamTrafo = (SoTransform*)seam->getPart("seamTransform",TRUE);
    SoTransform* secondaryTrafo = 
        (SoTransform*)seam->getPart("secondaryTransform",TRUE);
    NEWNODE(decompEng,SoDecomposeMatrix);
    sfd->multEng = new SoMultMatrix;
    sfd->invEng = new SoMultMatrix;
    sfd->compEng = new SoComposeMatrix;
    trakSource = WStudierstube::getPipEngine();

//--hook engines up    
    secondaryTrafo->translation.connectFrom(&decompEng->translation);
    secondaryTrafo->rotation.connectFrom(&decompEng->rotation);
    decompEng->matrix.connectFrom(&sfd->multEng->product);
    sfd->multEng->matrix1.connectFrom(&sfd->invEng->inverse);
    sfd->invEng->matrix1.connectFrom(&sfd->compEng->matrix);
    sfd->compEng->translation.connectFrom(&trakSource->translation);
    sfd->compEng->rotation.connectFrom(&trakSource->rotation);
    sfd->multEng->matrix2.connectFrom(&sfd->compEng->matrix);

    // connect seamTransform to pip engine -- cannot do that in .iv file cause
    // disconnecting the engine would lead to problems with unref and deletion
    sfd->seamTrafo->translation.connectFrom(&trakSource->translation);
    sfd->seamTrafo->rotation.connectFrom(&trakSource->rotation);
    
    sfd->invEng->ref(); //need this so it's not deleted if disconnected

    return sfd;
}

//-----------------------------------------------------------------------------
// callback if a seam is picked up by the fishnet node
// node: SoSeamKit that was picked up
// fishnet: pointer to fishnet node

void 
seamFishCallback(SoNode* node,SbVec3f ,void* ,SoFishnet* fishnet)
{
    int i;
    // find seam file for which the seam was picked up
    for(i=0; 
        i<sfdlist.getLength() && ((SeamFileData*)sfdlist[i])->seam != node;
        i++) ;

    assert(i<sfdlist.getLength()); //make sure we found it
    SeamFileData* sfdCaught = (SeamFileData*)sfdlist[i]; //get seam file
    
    currentSeamSep->replaceChild(0,sfdCaught->seam);

    // remove the seam from the stored seam separaotor
    int n=storedSeamSep->findChild(sfdCaught->seam);
    assert(sfdCaught->seam == node);
    assert(n != -1);
    storedSeamSep->removeChild(n);

    // remove frame
    n = seamFrameSep->findChild(sfdCaught->seamFrame);
    assert(n != -1);
    seamFrameSep->removeChild(n);

    // remove seam file from stored seam file list
    sfdlist.remove(sfdlist.find(sfdCurrent));
    delete sfdCurrent;
    sfdCurrent = sfdCaught;

    // transfer seam onto pip
    connectSeamToPip(sfdCurrent);
    seamLockButton->pressed = TRUE; //always pick it up in locked state
    seamPickupButton->buttonColor = SbColor(1,1,1);
    seamFishnetNode->setFishing(FALSE); //for now, lets pick up just one!
}

//-----------------------------------------------------------------------------
// callback for (un)lock button
// button: pointer to lock button

void
cbSeamLockButtonPress(void*, So3DCheckBox* button)
{
    if(button->pressed.getValue())
    {
        lockSeamOnPip(sfdCurrent);
    }
    else
    {
        unlockSeamOnPip(sfdCurrent);
    }
}

//-----------------------------------------------------------------------------
// callback for store button - store seam, ie give it a local copy of the
// world and disconnect it from pip
void
cbSeamStoreButtonPress(void*, So3DButton*)
{
    lockSeamOnPip(sfdCurrent);         //must be locked first
    disconnectSeamFromPip(sfdCurrent); //the disconected from pip

    // copy the current world for storage
    SoSeparator* currentWorld;
    currentWorld=(SoSeparator*)sfdCurrent->seam->getPart("secondaryWorld",TRUE);
    assert(currentWorld != NULL);
    SoSeparator* newWorld = (SoSeparator*)currentWorld->copy();
    assert(sfdCurrent->seam->setPart("secondaryWorld",newWorld));

    sfdCurrent->seam->setBGColor(SbColor(0.2,0.2,0.3)); //set different color
    storedSeamSep->addChild(sfdCurrent->seam); //add to stored seams

    //add frame
    sfdCurrent->seamFrame = (SoSeparator*)seamFrameProto->copy();
    seamFrameSep->addChild(sfdCurrent->seamFrame);
    GETCHILD(t,SoTransform,sfdCurrent->seamFrame,0);
    t->translation = sfdCurrent->seamTrafo->translation.getValue();
    t->rotation = sfdCurrent->seamTrafo->rotation.getValue();

    //** from here on there is a new seam on pip!
    sfdCurrent = newSeamOnPip(); //instead create new seam on pip
    currentSeamSep->replaceChild(0,sfdCurrent->seam); //replace current seam
    sfdlist.append(sfdCurrent);       //add new seam to list of seams
    seamLockButton->pressed = FALSE;  //unlock button


    //no pickup pending
    seamPickupButton->buttonColor = SbColor(1,1,1); 
    seamFishnetNode->setFishing(FALSE);
}

//-----------------------------------------------------------------------------
// callback for pickup button - prepare picking up of one seam

void
cbSeamPickupButtonPress(void*, So3DButton*)
{
    seamPickupButton->buttonColor = SbColor(0,1,1);
    seamFishnetNode->setFishing(TRUE); //release freeze->fish
}

//-----------------------------------------------------------------------------
// callback for maximize button - copy scene from current seam to world

void
cbSeamMaxButtonPress(void*, So3DButton*)
{
    int index = sceneRoot->findChild(worldSep);
    assert(index != -1);

    // replace world with world from seam
    worldSep=(SoSeparator*)sfdCurrent->seam->getPart("secondaryWorld",TRUE);
    sceneRoot->replaceChild(index,worldSep);
    // also change artifacts pointer to new set of artifacts
    artifacts = (SoSeparator*)worldSep->getChild(0);//let it point to new artif
}

//-----------------------------------------------------------------------------
//connect a seam with engines to the tracker
//idea: the secondaryTransform part of the seam is connected to a matrix
//inverse(T) * T, where T is the tracker matrix (rotation + translation
//later, we can have two effects: disconnecting inverse(T) from the tracker
//leaves the current view of the world (a "snapshot").
//disconnecting the seamTransform part of the seam freezes the seams position
//in both cases it is still possible to change the camera position with respect
//to the seam

void
setupSnapshot()
{
//--set up general stuff and first seam
    AVGETNODE(sceneRoot,SoSeparator,"SCENE_ROOT");
    AVGETNODE(seamPipCoord,SoCoordinate3,"SEAM_PIP_COORD");
    AVGETNODE(worldSep,SoSeparator,"GROUND");
    AVGETNODE(currentSeamSep,SoSeparator,"CURRENT_SEAM_SEP");
    AVGETNODE(storedSeamSep,SoSeparator,"STORED_SEAM_SEP");
    AVGETNODE(seamFrameSep,SoSeparator,"SEAM_FRAME_SEP");

#ifdef STB_VIENNA_AR
    seamFrameProto = readFile("data/seamframe_vienna.iv");
#else
    seamFrameProto = readFile("data/seamframe.iv");
#endif

    seamFrameProto->ref(); //make sure it is not deleted prematurely

    sfdCurrent = newSeamOnPip();
    currentSeamSep->addChild(sfdCurrent->seam);
    sfdlist.append(sfdCurrent);

//--set up buttons
    AVGETNODE(seamLockButton,So3DCheckBox,"SEAM_LOCK_BUTTON");
    seamLockButton->addPressCallback(cbSeamLockButtonPress,NULL);

    AVGETNODE(seamStoreButton,So3DButton,"SEAM_STORE_BUTTON");
    seamStoreButton->addPressCallback(cbSeamStoreButtonPress,NULL);

    AVGETNODE(seamPickupButton,So3DButton,"SEAM_PICKUP_BUTTON");
    seamPickupButton->addPressCallback(cbSeamPickupButtonPress,NULL);

    AVGETNODE(seamMaxButton,So3DButton,"SEAM_MAX_BUTTON");
    seamMaxButton->addPressCallback(cbSeamMaxButtonPress,NULL);

//--setup fishnet
    AGETNODE(fishnetCoord,SoCoordinate3,"PIP_COORD");
    seamFishnetNode = new SoFishnet;
    seamFishnetNode->setCallback(seamFishCallback,NULL);
    seamFishnetNode->radius = 0.1;
    seamFishnetNode->root = WStudierstube::root;
    seamFishnetNode->coord.setValues(0,fishnetCoord->point.getNum(),
                                 fishnetCoord->point.getValues(0));

    seamFishnetNode->artifacts = storedSeamSep;

    WStudierstube::pipSep->addChild(seamFishnetNode);
}

//-----------------------------------------------------------------------------
// set current seam on pip (in)visible
// onOrOff: flag for visibility

void
setCurrentSeamVisibility(SbBool onOrOff)
{
    GETNODE(sw,SoSwitch,"CURRENT_SEAM_SWITCH");
    if(sw) sw->whichChild = onOrOff ? 0 : -1;
}

//-----------------------------------------------------------------------------
//eof
