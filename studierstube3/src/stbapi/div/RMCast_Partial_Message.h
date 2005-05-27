/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
// RMCast_Partial_Message.h,v 1.3 2000/10/02 18:40:02 coryan Exp

// ============================================================================
//
// = DESCRIPTION
//   Helper class used in the reassembly layer of the realiable
//   multicast library.
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_PARTIAL_MESSAGE_H
#define ACE_RMCAST_PARTIAL_MESSAGE_H

#include <stbapi/stbapi.h>
#include <ace/Task.h>

#ifndef ACE_RMCAST_DEFAULT_HOLE_COUNT
#define ACE_RMCAST_DEFAULT_HOLE_COUNT 16
#endif /* ACE_RMCAST_DEFAULT_HOLE_COUNT */

//! Represent a partially received message in the
//! ACE_RMCast_Reassembly module
/*!
 * This class provides temporary storage for the fragments as they are
 * received in the ACE_RMCast_Reassembly module.  It also keeps track
 * of what portions of the message are still missing.
 */
class STBAPI_API ACE_RMCast_Partial_Message
{
public:
  //! Constructor, reserve enough memory for the complete message
  ACE_RMCast_Partial_Message (ACE_UINT32 message_size);

  //! Destructor
  ~ACE_RMCast_Partial_Message (void);

  //! Process a fragment
  /*!
   * A fragment starting at <offset> has been received, copy the
   * fragment contents and update the list of holes.
   */
  int fragment_received (ACE_UINT32 message_size,
                         ACE_UINT32 offset,
                         ACE_Message_Block *mb);

  //! Return 1 if the message is complete
  int is_complete (void) const;

  //! Return the body of the message, the memory is *not* owned by the
  //! caller
  ACE_Message_Block *message_body (void);

private:
  //! Insert a new hole into the list
  /*!
   * The class keeps an array to represent the missing portions of the
   * message.  This method inserts a new hole, i.e. a new element in
   * the array at index <i>. The <start> and <end> arguments represent
   * the offsets of the missing portion of the message.
   */
  int insert_hole (size_t i,
                   ACE_UINT32 start,
                   ACE_UINT32 end);

  //! Remove a hole from the list
  int remove_hole (size_t i);

private:
  //! Maintain the message storage
  ACE_Message_Block message_body_;

  //! Represent a missing portion of a message
  struct Hole
  {
    //! Offset where the missing portion of the message starts
    ACE_UINT32 start;
    //! Offset where the missing portion of the message ends
    ACE_UINT32 end;
  };

  //! Implement a growing array of Hole structures
  //@{
  Hole *hole_list_;
  size_t max_hole_count_;
  size_t hole_count_;
  //@}
};


inline int ACE_RMCast_Partial_Message::is_complete (void) const
{
  return (this->hole_count_ == 0)
    || (this->hole_count_ == 1
        && this->hole_list_[0].start == this->hole_list_[0].end);
}

inline ACE_Message_Block * ACE_RMCast_Partial_Message::message_body (void)
{
  return &this->message_body_;
}

#endif /* ACE_RMCAST_PARTIAL_MESSAGE_H */
