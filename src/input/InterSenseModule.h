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
/** header file for InterSenseModule module.
  *
  * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/InterSenseModule.h,v 1.6 2003/07/18 17:27:58 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section intersensemodule InterSenseModule
 * The InterTraxModule provides and drives @ref intersensesource nodes that 
 * generate standard events in certain intervals. It uses a configuration 
 * element called @c InterSenseConfig, which in turn contains one element
 * called @c ISTracker per configured tracker. Only trackers that are
 * configured will be used. The @c ISTracker element has the following 
 * attributes :
 * @li @c id a unique id to identify the tracker, this the same as the id 
 *           attribute of the @ref intersensesource nodes.
 * @li @c comport the serial port the tracker is connected to. If 0 then
 *        the first one is used or an InterTrax2 connected to the USB port.
 *
 * An example configuration element looks like this :
 * @verbatim
<InterSenseConfig>
    <ISTracker comport="0" id="InterTrax"/>
</InterSenseConfig>@endverbatim
 *
 * Note that this driver has not been fully tested, because we have only
 * InterTrax2 devices in our lab. The ISTracker element is a placeholder for
 * more advanced configuration options in the future.
 */

#ifndef _INTERSENSEMODULE_H
#define _INTERSENSEMODULE_H

#include "../OpenTracker.h"

struct ISTracker;

typedef std::vector<ISTracker *> ISTrackerVector;

/**
 * The module and factory to drive the InterSenseSource nodes. It constructs
 * InterSenseSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 *
 * On Windows it relies on the isense.dll and itrax2.dll distributed with 
 * OpenTracker in the bin directory. These include support for USB based
 * InterTrax2 devices. The Unix version relies on code from the Studierstube.
 * This code also includes a Windows variant, but it is not used because of
 * lack of USB support.
 *
 * @author Ivan Viola, Matej Mlejnek, Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API InterSenseModule : public Module, public NodeFactory
{
// Members
protected:
    /// stores information about the configured trackers and the
    /// sources.
    ISTrackerVector trackers;

// Methods
public:
    /** constructor method. */
    InterSenseModule() : 
        Module(), 
        NodeFactory()        
    { 
    };
    /** Destructor method, clears nodes member. */
    virtual ~InterSenseModule();
    /**
     * initializes the tracker module. For each configured tracker it
     * allocates an ISTracker structure and initializes the tracker.
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes,  ConfigNode * localTree);
    /** This method is called to construct a new Node. It compares
     * name to the InterTraxSource element name, and if it matches
     * creates a new InterTraxSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);

    /**
     * starts the module. Here we will reset the heading of any pure orientation
     * tracker. */
    virtual void start();

	/**
     * closes InterSense library */
    virtual void close();
    /**
     * pushes events into the tracker tree. Checks all trackers for new data
     * and fires the InterSenseSources, if new data is present.
     */
    virtual void pushState();
};

#endif
