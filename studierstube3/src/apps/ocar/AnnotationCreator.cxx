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
* PROJECT: ocar
* ======================================================================== */
/**
* source for AnnotationCreator kit class
*
* @author Gerhard Reitmayr
*
* $Id: AnnotationCreator.cxx 3426 2004-07-14 12:11:27Z tamer $
* @file                                                                   */
/* ======================================================================= */

#include <cassert>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoSearchAction.h>

#include <stbapi/misc/SoMultiSwitch.h>
#include <stbapi/misc/SoStringMap.h>
#include <stbapi/div/SoDivGroup.h>

#include "AnnotationCreator.h"

SO_KIT_SOURCE(AnnotationCreator);

void AnnotationCreator::initClass(void)
{
    static SbBool first = TRUE;
    if (first) {
        SO_KIT_INIT_CLASS(AnnotationCreator, SoBaseKit, "BaseKit" );
        first = FALSE;
    } 
}

AnnotationCreator::AnnotationCreator() :
    oldButton( 0 )
{
    SO_KIT_CONSTRUCTOR(AnnotationCreator);
    
    isBuiltIn = TRUE;

    SO_KIT_ADD_CATALOG_ENTRY(hidden, SoSwitch, FALSE, this, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(markerTemplate, SoGroup, FALSE, hidden, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(icons, SoGroup, FALSE, this, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(markers, SoDivGroup, FALSE, hidden, \x0, TRUE);
    
    SO_KIT_INIT_INSTANCE();
    
    SO_KIT_ADD_FIELD( cursorPosition, (0,0,0));
    SO_KIT_ADD_FIELD( pickNormal, (0,0,0));
    SO_KIT_ADD_FIELD( isPicking, (FALSE));
    SO_KIT_ADD_FIELD( selectedIcon, (-1));
    SO_KIT_ADD_FIELD( selectedBuilding, (""));
    SO_KIT_ADD_FIELD( selectedWords, (""));
    SO_KIT_ADD_FIELD( iconTemplate, (NULL));
    SO_KIT_ADD_FIELD( colorTemplate, (NULL));
    SO_KIT_ADD_FIELD( pickButton, (0));
    SO_KIT_ADD_FIELD( markerMap, (NULL));
    SO_KIT_ADD_FIELD( indexMap, (NULL));
    SO_KIT_ADD_FIELD( markerKeywords, (""));
    SO_KIT_ADD_FIELD( name, (""));

    markerKeywords.deleteValues(0);
    markerKeywords.setDefault( TRUE );
    ((SoSwitch *)hidden.getValue())->whichChild.setValue(-1);

    buttonSensor.setData( this );
    buttonSensor.setFunction( AnnotationCreator::buttonChanged );
    markerSensor.setData( this );
    markerSensor.setFunction( AnnotationCreator::markerChanged );
    markerSensor.setPriority( 2 );

    setUpConnections( TRUE, TRUE );
}

AnnotationCreator::~AnnotationCreator()
{
}

SbBool AnnotationCreator::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        SoBaseKit::setUpConnections(onOff, doItAlways);
        // We connect AFTER base class.

        buttonSensor.attach( &pickButton );
        SoDivGroup * group = (SoDivGroup *)markers.getValue();
        markerSensor.attach( group );
        group->active.setValue( TRUE );
    }
    else {
        // We disconnect BEFORE base class.
        buttonSensor.detach();
        markerSensor.detach();
     
        SoBaseKit::setUpConnections(onOff, doItAlways);        
    }
    return !(connectionsSetUp = onOff);
}

/*
void AnnotationCreator::selectionFired( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    AnnotationCreator * self = (AnnotationCreator *)data;
}
*/

void AnnotationCreator::buttonChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    AnnotationCreator * self = (AnnotationCreator *)data;
    
    // test, if we got a release and we are picking ?!
    if( self->pickButton.getValue() == 0 && self->oldButton == 2 && self->isPicking.getValue())
    {
        // then create a new icon from the template icon and color !
        self->createMarker();
    }
    self->oldButton = self->pickButton.getValue();
}

