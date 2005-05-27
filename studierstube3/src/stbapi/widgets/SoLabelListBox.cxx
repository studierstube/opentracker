/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** implementation of AbstractToogleButtonBox widget
  *
  * @author  Thomas Psik
  *
  * $Id: SoLabelListBox.cxx 4139 2005-03-22 09:59:04Z kaufmann $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoMaterial.h>

#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/widgets/SoToggleButton.h>
#include <stbapi/widgets/SoIncrementalSlider.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/SoAbortGroup.h>

#include <stbapi/widgets/SoLabelListBox.h>
/*
#include <stbapi/styles/SoButtonStyle.h>
*/


#define NO_WARNINGS
//#define _DEBUG_LABEL_LISTBOX

#if defined(_DEBUG_LABEL_LISTBOX) && defined(_DEBUG)
#undef NO_WARNINGS
#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif


#if defined(NO_WARNINGS)
#define WARNING_CODE(a) ;
#else
#define WARNING_CODE(a) a;
#endif


SO_KIT_SOURCE(SoLabelListBox);

void SoLabelListBox::initClass()
{
    SO_KIT_INIT_CLASS(SoLabelListBox,  SoBehaviorKit, "SoBehaviorKit");
}

SoLabelListBox::SoLabelListBox()
{
    SO_KIT_CONSTRUCTOR(SoLabelListBox);

	// ??? ask gerhard ???
	//    isBuiltIn = TRUE;

	//--------------------------------------+------------+------+---------------+-------+---------------
	//                                      |            |      |               | right |
	//                                      |            | null |               | Sib-  | is
	//                                      |            | By   |               | ling  | public
	//                      name            | className  | Def. |  parent Name  | Name  | part
	//--------------------------------------+------------+------+---------------+-------+---------------
    SO_KIT_ADD_CATALOG_ENTRY(hiddenGeometry, SoAbortGroup,   FALSE, this,           \x0, TRUE );
 
	SO_KIT_ADD_CATALOG_ENTRY(backplaneSep,		SoSeparator, FALSE, this,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(backplaneTransform,SoTransform, FALSE, backplaneSep,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(backplaneScale,	SoScale,	 FALSE, backplaneSep,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(backplane,		    SoSeparator, FALSE, backplaneSep,	\x0, TRUE);

	SO_KIT_ADD_CATALOG_ENTRY(navseparator,	    SoSeparator,         FALSE,  this,              \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(navHiddenGeometry, SoAbortGroup,        FALSE,  navseparator,      \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(navTransform,      SoTransform,         FALSE,  navHiddenGeometry, \x0, TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(slider,		    SoIncrementalSlider, FALSE,  navHiddenGeometry, \x0, TRUE );
		
	SO_KIT_ADD_CATALOG_LIST_ENTRY(labels,		 SoGroup,	FALSE, hiddenGeometry, \x0, SoLabelKit,  TRUE);
	SO_KIT_ADD_CATALOG_LIST_ENTRY(buttons,		 SoGroup,	FALSE, this,		   \x0, SoSeparator, TRUE);

	SO_KIT_INIT_INSTANCE();	
  
    SO_KIT_ADD_FIELD( numOfRows, (5));
    SO_KIT_ADD_FIELD( numOfCols, (1));
    SO_KIT_ADD_FIELD( navigationSize, (0.2)); // 20%
    SO_KIT_ADD_FIELD( spacingWidth, (0.05)); // 5%
    SO_KIT_ADD_FIELD( spacingDepth, (0.05)); // 5%
	SO_KIT_ADD_FIELD( startEndSpacingWidth, (.05)); // 5%
	SO_KIT_ADD_FIELD( startEndSpacingDepth, (.05)); // 5%
    SO_KIT_ADD_FIELD( displayIndex, (0));
	SO_KIT_ADD_FIELD( displayIndexIn, (0));
	SO_KIT_ADD_FIELD( selectedIndices, (-1));
	SO_KIT_ADD_FIELD( selectedIndicesIn, (0));

    SO_KIT_ADD_FIELD( multipleSelections, (FALSE));
	SO_KIT_ADD_FIELD( noneSelectionAllowed, (FALSE));

	selectedIndices.setNum(0);
	selectedIndicesIn.setNum(0);

	 SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, NONE);
     SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, TOP);
     SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, LEFT);
     SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, RIGHT);
     SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, BOTTOM);
     SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, INLINE_ROW);
     SO_NODE_DEFINE_ENUM_VALUE(NavigationPlacing, INLINE_COL);
	 
     SO_NODE_SET_SF_ENUM_TYPE(navigationPlacing, NavigationPlacing);

	 SO_KIT_ADD_FIELD( navigationPlacing, (LEFT));

	 SO_NODE_DEFINE_ENUM_VALUE(NavigationDisplay, _NONE);
     SO_NODE_DEFINE_ENUM_VALUE(NavigationDisplay, ALWAYS);
     SO_NODE_DEFINE_ENUM_VALUE(NavigationDisplay, AUTO);
	 
     SO_NODE_SET_SF_ENUM_TYPE(navigationDisplay, NavigationDisplay);

	 SO_KIT_ADD_FIELD( navigationDisplay, (NONE));

	 SO_KIT_ADD_FIELD( navigationButtonToSliderRatio, (0.2));

	//  SO_KIT_ADD_FIELD( selectedIndices, (-1));
  
	 SO_KIT_ADD_FIELD( interval, (0.5));    
  
	
	SoAbortGroup *abortGrp = (SoAbortGroup*)hiddenGeometry.getValue(); //SO_GET_PART(this,"hiddenGeometry", SoAbortGroup);
	assert( abortGrp != NULL );
	abortGrp->abortActions = SoAbortGroup::RENDER | SoAbortGroup::BBOX |  SoAbortGroup::EVENT3D;

	setPart("backplane", readMemory(defaultBackPlane));

/*
	for (int i =0 ; i< 50; i++) 
	{
		pressedSensor[i].setData(this);
		pressedSensor[i].setFunction( SoLabelListBox::buttonChanged );
	}
*/

	displayIndexSensor.setData(this);
	displayIndexSensor.setFunction(SoLabelListBox::displayIndexChangedCB );

	displayIndexInSensor.setData(this);
	displayIndexInSensor.setFunction(SoLabelListBox::displayIndexInChangedCB );
	
	selectedIndicesInSensor.setData(this);
	selectedIndicesInSensor.setFunction(SoLabelListBox::selectedIndicesInChangedCB );


	spacingSensor.setData(this);
	spacingSensor.setFunction(SoLabelListBox::configChangedCB);
	
	// style related info
	buttonStyles = NULL;
	currentStyleIdx = -1;
	currentStyleButton = NULL;

	// setup field connections
    setUpConnections(TRUE, TRUE);
}

