/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
//
// RMCast_IO_UDP.cpp,v 1.12 2000/12/20 22:00:33 oci Exp
//

#define ACE_NLOGGING

// #define DIV_DROP_TEST
#define DIV_FAILURE_RATE 0.5

#include "RMCast_IO_UDP.h"
#include "RMCast_UDP_Proxy.h"
#include "RMCast_Module_Factory.h"

#include <ace/ACE.h>
#include <ace/Handle_Set.h>
#include <ace/Reactor.h>
#include <ace/Message_Block.h>

ACE_RMCast_IO_UDP::ACE_RMCast_IO_UDP (ACE_RMCast_Module_Factory *factory)
:  factory_ (factory)
{
    int rc = ACE::get_ip_interfaces (addr_number, the_addr_array);
    if( rc != 0 )
    {
        ACE_DEBUG ((LM_DEBUG, "IO_UDP::constructor - error getting interfaces\n" ));
        addr_number = 0;
    }
}


ACE_RMCast_UDP_Proxy * ACE_RMCast_IO_UDP::allocate_and_bind_proxy (ACE_RMCast_Module *module,
                                            const ACE_INET_Addr& from_address)
{
  ACE_RMCast_UDP_Proxy *proxy;
  ACE_NEW_RETURN (proxy,
                  ACE_RMCast_UDP_Proxy(this,
                                       from_address),
                  0);
  proxy->next (module);

  if (this->map_.bind (from_address, proxy) != 0)
    {
      // @@ LOG??
      return 0;
    }

  return proxy;
}

ACE_RMCast_IO_UDP::~ACE_RMCast_IO_UDP (void)
{
    if( addr_number  != 0 )
        delete[] the_addr_array;
}

int
ACE_RMCast_IO_UDP::init (const ACE_INET_Addr &mcast_group,
                         const ACE_Addr &local,
                         const ACE_TCHAR *net_if,
                         int protocol_family,
                         int protocol,
                         int reuse_addr)
{	
    this->mcast_group_ = mcast_group;
    
    int rc = (this->mgram_.subscribe(mcast_group, reuse_addr, net_if )) | 
        (this->dgram_.open(local, protocol_family, protocol, reuse_addr));
    
    ACE_INET_Addr local_addr;
    this->dgram_.get_local_addr( local_addr );
        
    for( unsigned int i = 0; i < addr_number; i++ )
    {        
        the_addr_array[i].set_port_number( local_addr.get_port_number());
        ACE_DEBUG((LM_DEBUG,"interface %d : %s:%d\n", i, the_addr_array[i].get_host_name(),
            the_addr_array[i].get_port_number()));
    }

#ifdef DIV_DROP_TEST
    ACE_OS::srand((u_int)this);
#endif

    return rc;
}

int
ACE_RMCast_IO_UDP::handle_events (ACE_Time_Value *tv)
{
    ACE_HANDLE h = this->dgram_.get_handle ();
    if (h == ACE_INVALID_HANDLE)
        return -1;
    
    ACE_Handle_Set handle_set;
    handle_set.set_bit (h);
    
    ACE_Countdown_Time countdown (tv);
    
    int r = ACE_OS::select (int(h) + 1,
        handle_set, 0, 0,
        tv);
    if (r == -1)
    {
        if (errno == EINTR)
            return 0;
        else
            return -1;
    }
    else if (r == 0)
    {
        return 0;
    }
    
    return this->handle_input (h);
}

