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
/** source file for SoEnterGesture class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoEnterGesture.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/interaction/SoEnterGesture.h>


SO_NODE_SOURCE(SoEnterGesture);



void SoEnterGesture::initClass()
{
  SO_NODE_INIT_CLASS(SoEnterGesture, SoGesture, "SoGesture");
}


SoEnterGesture::SoEnterGesture()
{
  initSoEnterGesture();
}


void SoEnterGesture::initSoEnterGesture()
{
  SO_NODE_CONSTRUCTOR(SoEnterGesture);
}


SbBool SoEnterGesture::detect(const Sb3DEventHistory &history, const SbXfBox3f &bbox)
{
  So3DEvent *currentEvent = history.getCurrentEvent();
  So3DEvent *previousEvent = history.getPreviousEvent();

  SoSFBool *currentIntSection = history.getCurrentIntSection();
  SoSFBool *previousIntSection = history.getPreviousIntSection();

  // ignore event, if it station is blocked
  if(currentEvent == NULL || previousEvent == NULL || isBlocked(currentEvent->getStation()))
    return(false);

  // if previous event was outside, and current is inside, so we entered the bbox
  if(!previousIntSection->getValue() && currentIntSection->getValue())
		return(true);
  return(false);
}
