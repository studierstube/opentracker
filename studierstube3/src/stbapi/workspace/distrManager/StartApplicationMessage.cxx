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
 * implementation of StartApplicationMessage
 *
 * @author Gerhard Reitmayr
 *
 * $Id: StartApplicationMessage.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4800) // warning about char to bool conversion
#endif

#include <stbapi/workspace/distrManager/StartApplicationMessage.h>
#include <stbapi/workspace/distrManager/SessionVisitor.h>
#include <stbapi/util/buffer.h>

using namespace std;

StartApplicationMessage::StartApplicationMessage()
: SessionMessage( SessionVisitor::START_APPLICATION )
, userID(0)
, application("")
, locale("")
, name("")
, attach( false )
{

}

StartApplicationMessage::~StartApplicationMessage()
{
}

void StartApplicationMessage::execute( SessionVisitor & visitor )
{
    visitor.execute( *this );
}

Message * StartApplicationMessage::create( CBuffer * buffer )
{
    StartApplicationMessage * message = new StartApplicationMessage;
    message->readMessage( buffer );
    return message;
}

bool StartApplicationMessage::readMessage( CBuffer * buffer )
{
    SessionMessage::readMessage( buffer );
    char * help;
    if( (help = buffer->getString()) != NULL )
        application = help;
    else 
        return false;
    if( (help = buffer->getString()) != NULL )
        locale = help;
    else
        return false;
    if( (help = buffer->getString()) != NULL )
        name = help;
    else
        return false;    
    userID = buffer->getULong();
    attach = buffer->getChar();
    return true;

}

bool StartApplicationMessage::writeMessage( CBuffer * buffer )
{
    SessionMessage::writeMessage( buffer );
    buffer->addString((char *) application.c_str());
    buffer->addString((char *) locale.c_str());
    buffer->addString((char *) name.c_str());
    buffer->addULong( userID );
    buffer->addChar( attach );
    return true;
}