int
ACE_RMCast_IO_UDP::handle_input (ACE_HANDLE handle)
{
    // @@ We should use a system constant instead of this literal
    const int max_udp_packet_size = 65536;
    char buffer[max_udp_packet_size];
    ssize_t r;
    
    ACE_INET_Addr from_address;
    if( handle == this->dgram_.get_handle())
    {
        r = this->dgram_.recv (buffer, sizeof(buffer), from_address);
        // ACE_DEBUG ((LM_DEBUG, "IO_UDP::handle_input - data from datagram\n" ));
        
    }
    else
    {
        r = this->mgram_.recv (buffer, sizeof(buffer), from_address);
        // ACE_DEBUG ((LM_DEBUG, "IO_UDP::handle_input - data from multicast\n" ));
    }
    
    if (r == -1)
    {
        // @@ LOG??
        ACE_ERROR ((LM_ERROR,
            "RMCast_IO_UDP::handle_input () - error in recv %p\n",
            ACE_TEXT ("")));
        return 0;
    }
    
    // ACE_HEX_DUMP ((LM_DEBUG, buffer, 16, "Receiver::handle_input"));
    
    ACE_DEBUG((LM_DEBUG, "Packet from %s:%u\n", from_address.get_host_name(), from_address.get_port_number()));  
        
    for(unsigned int i = 0; i < this->addr_number; i++ )
    {
        if( from_address == this->the_addr_array[i] )
        {
            ACE_DEBUG((LM_DEBUG, "Threw packet away, because its coming from me %d!\n", i));
            return 0;
        }
    }


#ifdef DIV_DROP_TEST
    // test conditions for artificial packet drop rates
    double rv = (double)rand()/RAND_MAX;
    if( rv < DIV_FAILURE_RATE )
        return 0;
#endif
    // 

    // @@ Locking!
    
    int type = buffer[0];
    
    if (type < 0 || type >= ACE_RMCast::MT_LAST)
    {
        // @@ Log: invalid message type!!
        // @@ TODO: should we return -1?  The socket is still valid, it
        // makes little sense to destroy it just because one remote
        // sender is sending invalid messages. Maybe we should
        // strategize this too, and report the problem to the
        // application, this could indicate a misconfiguration or
        // something worse...
        
        // In any case the proxy should be destroyed, its peer is making
        // something really wrong.
        ACE_RMCast_UDP_Proxy *proxy;
        if (this->map_.unbind (from_address, proxy) == 0)
        {
            this->factory_->destroy (proxy->next ());
            delete proxy;
        }
        return 0;
    }
    
    ACE_RMCast_UDP_Proxy *proxy;
    if (this->map_.find (from_address, proxy) != 0)
    {
        //ACE_DEBUG ((LM_DEBUG,
        //            "IO_UDP::handle_input - new proxy from <%s:%d>\n",
        //            from_address.get_host_addr (),
        //            from_address.get_port_number ()));
        
        // @@ We should validate the message *before* creating the
        // object, all we need is some sort of validation strategy, a
        // different one for the receiver and another one for the
        // sender.
        
#if 0
        if (type == ACE_RMCast::MT_ACK
            || type == ACE_RMCast::MT_JOIN
            || type == ACE_RMCast::MT_LEAVE
            || type == ACE_RMCast::MT_ACK_LEAVE)
        {
            // All these message types indicate a problem, the should be
            // generated by receivers, not received by them.
            return 0;
        }
#endif /* 0 */
        ACE_RMCast_Module *module = this->factory_->create ();
        if (module == 0)
        {
            // @@ LOG??
            // Try to continue working, maybe the module can be created
            // later.
            return 0;
        }
        // This is necessary to satisfy the xgcc for Lynx on Solaris
        // by including the code directly causes :
        // RMCast_IO_UDP.cpp:202: error: internal error--unrecognizable insn:
        // (insn 1510 1507 524 (set (mem:SI (plus:SI (reg:SI 28 r28)
        //                 (const_int 65536)))
        //         (reg:SI 0 r0)) -1 (insn_list 528 (insn_list 1507 (nil)))
        //     (nil))
        // /usr/lynx/home2/jose/98r2/src/gcc/toplev.c:1489: Internal compiler error in function fatal_insn
        // to be thrown at the end of the function.
        if ((proxy = allocate_and_bind_proxy(module,from_address)) == 0)
            return 0;
    }
    
    // Have the proxy process the message and do the right thing.
    if (proxy->receive_message (buffer, r) != 0)
    {
        (void) this->map_.unbind (from_address);
        this->factory_->destroy (proxy->next ());
        delete proxy;
    }
    
    return 0;
}

ACE_HANDLE
ACE_RMCast_IO_UDP::get_handle_udp (void) const
{
    return this->dgram_.get_handle ();
}

ACE_HANDLE
ACE_RMCast_IO_UDP::get_handle_mcast (void) const
{
    return this->mgram_.get_handle ();
}

int
ACE_RMCast_IO_UDP::data (ACE_RMCast::Data &data)
{
    return this->send_data (data, this->mcast_group_);
}

int
ACE_RMCast_IO_UDP::poll (ACE_RMCast::Poll &poll)
{
    return this->send_poll (poll, this->mcast_group_);
}

int
ACE_RMCast_IO_UDP::ack_join (ACE_RMCast::Ack_Join &ack_join)
{
    return this->send_ack_join (ack_join, this->mcast_group_);
}

int
ACE_RMCast_IO_UDP::ack_leave (ACE_RMCast::Ack_Leave &ack_leave)
{
    return this->send_ack_leave (ack_leave, this->mcast_group_);
}

int
ACE_RMCast_IO_UDP::ack (ACE_RMCast::Ack &ack)
{
    return this->send_ack (ack, this->mcast_group_);
}

int
ACE_RMCast_IO_UDP::join (ACE_RMCast::Join &join)
{
    return this->send_join (join, this->mcast_group_);
}

