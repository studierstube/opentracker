/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** Implementation of class SoPanelGroup
  *
  * @author Valerie Maquil
  *
  * $Id: SoPanelGroup.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#include "SoPanelGroup.h"


#if defined(_DEBUG_PANEL_GROUP) && defined(DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif


SO_KIT_SOURCE(SoPanelGroup);

//-----------------------------------------------------------------------------------------------------

void SoPanelGroup::initClass()
{
    SO_KIT_INIT_CLASS(SoPanelGroup,  SoWidgetLayoutGroup, "SoWidgetLayoutGroup");
}

//-----------------------------------------------------------------------------------------------------

SoPanelGroup::SoPanelGroup()
{
    SO_KIT_CONSTRUCTOR(SoPanelGroup);


	//--------------------------------------+------------+------+---------------+-------+---------------
	//                                      |            |      |               | right |
	//                                      |            | null |               | Sib-  | is
	//                                      |            | By   |               | ling  | public
	//                      name            | className  | Def. |  parent Name  | Name  | part
	//--------------------------------------+------------+------+---------------+-------+---------------
	SO_KIT_ADD_CATALOG_ENTRY(titles,		SoLabelListBox,	 TRUE,  hiddenGeometry,          \x0,     TRUE);
	SO_KIT_ADD_CATALOG_LIST_ENTRY(sheets,	SoGroup,		 TRUE, hiddenGeometry,          \x0, SoLayoutKit,    TRUE);

	SO_KIT_INIT_INSTANCE();	
  
    SO_KIT_ADD_FIELD( panelIndex, (0));
	SO_KIT_ADD_FIELD( panelIndexIn, (0));
	SO_KIT_ADD_FIELD( numOfButtons, (3));
	SO_KIT_ADD_FIELD( titlesToSheetsRatio, (0.20));

	SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, TOP);
    SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, LEFT);
    SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, RIGHT);
    SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, BOTTOM);
    SO_NODE_SET_SF_ENUM_TYPE(navigationPlacing, NavigationPlacing);

	SO_KIT_ADD_FIELD( navigationPlacing, (TOP));

	panelIndexSensor.setData(this);
	panelIndexSensor.setFunction(SoPanelGroup::panelIndexChangedCB );

	panelIndexInSensor.setData(this);
	panelIndexInSensor.setFunction(SoPanelGroup::panelIndexInChangedCB );
	
	sheetsSensor.setData(this);
	sheetsSensor.setFunction(SoPanelGroup::sheetsChangedCB);
	layoutPanel = NULL;

	setUpConnections(TRUE, TRUE);

	//resizeCB();
}

//-----------------------------------------------------------------------------------------------------

void SoPanelGroup::sheetsChangedCB( void * data, SoSensor * sensor )
{	
	SoPanelGroup *self = (SoPanelGroup*) data;
	assert(self);

	self->updateLayout();
}

//-----------------------------------------------------------------------------------------------------

