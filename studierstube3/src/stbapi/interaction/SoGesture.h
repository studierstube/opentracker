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
/** class definition of SoGesture
  *
  * SoGesture encapsulates high level event semantics 
  *
  * @author  Zsolt Marx
  *
  * $Id: SoGesture.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_GESTURE_
#define _SO_GESTURE_

#include <Inventor/SbBox.h>
#include <Inventor/SbBasic.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>

#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/Sb3DEventHistory.h>


/**
 * A SoGesture is a certain characteristic pattern in a sequence of
 * So3DEvents. To create a new gesture, subclass from SoGesture and
 * overload the virtual detect()-method.
 * @ingroup interaction
 * @ingroup widgets
 */
class STBAPI_API SoGesture : public SoNode
{
  SO_NODE_ABSTRACT_HEADER(SoGesture);

public:
  SoMFInt32 stations;         // station numbers this Gesture should listen (or block)
  SoSFBool blocked;           // should we block stations (or allow them)

public:
  static void initClass();

protected:
  SoGesture();

public:
	/**
	 * the method where the checking if a certain
	 * gesture has been performed, is done.
	 */
	virtual SbBool detect(const Sb3DEventHistory &history, const SbXfBox3f &bbox);

  /**
   * helper method to detect if this station is blocked or allowed
   */
  bool isBlocked(int station) const;


  /**
   * helper method to get the previous allowed event (could be NULL)
   */
  So3DEvent* getPreviousEvent(const Sb3DEventHistory &history) const;
};

#endif // _SO_GESTURE_