int
ACE_RMCast_IO_UDP::leave (ACE_RMCast::Leave &leave)
{
    return this->send_leave (leave, this->mcast_group_);
}

int
ACE_RMCast_IO_UDP::send_data (ACE_RMCast::Data &data,
                              const ACE_INET_Addr &to)
{
    // ACE_DEBUG ((LM_DEBUG,
    //            "IO_UDP::send_data - pushing out to <%s:%d>\n",
    //            to.get_host_addr (),
    //            to.get_port_number ()));
    
    // The first message block contains the header
    // @@ TODO: We could keep the header pre-initialized, and only
    // update the portions that do change...
    ACE_UINT32 tmp;
    char header[1 + 3 * sizeof(ACE_UINT32)];
    header[0] = ACE_RMCast::MT_DATA;
    
    tmp = ACE_HTONL (data.sequence_number);
    ACE_OS::memcpy (header + 1,
        &tmp, sizeof(ACE_UINT32));
    tmp = ACE_HTONL (data.total_size);
    ACE_OS::memcpy (header + 1 + sizeof(ACE_UINT32),
        &tmp, sizeof(ACE_UINT32));
    tmp = ACE_HTONL (data.fragment_offset);
    ACE_OS::memcpy (header + 1 + 2 * sizeof(ACE_UINT32),
        &tmp, sizeof(ACE_UINT32));
    
    iovec iov[IOV_MAX];
    int iovcnt = 1;
    
    iov[0].iov_base = header;
    iov[0].iov_len = sizeof(header);
    
    ACE_Message_Block *mb = data.payload;
    
    for (const ACE_Message_Block *i = mb; i != 0; i = i->cont ())
    {
        iov[iovcnt].iov_base = i->rd_ptr ();
        iov[iovcnt].iov_len = i->length ();
        iovcnt++;
        if (iovcnt >= IOV_MAX)
            return -1;
    }
    
    // @@ This pacing stuff here reduced the number of packet lost in
    // loopback tests, but it should be taken out for real applications
    // (or at least made configurable!)
    // ACE_Time_Value tv (0, 100);
    // ACE_OS::sleep (tv);
    
    // ACE_SOCK_MCast_Dgram disallows sending, but it actually works.
    //  ACE_SOCK_Dgram &dgram = this->dgram_;
    
    if (dgram_.send (iov, iovcnt, to) == -1)
    {
        ACE_DEBUG((LM_DEBUG,"IO_UDP::send_data error sending %d blocks for %d\n", iovcnt, data.sequence_number));
		ACE_Time_Value tv (0, 10000);
		ACE_OS::sleep (tv);
        dgram_.send(iov,iovcnt,to);
    }
    
#if 0
    ACE_HEX_DUMP ((LM_DEBUG,
        (char*)iov[0].iov_base,
        iov[0].iov_len,
        "Sending"));
#endif
    
    return 0;
}

int
ACE_RMCast_IO_UDP::send_poll (ACE_RMCast::Poll &,
                              const ACE_INET_Addr &to)
{
    //ACE_DEBUG ((LM_DEBUG,
    //            "IO_UDP::send_poll - pushing out to <%s:%d>\n",
    //            to.get_host_addr (),
    //            to.get_port_number ()));
    
    // @@ TODO: We could keep the header pre-initialized, and only
    // update the portions that do change...
    char header[16];
    header[0] = ACE_RMCast::MT_POLL;
    
    // ACE_SOCK_MCast_Dgram disallows sending, but it actually works.
    //ACE_SOCK_Dgram &dgram = this->dgram_;
    
    if (dgram_.send (header, 1, to) == -1)
        return -1;
    
    return 0;
}

int
ACE_RMCast_IO_UDP::send_ack_join (ACE_RMCast::Ack_Join &ack_join,
                                  const ACE_INET_Addr &to)
{
    //ACE_DEBUG ((LM_DEBUG,
    //            "IO_UDP::send_ack_join - pushing out to <%s:%d>\n",
    //            to.get_host_addr (),
    //            to.get_port_number ()));
    
    // @@ TODO: We could keep the header pre-initialized, and only
    // update the portions that do change...
    char header[16];
    header[0] = ACE_RMCast::MT_ACK_JOIN;
    
    ACE_UINT32 tmp = ACE_HTONL (ack_join.next_sequence_number);
    ACE_OS::memcpy (header + 1,
        &tmp, sizeof(ACE_UINT32));
    // ACE_SOCK_MCast_Dgram disallows sending, but it actually works.
    //ACE_SOCK_Dgram &dgram = this->dgram_;
    
    if (dgram_.send (header, 1 + sizeof(ACE_UINT32), to) == -1)
        return -1;
    
    return 0;
}

