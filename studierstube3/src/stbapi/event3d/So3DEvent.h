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
/** c++ header for So3DEvent class
  *
  * @author   Andreas Zajic, Markus Krutz, Gerd Hesina
  *
  * $Id: So3DEvent.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SO_3DEVENT_H__
#define __SO_3DEVENT_H__

#include <Inventor/events/SoSubEvent.h>
#include <Inventor/SoPath.h>
#include <Inventor/SbLinear.h>

#include <stbapi/stbapi.h>
#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#define STB_PRIMARY_BUTTON 			0
#define STB_SECONDARY_BUTTON 		1

#define STB_PRIMARY_BUTTON_EVENT 	1
#define STB_SECONDARY_BUTTON_EVENT  2

/**
 * 3D event class used by 3D aware nodes. Documented at 
 * http://www.studierstube.org/stbapi/So3DEvent.html
 *
 * So3DEvent inherits from SoEvent and encapsulates
 * a 3D user event which is either a pen movement,
 * a button state change, a notification about the
 * pen leaving or entering a scene object's bounding
 * box.
 * @ingroup event
 */
class STBAPI_API So3DEvent : public SoEvent {

	SO_EVENT_HEADER();

public:

	/// enums for easy evaluation of event data
	enum EventType {
		ET_MOVE_EVENT = 0,
		ET_BUTTON0_EVENT,
		ET_BUTTON1_EVENT,
		ET_BUTTON2_EVENT,
		ET_BUTTON3_EVENT,
		ET_BUTTON4_EVENT,
		ET_BUTTON5_EVENT,
		ET_BUTTON6_EVENT,
		ET_BUTTON7_EVENT,
		ET_MASTERMODE_EVENT,
		ET_SLAVEMODE_EVENT,
		ET_NORMALMODE_EVENT,
		ET_EXIT_EVENT,
		ET_ENTER_EVENT
	};

	enum ButtonNum {
		BUTTON0=0,
		BUTTON1,
		BUTTON2,
		BUTTON3,
		BUTTON4,
		BUTTON5,
		BUTTON6,
		BUTTON7
	};

	//
	// constructor and destructor
	//
	
	So3DEvent();
	~So3DEvent();

	//
	// functions for setting up and getting eventdata
	//

	/**
	 * The copy method returns a copy of the instance,
	 * the caller is responsible for deleting the copy afterwards
	 */
	So3DEvent *copy() const;

	void setStation(int st);
	int getStation() const;

	void setType(enum EventType t);
	EventType getType() const;

	void setTranslation(const SbVec3f &t);
	const SbVec3f &getTranslation(void) const;

	void setRotation(const SbRotation &r);
	const SbRotation &getRotation(void) const;

	void setButton(int bNum, SbBool b);
	SbBool getButton(int bNum) const;
	
	void setTime(const SbTime &t);
	const SbTime &getTime() const;

	void setDestination(const SoFullPath *dest);
	const SoFullPath *getDestination(); //path returned should not be changed from outside


	//
	// initialize Class
	//

	static void initClass();

	
private:
	
	//
	// variables that store the data
	//

	int station;

	enum EventType type;

	SbVec3f translation;
	SbRotation rotation;

	const SoFullPath *destination;	// we must not change the path, destination points to

	SbBool button[8];	    // TRUE, FALSE

	SbTime time;
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
