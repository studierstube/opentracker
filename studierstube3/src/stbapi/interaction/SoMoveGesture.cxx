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
/** source file for SoMoveGesture class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoMoveGesture.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/interaction/SoMoveGesture.h>


SO_NODE_SOURCE(SoMoveGesture);



void SoMoveGesture::initClass()
{
  SO_NODE_INIT_CLASS(SoMoveGesture, SoGesture, "SoGesture");
}


SoMoveGesture::SoMoveGesture()
{
  initSoMoveGesture();
}


void SoMoveGesture::initSoMoveGesture()
{
  SO_NODE_CONSTRUCTOR(SoMoveGesture);
}


SbBool SoMoveGesture::detect(const Sb3DEventHistory &history, const SbXfBox3f &bbox)
{
  So3DEvent *currentEvent = history.getCurrentEvent();
  So3DEvent *previousEvent = history.getPreviousEvent();

  // ignore event, if it station is blocked
  if(currentEvent == NULL || previousEvent == NULL || isBlocked(currentEvent->getStation()) || isBlocked(previousEvent->getStation()))
    return(false);

	return (history.getCurrentEvent()->getType() == So3DEvent::ET_MOVE_EVENT);
}


