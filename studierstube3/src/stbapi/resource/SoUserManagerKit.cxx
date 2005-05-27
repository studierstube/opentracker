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
/** SoUserManagerKit.h contains the implementation of class SoUserManagerKit
  *
  * @author Andreas Zajic
  *
  * $Id: SoUserManagerKit.cxx 4113 2004-12-21 14:27:35Z breiting $
  * @file                                                                    */
 /* ======================================================================== */

#include <cassert>

#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>

#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>

#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/workspace/SoLocaleManagerKit.h>
#include <stbapi/workspace/distrManager/StbDistrManager.h>

#include <stbapi/resource/SoUserKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/SoTabKit.h>
#include <stbapi/resource/SoTabButtonKit.h>
#include <stbapi/resource/SoPullDownKit.h>

SO_KIT_SOURCE(SoUserManagerKit);

SoUserManagerKit* SoUserManagerKit::instance = NULL;

//----------------------------------------------------------------------------

SoUserManagerKit::SoUserManagerKit()
{

    SO_KIT_CONSTRUCTOR(SoUserManagerKit);

    //       -----------------------name---|-type-------|nuldef|-par |rite|public
    SO_KIT_ADD_CATALOG_ENTRY(userKits,       SoSeparator, FALSE, this,\x0 ,FALSE);

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

}

//----------------------------------------------------------------------------

SoUserManagerKit::~SoUserManagerKit()
{

}

//----------------------------------------------------------------------------

SoUserManagerKit* SoUserManagerKit::getInstance()
{
    if(instance == NULL) instance = new SoUserManagerKit();
    return instance;
}

