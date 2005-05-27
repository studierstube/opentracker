// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       fishtool
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of fishnet selection tool
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:17:53  gh      last modification
//  17-sep-98 21:47:52  ds      created
// ===========================================================================


#include <stdio.h>
#include <assert.h>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/interaction/So3DSensor.h>
#include <stbapi/seams/SoSeamKit.h>
#include <stbapi/misc/SoArtifactKit.h>
//#include <stbapi/misc/SoFishnet.h>

#include <stbapi/misc/wstudierstube.h>
#include "piputil.h"
#include <stbapi/misc/SoFishnet.h>

// ---------------------------------------------------------------------------

SoCoordinate3* fishnetCoord; //coordiantes of pip = fishnet
SoSeparator* fishnetSep;     //separator to place 3D icons ("fishes")
SoFishnet* fishnetNode;      //actual fishnet node

// ---------------------------------------------------------------------------
// delete icon for which its detector has called back
// data: pointer to icon
// detector: the icon's detector

void
cbFinishDeleteFish(void* data, So3DSensor* detector)
{
    SoArtifactKit* artifact = (SoArtifactKit*)data;
    if(artifacts->findChild(artifact) != -1) //if found
        artifact->selected = FALSE;          //unselect
    fishnetSep->removeChild(detector);       //and remove icon
}

// ---------------------------------------------------------------------------
// create new icon after an artifact has been caught in fishnet
// artifact: the artifact that has been caught
// t: the position in fishnet space where it has been caught
// returns: the new icon

SoSeparator*
newFish(SoArtifactKit* artifact,SbVec3f t)
{
    So3DSensor* dsep = new So3DSensor; //new detector
    dsep->setReleaseCallback(cbFinishDeleteFish,artifact); //call if picked
    SoTransform* trafo = new SoTransform;
    trafo->translation = t+SbVec3f(0,-1,0); //position slightly above pip
    dsep->addChild(trafo);
    SoNode* ref;
    assert(ref=artifact->getPart("geometry",FALSE)); //set geometry of icon
    dsep->addChild(ref);
    return dsep;
}

// ---------------------------------------------------------------------------
// callback executes when the fishnet catches an artifact

void 
fishCallback(
    SoNode* node,                  //pointer to fish
    SbVec3f pos,                   //position of catching in fishnet space
    void* data,                    //user data
    SoFishnet* fishnet             //pointer to fishnet instance
    )
{
    SoArtifactKit* artifact = (SoArtifactKit*)node;
    SbBool caught = !artifact->selected.getValue(); //if not already select
    artifact->selected = TRUE;                      //then select
    if(caught) fishnetSep->addChild(newFish(artifact,pos)); //and add icon
}

// ---------------------------------------------------------------------------
// arrange icons on the pip so they do not overlap

void
cbArrange(void*, So3DButton*)
{
    for(int i=0; i<fishnetSep->getNumChildren(); i++)
    {
        GETCHILD(dsep,So3DSensor,fishnetSep,i);
        GETCHILD(trafo,SoTransform,dsep,0);
        // arrange in a regular grid with spacing two inches 
        trafo->translation = SbVec3f(-3.5+(i%4)*2,-1,1-(i/4)*2);
    }
}

// ---------------------------------------------------------------------------
// initialize the fishing module

void
setupFishing(SoSeparator* sceneRoot, SoSeparator* pipRoot)
{
    AVGETNODE(fishnetCoord,SoCoordinate3,"PIP_COORD");    //fishnet polygon
    AVGETNODE(fishnetSep,SoSeparator,"FISH_SEP");         //root of all icons
    AGETNODE(arrangeButton,So3DButton,"ARRANGE_BUTTON");
    arrangeButton->addPressCallback(cbArrange,NULL);

//--setup fishnet node
    fishnetNode = new SoFishnet;
    fishnetNode->setCallback(fishCallback,NULL);
    fishnetNode->radius = 0.1;
    fishnetNode->root = sceneRoot;
    fishnetNode->coord.setValues(0,fishnetCoord->point.getNum(),
                                 fishnetCoord->point.getValues(0));
    fishnetNode->artifacts = artifacts;
    pipRoot->addChild(fishnetNode); //add fishnet to pip
}

// ---------------------------------------------------------------------------
// reset the fishing, ie remove all icons from pip 

void
resetFishing()
{
    //int max = fishnetSep->getNumChildren();
    //for(int i=0; i<max; i++) fishnetSep->removeChild(0);

    fishnetSep->removeAllChildren();
}

// ---------------------------------------------------------------------------
// switch fishing mode on or off
// onOrOff: flag to set fishing mode

void
setFishingMode(SbBool onOrOff)
{
    if(fishnetNode) fishnetNode->setFishing(onOrOff);
}

// ---------------------------------------------------------------------------
//eof