SbBool SoLabelListBox::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBehaviorKit::setUpConnections(onOff, doItAlways);
		// never traverse the children of the geometry switch !
		//((SoSwitch *)geometrySwitch.getValue())->whichChild.setValue( -1 );
        // setup the widget
		//((SoScale*)backplaneScale.getValue())->scaleFactor.connectFrom(& ((SoScale*)scale.getValue())->scaleFactor);
		SO_GET_PART(this, "backplaneScale", SoScale)->scaleFactor.connectFrom(	&SO_GET_PART(this, "scale", SoScale)->scaleFactor);

		SoIncrementalSlider* slid = SO_GET_PART(this, "slider", SoIncrementalSlider);
		assert (slid != NULL) ;
		slid->increment.setValue(1);
		slid->setUpConnections(onOff, doItAlways);
	
		slid->cropValueToIncrement.setValue(TRUE);
		slid->valueIn.setValue(displayIndex.getValue());
		displayIndex.connectFrom(&slid->value);
	
        // setup listeners
   
		spacingSensor.attach( &spacingWidth );
	    displayIndexSensor.attach( &displayIndex );
	    displayIndexInSensor.attach( &displayIndexIn );
		selectedIndicesInSensor.attach (&selectedIndicesIn);
				
		if (noneSelectionAllowed.getValue() == FALSE)
		{
			// if no selectedIndicesIn is set then initialise with first index
			if (selectedIndicesIn.getNum() == 0)
				states.set1Value(0, TRUE);
		}
		
		selectedIndicesInSensor.schedule();

		spacingSensor.schedule();

		displayIndexInSensor.schedule();
		

		}
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
        navButtonSensor.detach();
        spacingSensor.detach();
        numButtonsSensor.detach();
		displayIndexSensor.detach();
		selectedIndicesInSensor.detach ();

		for (int i= 0 ; i<focusSensors.getLength(); i++)
		{
			((SoFieldSensor*)focusSensors[i])->detach();
			delete (SoFieldSensor*)focusSensors[i];
		}

        SoBehaviorKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);

}

