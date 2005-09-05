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
/** header file for Ubisense source node.
  *
  * @author Mathis Csisinko
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section ubisensesource UbisenseSource
 * The UbisenseSource node is a simple EventGenerator that inserts events 
 * by making use of the Ubisense API. See the @ref ubisensemodule for
 * information about how to configure the module as a whole. It has the
 * following elements :
 * @li @c object the name of any Ubisense object which should be tracked
 *
 * An example element looks like this :
 * @verbatim
<UbisenseSource object="Person" />@endverbatim
 */

#ifndef _UBISENSESOURCE_H
#define _UBISENSESOURCE_H

#include "../OpenTracker.h"

#ifdef USE_UBISENSE

#include "UbisenseModule.h"
#include "UClientAPI/location_client.h"
#include "UClientAPI/data_client.h"

using namespace UClientAPI;

namespace ot {

/** A UbisenseSource represents a single Ubisense object that is tracked by the 
 * Ubisense library.
 * @ingroup input
 */
class OPENTRACKER_API UbisenseSource: public Node
{
	friend UbisenseModule;
	friend UbisenseModule::WrappedDataClient;

// methods
protected:
    /** constructor method
     * @param object the Ubisense object to track
     * @param locationClient a Ubisense LocationClient instance
     * @param dataClient a Ubisense DataClient instance
     */
	UbisenseSource(const Object &object,const LocationClient &locationClient,DataClient &dataClient);

    /// destructor method
	virtual ~UbisenseSource();

    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator() { return 1; }

	const Object & getObject() const { return object; }

	bool calcState();

	State & getState() { return state; }

	void setButtonState(unsigned short,double);

// members
private:
	unsigned short button;
	double lastTime,buttonTime;
	State state;
	const Object object;
	const LocationClient &locationClient;
	DataClient &dataClient;
};

} // namespace ot

#endif  // USE_UBISENSE

#endif
