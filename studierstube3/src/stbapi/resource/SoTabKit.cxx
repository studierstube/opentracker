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
/** SoTabKit.h contains the implementation of class SoTabKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoTabKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>

#include "SoPipSheetKit.h"
#include "SoTabKit.h"

SO_KIT_SOURCE(SoTabKit);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
SoTabKit::initClass()
{
    SO_KIT_INIT_CLASS(SoTabKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
/** A dummy constructor */

SoTabKit::SoTabKit()
: maxTabs(0), showActiveButton_(FALSE)
{
    SO_KIT_CONSTRUCTOR(SoTabKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        tabRoot,    SoSeparator ,FALSE, this,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabTransform, SoTransform ,TRUE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabSwitch,    SoSwitch ,FALSE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_LIST_ENTRY(
        tabButtons,    SoSeparator ,TRUE, tabSwitch,     \0, SoTabButtonKit,FALSE);

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    showTabs(FALSE);    
}

//----------------------------------------------------------------------------
/** Constructor of the kit, inits the nodes of the kit. */

SoTabKit::SoTabKit(int maxTabs, StbSkinLoader *skin, char *prefix)
: skin(skin), prefix(prefix), maxTabs(maxTabs), showActiveButton_(FALSE)
{
    SO_KIT_CONSTRUCTOR(SoTabKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        tabRoot,    SoSeparator ,FALSE, this,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabTransform, SoTransform ,TRUE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        tabSwitch,    SoSwitch ,FALSE, tabRoot,     \0,FALSE);
    SO_KIT_ADD_CATALOG_LIST_ENTRY(
        tabButtons,    SoSeparator , FALSE, tabSwitch,     \0, SoTabButtonKit,FALSE);

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    showTabs(TRUE);    
}


//----------------------------------------------------------------------------
/** The destructor releases all grabbers of this tabKit. */

SoTabKit::~SoTabKit()
{
    Stb3DEventGenerator::releaseAllGrabbers(this);
}

//----------------------------------------------------------------------------
/**Sets the nodekit-internal transform-node (positions the tabKit on the PIP)*/

void
SoTabKit::setupTransform(SbVec3f start, float barSize, SbVec3f pipSize) 
{
    if (barSize == 0.0)
        return;
    float tx,ty,tz;
    PUAGETNODE(refTranslation,SoTranslation,prefix,"TRANSLATION",skin);
    refTranslation->translation.getValue().getValue(tx,ty,tz); 
    PUAGETNODE(refButton,So3DButton,prefix,"BUTTON",skin);
    float width  = refButton->width.getValue();
    float depth  = refButton->depth.getValue();
    float height = refButton->height.getValue();
        // get size of pip
    float px,py,pz;
    px = pipSize.getValue()[0]; py = pipSize.getValue()[1];
    pz = pipSize.getValue()[2];

        // calculate scale factor for tabs
    float boxX = 1.0*width +  float(maxTabs-1) * tx;
    float boxY = 1.0*depth +  float(maxTabs-1) * tz;
    if (start[0]<0.0 || start[0]>= 1.0)
        start[0] = 0.0;
    float sfx = (px*barSize)/boxX;
    if (start[1]<0.0 || start[1]>= 1.0)
        start[1] = 0.0;
    float sfy = (py*barSize)/boxY;

    float sft = (sfx<sfy) ? sfx : sfy;

    // set transform node for tab bar
    getTransform()->scaleFactor.setValue(sft,sft,sft);
    getTransform()->rotation.setValue(SbRotation(SbVec3f(1,0,0),M_PI/2.0));
    getTransform()->translation.setValue(
                                -px/2.0 + px*start[0] - tx*sft + width*sft/2.0,
                                 py/2.0 - py*start[1] + tz*sft - depth*sft/2.0,
                                 pz/2.0 + height*sft/2.0);

}

//----------------------------------------------------------------------------
/** Add a button to the tabKit. */

void
SoTabKit::addTab(int buttonID, So3DButtonCB* callback, void *data, SoTexture2* buttonTexture)
{
    //  check if ID is valid
    if (buttonID > -1 && getButtonKit(buttonID))
        return;
    getButtons()->addChild(new SoTabButtonKit(
       this, buttonID,callback,data,skin,prefix,buttonTexture));
    // if first button
    if (getButtons()->getNumChildren() == 1)
        setActiveButton((SoTabButtonKit *)getButtons()->getChild(0));   
}

//----------------------------------------------------------------------------
/** Remove a button from the tabKit identified by ID. */

void
SoTabKit::removeTab(int buttonID)
{
    if ((buttonID < 0) || (getButtonKit(buttonID) == NULL))
        return;
    getButtons()->removeChild(getButtonKit(buttonID));
}

//----------------------------------------------------------------------------
/** Remove a button from the tabKit identified by index. */

void
SoTabKit::removeTabIndexed(int index)
{
	if( index >= 0 && index < getButtons()->getNumChildren())
		getButtons()->removeChild(index);
}

//----------------------------------------------------------------------------
/** Returns the ID of the given button. */

int
SoTabKit::getButtonID(So3DButton *button)
{
    for (int i=0;i<getButtons()->getNumChildren();i++)
        if (((SoTabButtonKit *)getButtons()->getChild(i))->getButton() == 
            button) 
            return ((SoTabButtonKit *)getButtons()->getChild(i))->getButtonID();

    return -1;

}

//----------------------------------------------------------------------------
/** Returns sequential index of given button. */

int
SoTabKit::getButtonIndex(So3DButton *button)
{
    for (int i=0;i<getButtons()->getNumChildren();i++)
        if (((SoTabButtonKit *)getButtons()->getChild(i))->getButton() == 
            button) 
            return i;

    return -1;
}

//----------------------------------------------------------------------------
/** Returns a tabButtonKit of the tabKit identified by ID or index. */

SoTabButtonKit *
SoTabKit::getButtonKit(int buttonID, SbBool indexed)
{
    if (indexed) 
        if ((buttonID < 0) || (buttonID >= getButtons()->getNumChildren()))
            return NULL;
        else
            return ((SoTabButtonKit *)getButtons()->getChild(buttonID));
  
    for (int i=0;i<getButtons()->getNumChildren();i++)
        if (((SoTabButtonKit *)getButtons()->getChild(i))->getButtonID() == 
            buttonID) 
            return (SoTabButtonKit *)getButtons()->getChild(i);
    return NULL;
}

//----------------------------------------------------------------------------
/** Returns the list of tabButtonKits managed by this kit. */

SoNodeKitListPart *
SoTabKit::getButtons()
{
    return SO_GET_ANY_PART(this,"tabButtons",SoNodeKitListPart);
}

//----------------------------------------------------------------------------
/** Returns the transform-node, which determines the position on the PIP. */

SoTransform *
SoTabKit::getTransform()
{
    return SO_GET_ANY_PART(this,"tabTransform",SoTransform);
}

//----------------------------------------------------------------------------
/** Method to show or hide the tabBar. */

void
SoTabKit::showTabs(SbBool onOff)
{
    if (onOff)
        SO_GET_ANY_PART(this,"tabSwitch",SoSwitch)->whichChild = 0;
    else {
        SO_GET_ANY_PART(this,"tabSwitch",SoSwitch)->whichChild = -1;
        resetAllPullDownMenues();
    }
}

//----------------------------------------------------------------------------
/** Determine if active buttons should be highlighted. */

void
SoTabKit::showActiveButton(SbBool onOff)
{
    if (onOff != showActiveButton_) {
        showActiveButton_ = onOff;
        if (getButtons()->getNumChildren())
             if (onOff)
                 setActiveButton((SoTabButtonKit *)getButtons()->getChild(0));
             else
                 setActiveButton(NULL);
    }    
}

//----------------------------------------------------------------------------
/** Returns whether the tabBar is visible or not. */

SbBool
SoTabKit::isVisible()
{
    if (SO_GET_ANY_PART(this,"tabSwitch",SoSwitch)->whichChild.getValue() == 0)
        return TRUE;
    else 
        return FALSE;
}

//----------------------------------------------------------------------------
/** Hides all pull-down menues in the tabKit but the specified one. */

void
SoTabKit::resetAllPullDownMenues(SoTabButtonKit *but)
{
    for (int i=0;i<getButtons()->getNumChildren();i++)
        if (((SoTabButtonKit *)getButtons()->getChild(i)) != but) 
            ((SoTabButtonKit *)getButtons()->getChild(i))->showPullDown(FALSE);
}

//----------------------------------------------------------------------------
/** Force highlight on the given button and a button in the buttonKit's pull-down menue. */

void
SoTabKit::setActiveButton(SoTabButtonKit *button, SbName name)
{
    if (!showActiveButton_)
        return;
    for (int i=0;i<getButtons()->getNumChildren();i++)
        if (((SoTabButtonKit *)getButtons()->getChild(i)) != button) {
            PUAGETNODE(refButton,So3DButton,prefix,"BUTTON",skin);
            ((SoTabButtonKit *)getButtons()->getChild(i))->getButton()->
                         buttonColor = refButton->buttonColor.getValue();
            ((SoTabButtonKit*)getButtons()->getChild(i))->
                         setActivePullDownButton("");
        }
        else {
            PUAGETNODE(material,SoMaterial,prefix,"BUTTON_ACTIVE_COLOR",skin);
            button->getButton()->buttonColor = material->diffuseColor[0];
            button->setActivePullDownButton(name);
        }
}

//----------------------------------------------------------------------------
/** Set parent of this kit */

void
SoTabKit::setStreamedTab(StbSkinLoader* theSkin, char* thePrefix, int theMaxSheets)
{
    skin = theSkin;
    prefix = strdup(thePrefix);
    maxTabs = theMaxSheets;
}

//----------------------------------------------------------------------------

//eof

