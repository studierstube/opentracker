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
/** source file for SoLocaleKit
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoLocaleKit.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include "SoLocaleKit.h"
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>

SO_KIT_SOURCE(SoLocaleKit);

// Inventor class initialization
void SoLocaleKit::initClass()
{
	SO_KIT_INIT_CLASS( SoLocaleKit, SoTrackedArtifactKit, "SoTrackedArtifactKit" );
}

SoLocaleKit::SoLocaleKit()
{
	SO_KIT_CONSTRUCTOR(SoLocaleKit);

    SO_KIT_CHANGE_ENTRY_TYPE(geometry, SoSeparator, SoSeparator );
    SO_KIT_ADD_CATALOG_LIST_ENTRY( users, SoGroup, FALSE, geometry, \0, SoUserKit, TRUE );
	SO_KIT_ADD_CATALOG_LIST_ENTRY( windows, SoGroup, FALSE, geometry, \0, SoGroup, TRUE );

    SO_KIT_ADD_FIELD(name, ("")); 
    SO_KIT_ADD_FIELD(internalStations, (-1));
    SO_KIT_ADD_FIELD(shared, (FALSE));

	SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    So3DSeparator * root = SO_GET_ANY_PART(this,"artifactRoot",So3DSeparator);
    root->eventMask = So3DSeparator::BM_ALL_EVENTS;
    root->boundingBoxCheck.setValue( FALSE );
        
    nameSensor.setFunction( SoLocaleKit::nameChanged );
    nameSensor.setData( this );
    nameSensor.attach( &name );

    stationSensor.setFunction( SoLocaleKit::stationChanged );
    stationSensor.setData( this );
    stationSensor.attach( &internalStations );
}

SoLocaleKit::SoLocaleKit( const SbName & name_ ) 
{
    SO_KIT_CONSTRUCTOR(SoLocaleKit);

    SO_KIT_CHANGE_ENTRY_TYPE(geometry, SoSeparator, SoSeparator );
    SO_KIT_ADD_CATALOG_LIST_ENTRY( users, SoGroup, FALSE, geometry, \0, SoUserKit, TRUE );
	SO_KIT_ADD_CATALOG_LIST_ENTRY( windows, SoGroup, FALSE, geometry, \0, SoGroup, TRUE );

    SO_KIT_ADD_FIELD(name, (""));  
    SO_KIT_ADD_FIELD(internalStations, (-1));
    SO_KIT_ADD_FIELD(shared, (FALSE));

	SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);

	So3DSeparator * root = SO_GET_ANY_PART(this,"artifactRoot",So3DSeparator);
    root->eventMask = So3DSeparator::BM_ALL_EVENTS;
    root->boundingBoxCheck.setValue( FALSE );

    name.setValue( name_ );
    SbString nodeName("LOCALE_");
    nodeName += name.getValue().getString();
    setName( nodeName );

    nameSensor.setFunction( SoLocaleKit::nameChanged );
    nameSensor.setData( this );
    nameSensor.attach( &name );

    stationSensor.setFunction( SoLocaleKit::stationChanged );
    stationSensor.setData( this );
    stationSensor.attach( &internalStations );
}

SoLocaleKit::~SoLocaleKit()
{
    nameSensor.detach();
    stationSensor.detach();
}

void SoLocaleKit::nameChanged( void * data, SoSensor * sensor )
{
    SoLocaleKit * self = (SoLocaleKit *) data;
    SbString nodeName("LOCALE_");
    nodeName += self->name.getValue().getString();
    self->setName( nodeName );
}

void SoLocaleKit::stationChanged( void * data, SoSensor * sensor )
{
    SoLocaleKit * self = (SoLocaleKit *) data;
    SoFieldList list;
    self->getMover()->translation.getForwardConnections( list );
    for( int i = 0; i < Stb3DEventGenerator::getNumStations(); i++ )
    {
        SoSFVec3f * field = Stb3DEventGenerator::getTranslationOffsetField(i);
        bool fieldConnected = (list.find( field ) != -1);
        bool stationListed = (self->internalStations.find( i ) != -1);
        if( fieldConnected && !stationListed )
        {
            field->disconnect();
            Stb3DEventGenerator::getRotationOffsetField(i)->disconnect();
        }
        else if( !fieldConnected && stationListed )
        {
            field->connectFrom( & self->getMover()->translation );
            Stb3DEventGenerator::getRotationOffsetField(i)->connectFrom( & self->getMover()->rotation );
        }
    }
}
