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
/** class definition of SoListBox
  *
  * @author  Gerhard Reitmayr
  *
  * $Id: SoListBox.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOLISTBOX_H_
#define _SOLISTBOX_H_

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <stbapi/widgets/SoButtonKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * This class provides a simple list box interface with a number of buttons 
 * showing different items on the left and two navigation buttons with up/down
 * arrows on the right. Some influence can be taken on the number of buttons
 * and general layout. There is a hardcoded default button geometry , but you can
 * configure that as well. Besides that it uses only fields to 
 * communicate its state. 
 *
 * The behaviour is simple. A list of string items is displayed on the column of buttons
 * on the left. On the right there are two buttons to navigate up and down
 * through the list, if there are more items then buttons. Item buttons can be
 * pressed or not pressed and the pressed buttons define the selected items that 
 * are output in the selected and selectedIndex fields.
 *
 * The look of the list box can be modified in different ways. You can specify
 * the up and down buttons using the parts @c up and @c down. These need to be
 * of type SoPushButton (or derived of course). The look of the item buttons 
 * can be specified with four templates for the four states of the buttons that
 * are stored in the parts @c on , @c off , @c onHl and @c offHl .
 * These buttons are of type SoToggleButton and the templates must adhere to the
 * rules given there. In addition to that the templates also must contain a
 * SoText3 node that is set to contain the item text. If you want to have more 
 * layout or design options take a look at SoListBox::adaptToConfig().
 *
 * File format:
@verbatim
SoListBox {
    SoMFString 	values               []
    SoMFString 	selected             []
    SoMFInt32 	selectedIndices      []
    SoSFBool 	multipleSelections   FALSE
    SoSFFloat 	width                1
    SoSFFloat 	height               1
    SoSFFloat 	depth                1
    SoSFFloat 	navButtonWidth       0.2
    SoSFFloat 	spacing              0.02
    SoSFInt32 	numButtons           5
    SoSFInt32 	displayIndex         0
    SoSFTime    interval             0.5
  }
@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup widgets
 * @todo optimize for DIV
 */
class SoListBox : public SoBaseKit  
{
    SO_KIT_HEADER(SoListBox);
    
	SO_KIT_CATALOG_ENTRY_HEADER(geometrySwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(off);
	SO_KIT_CATALOG_ENTRY_HEADER(on);
	SO_KIT_CATALOG_ENTRY_HEADER(offHl);
	SO_KIT_CATALOG_ENTRY_HEADER(onHl);
    SO_KIT_CATALOG_ENTRY_HEADER(navigation);
    SO_KIT_CATALOG_ENTRY_HEADER(xfUp);
    SO_KIT_CATALOG_ENTRY_HEADER(up);
    SO_KIT_CATALOG_ENTRY_HEADER(xfDown);
    SO_KIT_CATALOG_ENTRY_HEADER(down);
    SO_KIT_CATALOG_ENTRY_HEADER(buttons);
    
public:
    /** Inventor class initialization */
    static void initClass();

    SoListBox();
	
    virtual ~SoListBox();

    /** this field contains the string values displayed on the list box and
     * returned in the selected field. If this field is changed the list box
     * updates itself and deletes any selections.
     */
    SoMFString values;

    /** this field contains the currently selected entries */
    SoMFString selected;

    /** this field contains the indices of the currently selected entries, 
     * starting with 0 and counting from the top 
     */
    SoMFInt32 selectedIndices;

    /** flag denotes whether multiple selections are allowed or not */
    SoSFBool multipleSelections;

    /**
	 * the widget's width
	 */
	SoSFFloat width;

	/**
	 * the widget's height
	 */
   	SoSFFloat height;

	/**
	 * the widget's depth
	 */
	SoSFFloat depth;

    /** the width of the up/down buttons in an absolute value. Should be smaller
     * then width - spacing for obvious reasons, but is probably not checked :)
     */
    SoSFFloat navButtonWidth;

    /** defines the spacing between the up/down buttons and the item buttons and 
     * the between the individual item buttons in the left row again in an absolute
     * value. */
    SoSFFloat spacing;

    /** defines the number of item buttons to generate and use. Note, that there
     * will alwaws by numButtons generated, even if less entries are available. 
     */
    SoSFInt32 numButtons;

    /** this field defines the index of the item appearing on the top most (first) 
     * button. It allows the user to check for up/down movements and to 
     * determine which entries are visible.
     */
    SoSFInt32 displayIndex;

    /// interval between display movements when nav buttons are pressed
    SoSFTime interval;

protected:
    
    /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// this method will relayout the listbox and recreate the buttons
    void adaptToConfig(void);

    /// this method will set the button state to the current listbox state
    void adaptToState(void);

    /** this method sets a data button to the correct state
     * @param index number of the button starting with 0
     * @param string line to display
     * @param state pressed or not
     */
    void setButton( int index, SbString string, SbBool state );

    SoFieldSensor valuesSensor;
    SoFieldSensor widthSensor;
    SoFieldSensor heightSensor;
    SoFieldSensor depthSensor;
    SoFieldSensor navButtonSensor;
    SoFieldSensor spacingSensor;
    SoFieldSensor numButtonsSensor;
	SoFieldSensor selectedSensor;
	SoFieldSensor selectedIndexSensor;
    SoTimerSensor navTimer;
    
    static void valuesChanged( void * data, SoSensor * sensor  );

    static void configChanged( void * data, SoSensor * sensor );

    static void buttonPressed( void * data, SoButtonKit* buttonKit );

    static void navPressed( void * data, SoButtonKit * buttonKit );
    static void navReleased( void * data, SoButtonKit * buttonKit );

    static void navTimerFired( void * data, SoSensor * sensor );

	static void selectedChanged( void * data, SoSensor * sensor );

	static void selectedIndexChanged( void * data, SoSensor * sensor );
    
    SbBool navigatingUp;

	static char * offString;
	static char * onString;
	static char * offHlString;
	static char * onHlString;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // !defined(_SOLISTBOX_H_)
