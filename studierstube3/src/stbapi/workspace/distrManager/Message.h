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
/** header file for Message class
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Message.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

class CBuffer;

/**
 * The class Message is the base class for all messages used to communicate
 * between the SessionManager and any Studierstube clients. It provides
 * the basic interface to query a message type and to marshal and
 * demarshal the message into a CBuffer.
 *
 * Every subclass needs to define a unique type identifier for it. These
 * need to be unique within the system or application that uses these
 * Message types. Moreover it has to override the two methods
 * readMessage and writeMessage to provide correct marshalling of the Message.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class Message
{
public:
    Message( short type_ = -1 ) : type(type_) {};
	Message( CBuffer * buffer );
	virtual ~Message();

    /// returns the type of the message.
    short getType() const
    {
        return type;
    };

    /**
     * reads the content of a CBuffer and tries to set the message
     * data to the CBuffer contents. If successful it returns true
     * otherwise false. This implementation reads only the type
     * identifier to the buffer. Any other message implementation
     * needs to call the superclass implementation first.
     * @param buffer pointer to the CBuffer containing the message
     */
    virtual bool readMessage( CBuffer * buffer );
    /**
     * writes the message data  to the CBuffer contents. If successful it returns true
     * otherwise false. This implementation writes only the type
     * identifier to the buffer. Any other message implementation
     * needs to call the superclass implementation first.
     * @param buffer pointer to the CBuffer containing the message
     */
    virtual bool writeMessage( CBuffer * buffer );

protected:
    /// the type identifier
    short type;
};

/**
 * defines a function that tries to construct a message from
 * a CBuffer. This type is used by the MessageFactory to build
 * a message from a CBuffer.
 * @see MessageFactory
 */
typedef Message * MessageFactoryFunction( CBuffer * );

#endif // _MESSAGE_H_
