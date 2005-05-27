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
/** source file for Sb3DEventHistory
  *
  * subclasses from Open Inventor's SbPList, to conveniently manage
  * a history of So3DEvents.
  *
  * @author  Zsolt Marx, Hannes Kaufmann
  *
  * $Id: Sb3DEventHistory.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/event3d/Sb3DEventHistory.h>

/**
 * the constructor
 */
Sb3DEventHistory::Sb3DEventHistory(const int psize)
{
	totalSize = psize;

	// call the parent constructor
	SbPList::SbPList(totalSize);
}

/**
 * the destructor
 */
Sb3DEventHistory::~Sb3DEventHistory()
{
}

/**
 * adds a copy of the given event to the top of the list
 */
void Sb3DEventHistory::addEvent(const So3DEvent* event, const SbXfBox3f* bbox)
{
	if (getLength() > 6) 
	{
		// filter all studierstube generated EXIT events			
		if (event->getType() != So3DEvent::ET_EXIT_EVENT)
		{
			So3DEvent *eventCopy = new So3DEvent();
			(*eventCopy) = (*event);
			
			SbXfBox3f *bboxCopy = new SbXfBox3f();
			(*bboxCopy) = (*bbox);
			
			// calculates and stores for every event its intersection with its bounding box
			SoSFBool *intSection = new SoSFBool();
			intSection->setValue(bboxCopy->intersect(eventCopy->getTranslation()));

			insert(intSection,0);
			insert(bboxCopy, 0);
			insert(eventCopy,0);
			
			// keep the history size below the given total size
			if (getLength()>totalSize) 
			{
				delete(get(totalSize));
				delete(get(totalSize+1));
				delete(get(totalSize+2));

				truncate (totalSize);
			}
		}
	}
	else
	{
		So3DEvent *eventCopy = new So3DEvent();
		(*eventCopy) = (*event);
		
		SbXfBox3f *bboxCopy = new SbXfBox3f();
		(*bboxCopy) = (*bbox);

		SoSFBool *intSection = new SoSFBool();
		intSection->setValue(bboxCopy->intersect(eventCopy->getTranslation()));

		insert(intSection,0);
		insert(bboxCopy, 0);
		insert(eventCopy,0);
	}
}

/**
 * typecasts and returns the element with the given
 * index
 */
So3DEvent *Sb3DEventHistory::getEvent(int index) const
{
	if (getLength() > index)
		return (So3DEvent *) get(index);
	else 
		return NULL;
}

/**
 * typecasts and returns the element with the given
 * index
 */
SbXfBox3f *Sb3DEventHistory::getBBox(int index) const
{
	if (getLength() > index)
		return (SbXfBox3f *) get(index);
	else 
		return NULL;
}

/**
 * typecasts and returns the element with the given
 * index
 */
SoSFBool *Sb3DEventHistory::getIntSection(int index) const
{
	if (getLength() > index)
		return (SoSFBool *) get(index);
	else 
		return NULL;
}


/**
 * returns the most current element
 */
So3DEvent *Sb3DEventHistory::getCurrentEvent() const
{
	if( getLength() > 0 )
		return getEvent(0);
	else
		return NULL;
}

/**
 * returns the previous element
 */
So3DEvent *Sb3DEventHistory::getPreviousEvent() const
{
	// make sure that enough events are in the list
	if (getLength() > totalSize-6)
	{
		// go through the list to find previousEvents from the same station/source
		for (int i=3; i<totalSize-6; i++)
		{
			// current and previous event must be of same source in order that
			// a comparison between those two can be of value
			if (getEvent(i)->getStation() == getCurrentEvent()->getStation())
				return getEvent(i);
			i++;
			i++;
		}
		return NULL;
	}
	else
		return NULL;
}

/**
 * returns the most current Bounding Box
 */
SbXfBox3f *Sb3DEventHistory::getCurrentBBox() const
{
	if( getLength() > 1 )
		return getBBox(1);
	else
		return NULL;
}

/**
 * returns the previous Bounding Box
 */
SbXfBox3f *Sb3DEventHistory::getPreviousBBox() const
{
	// make sure that enough events are in the list
	if (getLength() > totalSize-6)
	{
		// go through the list to find previousEvents from the same station/source
		for (int i=3; i<totalSize-6; i++)
		{
			// current and previous event must be of same source in order that
			// a comparison between those two can be of value
			if (getEvent(i)->getStation() == getCurrentEvent()->getStation())
				return getBBox(i+1);
			i++;
			i++;
		}
		return NULL;
	}
	else
		return NULL;
}

/**
 * returns the most current intersection of event-translation with its bounding box
 */
SoSFBool *Sb3DEventHistory::getCurrentIntSection() const
{
	if( getLength() > 2 )
		return getIntSection(2);
	else
		return NULL;
}

/**
 * returns the previous intersection of event-translation with its bounding box
 */
SoSFBool *Sb3DEventHistory::getPreviousIntSection() const
{
	// make sure that enough events are in the list
	if (getLength() > totalSize-6)
	{
		// go through the list to find previousEvents from the same station/source
		for (int i=3; i<totalSize-6; i++)
		{
			// current and previous event must be of same source in order that
			// a comparison between those two can be of value
			if (getEvent(i)->getStation() == getCurrentEvent()->getStation())
				return getIntSection(i+2);
			i++;
			i++;
		}
		return NULL;
	}
	else
		return NULL;
}

/**
 * prints the history
 */
void Sb3DEventHistory::print()
{
	printf("EVENTHISTORY:");
	for (int i=0; i<getLength(); i++)
	{
		if (getEvent(i)->getType() == So3DEvent::ET_EXIT_EVENT)
			printf("X");
		else if (getEvent(i)->getType() == So3DEvent::ET_MOVE_EVENT)
			printf("M");
		else if ((getEvent(i)->getType() >= So3DEvent::ET_BUTTON0_EVENT) ||
				 (getEvent(i)->getType() >= So3DEvent::ET_BUTTON7_EVENT))
			printf("B");
	}
	printf("\n");
}


