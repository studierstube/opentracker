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
/** class definition of SoButtonReleaseGesture
  *
  * SoButtonReleaseGesture can check through its detect method,
  * whether the button's state has just changed from pressed
  * to unpressed
  *
  * @author  Zsolt Marx
  *
  * $Id: SoButtonReleaseGesture.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/SbBox.h>
#include <Inventor/fields/SoSFInt32.h>

#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/Sb3DEventHistory.h>

#include <stbapi/interaction/SoGesture.h>

/**
 * SoButtonReleaseGesture can check through its detect method,
 * whether the button's state has just changed from pressed
 * to unpressed
 * @ingroup widgets
 */
class STBAPI_API SoButtonReleaseGesture : public SoGesture
{
  SO_NODE_HEADER(SoButtonReleaseGesture);

public:
  static void initClass();

protected:
  void initSoButtonReleaseGesture();

public:

	/**
	 * The standard constructor assumes, we are interested
	 * in button number 0.
	 */
	SoButtonReleaseGesture();

	/**
	 * The constructor allows one to specify a certain button
	 * number
	 */
	SoButtonReleaseGesture(int number);

	/// has the button just been pressed?
	SbBool detect(const Sb3DEventHistory &history, const SbXfBox3f &bbox);

private:

	// the number of the pen hardware's button
	SoSFInt32 buttonNumber;
};

