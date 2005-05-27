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
 * implementation of AddUserMessage
 *
 * @author Gerhard Reitmayr
 *
 * $Id: AddUserMessage.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <stbapi/workspace/distrManager/AddUserMessage.h>
#include <stbapi/workspace/distrManager/SessionVisitor.h>
#include <stbapi/util/buffer.h>

AddUserMessage::AddUserMessage()
: SessionMessage( SessionVisitor::ADD_USER )
{

}

AddUserMessage::~AddUserMessage()
{

}

void AddUserMessage::execute( SessionVisitor & visitor )
{
    visitor.execute( *this );
}

Message * AddUserMessage::create( CBuffer * buffer )
{
    AddUserMessage * message = new AddUserMessage;
    message->readMessage( buffer );
    return message;
}

bool AddUserMessage::readMessage( CBuffer * buffer )
{
    if( !SessionMessage::readMessage( buffer ))
        return false;  
    userId = buffer->getULong();
    char * help = buffer->getString();
    if( help != NULL )
        userKit = help;
    else
        return false;
    help = buffer->getString();
    if( help != NULL )
        locale = help;
    else
        return false;
    return true;
}

bool AddUserMessage::writeMessage( CBuffer * buffer )
{
    SessionMessage::writeMessage( buffer ); 
    buffer->addULong( userId );
    buffer->addString((char *) userKit.c_str());
    buffer->addString((char *) locale.c_str());
    return true;
}