void SoLabelListBox::resizeWidgetCB()
{
	DEBUG_CODE(printf ("SoLabelListBox::resizeWidgetCB \n"););
	adaptToConfig();
}


// this method will relayout the AbstractToogleButtonBox and recreate the buttons
void SoLabelListBox::adaptToConfig(void)
{
	SoBehaviorKit::resizeWidgetCB();
	
	// set up geometry for sub widgets
	// setup geometry parts
	float widthV = widgetDim[0];// width.getValue();
	float depthV = widgetDim[2];// depth.getValue();
	float heightV = widgetDim[1];// height.getValue();
	
	DEBUG_CODE(printf ("SoLabelListBox::adaptToConfig %f %f %f \n",widthV, depthV, heightV););


	// setup layout of widgets
	
	int numRows = numOfRows.getValue();
	int numCols = numOfCols.getValue();
	assert (numRows > 0);
	assert (numCols > 0);

	float navWidth = 0.0f;
	float navDepth = 0.0f;
	float navWidthCorrection = 0.0f;
	float navDepthCorrection = 0.0f;
	
	float widthV_2 = widthV/2.0f; 
	float depthV_2 = depthV/2.0f;

	float navSizeV = navigationSize.getValue();
	int navPlacing = navigationPlacing.getValue();

	switch (navPlacing) 
	{
		case INLINE_COL:
			navDepthCorrection = depthV * navSizeV*2.0;
			navWidth = depthV;
			navDepth = widthV;			
			break;
		case INLINE_ROW:
			navWidthCorrection = widthV * navSizeV*2.0;
			navWidth = widthV;
			navDepth = depthV;			
			break;
		case TOP:
		case BOTTOM:
			navDepthCorrection = depthV * navSizeV;
			navDepth = navDepthCorrection;
			navWidth = widthV;			
			break;
		case LEFT:
		case RIGHT:
			navWidthCorrection = widthV * navSizeV;
			navDepth = navWidthCorrection;
			navWidth = depthV;
			break;
	}
	float navMoveWidth = 0.0f;
	float navMoveDepth = 0.0f;
	float rotAngle = 0.0f;

	switch (navPlacing) 
	{
		case TOP:
		//	navMoveWidth = 0.0f;
			navMoveDepth = -depthV_2 + navDepth /2.0;
			break;
		case LEFT:
			// width and depth are swapped !!
			navMoveWidth = -widthV_2 + navDepth /2.0;
			rotAngle = -1.57f;
		
		//	navMoveDepth = 0.0f;
		case INLINE_COL:
			rotAngle = -1.57f;
			break;
	}


	SoTransform *backplaneTransform = SO_GET_PART( this, "backplaneTransform", SoTransform );

	assert( backplaneTransform!= NULL);
		backplaneTransform->translation.setValue(widgetMove[0], widgetMove[1], widgetMove[2]);
		// move the navigation widgets to the right position
	// 0,0 will then be the center of the up button
	SoTransform *transform = SO_GET_PART( this, "navTransform", SoTransform );
	assert( transform!= NULL);
	transform->translation.setValue(widgetMove[0]+navMoveWidth, widgetMove[1]+0, widgetMove[2]+navMoveDepth);
	transform->rotation.setValue(SbVec3f(0,1,0), rotAngle);

	// insert navigation slider 
	SoIncrementalSlider *slid = SO_GET_PART( this, "slider", SoIncrementalSlider );
	assert(slid != NULL);
	slid->width.setValue( navWidth );
	slid->depth.setValue( navDepth );
	slid->height.setValue( height.getValue());
//	slid->setUpConnections(TRUE,TRUE);

	if (navPlacing == INLINE_COL || navPlacing == INLINE_ROW)
		slid->buttonSpace.setValue(navigationSize.getValue());
	else
	{
		slid->buttonSpace.setValue(navigationButtonToSliderRatio.getValue() );
		if (navigationButtonToSliderRatio.getValue() > .4) 
			slid->hideSlider.setValue(TRUE);
		else if (navigationButtonToSliderRatio.getValue() < .1)
			slid->hideButtons.setValue(TRUE);
	}

	if (navPlacing == INLINE_COL || navPlacing == INLINE_ROW)
	{
		slid->hideSlider.setValue(TRUE);
	}

	// remove all sensors
	for (int i= 0 ; i<pressedSensors.getLength(); i++)
	{
		((SoFieldSensor*)pressedSensors[i])->detach();
		delete (SoFieldSensor*)pressedSensors[i];
	}

	pressedSensors.truncate(0);

	for (int j= 0 ; j<focusSensors.getLength(); j++)
	{
		((SoFieldSensor*)focusSensors[j])->detach();
		delete (SoFieldSensor*)focusSensors[j];
	}

	focusSensors.truncate(0);

	SoFieldSensor *sliderInsideSensor = new SoFieldSensor();
	sliderInsideSensor->setData(this);
	sliderInsideSensor->setFunction(insideChangedCB);
	sliderInsideSensor->attach(& (slid->inside));
	focusSensors.append(sliderInsideSensor);

	
	// clear out buttons
    SoNodeKitListPart * list = (SoNodeKitListPart*)buttons.getValue();
    assert(list != NULL);
	while( list->getNumChildren() != 0 )
        list->removeChild( 0 );
	
	updateNavigationDisplay();

	SoAbortGroup *abortGrp = (SoAbortGroup*)navHiddenGeometry.getValue(); //SO_GET_PART(this,"hiddenGeometry", SoAbortGroup);
	assert( abortGrp != NULL );
	
	// buttonStartPos at the left / top side
	float buttonStartPosW = -widthV_2;
	float buttonStartPosD = -depthV_2;
	bool navHidden = (abortGrp->abortActions.getValue() != SoAbortGroup::NONE);
	// slider hidden ?? 
	if (navHidden) 
	{
		// use all space for buttons
		navWidthCorrection = 0;
		navDepthCorrection = 0;

	}
	else
	{
		
		// buttonStartPos adjust with navigation size
		if (navPlacing == TOP) 
		{
			buttonStartPosD += navDepthCorrection;
		} 
		else if (navPlacing == LEFT) 
		{
			buttonStartPosW += navWidthCorrection;
		}
		else if (navPlacing == INLINE_ROW) 
		{
			buttonStartPosW += navWidthCorrection /2.0;
		}
		else if (navPlacing == INLINE_COL) 
		{
			buttonStartPosD += navDepthCorrection /2.0;
		}
	}


	// space for buttons
	float buttonW = ( widthV - navWidthCorrection);
    float buttonD = ( depthV - navDepthCorrection);
	
	float spacingWV = spacingWidth.getValue() * buttonW;
	float spacingDV = spacingDepth.getValue() * buttonD;

	float startEndSpacingWV = startEndSpacingWidth.getValue() * buttonW;
	float startEndSpacingDV = startEndSpacingDepth.getValue() * buttonD;

	// slider hidden ?? 
	if (navHidden) 
	{
		// use all space for buttons
		if (navPlacing == INLINE_ROW || navPlacing == RIGHT || navPlacing == LEFT ) 
			startEndSpacingWV = 0.0f;
		else if (navPlacing == INLINE_COL || navPlacing == TOP || navPlacing == BOTTOM) 
			startEndSpacingDV = 0.0f;
	}

	// button size
//	float buttonWidth = buttonW + ((1-numCols) * buttonW*spacingWV) / numCols;
//  float buttonDepth = buttonD + ((1-numRows) * buttonD*spacingDV) / numRows;
	float buttonWidth = (buttonW - (numCols-1)*spacingWV - startEndSpacingWV*2.0) / numCols;
	float buttonDepth = (buttonD - (numRows-1)*spacingDV - startEndSpacingDV*2.0) / numRows;

	// buttonStartPos add half a spacing
	buttonStartPosW += startEndSpacingWV;
	buttonStartPosD += startEndSpacingDV;
	
	// buttonStartPos finally add half a button size
	buttonStartPosW += buttonWidth /2.0;
	buttonStartPosD += buttonDepth /2.0;

	// TODO correct start pos
	if (navPlacing!= TOP && navPlacing!= BOTTOM) 
	{
	}
	
	// rebuild buttons
    for( int c = 0; c < numCols; c++ )
    {
	    for( int r = 0; r < numRows; r++ )
		{
			int index = numRows*c+r;
			// clone Button
			SoToggleButton * toggle = new SoToggleButton();
		//	toggle->ref();
			toggle->width.setValue( buttonWidth );
			toggle->depth.setValue( buttonDepth );
			toggle->height.setValue( height.getValue());
			//toggle->setUpConnections(TRUE, TRUE);
			
			SbString name("_AbstractToogleButtonBox_");
			name += SbString(index );
			toggle->setName( name );
			
			//pressedSensor[index].attach(& (toggle->on));

			// attach sensor to on field to trigger an update.		
			SoFieldSensor *triggerSensor = new SoFieldSensor();
			triggerSensor->setData(this);
			triggerSensor->setFunction( SoLabelListBox::buttonChanged );
			triggerSensor->attach(& (toggle->on));
			pressedSensors.append(triggerSensor);

			//attach sensor to inside fields to propagate the status to the list box
			SoFieldSensor *insideSensor = new SoFieldSensor();
			insideSensor->setData(this);
			insideSensor->setFunction(SoLabelListBox::insideChangedCB);
			insideSensor->attach(& (toggle->inside));
			focusSensors.append(insideSensor);
	
			float butPosW = buttonStartPosW + c * (buttonWidth+spacingWV);
			float butPosD = buttonStartPosD + r * (buttonDepth+spacingDV);

			SoTransform * transform = new SoTransform;
			transform->translation.setValue(widgetMove[0]+butPosW , widgetMove[1]+0, widgetMove[2]+butPosD);
			SoSeparator * separator = new SoSeparator;
			//separator->ref();
			separator->addChild( transform );
			separator->addChild( toggle );
			//separator->touch();
			list->addChild( separator );
			applyStyleToButton(toggle, index);

		//	separator->unref();
		//	toggle->unref();
		}
    }                                                    


}

