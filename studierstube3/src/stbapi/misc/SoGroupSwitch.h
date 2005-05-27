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
/** SoGroupSwitch header file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoGroupSwitch.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOGROUPSWITCH_H_
#define _SOGROUPSWITCH_H_

#include "SoMultiSwitch.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class STBAPI_API SoGroupSwitch;

/** 
 * This callback function is used to test whether a certain set of children
 * should be included in the traversal. 
 * @param userdata pointer to user supplied data
 * @param switchNode pointer to the SoGroupSwitch node calling
 * @param action pointer to the action currently applied
 * @param number the number of the set to be traversed
 */
typedef SbBool SoGroupSwitchCB( void * userdata, SoGroupSwitch * switchNode, 
                                SoAction * action, int number );

/**
 * The SoGroupSwitch is an extension to the SoMultiSwitch node to allow some
 * experiements with the context sensitive scene graph concept. It is now 
 * superceded by the context sensitive rendering nodes around SoContext.
 * It allows to
 * define subsets of its children and the SoGroupSwitchElement selects during 
 * traversal which sets of children will be traversed. Before each set is added to
 * the sequence of children a callback function is called to test whether it
 * should be included. The ordered field behaves as in the SoMultiSwitch node.
 *
 * Note that the values SO_SWITCH_NONE, SO_SWITCH_INHERIT and SO_SWITCH_ALL have
 * no meaning in the whichChildren field anymore. Only SO_SWITCH_NONE and SO_SWITCH_ALL
 * are supported, if they appear in the state.
 *
 * The SoSwitchSelect node allows to set the value of the SoGroupSwitchElement
 * from within the scene graph. 
 *
 * File format :
<pre>SoGroupSwitch {
  SFBool  ordered       TRUE
  MFInt32 whichChildren [-1]
  MFInt32 numChildren   [-1]
}</pre>
 *
 * @author Gerhard Reitmayr
 * @ingroup util
 */
class STBAPI_API SoGroupSwitch : public SoMultiSwitch
{
    SO_NODE_HEADER(SoGroupSwitch);

public:

    /** 
     * this field stores the number of indices in the whichChildren field
     * that belong to each subset of children. It works similar to the
     * fields in SoFaceSet.
     */
    SoMFInt32 numChildren;

    /** static method to initialize class for OpenInventor. It also
     * initializes SoGroupSwitchElement and enables it for all necessary
     * actions.  
     */
    static void initClass();

    /// constructor
    SoGroupSwitch();

    /// constructor that takes approximate number of children.
    SoGroupSwitch(int numChildren);

    /**
     * sets the test callback to be used for testing sets to be traversed.
     * @param func pointer to the callback function
     * @param data void pointer that is passed to the user function */
    void setGroupCallback( SoGroupSwitchCB * func, void * data = NULL );
    
    /**
     * removes a set callback again. Now sets will be traversed again without 
     * testing. */
    void removeGroupCallback();

    /** Overrides method in SoMultiSwitch to return default value
     * because we can only determine within an action which nodes to process. */
    virtual SbBool affectsState() const;

protected:
    
    /** does the actual action work. It computes which children to traverse
     * and implements some special behaviour for bounding box actions. 
     * @param action the action to apply to the children
     */
    virtual void doAction(SoAction *action);    

    /// destructor
    virtual ~SoGroupSwitch();

    /// stores the function pointer or NULL, if no group callback function is set.
    SoGroupSwitchCB * cbGroupFunc;

    /// stores the user data pointer or NULL, if no group callback function is set.
    void * userDataGroup;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
