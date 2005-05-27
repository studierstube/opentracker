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
/** header file for AddUserMessage
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: AddUserMessage.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _ADDUSERMESSAGE_H_
#define _ADDUSERMESSAGE_H_

#include <stbapi/workspace/distrManager/SessionMessage.h>
#include <string>

/**
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class AddUserMessage : public SessionMessage  
{
public:
    unsigned int userId;
    std::string userKit;
    std::string locale;

	AddUserMessage();
	virtual ~AddUserMessage();

    void execute( SessionVisitor & visitor );

    virtual bool readMessage( CBuffer * buffer );
    virtual bool writeMessage( CBuffer * buffer );

    static MessageFactoryFunction create;
};

#endif // _ADDUSERMESSAGE_H_
