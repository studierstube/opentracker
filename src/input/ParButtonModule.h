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
/** header file for ParButton module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ParButtonModule.h,v 1.1 2001/08/04 18:07:31 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section parbuttonmodule ParButtonModule (Parallel Button Input)
 * 
 * User guide description of what it does here
 * list of configuration attributes :
 *
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

#ifndef _PARBUTTONMODULE_H
#define _PARBUTTONMODULE_H

#include "../OpenTracker.h"

/**
 * developer level information and implementation specifics here
 *
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API ParButtonModule : public Module, public NodeFactory
{
// Members
protected:
    /// maps devices to nodes
    std::map<std::string, Node *> nodes;
public:
    /** constructor method. */
    ParButtonModule() : 
        Module(), 
        NodeFactory()        
    {}
    /** This method is called to construct a new Node. It compares
     * name to the InterTraxSource element name, and if it matches
     * creates a new InterTraxSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);    
	/**
     * closes devices */
    virtual void close();
    /**
     * pushes events into the tracker tree. Tries to read data from the
     * opened parallel ports and passes the events on.
     */
    virtual void pushState();
};

#endif

