/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// RMCast_Copy_On_Write.h,v 1.5 2000/10/19 16:09:32 coryan Exp
//

#ifndef ACE_RMCAST_COPY_ON_WRITE_H
#define ACE_RMCAST_COPY_ON_WRITE_H

#include "RMCast_Worker.h"
#include <ace/Synch.h>

//! A wrapper to implement reference counted collections
template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Collection
{
public:
  //! Constructor
  ACE_RMCast_Copy_On_Write_Collection (void);

  //! Increment the reference count
  void _incr_refcnt (void);

  //! Decrement the reference count
  void _decr_refcnt (void);

  //! The actual collection
  COLLECTION collection;

private:
  //! The reference count
  ACE_UINT32 refcount_;
};

// ****************************************************************

template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Read_Guard;

template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Write_Guard;

//! Base class for the Copy_On_Write collection, used to simplify the
//! declaration of the Write_Guard
template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Container
{
public:
  //! Constructor
  ACE_RMCast_Copy_On_Write_Container (void);

  //! Let the Write_Guard access the internal fields.
  friend class ACE_RMCast_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR>;

  //! Let the Read_Guard access the internal fields.
  friend class ACE_RMCast_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR>;

  //! A shorter name for the actual collection type
  typedef ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;

protected:
  //! Number of pending writes
  int pending_writes_;

  //! If non-zero then a thread is changing the collection.
  /*!
   * Many threads can use the collection simulatenously, but only one
   * change it.
   */
  int writing_;

  //! A mutex to serialize access to the collection pointer.
  ACE_SYNCH_MUTEX mutex_;

  //! A condition variable to wait to synchronize multiple writers.
  ACE_SYNCH_CONDITION cond_;

  //! The collection, with reference counting added
  Collection *collection_;
};

// ****************************************************************

//! Implement a read guard for a reference counted collection
template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Read_Guard
{
public:
  typedef ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;
  typedef ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR> Container;

  //! Constructor
  ACE_RMCast_Copy_On_Write_Read_Guard (Container &container);

  //! Destructor
  ~ACE_RMCast_Copy_On_Write_Read_Guard (void);

  //! A reference to the collection
  Collection *collection;

private:
  //! Synchronization
  ACE_SYNCH_MUTEX &mutex_;
};

// ****************************************************************

//! Implement the write guard for a reference counted collecion
/*!
 * This helper class atomically increments the reference count of a
 * ACE_RMCast_Copy_On_Write_Collection and reads the current
 * collection in the Copy_On_Write class.
 */
template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Write_Guard
{
public:
  typedef ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;
  typedef ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR> Container;

  //! Constructor
  ACE_RMCast_Copy_On_Write_Write_Guard (Container &container);

  //! Destructor
  ~ACE_RMCast_Copy_On_Write_Write_Guard (void);

  //! The collection
  Collection *copy;

private:
  //! Keep a reference to the mutex
  ACE_SYNCH_MUTEX &mutex;

  //! Keep a reference to the condition variable
  ACE_SYNCH_CONDITION &cond;

  //! Use a reference to update the pending writes count
  int &pending_writes;

  //! Use a reference to update the writing flag
  int &writing_flag;

  //! Use this reference to update the collection once the
  //! modifications are finished.
  Collection *&collection;
};


// ****************************************************************

//! Implement a copy on write wrapper for a map-like collection
/*
 *
 * <B>WARNING: </B>  This class may be moved away in the future, I'm
 * investigating how it could be converted into a reusable component
 * in ACE.   I won't make promises on when will that happen, but I
 * won't promise that it will stay here either.
 *
 */
template<class KEY, class ITEM, class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write : public ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR>
{
public:
  //! The Read_Guard trait
  typedef ACE_RMCast_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR> Read_Guard;

  //! The Write_Guard trait
  typedef ACE_RMCast_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR> Write_Guard;

  //! The underlying collection type
  typedef ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;

  //! Constructor
  ACE_RMCast_Copy_On_Write (void);

  //! Destructor
  ~ACE_RMCast_Copy_On_Write (void);

  //! Iterate over all the elements invoking \param worker on each one.
  int for_each (ACE_RMCast_Worker<KEY,ITEM> *worker);

  //! Get the first key
  KEY first_key (void);

  /// Return non-zero if the collection is empty
  int empty (void);

  //! Add a new element
  int bind (KEY const & key, ITEM const & item);

  //! Remove an element
  int unbind (KEY const & key);

  //! Bind assuming the Write_Guard is held
  int bind_i (Write_Guard &guard, KEY const & key, ITEM const & item);

  //! Unbind assuming the Write_Guard is held
  int unbind_i (Write_Guard &guard, KEY const & key);
};

template<class COLLECTION, class ITERATOR> ACE_INLINE
ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR>::
    ACE_RMCast_Copy_On_Write_Collection (void)
      :  refcount_ (1)
{
}


template<class COLLECTION, class ITERATOR> ACE_INLINE
ACE_RMCast_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR>::
    ACE_RMCast_Copy_On_Write_Read_Guard (Container &container)
  : collection (0)
  , mutex_ (container.mutex_)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->collection = container.collection_;
  this->collection->_incr_refcnt ();
}

