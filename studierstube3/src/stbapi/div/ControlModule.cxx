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
 * source file for the ACE RMCast ControlModule
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ControlModule.cxx 4150 2005-04-01 09:35:42Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// comment this out to get a lot of debugging messages
#define ACE_NLOGGING

#include <stbapi/div/ControlModule.h>
#include <stbapi/div/divmain.h>
#include <stbapi/util/buffer.h>
#ifdef WIN32
#include <../zlib/zlib.h>
#else
#include <zlib.h>
#endif

/**
 * All the DIV networking stuff is managed in one thread per process. This
 * thread runs an ACE Reactor to handle incoming and outgoing data. The
 * following pointer holds the singleton and is initiated by the first
 * DIV object.
 */
ReactorTask * divTask = NULL;

// message handler implementations

MessageHandler::~MessageHandler(void)
{
}

int MessageHandler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
    queue.notification_strategy( NULL );
    reactor()->purge_pending_notifications( this );
    delete this; //commit suicide
    return 0;
}

int InputMessageHandler::handle_input (ACE_HANDLE fd)
{
    if( !closing )
    {
        ACE_Message_Block * mb;
        int val;
        while( (!queue.is_empty()) && (val = queue.dequeue_head( mb )) >= 0 && !closing )
        {
            control->div->networkProcessing( mb->rd_ptr(), mb->length());
            mb->release();
            if( val == 0 )
                break;
            reactor()->purge_pending_notifications( this );
        }
        if( val == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "Error retrieving message block from queue in div %x\n", &div));
        }
    }
    return closing?-1:0;
}

int OutputMessageHandler::handle_input (ACE_HANDLE fd)
{
    if( !closing )
    {
        ACE_Message_Block * mb;
        int val;
        while( (!queue.is_empty()) && (val = queue.dequeue_head( mb )) >= 0 && !closing)
            {
            ACE_DEBUG((LM_DEBUG,"%T:OutputMessages: before block compressing\n"));
            control->compress( mb );
            ACE_DEBUG((LM_DEBUG,"%T:OutputMessages: block compressed !\n"));
            ACE_RMCast::Data packet;
            packet.payload = mb;
            if(control->rmcast.data (packet) != 0)
            {
                ACE_DEBUG((LM_DEBUG,"OutputMessages : error sending data !\n"));
            }
            ACE_DEBUG((LM_DEBUG,"OutputMessages:sent packet %T, %d, %d\n", packet.sequence_number, packet.total_size ));
            mb->release();
            if( val == 0 )
                break;
            reactor()->purge_pending_notifications( this );
        }
        if( val == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "Error retrieving message block from queue in div %x\n", &div));
        }
    }
    return closing?-1:0;
}

// implementations for ControlModule RMCast module

ControlModule::ControlModule (CDivMain * div_, const ACE_INET_Addr * mgroup, bool compress_, const ACE_TCHAR* nic)
: state( ACTIVE )
, rmcast( this )
, div(div_)
, useCompression( compress_ )
{
    if( divTask == NULL )
        divTask = new ReactorTask( true );

    in = new InputMessageHandler( this, ACE_Reactor::instance());
    out = new OutputMessageHandler( this, divTask->getReactor());

    compressionBuffer = new char[2048];
	compressionSize = 2048;

	if (rmcast.init(*mgroup, nic) == -1)
    {
        ACE_DEBUG((LM_DEBUG,"ControlModule : cannot open multicast group for %s:%d\n",
            mgroup->get_host_addr(), mgroup->get_port_number()));
		return;
    }

    rmcast.reactive_incoming_messages (divTask->getReactor());
    // Probably tuneable : send polls regularely (every 5th second)
    rmcast.reactive_resends (divTask->getReactor(), ACE_Time_Value( 5, 0));

    // send a single poll to signal existence to others
    ACE_RMCast::Poll poll;
    rmcast.io_udp_.poll( poll );
    // send a single join to immediatly join the group
    ACE_RMCast::Join join;
    rmcast.io_udp_.join( join );
}

