// Adaptive_Lock_Performance_Test_Base.h,v 1.3 2004/02/24 17:33:12 jwillemsen Exp

#ifndef ACE_ADAPTIVE_LOCK_PERFORMANCE_TEST_BASE_H
#define ACE_ADAPTIVE_LOCK_PERFORMANCE_TEST_BASE_H
#include "Benchmark_Performance.h"

ACE_RCSID(Synch_Benchmarks, Adaptive_Lock_Performance_Test_Base, "Adaptive_Lock_Performance_Test_Base.h,v 1.3 2004/02/24 17:33:12 jwillemsen Exp")

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Adaptive_Lock_Performance_Test_Base : public Benchmark_Performance
{
public:
  virtual int init (int, ACE_TCHAR *[]) = 0;
  virtual int fini (void);
  virtual int svc (void);

  int set_lock (ACE_Lock *lock);

private:
  static ACE_Lock *lock_;
};
#endif /* ACE_HAS_THREADS */
#endif /* ACE_ADAPTIVE_LOCK_PERFORMANCE_TEST_BASE_H */
