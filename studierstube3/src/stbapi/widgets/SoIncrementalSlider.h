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
/** class definition of SoIncrementalSlider
  *
  * @author  Thomas Psik
  * $Id: SoIncrementalSlider.h 4139 2005-03-22 09:59:04Z kaufmann $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_INCREMENTAL_SLIDER_
#define _SO_INCREMENTAL_SLIDER_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/SbPList.h>

//#include <Inventor/engines/SoInterpolate.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/widgets/SoSimpleSlider.h>
#include <stbapi/interaction/SoBehaviorKit.h>


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The SoIncrementalSlider class implements a typical 2D slider widget. It allows to set 
 * a range of values and select from that by moving the slider knob or pressing
 * the buttons on the left and the right. All of the widgets geometry can be parametrized 
 * with your own scenegraphs.
 *
 * The minValue and maxValue fields set the value range and the value field
 * will output the current value set. The width, height and depth fields
 * from the super class SoBehaviorKit set the size of the slider. 
 *
 *
 * File format :
@verbatim
SoIncrementalSlider {
    # from SoLayoutKit
    SoSFFloat width				1
    SoSFFloat height			1
    SoSFFloat depth				1

    SoSFFloat minValue			0
    SoSFFloat maxValue			20
 
    SoSFFloat value				5.0
    SoSFFloat valueIn			5.0	
    SoSFFloat alpha				0.5	
    SoSFFloat alphaIn			0.5	
 
    SoSFBool cropValueToIncrement FALSE	

	SoSFBool hideSlider			  FALSE # if TRUE hides slider (only buttons will be visible), 
										# if set TRUE and hideButtons is also TRUE hideButtons will be set to FALSE !!
	SoSFBool hideButtons		  FALSE # if TRUE hides up/down (only slider will be visible), 
										# if set TRUE and hideSlider is also TRUE hideSlider will be set to FALSE !!

	SoSFFloat increment			1.0

	SoSFTime interval			0.2
	
	SoSFFloat buttonSpace		0.15
	SoSFFloat widthDragger		0.05
	SoSFFloat scaleDraggerPath	0.8


	SoSFTrigger startDragging         	# trigger that will be fired before the value is changed (by grabbing the knop or pressing a button)
	SoSFTrigger finishedDragging 	    # trigger that will be fired after  the value is changed (by releasing the knop or pressing a button)
  
}
@endverbatim
 *
 * @ingroup widgets
 */
class STBAPI_API SoIncrementalSlider : public SoBehaviorKit
{
	SO_KIT_HEADER(SoIncrementalSlider);


//	SO_BEHAVIOR_CATALOG_ENTRY_HEADER(buttonAbort);
	SO_KIT_CATALOG_ENTRY_HEADER(sliderSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(sliderAbortGroup);
	SO_KIT_CATALOG_ENTRY_HEADER(sliderTranslation);
	SO_KIT_CATALOG_ENTRY_HEADER(slider);
	SO_KIT_CATALOG_ENTRY_HEADER(buttonSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(buttonAbortGroup);
	SO_KIT_CATALOG_ENTRY_HEADER(upTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(up);
	SO_KIT_CATALOG_ENTRY_HEADER(downTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(down);


public:
	///the current value of the slider (field for connections out)
    SoSFFloat value;
	///the current value of the slider (field for connections in/out)
    SoSFFloat valueIn;

	///the current alpha of the slider (field for connections out). Is in [0;1] and represents a normalized version of value.
	SoSFFloat alpha;
	///the current of the slider (field for connections in/out). Is in [0;1] and represents a normalized version of value.
    SoSFFloat alphaIn;
 
	/// the value of the slider's minimum position
	SoSFFloat minValue;
	/// the value of the slider's maximum position			
	SoSFFloat maxValue;

 
	SoSFBool cropValueToIncrement;
	/** if TRUE the backplane and the moving knob will not be rendered
	 *  if set TRUE and hideButtons is also TRUE hideButtons will be set to FALSE !!
	 */
	SoSFBool hideSlider;

	/** if TRUE hides up/down (only slider will be visible), 
	 *  if set TRUE and hideSlider is also TRUE hideSlider will be set to FALSE !!
	 */
	SoSFBool hideButtons;

	///indicates the resolution of the slider, e.g. the interval between two values the slider should be able to display
	SoSFFloat increment;
	
	///portion of space of the slider back plane that is used by the increament and decreament buttons. This value is given in normalized percentage [0;1],  
	SoSFFloat buttonSpace;

	 /// interval between display movements when navigation buttons are pressed
	SoSFTime interval;
	
	/**represents the relative distance of the maximum position of the slider knob to the end of the back plane [0;0.5].
	This value should correspond to half the width of the slider knob. */
	SoSFFloat widthDragger;
	///scale value for the region of the slider path
	SoSFFloat scaleDraggerPath;

	///trigger that will be fired before the value is changed (by grabbing the knop or pressing a button)
	SoSFTrigger startDragging;

	///trigger that will be fired after the value is changed (by releasing the knop or pressing a button)
	SoSFTrigger finishedDragging;


	/** Inventor class initialization */
	static void initClass();

	///the constructor
	SoIncrementalSlider();
	///the destructor
	~SoIncrementalSlider();
	
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways); ///< sets up the connection of the sensors
 
	/// is called by SoBehaviorKit when the value of the tooltip value is changed
	virtual void adaptToTooltips();
	
protected:
	/// adapts the geometry of the slider due to a change of the value or other parameters
	virtual void resizeWidgetCB();
	/// is called by SoBehaviorKit when the slider gets enabled or disabled
	virtual void adaptToEnable();
	
	
	///recognises a call for increasing the value of the slider
	SoFieldSensor upSensor;
	///function that is called when the increase button is pressed
    static void upPressed( void * data, SoSensor * sensor );
	
	///recognises a call for decreasing the value of the slider
    SoFieldSensor downSensor;
	///function that is called when the decrease button is pressed
    static void downPressed( void * data, SoSensor * sensor );
	
	///recognises changes of the alpha
 	SoFieldSensor alphaSensor;
	///function that is called when the alpha of the slider changes
	static void alphaChangedCB(void *data, SoSensor *sensor);
	
	///function for SoSlider::startDragging
	static void startDraggingCB(void* userData, SoDraggerKit* slider);
	
	///function for SoSlider::finishedDragging
	static void finishDraggingCB(void* userData, SoDraggerKit* slider);
	
	///recognises changes of the field hideSlider
	SoFieldSensor hideSliderSensor;
	///function that hides or unhides the slider and the moving knob
   	static void hideSliderChangedCB(void *data, SoSensor *);

	///recognises changes of the field hideButtons
	SoFieldSensor hideButtonsSensor;
	///function that hides or unhides the buttons and the moving knob
   	static void hideButtonsChangedCB(void *data, SoSensor *);

	SoFieldSensor insideSensor[3];
	static void insideChangedCB(void *data, SoSensor *);

	// these strings contain the default geometry
	///contains the default geometry for the label of the increasing button
	static const char *defaultUpButtonLabel;
	///contains the default geometry for the label of the decreasing button
	static const char *defaultDownButtonLabel;


};

#endif

