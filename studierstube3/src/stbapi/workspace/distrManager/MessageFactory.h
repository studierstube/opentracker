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
 * header file for MessageFactory
 *
 * @author Gerhard Reitmayr
 *
 * $Id: MessageFactory.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */


#ifndef _MESSAGEFACTORY_H_
#define _MESSAGEFACTORY_H_

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <map>
#include <stbapi/workspace/distrManager/Message.h>

/**
 * The class MessageFactory provides a way to construct a number of messages
 * from a given char data array. Typically such an array is read from a 
 * stream or contents of a datagram and needs to be parsed into a concrete
 * Message object. The MessageFactory stores a number of MessageFactoryFunctions
 * and the type identifiers of the corresponding messages to delegate the
 * construction to the correct MessageFactoryFunction. Therefore, for
 * each message used in the system a corresponding MessageFactoryFunction
 * has to be implemented and registered with the MessageFactory object.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class MessageFactory  
{
    typedef std::map<short, MessageFactoryFunction *> MessageFactoryMap;

public:
    /// constructor
	MessageFactory();
    /// destructor
	virtual ~MessageFactory();

    /**
     * adds a new MessageFactoryFunction indexed by the given type identifier.
     * It will replace any function stored before under this identifier. It 
     * will do nothing if passed a NULL pointer.
     * @param type the Message type identifier
     * @param function pointer to the factory function
     */
    void addMessageFactory( short type, MessageFactoryFunction * function );
    /**
     * removes the MessageFactoryFunction indexed by the passed type identifier.
     * @param type the Message type identifier
     */
    void removeMessageFactory( short type );

    /**
     * parses a char data buffer and creates a new Message object containing
     * the message. It uses one of the stored MessageFactoryFunctions to 
     * construct the right class of Message. First it reads in a type
     * identifier from the data buffer. Then it resets it and uses the
     * corresponding factory function to create the message. If this
     * fails or there is no such function, it will return NULL.
     * @param data pointer to the data buffer
     * @param length number of bytes in the data buffer
     */
    Message * createMessage( const char * data, unsigned int length );

protected:
    /// map to store MessageFactoryFunctions indexed by the type identifier.
    MessageFactoryMap factories;
};

#endif // _MESSAGEFACTORY_H_
