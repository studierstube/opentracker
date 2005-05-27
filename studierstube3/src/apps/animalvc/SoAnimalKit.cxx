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
#include <iostream.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>
#include "SoAnimalKit.h"

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
    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoAnimalKit::~SoAnimalKit()
{

}

//-----------------------------------------------------------------------

void
SoAnimalKit::resetButtonCB(void* data, So3DButton* button)
{
  /*
    SoAnimalKit* self = (SoAnimalKit*)data;
	uint32_t * colordata = ((SoVertexProperty*)self->ts->vertexProperty.getValue())->orderedRGBA.startEditing();
    for(int i=0; i<self->vp->vertex.getNum(); i++)
		colordata[i] = self->packed.getValue();
    ((SoVertexProperty*)self->ts->vertexProperty.getValue())->orderedRGBA.finishEditing();
  */
}

//-----------------------------------------------------------------------

void
SoAnimalKit::colorButtonCB(void* data, So3DButton* button)
{
    SoAnimalKit* self = (SoAnimalKit*)data;
    self->ballMat->diffuseColor.setValue(button->buttonColor.getValue());
    self->packed.setValue(self->ballMat->diffuseColor[0].getPackedValue());
}

//-----------------------------------------------------------------------

void
SoAnimalKit::colorWidgetCB(void* data, SoVCWidgetKit* widgetKit)
{
    SoAnimalKit* self = (SoAnimalKit*)data;
    
    SoSwitch* contSwitch = widgetKit->getContent();
    if(contSwitch != NULL)
      {
	cerr << "&&& ANIMAL &&& contSwitch != NULL)" << endl;
	contSwitch->ref();
	
	SoSearchAction sa;
	sa.setSearchingAll(TRUE);
	sa.setType(SoMaterial::getClassTypeId());
	sa.apply(contSwitch);
	SoFullPath* path = (SoFullPath*)sa.getPath();
	assert(path != NULL);
	
	SoMaterial* contMat = (SoMaterial*)path->getTail();
	float c1, c2, c3;

	SbColor* colMat = new SbColor();
	//	colMat->setPackedValue(contMat->diffuseColor[0].getPackedValue());
	SbVec3f col(contMat->diffuseColor[0][0],
		    contMat->diffuseColor[0][1],
		    contMat->diffuseColor[0][2]);
	
/*
	SbColor colMat((contMat->diffuseColor[0])[0],
		       (contMat->diffuseColor[0])[1],
		       (contMat->diffuseColor[0])[2]);
*/

	cerr << "&&& ANIMAL &&& color set = " << col[0] << ", " << col[1] << ", " << col[2] << endl;
	self->ballMat->diffuseColor.setValue(col[0], col[1], col[2]);
	self->packed.setValue(self->ballMat->diffuseColor[0].getPackedValue());
      }
    else
      	cerr << "&&& ANIMAL &&& contSwitch == NULL)" << endl;
}
    
//-----------------------------------------------------------------------

SbBool
SoAnimalKit::windowEventCB(void* data, int message, SoWindowKit* window, 
			   int userID, So3DEvent* event, SoPath* path) 
{
    if (message != WM_EVENT) return FALSE;

    if (!(event->getButton(So3DEvent::BUTTON0)
	  && (event->getType() == So3DEvent::ET_BUTTON0_EVENT
	      ||  event->getType() == So3DEvent::ET_MOVE_EVENT))) return FALSE;

    printf("Animal: WindowEvent\n");    
    SbVec3f eventPos;
    
    SbViewportRegion view;
    SoGetMatrixAction ma(view);
    path->ref();
    ma.apply((SoFullPath*)path);
    path->unrefNoDelete();
    SbMatrix globalTransform = ma.getInverse();
    
    globalTransform.multVecMatrix( event->getTranslation(), eventPos);
    
    if (transi)
      eventPos -= transi->translation.getValue();
    
    for(int i=0; i<vp->vertex.getNum(); i++)
      {
	if((eventPos-vp->vertex[i]).length() < 0.03)
	  ((SoVertexProperty*)ts->vertexProperty.getValue())->orderedRGBA.set1Value(i,packed.getValue());
      }
    ballTrafo->translation = eventPos;
    return TRUE;
}

