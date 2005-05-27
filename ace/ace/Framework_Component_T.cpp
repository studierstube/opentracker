// Framework_Component_T.cpp
// Framework_Component_T.cpp,v 1.6 2004/06/14 13:58:41 jwillemsen Exp
#ifndef ACE_FRAMEWORK_COMPONENT_T_CPP
#define ACE_FRAMEWORK_COMPONENT_T_CPP

#include "ace/Framework_Component_T.h"

ACE_RCSID(ace, Framework_Component_T, "Framework_Component_T.cpp,v 1.6 2004/06/14 13:58:41 jwillemsen Exp")

template <class Concrete>
ACE_Framework_Component_T<Concrete>::ACE_Framework_Component_T (Concrete *concrete)
  : ACE_Framework_Component ((void *) concrete, concrete->dll_name (), concrete->name ())
{
  ACE_TRACE ("ACE_Framework_Component_T<Concrete>::ctor");
}

template <class Concrete>
ACE_Framework_Component_T<Concrete>::~ACE_Framework_Component_T (void)
{
  ACE_TRACE ("ACE_Framework_Component_T<Concrete>::~ACE_Framework_Component_T");
  Concrete::close_singleton ();
}

template <class Concrete> void
ACE_Framework_Component_T<Concrete>::close_singleton (void)
{
  ACE_TRACE ("ACE_Framework_Component_T<Concrete>::close_singleton");
  Concrete::close_singleton ();
}

#endif /*ACE_FRAWORK_COMPONENT_T_CPP*/
