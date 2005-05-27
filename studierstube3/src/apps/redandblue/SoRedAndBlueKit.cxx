// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoRedAndBlueKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    "Red and Blue" Demo
//  VERSION:    3.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg, zaa Andreas Zajic
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <Inventor/nodes/SoSphere.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include "SoRedAndBlueKit.h"
#include <stbapi/workspace/StbWinManager.h>
#include <stbapi/workspace/StbMsgManager.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoRedAndBlueKit);

//--initialization of context in Studierstube
//CONTEXT_APPLICATION(SoRedAndBlueKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void
SoRedAndBlueKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoRedAndBlueKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// virtual constructor
//----------------------------------------------------------------------------

SoContextKit*
SoRedAndBlueKit::factory()
{
    return new SoRedAndBlueKit;
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoRedAndBlueKit::SoRedAndBlueKit()
{
    SO_KIT_CONSTRUCTOR(SoRedAndBlueKit);
    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoRedAndBlueKit::~SoRedAndBlueKit()
{
    printf("RedAndBlue Destructor!\n");
    //destructor();
    printf("SoRedAndBlue destructed\n");
}

//----------------------------------------------------------------------------
// Callback function called when a color button was pressed
//----------------------------------------------------------------------------

void 
SoRedAndBlueKit::colorButtonCB(void* data, So3DButton* button)
{
//--this is a static function so we don't know which instance
//--thus pointer to context is passed in as <data> and we must cast it
    SoRedAndBlueKit* self = (SoRedAndBlueKit*)data;

    //char buffer[100];
    //sprintf(buffer,"SAVE_BUTTON_%d_%s",self->userID.getValue(),self->getName().getString());
    SbName bname("SAVE_BUTTON");
    if (button->getName()==bname) {
        printf("save button pressed\n");
        SoApplicationKit* appKit=SoContextManagerKit::getInstance()->getApp(self->appID.getValue());
        writeFile("rbappsave.iv",appKit);
        writeFile("userkitsave.iv", StbWorkspace::getInstance()->getUserKit(self->userID.getValue()));
        //((SoWindowKit*)self->windows[0])->setPosition(SbVec3f(1.0,0.0,0.0));
        return;
    }
//--get pointer to window content
    SoSeparator* winRoot = self->getWindow(0)->getClientVolumeSeparator();
//--child 0 of window content is the material we want to change
    SoMaterial* mat = (SoMaterial*)winRoot->getChild(0);
//--now copy color from button to the scene material
    mat->diffuseColor.setValue(button->buttonColor.getValue());
}


//----------------------------------------------------------------------------
// checkPip
//----------------------------------------------------------------------------

SbBool
SoRedAndBlueKit::checkPipGeometry()
{


    return TRUE;
}

SbBool
SoRedAndBlueKit::checkWindowGeometry()
{
    // no windows saved in the field: create new window for R&B
	return TRUE;
    if (((SoGroup*)windowGroup.getValue())->getNumChildren()==0)
	{
        //--create a default window
        WindowCreate wc;
        SoWindowKit::defaultWindow(wc);

        SoWindowKit* windowKit =StbWinManager::getInstance()->createWindow(appID.getValue(), NULL,
                                                                &wc, NULL, NULL);
        windowKit->size = SbVec3f(0.3,0.3,0.3);

    //--build scene graph composed of a material and a sphere node
    //--and place it window's client area

        SoSeparator* clientVolume = windowKit->getClientVolumeSeparator();
        clientVolume->addChild(new SoMaterial);
        SoSphere* sph = new SoSphere;
        sph->radius = 0.1;
        clientVolume->addChild(sph);
    
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
//		    windows.setValue(windowKit);
            ((SoGroup*)windowGroup.getValue())->addChild(windowKit);
            return TRUE;
		}
        return FALSE;
	}
    return TRUE;
}
//----------------------------------------------------------------------------

void
SoRedAndBlueKit::setSheetMasterMode(SoNode* pipSheet, SbBool masterMode)
{
//--find pointers to the red and blue color buttons
	char buffer[100];
    int uid=getUserFromSheet(pipSheet);
	sprintf(buffer,"RED_BUTTON");
    So3DButton* bRed  = (So3DButton*)findNode(pipSheet,buffer);
	sprintf(buffer,"BLUE_BUTTON");
    So3DButton* bBlue = (So3DButton*)findNode(pipSheet,buffer);
	sprintf(buffer,"SAVE_BUTTON");
    So3DButton* bSave = (So3DButton*)findNode(pipSheet,buffer);
    if(masterMode) // if context is master -> set callbacks
    {
        bRed->addReleaseCallback(colorButtonCB,this);
        bBlue->addReleaseCallback(colorButtonCB,this);
        bSave->addReleaseCallback(colorButtonCB,this);
    }
    else // if context is slave -> remove callbacks
    {
        bRed->removeReleaseCallback(colorButtonCB);
        bBlue->removeReleaseCallback(colorButtonCB);
        bSave->removeReleaseCallback(colorButtonCB);
    }
}

//----------------------------------------------------------------------------
//eof

