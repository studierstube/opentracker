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
/** SoUserKit.h contains the implementation of class SoUserKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoUserKit.cxx 4141 2005-03-29 10:46:07Z kaufmann $
  * @file                                                                    */
 /* ======================================================================== */

#include <cassert>

#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/actions/SoWriteAction.h>

#include <stbapi/util/ivio.h>
#include <stbapi/util/fileBrowser.h>
#include <stbapi/misc/ivmacros.h>

#include <stbapi/context/StudierstubeContext.h>
#include <stbapi/context/SoContext.h>

#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>

#include <stbapi/resource/SoUserManagerKit.h>

#include "SoUserKit.h"
#include "SoTabKit.h"
#include "SoTabButtonKit.h"
#include "SoPullDownKit.h"

struct PullDownCallbackData 
{
    SbAppID appID;
    SoUserKit *parent; 
};

struct setPipData 
{
    SbAppID appID;
    SoNode *pipGeom;
    SbBool usePipCube;
    SbName ctxName;
    SoPipKit *pip;
};

#include <stbapi/workspace/distrManager/StbDistrManager.h>

SO_KIT_SOURCE(SoUserKit);

//----------------------------------------------------------------------------

void
SoUserKit::initClass()
{
    SO_KIT_INIT_CLASS(SoUserKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------

SoUserKit::SoUserKit()
: 
root(NULL), exclusiveGeometryRoot(NULL), userDoesDrag(FALSE),
systemBarLocked(FALSE), systemBarMessageSent(FALSE), focus(NULL),
currentApp(-1), systemBar(NULL), taskBar(NULL), windowBar(NULL),
skin(NULL), vcManager(NULL)
{
    SO_KIT_CONSTRUCTOR(SoUserKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    // name,className,nullByDefault,parentName,rightSiblingName,isPublic

//--------------- SoUser structure -------------------------------------------
//
//                         kitRoot
//                           |
//        +--------+---------+-----------+--------------+
//        |        |         |           |              |
//     context    pen       pip       display        auxGeometry
//
//----------------------------------------------------------------------------
                   
    SO_KIT_ADD_CATALOG_ENTRY(
        context,    SoContext,FALSE, this,     \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(
        pen,    SoPenKit,TRUE, this,     \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(
        pip,    SoPipKit,TRUE, this,     \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(
        vc,    SoVCKit,TRUE, this,     \0, TRUE);    
    SO_KIT_ADD_CATALOG_ENTRY(
        display,    SoDisplayKit,TRUE, this,     \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(
        auxGeometry, SoSeparator ,TRUE, this,   \0, TRUE);

    // add field
    SO_NODE_ADD_FIELD(color ,(0,0.25,0.5));

    SO_KIT_ADD_FIELD(userID, (-1)); 
	SO_KIT_ADD_FIELD(exclusiveGeometry, (NULL));
    SO_KIT_ADD_FIELD(local, (TRUE));

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

}


//----------------------------------------------------------------------------

SoUserKit::~SoUserKit()
{
    if (systemBar)
        systemBar->unref();
    if (taskBar)
        taskBar->unref();
    if (windowBar)
        windowBar->unref();
}

//----------------------------------------------------------------------------

SoPenKit *
SoUserKit::getPen()
{
	return SO_CHECK_ANY_PART(this,"pen",SoPenKit);
}

//----------------------------------------------------------------------------

SoPipKit *
SoUserKit::getPip()
{
	return SO_CHECK_ANY_PART(this,"pip",SoPipKit);
}

//----------------------------------------------------------------------------

SoVCKit*
SoUserKit::getVC()
{
  return SO_CHECK_ANY_PART(this,"vc",SoVCKit);
}
//----------------------------------------------------------------------------

SoDisplayKit *
SoUserKit::getDisplay()
{
	return SO_CHECK_ANY_PART(this, "display", SoDisplayKit);
}

//----------------------------------------------------------------------------

SoVCManager*
SoUserKit::getVCManager()
{
  return vcManager;
}
//----------------------------------------------------------------------------

SoSeparator *
SoUserKit::getAuxGeometry()
{
	return SO_GET_ANY_PART(this, "auxGeometry", SoSeparator);
}

//----------------------------------------------------------------------------

void 
SoUserKit::clearAuxGeometry()
{
	setPart("auxGeometry", NULL);
}

//----------------------------------------------------------------------------

int
SoUserKit::setUserID(int userID)
{
    if (SoUserManagerKit::getInstance()->getUserKit(this->userID.getValue())) // check if kit is checked in
        return FALSE;
    this->userID.setValue(userID);
    SoContext * context = SO_GET_PART(this, "context", SoContext);
    if(context != NULL)
    {
        context->value.setValue(SbString(userID));
        context->mode.setValue(SoContext::ADD);
        context->index.setValue(StudierstubeContext::Owner);
    }

    return TRUE;
}

//----------------------------------------------------------------------------

int
SoUserKit::getUserID()
{
    return userID.getValue();
}

//----------------------------------------------------------------------------

int
SoUserKit::getCurrentApp()
{
    return currentApp;
}

//----------------------------------------------------------------------------

void
SoUserKit::setFocus(SoWindowKit *window)
{
    focus = window;
    setWindowButtons();
}

//----------------------------------------------------------------------------
void
SoUserKit::setWindowButtons()
{
	if (getPip() == NULL)
		return;

    assert(windowBar); assert(skin);
    if (focus && currentApp != 0) 
    {
        SoTexture2 *texture;
        if (focus->state.getValue() != SoWindowKit::MINIMIZED)
            UAVGETNODE(texture,SoTexture2,"WINDOW_TEXTURE_MIN",skin)
        else
            UAVGETNODE(texture,SoTexture2,"WINDOW_TEXTURE_RESTORE",skin);
        windowBar->getButtonKit(WIN_MIN)->setTexture(texture);

        if (focus->state.getValue() != SoWindowKit::MAXIMIZED)
            UAVGETNODE(texture,SoTexture2,"WINDOW_TEXTURE_MAX",skin)
        else
            UAVGETNODE(texture,SoTexture2,"WINDOW_TEXTURE_RESTORE",skin);
        windowBar->getButtonKit(WIN_MAX)->setTexture(texture);

        if (focus->style.getValue() & SoWindowKit::BORDER) 
            UAVGETNODE(texture,SoTexture2,"WINDOW_TEXTURE_BORDER_OFF",skin)
        else 
            UAVGETNODE(texture,SoTexture2,"WINDOW_TEXTURE_BORDER_ON",skin)
        windowBar->getButtonKit(WIN_BORDER)->setTexture(texture);
        windowBar->showTabs(TRUE);
    }
    else 
        windowBar->showTabs(FALSE);
    
    getPip()->updatePip();
}

//----------------------------------------------------------------------------
void
SoUserKit::updateWindowButtons(SoWindowKit *focus)
{
    assert(focus);
    for (int i = 0;i<SoUserManagerKit::getInstance()->getUserNumber();i++)
        if (SoUserManagerKit::getInstance()->getUserKitSequentially(i)->focus == focus)
            SoUserManagerKit::getInstance()->getUserKitSequentially(i)->setWindowButtons();
}

//----------------------------------------------------------------------------
SoWindowKit *
SoUserKit::getFocus()
{
    return focus;
}

//----------------------------------------------------------------------------

void
SoUserKit::freeResources(SbAppID appID)
{
    if (getPip())
      getPip()->freeResources(appID);

    if (vcManager)
      vcManager->freeResources(appID);                
}

//----------------------------------------------------------------------------

void
SoUserKit::createResources(SbAppID appID, SbName appName)
{
    if (getPip())
    {
      getPip()->createResources(appID);

      PullDownCallbackData *data = new PullDownCallbackData;
      data->parent = this;
      data->appID = appID;
      assert(taskBar->getButtonKit(appID));
      taskBar->getButtonKit(appID)->
        addPullDownButton(appName,
        pullDownCB,data);
    }

    if (vcManager)
    {
      vcManager->createResources(appID);     
    }
}

//----------------------------------------------------------------------------

void
SoUserKit::useResources(SbAppID appID)
{   
    if( currentApp != appID )
    {
        SoApplicationKit * app = SoContextManagerKit::getInstance()->getApp( currentApp );
        SoContextKit * ctx = NULL;
        if( NULL != app )
            ctx = app->getContext();
        if( NULL != ctx )
            ctx->focusChange( userID.getValue(), FALSE );
        app = SoContextManagerKit::getInstance()->getApp( appID );
        ctx = NULL;
        if( NULL != app )
            ctx = SoContextManagerKit::getInstance()->getApp( appID )->getContext();
        if( NULL != ctx )
            ctx->focusChange( userID.getValue(), TRUE );

        // store appID of user's current app
        currentApp = appID;
    }
    
  if (getPip())
  {
    taskBar->showTabs(FALSE);

    // switch to application pip-sheet
    getPip()->useResources(appID);

    if (appID == 0)
    {
        windowBar->showTabs(FALSE);
        taskBar->showTabs(TRUE);
    }

    // set active button
    taskBar->setActiveButton(taskBar->getButtonKit(appID));

    // update texture of active task button
    if (taskBar->getButtonKit(appID))
        systemBar->getButtonKit(ACTIVE_TASK)->
            copyTexture(taskBar->getButtonKit(appID));
  }

  if (vcManager)
  {
    vcManager->useResources(appID);
  }
}

//----------------------------------------------------------------------------

void
SoUserKit::setMainPanelGeometry(SbAppID appID, SoNode* mpGeom)
{
  if(vcManager)
    vcManager->setMainPanelGeometry(appID, mpGeom);
}

//----------------------------------------------------------------------------


void
SoUserKit::setPipGeometry(SbAppID appID, SoNode* pipGeom, SbBool usePipCube)
{
	if (getPip() == NULL)
		return;
    getPip()->setPipGeometry(appID, pipGeom, usePipCube);
}

//----------------------------------------------------------------------------

void
SoUserKit::setupUserForWorkspace(SoSeparator *geometryRoot)
{
	// setup user root
	// set name for root
	char buf[40]; 
    root = new SoSeparator;
    sprintf(buf,"ROOT_USER%d",getUserID());
    root->setName(SbName(buf));
	// add context node for render user id
    SoContext * context = new SoContext;
    context->index.setValue( StudierstubeContext::User );
    context->value.setValue( getUserID());
    context->mode.setValue( SoContext::ADD );
    root->addChild( context );
   
    // set context node for owner id of SoUserKit and subgraphs
    context = SO_GET_PART(this, "context", SoContext);
    if(context != NULL)
    {
        context->value.setValue(SbString(getUserID()));
        context->mode.setValue(SoContext::ADD);
        context->index.setValue(StudierstubeContext::Owner);
    }

    if(getVC())
    {
        vcManager = new SoVCManager();
        vcManager->setVC(getVC());
        vcManager->setUserKit(this);
        vcManager->setupBars();   
        vcManager->createResources(0);
        vcManager->useResources(0);
    }
    else
        printf("WARNING: user with ID %d has no VC\n",getUserID());    
    
	// add exclusive geometry to user's graph
	exclusiveGeometryRoot = new SoSeparator;
	root->addChild(exclusiveGeometryRoot);
 	for (int i=0;i<exclusiveGeometry.getNum();i++)
		if (exclusiveGeometry[i])
			exclusiveGeometryRoot->addChild(exclusiveGeometry[i]);
	// add scene geometry to user's graph
    root->addChild(geometryRoot);

	// set root for pen(s), if pen exists
	if (getPen())
        getPen()->setEventRoot(geometryRoot);
	else
        printf("WARNING: user with ID %d has no PEN\n",getUserID());
    
    // setup tabs for userKit's Pip, if pip exists
    if (getPip())
    {        
        // create System Resources
        setupTabs();
                
        getPip()->createResources(0);
        
        SoPipSheetKit* pipS=(SoPipSheetKit*)readNodeFromFile("system/systemsheet.iv", 
            StbWorkspace::getInstance()->getWorkspaceDirectory().getString());
        assert(pipS != NULL && "SoUserKit could not load system/systemsheet.iv !");
        
        SoSeparator* loaderSheet=stbLoader->setupLoaderSheet();
        stbLoader->fixFilter();
        
        pipS->addSheet(loaderSheet, NULL);
        
        getPip()->setPipGeometry(0,pipS,TRUE);
        
        getPip()->useResources(0);
        
        char buffer[100];
        
        sprintf(buffer,"OPEN_BUTTON");
        So3DButton* bOpen = (So3DButton*)findNode(getPip()->getPipSheetKit(0),buffer);
        sprintf(buffer,"SAVE_BUTTON");
        So3DButton* bSave = (So3DButton*)findNode(getPip()->getPipSheetKit(0),buffer);
        sprintf(buffer,"SAVEAS_BUTTON");
        So3DButton* bSaveAs = (So3DButton*)findNode(getPip()->getPipSheetKit(0),buffer);
        sprintf(buffer,"SAVEALL_BUTTON");
        So3DButton* bSaveAll = (So3DButton*)findNode(getPip()->getPipSheetKit(0),buffer);
	    sprintf(buffer,"SHUTDOWN_BUTTON");
    	So3DButton* bShutdown = (So3DButton*)findNode(getPip()->getPipSheetKit(0),buffer);
        
        bOpen->addReleaseCallback(systemMenuCB,this);
        bSave->addReleaseCallback(systemMenuCB,this);
        bSaveAs->addReleaseCallback(systemMenuCB,this);
        bSaveAll->addReleaseCallback(systemMenuCB,this);
        bShutdown->addReleaseCallback(systemMenuCB, this);

    }
	else
		printf("WARNING: user with ID %d has no PIP\n",getUserID());

	if (getDisplay() == NULL)
		printf("WARNING: user with ID %d has no DISPLAY\n",getUserID());
}


//----------------------------------------------------------------------------
void
SoUserKit::setupTabs()
{
    if (getPip() == NULL)
		return;

        // load skin
    SbString name = StbWorkspace::getInstance()->getWorkspaceDirectory();
    name += "graphix/";
    getPip()->skinDirectory = name;
    name += getPip()->skinFileName.getValue();    
    skin = StbSkinLoader::getSkin(name.getString());
    assert(skin);

        // create system tab bar
    systemBar = new SoTabKit(4,skin,"SYSTEM_BAR_");
    systemBar->ref();

        // add active task button to system bar
    systemBar->addTab(ACTIVE_TASK,systemBarCB,this,NULL);

    SoTexture2* texture=NULL;

    UAVGETNODE(texture,SoTexture2,"SYSTEM_MENU_TEXTURE",skin);
    systemBar->addTab(SYSTEM_MENU,systemBarCB,this,texture);


    UAVGETNODE(texture,SoTexture2,"SYSTEM_CLOSE_APP_TEXTURE",skin);
    systemBar->addTab(CLOSE_APP,systemBarCB,this,texture);
       // set transform node for system bar
    UAGETNODE(start,SoTranslation,"SYSTEM_BAR_START_TRANSLATION",skin);
    UAGETNODE(size_,SoInfo,"SYSTEM_BAR_SIZE",skin);
    float size;
    sscanf(size_->string.getValue().getString(),"%f",&size);

    systemBar->setupTransform(start->translation.getValue(),size,
                              getPip()->pipSize.getValue());

        // create task bar    
    UAGETNODE(maxTabs_,SoInfo,"TASK_BAR_MAX_TAB_BUTTONS",skin);
    int maxTabs = atoi(maxTabs_->string.getValue().getString());
    taskBar = new SoTabKit(maxTabs,skin,"TASK_BAR_");    
    taskBar->ref();
    taskBar->showActiveButton(TRUE);
        // set transform for task bar    
    UAVGETNODE(start,SoTranslation,"TASK_BAR_START_TRANSLATION",skin);
    UAVGETNODE(size_,SoInfo,"TASK_BAR_SIZE",skin);
    sscanf(size_->string.getValue().getString(),"%f",&size);

    taskBar->setupTransform(start->translation.getValue(),size,
                            getPip()->pipSize.getValue());
    taskBar->showTabs(FALSE);

    // create window bar    
    windowBar = new SoTabKit(3,skin,"WINDOW_BAR_");    
    windowBar->ref();

    texture = NULL;
    windowBar->addTab(WIN_MIN,windowBarCB,this,texture);
    windowBar->addTab(WIN_MAX,windowBarCB,this,texture);
    windowBar->addTab(WIN_BORDER,windowBarCB,this,texture);

        // set transform for window bar    
    UAVGETNODE(start,SoTranslation,"WINDOW_BAR_START_TRANSLATION",skin);
    UAVGETNODE(size_,SoInfo,"WINDOW_BAR_SIZE",skin);
    sscanf(size_->string.getValue().getString(),"%f",&size);

    windowBar->setupTransform(start->translation.getValue(),size,
                              getPip()->pipSize.getValue());

    // hook system, task and window bar to pip
    getPip()->useSystemBar(systemBar);
    getPip()->useTaskBar(taskBar);
    getPip()->useWindowBar(windowBar); 

    stbLoader = new fileBrowser;

    stbLoader->setFilter(".iv",FALSE);
    stbLoader->setFileButtonCallback(browseCB, this);



}

void 
SoUserKit::browseCB(void* data, const char *filePath)
{
    SoNodeList alist;
	SoUserKit* uk=(SoUserKit*)data;
	
    // don't start applications, if the user is not configured
    // for the local instance. Only the instance that
    // configured this user should start an application
    if( uk->local.getValue() == FALSE )
        return;

	if(SoContextManagerKit::getInstance()->loadApplication(filePath, alist, uk->getUserID(),
														   StbWorkspace::getInstance()->getDefaultLocale().getString()) >= 0)
	{
	    uk->useResources(((SoApplicationKit*)alist[0])->getContext()->appID.getValue());
		uk->getPip()->updatePip();
		uk->getPip()->getPipSheetKit(0)->switchPipSheet(0);
	}
}
//----------------------------------------------------------------------------

void
SoUserKit::removeTaskBarButton(SbAppID appID)
{
    if (getPip()) {
        taskBar->removeTab(appID);
    }
}

//----------------------------------------------------------------------------

void
SoUserKit::addTaskBarButton(SbAppID appID, SoTexture2 *texture)
{
	if (getPip() == NULL)
		return;

        PullDownCallbackData *data = new PullDownCallbackData;
        data->parent = this;
        data->appID = appID;
        taskBar->addTab(appID,pullDownCB,data,texture);
}

//----------------------------------------------------------------------------

void
SoUserKit::addTaskBarElement(SbAppID appID)
{
  // TODO: use vcManager
}

//----------------------------------------------------------------------------

void
SoUserKit::removeTaskBarElement(SbAppID appID)
{
  // TODO: use vcManager
}

//----------------------------------------------------------------------------




void
SoUserKit::systemMenuCB(void* userData, So3DButton* button)
{
    SoUserKit *uk = (SoUserKit*)userData;
    
    if(button->getName()=="OPEN_BUTTON")
    {
        // show fileBrowser
        uk->getPip()->getPipSheetKit(0)->switchPipSheet(1);
        uk->stbLoader->fixFilter(TRUE);
    }
    else
    if(button->getName()=="SAVE_BUTTON")
    {
        SoContextManagerKit::getInstance()->saveApplication(uk->currentApp);
    }
    else
    if(button->getName()=="SAVEAS_BUTTON")
    {
        SoContextManagerKit::getInstance()->saveApplicationAs(uk->currentApp);
    }
    else
    if(button->getName()=="SAVEALL_BUTTON")
    {
        SoContextManagerKit::getInstance()->saveAll();
    }
    else
    if(button->getName()=="SHUTDOWN_BUTTON")
    {
	  SoGui::exitMainLoop();
    }
    uk->getPip()->updatePip();
}

//----------------------------------------------------------------------------
void
SoUserKit::systemBarCB(void* userData, So3DButton* button)
{
    SoUserKit *uk = (SoUserKit*)userData;

	assert(uk);

    uk->systemBarMessageSent = FALSE;
    CHECKFORMASTER;

	int id  = uk->systemBar->getButtonID(button);
	int tabInfo = !uk->taskBar->isVisible();

    if(!uk->systemBarLocked) 
    {
        StbDistrManager::getInstance()->systemBarChanged( 
            uk->getUserID(), id, tabInfo );
        uk->systemBarMessageSent = TRUE;
    }
    uk->systemBarLocked = FALSE;
}

//----------------------------------------------------------------------------

void
SoUserKit::taskBarCB(void* userData, So3DButton* button)
{
    SoUserKit *uk = (SoUserKit*)userData;
	CHECKFORMASTER;
	int buttonId = uk->taskBar->getButtonID(button);
	uk->useResources(buttonId);
    StbDistrManager::getInstance()->taskBarChanged( uk->getUserID(), buttonId );
}

//----------------------------------------------------------------------------

void
SoUserKit::windowBarCB(void* userData, So3DButton* button)
{
    SoUserKit *uk = (SoUserKit*)userData;
    CHECKFORMASTER;

    int id  = uk->windowBar->getButtonID(button);
    if (uk->focus)
    {
        switch (id)
        {
            case WIN_BORDER: 
			{
                uk->focus->style =
                   (uk->focus->style.getValue() ^ SoWindowKit::BORDER);
                break;
			}
            case WIN_MIN:   // minimize
                if (uk->focus->state.getValue() != SoWindowKit::MINIMIZED)
                    uk->focus->state.setValue(SoWindowKit::MINIMIZED);
                else
                    uk->focus->state.setValue(SoWindowKit::NORMAL);
                break;
            case WIN_MAX: // maximize
                if (uk->focus->state.getValue() != SoWindowKit::MAXIMIZED)
                    uk->focus->state.setValue(SoWindowKit::MAXIMIZED);
                else
                    uk->focus->state.setValue(SoWindowKit::NORMAL);
                break;
            default: break;
        }
        updateWindowButtons(uk->focus);
        StbDistrManager::getInstance()->windowBarChanged(
            uk->getUserID(),uk->focus->style.getValue(), uk->focus->state.getValue());
   }
}

//----------------------------------------------------------------------------

void
SoUserKit::pullDownCB(void* userData, So3DButton* button)
{
    PullDownCallbackData *data = (PullDownCallbackData*)userData;

    data->parent->taskBar->showTabs(FALSE);

    SoContextKit *ctx;
    ctx = SoContextManagerKit::getInstance()->getApp(data->appID)->getContext();
    assert(ctx);
	if(ctx->getMasterMode()==TRUE)
	{
        ctx->forceFocusChange(data->parent->userID.getValue());
        StbDistrManager::getInstance()->focusChanged(
            data->parent->userID.getValue(), ctx->appID.getValue());
	}
}

//----------------------------------------------------------------------------

//eof

