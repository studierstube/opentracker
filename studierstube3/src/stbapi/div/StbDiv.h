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
/** header file for div and studierstube integration
  *
  * @author   Gerd Hesina, Jan Prikryl, Gerhard Reitmayr
  *
  * $Id: StbDiv.h 3504 2004-07-20 11:34:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _STBDIV_H_INCLUDED_
#define _STBDIV_H_INCLUDED_

#include "divmain.h"

class So3DCheckBox;
class SoBase3DSlider;
class SoBase3DButton;
class SoBase3DDial;
class So3DRadioButton;
class SoDragSeparator;
class SoDragKit;

/**
 * This class implements additional DIV messages for Studierstube specific
 * distribution. These messages transport special information for the 
 * following 3D widgets : 
 * @li So3DCheckBox
 * @li SoBase3DSlider
 * @li SoBase3DDial
 * @li SoBase3DButton
 * @li So3DRadioButton
 * @li SoDragKit
 * @li SoDragSeparator
 * Note, that these widgets are all old widgets and should not be used anymore.
 * Therefore we hope to deprecate this class as well in the future. Any special
 * communication needed by an OpenInventor node should be implemented on the
 * node level, not the DIV level !
 *
 * @ingroup div
 * @author Gerd Hesina, Gerhard Reitmayr
 */
class STBAPI_API StbDiv : public CDivMain
{
public:
	
	/** special messages for the StbDiv extensions */
	enum {
		DIV_SET_CHECKBOX_STATE = CDivMain::DIV_LAST_MESSAGE_ID,
		DIV_SET_SLIDER_STATE,
		DIV_SET_DRAGKIT_STATE,
		DIV_SET_DRAGSEPARATOR_STATE,
		DIV_SET_BUTTON_STATE,
		DIV_SET_DIAL_STATE,
		DIV_SET_RADIOBUTTON_STATE
	};
	
	StbDiv(const char* address, unsigned int port, Mode theMode, bool initFromRemote = false, const char *nic = NULL);
	
	virtual ~StbDiv();

	void setCheckBoxState(So3DCheckBox *theCheckBox);

	void setSliderState(SoBase3DSlider *theSlider, float currentValue);

	void setDialState(SoBase3DDial *theDial, float currentValue);

	void setButtonState(SoBase3DButton *theButton);

	void setRadioButtonState(So3DRadioButton *theRadioButton);

	void setDragKitState(SoDragKit *theDragKit, SbVec3f trans, SbRotation rot);

	void setDragSeparatorState(SoDragSeparator *theDragSeparator, 
		SbVec3f trans, SbRotation rot);

protected:
	virtual SbBool processMessage(StbBuffer *inBuffer, short divCommand);
};

#endif
//eof
