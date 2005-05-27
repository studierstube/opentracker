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
/** header file for SoDivGroup node
  *
  * @author   Gerhard Reitmayr
  *
  * $Id: SoDivGroup.h 4076 2004-11-30 18:33:09Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SODIVGROUP_H_
#define _SODIVGROUP_H_

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class CDivMain;

/**
 * The SoDivGroup node is a extended OpenInventor group node that also shares
 * its children in a DIV group. The parameters of the DIV group and the
 * local instance of the SoDivGroup node are set via OpenInventor fields.
 *
 * Note that for the SoDivGroup to work correctly all instances that share the
 * same scene graph have to have the same DEF name ! If you do not specifiy a
 * name for an SoDivGroup node, it will create a unique name that is the same
 * for all SoDivGroups in the same multicast group. Therefore no name will
 * result in a shared scene graph among all SoDivGroup nodes with the same
 * multicast group (specified by ip address and port number).
 *
 * The network interface (nic) can also be specified. Please note that the definition
 * of the nic depends on the actual OS. 
 * @li Win32: ip-address of the network interface eg. "192.168.3.1"
 * @li UNIX: network interface name eg. "eth1" this will result in files that are *NOT* OS independent !!
 * 
 * If no nic is specified the DivGroup works as before (depending on the OS the default or all network interfaces will be used).
 *
 * Here is an example of how a configured SoDivGroup looks like
 @code
 SoDivGroup {
     multicastGroup "224.100.200.111"
     port 7890
     initFromMaster FALSE
     active TRUE
     isMaster TRUE

     # everything in here will be shared
 } 
 @endcode
 * @ingroup div
 * @author Gerhard Reitmayr
 */
class STBAPI_API SoDivGroup : public SoGroup
{
    SO_NODE_HEADER(SoDivGroup);
public:

    /** 
     * the ip address of the multicast group to use
     */
    SoSFString multicastGroup;
    
    /**
     * the port number of the multicast group to use
     */
    SoSFInt32  port;

	/**
     * the network interface the multicast group should use (default is ""), 
	 * if you are not sure what the nic is, just do not specify it. 
     */
    SoSFString nic;

    /** 
     * a flag denoting if this node is the master or a
     * slave of the group. This field is only readable, changing it
     * does nothing.
     */
    SoSFBool isMaster;

    /**
     * tells the node to try and become master
     */
    SoSFTrigger getMaster;

    /**
     * this flag activates or disactives the DIV on this node.
     * If you change it, the node will start or stop participating.
     */
    SoSFBool active;

    /**
     * this flag describes whether the node should get the current
     * state from the master upon activation or just start processing
     * events.
     */
    SoSFBool initFromMaster;

    /** static method to initialize class for OpenInventor. It also
     * initializes SoGroupSwitchElement and enables it for all necessary
     * actions.  
     */
    static void initClass();

	SoDivGroup();	

    CDivMain * getDiv()
    {
        return div;
    }

protected:

    virtual ~SoDivGroup();

    /// the actual implementation
    CDivMain * div;

    /// field observer for the active field
    SoFieldSensor activeObs;

    /// field observer for events to getMaster trigger
    SoFieldSensor masterObs;

    static void activeChangedCB( void * data, SoSensor * sensor );

    static void getMasterTriggeredCB( void * data, SoSensor * sensor );

    static void masterChangedCB( void * data, CDivMain * div );
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SODIVGROUP_H_
