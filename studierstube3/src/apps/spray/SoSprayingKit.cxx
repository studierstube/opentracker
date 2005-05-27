   /* ========================================================================
  * (C) 2002 Vienna University of Technology
  * ========================================================================
  * PROJECT: Spray
  * ======================================================================== */ 
/** Sourcefile of SoSprayingKit.
  * SoSprayingKit.cxx implements a small multi user painting application
  * for the Studierstube AR System.
  *
  * @author  Andreas Zajic, Dieter Schmalstieg, Anton Fuhrmann, Hermann Wurnig
  *
  * $Id: SoSprayingKit.cxx 3781 2004-09-28 11:19:51Z reitmayr $
  * @file  SoSprayingKit.cxx                                                */
 /* ======================================================================= */

#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <stbapi/util/os.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/widgets/So3DSlider.h>
#include <stbapi/widgets/So3DCheckBox.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/window3d/SoWindowKit.h>

#include "SoSprayingKit.h"


SO_KIT_SOURCE(SoSprayingKit);


const int DRAWING_MODE  = 0;
const int SPRAYING_MODE = 1;


// static class initialization

void
SoSprayingKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoSprayingKit, SoContextKit, "SoContextKit");
}


// constructor

SoSprayingKit::SoSprayingKit()
:
mesh(NULL),
vp(NULL)
{
    SO_KIT_CONSTRUCTOR(SoSprayingKit);
    SO_KIT_ADD_FIELD(voxelField, (0));
    SO_KIT_ADD_FIELD(templateMesh, (NULL));
    SO_KIT_INIT_INSTANCE();
}


// virtual constructor

SoContextKit*
SoSprayingKit::factory()
{
    return new SoSprayingKit();
}


// destructor

SoSprayingKit::~SoSprayingKit()
{
}


// add a voxel to the scene

void
SoSprayingKit::drawVoxel(SbVec3f position,
                         SbColor color,
                         float size)
{
    int i, voxels;

    if(getDivObject()->getDiv())
        getDivObject()->getDiv()->atomicActionBegin();

	voxels=voxelField.getValue()+1;
    voxelField.setValue(voxels);
    int triNum = ((SoIndexedTriangleStripSet*)templateMesh.getValue())->coordIndex.getNum();
    int triBase = (voxels-1)*triNum;

//	mesh->enableNotify(FALSE);
//--copy vertices
    SoVertexProperty* templateVP;
    templateVP = (SoVertexProperty*)((SoIndexedTriangleStripSet*)templateMesh.getValue())->vertexProperty.getValue();

    int vertexBase = (voxels-1)*8;

    vp->vertex.enableNotify(FALSE);
    vp->vertex.setNum(vertexBase+8);
    vp->vertex.enableNotify(TRUE);

    for(i=0; i<8; i++)
		((SoVertexProperty*)mesh->vertexProperty.getValue())->vertex.set1Value(vertexBase+i,
        templateVP->vertex[i] * size + position);
   
	//--create color
    int colBase = voxels;
    ((SoVertexProperty*)mesh->vertexProperty.getValue())->orderedRGBA.set1Value(colBase-1,color.getPackedValue());

    //--copy coords
	mesh->coordIndex.enableNotify(FALSE);
	mesh->materialIndex.enableNotify(FALSE);
    mesh->coordIndex.setNum(triBase+triNum);
    mesh->materialIndex.setNum(triBase+triNum);
	mesh->coordIndex.enableNotify(TRUE);
	mesh->materialIndex.enableNotify(TRUE);

    for(i=0; i<triNum; i++)
    {
        int idx = ((SoIndexedTriangleStripSet*)templateMesh.getValue())->coordIndex[i];

        mesh->coordIndex.set1Value(triBase+i, idx == -1 ? -1 : idx+vertexBase);
		mesh->materialIndex.set1Value(triBase+i, colBase-1);
    }

    if(getDivObject()->getDiv())
        getDivObject()->getDiv()->atomicActionEnd();
}


// process 3d events and start spraying

