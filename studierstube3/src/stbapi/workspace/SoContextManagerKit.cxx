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
/** Implementation of class SoContextManagerKit
 *
 * @author  Dieter Schmalstieg, Gerd Hesina, Andreas Zajic
 *
 * $Id: SoContextManagerKit.cxx 4113 2004-12-21 14:27:35Z breiting $
 * @file  SoContextManagerKit.cxx                                                                 */
/* ======================================================================= */

#include <iostream>
#include <cstdio>
#include <string>

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <cassert>
#include <sys/stat.h>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>
#include <stbapi/util/os.h>

#include <stbapi/workspace/distrManager/StbDistrManager.h>

#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/workspace/defines.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/workspace/SoLocaleManagerKit.h>

#include <stbapi/vc/SoVCMainPanelKit.h>

using namespace std;

SO_KIT_SOURCE(SoContextManagerKit);

//----------------------------------------------------------------------------

SoContextManagerKit* SoContextManagerKit::instance = NULL;

void
SoContextManagerKit::initClass()
{
    SO_KIT_INIT_CLASS(SoContextManagerKit,SoBaseKit,"BaseKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoContextManagerKit::SoContextManagerKit()
{    
    SO_KIT_CONSTRUCTOR(SoContextManagerKit);
    
    //       -----------------------name---|-type-------|nuldef|-par |rite|public
    SO_KIT_ADD_CATALOG_ENTRY(masterContexts,     SoGroup, FALSE, this,\x0 ,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(slaveContexts ,     SoGroup, FALSE, this,\x0 ,FALSE);
    
    SO_KIT_INIT_INSTANCE();
    
    setSearchingChildren(TRUE);
    
    modeAction=new SoHandle3DEventAction;
    modeAction->setTraverseAll(TRUE);
    
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoContextManagerKit::~SoContextManagerKit()
{
    delete modeAction;
}

//----------------------------------------------------------------------------
// singleton accessor
//----------------------------------------------------------------------------

SoContextManagerKit*
SoContextManagerKit::getInstance()
{
    if(instance == NULL) instance = new SoContextManagerKit();
    return instance;
}

//----------------------------------------------------------------------------
// add context to system or change its master/slave replication state
//----------------------------------------------------------------------------

void
SoContextManagerKit::setMasterMode(SoDivGroup *appGroup, SbBool isMaster)
{
    So3DEvent modeEvent;
    if(isMaster)
    {
        int index = ((SoGroup*)masterContexts.getValue())->findChild(appGroup);
        if(index == -1) 
        {
            ((SoGroup*)masterContexts.getValue())->addChild(appGroup);
        }            
        modeEvent.setType(So3DEvent::ET_MASTERMODE_EVENT);
        modeAction->setEvent(&modeEvent);
        modeAction->apply(appGroup);
        if(((SoGroup*)slaveContexts.getValue())->findChild(appGroup) != -1)
            ((SoGroup*)slaveContexts.getValue())->removeChild(appGroup);
    }
    else // is slave
    {
        int index = ((SoGroup*)slaveContexts.getValue())->findChild(appGroup);
        if(index == -1) ((SoGroup*)slaveContexts.getValue())->addChild(appGroup);
        modeEvent.setType(So3DEvent::ET_SLAVEMODE_EVENT);
        modeAction->setEvent(&modeEvent);
        modeAction->apply(appGroup);                    
        if(((SoGroup*)masterContexts.getValue())->findChild(appGroup) != -1)
            ((SoGroup*)masterContexts.getValue())->removeChild(appGroup);
    }
    // in any case we send the value as to the sman2
    StbDistrManager::getInstance()->setApplicationMode(((SoApplicationKit*)(appGroup->getChild(0)))->getContext()->appID.getValue(), isMaster );
}

//----------------------------------------------------------------------------

SoApplicationKit*
SoContextManagerKit::getApp(SbAppID appid)
{
    int i;
    
    SoGroup* appGroup;
    
    for(i=0;i<((SoGroup*)masterContexts.getValue())->getNumChildren();i++)
    {
        appGroup=(SoGroup*)((SoGroup*)masterContexts.getValue())->getChild(i);
        if (((SoApplicationKit*)(appGroup->getChild(0)))->getContext()->appID.getValue()==appid)
            return (SoApplicationKit*)(appGroup->getChild(0));
    }
    for(i=0;i<((SoGroup*)slaveContexts.getValue())->getNumChildren();i++)
    {
        appGroup=(SoGroup*)((SoGroup*)slaveContexts.getValue())->getChild(i);
        if (((SoApplicationKit*)(appGroup->getChild(0)))->getContext()->appID.getValue()==appid)
            return (SoApplicationKit*)(appGroup->getChild(0));
    }
    return NULL;
}

SoApplicationKit * SoContextManagerKit::getAppByName( const SbName & name )
{
    SoSearchAction search;
    search.setType( SoApplicationKit::getClassTypeId());
    search.setName( name );
    search.setSearchingAll( TRUE );
    search.apply( this );
    
    if( search.getPath() != NULL )
        return (SoApplicationKit *)((SoFullPath *)search.getPath())->getTail();
    return NULL;
}


//----------------------------------------------------------------------------

SoDivGroup*
SoContextManagerKit::getAppGroup(SbAppID appid)
{
    int i;
    SoDivGroup* appGroup;
    
    for(i=0;i<((SoGroup*)masterContexts.getValue())->getNumChildren();i++)
    {
        appGroup=(SoDivGroup*)((SoGroup*)masterContexts.getValue())->getChild(i);
        if (((SoApplicationKit*)(appGroup->getChild(0)))->getContext()->appID.getValue()==appid)
            return appGroup;
    }
    for(i=0;i<((SoGroup*)slaveContexts.getValue())->getNumChildren();i++)
    {
        appGroup=(SoDivGroup*)((SoGroup*)slaveContexts.getValue())->getChild(i);
        if (((SoApplicationKit*)(appGroup->getChild(0)))->getContext()->appID.getValue()==appid)
            return appGroup;
    }
    return NULL;
}        

//----------------------------------------------------------------------------
// get number of applications currently managed by SoContextManagerKit
//----------------------------------------------------------------------------

int
SoContextManagerKit::getAppNum(SbBool isMaster)
{
    SoGroup* g = isMaster ? ((SoGroup*)masterContexts.getValue()) : ((SoGroup*)slaveContexts.getValue());
    
    return g->getNumChildren();
}

//----------------------------------------------------------------------------

int
SoContextManagerKit::getAppNum()
{
    return getAppNum(TRUE)+getAppNum(FALSE);
}

//----------------------------------------------------------------------------
// get a master div object which is responsible for a given node
//----------------------------------------------------------------------------

CDivMain*
SoContextManagerKit::getDivObjectFromNode(SoNode* nodeInContext)
{
    for(int i=0; i<((SoGroup*)masterContexts.getValue())->getNumChildren(); i++)
    {
        GETCHILD(appGroup,SoGroup,((SoGroup*)masterContexts.getValue()),i);
        SoApplicationKit *appKit=(SoApplicationKit*)appGroup->getChild(0);
        if(appKit->getContext()->isNodeInContext(nodeInContext))
            return(appKit->getContext()->getDivObject()->getDiv());
    }
    return NULL;
}

//----------------------------------------------------------------------------
// returns a context pointer if a given node is associated with a master context
//----------------------------------------------------------------------------

SoContextKit*
SoContextManagerKit::getMasterContextFromNode(SoNode* nodeInMasterContext)
{
    for(int i=0; i<((SoGroup*)masterContexts.getValue())->getNumChildren(); i++)
    {
        GETCHILD(appGroup,SoGroup,((SoGroup*)masterContexts.getValue()),i);
        if(((SoApplicationKit*)(appGroup->getChild(0)))->getContext()->isNodeAssociated(nodeInMasterContext))
            return ((SoApplicationKit*)(appGroup->getChild(0)))->getContext();
    }
    return NULL;
}

//----------------------------------------------------------------------------
// returns a context pointer if a given node is associated with a slave context
//----------------------------------------------------------------------------

SoContextKit*
SoContextManagerKit::getSlaveContextFromNode(SoNode* nodeInSlaveContext)
{
    for(int i=0; i<((SoGroup*)slaveContexts.getValue())->getNumChildren(); i++)
    {
        GETCHILD(appGroup,SoGroup,((SoGroup*)slaveContexts.getValue()),i);
        if(((SoApplicationKit*)(appGroup->getChild(0)))->getContext()->isNodeAssociated(nodeInSlaveContext))
            return ((SoApplicationKit*)(appGroup->getChild(0)))->getContext();
    }
    return NULL;
}

//----------------------------------------------------------------------------
// returns a context pointer if a given node is associated with a master or slave context
//----------------------------------------------------------------------------

SoContextKit*
SoContextManagerKit::getAnyContextFromNode(SoNode* nodeInAnyContext)
{
    SoContextKit *ctx=NULL;
    ctx = getMasterContextFromNode(nodeInAnyContext);
    if(ctx) return ctx;
    ctx = getSlaveContextFromNode(nodeInAnyContext);
    if(ctx) return ctx;
    return NULL;
}

//----------------------------------------------------------------------------
// returns TRUE if a given node is associated with a master context
//----------------------------------------------------------------------------

SbBool
SoContextManagerKit::isNodeInMasterContext(SoNode* nodeInMasterContext)
{
    return getMasterContextFromNode(nodeInMasterContext)!=NULL;
}

//----------------------------------------------------------------------------
// returns TRUE if a given node is associated with a slave context
//----------------------------------------------------------------------------

SbBool
SoContextManagerKit::isNodeInSlaveContext(SoNode* nodeInSlaveContext)
{
    return getSlaveContextFromNode(nodeInSlaveContext)!=NULL;
}

//----------------------------------------------------------------------------
// returns TRUE if a given node is associated with a master or slave context
//----------------------------------------------------------------------------

SbBool
SoContextManagerKit::isNodeInAnyContext(SoNode* nodeInAnyContext)
{
    return getAnyContextFromNode(nodeInAnyContext)!=NULL;
}


//----------------------------------------------------------------------------
// load an application from .iv-file
//----------------------------------------------------------------------------
int
SoContextManagerKit::loadApplication(const SbString fileName,
                                     SoNodeList &appList,
                                     int userID, const SbName & locale)
{
    SoSeparator* loadSep;
    loadSep = readFile(fileName.getString());
    
    return loadApps(loadSep, fileName, appList, userID, locale);
}

//----------------------------------------------------------------------------
// load an application from a memory buffer
//----------------------------------------------------------------------------

int
SoContextManagerKit::loadApplication(void* buffer, int buffersize,
                                     SoNodeList &appList, int userID, const SbName & locale)
{
    SoSeparator* loadSep;
    SoInput myInput;
    
    myInput.setBuffer(buffer, buffersize);
    
    loadSep=SoDB::readAll(&myInput);
    
    return loadApps(loadSep, "MEMORYBUFFER", appList, userID, locale);
}

//----------------------------------------------------------------------------
// do the loading stuff
//----------------------------------------------------------------------------

int
SoContextManagerKit::loadApps(SoSeparator* loadSep, const SbString fileName, SoNodeList &appList, int userID, const SbName & locale)
{
    
    if (loadSep==NULL) return -1;
    loadSep->ref();
    
    // nuke all SoFile nodes that might contain an SoApplicationKit
    // however do not touch SoFile nodes inside applications !
    SoTypeList stopTypes;
    stopTypes.append( SoApplicationKit::getClassTypeId() );
    replaceFileNodes( loadSep, 0, &stopTypes );

    // search for ApplicationKits
    
    SoSearchAction mySearchAction;
    
    mySearchAction.setType(SoApplicationKit::getClassTypeId());
    mySearchAction.setInterest(SoSearchAction::ALL);
    
    mySearchAction.apply(loadSep);
    
    SoPathList *myPathList;
    
    myPathList=&mySearchAction.getPaths();
    int numPaths=myPathList->getLength();
    
    if (numPaths==0) {
        printf("INFO: no SoApplicationKit found in %s\n", fileName.getString());
        return -1;
    }
    
    SoApplicationKit* appKit;
    int i, appCount=0;
    
    for(i=0;i<numPaths;i++) {
        appKit=(SoApplicationKit*)(*myPathList)[i]->getTail();
        
        // set user id
        appKit->getContext()->userID.setValue(userID);
        
        // set filename
        if(fileName!="MEMORYBUFFER")
        {
            appKit->loadFileName.setValue(fileName);
        }
        
        // generate Div group for application distribution and append appKit
        SoDivGroup *appGroup=new SoDivGroup;        
        appGroup->addChild(appKit);
        
        // register applicationkit at contextmanager
        if (!registerApp(appGroup, TRUE,locale)) return -1;
        
        // set name of appgroup
        char buffer[100];
        sprintf(buffer,"%s_%d","GR",appKit->getContext()->appID.getValue());
        appGroup->setName(buffer);        
        
        printf("INFO: loaded application %s - %i\n", appKit->getContext()->getAppName().getString(), appKit->getContext()->appID.getValue());
        
        StbDistrManager::getInstance()->startApplication(
            appKit->getContext()->appID.getValue(), locale, userID );
        
        appList.append(appKit);
        
        appCount++;        
    }
    
    loadSep->unref();
    
    return appCount;    
}

//----------------------------------------------------------------------------
// register PipSheets of all Applications for a given user
//----------------------------------------------------------------------------

void SoContextManagerKit::registerPipSheets(int uid) {
    SoGroup *m=(SoGroup*)masterContexts.getValue();
    SoGroup *s=(SoGroup*)slaveContexts.getValue();
    SoApplicationKit * appKit;
    
    SoUserKit * userKit = SoUserManagerKit::getInstance()->getUserKit(uid);

    int i;
    for(i=0; i<m->getNumChildren(); i++) {
        appKit = ((SoApplicationKit*)((SoGroup*)(m->getChild(i)))->getChild(0));
        appKit->getContext()->registerPipSheet(uid);
        if( userKit->getPip() != NULL)
            appKit->getContext()->checkPipMasterMode(appKit->getContext()->getPipSheet(uid), TRUE);
    }
    for(i=0; i<s->getNumChildren(); i++) {
        appKit = ((SoApplicationKit*)((SoGroup*)(s->getChild(i)))->getChild(0));
        appKit->getContext()->registerPipSheet(uid);
        if( userKit->getPip() != NULL)
            appKit->getContext()->checkPipMasterMode(appKit->getContext()->getPipSheet(uid), FALSE);
    }
}

//----------------------------------------------------------------------------
// register MainPanels for Virtual Cockpit
//----------------------------------------------------------------------------
void SoContextManagerKit::registerMainPanels(int uid) {
    SoGroup *m=(SoGroup*)masterContexts.getValue();
    SoGroup *s=(SoGroup*)slaveContexts.getValue();
    SoApplicationKit * appKit;

    SoUserKit * userKit = SoUserManagerKit::getInstance()->getUserKit(uid);

    int i;
    for(i=0; i<m->getNumChildren(); i++) {
        appKit = ((SoApplicationKit*)((SoGroup*)(m->getChild(i)))->getChild(0));
        appKit->getContext()->registerMainPanel(uid);
        if( userKit->getVC() != NULL)
            appKit->getContext()->checkMainPanelMasterMode(appKit->getContext()->getMainPanel(uid), TRUE);
    }
    for(i=0; i<s->getNumChildren(); i++) {
        appKit = ((SoApplicationKit*)((SoGroup*)(s->getChild(i)))->getChild(0));
        appKit->getContext()->registerMainPanel(uid);
        if( userKit->getVC() != NULL)
            appKit->getContext()->checkMainPanelMasterMode(appKit->getContext()->getMainPanel(uid), FALSE);
    }
}

//----------------------------------------------------------------------------
int
SoContextManagerKit::registerApp(SoDivGroup* appGroup, SbBool masterMode, const SbName & locale)
{
    
    SoApplicationKit* appKit=(SoApplicationKit*)appGroup->getChild(0);
    
    appGroup->ref();     
    
    // tell context about appkit
    appKit->getContext()->setAppGroup(appGroup);
    
    //generate AppID using pointer 
    appKit->getContext()->appID.setValue((long)appGroup);
    
    if(!appKit->getContext()->initContext(masterMode)) 
	{
		appGroup->unref();
		return FALSE;
	}
    
    SoLocaleManagerKit::getInstance()->addApplicationKit( appGroup, locale );
    
	appGroup->unref();
    return TRUE;
}
//----------------------------------------------------------------------------

void
SoContextManagerKit::saveApplication(SbAppID appID)
{
    SoApplicationKit* app= getApp(appID);
    if (!app) return;
    
    if (!(app->readOnly.getValue()))
    {
        writeFile(app->loadFileName.getValue().getString(), app, FALSE);
    }
    else
    {
        saveApplicationAs(appID);
    }
}

void
SoContextManagerKit::saveApplicationAs(SbAppID appID)
{    
    unsigned int i=0;
    char buffer[80];
    
    SoApplicationKit* app= getApp(appID);
    if (!app) return;
    
    int len=app->loadFileName.getValue().getLength();
    SbString fname=app->loadFileName.getValue().getSubString(0, len-4); //extension is always .iv

    do {
        sprintf(buffer, "%s_%02u%s",fname.getString(), i++, ".iv");
    }
    while(!(access(buffer, 0)==-1));
    
    writeFile(buffer, app, FALSE);    
}

void
SoContextManagerKit::saveAll()
{
    int i;
    char buffer[80];
    
    SoApplicationKit *appKit;
    SoDivGroup *divGroup;
    SoSeparator *saveSep=new SoSeparator();
    saveSep->ref();
    
    for(i=0;i<((SoGroup*)masterContexts.getValue())->getNumChildren();i++)
    {
        divGroup=(SoDivGroup*)((SoGroup*)masterContexts.getValue())->getChild(i);
        appKit=(SoApplicationKit*)divGroup->getChild(0);
        saveSep->addChild(appKit);
    }
    // save SlaveContext?????
    for(i=0;i<((SoGroup*)slaveContexts.getValue())->getNumChildren();i++)
    {
        divGroup=(SoDivGroup*)((SoGroup*)slaveContexts.getValue())->getChild(i);
        appKit=(SoApplicationKit*)divGroup->getChild(0);
        saveSep->addChild(appKit);
    }
    
    i=0;
#ifdef WIN32
    do {
        
        sprintf(buffer, "%s_%02d%s","default", i++,".iv");
    }
    while(!(access(buffer,0)==-1));
#else
    printf("Error: LINUX version not implemented!\n"); 
    exit(1);
#endif
    
    writeFile(buffer, saveSep, FALSE);
    saveSep->unref();
}

SbBool
SoContextManagerKit::stopApplication(SbAppID appID)
{    
    SoApplicationKit* appKit=getApp(appID);
    
    if (appKit) 
    {
        printf("INFO: stopped application %s\n", appKit->getName().getString());

        StbDistrManager::getInstance()->stopApplication( appID );        
        SoDivGroup * appGroup = (SoDivGroup *)getAppGroup( appID );
        appGroup->active.setValue( FALSE );
        appKit->getContext()->exitContext();        
        if(((SoGroup*)masterContexts.getValue())->findChild(appGroup) != -1) ((SoGroup*)masterContexts.getValue())->removeChild(appGroup);
        if(((SoGroup*)slaveContexts.getValue())->findChild(appGroup) != -1)  ((SoGroup*)slaveContexts.getValue())->removeChild(appGroup);
        SoLocaleManagerKit::getInstance()->removeApplicationKit( appGroup );
        
        int i;
        for (i=0;i<SoUserManagerKit::getInstance()->getUserNumber();i++)
        {
            SoUserManagerKit::getInstance()->getUserKitSequentially(i)->freeResources(appID);
            SoUserManagerKit::getInstance()->getUserKitSequentially(i)->
                removeTaskBarButton(appID); 
            
        }

        return TRUE;
    }
    return FALSE;
}


void
SoContextManagerKit::shutDown()
{
    
    while (((SoGroup*)masterContexts.getValue())->getNumChildren()>0)
    {
        SoGroup * appGroup = (SoGroup *)((SoGroup*)masterContexts.getValue())->getChild(0);
        if( appGroup->getChild(0)->isOfType( SoApplicationKit::getClassTypeId()))
        {
            stopApplication(((SoApplicationKit*)appGroup->getChild(0))->getContext()->appID.getValue());
        }
    }
}


SoPipSheetKit*
SoContextManagerKit::getSheetFromNode(SoNode* nodeInSheet, SbAppID appID)
{
    if (nodeInSheet == NULL)
        return NULL;
    
    const int32_t *userIDs = SoUserManagerKit::getInstance()->getUserIDs();
    for(int i=0; i<SoUserManagerKit::getInstance()->getUserNumber(); i++) {
        // if user has no sheet skip search action
        if (getPipSheet(userIDs[i],appID) == NULL)
            continue;
        SoSearchAction sa;
        sa.setNode(nodeInSheet);
        sa.setSearchingAll(TRUE);
        SoPipSheetKit *psk = SoUserManagerKit::getInstance()->
            getUserKit(userIDs[i])->getPip()->getPipSheetKit(appID);
        sa.apply(psk);
        if((SoFullPath*)(sa.getPath()) != NULL) 
            return psk;
    }
    return NULL;
}

SoVCMainPanelKit*
SoContextManagerKit::getMainPanelFromNode(SoNode* nodeInMainPanel, SbAppID appID)
{
    if (nodeInMainPanel == NULL)
        return NULL;

    const int32_t *userIDs = SoUserManagerKit::getInstance()->getUserIDs();
    for(int i=0; i<SoUserManagerKit::getInstance()->getUserNumber(); i++) {
        // if user has no MainPanel skip search action
        if (getMainPanel(userIDs[i],appID) == NULL)
            continue;
        SoSearchAction sa;
        sa.setNode(nodeInMainPanel);
        sa.setSearchingAll(TRUE);
        SoVCMainPanelKit *mpk = SoUserManagerKit::getInstance()->
            getUserKit(userIDs[i])->getVC()->getMainPanelKit(appID);
        sa.apply(mpk);
        if((SoFullPath*)(sa.getPath()) != NULL)
            return mpk;
    }
    return NULL;
}

int
SoContextManagerKit::getUserFromSheet(SoNode* nodeInSheet, SbAppID appID)
{
    if (nodeInSheet == NULL)
        return -1;
    
    const int32_t *userIDs = SoUserManagerKit::getInstance()->getUserIDs();
    for(int i=0; i<SoUserManagerKit::getInstance()->getUserNumber(); i++) {
        // if user has no sheet skip search action
        if (getPipSheet(userIDs[i],appID) == NULL)
            continue;
        SoSearchAction sa;
        sa.setNode(nodeInSheet);
        sa.setSearchingAll(TRUE);
        sa.apply(SoUserManagerKit::getInstance()->getUserKit(userIDs[i])->getPip()->
            getPipSheetKit(appID));
        
        if((SoFullPath*)(sa.getPath()) != NULL) 
            return userIDs[i];
    }
    return -1;
}

int
SoContextManagerKit::getUserFromMainPanel(SoNode* nodeInMainPanel, SbAppID appID)
{
    if (nodeInMainPanel == NULL)
        return -1;

    const int32_t *userIDs = SoUserManagerKit::getInstance()->getUserIDs();
    for(int i=0; i<SoUserManagerKit::getInstance()->getUserNumber(); i++) {
        // if user has no MainPanel skip search action
        if (getMainPanel(userIDs[i],appID) == NULL)
            continue;
        SoSearchAction sa;
        sa.setNode(nodeInMainPanel);
        sa.setSearchingAll(TRUE);
        sa.apply(SoUserManagerKit::getInstance()->getUserKit(userIDs[i])->getVC()->
            getMainPanelKit(appID));

        if((SoFullPath*)(sa.getPath()) != NULL)
            return userIDs[i];
    }
    return -1;
}

SoNode*
SoContextManagerKit::getPipSheet(int uid, SbAppID appID)
{
    if (SoUserManagerKit::getInstance()->getUserKit(uid)->getPip())
        return SoUserManagerKit::getInstance()->getUserKit(uid)->getPip()->
        getPipSheetKit(appID);
    else
        return NULL;
}

SoNode*
SoContextManagerKit::getMainPanel(int uid, SbAppID appID)
{
  //cerr << "***SoContextManagerKit: (4)***" << endl;   
    if (SoUserManagerKit::getInstance()->getUserKit(uid)->getVC())
        return SoUserManagerKit::getInstance()->getUserKit(uid)->getVC()->
        getMainPanelKit(appID);
    else
        return NULL;
}

void
SoContextManagerKit::registerSlaveAppCB(void *data, SoSensor *s)
{ 
    
    SoNodeSensor *sensor=(SoNodeSensor*)s;
    
    SoDivGroup* group=(SoDivGroup*)sensor->getAttachedNode();

    // FIXME: Coin3d triggers the sensor too often - which can lead to
    //        NULL values in group. 20030507 tamer.
#ifdef __COIN__
    if (group == NULL) return;
#endif

    if( group->getNumChildren() > 0 )
    {
        if (group->getChild(0)->isOfType(SoApplicationKit::getClassTypeId()))
        {
            printf("registerSlaveAppCB: found AppKit\n");
            
            SbName * locale = (SbName *) data;
            SoContextManagerKit::getInstance()->registerApp(group, FALSE, *locale);
            
            delete locale;
            sensor->detach();
        }
        else
        {
            printf("registerSlaveAppCB: expected SoApplicationKit, found something else\n");
            printf("Found:%s",group->getChild(0)->getTypeId().getName().getString());
            // writeFile("slave.iv",group);
            // delete data;
            assert(0);            
        }
    }
    else
    {
        printf("Group not filled yet, I wait a little longer ...\n");
    }
}



long SoContextManagerKit::prepareApplication(const SbName & locale)
{
    SoDivGroup *appGroup=new SoDivGroup;
    SbAppID appId = (SbAppID)appGroup;
    
    char buffer[100];
    
    // use unique appid to name the appGroup    
    sprintf(buffer,"%s_%d","GR",appId);
    appGroup->setName(buffer);
    appGroup->ref();
    
    printf("new AppGroup :%s\n",appGroup->getName().getString());
    
    SoNodeSensor *s=new SoNodeSensor;
    
    s->setPriority(1);
    s->setFunction(registerSlaveAppCB);
    s->setData( new SbName( locale ));
    s->attach(appGroup);
    
    return (long)appGroup;
}

void SoContextManagerKit::setDivParams( SbAppID appId, const SbString & address, int port,
                                       SbBool active, SbBool master, SbBool initFromMaster )
{    
    SoDivGroup * appGroup = (SoDivGroup *) getAppGroup( appId );
    if( appGroup == NULL ) // if not registered yet, we could find it under the following name
    {
        char buffer[100];
        sprintf(buffer,"%s_%d","GR",appId);
        SoNode * node = SoNode::getByName( buffer );
        if( node == NULL )
        {
            printf("SoContextManagerKit::setDivParams unknown application %d\n", appId );
            return;
        }
        if( !node->isOfType( SoDivGroup::getClassTypeId()))
        {
            printf("stDivParams group node not a DivGroup !\n" );
            return;
        }
        appGroup = (SoDivGroup *) node;
    }

    // replace all occurences of . with _ to make the name a valid one
    string validAddress(address.getString());
    unsigned int idx = validAddress.find(".", 0);
    for (;idx != string::npos; idx = validAddress.find(".", idx))
       validAddress.replace(idx,1,"_");

    if( appGroup->active.getValue() != active )
    {
        if( appGroup->active.getValue() == FALSE )
        {
            // create a unique name for all DivGroups in this group
            SbString name = "DIV_";
            name += validAddress.c_str();
            name += "_";
            name += SbString( port );
            printf("App Group for DivParams %s\n", name.getString());
            appGroup->setName( name );
            appGroup->multicastGroup.setValue( address );
            appGroup->port.setValue( port );
            appGroup->initFromMaster.setValue( initFromMaster );
            appGroup->isMaster.setValue( master );            
            appGroup->active.setValue( active );
        }
        else
        {
            appGroup->active.setValue( active );
            appGroup->isMaster.setValue( master );
            appGroup->initFromMaster.setValue( initFromMaster );
        }
    } 
    else
    {
        if( appGroup->active.getValue() == TRUE )
        {
            if( appGroup->isMaster.getValue() == FALSE && master == TRUE )
            {
                // become a master because sman says so !
                appGroup->active.setValue( FALSE );
                SbString name = "DIV_";
                name += validAddress.c_str();
                name += "_";
                name += SbString( port );
                printf("App Group for DivParams %s\n", name.getString());
                appGroup->setName( name );
                appGroup->multicastGroup.setValue( address );
                appGroup->port.setValue( port );
                appGroup->initFromMaster.setValue( initFromMaster );
                appGroup->isMaster.setValue( master );            
                appGroup->active.setValue( active );
            }
        }
    }    
}

void SoContextManagerKit::deleteContextCB( void * data, SoSensor * sensor )
{
    SoContextKit * ctx = (SoContextKit *) data;
    SoContextManagerKit::getInstance()->stopApplication( ctx->appID.getValue());
    delete ((SoOneShotSensor *) sensor);
}

