// ===========================================================================
//  (C) 2003 Vienna University of Technology
// ===========================================================================
//  NAME:       SoMD2_ArtKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    MD2 on ARToolkit Artifact Example Application
//  VERSION:    1.0
// ===========================================================================
//  AUTHORS:    dw      Daniel Wagner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/resource/SoTrackedArtifactKit.h>
#include <stbapi/window3d/SoWindowKit.h>

#include "SoMD2_ArtKit.h"


SO_KIT_SOURCE(SoMD2_ArtKit);


//----------------------------------------------------------------------------
void
SoMD2_ArtKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoMD2_ArtKit, SoContextKit, "SoContextKit");
}


//----------------------------------------------------------------------------
SoContextKit*
SoMD2_ArtKit::factory()
{
    return new SoMD2_ArtKit();
}


//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------
SoMD2_ArtKit::SoMD2_ArtKit()
{
    SO_KIT_CONSTRUCTOR(SoMD2_ArtKit);
    SO_KIT_INIT_INSTANCE();

	md2 = NULL;
}


//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------
SoMD2_ArtKit::~SoMD2_ArtKit()
{

}


// callback when one of our pip buttons is pressed
//
void
SoMD2_ArtKit::MD2ButtonCB(void* data, So3DButton* button)
{
    SoMD2_ArtKit* self = (SoMD2_ArtKit*)data;
	SbString str = button->nodeId.getValue();

	if(self->md2)
		self->md2->animname.setValue(str);
}


SbBool
SoMD2_ArtKit::checkWindowGeometry()
{
	// somehow the application crashes if we
	// try this in the md2atart.iv
	// (this is an open stb bug waiting to be fixed...)
	//
	SoWindowKit* windowKit = (SoWindowKit*)myFindNode(windowGroup.getValue(), "MD2WINDOWKIT");
	if(windowKit)
		windowKit->style = SoWindowKit::CLIENTVOLUMEVISIBLE | SoWindowKit::VISIBLE;

    return TRUE;
}


// same as findNode but does no assert if nothing is found
//
SoNode*
SoMD2_ArtKit::myFindNode(SoNode* root, SbString name)
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


// setup pip stuff
//
void
SoMD2_ArtKit::checkPipMasterMode(SoNode* pipSheet, SbBool masterMode)
{
    printf("MeshShowKit: checkPipMasterMode\n");
    So3DButton* b;
	char str[100];

	md2 = (SoMD2NodeKit*)myFindNode(windowGroup.getValue(), "MD2NODE");

	if(md2)
	{
		So3DSlider*	speedSlider = (So3DSlider*)myFindNode(pipSheet,"ANIMSPEED");
		md2->animtime.connectFrom(&speedSlider->currentValue);

		if(masterMode)
		{
			for(int i=0; i<10; i++)
			{
				sprintf(str, "PLAY_ANIM%02d", i);				
				if((b = (So3DButton*)myFindNode(pipSheet,str)))
					b->addPressCallback(MD2ButtonCB,this);
			}
		}
		else
		{
			for(int i=0; i<10; i++)
			{
				sprintf(str, "PLAY_ANIM%02d", i);
				if((b = (So3DButton*)myFindNode(pipSheet,str)))
					b->removePressCallback(MD2ButtonCB);
			}
		}
	}
}
