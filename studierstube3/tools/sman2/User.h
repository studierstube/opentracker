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
 * User data type for storying user information
 *
 * @author Gerhard Reitmayr
 *
 * $Id: User.h 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _USER_H_
#define _USER_H_

#include <string>

/**
 * The User class stores information about a user kit that exists in a certain
 * locale. It is uniquely identified by the userId and stores its definition
 * as a string of OpenInventor ascii file format. This definition is posted
 * to new Studierstube instances joining a locale.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class User  
{
public:
	User();
    User( const User & );
	virtual ~User();

    /// defines equality by comparing the userId and userKit content
    const User & operator= ( const User & );

    /// stores the unique id of the user
    unsigned int userId;
    /// stores the definition of the UserKit
    std::string userKit;
};

#endif // _USER_H_
