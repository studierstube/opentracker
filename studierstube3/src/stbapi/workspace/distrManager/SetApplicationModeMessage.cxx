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
 * implementation of SetApplicationModeMessage
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SetApplicationModeMessage.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32  
#pragma warning(disable:4800) // warning about char to bool conversion
#endif

#include <stbapi/workspace/distrManager/SetApplicationModeMessage.h>
#include <stbapi/workspace/distrManager/SessionVisitor.h>
#include <stbapi/util/buffer.h>

SetApplicationModeMessage::SetApplicationModeMessage()
: SessionMessage( SessionVisitor::SET_APPLICATION_MODE )
, application("")
, master( false )
{

}

SetApplicationModeMessage::~SetApplicationModeMessage()
{

}

void SetApplicationModeMessage::execute( SessionVisitor & visitor )
{
    visitor.execute( *this );
}

Message * SetApplicationModeMessage::create( CBuffer * buffer )
{
    SetApplicationModeMessage * message = new SetApplicationModeMessage;
    message->readMessage( buffer );
    return message;
}

bool SetApplicationModeMessage::readMessage( CBuffer * buffer )
{
    if( !SessionMessage::readMessage( buffer ))
        return false;  
    char * help = buffer->getString();
    if( help == NULL )
        return false;
    else
        application = help;
    master = buffer->getChar();
    return true;
}

bool SetApplicationModeMessage::writeMessage( CBuffer * buffer )
{
    SessionMessage::writeMessage( buffer ); 
    buffer->addString((char *)application.c_str());
    buffer->addChar( master );
    return true;
}