// update navigation display
void SoLabelListBox::updateNavigationDisplay()
{
	// check if the nagivation slider can be hidden and adapt layout		
	SoAbortGroup *abortGrp = (SoAbortGroup*)navHiddenGeometry.getValue(); //SO_GET_PART(this,"hiddenGeometry", SoAbortGroup);
	assert( abortGrp != NULL );

	int oldDisplay = abortGrp->abortActions.getValue();
	bool skipDisplay = false;

	if (navigationDisplay.getValue() != ALWAYS)
	{
		SoNodeKitListPart * labelList = (SoNodeKitListPart *)labels.getValue();
		int numentries =  labelList->getNumChildren();
		int numRows = numOfRows.getValue();
		int numCols = numOfCols.getValue();
		assert (numRows > 0);
		assert (numCols > 0);

		if (numRows*numCols >= numentries)
		{
			// insert navigation slider 
			SoIncrementalSlider *slid = SO_GET_PART( this, "slider", SoIncrementalSlider );
			assert(slid != NULL);
			abortGrp->abortActions = SoAbortGroup::RENDER | SoAbortGroup::BBOX |  SoAbortGroup::EVENT3D;
			skipDisplay = true;
		}
	}
	
	// display and layout navigation widgets
	if (!skipDisplay)
	{
		abortGrp->abortActions = SoAbortGroup::NONE;	
	}

	
	if (abortGrp->abortActions.getValue() != oldDisplay)
	{
		adaptToConfig();
	}
}