template<class COLLECTION, class ITERATOR> ACE_INLINE
ACE_RMCast_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR>::
    ~ACE_RMCast_Copy_On_Write_Read_Guard (void)
{
  if (this->collection != 0)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
      this->collection->_decr_refcnt ();
    }
}

template<class COLLECTION, class ITERATOR> void
ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR>::_incr_refcnt (void)
{
  // LOCKING: no locking is required, the caller grabs the mutex.
  this->refcount_++;
}

template<class COLLECTION, class ITERATOR> void
ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR>::_decr_refcnt (void)
{
  // LOCKING: no locking is required, the caller grabs the mutex.
  {
    this->refcount_--;
    if (this->refcount_ != 0)
      return;
  }
  //@@ TODO: If this wrapper is going to be completely general some
  // kind of functor has to be provided to remove the elements in the
  // collection, in case the are no self-managed

  delete this;
}

// ****************************************************************

template<class KEY, class ITEM, class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write<KEY,ITEM,COLLECTION,ITERATOR>::
    ACE_RMCast_Copy_On_Write (void)
      : ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR> ()
{
}

template<class KEY, class ITEM, class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write<KEY,ITEM,COLLECTION,ITERATOR>::
    ~ACE_RMCast_Copy_On_Write (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);

  while (this->pending_writes_ != 0)
    this->cond_.wait ();

  this->collection_->_decr_refcnt ();
  this->collection_ = 0;
}

template<class KEY, class ITEM, class COLLECTION, class ITERATOR> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,COLLECTION,ITERATOR>::
    for_each (ACE_RMCast_Worker<KEY,ITEM> *worker)
{
  Read_Guard ace_mon (*this);

  ITERATOR end = ace_mon.collection->collection.end ();
  for (ITERATOR i = ace_mon.collection->collection.begin (); i != end; ++i)
    {
      int r = worker->work ((*i).key (), (*i).item ());
      if (r == 1)
        return 0; // Abort loop, but no error
      if (r == -1)
        return -1;
    }
  return 0;
}

template<class KEY, class ITEM, class C, class ITERATOR> KEY
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,ITERATOR>::first_key (void)
{
  Read_Guard ace_mon (*this);
  ITERATOR end = ace_mon.collection->collection.end ();
  ITERATOR begin = ace_mon.collection->collection.begin ();
  if (begin == end)
    {
      return KEY ();
    }
  return (*begin).key ();
}

template<class KEY, class ITEM, class C, class ITERATOR> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,ITERATOR>::empty (void)
{
  Read_Guard ace_mon (*this);
  ITERATOR end = ace_mon.collection->collection.end ();
  ITERATOR begin = ace_mon.collection->collection.begin ();

  return end == begin;
}

template<class KEY, class ITEM, class C, class I> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,I>::bind (KEY const & k,
                                              ITEM const & i)
{
  Write_Guard ace_mon (*this);

  return this->bind_i (ace_mon, k, i);
}

template<class KEY, class ITEM, class C, class I> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,I>::unbind (KEY const & k)
{
  Write_Guard ace_mon (*this);

  return this->unbind_i (ace_mon, k);
}

template<class KEY, class ITEM, class C, class I> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,I>::bind_i (Write_Guard &ace_mon,
                                                KEY const & k,
                                                ITEM const & i)
{
  return ace_mon.copy->collection.bind (k, i);
}

template<class KEY, class ITEM, class C, class I> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,I>::unbind_i (Write_Guard &ace_mon,
                                                  KEY const & k)
{
  return ace_mon.copy->collection.unbind (k);
}

// ****************************************************************

template<class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR>::ACE_RMCast_Copy_On_Write_Container (void)
  : pending_writes_ (0)
  , writing_ (0)
  , cond_ (mutex_)
{
  ACE_NEW (this->collection_, Collection);
}

// ****************************************************************

template<class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR>::
  ACE_RMCast_Copy_On_Write_Write_Guard (Container &container)
  : copy (0)
  , mutex (container.mutex_)
  , cond (container.cond_)
  , pending_writes (container.pending_writes_)
  , writing_flag (container.writing_)
  , collection (container.collection_)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex);

    this->pending_writes++;

    while (this->writing_flag != 0)
      this->cond.wait ();

    this->writing_flag = 1;
  }

  // Copy outside the mutex, because it may take a long time.
  // Nobody can change it, because it is protected by the
  // writing_flag.

  // First initialize it (with the correct reference count
  ACE_NEW (this->copy, Collection);
  // Copy the contents
  this->copy->collection = this->collection->collection;
}

template<class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR>::
    ~ACE_RMCast_Copy_On_Write_Write_Guard (void)
{
  Collection *tmp = 0;
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex);

    tmp = this->collection;
    this->collection = this->copy;
    this->writing_flag = 0;
    this->pending_writes--;

    this->cond.signal ();
  }
  // Delete outside the mutex, because it may take a long time.
  // @@ Is this right?  What happens if several readers are still
  // using the old copy?
  tmp->_decr_refcnt ();
}


#endif /* ACE_RMCAST_COPY_ON_WRITE_H */
