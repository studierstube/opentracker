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
/** SoPartitionKit header file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoPartitionKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOPARTITIONKIT_H__
#define __SOPARTITIONKIT_H__

#include <stbapi/stbapi.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/SoLists.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/tracker/SoStationKit.h>
#include <stbapi/misc/SoMultiSwitch.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * does some even more miraculous things with SoStationKit nodes. It manages
 * several interdependend SoStationKit nodes and switches between them according
 * to a directed adjacence graph defined in its fields. For each first SoStationKit 
 * contained in a path starting from a node in the part stationKits 
 * there is a set of neighbor SoStationKits. These sets
 * are stored in the field portals in the same order as the SoStationKits appear
 * in the part stationKits. The number of portals for each SoStationKit is stored
 * in numPortals in the same order. 
 *
 *  File format :
 *<pre>SoPartitionKit {
 *  activeChild -1
 *  numPortals [-1]
 *  portals [-1]
 *  stationKits SoMultiSwitch { 
 *  }
 *}
 * </pre>
 *
 *  The kit has the following structure :
 *<pre>
 *   stationKits
 * </pre>
 * @todo implement listener for children changes
 * @ingroup tracking
 */
class STBAPI_API SoPartitionKit : public SoBaseKit
{
    SO_KIT_HEADER(SoPartitionKit);

    /** StationKit Switch. This SoMultiSwitch part holds the SoStationKits
     * that the SoPartitionKit manages. */
    SO_KIT_CATALOG_ENTRY_HEADER(stationKits);

public: // member-variables

    /** This field holds the index of the currently active child. It is visible
     * and gets 3D events. It's neighbors as defined in portals are not visible
     * but do get 3D events as well. All other children are not visible and are
     * not receiving 3D events. */
    SoSFInt32 activeChild;

    /** This field contains the number of portals for each SoStationKit in
     * the stationKits part. It is ordered as the StationKits appear in the
     * part. */
    SoMFInt32 numPortals;
  
    /** This field contains the number of the SoStationKits the portals for
     * each kit go to. This works similar to a SoFaceSet node. The indices are
     * set for each SoStationKit in the stationKit part in order. The numPortals 
     * field tells how many indices are there for each kit. */
    SoMFInt32 portals;
  
    /// static method to initialize class for OpenInventor
    static void initClass();	

    /// constructor
    SoPartitionKit();

	/// destructor			
    virtual ~SoPartitionKit();		

protected:

    /** This method sets the data in the SoMultiSwitch stationKits part to 
     * allow only the currently active SoStationKit and its neighbors to 
     * receive events. Furthermore it sets the currently active SoStationKit
     * visible and all neighbors invisible.
     * @param index the index of the new active StationKit 
     */
    void setActiveChild( int index );

    /** This callback is set on each SoStationKit so that the SoPartitionKit
     * node is notified of any events from children. It decides whether to
     * change the active SoStationKit and to which one. 
     */
    static void eventCB( void *data, int station, SoStationKit *kit, 
                         SoHandle3DEventAction *h3a);

    /**
     * This callback is set on the SoMultiSwitch to get information about
     * changes to its children.
     * @note it is not implemented yet.
     */
    static void childCB( void *data, SoSensor * sensor );    

    /**
     * This callback is triggered by a SoOneShotSensor scheduled in the
     * constructor of the SoPartitionKit. It is necessary to set the
     * eventCB on all children of the SoMultiSwitch and they are not
     * present at the time of construction. There this is done here.
     */
    static void initChildren( void * data, SoSensor * sensor );

    /**
     * This callback is notified when the activeChild field is changed.
     * If it is changed from the outside (not by the SoPartitionKit itself),
     * it sets the given child to be the new active child.
     */
    static void activeChanged( void * data, SoSensor * sensor );

    SoNodeSensor childSensor;
    /// sensor for changes on the activeChild field
    SoFieldSensor activeSensor;
    /// node list to store pointers to the SoStationKits below
    SoNodeList kitList;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

