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
/** SoPipKit.cxx contains the implementation of class SoPipKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoPipKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file
  */
 /* ======================================================================== */

#include <cassert>
#include <cstdlib>

#include <Inventor/SbBox.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>

#include "SoPipSheetKit.h"
#include "SoPipKit.h"
#include "SoTabKit.h"
#include "StbSkinLoader.h"

#include <stbapi/util/ivio.h>

const int PIP_INFO = FALSE;

SO_KIT_SOURCE(SoPipKit);


//----------------------------------------------------------------------------

/** Inventor class initialization */

void
SoPipKit::initClass()
{
    SO_KIT_INIT_CLASS(SoPipKit, SoTrackedArtifactKit, "SoTrackedArtifactKit");
}

//----------------------------------------------------------------------------

/** The constructor inits the fields of the kit an enables all 3D events.*/

SoPipKit::SoPipKit()
:
currentSheetKit(NULL)
{
    SO_KIT_CONSTRUCTOR(SoPipKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        task, SoTabKit, TRUE, onOffSwitch,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        system, SoTabKit, TRUE, onOffSwitch,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        window, SoTabKit, TRUE, onOffSwitch,     \0,FALSE);
    SO_KIT_ADD_CATALOG_LIST_ENTRY(
        pipSheets, SoSwitch ,FALSE, onOffSwitch,   \0,SoPipSheetKit,TRUE);

    // add field
    SO_KIT_ADD_FIELD(pipSize, (0.3,0.2,0.01));
    SO_KIT_ADD_FIELD(sheetAreaUpperLeft, (0.0,0.0));
    SO_KIT_ADD_FIELD(sheetAreaLowerRight, (1.0,1.0));
    SO_KIT_ADD_FIELD(skinFileName, ("skinWS1.iv"));
    SO_KIT_ADD_FIELD(skinDirectory, ("./graphix/"));
    SO_KIT_ADD_FIELD(systemBar, (TRUE));
    SO_KIT_ADD_FIELD(windowBar, (TRUE));

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    setEventMask(So3DSeparator::BM_ALL_EVENTS);

	systemBarSensor.setFunction( SoPipKit::barCB );
	systemBarSensor.setData( this );
    systemBarSensor.attach(&systemBar);
	windowBarSensor.setFunction( SoPipKit::barCB );
	windowBarSensor.setData( this );
    windowBarSensor.attach(&windowBar);
}

//----------------------------------------------------------------------------

/** The destructor releases all grabbers of the PIP */

SoPipKit::~SoPipKit()
{
	windowBarSensor.detach();
	systemBarSensor.detach();
    Stb3DEventGenerator::releaseAllGrabbers(this);
}

//----------------------------------------------------------------------------

/** This method is used to create an initial PIP-sheet for an
    application (and context) */

void
SoPipKit::createResources(SbAppID appID)
{
    if (getPipSheetKit(appID) == NULL) {
        SoPipSheetKit *psk = new SoPipSheetKit;
        psk->appID = appID;
        psk->parent = this;
        getPipSheets()->addChild(psk);
        if (PIP_INFO)
            printf("INFO(SoPipKit): created resource (appID: %d)\n",
                     appID);
    }
    else {
        printf("ERROR(SoPipKit): can't create resource (appID: %d)\n",
                     appID);
//        exit(-1);
    }
}

//----------------------------------------------------------------------------

/** This method is used to free (delete) a PIP-sheet of an
    application(and context). */

void
SoPipKit::freeResources(SbAppID appID)
{

    for (int i = 0; i<getPipSheets()->getNumChildren(); i++)
        if (((SoPipSheetKit *)getPipSheets()->getChild(i))->appID == appID)
        {
            Stb3DEventGenerator::releaseAllGrabbers(getPipSheets()->getChild(i));
            if (PIP_INFO)
                printf("INFO(SoPipKit): freed resource (appID: %d)\n",appID);
            if (getPipSheets()->getChild(i) == currentSheetKit)
                currentSheetKit = NULL;
            getPipSheets()->removeChild(i);
            i--;
        }
}

//----------------------------------------------------------------------------

/** This method is used to switch to a PIP-sheet of an application
    (and context). */

void
SoPipKit::useResources(SbAppID appID)
{
    SoPipSheetKit *psk = getPipSheetKit(appID);
    // switch to associated pipSheetKit
    if (psk) {
        // switch to sheet
        SbString setString = "whichChild ";
        setString += SbString(getPipSheets()->findChild(psk));
        getPipSheets()->containerSet(setString.getString());
        // store pointer to currently used pipSheetKit
        currentSheetKit = psk;
        updatePip();
        if (PIP_INFO)
            printf("INFO(SoPipKit): using resource (appID: %d)\n",
                     appID);
    }
    else {
        printf("ERROR(SoPipKit): can't use resource (appID: %d)\n",
                     appID);
//        exit(-1);
    }
}

//----------------------------------------------------------------------------

/** Set the PIP-geometry (the PIP-sheet) for the specified application
    (context). */