int
ACE_RMCast_IO_UDP::send_ack_leave (ACE_RMCast::Ack_Leave &,
                                   const ACE_INET_Addr &to)
{
    //ACE_DEBUG ((LM_DEBUG,
    //            "IO_UDP::send_ack_leave - pushing out to <%s:%d>\n",
    //            to.get_host_addr (),
    //            to.get_port_number ()));
    
    // @@ TODO: We could keep the header pre-initialized, and only
    // update the portions that do change...
    char header[16];
    header[0] = ACE_RMCast::MT_ACK_LEAVE;
    
    // ACE_SOCK_MCast_Dgram disallows sending, but it actually works.
    //ACE_SOCK_Dgram &dgram = this->dgram_;
    
    if (dgram_.send (header, 1, to) == -1)
        return -1;
    
    return 0;
}

int
ACE_RMCast_IO_UDP::send_ack (ACE_RMCast::Ack &ack,
                             const ACE_INET_Addr &to)
{
    // ACE_DEBUG ((LM_DEBUG,
    //            "IO_UDP::send_ack - pushing (%d:%d) out to <%s:%d>\n",
    //            ack.next_expected,
    //            ack.highest_received,
    //            to.get_host_addr (),
    //            to.get_port_number ()));
    
    // @@ TODO: We could keep the header pre-initialized, and only
    // update the portions that do change...
    char header[16];
    header[0] = ACE_RMCast::MT_ACK;
    
    ACE_UINT32 tmp = ACE_HTONL (ack.next_expected);
    ACE_OS::memcpy (header + 1,
        &tmp, sizeof(ACE_UINT32));
    tmp = ACE_HTONL (ack.highest_received);
    ACE_OS::memcpy (header + 1 + sizeof(ACE_UINT32),
        &tmp, sizeof(ACE_UINT32));
    
    // ACE_SOCK_MCast_Dgram disallows sending, but it actually works.
    //ACE_SOCK_Dgram &dgram = this->dgram_;
    
    if (dgram_.send (header, 1 + 2*sizeof(ACE_UINT32), to) == -1)
        return -1;
    
    return 0;
}

int
ACE_RMCast_IO_UDP::send_join (ACE_RMCast::Join &,
                              const ACE_INET_Addr &to)
{
    // ACE_DEBUG ((LM_DEBUG,
    //            "IO_UDP::send_join - pushing out to <%s:%d>\n",
    //            to.get_host_addr (),
    //            to.get_port_number ()));
    
    // @@ TODO: We could keep the header pre-initialized, and only
    // update the portions that do change...
    char header[16];
    header[0] = ACE_RMCast::MT_JOIN;
    
    // ACE_SOCK_MCast_Dgram disallows sending, but it actually works.
    //ACE_SOCK_Dgram &dgram = this->dgram_;
    
    if (dgram_.send (header, 1, to) == -1)
        return -1;
    
    return 0;
}

int
ACE_RMCast_IO_UDP::send_leave (ACE_RMCast::Leave &,
                               const ACE_INET_Addr &to)
{
    // ACE_DEBUG ((LM_DEBUG,
    //            "IO_UDP::send_leave - pushing out to <%s:%d>\n",
    //            to.get_host_addr (),
    //            to.get_port_number ()));
    
    // @@ TODO: We could keep the header pre-initialized, and only
    // update the portions that do change...
    char header[16];
    header[0] = ACE_RMCast::MT_LEAVE;
    
    // ACE_SOCK_MCast_Dgram disallows sending, but it actually works.
    //ACE_SOCK_Dgram &dgram = this->dgram_;
    
    if (dgram_.send (header, 1, to) == -1)
        return -1;
    
    return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<ACE_INET_Addr,ACE_RMCast_UDP_Proxy*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr,ACE_RMCast_UDP_Proxy*,ACE_Hash<ACE_INET_Addr>,ACE_Equal_To<ACE_INET_Addr>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_INET_Addr,ACE_RMCast_UDP_Proxy*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr,ACE_RMCast_UDP_Proxy*,ACE_Hash<ACE_INET_Addr>,ACE_Equal_To<ACE_INET_Addr>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr,ACE_RMCast_UDP_Proxy*,ACE_Hash<ACE_INET_Addr>,ACE_Equal_To<ACE_INET_Addr>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr,ACE_RMCast_UDP_Proxy*,ACE_Hash<ACE_INET_Addr>,ACE_Equal_To<ACE_INET_Addr>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_INET_Addr,ACE_RMCast_UDP_Proxy*>;

template class ACE_Equal_To<ACE_INET_Addr>;
template class ACE_Hash<ACE_INET_Addr>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
