/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** SoTabButonKit.h contains the implementation of class SoTabButtonKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoTabButtonKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>

#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/workspace/distrManager/StbDistrManager.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>

#include "SoTabKit.h"
#include "SoTabButtonKit.h"
#include "SoPullDownKit.h"


SO_KIT_SOURCE(SoTabButtonKit);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
SoTabButtonKit::initClass()
{
    SO_KIT_INIT_CLASS(SoTabButtonKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
/** Constructor (required for streaming!) */

SoTabButtonKit::SoTabButtonKit()
: highlightSensor(NULL), parent(NULL), 
  skin(NULL), prefix(NULL), actualTexture(NULL)
{
    SO_KIT_CONSTRUCTOR(SoTabButtonKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        tabRoot,    SoGroup ,FALSE, this,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabTransform, SoTransform ,FALSE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabButton,    So3DButton ,FALSE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabPullDownSwitch,    SoSwitch ,FALSE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_LIST_ENTRY(
        pullDownButtons,    SoSeparator ,FALSE, tabPullDownSwitch,     \0, 
        SoPullDownKit,FALSE);

    SO_KIT_ADD_FIELD(buttonId, (-1)); 
    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);
}

//----------------------------------------------------------------------------
/** The constructor of the kit inits the nodes of this kit. */

SoTabButtonKit::SoTabButtonKit(SoTabKit *parent,
                   int buttonID,
                   So3DButtonCB* callback,
                   void *data,
                   StbSkinLoader *skin,
                   char *prefix,
                   SoTexture2* buttonTexture)
: highlightSensor(NULL), parent(parent),
  skin(skin), prefix(prefix), actualTexture(NULL)
{
    SO_KIT_CONSTRUCTOR(SoTabButtonKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        tabRoot,    SoGroup ,FALSE, this,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabTransform, SoTransform ,FALSE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabButton,    So3DButton ,FALSE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabPullDownSwitch,    SoSwitch ,FALSE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_LIST_ENTRY(
        pullDownButtons,    SoSeparator ,FALSE, tabPullDownSwitch,     \0, 
        SoPullDownKit,FALSE);

    SO_KIT_ADD_FIELD(buttonId, (-1)); 
    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);
    buttonId.setValue(buttonID);

    PUAGETNODE(refTranslation,SoTranslation,prefix,"TRANSLATION",skin);
    PUAGETNODE(refButton,So3DButton,prefix,"BUTTON",skin);
    getTransform()->translation = refTranslation->translation.getValue(); 
    getButton()->width  = refButton->width.getValue();
    getButton()->height = refButton->height.getValue();
    getButton()->depth  = refButton->depth.getValue();
    getButton()->buttonColor = refButton->buttonColor.getValue();

    // setup button texture
    if (buttonTexture) 
        getButton()->setupTexture(buttonTexture,0,0);
    actualTexture = buttonTexture;

    getButton()->addReleaseCallback(releaseCB,this);
    // set tab button callback
    if (callback)
        getButton()->addReleaseCallback(callback,data);

    // setup highlight sensor
    highlightSensor = new SoFieldSensor(highlightCB, this);
    highlightSensor->setPriority(SoFieldSensor::getDefaultPriority());
    highlightSensor->attach(&getButton()->highlighted); 
}


//----------------------------------------------------------------------------
/** Destructor of the kit. */

SoTabButtonKit::~SoTabButtonKit()
{
    if (highlightSensor) 
        delete highlightSensor;
    Stb3DEventGenerator::releaseAllGrabbers(this);
}

//----------------------------------------------------------------------------
/** Set release callback for this kit's button. */

void
SoTabButtonKit::setCallback(So3DButtonCB*callback,void*data)
{
    // set tab button callback
    if (callback)
        getButton()->addReleaseCallback(callback,data);
}

//----------------------------------------------------------------------------
/** Set parent of this kit */

void
SoTabButtonKit::setStreamedTab(SoTabKit *theParent, StbSkinLoader* theSkin, char* thePrefix, int theMaxSheets)
{
    parent = theParent;
    skin = theSkin;
    prefix = strdup(thePrefix);
    getButton()->addReleaseCallback(releaseCB,this);

    // setup highlight sensor
    highlightSensor = new SoFieldSensor(highlightCB, this);
    highlightSensor->setPriority(SoFieldSensor::getDefaultPriority());
    highlightSensor->attach(&getButton()->highlighted); 
}

//----------------------------------------------------------------------------
/** Add a pullDownButton to this kit. */

void
SoTabButtonKit::addPullDownButton(SbName name,So3DButtonCB*callback,void*data)
{
    assert(getPullDownButtons());
    getPullDownButtons()->
      addChild(new SoPullDownKit(name,callback,data,skin,prefix,actualTexture,
                                 !getPullDownButtons()->getNumChildren()));  
}

//----------------------------------------------------------------------------
/** Remove a pullDownButton from this kit. */

void
SoTabButtonKit::removePullDownButton(SbName name)
{
    SoNodeKitListPart * pdb = getPullDownButtons();    
    assert(pdb);

    for (int i=0;i<pdb->getNumChildren();i++)
        if (((SoPullDownKit *)pdb->getChild(i))->name == name) 
        {
            pdb->removeChild(i);
            break;
        }
}

//----------------------------------------------------------------------------
/** Highlight the pullDownButton specified by name. */

void
SoTabButtonKit::setActivePullDownButton(SbName name)
{
    SoNodeKitListPart *pdb = getPullDownButtons();
    assert(pdb);
    for (int i=0;i<pdb->getNumChildren();i++)
        if (((SoPullDownKit *)pdb->getChild(i))->name == name) {            
            PUAGETNODE(material,SoMaterial,prefix,"BUTTON_ACTIVE_COLOR",skin);
            ((SoPullDownKit *)pdb->getChild(i))->getTextureButton()->
                              buttonColor = material->diffuseColor[0];
        }
        else {
            PUAGETNODE(refButton,So3DButton,prefix,"BUTTON",skin);
            ((SoPullDownKit *)pdb->getChild(i))->getTextureButton()->
                      buttonColor = refButton->buttonColor.getValue();
        }
}
//----------------------------------------------------------------------------
/** Set a custom text for a pullDownButton of this kit. */

void
SoTabButtonKit::setPullDownText(SbName name, const char *text)
{
    SoNodeKitListPart * pdb = getPullDownButtons();    
    assert(pdb);
    for (int i=0;i<pdb->getNumChildren();i++)
        if (((SoPullDownKit *)pdb->getChild(i))->name == name) {            
            ((SoPullDownKit *)pdb->getChild(i))->set3DText(text);
            return;
    }
}

//----------------------------------------------------------------------------
/** Get the button of this kit. */

So3DButton *
SoTabButtonKit::getButton()
{
    return SO_GET_ANY_PART(this,"tabButton",So3DButton);
}

//----------------------------------------------------------------------------
/** Show or hide all pullDownButtons of this kit. */

void
SoTabButtonKit::showPullDown(SbBool onOff)
{
    SO_GET_ANY_PART(this,"tabPullDownSwitch",SoSwitch)->whichChild = 
                                                        onOff ? 0 : -1;
}

//----------------------------------------------------------------------------
/** Get listPart containing all pullDownButtons of this kit. */

SoNodeKitListPart *
SoTabButtonKit::getPullDownButtons()
{
    return SO_GET_ANY_PART(this,"pullDownButtons",SoNodeKitListPart);
}

//----------------------------------------------------------------------------
/** Get the transform-node of this kit. */

SoTransform *
SoTabButtonKit::getTransform()
{
    return SO_GET_ANY_PART(this,"tabTransform",SoTransform);
}

//----------------------------------------------------------------------------
/** Returns the ID of the tabButtonKit. */

int
SoTabButtonKit::getButtonID()
{
    return buttonId.getValue();
}

//----------------------------------------------------------------------------
/** Set the texture of this kit's button. */

void
SoTabButtonKit::setTexture(SoTexture2 *texture)
{
    if(!texture) return;  // GH: don't try to add an empty texture
    getButton()->setupTexture(texture,0,0);
	actualTexture = texture;
}

//----------------------------------------------------------------------------
/** Copy the texture from an other kit's button to this kit's button. */

void
SoTabButtonKit::copyTexture(SoTabButtonKit *from)
{
	setTexture(from->actualTexture);
}

//----------------------------------------------------------------------------
/** Callback of a highlightSensor monitoring a tabButtonKit-button. */

void 
SoTabButtonKit::highlightCB(void *data, SoSensor *)
{
    SoTabButtonKit *tbk = (SoTabButtonKit *)data;
    // if a tab button got highlighted or pressed
    if (tbk->getButton()->highlighted.getValue() > 0) {
        tbk->parent->resetAllPullDownMenues(tbk);
        tbk->showPullDown(TRUE);
    }
}

//----------------------------------------------------------------------------
/** Release callback of tabButtonKit-buttons. */

void
SoTabButtonKit::releaseCB(void* userData, So3DButton* button)
{
    SbAppID appId;
    SoTabButtonKit *tbk = (SoTabButtonKit *)userData;
    SoContextKit * ctx = SoContextManagerKit::getInstance()->getAnyContextFromNode(tbk);    
    if(ctx != NULL)
    {
        if( ctx->getMasterMode())
        {
            tbk->parent->setActiveButton(tbk);
            appId = ctx->appID.getValue();
        }        
        else
            return;
    }
    // this must be the system pip sheet !
    else 
        appId = 0;
    SoUserKit * userkit = SoUserManagerKit::getInstance()->getUserKitFromNode( tbk );
    if( userkit == NULL )  // this node is not part of a userkit's pipsheet or something
        return;
    int userId = userkit->userID.getValue();
    StbDistrManager::getInstance()->sheetBarChanged( userId, appId, tbk->parent->getButtonIndex(button));
}

//----------------------------------------------------------------------------
//eof