void SoLabelListBox::updateLabels()
{
	SoNodeKitListPart * labelList = (SoNodeKitListPart *)labels.getValue();
	int numentries =  labelList->getNumChildren();
	states.setNum(numentries);

	int numRows = numOfRows.getValue();
	int numCols = numOfCols.getValue();
	assert (numRows > 0);
	assert (numCols > 0);

	int numButtons = numCols * numRows;

	SoIncrementalSlider *slid = SO_GET_PART( this, "slider", SoIncrementalSlider );
	assert(slid != NULL);

	slid->minValue.setValue(0);
	int navPlacing = navigationPlacing.getValue();

	if (numentries - numButtons > 0)
	{
		slid->maxValue.setValue(numentries - numButtons);
		slid->enable.setValue(TRUE);
	}
	else
	{
		slid->enable.setValue(FALSE);
		slid->maxValue.setValue(0);
		displayIndex.setValue(0);
	}

	updateNavigationDisplay();

}


void SoLabelListBox::displayIndexInChangedCB( void * data, SoSensor * sensor )
{
	SoLabelListBox *self = (SoLabelListBox*) data;
	assert(self);
	// no connect from !!
	if(self->displayIndex.getValue() != self->displayIndexIn.getValue())
	{
		self->displayIndex.setValue(self->displayIndexIn.getValue());
		DEBUG_CODE(	printf("SoLabelListBox::displayIndexInChangedCB [%d]\n"););
	}
//	self->adaptToState();
}

