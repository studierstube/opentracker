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
 * header file for SoBuildPanelAction
 *
 * @author  Valerie Maquil
 *
 * $Id: SoBuildPanelAction.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SOBuildPanelAction_H__
#define __SOBuildPanelAction_H__


#include <Inventor/actions/SoSubAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <stbapi/widgets/SoTextListBox.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoWidgetLayoutGroup.h"
#include "SoWidgetSelectionFromPuc.h"
#include "..\puc\SoPucGroup.h"

#include "SoBuildStbStyleAction.h"
#include "SoBuildPucStyleAction.h"
#include "SoBuildPucExtStyleAction.h"

#include <string>
#include <stdlib.h>

#define NORMAL 0
#define PANELGROUP 1


/**
 * action class that traverses only the upper hierarchies of the PUC scenegraph 
 * and creates a SoPanelGroup if the puc group has one enumerated state and a 
 * certain number of puc groups as children. The remaining generation process concerning 
 * the layout of the different panels is then forwarded to the specified layout action.
 *
 * @ingroup puc
 * @see SoPanelGroup, SoPanel, 
 * 
*/
class PUC_PIP_LAYOUT_API SoBuildPanelAction : public SoAction {
	SO_ACTION_HEADER(SoBuildPanelAction);

public:

	/** initialize the class
	 * (must be called before any instance of the class is created)
	 */
	static void initClass();

	/// constructor
	SoBuildPanelAction();
    /// destructor
	virtual ~SoBuildPanelAction();

	///returns the scenegraph containing all the UI-information
	SoSeparator *getPipLayout() const {return pipLayout; }

	///sets the dimensions of the whole UI (widthMain, depthMain, heightMain)
	void setDimensions(float width, float heigth, float depth);

	///sets the style for the whole UI
	void setStyle(int style);
	
	///sets the number of columns used for the style PUCEXT
	void setColumns(int columns);
	
	///sets the number of units used for the style STB
	void setUnits(int units);


protected:

	static const int errorReturnCode ;	
	///is called before traversal of the scenegraph
	virtual void beginTraversal(SoNode *node);
	///is called after traversal of the scenegraph
	virtual void endTraversal(SoNode *node);


private:
	///node list containing different layout groups for each panel
	SoNodeKitListPart *sheetsNodeList;
	///
	SoWidgetLayoutGroup *layoutGroup;

	int traversalState;
	float widthMain;	///<width used to generate the layout
	float depthMain;	///<depth used to generate the layout
	float heightMain;	///<height used to generate the layout

	enum style	{PUC = 0,STB = 1,PUCEXT = 2} ; ///<different possibilities for the style
	int actionStyle; ///<style used to generate the layout
	int units; ///<number of units used for the style STB
	int columns; ///<number of columns used for the style PUCEXT
	
	///constitutes the root of the layout scenegraph
	SoSeparator *pipLayout;

	///starts the traversal of the specified style action for the part of the group tree corresponding to that panel sheet.
	SoWidgetLayoutGroup *applyLayoutAction(SoNode *node);

	/** function that is called for nodes that perform a default action behaviour
	 * e.g.separators
     */
	static void callDoAction(SoAction *, SoNode *);
	///function that is called for a SoPucGroup. The behavior depends on weather the node belongs to the first or second hierarchy of puc groups.
	static void pucGroupAction(SoAction *, SoNode *);

	//static void specificationActionError(std::string msg);

	///checks if the structure of group is a structure of a panel group
	bool isPanelStructure(SoPucGroup *group);

	///current hierarchical level
	int level;
	

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif