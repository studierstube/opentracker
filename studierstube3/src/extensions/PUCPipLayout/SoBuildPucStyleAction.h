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
 * header file for SoBuildPucStyleAction
 *
 * @author  Valerie Maquil
 *
 * $Id: SoBuildPucStyleAction.h 3521 2004-07-21 08:42:40Z tomp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SOBuildPucStyleAction_H__
#define __SOBuildPucStyleAction_H__


#include <Inventor/actions/SoSubAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoWidgetLayoutGroup.h"
#include "SoWidgetSelectionFromPuc.h"
#include "PucPipLayout.h"

#include <string>
#include <stdlib.h>


/**
 * action class that traverses the PUC scenegraph to create widgets for 
 * each state and connect them to their states using the PUC style. 
 *
 * The PUC style creates one row for each widget and places the label on his 
 * left. Each widget will have the same width, list boxes 
 * will be twice as deep as the other widgets and puc groups will be ignored.
 *
 * @ingroup puc
 * @see SoBuildPucExtStyleAction, SoBuildStbStyleAction
*/
class PUC_PIP_LAYOUT_API SoBuildPucStyleAction : public SoAction {
	SO_ACTION_HEADER(SoBuildPucStyleAction);

public:

	/** initialize the class
	 * (must be called before any instance of the class is created)
	 */
	static void initClass();

	/// constructor
	SoBuildPucStyleAction();
    /// destructor
	virtual ~SoBuildPucStyleAction();

	/**returns a pointer to a SoWidgetLayoutGroup containing all the UI-information 
	 * for the corresponding panel. This SoWidgetLayoutGroup constitutes thus the 
	 * root node of a part of the interface tree. */
	SoWidgetLayoutGroup *getLayoutGroup() {return mainLayoutGroup;}
	/**layout group representing the root of this part of the interface tree: All the 
	 * UI-information commuted by this action is added as children to this node.
	 */
	SoWidgetLayoutGroup *mainLayoutGroup;

protected:

	//static const int errorReturnCode ;	
	///is called before traversal of the scenegraph. Initializes the mainLayoutGroup.
	virtual void beginTraversal(SoNode *node);

private:
	///actual number of rows
	int curNumOfRows;

	///node list of the mainLayoutGroup
	SoNodeKitListPart *mainNodeList;

		
	/** function that is called for nodes that perform a default action behaviour
	 * e.g.separators
     */
	static void callDoAction(SoAction *, SoNode *);
	static void stateBoolAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateBool
	static void stateFixedAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateFixed
	static void stateFloatAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateFloat
	static void stateIntAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateInt
	static void stateStringAction(SoAction *, SoNode *);///<function that is called for a SoPucStateString
	static void stateEnumAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateEnumerated
	static void commandAction(SoAction *, SoNode *);	///<function that is called for a SoPucCommand
	static void pucGroupAction(SoAction *, SoNode *);	///<function that is called for a SoPucGroup

	static void specificationActionError(std::string msg);

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif