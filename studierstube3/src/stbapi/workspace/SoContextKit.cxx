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
/** Studierstube foundation class implementation
  *
  * @author  Dieter Schmalstieg, Andreas Zajic
  *
  * $Id: SoContextKit.cxx 4113 2004-12-21 14:27:35Z breiting $
  * @file  SoContextKit.cxx                                                 */
 /* ======================================================================= */

#include <cstdio>
#include <cstdlib>
#include <cassert>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>
#include <stbapi/workspace/appdef.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/distrManager/StbDistrManager.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/context/SoContext.h>
#include <stbapi/context/StudierstubeContext.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/vc/SoVCMainPanelKit.h>

SO_KIT_SOURCE(SoContextKit);

SbBool SoContextKit::baseInitFlag = FALSE;  //prevent multiple initClass()

//----------------------------------------------------------------------------

void
SoContextKit::initClass()
{
    if(baseInitFlag) return;
    baseInitFlag = TRUE;

    SO_KIT_INIT_CLASS(SoContextKit, SoBaseKit, "BaseKit");
    (void *) SoContextManagerKit::getInstance(); // init
}

//----------------------------------------------------------------------------
// default constructor

SoContextKit::SoContextKit()
: isMaster(TRUE),
agroup(NULL)
{
    SO_KIT_CONSTRUCTOR(SoContextKit);

    SO_KIT_ADD_FIELD(appID,(-1));
    SO_KIT_ADD_FIELD(userID,(-1));

    //-----------------------name---|-type-------|nuldef|par |rite|public

    SO_KIT_ADD_CATALOG_ENTRY(context,   SoContext, FALSE, this, \x0, FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(windowGroup, SoGroup, FALSE, this, \x0, TRUE );

	SO_KIT_ADD_FIELD(templatePipSheet, (NULL));
    SO_KIT_ADD_FIELD(clonePipSheet, (FALSE));
    SO_KIT_ADD_FIELD(templateMainPanelFilename, (""));
    SO_KIT_ADD_FIELD(cloneMainPanel, (FALSE));

    SO_KIT_ADD_FIELD(exitTrigger, ());
  
    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);
    modeAction=new SoHandle3DEventAction;
    modeAction->setTraverseAll(TRUE);

	exitTriggerSensor.setData(this);
	exitTriggerSensor.setPriority(1);
	exitTriggerSensor.setFunction(exitTriggerCB);

}

//----------------------------------------------------------------------------

void SoContextKit::exitTriggerCB( void * userdata, SoSensor * sensor )
{
	printf("SoContextKit::exitTriggerCB\n");
	//((SoContextKit*)userdata)->context.getValue()->touch();
	
	//TODO must process an event to end mainloop ??!??!

	SoGui::exitMainLoop();	

//	SoKeyboardEvent *quitEvent = new SoKeyboardEvent();
//	quitEvent->setKey(SoKeyboardEvent::X);
//	SoGui::dispatchEvent(quitEvent);

		
	//	SoOneShotSensor *sen = new SoOneShotSensor(exitMainLoop,userdata);
//	sen->setPriority(100);
//	sen->schedule();
}


//----------------------------------------------------------------------------

