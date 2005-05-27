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
/** class definition of SoLabelListBox
  *
  * @author  Thomas Psik
  *
  * $Id: SoLabelListBox.h 4139 2005-03-22 09:59:04Z kaufmann $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoLabelListBox_H_
#define _SoLabelListBox_H_

#include <Inventor/SbPList.h>

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFBool.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <stbapi/widgets/SoButtonKit.h>
#include <stbapi/widgets/SoToggleButton.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * This class provides a simple list box interface with a number of buttons 
 * showing different items and a SoIncrementalSlider allowing to navigate through the
 * possible items. When there are more items than buttons, the
 * slider can be used to navigate up and down through the list. When all the items can
 * be displayed on one page, the slider is disabled.
 *
 * Some influence can be taken on the number of buttons and general layout. The position
 * of the slider can be specified, the buttons may be placed in a certain
 * number of rows and columns and with a given spacing between them. 
 *
 * File format:
@verbatim
SoLabelListBox {
    SoSFInt32   selectedIndices      0
    SoMFInt32 	selectedIndicesIn    0
 
	
    SoSFInt32	numOfRows			 5
    SoSFInt32	numOfCols			 1
	SoSFFloat 	spacingWidth         0.05 // 5%
	SoSFFloat 	spacingDepth         0.05 // 5%
	SoSFFloat 	startEndSpacingWidth 0.05 // 5%
	SoSFFloat 	startEndSpacingDepth 0.05 // 5%

    SoSFEnum    navigationPlacing    LEFT
	SoSFFloat	navigationSize		 0.2		  // 20%
	SoSFFloat   navigationButtonToSliderRatio 0.2 # means that each button will take 20% of the space, leaving 60% for the slider. If the value is below .1 then
	                                              # the buttons will be hidden, if the value is bigger than .4 than the slider will be hidden.


	SoSFInt32 	displayIndex         0
	SoSFInt32	displayIndexIn		 0
    SoSFTime    interval             0.5

    SoSFBool	multipleSelections   FALSE
	SoSFBool    noneSelectionAllowes FALSE

  	# from SoBehaviorKit
  	SoSFFloat	width					1
	SoSFFloat	height					1
	SoSFFloat	depth					1

	SoSFFloat	hysteresisBoxScale		1.0
	SoSFBool	enable					TRUE

	}
@endverbatim

 *
 * @author Thomas Psik
 * @ingroup widgets
 * @todo optimize for DIV
 */


class STBAPI_API SoLabelListBox : public SoBehaviorKit 
{
     SO_KIT_HEADER(SoLabelListBox);
    SO_KIT_CATALOG_ENTRY_HEADER(hiddenGeometry);
	