void SoLabelListBox::displayIndexChangedCB(void *data, SoSensor * sensor )
{
	SoLabelListBox *self = (SoLabelListBox*) data;
	assert(self);
	// optional !! -> displayIndex && displayIndexIn out of sync
	//	self->displayIndexIn.setValue(self->displayIndex.getValue())
	DEBUG_CODE(printf("SoLabelListBox::displayIndexChanged to [%d]\n",self->displayIndex.getValue()););
	self->adaptToState();
} 

// this method will set the button state to the current AbstractToogleButtonBox state
void SoLabelListBox::adaptToState(void)
{
    int i;
	
	int numButtons = numOfCols.getValue() * numOfRows.getValue();
    int displIdx = displayIndex.getValue();
	if (displIdx < 0) 
	{
		WARNING_CODE (printf("warning SoLabelListBox::adaptToState() displayIndex is %d (<0)\n",displIdx););
		return;
	}
	for( i = 0; i < numButtons; i++ )
    {
		int idx = i + displIdx;
        setButton( i, idx );
    }

/*	selectedIndices.setNum(0);
	int count = 0;
	for (i = 0; i< states.getNum(); i++) 
	{
		if (states[i] == TRUE)
		{
			selectedIndices.set1Value(count, i);
			count++;
		}
	}
*/	

}

void SoLabelListBox::setButton( int buttonIndex, int index)
{
    // name we look for
	SbString name("_AbstractToogleButtonBox_");
	name += SbString(buttonIndex);        

	SoNodeKitListPart * labelList = (SoNodeKitListPart *)labels.getValue();
	int numentries =  labelList->getNumChildren();
	SoLabelKit *labelKit = NULL;
	SbBool state = FALSE;

	if ( index < numentries)
	{
		labelKit = (SoLabelKit *)labelList->getChild(index);
		state = states[index];
	}

    // search for button with name = buttonIndex, probably faster then search action
	SoNodeKitListPart *list = (SoNodeKitListPart *)buttons.getValue();
	SoToggleButton * button = NULL;
	int i;
	for( i = 0; i < list->getNumChildren() && button == NULL; i++ )
	{
		SoSeparator * sep = (SoSeparator *)list->getChild(i);
		if (sep->getNumChildren() > 1) 
		{
			SoNode * candidate = sep->getChild(1);
			assert( candidate != NULL );
			if( candidate->getName() == name.getString() && candidate->isOfType( SoToggleButton::getClassTypeId()))
				button = (SoToggleButton *)candidate;
		}
	}

	assert( button != NULL );

	button->setPart("label", labelKit);
	
	//TODO find out how to re-apply the styles ???
	if (index < numentries)
		applyStyleToButton(button, index);

	if (labelKit == NULL) 
	{
		button->enable.setValue(FALSE);
		state = FALSE;
	}
	else 
	{
		button->enable.setValue(TRUE);
	}

	int buttonId = getButtonId(button);
	SoFieldSensor* sensor = (SoFieldSensor*)pressedSensors[buttonId];
	assert(sensor != NULL);
	sensor->detach();

	//SoFieldSensor* fsensor = (SoFieldSensor*)focusSensors[buttonId];
	//assert(fsensor != NULL);
	//fsensor->detach();
//	pressedSensor[buttonId ].detach();
	button->on.setValue( state );
	sensor->attach(&button->on);
	//fsensor->attach(&button->inside);
	//	pressedSensor[buttonId ].attach(&button->on);
	button->adaptGeometryToState();

}


