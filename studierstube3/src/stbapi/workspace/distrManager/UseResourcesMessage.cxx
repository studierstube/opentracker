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
 * implementation of UseResourcesMessage type
 *
 * @author Gerhard Reitmayr
 *
 * $Id: UseResourcesMessage.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <stbapi/workspace/distrManager/UseResourcesMessage.h>
#include <stbapi/workspace/distrManager/UserKitMessageVisitor.h>
#include <stbapi/util/buffer.h>

UseResourcesMessage::UseResourcesMessage()
: UserKitMessage( UserKitMessageVisitor::USE_RESOURCES )
{

}

UseResourcesMessage::~UseResourcesMessage()
{

}

void UseResourcesMessage::execute( UserKitMessageVisitor & visitor )
{
    visitor.execute( *this );
}

bool UseResourcesMessage::readMessage( CBuffer * buffer )
{
    if( !UserKitMessage::readMessage( buffer ))
        return false;
    char * help;
    if( (help = buffer->getString()) != NULL )
        appId = help;
    else 
        return false;
    if( (help = buffer->getString()) != NULL )
        windowName = help;
    else 
        return false;
    return true;
}

bool UseResourcesMessage::writeMessage( CBuffer * buffer )
{
    UserKitMessage::writeMessage( buffer ); 
    buffer->addString((char *) appId.c_str());
    buffer->addString((char *) windowName.c_str());
    return true;
}

Message * UseResourcesMessage::create( CBuffer * buffer )
{
    UseResourcesMessage * message = new UseResourcesMessage;
    message->readMessage( buffer );
    return message;
}
