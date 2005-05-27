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
/** SoSwitchSelect header file
  *
  * @author Gerhard Reitmayr
  *
  * $Id: SoSwitchSelect.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOSWITCHSELECT_H_
#define _SOSWITCHSELECT_H_

#include <stbapi/stbapi.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSearchAction.h>

#include "SoGroupSwitchElement.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**  
 * This class sets the SoGroupSwitchElement in the state of the current action.
 * This is an easy way to select which groups of children a SoGroupSwitch node
 * should traverse.
 *
 * File format :
<pre>SoSwitchSelect {
  MFInt32 groups [-1]
}</pre>
 *
 * @author Gerhard Reitmayr
 * @ingroup util
 */
class STBAPI_API SoSwitchSelect : public SoNode 
{
    SO_NODE_HEADER(SoSwitchSelect);
public:

    /** This field containes the indices of the groups of children in a
     * SoGroupSwitchs numChildren field. Its value is simply copied into
     * the state at traversal. */
    SoMFInt32 groups;

    /// static class initialization method of OpenInventor
    static void initClass();
    
    /// Constructor
    SoSwitchSelect();

protected:
    virtual void GLRender(SoGLRenderAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void handleEvent(SoHandleEventAction *action);
    virtual void pick(SoPickAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void search(SoSearchAction *action);

    /** does the actual action work. It just sets the SoGroupSwitchElement
     * state.
     */
    virtual void doAction(SoAction *action);

    static void  callDoAction(SoAction *, SoNode *);    
private:
    /// destructor
    ~SoSwitchSelect();
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
