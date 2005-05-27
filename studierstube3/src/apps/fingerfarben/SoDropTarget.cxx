/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
 /** 
  * Implements SoDropTarget, the base class for all the "Tool-based" interaction nodes.
  *    
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoDropTarget.cxx 3426 2004-07-14 12:11:27Z tamer $
  *
  * @file
*/
 /* ======================================================================= */

#include <memory>
#include <string>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SbBox.h>

#include <stbapi/event3d/Stb3DEventGenerator.h>


#include "SoDropTarget.h"

using namespace std;

// OpenInventor macros

SO_KIT_SOURCE(SoDropTarget);
BASE3D_SOURCE(SoDropTarget);
 
// class initialization for OpenInventor

void SoDropTarget::initClass(void)
{
    SO_KIT_INIT_CLASS(SoDropTarget, SoBaseKit, "BaseKit");
    BASE3D_INIT_CLASS(SoDropTarget);    
}

// constructor

SoDropTarget::SoDropTarget()
{
    SO_KIT_CONSTRUCTOR(SoDropTarget);

    //--------------------------------name--------------|-type----  |-deftype|nuldef|-parent---------|right sib|public
    SO_KIT_ADD_CATALOG_ENTRY         (base,              SoGroup,             FALSE, this,            \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ENTRY         (stateSwitch,       SoSwitch,            FALSE, base,            \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(geometry,          SoNode,     SoGroup, FALSE, stateSwitch,     \x0,      TRUE  );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(hoverGeometry,     SoNode,     SoGroup, FALSE, stateSwitch,     \x0,      TRUE  );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(actionGeometry,    SoNode,     SoGroup, FALSE, stateSwitch,     \x0,      TRUE  );

    SO_KIT_ADD_CATALOG_ENTRY         (boundsSeparator,   SoSeparator,         FALSE, base,            \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ENTRY         (nullDrawStyle,     SoDrawStyle,         FALSE, boundsSeparator, \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(bounds,            SoShape,    SoCube,  FALSE, boundsSeparator, \x0,      TRUE  );
        
    SO_KIT_ADD_FIELD(triggerTime, (2.0));
    SO_KIT_ADD_FIELD(trigger, (IMMEDIATE));
    SO_KIT_ADD_FIELD(check, (COORDS));
    
    SO_NODE_DEFINE_ENUM_VALUE(triggertype, IMMEDIATE);
    SO_NODE_DEFINE_ENUM_VALUE(triggertype, TIMED);
    SO_NODE_DEFINE_ENUM_VALUE(triggertype, CLICK);

    SO_NODE_DEFINE_ENUM_VALUE(checktype, COORDS);
    SO_NODE_DEFINE_ENUM_VALUE(checktype, BOUNDS);

    SO_NODE_SET_SF_ENUM_TYPE(trigger, triggertype);
    SO_NODE_SET_SF_ENUM_TYPE(check, checktype);

    SO_KIT_INIT_INSTANCE();
    
    BASE3D_CONSTRUCTOR;

    ((SoDrawStyle*)(nullDrawStyle.getValue()))->style = SoDrawStyle::INVISIBLE;
    ((SoSwitch*)(stateSwitch.getValue()))->whichChild.setValue(0);

    SoCube* bCube = (SoCube*)getPart("bounds", false);
    bCube->width.setValue(0.2);
    bCube->height.setValue(0.2);
    bCube->depth.setValue(0.2);

    pathToBounds = new SoPath();
    pathToBounds->setHead(base.getValue());
    pathToBounds->append(boundsSeparator.getValue());
    pathToBounds->append(getPart("bounds",false));
    pathToBounds->ref();

    timer.setFunction( timerFired );
    timer.setData( this );

	isMaster = true;

}

// destructor, clears all lists and removes any listeners etc.

SoDropTarget::~SoDropTarget()
{
    timer.unschedule();

}

SbBool SoDropTarget::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (onOff) {
        // We connect AFTER base class.
        //SoDragger::setUpConnections(onOff, doItAlways);


    }
    else {
        //nothing to disconnect

        // We disconnect BEFORE base class.
        //SoDragger::setUpConnections(onOff, doItAlways);
    }
    return !onOff;
}

void SoDropTarget::handle3DEvent(SoHandle3DEventAction *h3a)
{    

	if (!isMaster) return;

    //((SoSwitch*)(stateSwitch.getValue()))->whichChild.setValue(0);

    So3DEvent *ev = (So3DEvent*)h3a->getEvent();

    //printf("SoDropTarget: Event Received. Station:%d Button0:%d Button1:%d Button2:%d\n", ev->getStation(),ev->getButton(So3DEvent::BUTTON0),ev->getButton(So3DEvent::BUTTON1),ev->getButton(So3DEvent::BUTTON2));
    /*
    // if just moved inside bounding box and trigger mode is TIMED 
    timer.setTimeFromNow(triggerTime.getValue());
    timer.schedule();
    */

    // where are we?
    SbViewportRegion vr;
    SoGetBoundingBoxAction bba(vr);

    SoPath* fullPath = (SoPath*)(h3a->getPathFromRoot()->copy());
    fullPath->ref();
    fullPath->append(pathToBounds);
    bba.apply(fullPath);

    SbBox3f box(bba.getBoundingBox());

    SoNode* root = fullPath->getHead();
    fullPath->unref();

    SoSearchAction sa;
    sa.setType(SoDropItem::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.apply(root);

    SoPathList list = sa.getPaths();
    //printf("found %d items!\n", list.getLength());

    bool oneInside = false;
    bool oneAction = false;

    // check all found items
    for (int i=0; i<list.getLength(); i++){

        SoFullPath* itemPath = (SoFullPath*)list[i];
        SoDropItem* item = (SoDropItem*)itemPath->getTail();

        if (check.getValue() == COORDS) 
        {

            SoGetMatrixAction ma(vr);
            ma.apply(itemPath);
            SbMatrix itemTransform = ma.getMatrix();

            SbVec3f itemPos, s; SbRotation itemRot, so;
            itemTransform.getTransform(itemPos, itemRot, s, so);

            if (box.intersect(itemPos))
            {
                oneInside = true;
                if (item->state != SoDropItem::ACTION)
                {
                    if ((item->state == SoDropItem::OUTSIDE) && (trigger.getValue() == TIMED) && (triggerTime.getValue() > 0.0))
                    {
                        //we just moved inside -> start timer
                        SoAlarmSensor* timer = new SoAlarmSensor();
                        timerdata* td = new timerdata;
                        td->itemPath = itemPath;
                        itemPath->ref();
                        td->target = this;
                        timer->setFunction( timerFired );
                        timer->setData( td );
                        timer->setTimeFromNow( triggerTime.getValue());
                        timer->schedule();
                    }

                    item->state = SoDropItem::HOVER;
                    //printf("SoDropItem: position: [%f,%f,%f]\n", itemPos[0], itemPos[1], itemPos[2]);

                    if ((trigger.getValue() == IMMEDIATE) || (trigger.getValue() == TIMED) && (triggerTime.getValue() <= 0.0))
                    {
                        if (doHandleItem(itemPath))
                            oneAction = true;
                        item->state = SoDropItem::ACTION;
                    }
                }
                else
                {
                    oneAction = true;
                }
            }
            else 
            {
                if ((item->state == SoDropItem::HOVER) && (trigger.getValue() == TIMED))
                {
                    // possibly we have a timer running on this object
                    //item->timer.unschedule();
                }
                item->state = SoDropItem::OUTSIDE;
            }

        }
    }

    if (oneInside)
        if (oneAction)
        {
            //printf("one action\n");
            ((SoSwitch*)(stateSwitch.getValue()))->whichChild.setValue(2);
        }
        else
        { 
            //printf("one inside\n");
            ((SoSwitch*)(stateSwitch.getValue()))->whichChild.setValue(1);
        }
    else 
    {
        //printf("nothing\n");
        ((SoSwitch*)(stateSwitch.getValue()))->whichChild.setValue(0);
    }


}

SbBool SoDropTarget::isInterested(SoHandle3DEventAction *h3a)
{
    //printf("SoDropTarget: isInteresed?\n");
    //this only checks if any event occured within our bounding box
    So3DEvent *event = (So3DEvent *)h3a->getEvent();

	if (event->getType() == So3DEvent::ET_MASTERMODE_EVENT) {
		printf("SoDropTarget: MASTERMODE\n");
		isMaster = true;
		return false;
	}

	if (event->getType() == So3DEvent::ET_SLAVEMODE_EVENT) {
		printf("SoDropTarget: SLAVEMODE\n");
		isMaster = false;
		return false;
	}

	if (!isMaster) return false;

    SbViewportRegion vr;
    SoGetBoundingBoxAction bba(vr);

    SoPath* fullPath = (SoPath*)(h3a->getPathFromRoot()->copy());
    fullPath->append(pathToBounds);
    bba.apply(fullPath);

    SbBox3f box(bba.getBoundingBox());

    //check big bounding box around rotated geom
    if (box.intersect(event->getTranslation()))
    {
        //check tight bounding box (local coord. sys.)
        SbXfBox3f xfbox(bba.getXfBoundingBox());

        if (xfbox.intersect(event->getTranslation())) {
            return TRUE;
        }
    }

    return FALSE;

}

bool SoDropTarget::doHandleItem(SoFullPath* pathToItem)
{
    if (handleItem(pathToItem))
    {
        SoDropItem* item = (SoDropItem*)pathToItem->getTail();
        item->state = SoDropItem::ACTION;
        return true;
    }

    return false;
       
}

bool SoDropTarget::handleItem(SoFullPath* pathToItem)
{
    SoDropItem* item = (SoDropItem*)((SoFullPath*)pathToItem)->getTail();
    printf("Item handled with commands: %s\n", item->commands.getValue().getString());
    return true;
}

void SoDropTarget::timerFired(void *data, SoSensor *sensor)
{
    timerdata* td = (timerdata*)data;
    SoFullPath* pathToItem = td->itemPath;
    SoDropItem* item = (SoDropItem*)pathToItem->getTail();
    SoDropTarget* target = td->target;

    if (item->state == SoDropItem::HOVER)
    {
        //item is still inside bounds
        if (target->doHandleItem(pathToItem))
        {
            ((SoSwitch*)(target->stateSwitch.getValue()))->whichChild.setValue(2);
            item->state = SoDropItem::ACTION;
        }
    }
}