void SoLabelListBox::applyStyleToButton(SoToggleButton *button, int index)
{
	DEBUG_CODE(printf("SoLabelListBox::applyStyleToButton button [%p] index [%d] styles [%p]\n",button, index, buttonStyles ));
	if (buttonStyles == NULL) return;
	if (button == NULL) return;

	SoType styleActionType = SoType::fromName("SoReApplyWidgetStyleAction"); 
	if (styleActionType.isBad())  return;
	DEBUG_CODE(printf("found SoReApplyWidgetStyleAction. Appling to buttons\n"));
	
	if (!styleActionType.canCreateInstance())
	{
		DEBUG_CODE(printf("found SoReApplyWidgetStyleAction. can not create instance. abort.\n"));
		return;
	}
	
	SoAction *styleAction = (SoAction*) (styleActionType.createInstance());
	if (styleAction  == NULL)
	{
		DEBUG_CODE(printf("found SoReApplyWidgetStyleAction. can not create instance. abort.\n"));
		return;
	}
	
	currentStyleIdx = index;
	currentStyleButton = button;

	styleAction->apply(this);
	
	currentStyleButton = NULL;
	currentStyleIdx = -1;

	delete styleAction;
}

int SoLabelListBox::getButtonId(SoToggleButton *button) 
{
	SbName name = button->getName();
	assert((unsigned int)name.getLength() > strlen("_AbstractToogleButtonBox_"));

	int id;
	SbString nameStr = SbString(name).getSubString(strlen("_AbstractToogleButtonBox_"));
	
	if (sscanf(nameStr.getString(),"%d",&id) == 1) 
	{
		return id;
	}
	return -1;
}

SoLabelListBox::~SoLabelListBox()
{

}

void SoLabelListBox::selectedIndicesInChangedCB(void * data, SoSensor * sensor  )
{
	assert( data != NULL );
    SoLabelListBox * self = (SoLabelListBox *)data;  
	self->adaptToSelectedIndices();
}

void SoLabelListBox::adaptToSelectedIndices()
{
	bool changed = FALSE;

	if (multipleSelections.getValue() == FALSE)
	{
		int idx = -1;
		if (selectedIndicesIn.getNum() > 0)
		{
			idx = selectedIndicesIn[0];
		}

		if (idx < 0 || idx > states.getNum()) {
			WARNING_CODE(printf("SoLabelListBox::adaptToSelectedIndixes error index < 0 || index[%d] > numberOfElements[%d]\n", idx, states.getNum()););
			return;
		}
		
		DEBUG_CODE(	printf("SoLabelListBox::adaptToSelectedIndices idx[%d] is [%d]\n", idx, states[idx]););

		if (states[idx] != TRUE) {

			for (int i=0; i< states.getNum(); i++)
				states.set1Value(i, FALSE);
			states.set1Value(idx, TRUE);
			changed = TRUE;
		}
	}
	else
	{
		int i;
		for (i=0; i< states.getNum(); i++)
			states.set1Value(i, FALSE);
		for (i=0; i< selectedIndicesIn.getNum(); i++)
		{
			int idx = selectedIndicesIn[i];

			if (idx < 0 || idx > states.getNum()) {
				WARNING_CODE(printf("SoLabelListBox::adaptToSelectedIndixes error index < 0 || index[%d] > numberOfElements[%d]\n", idx, states.getNum()););
				return;
			}
			states.set1Value(idx, TRUE);

		}
		changed = TRUE;
	}

	// TODO 
	if (noneSelectionAllowed.getValue() == TRUE) 
	{
	
	}

	if (changed) 
	{

		// copy values 
		selectedIndices.copyFrom(selectedIndicesIn);
		displayIndexIn.setValue(0);
		adaptToState();
	}
}