	SO_KIT_CATALOG_ENTRY_HEADER(backplaneSep);
	SO_KIT_CATALOG_ENTRY_HEADER(backplaneTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(backplaneScale);
	SO_KIT_CATALOG_ENTRY_HEADER(backplane);

	SO_KIT_CATALOG_ENTRY_HEADER(navseparator);
	SO_KIT_CATALOG_ENTRY_HEADER(navHiddenGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(navTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(slider);

    SO_KIT_CATALOG_ENTRY_HEADER(labels);
    SO_KIT_CATALOG_ENTRY_HEADER(buttons);


public:
  	enum NavigationPlacing
	{
		NONE = 0,
		TOP = 1,
		LEFT = 2,
		RIGHT = 3,
		BOTTOM = 4,
		INLINE_ROW = 5,
		INLINE_COL = 6
	} ;

	enum NavigationDisplay
	{
		_NONE = 0,			///< NONE == AUTO
		ALWAYS = 1,			///< always show navigation slider
		AUTO = 2,			///< only show navigation slider if there are more labels than buttons (numOfRows*numOfCols)
	} ;


	/** Inventor class initialization */
    static void initClass();

	///contructor
    SoLabelListBox();
	
	///destructor
    virtual ~SoLabelListBox();

	///if TRUE multiple selections are allowed
    SoSFBool multipleSelections;

	///if TRUE no selection is allowed
	SoSFBool noneSelectionAllowed;

   
    /** defines the number of rows to use when layouting the buttons. 
	 * note that if navigationPlacing INLINE_ROW / INLINE_COL is specified 
	 * these 2 buttons will be added */
	SoSFInt32 numOfRows;

	/** defines the number of columns to use when layouting the buttons. 
	 * note that if navigationPlacing INLINE_ROW / INLINE_COL is specified these 2 buttons will be added */
    SoSFInt32 numOfCols;

    /** this field defines the index of the item appearing on the top most (first) 
     * button. It allows the user to check for up/down movements and to 
     * determine which entries are visible. (field for connections out)
     */
    SoSFInt32 displayIndex;
	
	/** this field defines the index of the item appearing on the top most (first) 
     * button. It allows the user to check for up/down movements and to 
     * determine which entries are visible. (field for connections in/out)
     */
	SoSFInt32 displayIndexIn;

    ///horizontal spacing between 2 elements (in percent [0;1])
    SoSFFloat spacingWidth;
	///vertical spacing between 2 elements (in percent [0;1])
    SoSFFloat spacingDepth;

	///margins at the left and the right side of the back plane(in percent [0;1])
	SoSFFloat startEndSpacingWidth;
	///margins at the top and the bottom of the back plane (in percent [0;1])
	SoSFFloat startEndSpacingDepth;

    ///percentage of space that is used by the navigation widgets
    SoSFFloat navigationSize;

	/**percentage of space that is used for up/down buttons, the rest is used for the slider. 
	(0.2 means that the buttons will take each 20% of the space) */
    SoSFFloat navigationButtonToSliderRatio;

	/**specifiy where the navigation widgets should be placed: NONE, TOP, LEFT, RIGHT, BOTTOM, 
	 * INLINE_ROW or INLINE_COLUMN (INLINE means that the first and last buttons will be the navigation 
	 * buttons, no slider will be displayed) */
    SoSFEnum navigationPlacing;

	/**specifiy whether the navigation widgets should be visible: NONE, ALWAYS, AUTO, 
	 * NONE (can be overridden by style) or AUTO means that the navigation widgets will only be displayed when there are
	 * more labels than buttons. ALWAYS will include the widgets always in the layout (though the widgets will be disabled if
	 * there are less / equal labels than buttons. */
    SoSFEnum navigationDisplay;

    /// interval between display movements when nav buttons are pressed, implemented in SoPushButton
    SoSFTime interval;

	/// states of the buttons
	SoMFBool states;

	/** this field contains the indices of the currently selected entries, 
     * starting with 0 and counting from the top (field for connections out)
     */
	SoMFInt32 selectedIndices;
	/** this field contains the indices of the currently selected entries, 
     * starting with 0 and counting from the top (field for connections in/out)
     */
	SoMFInt32 selectedIndicesIn;
 
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

	SoNode *buttonStyles;

protected:

    /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */

    /// this method will relayout the list box and recreate the buttons
    virtual void adaptToConfig();

	/// each sub class should behave correctly on dimension changes
	virtual void resizeWidgetCB();		
	
	/// adapts the parameters of the navigation slider (minValue, maxValue, enable) to the number of buttons
    virtual void updateLabels();

    /// this method adapts the states and labels of the different visible buttons according to the current display index
    virtual void adaptToState();

	/// this method checks if the navigation slider has to be displayed and eventually hiddes / shows it
	virtual void updateNavigationDisplay();

	///adapts the state of the buttons (pressed or not pressed) when the selected indices change
	virtual void adaptToSelectedIndices();
    ///this method changes the labels of the buttons when the slider moves
    virtual void setButton( int buttonIndex, int index);

    SoFieldSensor navButtonSensor;

	///field sensor attached to the spacing width, changed only at the creation of the list box

    SoFieldSensor spacingSensor;
    SoFieldSensor numButtonsSensor;
    
	///field sensors attached to the different on-values of the toogle buttons of the list box
    
//	SoFieldSensor pressedSensor[50];
	SbPList   pressedSensors;
	///field sensor attached to the current display index
	SoFieldSensor displayIndexSensor;
	///method called when the display index changes, calls adaptToState()
	static void displayIndexChangedCB( void * data, SoSensor * sensor );
	
	///field sensor attached to the displayIndexIn
	SoFieldSensor displayIndexInSensor;
	///method called when the displayIndexIn changes, assigns the value of displayIndexIn to the one of displayIndex
	static void displayIndexInChangedCB( void * data, SoSensor * sensor );

	///field sensor attached to the selected indices
    SoFieldSensor selectedIndicesInSensor;
	///method called when different indices are selected; calls adaptToSelectedIndices()
	static void selectedIndicesInChangedCB( void * data, SoSensor * sensor );

    static void valuesChanged( void * data, SoSensor * sensor );
	///method called at the creation of the list box; calls adaptToConfig(), updateLabels(), adaptToState().
    static void configChangedCB( void * data, SoSensor * sensor );

	static void buttonChanged( void * data, SoSensor * sensor );
    
	///specifies the default geometry of the back plane
	static const char *defaultBackPlane;

	virtual void updateFromButtonStates(SoSensor * sensor);
	virtual int getButtonId(SoToggleButton *button);

	SbPList   focusSensors;
	static void insideChangedCB( void * data, SoSensor * sensor );



public:
	void applyStyleToButton(SoToggleButton *button, int index);
	int currentStyleIdx;
	SoToggleButton *currentStyleButton; 

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // !defined(_SoLabelListBox_H_)
