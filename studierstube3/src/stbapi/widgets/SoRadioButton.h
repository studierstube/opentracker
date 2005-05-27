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
/** push button implementation
  *
  * implements a push button, is derived from SoButtonKit, basically
  * lays down, which geometry to use for the four possible states and
  * fixes the gesture, that toggles the button states.
  *
  * @author  Zsolt Marx
  *
  * $Id: SoRadioButton.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef _SO_RADIO_BUTTON_
#define _SO_RADIO_BUTTON_

#include <stbapi/widgets/SoButtonKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
  * implements a push button, is derived from SoButtonKit, basically
  * lays down, which geometry to use for the four possible states and
  * fixes the gesture, that toggles the button states.
 * @ingroup widgets
 */
class STBAPI_API SoRadioButton : public SoButtonKit
{
	SO_BEHAVIOR_HEADER(SoRadioButton);

public:

	/**
	 * the class initialization
	 */
	static void initClass();

	/**
	 * the constructor
	 */
	SoRadioButton();

	/**
	 * the destructor
	 */
	~SoRadioButton();

	void handle3DEvent(SoHandle3DEventAction *h3a);

protected:

	/**
	 * turn other radioButtons under the same group node off
	 */
	void turnSiblingsOff(SoHandle3DEventAction *h3a);

    static char* radioButtonOnString;
    static char* radioButtonOffString;
    static char* radioButtonHighlightedOnString;
    static char* radioButtonHighlightedOffString;


};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SO_RADIO_BUTTON_

