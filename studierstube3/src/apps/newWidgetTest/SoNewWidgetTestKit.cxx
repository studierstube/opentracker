// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoNewWidgetTestKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    "Red and Blue" Demo
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <Inventor/nodes/SoSphere.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/util/ivio.h>
#include <stbapi/workspace/SoContextKitSub.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/interaction/SoDragKit.h>
#include "SoNewWidgetTestKit.h"

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoNewWidgetTestKit);

//--initialization of context in Studierstube
CONTEXT_APPLICATION(SoNewWidgetTestKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void
SoNewWidgetTestKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoNewWidgetTestKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// virtual constructor
//----------------------------------------------------------------------------

SoContextKit*
SoNewWidgetTestKit::factory()
{
    return new SoNewWidgetTestKit;
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoNewWidgetTestKit::SoNewWidgetTestKit()
{
    SO_KIT_CONSTRUCTOR(SoNewWidgetTestKit);
    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoNewWidgetTestKit::~SoNewWidgetTestKit()
{
    destructor();
}

//----------------------------------------------------------------------------
// Callback function called when a color button was pressed
//----------------------------------------------------------------------------

void 
SoNewWidgetTestKit::colorButtonCB(void* data, So3DButton* button)
{
//--this is a static function so we don't know which instance
//--thus pointer to context is passed in as <data> and we must cast it
    SoNewWidgetTestKit* self = (SoNewWidgetTestKit*)data;
//--get pointer to window content
    SoSeparator* winRoot = self->getWindow(0)->getClientVolumeSeparator();
//--child 0 of window content is the material we want to change
    SoMaterial* mat = (SoMaterial*)winRoot->getChild(0);
//--now copy color from button to the scene material
    mat->diffuseColor.setValue(button->buttonColor.getValue());
}

//----------------------------------------------------------------------------
// Reads the pip sheet and sets the button callbacks
//----------------------------------------------------------------------------

SoNode*
SoNewWidgetTestKit::createPipSheet(int uid)
{
//--read pip sheet from file "sheet.iv"
	//char buffer[100];
    //comm->setContextAliasName(getName(),"R&B");
    //SoSeparator *newPipSheet = readFile("sheet.iv",comm->workingDirectory);
	SoNode *newPipSheet = readNodeFromFile("sheet.iv",comm->workingDirectory);
	newPipSheet->ref();
	/*
    So3DButton* bRed  = (So3DButton*)findNode(newPipSheet,"RED_BUTTON");
	sprintf(buffer,"RED_BUTTON_%d_%s", uid,getName().getString());
	bRed->setName(buffer);
    SoPushButton* bBlue = (SoPushButton*)findNode(newPipSheet,"BLUE_BUTTON");
	sprintf(buffer,"BLUE_BUTTON_%d_%s", uid,getName().getString());
	bBlue->setName(buffer);
	*/
	newPipSheet->unrefNoDelete();
	return newPipSheet;
}

//----------------------------------------------------------------------------
// createWindowGeometry
// create window and attach scene graph to it. scene graph containts
// a sphere the color of which can be set by buttons on pip
//----------------------------------------------------------------------------

SoWindowKit*
SoNewWidgetTestKit::createWindowGeometry(int index)
{
	//create a default window
    /*WindowCreate wc;
    SoWindowKit::defaultWindow(wc);
    SoWindowKit* windowKit = comm->createWindow(NULL, &wc, NULL, NULL);
    windowKit->size = SbVec3f(0.3,0.3,0.3);

    SoSeparator* clientVolume = windowKit->getClientVolumeSeparator();

	// add the material
    clientVolume->addChild(new SoMaterial);

	// add the sphere
    SoSphere* sph = new SoSphere;
    sph->radius = 0.1;
    clientVolume->addChild(sph);

    return windowKit;
	*/
	return NULL;
}

//----------------------------------------------------------------------------

void
SoNewWidgetTestKit::setSheetMasterMode(SoNode* pipSheet, SbBool masterMode)
{
//--find pointers to the red and blue color buttons
/*	char buffer[100];
    int uid=getUserFromSheet(pipSheet);
	sprintf(buffer,"RED_BUTTON_%d_%s", uid,getName().getString());
    So3DButton* bRed  = (So3DButton*)findNode(pipSheet,buffer);
	sprintf(buffer,"BLUE_BUTTON_%d_%s", uid,getName().getString());
    SoPushButton* bBlue = (SoPushButton*)findNode(pipSheet,buffer);
    if(masterMode) // if context is master -> set callbacks
    {
        bRed->addReleaseCallback(colorButtonCB,this);
 //       bBlue->addReleaseCallback(colorButtonCB,this);
    }
    else // if context is slave -> remove callbacks
    {
        bRed->removeReleaseCallback(colorButtonCB);
 //       bBlue->removeReleaseCallback(colorButtonCB);
    }*/
}

//----------------------------------------------------------------------------
//eof

