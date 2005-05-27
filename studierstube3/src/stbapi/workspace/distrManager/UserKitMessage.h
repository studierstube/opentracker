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
/**
 * abstract message type for all UserKitMessages.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: UserKitMessage.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _USERKITMESSAGE_H_
#define _USERKITMESSAGE_H_

#include <stbapi/workspace/distrManager/SessionMessage.h>
#include <stbapi/workspace/distrManager/SessionVisitor.h>

class UserKitMessageVisitor;

/**
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class UserKitMessage : public SessionMessage  
{
public:
    unsigned int userId;

    UserKitMessage( short type = SessionVisitor::USERKIT_PARAM );
	virtual ~UserKitMessage();

    void execute(SessionVisitor &visitor);

    virtual void execute(UserKitMessageVisitor &visitor) = 0;

    virtual bool readMessage( CBuffer * buffer );
    virtual bool writeMessage( CBuffer * buffer );
};

#endif // _USERKITMESSAGE_H_
