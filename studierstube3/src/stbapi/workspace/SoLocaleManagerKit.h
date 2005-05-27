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
/** header file for SoLocaleManagerKit
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoLocaleManagerKit.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOLOCALEMANAGERKIT_H_
#define _SOLOCALEMANAGERKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <stbapi/stbapi.h>

// forward class declarations
class SoLocaleKit;
class SoUserKit;
class SoWindowKit;

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The general manager class for locales. It manages the locale data structures,
 * adds and removes parts of locales such as windows (or contexts) and userKits.
 * It transforms 3d events passing through depending on the locale they are
 * attributed to.
 * 
 * @author Gerhard Reitmayr
 * @ingroup workspace
 */
class STBAPI_API SoLocaleManagerKit : public SoBaseKit
{
    SO_KIT_HEADER( SoLocaleManagerKit );
	SO_KIT_CATALOG_ENTRY_HEADER( visibleLocales );

public:
	/// static OpenInventor initialization
	static void initClass();
	/// constructor
	SoLocaleManagerKit();
	/// destructor
	virtual ~SoLocaleManagerKit();

	/** 
	 * returns the singleton instance of the SoLocaleManagerKit. Each 
	 * Studierstube process sports on of these. If no singleton is present yet
	 * it creates a new one and returns it.
	 * @return pointer to the singleton instance. 
	 */
	static SoLocaleManagerKit * getInstance();

	/** 
	 * creates a new locale with the given name and returns it. It creates the actual node
	 * and adds it to the correct part. This depends on the locale being
	 * visible or not. If a locale with the same name is already present, 
	 * no new locale is created, but the existing one is returned.
	 * @param name the unique name of the new locale
	 */
	SoLocaleKit * createLocale( const SbName & name );

	/**
	 * destroys the locale specified by given name. If no locale of the given
	 * name exists, nothing happens. 
     * @param name the name of the locale to be destroyed
	 * @note When the locale is destroyed any children of the locale might be
	 * destroyed as well, if their reference counters go to zero.
	 */
	void destroyLocale( const SbName & name );

	/** 
	 * returns a locale specified by a given name. It returns NULL
	 * if no such locale is known.
	 * @param name the name of the locale
	 * @return the sought after locale
	 */
	SoLocaleKit * getLocale( const SbName & name );

	/** 
	 * returns the locale a given SoNode is in. It returns NULL
	 * if no such locale was found.
	 * @param node the SoNode to look for. 
	 * @return the locale that contains the node 
	 */
    SoLocaleKit * getLocaleFromNode( SoNode * node );

	/** 
	 * adds a given SoUserKit to a locale. In this version the locale
	 * is specified by a pointer to it. If the SoUserKit is already present
	 * in the specified locale nothing happens. 
	 * @note It is not checked whether the SoUserKit is already present
	 * in another locale !!
	 * @param userKit the SoUserKit to add
	 * @param locale the locale to add it to
	 */
	void addUserKit( SoUserKit * userKit, SoLocaleKit * locale);

	/** 
	 * adds a given SoUserKit to a locale. In this version the locale
	 * is specified its name. If the SoUserKit is already present
	 * in the specified locale nothing happens. 
	 * @note It is not checked whether the SoUserKit is already present
	 * in another locale !!
	 * @param userKit the SoUserKit to add
	 * @param name the name of the locale to add it to
	 */
	void addUserKit( SoUserKit * userKit, const SbName & name );

	/** 
	 * removes an SoUserKit from its locale. This assumes the SoUserKit
	 * is only present in one locale and it is resumed from that one.
	 * If the SoUserKit is not present in any locale, nothing happens.
	 * @param userKit pointer to the SoUserKit to remove.
	 */
	void removeUserKit( SoUserKit * userKit);

    /** 
	 * adds a given SoWindowKit to a locale. In this version the locale
	 * is specified by a pointer to it. If the SoWindowKit is already present
	 * in the specified locale nothing happens. 
	 * @note It is not checked whether the SoWindowKit is already present
	 * in another locale !!
	 * @param appGroup the SoWindowKit to add
	 * @param locale the locale to add it to
	 */
	void addApplicationKit( SoGroup * appGroup, SoLocaleKit * locale);

	/** adds a given SoWindowKit to a locale. In this version the locale
	 * is specified by its name. If the SoWindowKit is already present
	 * in the specified locale nothing happens. 
	 * @note It is not checked whether the SoWindowKit is already present
	 * in another locale !!
	 * @param appGroup the SoWindowKit to add
	 * @param name the name of the locale to add it to
	 */
	void addApplicationKit( SoGroup * appGroup, const SbName & name);

	/** 
	 * removes an SoWindowKit from its locale. This assumes the SoWindowKit
	 * is only present in one locale and it is resumed from that one.
	 * If the SoWindowKit is not present in any locale, nothing happens.
	 * @param appGroup pointer to the application group to remove.
	 */
	void removeApplicationKit( SoGroup * appGroup);	

protected :
	/// pointer to the singleton instance
	static SoLocaleManagerKit * instance;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // !defined _SOLOCALEMANAGERKIT_H_
