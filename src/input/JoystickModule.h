 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for JoystickModule module.
  *
  * @author Rainer Splechtna
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section JoystickModule JoystickModule
 * The JoystickModule provides and drives @ref JoystickSource nodes that 
 * generate events. The member position of the state gives the 
 * current normalized position of the joystick. The x,y and z components 
 * of position reflect the current position of the joystick's x,y and z-axes
 * (ranging from -1 to +1, e.g. -1,-1,z means that the joystick is in the upper
 * left corner). The member orientation is not used. The member button is used
 * as usual (note that right now only the first four buttons of a joystick are 
 * used).
 *
 * It doesn't have a configuration element
 * but reserves the name 'JoystickConfig' for it. It does something useful as soon as
 * at least one @ref JoystickSource node is created.
 */

#ifndef _JOYSTICKMODULE_H
#define _JOYSTICKMODULE_H

#include "../OpenTracker.h"

#ifdef USE_JOYSTICK

#include <windows.h>
#include <mmsystem.h>

#include "../dllinclude.h"

#include <vector>

namespace ot {

typedef std::vector<Node*> NodeVector;

/**
 * The module and factory to drive the JoystickSource nodes. It constructs
 * JoystickSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration. The implementation uses the 
 * winmm-library and therfore supports only two joysticks. 
 * @author Rainer Splechtna
 * @ingroup input
 */
class OPENTRACKER_API JoystickModule : public ThreadModule, public NodeFactory
{
// Members
protected:
    /// stores which joysticks are available
    int joyPresent[2];
    /// stores capabilities of joysticks
    JOYCAPS   joyCaps[2];

    /// stores x-axis range of joysticks
    int xRange[2];
    /// stores y-axis range of joysticks
    int yRange[2];
    /// stores z-axis range of joysticks
    int zRange[2];

    /** Updates member joyCaps for specified joystick ID.
     *  @param id joystick ID */
    void getCaps(int id);
    /// list of JoystickNode nodes in the tree
    NodeVector nodes;

    /** This method is the code executed in its own thread. It polls all
      * present joysticks and updates the tmpState of the associated 
      * @ref JoystickSource-nodes. */
    virtual void run();

    /// flag to stop the thread
    int stop;

// Methods
public:
    /** constructor method. */
    JoystickModule() : ThreadModule(), NodeFactory(), stop(0)
    {
    };

    /** Destructor method, clears nodes member. */
    virtual ~JoystickModule();

    /** This method is called to construct a new Node. It compares
     * name to the JoystickNode element name, and if it matches
     * creates a new JoystickNode node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);
    /**
     * Pushes events into the tracker tree. Checks all JoystickSource nodes and
     * pushes new events, if a JoystickSource node fires.
     */
    virtual void pushState();

    /**
     * Tests for available joysticks and starts the polling-thread.
     */
    virtual void start();

    /// Only sets the stop flag to 1.
    virtual void close();

private:

    /// This method polls all present joysticks.
    void pollJoysticks();
};

} // namespace ot

#endif
#endif