SbBool
SoContextKit::initContext(SbBool masterMode)
{
	int i;

    // setup context node
    SoContext * ctxNode = (SoContext *) context.getValue();
    assert(ctxNode);
    ctxNode->mode.setValue( SoContext::ADD );
    ctxNode->index.setValue( StudierstubeContext::Application );
    ctxNode->value.setValue( appID.getValue());

    // check geometry
    if (!checkWindowGeometry()) return FALSE;

    if (!checkPipGeometry()) return FALSE;
    
    if(!checkMainPanelGeometry()) return FALSE;

    // register Pipsheets for all users
    const int32_t *userIDs = SoUserManagerKit::getInstance()->getUserIDs();
    for(i=0; i<SoUserManagerKit::getInstance()->getUserNumber(); i++)
    {
        registerPipSheet(userIDs[i]);
        registerMainPanel(userIDs[i]);
    }

    // register application callback with windows
    for(i=0; i<getNumWindows(); i++) 
    {
        SoWindowKit* w = getWindow(i);        
        w->setApplicationCallback(staticWindowEventCB, this);
	}

    // register
    SoContextManagerKit::getInstance()->setMasterMode(getAppGroup(), masterMode);

    //also set up own master/slave status
    setMasterMode(masterMode); 
    
    forceFocusChange();

	exitTriggerSensor.attach(&exitTrigger);

    return TRUE;
}
//----------------------------------------------------------------------------
SbBool
SoContextKit::registerPipSheet(int uid)
{
    
    SoNode* pip = templatePipSheet.getValue();
    
    if (SoUserManagerKit::getInstance()->getPipFromUser(uid) != -1)
    {
        // search for taskbar texture
        
        SoSearchAction sAction;    
        SoApplicationKit* appKit=getAppKit();
        appKit->ref();
        sAction.setType(SoTexture2::getClassTypeId());
        sAction.setSearchingAll(TRUE);
        sAction.apply(appKit->getAppGeom());
        SoPath *sPath = sAction.getPath();
        SoTexture2 *texture = NULL;
        if (sPath)
        {
            texture = (SoTexture2 *)sPath->getTail();
        }
        appKit->unref();
        
        // add taskbar button
        SoUserKit * user = SoUserManagerKit::getInstance()->getUserKit( uid );
        assert(user);
        user->addTaskBarButton(appID.getValue(),texture);        
        user->createResources(appID.getValue(), getAppKit()->getName());        
        if(pip)
        {
            if (clonePipSheet.getValue()) {
                pip = copyNode( pip, TRUE, TRUE );
                user->setPipGeometry(appID.getValue(), pip, TRUE);
            }
            else
            {
                user->setPipGeometry(appID.getValue(), pip, TRUE);
            }
            checkPipConnections( uid, pip );
        }
        return TRUE;
    }    
    return FALSE;
}
//----------------------------------------------------------------------------

SbBool
SoContextKit::registerMainPanel(int uid)
{
    SoNode* mpk = readNodeFromFile(templateMainPanelFilename.getValue().getString(),
                                   StbWorkspace::getInstance()->getWorkspaceDirectory().getString());

    if (SoUserManagerKit::getInstance()->getVCFromUser(uid) != -1)
    {
        // add taskBar widget
        SoUserKit * user = SoUserManagerKit::getInstance()->getUserKit( uid );
        assert(user);
        user->addTaskBarElement(appID.getValue());
        user->createResources(appID.getValue(), getAppKit()->getName());
        if(mpk)
        {
            if (cloneMainPanel.getValue()) {
                mpk = copyNode( mpk, TRUE, TRUE );
                user->setMainPanelGeometry(appID.getValue(), mpk);
            }
            else
            {
                user->setMainPanelGeometry(appID.getValue(), mpk);
            }
            checkMainPanelConnections( uid, mpk );
        }
        return TRUE;
    }
    return FALSE;
}

//----------------------------------------------------------------------------
void
SoContextKit::forceFocusChange()
{    
	int userid = userID.getValue();
    SoUserKit * userKit = SoUserManagerKit::getInstance()->getUserKit(userid);
    if( userKit != NULL )
    {        
        // switch the focus and pip sheet of that particular user
        // who created the window to that new window
        if( getNumWindows() > 0 )
        {
            userKit->setFocus(getWindow(0));
        }
        userKit->useResources(appID.getValue());
    }
}

//----------------------------------------------------------------------------
void
SoContextKit::forceFocusChange(int userID)
{    
    // switch the focus and pip sheet to the first window of this context
    // for user with userID
    SoUserKit * userKit = SoUserManagerKit::getInstance()->getUserKit(userID);
    if( getNumWindows() > 0)
    {
        userKit->setFocus(getWindow(0));
    }
    userKit->useResources(appID.getValue());
}
//----------------------------------------------------------------------------