void SoLabelListBox::valuesChanged( void * data, SoSensor * sensor  )
{
/*
	assert( data != NULL );
    SoLabelListBox * self = (SoLabelListBox *)data;    
    self->displayIndex.setValue( 0 );

	self->selectedSensor.detach();
    self->selected.deleteValues(0, -1);
	self->selectedSensor.attach( &self->selected );

	self->selectedIndexSensor.detach();
    self->selectedIndices.deleteValues(0, -1);    
	self->selectedIndexSensor.attach( &self->selectedIndices );
    
	self->adaptToState();
*/
}

void SoLabelListBox::configChangedCB( void * data, SoSensor * sensor  )
{
	assert( data != NULL );
    ((SoLabelListBox *)data)->adaptToConfig();
    ((SoLabelListBox *)data)->updateLabels();
    ((SoLabelListBox *)data)->adaptToState();

}

void SoLabelListBox::buttonChanged( void * data, SoSensor * sensor  )
{
    SoLabelListBox * self = (SoLabelListBox *)data;
	assert( self != NULL );
	self->updateFromButtonStates(sensor);
}

void SoLabelListBox::updateFromButtonStates(SoSensor * sensor)
{
 // search for button with name = index, probably faster then search action
	SoNodeKitListPart *buttonList = (SoNodeKitListPart *)buttons.getValue();
	SoToggleButton * button = NULL;

	int idPressed = -1;
	int i;
	
	idPressed = pressedSensors.find(sensor);
	assert(idPressed >= 0);

	
	int startIdx = displayIndex.getValue();
	int changedIdx = -1;
	bool singleSelection = (multipleSelections.getValue() == FALSE);
	
	for( i = 0; i < buttonList->getNumChildren(); i++ )
	{
		SoSeparator * sep = (SoSeparator *)buttonList->getChild(i);
		if (sep->getNumChildren() > 1) 
		{
			SoNode * candidate = sep->getChild(1);
			assert( candidate != NULL );
			if( candidate->isOfType( SoToggleButton::getClassTypeId()))
			{
				button = (SoToggleButton *)candidate;
				SbName name = button->getName();
				SbString nameStr = SbString(name).getSubString(strlen("_AbstractToogleButtonBox_"));
				
				int buttonId = getButtonId(button);

				if (buttonId > -1) 
				{
					int idx = startIdx+buttonId;
					if (singleSelection) 
					{
						SoFieldSensor* sensor = (SoFieldSensor*)pressedSensors[buttonId];
						assert(sensor != NULL);
						sensor->detach();
					//	pressedSensor[buttonId].detach();
						if (buttonId != idPressed) 
						{
							button->on.setValue(FALSE);
						} 
						else 
						{
							if (noneSelectionAllowed.getValue() == FALSE)
							{
								if (button->on.getValue() != TRUE)
									button->on.setValue(TRUE); // cannot turn of only selection !
							}
							// store information which index was pressed.
							changedIdx = idx; 
						}	
						sensor->attach(& (button->on));
						// pressedSensor[buttonId].attach(& (button->on));
					}
					
					SbBool state = button->on.getValue();
					states.set1Value(idx, state);
				    DEBUG_CODE(printf("button [%d] idx [%d] state [%d]\n",buttonId, idx, state););
				}
			}
		}
	}
	
	// in case there should be only one selected update states information
	if (singleSelection) 
	{
		for (i = 0; i< states.getNum(); i++) 
		{
			if (i != changedIdx) 
				states.set1Value(i, FALSE);
		}
	}

	selectedIndices.setNum(0);
	int count = 0;
	for (i = 0; i< states.getNum(); i++) 
	{
		if (states[i] == TRUE)
		{
			selectedIndices.set1Value(count, i);
			count++;
		}
	}
}
// ---------------------------------------------------------------

void SoLabelListBox::insideChangedCB(void *data, SoSensor *sensor)
{
	SoLabelListBox *self = (SoLabelListBox*) data;
	assert(self);

	SoFieldSensor *fSensor = (SoFieldSensor *)sensor;
	SoBehaviorKit *curWidget = (SoBehaviorKit *)fSensor->getAttachedField()->getContainer();

	self->inside.setValue(curWidget->inside.getValue());
}


// ---------------------------------------------------------------

const char *SoLabelListBox::defaultBackPlane = "SoSeparator { SoMaterial { diffuseColor .5 .5 .5} SoCube {width 1  height .05 depth 1} } ";