//----------------------------------------------------------------------------
SbBool
SoAnimalKit::checkPipGeometry()
{
    return TRUE;
}

SbBool
SoAnimalKit::checkMainPanelGeometry()
{
  return true;
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

        windowKit->style = 0x0|
 	  SoWindowKit::CLIENTVOLUMEVISIBLE | 
	  SoWindowKit::TITLE |
	  SoWindowKit::MOVEABLE | 
	  SoWindowKit::RESIZEABLE | 
	  SoWindowKit::VISIBLE;
//         windowKit->state = SoWindowKit::MAXIMIZED;
        
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
	SoSeparator *sepi = new SoSeparator;
	transi = new SoTransform;
	transi->translation.setValue(0,0.2,-1);
	sepi->addChild(transi);
	sepi->addChild(node);
	clientVolume->addChild(sepi);
//         clientVolume->addChild(node);
        ballTrafo = new SoTranslation;
        ballMat = new SoMaterial;
        ballMat->diffuseColor = SbColor(1,0,0);
        sph = new SoSphere;
        sepi->addChild(ballTrafo);
        sepi->addChild(ballMat);
        sepi->addChild(sph);

//         clientVolume->addChild(ballTrafo);
//         clientVolume->addChild(ballMat);
//         clientVolume->addChild(sph);
        
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
    // name += "_RESET";
    //bReset = (So3DButton*)findNode(pipSheet,name);
     //name = getName().getString();
    //name += "_SIZE";    
    //sizeSlider = (So3DSlider*)findNode(pipSheet,name);
    
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
    sph->radius = 0.03;
    //sph->radius.connectFrom(&sizeSlider->currentValue);

    if(masterMode)
    {
        for(i=0; i<36; i++) 
        {
            SbString bName = getName().getString();
			bName += "_COLOR_";
            bName += SbString(i);
            b = (So3DButton*)findNode(pipSheet,bName);
            b->addPressCallback(colorButtonCB,this);
        }
	// bReset->addPressCallback(resetButtonCB,this);
    }
    else
    {
        for(i=0; i<36; i++) 
        {
            SbString bName = getName().getString();
			bName += "_COLOR_";
            bName += SbString(i);
            b = (So3DButton*)findNode(pipSheet,bName);
            b->removePressCallback(colorButtonCB);
        }
	//        bReset->removePressCallback(resetButtonCB);
    }
}

void
SoAnimalKit::checkMainPanelMasterMode(SoNode *mainPanelContents, SbBool masterMode )
{
    printf("AnimalKit: checkMPMasterMode\n");
    SoVCWidgetKit* w;

    int i;
    SoVCWidgetKit* wReset;
    SbString name = getName().getString();
    
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


    int j;
    if(masterMode)
    {
      for(i=0; i<6; i++) 
        {
	  for(j=1; j<7; j++)
	    {
	      SbString wName = getName().getString();
	      wName += "_COLOR_";
	      wName += SbString(i);
	      wName += SbString(j);
	      //cerr << "&&& ANIMAL &&& node search string =" << wName.getString() << endl;
	      w = (SoVCWidgetKit*)findNode(mainPanelContents, wName);
	      //cerr << "&&& ANIMAL &&& node found aft CB set" << endl;
	      w->addPressCallback(colorWidgetCB, this);
	      //cerr << "&&& ANIMAL &&& node found bef CB set" << endl;
	    }
        }
    }
    else
    {
      for(i=0; i<6; i++) 
        {
	  for(j=1; j<7; j++)
	    {
	      SbString wName = getName().getString();
	      wName += "_COLOR_";
	      wName += SbString(i);
	      wName += SbString(j);
	      //cerr << "&&& ANIMAL &&& node search string =" << wName.getString() << endl;
	      w = (SoVCWidgetKit*)findNode(mainPanelContents, wName);
	      //cerr << "&&& ANIMAL &&& node found aft CB set" << endl;
	      w->removePressCallback(colorWidgetCB);
	      //cerr << "&&& ANIMAL &&& node found bef CB set" << endl;
	    }
        }
    }
}

//----------------------------------------------------------------------------
//eof

