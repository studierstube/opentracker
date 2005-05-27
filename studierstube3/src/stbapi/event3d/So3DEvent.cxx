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
/** Implementation of class So3DEvent
  *
  * @author Andreas Zajic, Markus Krutz, Gerd Hesina
  *
  * $Id: So3DEvent.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/event3d/So3DEvent.h>

SO_EVENT_SOURCE(So3DEvent);

//
// initialize the class (must be called before any instance of the class is created)
//

void So3DEvent::initClass() {
	SO_EVENT_INIT_CLASS(So3DEvent, SoEvent);
}

//
// constructor
//

So3DEvent::So3DEvent() {

	translation.setValue(0.0, 0.0, 0.0);
	rotation.setValue((SbRotation::identity()).getValue());
	for (int i=0; i<8; ++i) {
		button[i] = FALSE;
	}
}

//
// destructor
//

So3DEvent::~So3DEvent() {
}

//
// set and get the station 
//

void So3DEvent::setStation(int st) {
	station=st;
}

int So3DEvent::getStation() const{
	return station;
}

//
// set and get the eventtype
//

void So3DEvent::setType(enum EventType t) {
	type=t;
}

So3DEvent::EventType So3DEvent::getType() const{
	return type;
}

//
// set and get the translation 
//

void So3DEvent::setTranslation(const SbVec3f &t) {
	translation.setValue(t.getValue());
}

const SbVec3f &So3DEvent::getTranslation(void) const{
	return translation;
}

//
// set and get the rotation(orientation)
//

void So3DEvent::setRotation(const SbRotation &r) {
	rotation.setValue(r.getValue());
}

const SbRotation &So3DEvent::getRotation(void) const{
	return rotation;
}

//
// set and get the buttonstate
//

void So3DEvent::setButton(int bNum, SbBool bstate) {

	button[bNum]=bstate;
}

SbBool So3DEvent::getButton(int bNum) const{
	return button[bNum];
}

//
// set and get the timestamp
//

void So3DEvent::setTime(const SbTime &t) {
  time=t;
}

const SbTime &So3DEvent::getTime() const{
  return time;
}

//
// set and get the path of the destination
//

void So3DEvent::setDestination(const SoFullPath *dest) {
	destination = dest;
}

const SoFullPath *So3DEvent::getDestination() {
	return destination;
}

/**
 * the copy method
 */
So3DEvent *So3DEvent::copy() const
{
    So3DEvent *newEvent = new So3DEvent;

    (*newEvent) = (*this);

    return newEvent;
}

