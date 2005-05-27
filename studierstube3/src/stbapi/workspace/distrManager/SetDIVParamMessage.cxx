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
 * implementation of SetDIVParamMessage
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SetDIVParamMessage.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32  
#pragma warning(disable:4800) // warning about char to bool conversion
#endif

#include <stbapi/workspace/distrManager/SetDIVParamMessage.h>
#include <stbapi/workspace/distrManager/SessionVisitor.h>
#include <stbapi/util/buffer.h>

SetDIVParamMessage::SetDIVParamMessage()
: SessionMessage( SessionVisitor::SET_DIV_PARAM )
, application("")
, active( false )
, initFromMaster( false )
, multicastAddress("")
, port(0)
{

}

SetDIVParamMessage::~SetDIVParamMessage()
{

}

void SetDIVParamMessage::execute( SessionVisitor & visitor )
{
    visitor.execute( *this );
}

Message * SetDIVParamMessage::create( CBuffer * buffer )
{
    SetDIVParamMessage * message = new SetDIVParamMessage;
    message->readMessage( buffer );
    return message;
}


bool SetDIVParamMessage::readMessage( CBuffer * buffer )
{
    if( !SessionMessage::readMessage( buffer ))
        return false;
    char * help;
    if( (help = buffer->getString()) != NULL )
        application = help;
    else
        return false;
    active = buffer->getChar();
    master = buffer->getChar();
    initFromMaster = buffer->getChar();
    if( (help = buffer->getString()) != NULL )
        multicastAddress = help;
    else 
        return false;
    port = buffer->getULong();
    return true;

}

bool SetDIVParamMessage::writeMessage( CBuffer * buffer )
{
    SessionMessage::writeMessage( buffer );
    buffer->addString((char *)application.c_str());
    buffer->addChar( active );
    buffer->addChar( master );
    buffer->addChar( initFromMaster );    
    buffer->addString((char *) multicastAddress.c_str());
    buffer->addULong( port );
    return true;
}
