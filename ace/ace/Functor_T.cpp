/* -*- C++ -*- */
// Functor_T.cpp,v 4.6 2004/06/16 07:57:20 jwillemsen Exp

#ifndef ACE_FUNCTOR_T_C
#define ACE_FUNCTOR_T_C

#include "ace/Functor_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/Functor_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Functor_T, "Functor_T.cpp,v 4.6 2004/06/16 07:57:20 jwillemsen Exp")

ACE_ALLOC_HOOK_DEFINE(ACE_Command_Callback)

///////////////////////////////////
// GOF Command Pattern Templates //
///////////////////////////////////

// Constructor.

template <class RECEIVER, class ACTION>
ACE_Command_Callback<RECEIVER, ACTION>::ACE_Command_Callback (RECEIVER &recvr,
                                    ACTION action)
  : receiver_ (recvr),
    action_ (action)
{
}

template <class RECEIVER, class ACTION>
ACE_Command_Callback<RECEIVER, ACTION>::~ACE_Command_Callback (void)
{
}

// Invokes an operation.

template <class RECEIVER, class ACTION> int
ACE_Command_Callback<RECEIVER, ACTION>::execute (void *arg)
{
  return (receiver_.*action_) (arg);
}


#endif /* ACE_FUNCTOR_T_C */
