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
/** source file of the SoLocaleManagerKit class
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoLocaleManagerKit.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/actions/SoSearchAction.h>

#include <stbapi/workspace/SoLocaleManagerKit.h>
#include <stbapi/workspace/SoLocaleKit.h>
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/window3d/SoWindowKit.h>

// pointer to the singleton instance
SoLocaleManagerKit * SoLocaleManagerKit::instance = NULL;

SO_KIT_SOURCE( SoLocaleManagerKit );

// static OpenInventor initialization
void SoLocaleManagerKit::initClass()
{
	SO_KIT_INIT_CLASS(SoLocaleManagerKit, SoBaseKit, "BaseKit" );
}

// constructor
SoLocaleManagerKit::SoLocaleManagerKit()
{
	SO_KIT_CONSTRUCTOR(SoLocaleManagerKit);
	SO_KIT_ADD_CATALOG_LIST_ENTRY( visibleLocales, SoGroup, FALSE, this, \0, SoLocaleKit, TRUE );
	SO_KIT_INIT_INSTANCE();	

    setSearchingChildren(TRUE);
}

// destructor
SoLocaleManagerKit::~SoLocaleManagerKit()
{
}

// returns the singleton instance of the SoLocaleManagerKit
SoLocaleManagerKit * SoLocaleManagerKit::getInstance()
{
	if( instance == NULL )
		instance = new SoLocaleManagerKit;
	return instance;
}

// creates a new locale with the given name. 
SoLocaleKit * SoLocaleManagerKit::createLocale( const SbName & name )
{
	SoLocaleKit * locale = getLocale( name );
	if( locale == NULL )
	{
		locale = new SoLocaleKit( name );
		SoNodeKitListPart * list = SO_GET_PART(this, "visibleLocales", SoNodeKitListPart);
		assert( list );
		list->addChild( locale );
	}
	return locale;
}

// destroys the locale specified by given name. 	 
void SoLocaleManagerKit::destroyLocale( const SbName & name )
{
	SoLocaleKit * locale = getLocale( name );
	if( locale != NULL )
	{
		SoNodeKitListPart * list = SO_GET_PART(this, "visibleLocales", SoNodeKitListPart);
		assert( list );
		list->removeChild( locale );
	}
}

// returns a locale specified by a given name. 
SoLocaleKit * SoLocaleManagerKit::getLocale( const SbName & name )
{
	SoSearchAction search;
	
    SbString searchName( "LOCALE_" );
	searchName += name.getString();	
	search.setName( searchName );
	search.apply( this );
	
	SoFullPath * path = (SoFullPath *)search.getPath();
	if( path == NULL )
		return NULL;
	SoNode * node = path->getTail();
	assert( node->isOfType( SoLocaleKit::getClassTypeId()));
    return (SoLocaleKit *) node;
}


// returns the locale a given SoUserKit is in
SoLocaleKit * SoLocaleManagerKit::getLocaleFromNode( SoNode * node )
{
    SoSearchAction search;

    search.setNode( node );
    search.apply( this );
    
    SoFullPath * path = (SoFullPath *) search.getPath();
    if( path == NULL )
        return NULL;
    SoLocaleKit * result = NULL;
    for( int i = 0; i < path->getLength(); i++ )
    {
        if( path->getNode(i)->isOfType( SoLocaleKit::getClassTypeId()))
        {
            result = (SoLocaleKit *) path->getNode(i);
            break;
        }
    }
    return result;
}

// adds a given SoUserKit to a locale.
void SoLocaleManagerKit::addUserKit( SoUserKit * userKit, SoLocaleKit * locale)
{
    SoNodeKitListPart * list = SO_GET_PART(locale, "users", SoNodeKitListPart);
    if( list->findChild( userKit ) == -1 )
    {
        list->addChild( userKit );
        // add the necessary stations to the locale
        if( userKit->getPip() != NULL )        
            locale->internalStations.find( userKit->getPip()->station.getValue(), TRUE);
        if( userKit->getPen() != NULL )
            locale->internalStations.find( userKit->getPen()->station.getValue(), TRUE);
        if( userKit->getDisplay() != NULL )
            locale->internalStations.find( userKit->getDisplay()->station.getValue(), TRUE);
    }
}

// adds a given SoUserKit to a locale. 
void SoLocaleManagerKit::addUserKit( SoUserKit * userKit, const SbName & name )
{
    SoLocaleKit * locale = getLocale( name );
    if( locale != NULL )    
        addUserKit( userKit, locale );
}

// removes an SoUserKit from its locale. 
void SoLocaleManagerKit::removeUserKit( SoUserKit * userKit)
{
    SoLocaleKit * locale = getLocaleFromNode( userKit );
    if( locale != NULL )
    {
        SoNodeKitListPart * list = SO_GET_PART(locale, "users", SoNodeKitListPart);
        // removes the necessary stations from the locale
        if( userKit->getPip() != NULL )
        {
            int index = locale->internalStations.find( userKit->getPip()->station.getValue());
            if( index != -1 )
                locale->internalStations.deleteValues( index, 1 );
        }
        if( userKit->getPen() != NULL )
        {
            int index = locale->internalStations.find( userKit->getPen()->station.getValue());
            if( index != -1 )
                locale->internalStations.deleteValues( index, 1 );
        }
        if( userKit->getDisplay() != NULL )
        {
            int index = locale->internalStations.find( userKit->getDisplay()->station.getValue());
            if( index != -1 )
                locale->internalStations.deleteValues( index, 1 );
        }
        list->removeChild( userKit );
    }
}

// adds a given SoWindowKit to a locale. 
void SoLocaleManagerKit::addApplicationKit( SoGroup * appGroup, SoLocaleKit * locale)
{
    SoNodeKitListPart * list = SO_GET_PART(locale, "windows", SoNodeKitListPart);
    if( list->findChild( appGroup ) == -1 )
        list->addChild( appGroup );
}

// adds a given SoWindowKit to a locale.
void SoLocaleManagerKit::addApplicationKit( SoGroup * appGroup, const SbName & name)
{
    SoLocaleKit * locale = getLocale( name );
    if( locale != NULL )    
        addApplicationKit( appGroup, locale );
}

// removes an SoWindowKit from its locale.
void SoLocaleManagerKit::removeApplicationKit( SoGroup * appGroup)
{
    SoLocaleKit * locale = getLocaleFromNode( appGroup );
    if( locale != NULL )
    {
        SoNodeKitListPart * list = SO_GET_PART(locale, "windows", SoNodeKitListPart);
        list->removeChild( appGroup );        
    }
}