void SoPanelGroup::updateLayout()
{	

	DEBUG_CODE(printf("SoPanelGroup::updateLayout \n"););
	SoNodeKitListPart *nodeListElements = SO_GET_PART(this, "elements", SoNodeKitListPart);
	while (nodeListElements->getNumChildren() !=0)
		nodeListElements->removeChild(0);

	
	//list box containing the titles of the panels
	SoLabelListBox *titlesListBox = (SoLabelListBox *)titles.getValue();

	//WidgetLayoutGroup containing the different contents of the panels
	SoNodeKitListPart *nodeListSheets = SO_GET_PART(this, "sheets", SoNodeKitListPart);
	
	if (layoutPanel == NULL)
		layoutPanel = new SoPanel();
	
	SoNodeKitListPart *panelElements = SO_GET_PART(layoutPanel, "elements", SoNodeKitListPart);

	while (panelElements->getNumChildren() !=0)
		panelElements->removeChild(0);

	int numSheets = nodeListSheets->getNumChildren();
	
	for (int i = 0; i<numSheets; i++)
	{
		panelElements->addChild(nodeListSheets->getChild(i));
	}
	
	int sizeTitles = (int)(100*titlesToSheetsRatio.getValue());
	int sizeSheets = 100 - sizeTitles;

	//both are added to the element part of this panel group
	if (navigationPlacing.getValue()==LEFT)
	{
		numOfCols.setValue(2);
		numOfRows.setValue(1);
		sizeOfCols.set1Value(0,sizeTitles);
		sizeOfCols.set1Value(1,sizeSheets);
		titlesListBox->numOfRows.setValue(numOfButtons.getValue());
		titlesListBox->numOfCols.setValue(1);
		titlesListBox->navigationPlacing.setValue(6);
		titlesListBox->navigationSize.setValue(0.1);
		titlesListBox->spacingDepth.setValue(0.03);
		titlesListBox->startEndSpacingDepth.setValue(0.03);
		nodeListElements->addChild(titlesListBox);
		nodeListElements->addChild(layoutPanel);
	}
	else if (navigationPlacing.getValue()==TOP)
	{
		numOfRows.setValue(2);
		numOfCols.setValue(1);
		sizeOfRows.set1Value(0,sizeTitles);
		sizeOfRows.set1Value(1,sizeSheets);
		titlesListBox->numOfRows.setValue(1);
		titlesListBox->numOfCols.setValue(numOfButtons.getValue());
		titlesListBox->navigationPlacing.setValue(5);
		titlesListBox->navigationSize.setValue(0.1);
		titlesListBox->spacingWidth.setValue(0.03);
		titlesListBox->startEndSpacingWidth.setValue(0.03);
		nodeListElements->addChild(titlesListBox);
		nodeListElements->addChild(layoutPanel);
	}

	sheetsSensor.detach();

	buildPipLayout();  

//	writeFile("panelGeom.iv",this,FALSE);
}

//-----------------------------------------------------------------------------------------------------

void SoPanelGroup::resizeCB()
{	
	DEBUG_CODE(printf("SoPanelGroup::resizeCB \n"););
	buildPipLayout();
}

//-----------------------------------------------------------------------------------------------------
	
SbBool SoPanelGroup::setUpConnections(SbBool onOff, SbBool doItAlways)
{
		// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoWidgetLayoutGroup::setUpConnections(onOff, doItAlways);

		SoLabelListBox *titlesListBox = (SoLabelListBox *)titles.getValue();
		if (titlesListBox != NULL)
			panelIndex.connectFrom(&titlesListBox->selectedIndices);

		
		//connect the field sensors
		panelIndexSensor.attach( &panelIndex );
	    panelIndexInSensor.attach( &panelIndexIn );

		if(sheets.getValue() != NULL)
		sheetsSensor.attach(sheets.getValue());


    }
    else {
        // We disconnect BEFORE base class. 
		panelIndexSensor.detach();
        panelIndexInSensor.detach();
 		sheetsSensor.detach();
       // Disconnect the field sensors.
        SoWidgetLayoutGroup::setUpConnections(onOff, doItAlways);


    }
    return !(connectionsSetUp = onOff);
}


//-----------------------------------------------------------------------------------------------------

SoPanelGroup::~SoPanelGroup()
{

}

//-----------------------------------------------------------------------------------------------------

void SoPanelGroup::panelIndexChangedCB( void * data, SoSensor * sensor )
{	
	SoPanelGroup *self = (SoPanelGroup*) data;
	assert(self);

	self->layoutPanel->setPanelIndexIn(self->panelIndex.getValue());
}

//-----------------------------------------------------------------------------------------------------

void SoPanelGroup::panelIndexInChangedCB( void * data, SoSensor * sensor )
{	
	SoPanelGroup *self = (SoPanelGroup*) data;
	assert(self);

	self->panelIndex.setValue(self->panelIndexIn.getValue());
	 
}

//-----------------------------------------------------------------------------------------------------

void  SoPanelGroup::doStyleAction(SoAction *action)
{
	SoNodeKitListPart *nodeList = SO_GET_PART(layoutPanel, "elements", SoNodeKitListPart);
	assert( nodeList != NULL );
	for (int i=0 ; i< nodeList->getNumChildren() ; i++)
		action->traverse(nodeList->getChild(i));

	action->traverse(titles.getValue());
}