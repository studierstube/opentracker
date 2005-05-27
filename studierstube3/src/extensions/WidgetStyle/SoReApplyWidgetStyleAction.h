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
 * header file for SoReApplyWidgetStyleAction
 *
 * @author  Thomas Psik
 *
 * $Id: SoReApplyWidgetStyleAction.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SOSoReApplyWidgetStyleAction_H__
#define __SOSoReApplyWidgetStyleAction_H__

#include <Inventor/actions/SoSubAction.h>

#include "WidgetStyle.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <string>

class SoWidgetStyleGroup;
/**
 * action class that traverses the PUC scenegraph to create a PUCDeviceSpecificationMessage for PUC clients
 * is called from SoPuc	Device.
 * @ingroup widgetstyle
 */
class WIDGETSTYLE_API SoReApplyWidgetStyleAction : public SoAction {
	SO_ACTION_HEADER(SoReApplyWidgetStyleAction);

public:

	/** initialize the class
	 * (must be called before any instance of the class is created)
	 */
	static void initClass();

	/// constructor
	SoReApplyWidgetStyleAction();

	/// destructor
	virtual ~SoReApplyWidgetStyleAction();
	static void *createInstance  (  void    );
protected:
	virtual void beginTraversal(SoNode *node);

private:
	/** function that is called for nodes that perform a default action behaviour
	 * e.g.separators
     */
	static void labelListBoxAction(SoAction *, SoNode *);

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
