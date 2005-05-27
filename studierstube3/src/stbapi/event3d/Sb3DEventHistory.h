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
/** class definition of Sb3DEventHistory
  *
  * subclasses from Open Inventor's SbPList, to conveniently manage
  * a history of So3DEvents.
  *
  * @author  Zsolt Marx
  *
  * $Id: Sb3DEventHistory.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/SbPList.h>
#include <Inventor/fields/SoSFBool.h>

#include <stbapi/event3d/So3DEvent.h>
#include <Inventor/SbBox.h>

#ifndef _SB_3D_EVENT_HISTORY_
#define _SB_3D_EVENT_HISTORY_

/**
 * Sb3DEventHistory is used to manage a constantly
 * sized history of events. When a So3DEvent is
 * added to the list, the oldest event becomes
 * forgotten.
 * @ingroup event
 */
class STBAPI_API Sb3DEventHistory : public SbPList
{

public:

	/**
	 * the constructor
	 */
	Sb3DEventHistory(const int size);

	/**
	 * the destructor
	 */
	~Sb3DEventHistory();

	/**
	 * adds a copy of the given event to the top of the list
	 */
	void addEvent(const So3DEvent* event, const SbXfBox3f* bbox);

	/**
	 * typecasts and returns the element with the given
	 * index
	 */
	So3DEvent *getEvent(int index) const;

	/**
	 * typecasts and returns the element with the given
	 * index
	 */
	SbXfBox3f *getBBox(int index) const;

	/**
	* typecasts and returns the element with the given
	* index
	*/
	SoSFBool *getIntSection(int index) const;

	/**
	 * returns the most current element
	 */
	So3DEvent *getCurrentEvent() const;

	/**
	 * returns the previous element
	 */
	So3DEvent *getPreviousEvent() const;

	/**
	 * returns the most current Bounding Box
	*/
	SbXfBox3f *getCurrentBBox() const;

	/**
	* returns the previous Bounding Box
	*/
	SbXfBox3f *getPreviousBBox() const;

	/**
	* returns the most current intersection of event-translation with its bounding box
	*/
	SoSFBool *getCurrentIntSection() const;

	/**
	* returns the previous intersection of event-translation with its bounding box
	*/
	SoSFBool *getPreviousIntSection() const;

	/**
	 * prints the history
	 */
	void print();

private:

	int totalSize;
};

#endif // _SB_3D_EVENT_HISTORY
