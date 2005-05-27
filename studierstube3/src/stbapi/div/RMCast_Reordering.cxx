/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
//
// RMCast_Reordering.cpp,v 1.3 2000/10/11 00:57:08 coryan Exp
//

// avoids ace debug messages !
// can be used per file or for the whole project
#define ACE_NLOGGING

#include "RMCast_Reordering.h"
#include "RMCast_Proxy.h"
#include <ace/Message_Block.h>

ACE_RMCast_Reordering::~ACE_RMCast_Reordering (void)
{
}

int
ACE_RMCast_Reordering::close (void)
{
    Messages_Iterator i = this->messages_.begin ();
    Messages_Iterator end = this->messages_.end ();
    
    while (i != end)
    {
        ACE_Message_Block::release ((*i).item ().payload);
        this->messages_.unbind ((*i).key ());
        i = this->messages_.begin ();
    }
    return this->ACE_RMCast_Module::close ();
}

int
ACE_RMCast_Reordering::data (ACE_RMCast::Data &data)
{
    int must_ack = 0;
    int result = 0;
    ACE_RMCast::Ack ack;
    
    ACE_DEBUG ((LM_DEBUG, "Received message (%d) with %d, %d\n", data.sequence_number, this->next_expected_, this->highest_received_));
    {
        //    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
        
        if (data.sequence_number < this->next_expected_)
        {
            // Old message.  Ack with the current status (look at the end
            // of this block).
            // must_ack;
            
            // ACE_DEBUG ((LM_DEBUG, ".... old message is ignored\n"));
        }
        
        else if (data.sequence_number == this->next_expected_)
        {
            //ACE_DEBUG ((LM_DEBUG, ".... message is in order, received\n"));
            
            // Accept the message, the current thread will dispatch it, so
            // it is marked as accepted (using the <next_expected> field).
            // Any other thread will not push that message because now it
            // is "old".
            
            this->next_expected_++;
            
            // Right message, process as many messages as possible from
            // the queue, then ack the right level...
            
            // NOTE: we cannot release the mutex while dispatching
            // events, otherwise: how do we stop other threads from
            // delivering messages out of order?  I.E. what if the
            // next thread receives the next message?
            if (this->next () != 0)
            {
                result = this->next ()->data (data);
            }
            
            // After delivering one message there may be more messages
            // pending
            if (result == 0)
                result = this->push_queued_messages (& must_ack);
            
            //@@ This should be strategized, for example, only Ack if
            //   there is a message out of order or something, otherwise
            //   continue with happiness.  That works well for "optimistic
            //   models".
            // must_ack = 0;
        }
        
        else
        {
            //ACE_DEBUG ((LM_DEBUG, ".... message out of sequence, saved\n"));
            
            // Out of sequence.
            ACE_RMCast::Data new_data = data;
            new_data.payload = ACE_Message_Block::duplicate (data.payload);
            (void) this->messages_.bind (data.sequence_number, new_data);
            if( data.sequence_number < this->highest_received_ )
            {
                this->highest_received_ = data.sequence_number;
                // re-ack, otherwise save it and ack.
                must_ack = 1;
            }
            ACE_DEBUG ((LM_DEBUG, "message out of sequence , nr %d, highest %d\n", 
                data.sequence_number, this->highest_received_));
        }
        /*
        if (this->highest_received_ < data.sequence_number)
        {
        this->highest_received_ = data.sequence_number;
        }
        */    
        ack.next_expected = this->next_expected_;
        ack.highest_received = this->highest_received_;
    }
    
    //ACE_DEBUG ((LM_DEBUG, "must ack %d\n", must_ack ));
    if (must_ack && data.source != 0)
    {
        (void) data.source->reply_ack (ack);
        ACE_DEBUG ((LM_DEBUG, "Ack message (%d, %d)\n", ack.next_expected, ack.highest_received));
        /*
        ACE_Time_Value now = ACE_OS::gettimeofday();
        
          if((now - lastNack) > ACE_Time_Value( 0, 200000 ))
          {
          lastNack = now;
          //ACE_DEBUG ((LM_DEBUG, "Ack message (%d, %d)\n", ack.next_expected, ack.highest_received));	 
          
            }
        */
    }
    
    return result;
}

int
ACE_RMCast_Reordering::ack_join (ACE_RMCast::Ack_Join &ack_join)
{
    ACE_DEBUG ((LM_DEBUG, "RMCast_Reordering::ack_join - <%d,%d>\n",
               this->next_expected_,
                ack_join.next_sequence_number));
    int must_ack = 0;
    ACE_RMCast::Ack ack;
    
    {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
        if (this->next_expected_ >= ack_join.next_sequence_number)
        {
            // Nothing to do in this case...
            return 0;
        }
        
        Messages_Iterator i = this->messages_.begin ();
        Messages_Iterator end = this->messages_.end ();
        
        while (i != end
            && (*i).key () < ack_join.next_sequence_number)
        {
            ACE_Message_Block::release ((*i).item ().payload);
            this->messages_.unbind ((*i).key ());
            i = this->messages_.begin ();
        }
        
        this->next_expected_ = ack_join.next_sequence_number;
        if (this->highest_received_ < ack_join.next_sequence_number)
            this->highest_received_ = ack_join.next_sequence_number - 1;	
        
        this->push_queued_messages (&must_ack);	
        
        ack.next_expected = this->next_expected_;
        ack.highest_received = this->highest_received_;
        
    }
    
    if( must_ack && ack_join.source != 0)
    {
        (void) ack_join.source->reply_ack (ack);
        
    }
        
    return ACE_RMCast_Module::ack_join( ack_join );
}

int 
ACE_RMCast_Reordering::poll (ACE_RMCast::Poll & poll)
{
    if( poll.source != 0 )
    {		
        ACE_RMCast::Ack ack;
        ack.next_expected = this->next_expected_;
        ack.highest_received = this->highest_received_;
        
        // ACE_DEBUG ((LM_DEBUG, "Ack message (%d, %d) for poll\n", ack.next_expected, ack.highest_received));	 
        (void) poll.source->reply_ack (ack);		
    }
    
    return ACE_RMCast_Module::poll( poll );
}

int
ACE_RMCast_Reordering::push_queued_messages (int * must_ack)
{
    Messages_Iterator i = this->messages_.begin ();
    Messages_Iterator end = this->messages_.end ();
    
    while (i != end
        && (*i).key () == this->next_expected_)
    {
        int r = 0;
        if (this->next () != 0)
        {
            ACE_RMCast::Data data = (*i).item ();
            r = this->next ()->data (data);
        }
        
        ACE_Message_Block::release ((*i).item ().payload);
        this->messages_.unbind ((*i).key ());
        i = this->messages_.begin ();
        this->next_expected_++;
        if (r != 0)
            return r;
    }
    // if there is still something in the queue, we got a hole !
    if( this->messages_.current_size() > 0 )
    {
        if( this->highest_received_ < this->next_expected_ )  
        {
            this->highest_received_ = (*this->messages_.begin()).key();
            *must_ack = 1;
            ACE_DEBUG((LM_DEBUG, "a new hole at %d - %d\n", this->next_expected_, this->highest_received_));	  
        } 
    }
    else
    {
        this->highest_received_ = this->next_expected_ - 1;
    }
    return 0;
}

