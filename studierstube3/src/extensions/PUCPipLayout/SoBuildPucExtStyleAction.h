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
 * $Id: SoBuildPucExtStyleAction.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SoBuildPucExtStyleAction_H__
#define __SoBuildPucExtStyleAction_H__


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoBuildPucStyleAction.h"
#include <Inventor/lists/SbIntList.h>


/**
 * action class that traverses the PUC scenegraph to create widgets for each state and connect them to their states using the 
 * PUCEXT style. 
 * The PUCEXT style represents an extended version of the PUC style. The widgets will be distributed on a given
 * number of columns (defined with the field "columns"), puc groups will be ignored and list boxes will be twice as deep as the
 * other widgets. Each widget will have the same width and their labels will be placed on the left.
 *
 * @ingroup puc
 * @see SoBuildPucStyleAction, SoBuildStbStyleAction
 */

class PUC_PIP_LAYOUT_API SoBuildPucExtStyleAction : public SoBuildPucStyleAction {
	SO_ACTION_HEADER(SoBuildPucExtStyleAction);

public:

	/** initialize the class
	 * (must be called before any instance of the class is created)
	 */
	static void initClass();

	/// constructor
	SoBuildPucExtStyleAction();
    /// destructor
	virtual ~SoBuildPucExtStyleAction();

	///sets the number of columns which should be used to generate the layout
	void setCols(int cols);



protected:

	///is called before traversal of the scenegraph
	virtual void beginTraversal(SoNode *node);
	///is called after traversal of the scenegraph
	virtual void endTraversal(SoNode *node);

private:
	///number of columns for the layout
	int numOfCols;
	///represents the actual number of rows for each column
	SbIntList numRows;
	///represents the actual height of each column
	SbIntList columnsHeight;

	///column with the lowest height where the next widget will be added
	SoWidgetLayoutGroup *layoutGroupPerCol;
	SoNodeKitListPart *nodeList;
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
	static void stateEnumAction(SoAction *, SoNode *);	///<function that is called for a SoPucStateEnumerated
	static void commandAction(SoAction *, SoNode *);	///<function that is called for a SoPucCommand
	static void pucGroupAction(SoAction *, SoNode *);	///<function that is called for a SoPucGroup

	/**returns the index of the column having the smallest relative height. This height is commuted by subsequently 
	summing up the relative depths of the widgets placed in this column */
	int getMinIndex(SbIntList list, int numCols);
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif