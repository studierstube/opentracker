/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
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
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: animal
 * ======================================================================== */
/**
 * header file for SoPucTriggerCommandAction
 *
 * @author  Thomas Psik
 *
 * $Id: SoPucTriggerCommandAction.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SOSoPucTriggerCommandAction_H__
#define __SOSoPucTriggerCommandAction_H__


#include <Inventor/actions/SoSubAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "PUC.h"

#include <string>
/**
 * action class that traverses the PUC scenegraph to find a specific PUCCommand to fire trigger
 * is called from SoPUCDevice.
 * @ingroup puc
 */
class PUC_API SoPucTriggerCommandAction : public SoAction {
	SO_ACTION_HEADER(SoPucTriggerCommandAction);

public:

	/** initialize the class
	 * (must be called before any instance of the class is created)
	 */
	static void initClass();

	/// constructor
	SoPucTriggerCommandAction();

	void setServer(PUCDeviceServer *deviceServer_);

    /// destructor
	virtual ~SoPucTriggerCommandAction();

	void setData(std::string commandName_);

protected:

	virtual void beginTraversal(SoNode *node);

private:
	PUCDeviceServer *deviceServer;

	std::string commandName;

	/** function that is called for nodes that perform a default action behaviour
	 * e.g.separators
     */
	static void callDoAction(SoAction *, SoNode *);

	// used for common puc states (int, ...)
	static void stdCommand(SoAction *, SoNode *);

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
