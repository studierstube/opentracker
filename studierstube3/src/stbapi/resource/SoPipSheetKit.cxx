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
/** SoPipSheetKit.cxx contains the implementation of class SoPipSheetKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoPipSheetKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>
#include <cstdlib>

#include <Inventor/SbBox.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoInfo.h>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/interaction/So3DSeparator.h> 
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/workspace/SoContextManagerKit.h>

#include "SoPipSheetKit.h"
#include "SoPipKit.h"
#include "SoTabKit.h"
#include "SoTabButtonKit.h"

SO_KIT_SOURCE(SoPipSheetKit);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
SoPipSheetKit::initClass()
{
    SO_KIT_INIT_CLASS(SoPipSheetKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
/** The constructor inits the fields of the kit. */    

SoPipSheetKit::SoPipSheetKit()
:
    appID(-1), maxSheets(16), active(FALSE),
    lastSheetShown(-1), parent(NULL)
{
    SO_KIT_CONSTRUCTOR(SoPipSheetKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic
    // SO_KIT_ADD_CATALOG_LIST_ENTRY(name, listContainerClassName,
    // nullByDefault, parentName,
    // rightName,listItemClassName,
    // isPublicPart)

//---------------SoPipSheet structure-----------------------------------------
//
//                         kitRoot
//                           |
//                 |---------------------|
//               tabs              sheetsRoot
//                                       |
//                              |-----------------|
//                          sheetsTrans         sheets
//
//----------------------------------------------------------------------------

    SO_KIT_ADD_CATALOG_ENTRY(
        tabs, SoTabKit ,TRUE, this,     \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(
        sheetsRoot,    So3DSeparator ,FALSE, this,     \0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(
        sheetsTrans, SoTransform ,TRUE, sheetsRoot,     \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(
        sheets,    SoSwitch ,FALSE, sheetsRoot,     \0, TRUE);

    // add field
    SO_KIT_ADD_FIELD(autoScaling, (TRUE)); 
    SO_KIT_ADD_FIELD(tabTextures, (NULL)); 
    SO_KIT_ADD_FIELD(pipParts, (FULL)); 

    SO_NODE_DEFINE_ENUM_VALUE(PipPartMask, PIP_BODY);
    SO_NODE_DEFINE_ENUM_VALUE(PipPartMask, SHEET);
    SO_NODE_DEFINE_ENUM_VALUE(PipPartMask, SHEET_TABS);
    SO_NODE_DEFINE_ENUM_VALUE(PipPartMask, SYSTEM_TABS);
    SO_NODE_DEFINE_ENUM_VALUE(PipPartMask, WINDOW_TABS);
    SO_NODE_DEFINE_ENUM_VALUE(PipPartMask, FULL);

    SO_NODE_SET_SF_ENUM_TYPE(pipParts, PipPartMask);

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    // init tabTextures field
    for (int i=0; i<maxSheets; i++)
        tabTextures.set1Value(i,NULL);
    
    ((So3DSeparator *)sheetsRoot.getValue())->boundingBoxCheck = FALSE;
}


//----------------------------------------------------------------------------
/** The destructor does nothing. */

SoPipSheetKit::~SoPipSheetKit()
{
    // nil
}

//----------------------------------------------------------------------------
/** Activate sheets of the kit, i.e. build sheetTab-buttons and switch
    to first sheet */

void
SoPipSheetKit::activateSheets(SoPipKit *parent)
{
        // store pointer to parent of this kit
    SoPipSheetKit::parent = parent;
    SbString name = parent->skinDirectory.getValue();
    name += parent->skinFileName.getValue();    
    StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());
    UAGETNODE(maxTabs_,SoInfo,"SHEET_BAR_MAX_TAB_BUTTONS",skin);
    maxSheets = atoi(maxTabs_->string.getValue().getString());

    int numSheets = 0;
    SbBool streamed = FALSE;
    // check for valid number of sheets
    if ((numSheets = getSheets()->getNumChildren()) > maxSheets) {
        printf("ERROR: Application (ID: %d) has too many sheets (%d)\n", 
                       appID, numSheets);
        printf("ERROR: max. %d sheets allowed per SoPipSheetKit\n",maxSheets);
        exit(1);
    }

    if(SO_CHECK_ANY_PART(this,"tabs",SoTabKit))
        streamed = TRUE;
    else
    {
        // create sheet tab bar    
        setAnyPart("tabs",new SoTabKit(maxSheets,skin,"SHEET_BAR_"));    

        // set transform for sheet bar    
        UAGETNODE(start,SoTranslation,"SHEET_BAR_START_TRANSLATION",skin);
        UAGETNODE(size_,SoInfo,"SHEET_BAR_SIZE",skin);
        float size;
        sscanf(size_->string.getValue().getString(),"%f",&size);
        getTabs()->setupTransform(start->translation.getValue(),size,parent->pipSize.getValue());
    }

    //  buildTabs
    for (int i = 0; i < numSheets; i++) 
	{
        if(streamed)
        {
            SoTabButtonKit *tbk = getTabs()->getButtonKit(i, TRUE);
            assert(tbk);
            tbk->setCallback(switchPipSheetCB, this);
            tbk->setStreamedTab(getTabs(), skin, "SHEET_BAR_", maxSheets);
            getTabs()->setStreamedTab(skin, "SHEET_BAR_", maxSheets);
        }
        else
        {
            if( i < tabTextures.getNum())
                getTabs()->addTab(-1,switchPipSheetCB, this,(SoTexture2*)tabTextures[i]);
            else
            {
                getTabs()->addTab(-1,switchPipSheetCB, this, new SoTexture2);
                printf("WARNING: number of tab textures less than tabs !\n");
            }
        }
	}

    // switch to first sheet 
    if(!streamed)
    {
        getSheets()->whichChild = 0;
        getTabs()->showActiveButton(TRUE);
    }
    else
        getTabs()->showActiveButton_ = TRUE;

        // set event interest
    SO_GET_ANY_PART(this,"sheetsRoot",So3DSeparator)->eventMask =
                                             So3DSeparator::BM_ALL_EVENTS;
    active = TRUE;
}

//----------------------------------------------------------------------------
/** Scales current sheet to fit into given sheetArea. */

void
SoPipSheetKit::scaleCurrentSheet(SbVec2f sheetAreaUL, SbVec2f sheetAreaLR)
{
    float px,py,pz,saWidth,saHeight;
    SbVec2f saCenter;
    px = parent->pipSize.getValue()[0];
    py = parent->pipSize.getValue()[1];
    pz = parent->pipSize.getValue()[2];
    saWidth = sheetAreaLR[0] - sheetAreaUL[0];
    saHeight = sheetAreaLR[1] - sheetAreaUL[1];
    saCenter = SbVec2f(sheetAreaUL[0]+saWidth/2.0f,sheetAreaUL[1]+saHeight/2.0f);
    // scale sheets
    static SbViewportRegion vp;
    static SoGetBoundingBoxAction bbAction(vp);
    bbAction.apply(getSheets());
    SbBox3f box = bbAction.getBoundingBox();

    // no scaling possible 
    if (box.isEmpty()) 
        return;  

    // autoscaling
    else if (autoScaling.getValue()) {
        float bx,by,bz;
        box.getSize(bx,by,bz);
        SbVec3f bc = box.getCenter();          
        float sfx = px*saWidth/bx;
        float sfy = py*saHeight/by;
        float sfs = (sfx < sfy) ? sfx : sfy;

        getSheetsTrans()->scaleFactor.setValue(sfs,sfs,sfs);
        getSheetsTrans()->translation.setValue(
                              -bc[0]*sfs - px*(0.5f-saCenter[0]),
                              -bc[1]*sfs + py*(0.5f-saCenter[1]),
                              -bc[2]*sfs + bz*sfs/2.0f + pz/2.0f);
    }
    // no autoscaling  (just compensate for visible tabs)
    else {
        float sfx = px*saWidth/px;
        float sfy = py*saHeight/py;
        float sfs = (sfx < sfy) ? sfx : sfy;

        getSheetsTrans()->scaleFactor.setValue(sfs,sfs,sfs);
        getSheetsTrans()->translation.setValue(-px*(0.5f-saCenter[0]),
                                               py*(0.5f-saCenter[1]),
                                               0.0f);
    }
}


//----------------------------------------------------------------------------
/** Add a sheet to the sheetKit. */

void
SoPipSheetKit::addSheet(SoNode* geometry, SoTexture2* buttonTexture)
{
    int index;

    if ((index = getSheets()->getNumChildren()) < maxSheets) {
        if (active) 
            getTabs()->addTab(-1,switchPipSheetCB, this,buttonTexture);
        else
            // store tab texture
            tabTextures.set1Value(index, buttonTexture);    
        // add geometry
        getSheets()->addChild(geometry);
    }
}


//----------------------------------------------------------------------------
/** Remove a sheet from the sheetKit. */

void
SoPipSheetKit::removeSheet(SoNode* geometry)
{
    int index;

    if ((index = getSheets()->findChild(geometry)) != -1) {
        // remove geometry
        if (active) {
            // switch to first sheet if removed sheet is visible
            if (getSheets()->whichChild.getValue() == index)
                getSheets()->whichChild = 0;
            // release any grabbers in the geometry
            Stb3DEventGenerator::releaseAllGrabbers(geometry);
        }      
        getSheets()->removeChild(geometry);
        if (active) {
            getTabs()->removeTabIndexed(index);
            getTabs()->setActiveButton(getTabs()->getButtonKit(0,TRUE));
        }
        else
            // remove tab texture
            tabTextures.deleteValues(index, 1);
    }
}

//----------------------------------------------------------------------------
/** Switch sheets on or off. */

void
SoPipSheetKit::showSheets(SbBool onOff)
{
    // turn on the last sheet shown
    if ((lastSheetShown > -1) && onOff) {
        getSheets()->whichChild = lastSheetShown;
        lastSheetShown = -1;
    }
    // turn sheet off and store last used sheet
    else if ((lastSheetShown == -1) && !onOff) {
        lastSheetShown = getSheets()->whichChild.getValue();
        getSheets()->whichChild = -1;
    }      
}

//----------------------------------------------------------------------------
/** Get the switch containing all sheets of this kit. */

SoSwitch *
SoPipSheetKit::getSheets()
{
    return SO_GET_ANY_PART(this,"sheets",SoSwitch);
}

//----------------------------------------------------------------------------
/** Get the transform node of the current sheet. */

SoTransform *
SoPipSheetKit::getSheetsTrans()
{
    return SO_GET_ANY_PART(this,"sheetsTrans",SoTransform);
}

//----------------------------------------------------------------------------
/** Get the sheetTabButton-kit. */

SoTabKit *
SoPipSheetKit::getTabs()
{
    return SO_GET_ANY_PART(this,"tabs",SoTabKit);
}

//----------------------------------------------------------------------------
/** Handles callbacks from sheetTab-buttons (switches to corresponding
    sheet). */

void
SoPipSheetKit::switchPipSheetCB(void* userData, So3DButton* button)
{
    SoPipSheetKit *parent = (SoPipSheetKit *)userData;
    if(SoContextManagerKit::getInstance()->isNodeInSlaveContext(button))
        return;
    //do not switch or issue callback if it is current sheet
    if (parent->getSheets()->whichChild.getValue() == 
        parent->getTabs()->getButtonIndex(button))
        return;

    // actually switch sheet geometry and update pip
    parent->getSheets()->whichChild = parent->getTabs()->getButtonIndex(button);
    parent->parent->updatePip();
}

//----------------------------------------------------------------------------
/** Switch to a pipSheet directly, without using the sheetTab-buttons. */

void
SoPipSheetKit::switchPipSheet(int index)
{
    if ((!active) ||
        (index >= getSheets()->getNumChildren()) || (index < 0) ||
        (getSheets()->whichChild.getValue() == index))
        return;
    // actually switch sheet geometry and update pip 
    getSheets()->whichChild = index;
    getTabs()->setActiveButton(getTabs()->getButtonKit(index,TRUE));
    parent->updatePip();
}

/* ===========================================================================
    End of SoPipSheetKit.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */



