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
 * header file for SoPucBuildLayoutAction
 *
 * @author  Valerie Maquil
 *
 * $Id: SoBuildStbStyleAction.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SoBuildStbStyleAction_H__
#define __SoBuildStbStyleAction_H__


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoBuildPucStyleAction.h"



/**
 * action class that traverses the PUC scenegraph to create widgets for each state 
 * and connect them to their states using the STB style. 
 * 
 * The STB style puts the widgets next to each other in the same row. A new row is 
 * created when the previous one is full (defined with the field "units" in 
 * SoPucPipLayout) or when the widget to place belongs to another puc group. All 
 * widgets will have the same depth, but sliders and list boxes will be twice as 
 * wide as push buttons or toggle buttons. One unit corresponds to the width of one 
 * push button or toggle button. Sliders and list boxes have a width of two units.
 *
 * @ingroup puc
 * @see SoBuildPucStyleAction, SoBuildPucExtStyleAction
 */

class PUC_PIP_LAYOUT_API SoBuildStbStyleAction : public SoBuildPucStyleAction {
	SO_ACTION_HEADER(SoBuildStbStyleAction);

public:

	/** initialize the class
	 * (must be called before any instance of the class is created)
	 */
	static void initClass();

	/// constructor
	SoBuildStbStyleAction();
    /// destructor
	virtual ~SoBuildStbStyleAction();
	///sets the number of units per row
	void setUnits(int units);



protected:

	///is called before traversal of the scenegraph. Initializes the mainLayouGroup.
	virtual void beginTraversal(SoNode *node);
	///is called after traversal of the scenegraph
	virtual void endTraversal(SoNode *node);

private:
	///number of units per row specified by the user
	int numOfUnits;
	///current number of units used in the current row
	int curNumOfUnits;
	///current number of columns (i.e. widgets) added in the current row
	int curNumOfCols;
	///is TRUE if the current row is the first row
	bool first;
	
	///current row where the next widget will be added
	SoWidgetLayoutGroup *layoutGroupPerRow;
	///node list of the layoutGroupPerRow
	SoNodeKitListPart *nodeList;
	///node list of the mainLayoutGroup
	SoNodeKitListPart *mainNodeList;

		
	/** function that is called for nodes that perform a default action behaviour
	 * e.g.separators
     */
	static void callDoAction(SoAction *, SoNode *);
	static void stateBoolAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateBool
	static void stateFixedAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateBool
	static void stateFloatAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateBool
	static void stateIntAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateInt
	static void stateEnumAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateEnumerated
	static void commandAction(SoAction *, SoNode *);	///<function that is called for a SoPucCommand
	static void pucGroupAction(SoAction *, SoNode *);	///<function that is called for a SoPucGroup

	///initializes a new SoWidgetLayoutGroup for the next row
	void initLayoutGroup();
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif