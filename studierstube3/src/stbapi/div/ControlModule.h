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
/** header file for ACE RMCast control module for interfacing with DIV
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ControlModule.h 3504 2004-07-20 11:34:20Z tomp $
 * @file                                                                   */
/* ======================================================================= */

#include <ace/Message_Queue.h>
#include <ace/INET_Addr.h>
#include <ace/Reactor_Notification_Strategy.h> 
#include <stbapi/div/RMCast_UDP_Reliable_Transceiver.h>
#include <stbapi/div/RMCast_UDP_Proxy.h>
#include <stbapi/util/ReactorTask.h>

// our message queue is mutex'ed
typedef ACE_Message_Queue<ACE_MT_SYNCH> MessageQueue;

class CDivMain;
class ControlModule;

/**
 * This class is the superclass for the InputMessageHandler and OutputMessageHandler.
 * It exposes the contained MessageQueue and stores a flag to notify the handle_input
 * method to return appropriatly to close the queue and deregister the MessageHandler
 * from the ACE Reactor.
 *
 * @author Gerhard Reitmayr
 * @ingroup div
 */
class MessageHandler : public ACE_Event_Handler 
{    
protected:
    MessageQueue queue;
    ACE_Reactor_Notification_Strategy strategy;
    bool closing;
    
public :
    MessageHandler (ACE_Reactor * reactor) 
        : strategy( reactor, this, ACE_Event_Handler::READ_MASK), closing( false )
    {
        queue.notification_strategy( & strategy );
    };
    
    virtual ~MessageHandler(void);
    
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);
    
    MessageQueue * getQueue()
    {
        return & queue;
    };
    
    void close(void)
    {
        closing = true;
    };
    
};

/** 
 * This class interfaces to the ACE framework and stores a queue of
 * div messages received by the protocol stack. The parent ControlModule
 * stores any received data messages in this queue. Then is is 
 * processed from the OpenInventor main thread to update local copies
 * of shared scene graphs.
 *
 * @author Gerhard Reitmayr
 * @ingroup div
 */
class InputMessageHandler : public MessageHandler 
{
protected:
    ControlModule * control;
public:
    InputMessageHandler(ControlModule * control_, ACE_Reactor * reactor) 
        : MessageHandler( reactor ), control( control_ )
    {
    }
    
    virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
};

/** 
 * This class interfaces to the ACE framework and stores a queue of
 * div messages to be send over the network. Messages are stored by
 * the send_data method of the parent ControlModule object. The
 * dedicated DIV thread then retrieves these messages and sends them
 * over the network.
 *
 * @author Gerhard Reitmayr
 * @ingroup div
 */
class OutputMessageHandler : public MessageHandler {
protected:
    ControlModule * control;

public:
    OutputMessageHandler(ControlModule * control_, ACE_Reactor * reactor) 
        : MessageHandler(reactor), control( control_ )
    {
    }
    
    virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
};

/**
 * This class handles the distribution of DIV packages via the reliable
 * multicast protocol. It provides the interface between the DIV implementation
 * in CDivMain and the actual network protocol implementation. It also compresses
 * and decompresses large messages.
 *
 * The network stack operates in a dedicated thread which is shared between all
 * CDivMain instances. An instance of OutputMessageHandler acts as output queue
 * to this thread and an instance of InputMessageHandler as input message queue from
 * this thread.
 *
 * @author Gerhard Reitmayr
 * @ingroup div
 */
class ControlModule : public ACE_RMCast_Module
{
public:
    ControlModule (CDivMain * div_, const ACE_INET_Addr * mgroup, bool compress_ = true,  const ACE_TCHAR* nic = 0);
    
    virtual ~ControlModule();
    
    virtual int close (void);
    virtual int data (ACE_RMCast::Data &data);
    virtual int ack_join (ACE_RMCast::Ack_Join &ack_join);
    virtual int ack_leave (ACE_RMCast::Ack_Leave &ack_leave);
    virtual int poll (ACE_RMCast::Poll &);
    virtual int ack (ACE_RMCast::Ack &);
    virtual int join (ACE_RMCast::Join &);
    virtual int leave (ACE_RMCast::Leave &);
    
    enum State { ACTIVE, LEAVING } state;
    
    void send_data( const char * data, unsigned long len );

protected:
    ACE_RMCast_UDP_Reliable_Transceiver rmcast;
    CDivMain * div;
    InputMessageHandler * in;
    OutputMessageHandler * out;
    bool useCompression;

    struct CompressionInfo {
        unsigned short command;
        unsigned long size;
    };

    void compress( ACE_Message_Block  * & payload );
    void uncompress( ACE_Message_Block * & payload );

    char * compressionBuffer;
    unsigned long compressionSize;

    friend class OutputMessageHandler;
    friend class InputMessageHandler;
};
