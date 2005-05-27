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
 * header file for SoGetPucSpecificationAction
 *
 * @author  Thomas Psik
 *
 * $Id: SoGetPUCSpecificationAction.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SOSoGetPucSpecificationAction_H__
#define __SOSoGetPucSpecificationAction_H__


#include <Inventor/actions/SoSubAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "PUC.h"

#include <string>

class SoPucActiveIfNode;
class SoPucElementBase;

/**
 * action class that traverses the PUC scenegraph to create a PUCDeviceSpecificationMessage for PUC clients
 * is called from SoPuc	Device.
 * @ingroup puc
 */
class PUC_API SoGetPucSpecificationAction : public SoAction {
	SO_ACTION_HEADER(SoGetPucSpecificationAction);

public:

	/** initialize the class
	 * (must be called before any instance of the class is created)
	 */
	static void initClass();

	/// constructor
	SoGetPucSpecificationAction();
    /// destructor
	virtual ~SoGetPucSpecificationAction();

	std::string getPucSpecification() const {return pucSpec; }

protected:

	static const int errorReturnCode ;	

	virtual void beginTraversal(SoNode *node);

private:
	std::string pucSpec;
	std::string pucPath;
	bool withInActiveIf;

	/** function that is called for nodes that perform a default action behaviour
	 * e.g.separators
     */
	static void callDoAction(SoAction *, SoNode *);
	static void deviceSpecification(SoAction *, SoNode *);

	//	static void groupSpecification(SoAction *, SoNode *);

	// used for common puc elements (group)
	static void stdElementBaseSpecification(SoAction *, SoNode *);
	static void stdElementBaseSpecificationStart(SoGetPucSpecificationAction *, SoPucElementBase *);
	static void stdElementBaseSpecificationEnd(SoGetPucSpecificationAction *, SoPucElementBase *);

	static void stdElementBaseInformation(SoAction *, SoNode *);

	static void commandSpecification(SoAction *, SoNode *);

	// functions to acumulate state information
    static void stdStateBaseSpecificationStart(SoAction *action, SoNode *node);
    static void stdStateBaseSpecificationEnd(SoAction *action, SoNode *node);
    static void stdStateBaseSpecification(SoAction *, SoNode *);

	// specific elements for state type
  	static void stateIntSpecification(SoAction *, SoNode *);
	static void stateFloatSpecification(SoAction *, SoNode *);
	static void stateFixedSpecification(SoAction *, SoNode *);
	static void stateEnumeratedSpecification(SoAction *, SoNode *);

	// specific elements for activeIf
  	static void activeIfOperator(SoAction *, SoNode *);
	static void activeIfClause(SoAction *, SoNode *);

	// called from within the code and not by specifing in action
	static void activeIfNode(SoGetPucSpecificationAction *, SoPucActiveIfNode *);

	static void stdStateBaseValueLabels(SoAction *action, SoNode *node);

	static void specificationActionError(std::string msg);

	static std::string getBaseAttributesStr(SoPucElementBase *elm);				/// helper function
	static std::string getPucActiveIfOperatorOpStr(SoSFEnum &op);			/// helper function
	static std::string getPucActiveIfIgnoreStr(SoSFEnum &ignore);			/// helper function
	static std::string getPucActiveIfClauseOpStr(SoSFEnum &op);			/// helper function

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
