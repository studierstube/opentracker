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
/** source file for SoButtonPressGesture class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoButtonPressGesture.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */\

#include <stbapi/interaction/SoButtonPressGesture.h>


SO_NODE_SOURCE(SoButtonPressGesture);



void SoButtonPressGesture::initClass()
{
  SO_NODE_INIT_CLASS(SoButtonPressGesture, SoGesture, "SoGesture");
}


/**
 * standard constructor
 */
SoButtonPressGesture::SoButtonPressGesture()
{
	initSoButtonPressGesture();
}

/**
 * the constructor allows you to specify
 * the number of the pen hardware's button
 */
SoButtonPressGesture::SoButtonPressGesture(int number)
{
  initSoButtonPressGesture();

	buttonNumber = number;
}


void SoButtonPressGesture::initSoButtonPressGesture()
{
  SO_NODE_CONSTRUCTOR(SoButtonPressGesture);

  SO_NODE_ADD_FIELD(buttonNumber, (0));
}


/**
 * has the scene object just been entered?
 */
SbBool SoButtonPressGesture::detect(const Sb3DEventHistory &history, const SbXfBox3f &bbox)
{
	So3DEvent *currentEvent = history.getCurrentEvent();
	SoSFBool *currentIntSection = history.getCurrentIntSection();

	// ignore event, if it station is blocked
	if(currentEvent == NULL || isBlocked(currentEvent->getStation()))
		return(false);
	
	SbBool pressed = true;
	int buttonStateChange = So3DEvent::ET_BUTTON0_EVENT + buttonNumber.getValue();
	
	if(currentIntSection->getValue() &&
		(currentEvent->getType() == So3DEvent::EventType(buttonStateChange)) &&
		(currentEvent->getButton(buttonNumber.getValue()) == pressed))
		return true;
	return false;
}


