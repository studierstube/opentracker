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
/** header file for the Ubisense interface module.
  *
  * @author Mathis Csisinko
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section ubisensemodule UbisenseModule
 * The UbisenseModule interfaces to the Ubisense API to track the position and
 * button state of Ubisense tags. It uses the information provided by @ref
 * ubisensesource nodes and inserts the stateevents through @ref
 * ubisensesource nodes into the tree. The configuration element is @c
 * UbisenseConfig.
 * Without the @c UbisenseConfig configuration element data from any cell is
 * tracked. But as soon as a @c UbisenseConfig configuration element is present,
 * data retrieval is restricted: A single @c UbisenseCell configuration element
 * identifies a Ubisense location cell to retrieve data from. Use more than one
 * @c UbisenseCell configuration element to retrieve data from any number of
 * cells. The @c UbisenseCell configuration element has the following attribute:
 * @li @c name a string identifying the Ubisense location cell.
 *
 * An example configuration element looks like this :
 * @verbatim
<UbisenseConfig>
    <UbisenseCell name="Location Cell 00001" />
</UbisenseConfig>@endverbatim
 */

#ifndef _UBISENSEMODULE_H
#define _UBISENSEMODULE_H

#include "../OpenTracker.h"

#ifdef USE_UBISENSE

#include "UClientAPI/location_client.h"
#include "UClientAPI/data_client.h"

using namespace UClientAPI;

namespace ot {

/**
 * A Ubisense tracking source module using the Ubisense API to track Ubisense
 * tags. It acts as a NodeFactory for UbisenseSource nodes. It also keeps a
 * vector of the created nodes to update them.
 *
 * @ingroup input
 */

class OPENTRACKER_API UbisenseModule: public Module,public NodeFactory
{
// methods
public:
    /// constructor method
    UbisenseModule();

    /// destructor method
    virtual ~UbisenseModule();

    /** This method is called to construct a new Node. It compares
     * name to the UbisenseSource element name, and if it matches
     * creates a new UbisenseSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL.
	 */
    virtual Node* createNode(const std::string &name,StringTable &attributes);

    /**
     * This method is called after initialisation is finished and before the
     * main loop is started.
	 */
    virtual void start();

    /**
     * closes the module.
	 */
    virtual void close();

    /**
     * pushes events into the tracker tree. Checks all source nodes for
     * new states and pushes them into the tracker tree.
     */
    virtual void pushState();

    /**
     * initializes the Ubisense module. 
     * @param attributes StringMap of elements attribute values.
     * @param pLocalTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable &attributes,ConfigNode* pLocalTree);

// members
protected:
    /// stores the sources
    NodeVector pSources;

private:
	class WrappedDataClient: public DataClient
	{
	public:
		WrappedDataClient(NodeVector &);

		virtual void on_button(const Object &,UbitagButton,double);

	private:
		NodeVector &pSources;
	};

	LocationClient locationClient;
	WrappedDataClient dataClient;
	UClientAPI::Set<String> cells; 
	//std::set<std::string> cells;
};

} // namespace ot

#endif  // USE_UBISENSE

#endif
