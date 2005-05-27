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
/** source file for SoGesture class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoGesture.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/interaction/SoGesture.h>


SO_NODE_ABSTRACT_SOURCE(SoGesture);


SoGesture::SoGesture()
{
  SO_NODE_CONSTRUCTOR(SoGesture);

  SO_NODE_ADD_FIELD(stations, (-1));
  SO_NODE_ADD_FIELD(blocked, (true));
}


void SoGesture::initClass()
{
  SO_NODE_INIT_ABSTRACT_CLASS(SoGesture, SoNode, "SoNode");
}


SbBool SoGesture::detect(const Sb3DEventHistory &history, const SbXfBox3f &bbox)
{
	// this method must never be invoked
	return TRUE;
}


bool SoGesture::isBlocked(int station) const
{
  for(int i = 0; i < stations.getNum(); ++i)
    if(stations[i] == station)
      return(blocked.getValue() == TRUE);
  return(blocked.getValue() != TRUE);
}

/*
So3DEvent* SoGesture::getPreviousEvent(const Sb3DEventHistory &history) const
{
  for(int i = 1; i < history.getLength(); ++i)
  {
    So3DEvent *e = history.getEvent(i);
    if(e != NULL && !isBlocked(e->getStation()))
      return(e);
  }
	return(NULL);
}
*/
