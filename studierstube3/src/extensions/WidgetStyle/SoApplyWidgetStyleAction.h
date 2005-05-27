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
 * header file for SoApplyWidgetStyleAction
 *
 * @author  Thomas Psik
 *
 * $Id: SoApplyWidgetStyleAction.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SOSoApplyWidgetStyleAction_H__
#define __SOSoApplyWidgetStyleAction_H__

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
class WIDGETSTYLE_API SoApplyWidgetStyleAction : public SoAction {
	SO_ACTION_HEADER(SoApplyWidgetStyleAction);

public:

	/** initialize the class
	 * (must be called before any instance of the class is created)
	 */
	static void initClass();

	/// constructor
	SoApplyWidgetStyleAction();

	/// destructor
	virtual ~SoApplyWidgetStyleAction();
	bool inFile;
	bool retrieveStyle;
protected:

	static const int errorReturnCode ;	

	virtual void beginTraversal(SoNode *node);

private:
	SoWidgetStyleGroup *activeStyle;

	/** function that is called for nodes that perform a default action behaviour
	 * e.g.separators
     */
	static void callDoAction(SoAction *, SoNode *);
	static void callDoFileAction(SoAction *, SoNode *);
	//	static void groupSpecification(SoAction *, SoNode *);
 
	// used for common puc elements (group)
	static void endAction(SoAction *, SoNode *);
	static void group(SoAction *, SoNode *);
	static void switchAction(SoAction *, SoNode *);

	static void widget(SoAction *, SoNode *);
	static void style(SoAction *, SoNode *);
	static void layoutKit(SoAction *action_, SoNode *node);

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
