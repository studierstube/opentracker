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
/** header file for the SoLocaleKit node kit.
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoLocaleKit.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOLOCALEKIT_H_
#define _SOLOCALEKIT_H_

#include <Inventor/SbString.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stbapi/resource/SoTrackedArtifactKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * a SoLocaleKit node kit represents a single locale in a workspace. It contains userkits
 * and application windows at given locations. Moreover it has a location itself
 * that describes it geometrical relation to all other locales. This location is
 * provided by a TrackerEngine via the underlying SoTrackedArtifactKit.
 *
 * @author Gerhard Reitmayr
 * @ingroup workspace
 */
class STBAPI_API SoLocaleKit : public SoTrackedArtifactKit  
{
	SO_KIT_HEADER(SoLocaleKit);

    SO_KIT_CATALOG_ENTRY_HEADER(users);
    SO_KIT_CATALOG_ENTRY_HEADER(windows);

public:
    /** Inventor class initialization */
    static void initClass();

	SoLocaleKit();

    SoLocaleKit( const SbName & name );

	virtual ~SoLocaleKit();

    /** 
     *the name of the locale. The node name is set automatically from this field
     * by augmenting it with the prefix LOCALE_.
     */
    SoSFName name;
    
    /** 
     * list of tracker stations that are working withing the locale. Therefore their
     * values need to be transformed with the inverse of the locales pose, before
     * send to all the other locales. 
     */
    SoMFInt32 internalStations;

    /**
     * signales whether the locale is shared or not 
     */
    SoSFBool shared;

protected:
    SoFieldSensor nameSensor;

    static void nameChanged( void * data, SoSensor * sensor );

    SoFieldSensor stationSensor;

    static void stationChanged( void * data, SoSensor * sensor );
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // ifndef _SOLOCALEKIT_H_