ControlModule::~ControlModule()
{
    if( compressionBuffer != NULL )
        delete compressionBuffer;
}

int ControlModule::close (void)
{
    if( state == LEAVING )
        return 0;
    state = LEAVING;
    in->close();
    out->close();
    rmcast.close();
    ACE_DEBUG((LM_DEBUG,"ControlModule::close - connection closed !\n"));
    return this->ACE_RMCast_Module::close ();
}

int ControlModule::data (ACE_RMCast::Data &data)
{
    ACE_DEBUG((LM_DEBUG,"ControlModule::data %T, %d, %d\n", data.sequence_number, data.payload->total_size()));
    if( state == LEAVING )
        return -1;
    ACE_Message_Block * mb = data.payload->duplicate();
    uncompress( mb );
    in->getQueue()->enqueue_tail( mb );
    return 0;
}

int ControlModule::ack_join (ACE_RMCast::Ack_Join &ack_join)
{
    ACE_DEBUG((LM_DEBUG,"ControlModule::ack_join - joined multicast group !\n"));
    if( state == LEAVING )
        return -1;
/*
    CBuffer buffer;
    buffer.addShort(CDivMain::DIV_JOINED);

    ACE_Message_Block * payload = new ACE_Message_Block(buffer.getSize());
    payload->copy((const char *) buffer.getBuffer(), buffer.getSize());
    in->getQueue()->enqueue_tail( payload );
*/
    return 0;
}

int ControlModule::ack_leave (ACE_RMCast::Ack_Leave &ack_leave)
{
    ACE_DEBUG((LM_DEBUG,"ControlModule::ack_leave - left multicast group !\n"));
    return 0;
}

int ControlModule::poll (ACE_RMCast::Poll &poll)
{
    // ACE_DEBUG((LM_DEBUG,"ControlModule::poll - from %s:%d\n",
    //           (((ACE_RMCast_UDP_Proxy *)poll.source)->peer_addr()).get_host_name(),
    //           (((ACE_RMCast_UDP_Proxy *)poll.source)->peer_addr()).get_port_number()));
    if( state == LEAVING )
        return -1;
/*
    CBuffer buffer;
    buffer.addShort(CDivMain::DIV_HEARTBEAT);

    ACE_Message_Block * payload = new ACE_Message_Block(buffer.getSize());
    payload->copy((const char *) buffer.getBuffer(), buffer.getSize());
    in->getQueue()->enqueue_tail( payload );
*/
    return 0;
}

int ControlModule::ack (ACE_RMCast::Ack &ack)
{
    /* ACE_DEBUG((LM_DEBUG,"ControlModule::ack %d,%d - from %s:%d\n",
               ack.next_expected, ack.highest_received,
               (((ACE_RMCast_UDP_Proxy *)ack.source)->peer_addr()).get_host_name(),
               (((ACE_RMCast_UDP_Proxy *)ack.source)->peer_addr()).get_port_number() ));
    */
    return 0;
}

int ControlModule::join (ACE_RMCast::Join &join)
{
    ACE_DEBUG((LM_DEBUG,"ControlModule::join from %s:%d\n",
               (((ACE_RMCast_UDP_Proxy *)join.source)->peer_addr()).get_host_name(),
               (((ACE_RMCast_UDP_Proxy *)join.source)->peer_addr()).get_port_number() ));
    return 0;
}

int ControlModule::leave (ACE_RMCast::Leave &leave)
{
    ACE_DEBUG((LM_DEBUG,"ControlModule::leave from %s:%d\n",
               (((ACE_RMCast_UDP_Proxy *)leave.source)->peer_addr()).get_host_name(),
               (((ACE_RMCast_UDP_Proxy *)leave.source)->peer_addr()).get_port_number() ));
    return 0;
}