void AnnotationCreator::createMarker()
{
    SoNode * marker = markerTemplate.getValue();
    SoNode * icon = iconTemplate.getValue();
    SoNode * color = colorTemplate.getValue();

    if( marker == NULL || icon == NULL || color == NULL )
        return;
    
    // create a copy of the template
    SoNode * result = ((SoGroup *)marker)->copy(TRUE);
    result->ref();

    SoSearchAction sa;
    sa.setType( SoInfo::getClassTypeId());
    sa.setInterest( SoSearchAction::ALL );
    sa.apply( result );
    SoPathList & list = sa.getPaths();
    int i;
    for( i = 0; i < list.getLength(); i++)
    {
        SoFullPath * path = (SoFullPath *) list[i];
        assert( path->getTail()->isOfType(SoInfo::getClassTypeId()));
        SoInfo * info = (SoInfo *)path->getTail();
        assert( path->getNodeFromTail(1)->isOfType(SoGroup::getClassTypeId()));
        SoGroup * group = (SoGroup *)path->getNodeFromTail(1);
        if (info->string.getValue() == "icon") {
            group->replaceChild( info, icon->copy(TRUE));
        } 
        else if (info->string.getValue() == "color") {
            group->replaceChild( info, color->copy(TRUE));
        }
        else if (info->string.getValue() == "user" ) {
            info->string.setValue( name.getValue());
        }
    }

    sa.setType( SoTransform::getClassTypeId());
    sa.setInterest( SoSearchAction::FIRST );
    sa.apply( result );
    SoFullPath * path = (SoFullPath *)sa.getPath();
    if( path != NULL )
    {
        SoTransform * transform = (SoTransform *)path->getTail();
        transform->translation.setValue( cursorPosition.getValue());
        // rotate geometry so that it aligns with the picked surface
        SbVec3f dir = pickNormal.getValue();
        SbVec3f zero(0,0,-1);
        SbRotation rot(zero, dir);
        transform->rotation.setValue(rot);
    }
 
    // use the pointer address as unique name
    SbString name = "MK_";
    name += SbString((int)result);
    result->setName( SbName( name ));

    // add to the new markers
    addMarker( result );
    // add to the distributed group
    SoDivGroup * divgroup = (SoDivGroup *)markers.getValue();
    if( divgroup->isMaster.getValue() == TRUE )
    {
        divgroup->addChild( result );
    }
   
    result->unref();
}

void AnnotationCreator::addMarker( SoNode * marker )
{
    SbString name = marker->getName().getString();
    SbString color, user;
    
    SoSearchAction sa;
    sa.setType( SoInfo::getClassTypeId());
    sa.setInterest( SoSearchAction::ALL );
    sa.apply( marker );
    SoPathList & list = sa.getPaths();
    int i;
    for( i = 0; i < list.getLength(); i++)
    {
        SoFullPath * path = (SoFullPath *) list[i];
        assert( path->getTail()->isOfType(SoInfo::getClassTypeId()));
        SoInfo * info = (SoInfo *)path->getTail();
        if( info->getName() == "USER" )
        {
            user = info->string.getValue();
        }
        else if( info->getName() == "COLOR" )
        {
            color = info->string.getValue();
        }
    }

    SoGroup * icons = SO_GET_PART( this, "icons", SoGroup );
    icons->addChild( marker );
    assert( indexMap.getValue()->isOfType(SoStringMap::getClassTypeId()));
    SoStringMap * idxMap = (SoStringMap *)indexMap.getValue();
    idxMap->a.set1Value(idxMap->a.getNum(), name);
    idxMap->b.set1Value(idxMap->b.getNum(), SbString(icons->getNumChildren()-1));

    assert( markerMap.getValue()->isOfType(SoStringMap::getClassTypeId()));
    SoStringMap * mkMap = (SoStringMap *)markerMap.getValue();
    mkMap->a.set1Value(mkMap->a.getNum(), user);
    mkMap->b.set1Value(mkMap->b.getNum(), name);
    mkMap->a.set1Value(mkMap->a.getNum(), color);
    mkMap->b.set1Value(mkMap->b.getNum(), name);

    if( markerKeywords.find(user) == -1 )
        markerKeywords.set1Value( markerKeywords.getNum(), user);
    if( markerKeywords.find(color) == -1 )
        markerKeywords.set1Value( markerKeywords.getNum(), color);
}

void AnnotationCreator::markerChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    AnnotationCreator * self = (AnnotationCreator *)data;
    SoNodeSensor * nodeSensor = (SoNodeSensor *) sensor;

    SoDivGroup * divgroup = (SoDivGroup *)self->markers.getValue();
    assert( self->indexMap.getValue()->isOfType(SoStringMap::getClassTypeId()));
    SoStringMap * idxMap = (SoStringMap *)self->indexMap.getValue();
    
    int i;
    for( i = 0; i < divgroup->getNumChildren(); i++)
    {
        SoNode * marker = divgroup->getChild(i);
        SbString name = marker->getName().getString();
        // we got a new marker ! add it to the data
        if( idxMap->a.find( name ) == -1 )
        {
            self->addMarker( marker );
        }
    }
}