void
SoContextKit::focusChange( int uid, SbBool focus  )
{
    // nil
}

//----------------------------------------------------------------------------
void
SoContextKit::exitContext()
{
    // probably not necessary, because the scene graph will go away anyway ?
    /*
   for(int i=0; i<getNumWindows(); i++) 
	{
		if(getWindow(i) != NULL) closeWindow(i);
        ((SoGroup*)windowGroup.getValue())->removeChild(i);
	}
    */
   delete modeAction;
}
//----------------------------------------------------------------------------
// destructor

SoContextKit::~SoContextKit()
{
    // do not call destructor() here, call in subclass
}

//----------------------------------------------------------------------------

SoWindowKit*
SoContextKit::getWindow(int i)
{
    if( i < 0 )
        return NULL;
    SoSearchAction search;
    search.setType( SoWindowKit::getClassTypeId());
    search.setInterest( SoSearchAction::ALL );
    search.setSearchingAll( TRUE );
    search.apply( windowGroup.getValue());
    SoPathList paths = search.getPaths();
    if( i >= paths.getLength())
        return NULL;
    return (SoWindowKit *)((SoFullPath*)paths[i])->getTail();
}

//----------------------------------------------------------------------------

int
SoContextKit::getNumWindows()
{
    SoSearchAction search;
    search.setType( SoWindowKit::getClassTypeId());
    search.setInterest( SoSearchAction::ALL );
    search.setSearchingAll( TRUE );
    search.apply( windowGroup.getValue());
    SoPathList paths = search.getPaths();
    return paths.getLength();
}

//----------------------------------------------------------------------------

void
SoContextKit::closeWindow(int i)
{
    //@todo what should happen here ? any real interesting functionality or should
    // it be removed and users just have to kill windows themselves (i.e.
    // remove them from the scenegraph ???
}

//----------------------------------------------------------------------------

SoNode*
SoContextKit::getPipSheet(int uid)
{
    return SoContextManagerKit::getInstance()->getPipSheet(uid, appID.getValue());
}

//----------------------------------------------------------------------------

SoNode*
SoContextKit::getMainPanel(int uid)
{
    return SoContextManagerKit::getInstance()->getMainPanel(uid, appID.getValue());
}

//----------------------------------------------------------------------------

SbBool
SoContextKit::staticWindowEventCB(
    void* data,
    int messageId, 
    SoWindowKit* win,
    int uid, 
    So3DEvent* event, 
    SoPath* path
    )
{
    SoContextKit* self = (SoContextKit*)data;
//printf("STATICWINDOWEVENTCB context %x window event %d\n",self,messageId);
    if( self == NULL )
    {
        printf("WARNING: illegal context %p encountered\n", (void*)self);
            return FALSE;
    }

    // if we got the focus, switch pip
	SbBool res = FALSE;
    if(messageId == CB_WINDOW_WILL_GET_FOCUS)
    {
        //printf("STATICWINDOWEVENTCB window will get focus\n");
        // switch to pip sheet for user <uid>
 
        SoUserManagerKit::getInstance()->getUserKit(uid)->useResources(self->appID.getValue());

        res = TRUE;
    }
    else if(messageId == WM_EVENT)
    {
        if(!self->isMaster && uid != -1)
        {
            if (((SoUserManagerKit::getInstance()->getUserKit(uid)->getPen()->
               focusPolicy.getValue() == SoPenKit::CLICK_TO_MASTER) &&
              (event->getType() == STB_PRIMARY_BUTTON_EVENT) &&
              (event->getButton(STB_PRIMARY_BUTTON) == TRUE)))
            {
                // pull trigger on the div group to get master !
                self->agroup->getMaster.setValue();
            }
        }
    }

    // now forward the window event to the user derived callback
	if(self->isMaster) // forward only to a master context
		res = self->windowEventCB(data,messageId,win,uid,event,path);
    return res;
}