void ControlModule::send_data( const char * data, unsigned long len )
{
    ACE_Message_Block * payload;

    payload = new ACE_Message_Block(len);
    payload->copy( data, len );
    out->getQueue()->enqueue_tail( payload );

    ACE_DEBUG((LM_DEBUG,"Send data %d - still %d\n", len, rmcast.count_blocks() ));
}

void ControlModule::compress( ACE_Message_Block * & payload )
{
    CompressionInfo compressionInfo;
    unsigned long len = payload->total_length();
    if( useCompression && len > (1024 - ( 1 + 3*sizeof( int ))))  // the limit for the RMCast fragment module
    {
        unsigned long requiredSize = (unsigned long)(len * 1.001 + 13); // this is given by zlib
        if( requiredSize >= compressionSize )
        {
            while( compressionSize <= requiredSize )
                compressionSize *= 2;
		    delete compressionBuffer;
        	compressionBuffer = NULL;
            compressionBuffer = new char[compressionSize];
        }
		if (compressionBuffer == NULL) {
            ACE_DEBUG((LM_ERROR,"ERROR: ControlModule: Error creating compressionBuffer with [ %lu bytes\n", compressionSize ));
			exit(0);
		}
        unsigned long finalSize = compressionSize;
        if( compress2((unsigned char *) compressionBuffer, &finalSize, (const unsigned char *)payload->rd_ptr(),
            len, Z_BEST_SPEED ) != Z_OK )
        {  // something went wrong, we send it uncompressed ...
            return;
        }
        else
        {
            compressionInfo.command = htons( CDivMain::DIV_COMPRESSED );
            compressionInfo.size = htonl( len );
            payload->release();
            payload = new ACE_Message_Block( sizeof( compressionInfo ));
            payload->copy( (const char *) &compressionInfo, sizeof( compressionInfo ));
            ACE_Message_Block * cont = new ACE_Message_Block( finalSize );
            cont->copy( compressionBuffer, finalSize );
            payload->cont(cont);

            ACE_DEBUG((LM_DEBUG,"ControlModule::send_data compressed to %f\n", (float)finalSize/len));
            ACE_DEBUG((LM_DEBUG,"    orig size %d, final size %d, block size %d \n",
                len, finalSize, payload->total_length()));
        }
    }
}

void ControlModule::uncompress( ACE_Message_Block * & payload )
{
    CompressionInfo * compInfo;

    compInfo = (CompressionInfo *) payload->rd_ptr();
    compInfo->command = ntohs( compInfo->command );
    if( compInfo->command == CDivMain::DIV_COMPRESSED )
    {
        ACE_DEBUG((LM_DEBUG,"ControlModule::data got compressed data\n"));
        compInfo->size = ntohl( compInfo->size );
        if( compressionSize < compInfo->size )
        {
            while( compressionSize < compInfo->size )
                compressionSize *= 2;
            delete compressionBuffer;
            compressionBuffer = new char[compressionSize];
        }
        // payload->rd_ptr( sizeof( CompressionInfo ));
        unsigned long finalSize = compressionSize;
        ACE_DEBUG((LM_DEBUG,"    comp size %d, data left %d \n", compInfo->size, payload->total_length()));
        if( Z_OK != ::uncompress((unsigned char *)compressionBuffer, &finalSize,
            (const unsigned char *)(payload->rd_ptr()+sizeof( CompressionInfo )), compInfo->size ))
        {
            ACE_DEBUG((LM_ERROR,"ControlModule::data error decompressing input !\n"));
            // payload->rd_ptr( -sizeof( CompressionInfo ));
            return;
        }
        else
        {
            ACE_DEBUG((LM_DEBUG,"ControlModule::data decompressed data at %f\n", (float)compInfo->size/payload->total_length() ));
        }
        payload->release();
        payload = new ACE_Message_Block( finalSize );
        payload->copy( compressionBuffer, finalSize );
    }
    else
    {
        compInfo->command = htons( compInfo->command );
    }
}
