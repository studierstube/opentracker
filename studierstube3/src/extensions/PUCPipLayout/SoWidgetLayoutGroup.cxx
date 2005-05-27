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
/** Implementation of class SoWidgetLayoutGroup
  *
  * @author Valerie Maquil
  *
  * $Id: SoWidgetLayoutGroup.cxx 4140 2005-03-22 19:16:43Z kaufmann $
  * @file                                                                   */
 /* ======================================================================= */


#include "SoWidgetLayoutGroup.h"
#include "SoPanel.h"

#if defined(_DEBUG_LAYOUTGROUP) && defined(DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif


SO_KIT_SOURCE(SoWidgetLayoutGroup);

//-----------------------------------------------------------------------------------------------------

void SoWidgetLayoutGroup::initClass()
{
    SO_KIT_INIT_CLASS(SoWidgetLayoutGroup,  SoLayoutKit, "SoLayoutKit");
}

//-----------------------------------------------------------------------------------------------------

SoWidgetLayoutGroup::SoWidgetLayoutGroup()
{
    SO_KIT_CONSTRUCTOR(SoWidgetLayoutGroup);


	//--------------------------------------+------------+------+---------------+-------+---------------
	//                                      |            |      |               | right |
	//                                      |            | null |               | Sib-  | is
	//                                      |            | By   |               | ling  | public
	//                      name            | className  | Def. |  parent Name  | Name  | part
	//--------------------------------------+------------+------+---------------+-------+---------------
    SO_KIT_ADD_CATALOG_ENTRY(hiddenGeometry, SoAbortGroup,   TRUE, this,           \x0, TRUE );
 
	
	SO_KIT_ADD_CATALOG_LIST_ENTRY(elements,		SoGroup,	FALSE, hiddenGeometry, \x0, SoLayoutKit,  TRUE);
	SO_KIT_ADD_CATALOG_LIST_ENTRY(pipLayout, SoSeparator,	FALSE, this,		   \x0, SoSeparator, TRUE);

	SO_KIT_INIT_INSTANCE();	
  
    SO_KIT_ADD_FIELD( numOfRows, (-1));
    SO_KIT_ADD_FIELD( numOfCols, (-1));
    SO_KIT_ADD_FIELD( spacingWidth, (0.20)); // 20%
    SO_KIT_ADD_FIELD( spacingDepth, (0.20)); // 20%
	SO_KIT_ADD_FIELD( sizeOfRows, (1));
	SO_KIT_ADD_FIELD( sizeOfCols, (1));
	SO_KIT_ADD_FIELD( layoutUpdated, ());

	SoAbortGroup *abortGrp = (SoAbortGroup *)readNodeFromMemory("SoAbortGroup { abortActions (RENDER | BBOX | EVENT3D) }");
		assert( abortGrp != NULL );
		assert( setPart("hiddenGeometry", abortGrp ) );

	setUpConnections(TRUE, TRUE);

}

//-----------------------------------------------------------------------------------------------------

	
SbBool SoWidgetLayoutGroup::setUpConnections(SbBool onOff, SbBool doItAlways)
{
		// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoLayoutKit::setUpConnections(onOff, doItAlways);
        // setup the widget
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
        SoLayoutKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
}


//-----------------------------------------------------------------------------------------------------

SoWidgetLayoutGroup::~SoWidgetLayoutGroup()
{

}

//-----------------------------------------------------------------------------------------------------

void SoWidgetLayoutGroup::resizeCB()
{	

	buildPipLayout();  
	
}

//-----------------------------------------------------------------------------------------------------