//----------------------------------------------------------------------------
void
SoUserManagerKit::initClass()
{
    SO_KIT_INIT_CLASS(SoUserManagerKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
int
SoUserManagerKit::checkInUser(SoUserKit *newUserKit, const SbName & locale)
{
    if (getUserKit(newUserKit->userID.getValue())) // check userID
        return FALSE;
    userIDs.set1Value(userIDs.getNum(), newUserKit->userID.getValue());
    ((SoSeparator*)(userKits.getValue()))->addChild(newUserKit);
    SoLocaleManagerKit::getInstance()->addUserKit( newUserKit, locale );
    return TRUE;
}

//----------------------------------------------------------------------------
int
SoUserManagerKit::checkOutUser(SoUserKit *oldUserKit)
{
    assert(userKits.getValue());
    if (!getUserKit(oldUserKit->userID.getValue())) // check userID
        return FALSE;
    userIDs.deleteValues(getUserKitIndex(oldUserKit->userID.getValue()), 1);
    ((SoSeparator*)(userKits.getValue()))->removeChild(oldUserKit);
    SoLocaleManagerKit::getInstance()->removeUserKit( oldUserKit );
    return TRUE;
}

//----------------------------------------------------------------------------

int
SoUserManagerKit::getUserNumber()
{
    assert(userKits.getValue());
    return ((SoSeparator*)(userKits.getValue()))->getNumChildren();
}

//----------------------------------------------------------------------------

SoUserKit *
SoUserManagerKit::getUserKit(int userID)
{
    assert(userKits.getValue());
    for (int i=0;i<getUserNumber();i++)       // search userID
        if (getUserKitSequentially(i)->userID.getValue() == userID)
           return getUserKitSequentially(i);
    return NULL;
}

//----------------------------------------------------------------------------

SoUserKit *
SoUserManagerKit::getUserKitSequentially(int index)
{
    assert(userKits.getValue());
    if (index < 0 || index >= getUserNumber())
        return NULL;
    return (SoUserKit *)((SoSeparator*)(userKits.getValue()))->getChild(index);
}

//----------------------------------------------------------------------------

int
SoUserManagerKit::getUserKitIndex(int userID)
{
    assert(userKits.getValue());
    for (int i=0; i<getUserNumber(); i++)
        if (getUserKitSequentially(i)->userID.getValue() == userID)
            return i;
    return -1;
}

//----------------------------------------------------------------------------

const int32_t *
SoUserManagerKit::getUserIDs()
{
    return userIDs.getValues(0);
}

//----------------------------------------------------------------------------
int
SoUserManagerKit::getUserFromStation(int station)
{

    SoUserKit *temp;
    for (int i=0; i<getUserNumber(); i++)
    {
       temp = getUserKitSequentially(i);
       if ((temp->getPen()     && temp->getPen()->station.getValue()     == station) ||
           (temp->getPip()     && temp->getPip()->station.getValue()     == station) ||
           (temp->getVC()      && temp->getVC()->station.getValue()      == station) ||
           (temp->getDisplay() && temp->getDisplay()->station.getValue() == station))
           return temp->userID.getValue();
    }
    return -1;
}

//----------------------------------------------------------------------------

int
SoUserManagerKit::getPenFromUser(int userID)
{
    SoUserKit *uk;
    if ((uk = getUserKit(userID)) && uk->getPen())
        return  uk->getPen()->station.getValue();
    else
        return -1;
}

//----------------------------------------------------------------------------

int
SoUserManagerKit::getVCFromUser(int userID)
{
  //cerr << "***SoUserManagerKit: (10)***" << endl;   
   SoUserKit *uk;
   if ((uk = getUserKit(userID)) && uk->getVC())
      return uk->getVC()->station.getValue();
   else
      return -1;
}

//----------------------------------------------------------------------------

int
SoUserManagerKit::getPipFromUser(int userID)
{
    SoUserKit *uk;
    if ((uk = getUserKit(userID)) && uk->getPip())
		return  uk->getPip()->station.getValue();
    else
        return -1;
}

//----------------------------------------------------------------------------

int
SoUserManagerKit::getHmdFromUser(int userID)
{
    SoUserKit *uk;
    if ((uk = getUserKit(userID)) && uk->getDisplay())
       return  uk->getDisplay()->station.getValue();
    else
       return -1;
}


SoUserKit *  SoUserManagerKit::getUserKitFromNode(SoNode * node )
{
    if( node == NULL )
        return NULL;
    SoSearchAction sa;
    sa.setNode( node );
    sa.setSearchingAll( TRUE );
    sa.apply( userKits.getValue());

    SoFullPath * path = (SoFullPath *)sa.getPath();
    if( path != NULL )
    {
        if( path->getNode(1)->isOfType( SoUserKit::getClassTypeId()))
            return (SoUserKit *) path->getNode(1);
    }
    return NULL;
}

//----------------------------------------------------------------------------

void SoUserManagerKit::windowBarChanged(int userId, int style, int state)
{
    SoUserKit *uk = getUserKit(userId);
	if((!uk)||!(uk&&uk->focus))
        return;
    uk->focus->style = style;
    switch (state)
    {
    case SoWindowKit::MINIMIZED:
        uk->focus->state.setValue(SoWindowKit::MINIMIZED);
        break;
	case SoWindowKit::MAXIMIZED:
        uk->focus->state.setValue(SoWindowKit::MAXIMIZED);
        break;
    case SoWindowKit::NORMAL:
        uk->focus->state.setValue(SoWindowKit::NORMAL);
        break;
    default:
        break;
    }
    uk->updateWindowButtons(uk->focus);
}

void SoUserManagerKit::taskBarChanged(int userId, int buttonId)
{
    SoUserKit *uk = getUserKit(userId);
    if(!uk)
        return;
    uk->useResources(buttonId);
}

void SoUserManagerKit::sheetBarChanged(int userId, SbAppID appId, int whichChild)
{ 
    SoUserKit * user = getUserKit(userId);
    assert( user );
    SoPipSheetKit *psk = user->getPip()->getPipSheetKit(appId);
    assert(psk);
    psk->switchPipSheet(whichChild);
}

void SoUserManagerKit::systemBarChanged(int userId, int buttonId, int tabInfo)
{
    SoUserKit *uk = getUserKit(userId);
    assert(uk);

    switch (buttonId)
    {
    case SoUserKit::ACTIVE_TASK:
        uk->taskBar->showTabs(tabInfo);
        uk->getPip()->updatePip();
        break;
    case SoUserKit::CLOSE_APP:
        SoContextManagerKit::getInstance()->stopApplication(uk->currentApp /*,TRUE*/);
        uk->getPip()->useResources(0);
        uk->getPip()->updatePip();
        break;
    case SoUserKit::SAVE_APP:
        SoContextManagerKit::getInstance()->saveApplication(uk->currentApp);
        break;
    case SoUserKit::SAVE_APP_AS:
        SoContextManagerKit::getInstance()->saveApplicationAs(uk->currentApp);
        break;
    case SoUserKit::SAVE_ALL:
        SoContextManagerKit::getInstance()->saveAll();
        break;
    case SoUserKit::SYSTEM_MENU:
        uk->taskBar->showTabs(FALSE);
        uk->getPip()->useResources(0);
        uk->getPip()->getPipSheetKit(0)->switchPipSheet(0);
        uk->getPip()->updatePip();
        break;
    default:
        break;
    }
}

void SoUserManagerKit::focusChanged(int userId, SbAppID appId)
{
    SoUserKit *uk = getUserKit(userId);
    assert(uk);
    SoContextKit *ctx = SoContextManagerKit::getInstance()->getApp(appId)->getContext();
    assert(ctx);
    ctx->forceFocusChange(userId);
}