//----------------------------------------------------------------------------

SbBool
SoContextKit::windowEventCB(void* data,
                   int messageId, 
                   SoWindowKit* win,
                   int uid, 
                   So3DEvent* event, 
                   SoPath *path)

{
    //printf("base class windowEventCB\n");
    return FALSE; // do nothing
}

//----------------------------------------------------------------------------

void
SoContextKit::scheduleDelete()
{
    //send a message to ourselves to remove context later
    SoOneShotSensor * oneShot = new SoOneShotSensor( SoContextManagerKit::deleteContextCB, this );
    oneShot->setPriority( 1 );
    oneShot->schedule();
}

//----------------------------------------------------------------------------

SbBool
SoContextKit::checkPipGeometry()
{
    return TRUE;
}

//----------------------------------------------------------------------------

SbBool
SoContextKit::checkMainPanelGeometry()
{
   return TRUE;
}

//----------------------------------------------------------------------------

SbBool
SoContextKit::checkWindowGeometry()
{
    return TRUE;
}

void SoContextKit::checkMainPanelConnections( int uid, SoNode* mainPanelContents)
{
    // nil
}

void SoContextKit::checkMainPanelMasterMode( SoNode * mainPanelContents, SbBool masterMode )
{
    // nil
}

void SoContextKit::checkPipConnections( int uid, SoNode * pipSheetContents )
{
    // nil
}

void SoContextKit::checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode )
{
    // nil
}
//----------------------------------------------------------------------------

SbString
SoContextKit::getAppName()
{
    return getAppKit()->getName().getString();
}

//----------------------------------------------------------------------------

int
SoContextKit::getUserId()
{
    return userID.getValue();
}

//----------------------------------------------------------------------------

SbBool
SoContextKit::getMasterMode()
{
    return isMaster;
}

void
SoContextKit::setMasterMode(SbBool masterMode)
{
    int i;
    isMaster = masterMode;

    // deal with pip sheets
    SoPipSheetKit *pip=NULL;

    const int32_t *userIDs = SoUserManagerKit::getInstance()->getUserIDs();
    for(i=0; i<SoUserManagerKit::getInstance()->getUserNumber(); i++) 
	{
        if (SoUserManagerKit::getInstance()->getUserKit(userIDs[i])->getPip())
            pip = SoUserManagerKit::getInstance()->getUserKit(userIDs[i])->getPip()->
                                        getPipSheetKit(appID.getValue());
		if(pip)
		{
            setSheetMasterMode(pip, masterMode);
		}
	}
        
    SoVCMainPanelKit *mpk = NULL;

    for(i=0; i<SoUserManagerKit::getInstance()->getUserNumber(); i++)
    {
      if(SoUserManagerKit::getInstance()->getUserKit(userIDs[i])->getVC())
        mpk = SoUserManagerKit::getInstance()->getUserKit(userIDs[i])->getVC()->
                                        getMainPanelKit(appID.getValue());
      if(mpk)
      {
        setMainPanelMasterMode(mpk, masterMode);
      }
    }

    // set correct context 
    SoContext * ctxNode = (SoContext *) context.getValue();
    assert(ctxNode);
    int index = ctxNode->index.find( StudierstubeContext::DivMode );
    if( index == -1 )
    {
        ctxNode->index.set1Value(ctxNode->index.getNum(), StudierstubeContext::DivMode );
        ctxNode->value.set1Value(ctxNode->value.getNum(), masterMode );
    }
    else
    {
        ctxNode->index.set1Value(index, StudierstubeContext::DivMode);
        ctxNode->value.set1Value(index, masterMode );
    }
}

//----------------------------------------------------------------------------

