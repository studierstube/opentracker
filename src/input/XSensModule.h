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
/** header file for XSensModule
  *
  * @author Gerhard Reitmayr
  * 
  * $Id: XSensModule.h 717 2004-07-27 11:54:49Z reitmayr $
  *
  * @file                                                                   */
 /* ======================================================================= */
/**
 * @page module_ref Module Reference
 * @section XSensModule XSensModule 
 *
 * This module implements the device driver for XSens MT9-B inertial trackers.
 * The corresponding source node is @ref XSensSource.
 * Currently only a single MT9-B attached a comport is supported. It has no
 * configuration element but reserves the name @c XSensConfig for it.
 *
 * @note The implementation for Linux is missing. To run it on Windows, one needs
 * to install the MT9 SDK Software from Xsens. It will not work without it.
 */

#ifndef _XSENSMODULE_H
#define _XSENSMODULE_H

#include "../OpenTracker.h"


namespace ot {


class XSensSource;


/**
 * This module simply supports the XSensSource. It uses the MTObj.dll or respective mtobj.so from the
 * bin directory.
 *
 * @ingroup input
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API XSensModule : public Module, public NodeFactory
{
public:
	XSensModule();
	virtual ~XSensModule();

	virtual Node * createNode( const std::string & name, StringTable & attributes );
	
	virtual void pushState();

	virtual void start();

	virtual void close();

protected:
    XSensSource * source;
};


}  // namespace ot


#endif // !defined(_XSENSMODULE_H)
