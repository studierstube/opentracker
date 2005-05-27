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
/** class definition of SoTextListBox
  *
  * @author  Thomas Psik
  *
  * $Id: SoTextListBox.h 4047 2004-11-25 12:34:27Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoTextListBox_H_
#define _SoTextListBox_H_

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
#include <stbapi/widgets/SoLabelListBox.h>

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
 * The difference to the SoLabelListBox lies in the type of the labels. In this case, labels representing the different
 * items can only be text strings, and are specified in the field "values".
 *
 * File format:
@verbatim
SoTextListBox {
	SoMFString values                ""
	SoMFString selectedValues	     ""

    #from SoLabelListBox
    SoSFInt32   selectedIndices      0
    SoMFInt32 	selectedIndicesIn    0
 
	
    SoSFInt32	numOfRows			 5
    SoSFInt32	numOfCols			 1
	SoSFFloat 	spacingWidth         0.05 // 5%
	SoSFFloat 	spacingDepth         0.05 // 5%
	SoSFFloat 	startEndSpacingWidth 0.05 // 5%
	SoSFFloat 	startEndSpacingDepth 0.05 // 5%

    SoSFEnum    navigationPlacing    LEFT
	SoSFFloat	navigationSize		 0.2 // 20%
	SoSFFloat   navigationButtonToSliderRatio 0.2 //20%


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


class STBAPI_API SoTextListBox : public SoLabelListBox 
{
    SO_KIT_HEADER(SoTextListBox);
     
public:
 

	/** Inventor class initialization */
    static void initClass();

	///constructor
    SoTextListBox();
	
	///destructor
    virtual ~SoTextListBox();

	///this field contains the labels of the different items in form of text strings
 	SoMFString values;
	
	SoMFString selectedValues;
    
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);


protected:
      /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */

    ///field sensor attached to the values
	SoFieldSensor valuesSensor;
	///method that is called when the values change
	static void valuesChangedCB( void * data, SoSensor * sensor );
	///in the SoTextListBox, labels just contain the different text strings of the values field
	virtual void updateLabels(void);

};

/*
class ButtonSensorData 
{
public:
	ButtonSensorData(SoTextListBox *ptr_, int id_) 
	{
		ptr = ptr_; id = id_;
	}

	SoTextListBox *ptr;
	int id;
};
*/

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // !defined(_SoTextListBox_H_)