void
SoPipKit::setPipGeometry(SbAppID appID, SoNode* pipGeom, SbBool usePipCube)
{
    SoPipSheetKit *psk = getPipSheetKit(appID);
    if (psk) {
        if (pipGeom) {
            SoPipSheetKit *pskNew;
            if (PIP_INFO)
                printf("INFO(SoPipKit): setting pip geo (appID: %d)\n",
                     appID);
            if (pipGeom->isOfType(SoPipSheetKit::getClassTypeId())) {
                pskNew = (SoPipSheetKit *)pipGeom;
            }
            else {       // add pipGeom as new pip sheet - legacy handling

                pskNew = new SoPipSheetKit;
                pskNew->addSheet(pipGeom);
                pskNew->pipParts = SoPipSheetKit::FULL^SoPipSheetKit::SHEET_TABS;
                // usePipCube - legacy handling
                pskNew->pipParts = usePipCube ?
                    pskNew->pipParts.getValue()|SoPipSheetKit::PIP_BODY :
                    pskNew->pipParts.getValue()&~SoPipSheetKit::PIP_BODY;

            }
            // set psk data
            pskNew->appID = appID;
            //pskNew->contextName = ctxName;
            // replace sheet of this application with new sheet
            Stb3DEventGenerator::releaseAllGrabbers(psk);
            getPipSheets()->replaceChild(psk,pskNew);
            // activate new sheet
            pskNew->activateSheets(this);
            int numTabs = pskNew->getSheets()->getNumChildren();
            char buffer[1024];
            if(appID!=0) //exclude system sheet
            {
                for(int i=0; i<numTabs; i++)
                {
                    //SoContextKit *ctx = (SoContextKit*)SoContextKit::getByName(ctxName);
                    //SoContextKit *ctx = SoContextManagerKit::getInstance()->
                    //    getApp(appID)->getContext();
                    //assert(ctx);
                    int userId = SoContextManagerKit::getInstance()->getUserFromSheet(pskNew->getTabs(), appID);
                    sprintf(buffer,"TB_%d_%d_%d", appID, i, userId);
                    pskNew->getTabs()->getButtonKit(i, TRUE)->getButton()->setName(buffer);
                    sprintf(buffer,"TBK_%d_%d_%d", appID, i, userId);
                    pskNew->getTabs()->getButtonKit(i, TRUE)->setName(buffer);
                }
            }
            if (psk == currentSheetKit) {
                currentSheetKit = pskNew;
                updatePip();
            }
        }
   }
   else {
       printf("ERROR(SoPipKit): can't set geometry (appID: %d)\n",
                     appID);
//       exit(-1);
   }
}

//----------------------------------------------------------------------------

/** Get the PIP-geometry (the PIP-sheet) of the specified application
    (context). */

SoPipSheetKit *
SoPipKit::getPipSheetKit(SbAppID appID)
{
    for (int i = 0; i<getPipSheets()->getNumChildren(); i++)
        if (((SoPipSheetKit *)getPipSheets()->getChild(i))->appID == appID) {
            return (SoPipSheetKit *)getPipSheets()->getChild(i);
        }
    return NULL;
}

//----------------------------------------------------------------------------

/** Get a SoNodeKitListPart containing all registered PIP-sheets.*/

SoNodeKitListPart *
SoPipKit::getPipSheets()
{
    return SO_GET_ANY_PART(this, "pipSheets",SoNodeKitListPart);
}

//----------------------------------------------------------------------------

/** Set the visibility of the PIP-parts for the currently used PIP-sheet. */

void
SoPipKit::setPipPartVisibility(unsigned int ppm)
{
    if (currentSheetKit)
        currentSheetKit->pipParts.setValue(ppm);
    updatePip();
}

//----------------------------------------------------------------------------

/** Set the visibility of the workspace tabs */

void
SoPipKit::showWorkspaceTabs(SbBool system, SbBool window)
{
    if (window)
    {
        if (SO_CHECK_ANY_PART(this,"window",SoTabKit))
            SO_CHECK_ANY_PART(this,"window",SoTabKit)->showTabs(TRUE);
    }
    else
        if (isActiveWindowBar())
            SO_CHECK_ANY_PART(this,"window",SoTabKit)->showTabs(FALSE);

    if (system) {
        if (SO_CHECK_ANY_PART(this,"system",SoTabKit))
            SO_CHECK_ANY_PART(this,"system",SoTabKit)->showTabs(TRUE);
    }
    else {
        if (isActiveTaskBar())
            SO_CHECK_ANY_PART(this,"task",SoTabKit)->showTabs(FALSE);
        if (SO_CHECK_ANY_PART(this,"system",SoTabKit))
            SO_CHECK_ANY_PART(this,"system",SoTabKit)->showTabs(FALSE);
    }
}

//----------------------------------------------------------------------------

/** Add systemBar to PIP geometry. */

void
SoPipKit::useSystemBar(SoTabKit *systemBar)
{
    // set systemBar
    setAnyPart("system",systemBar);
}

//----------------------------------------------------------------------------

/** Add taskBar to PIP geometry. */

void
SoPipKit::useTaskBar(SoTabKit *taskBar)
{
    // set taskBar
    setAnyPart("task",taskBar);
}

//----------------------------------------------------------------------------

