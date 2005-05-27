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
/** implementation of MessageFactory
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: MessageFactory.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include "MessageFactory.h"
#include <stbapi/util/buffer.h>

MessageFactory::MessageFactory()
{

}

MessageFactory::~MessageFactory()
{

}

void MessageFactory::addMessageFactory( short type, MessageFactoryFunction * function )
{
    if( function == NULL )
        return;
    factories[type] = function;
}

void MessageFactory::removeMessageFactory( short type )
{
    factories.erase( type );
}

Message * MessageFactory::createMessage(const char * data, unsigned int length )
{    
    CBuffer in((void *)data, length);
    short type = in.getShort();
    // resets the buffer to let the message read it once more.
    in.setBuffer((void *)data, length);

    MessageFactoryMap::iterator it = factories.find( type );
    if( it != factories.end())
    {
        Message * message = (*it).second( &in );
        return message;
    }
    return NULL;
}
