// ===========================================================================
//  (C) 2002 Vienna University of Technology
// ===========================================================================
//  NAME:       SoWormsKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Small simple game...
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    dw      Daniel Wagner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <assert.h>

#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/misc/ivmacros.h>
#include <stbapi/resource/SoTrackedArtifactKit.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/util/ivio.h>

#include "SoWormsKit.h"


SO_KIT_SOURCE(SoWormsKit);


//----------------------------------------------------------------------------

void
SoWormsKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoWormsKit, SoContextKit, "SoContextKit");

	//SoMD2NodeKit::initClass();
	SoTerrain::initClass();
}

//----------------------------------------------------------------------------

SoContextKit*
SoWormsKit::factory()
{
    return new SoWormsKit();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoWormsKit::SoWormsKit()
{
    SO_KIT_CONSTRUCTOR(SoWormsKit);
    SO_KIT_INIT_INSTANCE();

	theScene = NULL;
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoWormsKit::~SoWormsKit()
{

}

//-----------------------------------------------------------------------




void
SoWormsKit::MD2ButtonCB(void* data, So3DButton* button)
{
    SoWormsKit* self = (SoWormsKit*)data;
	SbString str = button->nodeId.getValue();

	//if(self->md2NodeKit)
	//	self->md2NodeKit->animname.setValue(str);
}



//-----------------------------------------------------------------------

SbBool
SoWormsKit::windowEventCB(void* data, int message, SoWindowKit* window, 
    int userID, So3DEvent* event, SoPath* path) 
{
    if (message != WM_EVENT) return FALSE;
    if (!(event->getButton(So3DEvent::BUTTON0)
        && (event->getType() == So3DEvent::ET_BUTTON0_EVENT
        ||  event->getType() == So3DEvent::ET_MOVE_EVENT))) return FALSE;

	SbVec3f eventPos;

	SbViewportRegion view;
    SoGetMatrixAction ma(view);
	path->ref();
    ma.apply((SoFullPath*)path);
	path->unrefNoDelete();
    SbMatrix globalTransform = ma.getInverse();

	globalTransform.multVecMatrix( event->getTranslation(), eventPos);

    return TRUE;
}

//----------------------------------------------------------------------------
SbBool
SoWormsKit::checkPipGeometry()
{
    return TRUE;
}

//----------------------------------------------------------------------------

SbBool
SoWormsKit::checkWindowGeometry()
{
    // no windows saved in the field: create new window for R&B    
    if (getNumWindows() == 0)
    {
        printf("MeshShow: creating a new window\n");
        
        SoWindowKit* windowKit = new SoWindowKit;
        windowKit->size = SbVec3f(1.2,1.2,1.2);
		//windowKit->size = SbVec3f(10,10,10);
		//windowKit->c
        windowKit->position = SbVec3f( 0, 0, 0 );
		//windowKit->style = SoWindowKit::CLIENTVOLUMEVISIBLE | SoWindowKit::VISIBLE;
		//windowKit->appearance = 0;

        // windowKit->orientation = SbRotation( SbVec3f(1.0 ,0,0), 1.5707963);
        
        SoSeparator* clientVolume = windowKit->getClientVolume();
        //--setup window content

		/*md2NodeKit = new SoMD2NodeKit;
		md2NodeKit->meshname.setValue("../apps/meshshow/Ryla.md2");
		md2NodeKit->skinname.setValue("../apps/meshshow/Ryla.jpg");
		md2NodeKit->position.setValue(-0.2, 0, -0.2);
		//md2NodeKit->orientation.setValue(1, 0, 0, 1.2);
		md2NodeKit->animname.setValue("run");
		md2NodeKit->animtime.setValue(1.5);
		clientVolume->addChild(md2NodeKit);*/



		theScene = new SoWormsScene;
		clientVolume->addChild(theScene);

		/*terrain = new SoTerrain;
		terrain->textureName = "../apps/worms/data/TerrainTexSmall.jpg";
		terrain->heightMapName = "../apps/worms/data/Heightmap.raw";
		terrain->position = SbVec3f(0, -0.4, 0);
		clientVolume->addChild(terrain);*/

		/*md2 = new SoMD2;
		md2->meshname.setValue("../apps/meshshow/Ryla.md2");
		md2->skinname.setValue("../apps/meshshow/Ryla.jpg");
		//md2->position.setValue(-0.2, 0, -0.2);
		md2->orientation.setValue(1, 0, 0, 1.2);
		md2->animname.setValue("run");
		md2->animtime.setValue(1.5);
		clientVolume->addChild(md2);*/


		/*SoGroup* group = new SoGroup;
		group->addChild(md2);


        SoTrackedArtifactKit* trackedArtifact = new SoTrackedArtifactKit;
		trackedArtifact->station = 6;
		trackedArtifact->setGeometry(group);
		clientVolume->addChild(trackedArtifact);*/


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



// myFindNode makes no assert (in contrast to findNode)
//
SoNode*
myFindNode(SoNode* root, SbString name)
{
    SoSearchAction sa;
	sa.setSearchingAll(TRUE);
    sa.setName(name);
    sa.apply(root);
    SoFullPath* path = (SoFullPath*)sa.getPath();
    if(path==NULL)
		return NULL;
    return path->getTail();
}


// master/slave mode for pip sheet
//
void
SoWormsKit::checkPipMasterMode(SoNode* pipSheet, SbBool masterMode)
{
    printf("MeshShowKit: checkPipMasterMode\n");

	if(theScene)
		theScene->setupButtonCallbacks(pipSheet, masterMode);

    /*So3DButton* b;
	char str[100];

	if(md2NodeKit)
	{
		speedSlider = (So3DSlider*)findNode(pipSheet,"ANIMSPEED");
		md2NodeKit->animtime.connectFrom(&speedSlider->currentValue);

		if(masterMode)
		{
			for(int i=0; i<10; i++)
			{
				sprintf(str, "PLAY_ANIM%02d", i);
				if(b = (So3DButton*)myFindNode(pipSheet,str))
					b->addPressCallback(MD2ButtonCB,this);
			}
		}
		else
		{
			for(int i=0; i<10; i++)
			{
				sprintf(str, "PLAY_ANIM%02d", i);
				if(b = (So3DButton*)myFindNode(pipSheet,str))
					b->removePressCallback(MD2ButtonCB);
			}
		}
	}*/
}
