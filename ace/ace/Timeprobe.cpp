// Timeprobe.cpp,v 4.23 2004/06/25 07:56:02 jwillemsen Exp

#include "ace/config-all.h"

ACE_RCSID(ace, Timeprobe, "Timeprobe.cpp,v 4.23 2004/06/25 07:56:02 jwillemsen Exp")

#if defined (ACE_COMPILE_TIMEPROBES)

#include "ace/Timeprobe.h"
#include "ace/Mutex.h"
#include "ace/Null_Mutex.h"

#if !defined (__ACE_INLINE__)
#include "ace/Timeprobe.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Timeprobe_Ex<ACE_TIMEPROBE_MUTEX,ACE_TIMEPROBE_ALLOCATOR>;
template class ACE_Function_Timeprobe<ACE_Timeprobe_Ex<ACE_TIMEPROBE_MUTEX,ACE_TIMEPROBE_ALLOCATOR> >;
template class ACE_Unbounded_Set_Iterator<ACE_Event_Descriptions>;
template class ACE_Unbounded_Set<ACE_Event_Descriptions>;
template class ACE_Node<ACE_Event_Descriptions>;
template class ACE_Timeprobe_Ex<ACE_Thread_Mutex, ACE_New_Allocator>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Timeprobe_Ex<ACE_TIMEPROBE_MUTEX,ACE_TIMEPROBE_ALLOCATOR>
#pragma instantiate ACE_Function_Timeprobe<ACE_Timeprobe_Ex<ACE_TIMEPROBE_MUTEX,ACE_TIMEPROBE_ALLOCATOR> >
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Event_Descriptions>
#pragma instantiate ACE_Unbounded_Set<ACE_Event_Descriptions>
#pragma instantiate ACE_Node<ACE_Event_Descriptions>
#pragma instantiate ACE_Timeprobe_Ex<ACE_Thread_Mutex, ACE_New_Allocator>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#  if defined (ACE_TSS_TIMEPROBES)
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_TSS_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_NULL_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#  else /* ACE_TSS_TIMEPROBES */
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#  endif /* ACE_TSS_TIMEPROBES */

#endif /* ACE_COMPILE_TIMEPROBES */