void SoWidgetLayoutGroup::buildPipLayout()
{
	SoNodeKitListPart *elementList = (SoNodeKitListPart *)elements.getValue();
	
	int numChildren = elementList->getNumChildren();

	//Adapt number of rows and columns
	int numCols = numOfCols.getValue();
	int numRows = numOfRows.getValue();

	if ((numCols < 0) && (numRows < 0)){
		numCols = ceil(sqrt(double(numChildren)));
		numRows = ceil(double(numChildren)/double(numCols));
	DEBUG_CODE(std::cout << "SoWidgetLayoutGroup::numCols" << numCols << std::endl;);
	DEBUG_CODE(std::cout << "SoWidgetLayoutGroup::numRows" << numRows << std::endl;);

	}
	else if (numCols < 0) { 
		numCols = ceil(double(numChildren)/double(numRows));
	}
	else if (numRows < 0) {
		numRows = ceil(double(numChildren)/double(numCols)); 
	}
	else if (numChildren > numCols + numRows) {
		numRows = ceil(double(numChildren)/double(numCols));
	}


	//Absolute spaces between widgets
	float absSpacingWidth = spacingWidth.getValue()*width.getValue()/(float)numCols;
	float absSpacingDepth = spacingDepth.getValue()*depth.getValue()/(float)numRows;


	//Get number of units in rows and columns
	int numColUnits = 0;
	int numRowUnits = 0;

	for (int n=0; n<numCols; n++)
	{
		
		if (n<sizeOfCols.getNum()) numColUnits += sizeOfCols[n];//if size of column was given
		else {
			numColUnits += 1; //default = 1
			sizeOfCols.set1Value(n,1); //fill it up with 1's
		}
	}

	for (int m=0; m<numRows; m++)
	{
		
		if (m<sizeOfRows.getNum()) numRowUnits += sizeOfRows[m];//if size of column was given
		else {
			numRowUnits += 1; //default = 1
			sizeOfRows.set1Value(m,1); //fill it up with 1's
		}
	}


	
	
	SoNodeKitListPart *nodeList = SO_GET_PART(this, "pipLayout", SoNodeKitListPart);
	assert( nodeList != NULL );
	while (nodeList->getNumChildren() !=0)
		nodeList->removeChild(0);


	int i=0;
	
	float newYOffset = 0;

	for (int j= 0; j< numRows; j++)
	{
		int rowSize = sizeOfRows[j]; //size of this row
		float newDepth = (depth.getValue()-((float)numRowUnits-1)*absSpacingDepth)/(float)numRowUnits; //new depth of one unit
		newDepth = newDepth*rowSize + (rowSize-1)*absSpacingDepth; //new depth for rowSize units
		float yInterval = newDepth + absSpacingDepth; //horizontal distance between two centers
		float yCorrection =	(newDepth/2)-(depth.getValue()/2); //horizontal translation to the center
		float newXOffset = 0;
		newYOffset += yCorrection;

		for (int k=0; k<numCols; k++)
		{
			int colSize = sizeOfCols[k]; //size of this column
			float newWidth = (width.getValue()-((float)numColUnits-1)*absSpacingWidth)/(float)numColUnits; //new width one unit
			newWidth = newWidth*colSize + (colSize-1)*absSpacingWidth; //new width for colSize units

			float xInterval = newWidth + absSpacingWidth; //vertical distance between two centers
			float xCorrection = (newWidth/2)-(width.getValue()/2); //vertical translation to the center

			if (i<numChildren)
			{	
					
			//Update sizes
			SoLayoutKit *widget = (SoLayoutKit *)elementList->getChild(i);
			widget ->width.setValue(newWidth);
			widget ->depth.setValue(newDepth);	
			widget ->height.setValue(height.getValue());

			if (widget->isOfType(SoWidgetLayoutGroup::getClassTypeId()) )
			{
				if (widget->getTypeId() == SoPanel::getClassTypeId() )
				{
					((SoPanel *)widget )->buildPipLayout();
				}
				else if (((SoWidgetLayoutGroup *)widget)->elements.getValue() != NULL)
				((SoWidgetLayoutGroup *)widget )->buildPipLayout();
			}
			//Translate Offset to the centre of the widget
			newXOffset += xCorrection;
				

			//create new separator with all widgets of this WidgetLayoutGroup and transforms
			SoSeparator *pipLayoutElement = new SoSeparator;
			SoTransform *elementTransform = new SoTransform;
			elementTransform->translation.setValue(newXOffset, 0, newYOffset);
			pipLayoutElement->addChild(elementTransform);
			pipLayoutElement->addChild(widget);


			nodeList->addChild(pipLayoutElement);
			
			//Update Offset for the next widget
			newXOffset += xInterval - xCorrection;

			}

			i++;
		}

		newYOffset += yInterval - yCorrection;
	}

	layoutUpdated.setValue();
}

//-----------------------------------------------------------------------------------------------------

void  SoWidgetLayoutGroup::doStyleAction(SoAction *action)
{
	SoNodeKitListPart *nodeList = SO_GET_PART(this, "pipLayout", SoNodeKitListPart);
	assert( nodeList != NULL );
	for (int i=0 ; i< nodeList->getNumChildren() ; i++)
		action->traverse(nodeList->getChild(i));

}

//-----------------------------------------------------------------------------------------------------