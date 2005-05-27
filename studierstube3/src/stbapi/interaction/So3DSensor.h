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
//  PROJECT:    Studierstube
//  CONTENT:    Class definition of So3DSensor
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:02:56  gh      last modification
// ===========================================================================


#ifndef _SO3DSENSOR_H_
#define _SO3DSENSOR_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>

#include <stbapi/event3d/Base3D.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DSensor;

// callback prototype for So3DSensor
typedef void So3DSensorCB(void*, So3DSensor*);

/**
 * 3d sensor detects 3d events and provides callbacks. Documented at
 * http://www.studierstube.org/stbapi/So3DSensor.html
 * Use this node if you need to sense pen clicks and movement inside general
 * volumes defined by scene graphs. When the button of your input-device is 
 * pressed, moved or released inside the bounding box of the geometry, the 
 * node calls back user-defined callbacks. Also, the tracker data inside the 
 * volume is available via the translation and rotation fields. 
 * This node has a sister node SoDragSeparator, which uses relative rather
 * than absolute translation/rotation and can easily be used for making 
 * dragable objects that follow the pen.
 * @ingroup event
 */
class STBAPI_API So3DSensor
:
    public SoSeparator, public Base3D 
{

    SO_NODE_HEADER(So3DSensor);
    BASE3D_HEADER;

public:
    ///translation of tracker in world coords
    SoSFVec3f translation; 
    ///rotation of tracker in world coords
    SoSFRotation rotation;   
    ///button number triggering tracking
	SoSFInt32 dragButton;           
    
    ///static class initialization
	static void initClass(          
        );
    ///constructor
	So3DSensor(                     
        );

    /** true if node is interested in 3d event
     * @param h3a action owning 3d event
     * @returns true if node is interested in 3d event */
	virtual SbBool isInterested(    
        SoHandle3DEventAction* h3a  
        );
    /** deal with 3d event
	 * @param h3a action owning 3d event */
    virtual void handle3DEvent(     
        SoHandle3DEventAction* h3a  
        );
    /** set callback for picking geometry
     * @param func callback function
     * @param data user supplied data */  
    void setPressCallback(          
        So3DSensorCB* func,         
        void* data=NULL             
        );
    /** remove callback for picking geometry*/
    void removePressCallback(       
        );

    /** set callback for releasing geometry
     * @param func callback function
     * @param data user supplied data */
    void setReleaseCallback(        
        So3DSensorCB* func,         
        void* data=NULL             
        );
    /** remove callback for releasing geometry*/
    void removeReleaseCallback(     
        );
    /** set callback for moving geometry
     * @param func callback function 
     * @param data user supplied data */
     void setMoveCallback(           
        So3DSensorCB* func,         
        void* data=NULL             
        );
    /** remove callback for moving geometry */
    void removeMoveCallback(        
        );

private:
     ///destructor
	~So3DSensor(                   
        );
    ///pointer to press callback
    So3DSensorCB* pressFunc;   
    ///user data for press callback
    void* pressData;       
    ///pointer to release callback
    So3DSensorCB* releaseFunc;  
    ///user data for release callback
    void* releaseData;        
    ///pointer to move callback
    So3DSensorCB* moveFunc;         
    ///user data for move callback
    void* moveData;                 

}; //So3DSensor

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