SbBool
SoSprayingKit::windowEventCB(void* data, int message, SoWindowKit* window,
    int uid, So3DEvent* event, SoPath* rootPath)
{
    if (message != WM_EVENT || uid == -1) return FALSE;
    
    SbBool pressEv = (event->getType() == So3DEvent::ET_BUTTON0_EVENT)
                     && (event->getButton(So3DEvent::BUTTON0) == TRUE);
    SbBool moveEv  = (event->getType() == So3DEvent::ET_MOVE_EVENT)
                     && (event->getButton(So3DEvent::BUTTON0) == TRUE);
    if(!pressEv && !moveEv) return FALSE;

    SbVec3f position; // get current position
    window->orientation.getValue().inverse().multVec(
        event->getTranslation()-window->position.getValue(), position);               

    spray(position, uid); // perform actual spraying work
    return TRUE;
}


// do the spraying

void
SoSprayingKit::spray(SbVec3f position, int uid)
{
    SoNode* sheet = getPipSheet(uid);
    So3DCheckBox* modeButton;
    modeButton = (So3DCheckBox*)findNode(sheet,"SPRAY_MODE_BUTTON");

    SoMaterial* mat = (SoMaterial*)findNode(sheet,"SPRAY_COLOR");
    SbColor color = mat->diffuseColor[0];

    So3DSlider* sizeSlider;
    sizeSlider = (So3DSlider*)findNode(sheet,"SPRAY_SIZE_SLIDER");
    float size = sizeSlider->currentValue.getValue();

    if(modeButton->pressed.getValue() == SPRAYING_MODE)
    {
        So3DSlider* radiusSlider;
        radiusSlider = (So3DSlider*)findNode(sheet,"SPRAY_RADIUS_SLIDER");
        SbVec3f randomVec((float)(osRand()-0.5),(float)(osRand()-0.5),(float)(osRand()-0.5));
        position += randomVec*radiusSlider->currentValue.getValue();
        drawVoxel(position,color,size);
    }
    else if(modeButton->pressed.getValue() == DRAWING_MODE)
    {
        SoTranslation* lastDrawingPos;
        lastDrawingPos = (SoTranslation*)findNode(sheet,"LAST_DRAWING_POS");
        SbVec3f last = lastDrawingPos->translation.getValue();
        float offset = (position-last).length();
        if(offset > size) // if far enough from last voxel
        {
            drawVoxel(position,color,size);
            lastDrawingPos->translation = position;
        }
    }
}


// Clearbutton callback - resets the scene

void
SoSprayingKit::clearButtonCB(void* data, So3DButton* button)
{
    SoSprayingKit* self = (SoSprayingKit*)data;
    self->mesh->coordIndex.setNum(1);
    self->mesh->materialIndex.setNum(1);
    self->vp->vertex.setNum(1);
    self->vp->orderedRGBA.setNum(1);
	self->voxelField.setValue(0);
}


// 

SbBool
SoSprayingKit::checkPipGeometry()
{
    return TRUE;
}


// create a window if none is defined in the loaderfile
    
SbBool
SoSprayingKit::checkWindowGeometry()
{
    SoGroup *wGroup = (SoGroup*)windowGroup.getValue();
    SoWindowKit *windowKit;
    if (wGroup->getNumChildren() == 0)
    {
        // create new Window

        windowKit = new SoWindowKit;

        windowKit->size.setValue(0.5, 0.5, 0.5);

        SoSeparator* clientVolume = windowKit->getClientVolume();
        vp = new SoVertexProperty;
        vp->materialBinding = SoVertexProperty::PER_VERTEX_INDEXED;
        mesh = new SoIndexedTriangleStripSet;
        mesh->vertexProperty = vp;
        clientVolume->addChild(mesh);
        voxelField.setValue(0);
    }
    else
    {
        // from .iv file
        windowKit = (SoWindowKit*)wGroup->getChild(0);
        SoSeparator* clientVolume= windowKit->getClientVolume();

        mesh = (SoIndexedTriangleStripSet*)(clientVolume->getChild(0));
        vp = (SoVertexProperty*)(mesh->vertexProperty.getValue());
    }
    return TRUE;
}



// Activate Clear button when application enters mastermode.

void
SoSprayingKit::checkPipMasterMode(SoNode* pipSheet,
                                  SbBool masterMode)
{

    So3DButton* clearButton;

    clearButton = (So3DButton*)findNode(pipSheet,"SPRAY_CLEAR_BUTTON");

    if(masterMode)
    {
        clearButton->addReleaseCallback(clearButtonCB,this);
    }
    else
    {
        clearButton->removeReleaseCallback(clearButtonCB);
    }
}

//----------------------------------------------------------------------------
//eof

