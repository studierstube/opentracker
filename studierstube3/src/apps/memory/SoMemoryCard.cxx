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
/** source file for SoMemoryCard kit
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoMemoryCard.cxx 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */


#include "memory.h"
#include "SoMemoryCard.h"
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>

SO_KIT_SOURCE(SoMemoryCard);

void SoMemoryCard::initClass()
{
	SO_KIT_INIT_CLASS(SoMemoryCard, SoStationKit, "SoStationKit");
}

SoMemoryCard::SoMemoryCard()
{
	SO_KIT_CONSTRUCTOR(SoMemoryCard);

	SO_KIT_ADD_CATALOG_ENTRY( modelSwitch, SoSwitch, FALSE, content, \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY( model,	   SoGroup,  FALSE, modelSwitch, \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY( animation,   SoGroup,  FALSE, modelSwitch, \x0, TRUE );	

	SO_NODE_ADD_FIELD( distance, (0.0025));

	SO_KIT_INIT_INSTANCE();

	transformUpdate.setValue( FALSE );
	stationPosition.set1Value(1, SbVec3f());
	stationOrientation.set1Value(1, SbRotation());

	addEventCallback(SoMemoryCard::eventCB, this);
}

SoMemoryCard::~SoMemoryCard()
{
	removeEventCallback(SoMemoryCard::eventCB);
}

void SoMemoryCard::eventCB(void *userData, int stationId, SoStationKit *stationKit, SoHandle3DEventAction * action)
{
	SoMemoryCard * self = (SoMemoryCard *)userData;
	if( stationId == self->stations[0] )
	{
		SoTransform * xf = (SoTransform*)self->transform.getValue();
		xf->translation.setValue( self->position.getValue());
		xf->rotation.setValue( self->orientation.getValue());
	} else { // test for timeout between both event channels
		if( self->timeOut.getValue() > 0.0 )
			if((self->lastEvent[1] - self->lastEvent[0]) > self->timeOut.getValue())
				((SoSwitch*)self->visibleSwitch.getValue())->whichChild.setValue(-1);

	}
	int animate = self->currentPosition[0].equals( self->currentPosition[1], self->distance.getValue());
	((SoSwitch *)self->modelSwitch.getValue())->whichChild.setValue( animate );
}