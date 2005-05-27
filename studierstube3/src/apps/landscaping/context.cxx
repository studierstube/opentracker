// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       context
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of context sensitive pip display module
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


#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoSwitch.h>
#include <stbapi/misc/wstudierstube.h>
#include "piputil.h"
#include "context.h"

//-----------------------------------------------------------------------------

SoTranslation* offset;           //offset of closest object on pip (crosshair)
SoText3* label;                  //text appearing on pip
SoSwitch* contextSwitch;         //switch whether context is on
SoSwitch* buildingSwitch;        //pointer to buildings to find names
SoSwitch* treeSwitch;            //pointer to trees to find names
SoArtifactKit* selectedArtifact; //the closes artifact
SoCoordinate3* pipCoord;         //the outline of the pip

//-----------------------------------------------------------------------------
// color object if one of the context sensitive color buttons has been pressed

void
cbContextButtonPress(void*, So3DButton* button)
{
    selectedArtifact->color = button->buttonColor.getValue();
}

//-----------------------------------------------------------------------------
// search for a model with a given name under a switch (buildings or trees)
// returns: true if the model is found
// idString: name (DEF) to search for
// modelSwitch: switch to search

SbBool
containsModel(SbString idString, SoSwitch* modelSwitch)
{
    for(int i=0; i<buildingSwitch->getNumChildren(); i++)
    {
        GETCHILD(model,SoSeparator,modelSwitch,i);
        if(model->getName().getString()==idString) return TRUE;
    }
    return FALSE;
}

//-----------------------------------------------------------------------------
// do the actual context sensitive computation

void
doContext()
{
    // find the matrix for pip
    SbMatrix matrix;
    SbVec3f eyeW,eyeP,isectP; //W=world,P=PIP
    if(!getMatrix(pipCoord,matrix))
    {
        printf("warning: could not get matrix for pip coords\n");
        return;
    }
    // compute eye position in pip space
    eyeW = WStudierstube::getMiddleEyePos();
    matrix.inverse().multVecMatrix(eyeW,eyeP);

//--find closest artifact
    selectedArtifact = NULL; //closest artifact
    SbVec3f closestPoint; //projected point of closest artifact
    for(int i=0; i<artifacts->getNumChildren(); i++) 
    {
        GETCHILD(artifact,SoArtifactKit,artifacts,i);
        if(artifact->selected.getValue()) artifact->selected=FALSE; //unselect
        SbBool res;
        // get penetration point of line eye-artifact in pip space
        res=projectArtifactToPip(artifact,eyeP,matrix,pipCoord,isectP);
        if(res) //penetration point within pip polygon
        {
        	if(selectedArtifact==NULL || isectP.length()<closestPoint.length())
        	{                                //if closer than previous points
            	selectedArtifact = artifact; //select it
            	closestPoint = isectP;       //new closest point
        	}
        }
    }

//--find category (building, tree) of selected artifact
    SbString idString;
    int category = -1;
    if(selectedArtifact)
    {
        selectedArtifact->selected = TRUE;
        idString = selectedArtifact->getIdString();

        if(containsModel(idString,buildingSwitch))  category = 1;
        else if(containsModel(idString,treeSwitch)) category = 0;
    }
    // set color button switch: 
    // (-1)=nothing (should never be), 0=trees (greens), 1=buildings (yellows)
    if(contextSwitch->whichChild.getValue() != category)
        contextSwitch->whichChild = category;

    offset->translation = closestPoint; //set crosshair to penetration point
    label->string = idString;           //set text to show name of model
}

//-----------------------------------------------------------------------------
// setup all nodes and buttons for the context sensitive model

void
setupContext()
{
    AVGETNODE(offset,SoTranslation,"CROSS_HAIR_OFFSET"); //crossh. from center
    AVGETNODE(label,SoText3,"CROSS_HAIR_TEXT");          //name of artifact
    AVGETNODE(contextSwitch,SoSwitch,"CONTEXT_SWITCH");  //switch for visibil.
    AVGETNODE(buildingSwitch,SoSwitch,"BUILDING_SWITCH");//buildings
    AVGETNODE(treeSwitch,SoSwitch,"TREE_SWITCH");        //trees
    AVGETNODE(pipCoord,SoCoordinate3,"PIP_COORD");       //pip polygon

    // green buttons for trees
    So3DButton* b;
    AVGETNODE(b,So3DButton,"GREEN00_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"GREEN02_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"GREEN04_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"GREEN06_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"GREEN08_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"GREEN10_BUTTON");
    b->addPressCallback(cbContextButtonPress);

    // yellow buttons for buildings
    AVGETNODE(b,So3DButton,"YELLOW00_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"YELLOW02_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"YELLOW04_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"YELLOW06_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"YELLOW08_BUTTON");
    b->addPressCallback(cbContextButtonPress);
    AVGETNODE(b,So3DButton,"YELLOW10_BUTTON");
    b->addPressCallback(cbContextButtonPress);
}

//-----------------------------------------------------------------------------
//eof
