 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: SignPost
  * ======================================================================== */
/** source file for SoInfoKit
  *
  * @author Gerhard Reitmayr
  * 
  * $Id: SoInfoKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <assert.h>

#include <Inventor/nodes/SoInfo.h>
#include <Inventor/actions/SoSearchAction.h>

#include <stbapi/misc/SoMultiSwitch.h>

#include "SoInfoKit.h"


SO_KIT_SOURCE(SoInfoKit);
 
// class initialization for OpenInventor

void SoInfoKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoInfoKit, SoContextKit, "SoContextKit");
}

// constructor

SoInfoKit::SoInfoKit()
{
    SO_KIT_CONSTRUCTOR(SoInfoKit);

    //-----------------------name-|-type--|-deftype|nuldef|-parent-|right sib|public
    // SO_KIT_ADD_CATALOG_ENTRY( , SoGroup, FALSE, this, \x0, FALSE );

    SO_KIT_INIT_INSTANCE();

    SO_KIT_ADD_FIELD( currentIcon, (NULL));
    SO_KIT_ADD_FIELD( pickedPath, (NULL));
    SO_KIT_ADD_FIELD( destination, (""));
    SO_KIT_ADD_FIELD( info, (""));
    SO_KIT_ADD_FIELD( index, (-1));

    info.set1Value(0,"");
    info.set1Value(1,"");
    info.set1Value(2,"");

    pathSensor.setData( this );
    pathSensor.setFunction( SoInfoKit::pathChanged );
    pathSensor.attach( &pickedPath );
}

SoInfoKit::~SoInfoKit()
{

}

void SoInfoKit::pathChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    SoInfoKit * self = (SoInfoKit *)data;
    // start with more than one because we expect a node to be at least below
    // a SoMultiSwitch node.
    if( self->pickedPath.getNum() > 1 )
    {
        for( int i = self->pickedPath.getNum()-2; i >= 0; i-- )
        {
            if( self->pickedPath[i]->isOfType(SoMultiSwitch::getClassTypeId()))
            {
                self->setPickedIcon(self->pickedPath[i+1], 
                    ((SoGroup *)self->pickedPath[i])->findChild(self->pickedPath[i+1]));
                break;
            }
        }
        if( i < 0)
            self->setPickedIcon( NULL, -1 );
    }
    else
        self->setPickedIcon( NULL, -1 );
}

void SoInfoKit::setPickedIcon( SoNode * icon, int childIndex )
{
    if( icon != NULL )
    {
        if( icon != currentIcon.getValue() )
        {
            SoSearchAction sa;
            sa.setType( SoInfo::getClassTypeId() );
            sa.setInterest( SoSearchAction::ALL );
            sa.apply( icon );

            SbString room;
            SbString tel;
            SbString person;

            SoPathList list = sa.getPaths();
            for( int i = 0; i < list.getLength(); i++)
            {
                assert(list[i]->getTail()->isOfType(SoInfo::getClassTypeId()));
                SoInfo * info = (SoInfo *)list[i]->getTail();
                if(info->getName() == "ROOM")
                    room = info->string.getValue();
                else if( info->getName() == "TEL" )
                    tel = info->string.getValue();
                else if( info->getName() == "PERSON" )
                    person = info->string.getValue();
            }

            currentIcon.setValue( icon );
            info.setNum(3);
            info.set1Value(0, room);
            info.set1Value(1, tel);
            info.set1Value(2, person);
            destination.setValue( room );
            index.setValue( childIndex );
        }
    }
    else
    {
        if( currentIcon.getValue() != NULL )
        {
            currentIcon.setValue(NULL);
            destination.setValue("");
            info.set1Value(0,"");
            info.set1Value(1,"");
            info.set1Value(2,"");
            index.setValue(-1);
        }
    }
}