/** Add windowBar to PIP geometry. */

void
SoPipKit::useWindowBar(SoTabKit *windowBar)
{
    // set taskBar
    setAnyPart("window",windowBar);
}

//----------------------------------------------------------------------------

/** Returns whether the taskBar is active (set and visible) */

SbBool
SoPipKit::isActiveTaskBar()
{
    if (SO_CHECK_ANY_PART(this,"task",SoTabKit) == NULL)
       return FALSE;
    else
       return SO_CHECK_ANY_PART(this,"task",SoTabKit)->isVisible();
}

//----------------------------------------------------------------------------

/** Returns whether the systemBar is active (set and visible) */

SbBool
SoPipKit::isActiveSystemBar()
{
    if (SO_CHECK_ANY_PART(this,"system",SoTabKit) == NULL)
       return FALSE;
    else
       return SO_CHECK_ANY_PART(this,"system",SoTabKit)->isVisible();
}

//----------------------------------------------------------------------------

/** Returns whether the windowBar is active (set and visible) */

SbBool
SoPipKit::isActiveWindowBar()
{
    if (SO_CHECK_ANY_PART(this,"window",SoTabKit) == NULL)
       return FALSE;
    else
       return SO_CHECK_ANY_PART(this,"window",SoTabKit)->isVisible();
}

//----------------------------------------------------------------------------

/** This method merges the two sheetAreas. */

void
SoPipKit::mergeSheetAreas(SoPipKit *mergeArea,SbVec2f &resultAreaUL,
                          SbVec2f &resultAreaLR)
{
    resultAreaUL[0] = (resultAreaUL[0] >
              mergeArea->sheetAreaUpperLeft.getValue()[0]) ?
                  resultAreaUL[0] :
                  mergeArea->sheetAreaUpperLeft.getValue()[0];
    resultAreaUL[1] = (resultAreaUL[1] >
              mergeArea->sheetAreaUpperLeft.getValue()[1]) ?
                  resultAreaUL[1] :
                  mergeArea->sheetAreaUpperLeft.getValue()[1];

    resultAreaLR[0] = (resultAreaLR[0] <
              mergeArea->sheetAreaLowerRight.getValue()[0]) ?
                  resultAreaLR[0] :
                  mergeArea->sheetAreaLowerRight.getValue()[0];
    resultAreaLR[1] = (resultAreaLR[1] <
              mergeArea->sheetAreaLowerRight.getValue()[1]) ?
                  resultAreaLR[1] :
                  mergeArea->sheetAreaLowerRight.getValue()[1];
}

//----------------------------------------------------------------------------
/** Force refresh of pip content. */

void
SoPipKit::updatePip()
{
    if (currentSheetKit) {
        // set pip parts visibility
        int ppv = currentSheetKit->pipParts.getValue();
        showGeometry(ppv & SoPipSheetKit::PIP_BODY);
        showWorkspaceTabs( systemBar.getValue() && ( ppv & SoPipSheetKit::SYSTEM_TABS),
                           windowBar.getValue() && ( ppv & SoPipSheetKit::WINDOW_TABS));
        currentSheetKit->getTabs()->showTabs((ppv & SoPipSheetKit::SHEET_TABS)
                                              && !isActiveTaskBar());

        currentSheetKit->showSheets((ppv & SoPipSheetKit::SHEET)
                                     && !isActiveTaskBar());

        // calculate currently useable sheetArea
        SbString name = skinDirectory.getValue();
        name += skinFileName.getValue();
        StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());
        assert(skin);

        // calculate current sheet area
            // init area with pip area
        SbVec2f resultUL,resultLR;
        resultUL = sheetAreaUpperLeft.getValue();
        resultLR = sheetAreaLowerRight.getValue();
           // merge with active tab bars
        SoPipKit *mergeArea;
        if (currentSheetKit->getTabs()->isVisible()) {
            UAGETNODE(mergeArea,SoPipKit,"SHEET_AREA",skin);
            mergeSheetAreas(mergeArea,resultUL,resultLR);
        }
        if (isActiveSystemBar()) {
            UAVGETNODE(mergeArea,SoPipKit,"SYSTEM_BAR_SHEET_AREA",skin);
            mergeSheetAreas(mergeArea,resultUL,resultLR);
        }
        if (isActiveTaskBar()) {
            UAVGETNODE(mergeArea,SoPipKit,"TASK_BAR_SHEET_AREA",skin);
            mergeSheetAreas(mergeArea,resultUL,resultLR);
        }
        if (isActiveWindowBar()) {
            UAVGETNODE(mergeArea,SoPipKit,"WINDOW_BAR_SHEET_AREA",skin);
            mergeSheetAreas(mergeArea,resultUL,resultLR);
        }

        // scale the pip sheet
        currentSheetKit->scaleCurrentSheet(resultUL,resultLR);
    }
}

void SoPipKit::barCB(void * data, SoSensor* sensor){
    SoPipKit * pipKit = (SoPipKit *)data;
    pipKit->showWorkspaceTabs(pipKit->systemBar.getValue(), pipKit->windowBar.getValue());
}

/* ===========================================================================
    End of SoPipKit.cxx
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
