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
/** SoLinkApp.cxx contains the implementation of class SoLinkApp
  *
  * @author Erich Pohn,Dieter Schmalstieg
  *
  * $Id: SoLinkApp.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/errors/SoDebugError.h>

#include "SoLinkApp.h"
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/SoLocaleManagerKit.h>

SO_NODE_SOURCE(SoLinkApp);

SoLinkApp* SoLinkApp::activeLink = NULL;

//----------------------------------------------------------------------------
// Inventor class initialization 

void SoLinkApp::initClass()
{
   SO_NODE_INIT_CLASS(SoLinkApp, SoNode, "Node");
}

//----------------------------------------------------------------------------
// constructor. 

SoLinkApp::SoLinkApp()
{
    SO_NODE_CONSTRUCTOR(SoLinkApp);

    SO_NODE_DEFINE_ENUM_VALUE(SoLinkApp, KEEP);
    SO_NODE_DEFINE_ENUM_VALUE(SoLinkApp, INVISIBLE);
    SO_NODE_DEFINE_ENUM_VALUE(SoLinkApp, REMOVE);

    SO_NODE_SET_SF_ENUM_TYPE(keepCurrent, SoLinkApp);

    SO_NODE_ADD_FIELD(fileName,        ("")   );
    SO_NODE_ADD_FIELD(workDir,         ("")   );
    SO_NODE_ADD_FIELD(position,        (0,0,0));
    SO_NODE_ADD_FIELD(rotation,        (0,0,1,0));
    SO_NODE_ADD_FIELD(size,            (1,1,1));
    SO_NODE_ADD_FIELD(keepCurrent,     (REMOVE));
    SO_NODE_ADD_FIELD(loadImmediately, (FALSE) );
    SO_NODE_ADD_FIELD(showImmediately, (FALSE) );
    SO_NODE_ADD_FIELD(description,     ("")   );
    SO_NODE_ADD_FIELD(trigger,         ()     );
    SO_NODE_ADD_FIELD(locale,          (""));

    context=NULL;

    linkAppSensor = new SoFieldSensor(&SoLinkApp::linkAppCB,this);
    linkAppSensor->setPriority(2);
    linkAppSensor->attach(&trigger);

    linkImmedSensor = new SoOneShotSensor(&SoLinkApp::linkImmedCB,this);
    linkImmedSensor->setPriority(2);
    linkImmedSensor->schedule();

    setPosSensor = new SoFieldSensor(&SoLinkApp::setPosCB,this);
    setPosSensor->setPriority(2);
    setPosSensor->attach(&position);
}
//----------------------------------------------------------------------------
// destructor. 

SoLinkApp::~SoLinkApp()
{
}


//----------------------------------------------------------------------------
// CallBack for loading application by trigger

void SoLinkApp::linkAppCB(void *data, SoSensor *sensor)
{
    if (data==NULL) return; // no pointer to application

    SoLinkApp* self = (SoLinkApp*)data;

	SoUserKit * uKit = SoUserManagerKit::getInstance()->getUserKitSequentially(0);
    int uID = (uKit != NULL)?(uKit->getUserID()):(-1);

    loadApp(self);
    // get window
    SoWindowKit*  ncw=self->context->getWindow(0);
    // set position of window
    if( self->position.isIgnored() == FALSE )
    {
        SbVec3f posStr=self->position.getValue();
        ncw->position.setValue(posStr); 
    }
    // set rotation of window
    if( self->rotation.isIgnored() == FALSE )
    {
        SbRotation rot = self->rotation.getValue();
        ncw->orientation.setValue( rot );
    }
    // set size of window
    if( self->size.isIgnored() == FALSE )
    {
        SbVec3f size = self->size.getValue();
        ncw->size.setValue( size );
    }
    // make Window visible 
    ncw->style.setValue(ncw->style.getValue() | SoWindowKit::VISIBLE);
    // get currently focused window and unfocus it
    (void *) SoUserManagerKit::getInstance()->getUserKitSequentially(0)->getFocus();
    //@kill??? focusedWindow->setFocus(uID,FALSE);            
    // set focus for this window
    //@kill??? ncw->setFocus(uID,TRUE);            
    self->context->forceFocusChange(uID);

    if (activeLink!=NULL)
    {
        if (activeLink!=self)
        {
            // there is a SoLinkApp context to be tested for remove/invisible
            if (activeLink->keepCurrent.getValue()==INVISIBLE)
            {
                // make Window invisible 
                SoWindowKit* ncw=activeLink->context->getWindow(0);
                ncw->style.setValue(ncw->style.getValue() & (~SoWindowKit::VISIBLE));
            }
            else if (activeLink->keepCurrent.getValue()==REMOVE)
            {
                // remove context
                long appID=activeLink->context->appID.getValue();
                SoContextManagerKit::getInstance()->stopApplication(appID);
                activeLink->context=NULL;
            }  
        }
    }
    // store this as active SoLinkApp
    activeLink=self;
}
//----------------------------------------------------------------------------
// CallBack for loading application by OneShotSensor

void SoLinkApp::linkImmedCB(void *data, SoSensor *sensor)
{

    if (data==NULL) return; // no pointer to application

    SoLinkApp* self = (SoLinkApp*)data;

    SoUserKit * uKit = SoUserManagerKit::getInstance()->getUserKitSequentially(0);
    int uID = (uKit != NULL)?(uKit->getUserID()):(-1);

    if (self->loadImmediately.getValue())
    {
        loadApp(self);
        if( self->context != NULL)
        {
            // get window
            SoWindowKit*  ncw=self->context->getWindow(0);
            if( ncw != NULL )
            {
                // set position of window
                SbVec3f posStr=self->position.getValue();
                ncw->position.setValue(posStr); 
                // make Window (in)visible and set focus depending on showImmediately
                if (self->showImmediately.getValue())
                {
                    // visible
                    ncw->style.setValue(ncw->style.getValue() | SoWindowKit::VISIBLE);
                    // get currently focused window and unfocus it
                    (void*) SoUserManagerKit::getInstance()->getUserKitSequentially(0)->getFocus();
                    //@kill??? focusedWindow->setFocus(uID,FALSE);            
                    // set focus for this window
                    //@kill??? ncw->setFocus(uID,TRUE);            
                    self->context->forceFocusChange(uID);
                }
                else
                {
                    // invisible
                    ncw->style.setValue(ncw->style.getValue() & (~SoWindowKit::VISIBLE));
                }
            }            
        }        
    }
}

//----------------------------------------------------------------------------
//loads application if yet not loaded
void SoLinkApp::loadApp(SoLinkApp* self)
{
    if (self->context!=NULL) return; // application already loaded

    SbString appName  = self->workDir.getValue().getString();
    int len = appName.getLength()-1;
    if (appName.getSubString(len,len) != "/") 
        appName+=SbString("/");
    appName += self->fileName.getValue().getString();

    SbString newName  = ""; //self->fileName.getValue().getString();

    SoUserKit * uKit = SoUserManagerKit::getInstance()->getUserKitSequentially(0);
    int uID = (uKit != NULL)?(uKit->getUserID()):(-1);
	
    // load application    
    SoNodeList appList;
    SbName localeName;
    if( SoLocaleManagerKit::getInstance()->getLocale(self->locale.getValue()) != NULL)
    {
        localeName = self->locale.getValue();
    }
    else
    {
        localeName = StbWorkspace::getInstance()->getDefaultLocale();
    }
    SoContextManagerKit::getInstance()->loadApplication(appName,appList,uID, localeName);    
    
    SoApplicationKit* appKit = (SoApplicationKit*) appList[0];
    if( appKit != NULL )
    {
        self->context=appKit->getContext();
        
        if (self->context==NULL) 
        {
            // newContext failed
            SoDebugError::postWarning( "SoLinkApp::loadApp", "error loading application %s", appName.getString());
            return;
        }
        // get window
        SoWindowKit*  ncw=self->context->getWindow(0);
        if (ncw==NULL)
        {
            // getWindow failed
            SoDebugError::postInfo( "SoLinkApp::loadApp", "application has no window");
            return;
        }  
    }
    else
        SoDebugError::postWarning( "SoLinkApp::loadApp", "no Appkit %s loaded", appName.getString());
}
//----------------------------------------------------------------------------
// CallBack for setting position

void SoLinkApp::setPosCB(void *data, SoSensor *sensor)
{
    if (data==NULL) return; // no pointer to application

    SoLinkApp* self = (SoLinkApp*)data;

    if (self->context==NULL) return; // yet no context loaded

    // get window
    SoWindowKit*  ncw=self->context->getWindow(0);
    // set position of window
    SbVec3f posStr=self->position.getValue();
    ncw->position.setValue(posStr); 

}
