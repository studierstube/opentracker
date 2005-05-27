/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: memory
 * ======================================================================== */
/**
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoMemoryCard.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _SoMemoryCard_H_
#define _SoMemoryCard_H_

#include <Inventor/fields/SoSFFloat.h>
#include <stbapi/tracker/SoStationKit.h>

class SoHandle3DEventAction;

/** 
 * @defgroup memory
 * A small demo application implementing a simple memory game.
 * @ingroup apps
 */

/** 
 * This class implements a switch between two different geometries based on the
 * distance to another SoMemoryCard instance. The kit itself is derived from
 * an SoStationKit and moves its geometry according to tracking input. If it
 * comes close enough to a defined other SoMemoryCard it switches to another
 * geometry. It actually uses the first two stations in the stations field to
 * do so. The first determines the location of the SoMemoryCard and the 
 * second the station of the other card. 
 *
 * The geometry in the model part is shown while the SoMemoryCard is not in
 * the proximity of its partner card. Otherwise the geomerty in the animation
 * part will be visible.
 *
 * @author Gerhard Reitmayr
 * @ingroup memory
 */
class MEMORY_API SoMemoryCard : public SoStationKit
{

	SO_KIT_HEADER(SoMemoryCard);

	SO_KIT_CATALOG_ENTRY_HEADER(modelSwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(model);
	SO_KIT_CATALOG_ENTRY_HEADER(animation);

public:
	SoMemoryCard();

	virtual ~SoMemoryCard();

	static void initClass();

	SoSFFloat distance;

protected:
	static void eventCB( void *userData, int stationId, 
		SoStationKit *stationKit, SoHandle3DEventAction *action);
};

#endif // _SoMemoryCard_H_
