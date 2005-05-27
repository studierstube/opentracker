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
  * Defines SoDropTarget, the base class for all the "Tool-based" interaction nodes.
  *    
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoDropTarget.h 3426 2004-07-14 12:11:27Z tamer $
  *
  * @file
*/
 /* ======================================================================= */

#ifndef _SODROPTARGET_H_
#define _SODROPTARGET_H_

#include <stbapi/event3d/Base3D.h>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/SoPath.h>

#include "fingerfarben.h"
#include "SoDropItem.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoDropItem;

/**
This is the base class for all the "Tool-based" interaction nodes. The name comes from the
fact that any SoDropItem can simply be "dropped" into the bounds of this node to apply
the action that is implemented by it. Of course also the SoDropTarget can be moved
to enclose the DropItem within its bounds.

Examples of uses of this class include a trashcan, that simply deletes the geometry that is
moved within, a print target that prints the geometry that is moved within its bounds or 
more complex geometry manipulation operations.

A SoDropItem can have three states in respect to a SoDropTarget: it can be outside the target
(state = OUTSIDE), it can be inside the target but the action is not yet triggered (state =
HOVER) or it can be still inside and the action has already been triggered (state = ACTION).
It will remain in this state until it is moved out of the bounds again.

For triggering actions, there are three possibilities: IMMEDIATE will trigger the action immediately
when the DropItem is moved inside the bounds (therefore, the state will never be HOVER).
TIMED will trigger the action after a given timeout, and CLICK will trigger the action
if a click event occurs within the bounds of the DropTarget.

TODO
One problem that is not currently handled is the possible n-to-n mapping
of Items and Targets - what if several items are within one target's bounds or one item
is within several target's bounds? currently the state is only managed in the
DropItem, and the DropTarget changes whenever a DropItem within changes state. Strictly
speaking, this works only for the case of non-overlapping bounds of all the DropTargets
and only one item inside one DropTarget at any time.

Also the handling of commands can be improved - currently a DropTarget checks if its command
is a substring of the items "commands" string (or if it equals "*"). There is currently no way 
to distinguish different instances of the same DropTarget subclass.

File Format / defaults:

@verbatim
SoDropTarget {
    SoSFEnum    trigger         IMMEDIATE   # IMMEDIATE, TIMED or CLICK
    SoSFEnum    check           COORDS      # COORDS or BOUNDS; what to check for the intersection test
    SoSFTime    triggerTime     2.0         # the time it takes to trigger the action in TIMED mode

    SoNode      geometry        Group {}    # the geometry that is displayed in normal state
                                
    SoNode      hoverGeometry   Group {}    # the geometry that is displayed if a DropItem is within the bounds

    SoNode      actionGeometry  Group {}    # the geometry that is displayed after the action has been performed

    SoShape     bounds          Cube {
                                    width 0.2 height 0.2 depth 0.2
                                }

}
@endverbatim

@ingroup fingerfarben
*/
class FINGERFARBEN_API SoDropTarget : public SoBaseKit, public Base3D 
{
    SO_KIT_HEADER(SoDropTarget);

     ///(hidden) root separator
    SO_KIT_CATALOG_ENTRY_HEADER(base);
    /// the actual user's content
    SO_KIT_CATALOG_ENTRY_HEADER(stateSwitch);  
    SO_KIT_CATALOG_ENTRY_HEADER(geometry);  
    SO_KIT_CATALOG_ENTRY_HEADER(hoverGeometry);
    SO_KIT_CATALOG_ENTRY_HEADER(actionGeometry);

    SO_KIT_CATALOG_ENTRY_HEADER(boundsSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(nullDrawStyle);
    SO_KIT_CATALOG_ENTRY_HEADER(bounds);

    BASE3D_HEADER;

public: // member-variables

    /// Possible values for the "trigger" field.
    enum triggertype {
        IMMEDIATE = 0,
        TIMED,
        CLICK
    };

    /// Possible values for the "check" field.
    enum checktype {
        COORDS = 0,     ///< Checks whether the coordinates of the SoDropItem are within the bounds.
        BOUNDS          ///< Checks whether the bounding box of the item is fully inside the bounds.
    };

    /**
     * timeout value for setting the content invisible in seconds. If the
     * timeOut is 0 then no timeout occurs. */
    SoSFTime triggerTime;

    /// Trigger policy.
    SoSFEnum trigger;

    /// Check policy @see checktype.
    SoSFEnum check;

public:// member-methods
				
    /// Static method to initialize class for OpenInventor.
    static void initClass();	

    /// Constructor.
    SoDropTarget();

	/// Destructor.		
    virtual ~SoDropTarget();

    /**
     * Checks whether the node is interested in a certain 3DEventAction.
     * @param h3a pointer to the 3DEventAction.
     * @return whether the event is significant to us.
     */
	virtual SbBool isInterested(SoHandle3DEventAction *h3a);

    SbBool setUpConnections(SbBool onOff, SbBool doItAlways);

    /** 
     * Handles the 3D event. 
     * @param h3a pointer to the 3DEventAction.
     */
	virtual void handle3DEvent(SoHandle3DEventAction *h3a);

    /**
    This method is called when an action for an item is triggered. Subclasses
    only have to implement this method to implement their own actions.

    @param pathToItem A SoFullPath* that points to the SoDropItem that triggered
            the action.
    @return Whether the method performed an action or not.
    */
    virtual bool handleItem(SoFullPath* pathToItem);

protected: // member-variables

    /// A sub-path from the root of this kit to the bounds node.
    SoPath* pathToBounds;

    /**
     * AlarmSensor to signal the time out. It triggers the
     * timerFired callback with a pointer to the SoStationKit. */
    SoAlarmSensor timer;

    /**
     * Called when the time out timer fires.
     * @param data pointer to the path to item
     * @param sensor the sensor that called the callback function */
    static void timerFired( void *data, SoSensor *sensor );

private:
    /// Internal method that in turn calls the virtual handleItem().
    bool doHandleItem(SoFullPath* pathToItem);

    /**
    Flag to indicate if we are in a master context.
    @todo Do we need this.
    */
	bool isMaster;
			
};

/**
Helper struct to carry the data we need to pass to the TimeSensor callback.
*/
struct timerdata {
    SoDropTarget* target;   ///< Pointer to the SoDropTarget instance.
    SoFullPath* itemPath;   ///< Pointer to the path to the item.
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SODROPTARGET_H_
