/* ========================================================================
 * LICENSE: see COPYING.ACE
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* -*- C++ -*- */
// RMCast_Worker.h,v 1.1 2000/10/02 18:40:03 coryan Exp
//

#ifndef ACE_RMCAST_WORKER_H
#define ACE_RMCAST_WORKER_H

#include <ace/config-all.h>

template<class KEY, class ITEM>
class ACE_RMCast_Worker
{
public:
  virtual ~ACE_RMCast_Worker (void);

  virtual int work (KEY const & key,
                    ITEM const & item) = 0;
};

template<class KEY, class ITEM>
ACE_RMCast_Worker<KEY,ITEM>::~ACE_RMCast_Worker (void)
{
}

#endif /* ACE_RMCAST_WORKER_H */
