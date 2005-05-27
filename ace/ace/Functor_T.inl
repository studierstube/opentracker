// -*- C++ -*-
//
// Functor_T.inl,v 4.1 2004/06/16 07:57:23 jwillemsen Exp

template <class TYPE> ACE_INLINE unsigned long
ACE_Hash<TYPE>::operator () (const TYPE &t) const
{
  return t.hash ();
}

template <class TYPE> ACE_INLINE unsigned long
ACE_Pointer_Hash<TYPE>::operator () (TYPE t) const
{
#if defined (ACE_WIN64)
  // The cast below is legit... we only want a hash, and need not convert
  // the hash back to a pointer.
#  pragma warning(push)
#  pragma warning(disable : 4311)   /* Truncate pointer to unsigned long */
#endif /* ACE_WIN64 */
  return reinterpret_cast<unsigned long> (t);
#if defined (ACE_WIN64)
#  pragma warning(pop)
#endif /* ACE_WIN64 */
}

template <class TYPE> ACE_INLINE bool
ACE_Equal_To<TYPE>::operator () (const TYPE &lhs,
                                 const TYPE &rhs) const
{
  return lhs == rhs;
}

template <class TYPE> ACE_INLINE bool
ACE_Less_Than<TYPE>::operator () (const TYPE &lhs,
                                  const TYPE &rhs) const
{
  return lhs < rhs;
}
