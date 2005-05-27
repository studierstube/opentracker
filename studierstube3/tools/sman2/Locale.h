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
 * PROJECT: sman2
 * ======================================================================== */
/**
 * header file for data structure class Locale
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Locale.h 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _LOCALE_H_
#define _LOCALE_H_

#include <set>
#include <string>
#include <map>
#include "Application.h"
#include "User.h"

/**
 * The class Locale stores information about a single locale known to the
 * SessionManager. It keeps track of the applications and users that are 
 * part of the locale.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class Locale  
{
public:
    /// constructor
    Locale();
    /// destructor
	virtual ~Locale();

    /// station number used to track the locale
  	int station;
    /// the unique name of the locale
    std::string name;
    
    /// set of applications that are part of the locale.
    std::set<Application *> applications;
    /** map of users that are part of the locale. It
     * maps from the user ids to strings containing the
     * SoUserKit definition of that user. These are
     * send to new clients to update them about these users.
     */
    std::map<unsigned int, User> users;
};

#endif // _LOCALE_H_