void
SoContextKit::setSheetMasterMode(SoNode* pipSheet, SbBool masterMode)
{
    So3DEvent modeEvent;
    modeEvent.setType(masterMode?So3DEvent::ET_MASTERMODE_EVENT:So3DEvent::ET_SLAVEMODE_EVENT);
    modeAction->setEvent(&modeEvent);
    modeAction->apply(pipSheet);
    // call back application code
    checkPipMasterMode( pipSheet, masterMode );
}

//----------------------------------------------------------------------------

void
SoContextKit::setMainPanelMasterMode(SoNode* mpk, SbBool masterMode)
{ 
    So3DEvent modeEvent;
    modeEvent.setType(masterMode?So3DEvent::ET_MASTERMODE_EVENT:So3DEvent::ET_SLAVEMODE_EVENT);
    modeAction->setEvent(&modeEvent);
    modeAction->apply(mpk);
    // call back application code
    checkMainPanelMasterMode( mpk, masterMode );

}

//----------------------------------------------------------------------------

int
SoContextKit::getUserFromSheet(SoNode* nodeInSheet)
{
    return SoContextManagerKit::getInstance()->
        getUserFromSheet(nodeInSheet,appID.getValue());
}

//----------------------------------------------------------------------------

SoPipSheetKit*
SoContextKit::getSheetFromNode(SoNode* nodeInSheet)
{
    return SoContextManagerKit::getInstance()->
        getSheetFromNode(nodeInSheet, appID.getValue());
}

//----------------------------------------------------------------------------

SoVCMainPanelKit*
SoContextKit::getMainPanelFromNode(SoNode* nodeInMainPanel)
{
    return SoContextManagerKit::getInstance()->
        getMainPanelFromNode(nodeInMainPanel, appID.getValue());
}

//----------------------------------------------------------------------------

SbBool
SoContextKit::isNodeInContext(SoNode* nodeInContext)
{
    SoSearchAction sa;
    sa.setNode(nodeInContext);
    sa.setSearchingAll(TRUE);
    sa.apply(this);
    return((SoFullPath*)(sa.getPath()) != NULL);
}

//----------------------------------------------------------------------------

SbBool
SoContextKit::isNodeInSheet(SoNode* nodeInSheet)
{
	return(getUserFromSheet(nodeInSheet) != -1);
}

//----------------------------------------------------------------------------

SbBool
SoContextKit::isNodeAssociated(SoNode* associatedNode)
{
	if(isNodeInSheet(associatedNode)) return TRUE;
	return(isNodeInContext(associatedNode));
}

//----------------------------------------------------------------------------

void
SoContextKit::setUniqueNodeName(SoNode* node, int uid)
{
	SbString nodeName=node->getName().getString();
	if(!nodeName)
		nodeName+="EMPTY";
	nodeName+="_";
	nodeName+=SbString(uid);
    nodeName+="_";
    nodeName+=getName().getString();
    node->setName(nodeName);
}

//----------------------------------------------------------------------------

SbString&
SoContextKit::makeUniqueNodeName(SbString templateString, int uid)
{
	SbString *nodeName=new SbString;
	if(!templateString)
		*nodeName+="EMPTY";
	else
		*nodeName=templateString;
	*nodeName+="_";
	*nodeName+=SbString(uid);
	*nodeName+="_";
    *nodeName+=getName().getString();
    return *nodeName;
}

//----------------------------------------------------------------------------

void SoContextKit::setAppGroup(SoDivGroup* appGroup)
{ 
	agroup = appGroup;
	SoFieldSensor * sensor = new SoFieldSensor( SoContextKit::masterChangeCB, this );
	sensor->attach( &appGroup->isMaster );
}

//----------------------------------------------------------------------------

void SoContextKit::masterChangeCB( void * data, SoSensor * sensor )
{
	SoContextKit * self = (SoContextKit *) data;
	SbBool newMaster = self->agroup->isMaster.getValue();
	if( newMaster != self->isMaster )
	{
		SoContextManagerKit::getInstance()->setMasterMode( self->agroup, newMaster );
		self->setMasterMode( newMaster );
	}
}

//eof
