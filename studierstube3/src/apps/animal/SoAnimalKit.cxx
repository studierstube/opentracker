// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoAnimalKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Demo application for workspace - multi user app
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <assert.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>
#include "SoAnimalKit.h"

#include <stbapi/workspace/StbWorkspace.h>


SO_KIT_SOURCE(SoAnimalKit);

SoSeparator* SoAnimalKit::sheet = NULL;
SoAnimalKit* SoAnimalKit::animalFocus = NULL;

//----------------------------------------------------------------------------
void
SoAnimalKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoAnimalKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------

SoContextKit*
SoAnimalKit::factory()
{
    return new SoAnimalKit();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoAnimalKit::SoAnimalKit()
:
ts(NULL),
vp(NULL),
ballTrafo(NULL),
ballMat(NULL),
sph(NULL)

{
    SO_KIT_CONSTRUCTOR(SoAnimalKit);

    SO_KIT_ADD_FIELD(packed, (0));
    SO_KIT_ADD_FIELD(currentMaterial, (NULL));
    SO_KIT_ADD_FIELD(currentColorR, (1));
    SO_KIT_ADD_FIELD(currentColorG, (0));
    SO_KIT_ADD_FIELD(currentColorB, (0));
	SO_KIT_ADD_FIELD(reset,			());

    SO_KIT_INIT_INSTANCE();
	isPainting = FALSE;
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoAnimalKit::~SoAnimalKit()
{

}

//-----------------------------------------------------------------------

void SoAnimalKit::resetCB(void *data, SoSensor *)
{
	resetButtonCB(data, NULL);
}

//-----------------------------------------------------------------------

void
SoAnimalKit::resetButtonCB(void* data, So3DButton* )
{
    SoAnimalKit* self = (SoAnimalKit*)data;
	uint32_t * colordata = ((SoVertexProperty*)self->ts->vertexProperty.getValue())->orderedRGBA.startEditing();
    for(int i=0; i<self->vp->vertex.getNum(); i++)
		colordata[i] = self->packed.getValue();
    ((SoVertexProperty*)self->ts->vertexProperty.getValue())->orderedRGBA.finishEditing();
}

//-----------------------------------------------------------------------

void SoAnimalKit::colorCB(void *data, SoSensor *) 
{
    SoAnimalKit* self = (SoAnimalKit*)data;
	SbColor color(self->currentColorR.getValue(), self->currentColorG.getValue(), self->currentColorB.getValue());

    self->packed.setValue(color.getPackedValue());
	self->ballMat->diffuseColor.setValue(color);

}


void
SoAnimalKit::colorButtonCB(void* data, So3DButton* button)
{
    SoAnimalKit* self = (SoAnimalKit*)data;
	self->changeColor(button->buttonColor.getValue());
}

//-----------------------------------------------------------------------

void SoAnimalKit::changeColor(const SbColor& color) 
{
	float r,g,b;
	color.getValue(r,g,b);
//	ballMat->diffuseColor.setValue(color);
	currentColorR.setValue(r);
	currentColorG.setValue(g);
	currentColorB.setValue(b);
//    packed.setValue(ballMat->diffuseColor[0].getPackedValue());
	//	will be called by sensor !!
}


//-----------------------------------------------------------------------

SbBool
SoAnimalKit::windowEventCB(void* data, int message, SoWindowKit* window, 
    int userID, So3DEvent* event, SoPath* path) 
{
	// if I am a slave dont do anything !!
//	if (!getMasterMode()) return FALSE;

    if (message != WM_EVENT) return FALSE;
    if (!(event->getButton(So3DEvent::BUTTON0)
        && (event->getType() == So3DEvent::ET_BUTTON0_EVENT
        ||  event->getType() == So3DEvent::ET_MOVE_EVENT))) {
		
		if (isPainting) 
		{
			ENABLEDIVFIELD((ts->vertexProperty));
	//		printf(" ################enabled\n");
			ts->vertexProperty.touch();
/*			uint32_t temp = ((SoVertexProperty*)ts->vertexProperty.getValue())->orderedRGBA[0];
			((SoVertexProperty*)ts->vertexProperty.getValue())->orderedRGBA.set1Value(0,temp);
*/
		}
		isPainting = false;
		return FALSE;
		
	}
/*
    if(animalFocus != this)
    {
        animalFocus = this;
        forceFocusChange();
    }
*/    
	if (!isPainting) {
		DISABLEDIVFIELD((ts->vertexProperty));
	//	printf("################disabled\n");
	}

	isPainting = TRUE;

	SbVec3f eventPos;

	SbViewportRegion view;
    SoGetMatrixAction ma(view);
	path->ref();
    ma.apply((SoFullPath*)path);
	path->unrefNoDelete();
    SbMatrix globalTransform = ma.getInverse();

    SbRotation so;
    SbVec3f trans = window->position.getValue();
    SbVec3f s(1,1,1);
    SbRotation rot = window->orientation.getValue();
    SbMatrix localTransform;
    localTransform.setTransform( trans, rot, s, so );
    globalTransform.multLeft(localTransform.inverse());

	globalTransform.multVecMatrix( event->getTranslation(), eventPos);

	for(int i=0; i<vp->vertex.getNum(); i++)
    {
        if((eventPos - vp->vertex[i]).length()
            < sizeSlider->currentValue.getValue())
			((SoVertexProperty*)ts->vertexProperty.getValue())->orderedRGBA.set1Value(i,packed.getValue());
    }
    ballTrafo->translation = eventPos;
//	printf("changed colors \n");
    return TRUE;
}

//----------------------------------------------------------------------------
SbBool
SoAnimalKit::checkPipGeometry()
{
    return TRUE;
}

//----------------------------------------------------------------------------

SbBool
SoAnimalKit::checkWindowGeometry()
{
    // no windows saved in the field: create new window for R&B    
    if (getNumWindows() == 0)
    {
        printf("Animal: creating a new window\n");

        SoWindowKit* windowKit = new SoWindowKit;
        windowKit->size = SbVec3f(0.5,0.5,0.5);
        windowKit->position = SbVec3f( 0, 0, 0 );
        // windowKit->orientation = SbRotation( SbVec3f(1.0 ,0,0), 1.5707963);
        
        SoSeparator* clientVolume = windowKit->getClientVolume();
        //--setup window content
        SoSeparator* node = readFile("../apps/animal/cow.iv");
        assert(node);
        //SoIndexedFaceSet* mesh = (SoIndexedFaceSet*)node;
        GETCHILD(mesh,SoIndexedTriangleStripSet,node,1);
        assert(mesh->isOfType(SoIndexedTriangleStripSet::getClassTypeId()));        
        ts=mesh;
        vp = (SoVertexProperty*)mesh->vertexProperty.getValue();
        assert(vp);
        clientVolume->addChild(node);
        ballTrafo = new SoTranslation;
        ballMat = new SoMaterial;
        ballMat->diffuseColor = SbColor(1,0,0);
        sph = new SoSphere;
        clientVolume->addChild(ballTrafo);
        clientVolume->addChild(ballMat);
        clientVolume->addChild(sph);
        
        packed.setValue(ballMat->diffuseColor[0].getPackedValue());
        vp->setName("VP");
        setUniqueNodeName(vp,0);
        ts->setName("TS");
        setUniqueNodeName(ts,0);
        sph->setName("SPH");
        setUniqueNodeName(sph,0);
        ballTrafo->setName("BT");
        setUniqueNodeName(ballTrafo,0);
        ballMat->setName("BM");
        setUniqueNodeName(ballMat,0);    
        if(windowKit != NULL) 
        {
            if((windowKit->getName().getString()==NULL)||!windowKit->getName())
            {
                SbString windowString;
                windowString+=getName().getString();
                windowString+="_WINDOW_";
                windowString+="0";
                windowKit->setName(windowString);
            }
            ((SoGroup*)windowGroup.getValue())->addChild(windowKit);
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}



//----------------------------------------------------------------------------

void
SoAnimalKit::checkPipMasterMode(SoNode* pipSheet, SbBool masterMode)
{
    printf("AnimalKit: checkPipMasterMode\n");
    So3DButton* b;
    int i;
    So3DButton* bReset;
	SbString name = getName().getString();
	name += "_RESET";
    bReset = (So3DButton*)findNode(pipSheet,name);
	name = getName().getString();
	name += "_SIZE";    
    sizeSlider = (So3DSlider*)findNode(pipSheet,name);
    
	if(sph == NULL)
		sph = (SoSphere*)SoNode::getByName(makeUniqueNodeName("SPH",0));
	if(ts == NULL)
		ts = (SoIndexedTriangleStripSet*)SoNode::getByName(makeUniqueNodeName("TS",0));
	if(vp == NULL)
		vp = (SoVertexProperty*)SoNode::getByName(makeUniqueNodeName("VP",0));
	if(ballTrafo == NULL)
		ballTrafo = (SoTranslation*)SoNode::getByName(makeUniqueNodeName("BT",0));
	if(ballMat == NULL)
		ballMat = (SoMaterial*)SoNode::getByName(makeUniqueNodeName("BM",0));

	packed.setValue(ballMat->diffuseColor[0].getPackedValue());        
    sph->radius.connectFrom(&sizeSlider->currentValue);

	currentMaterial.setValue(ballMat);

    if(masterMode)
    {
        for(i=0; i<8; i++) 
        {
            SbString bName = getName().getString();
			bName += "_COLOR_";
            bName += SbString(i);
            b = (So3DButton*)findNode(pipSheet,bName);
            b->addPressCallback(colorButtonCB,this);
        }
        bReset->addPressCallback(resetButtonCB,this);
		resetSensor.setData(this);
		resetSensor.setFunction(resetCB);
		resetSensor.attach(&reset);

		colorSensorR.setData(this);
		colorSensorR.setFunction(colorCB);
		colorSensorR.attach(&currentColorR);
		colorSensorG.setData(this);
		colorSensorG.setFunction(colorCB);
		colorSensorG.attach(&currentColorG);
		colorSensorB.setData(this);
		colorSensorB.setFunction(colorCB);
		colorSensorB.attach(&currentColorB);

    
	}
    else
    {
        for(i=0; i<8; i++) 
        {
            SbString bName = getName().getString();
			bName += "_COLOR_";
            bName += SbString(i);
            b = (So3DButton*)findNode(pipSheet,bName);
            b->removePressCallback(colorButtonCB);
        }
        bReset->removePressCallback(resetButtonCB);
    }
}

//----------------------------------------------------------------------------
//eof

