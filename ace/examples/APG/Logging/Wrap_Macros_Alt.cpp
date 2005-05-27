// Wrap_Macros_Alt.cpp,v 1.1 2004/01/01 21:01:00 shuston Exp

#include "Log_Msg_Alt.h"

void foo (void);

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TRACE (ACE_TEXT ("main"));
  
  // Listing 1 code/ch03
  ACE_DEBUG ((MY_DEBUG ACE_TEXT ("Hi Mom\n")));

  ACE_DEBUG ((MY_DEBUG ACE_TEXT ("Goodnight\n")));
  // Listing 1

  return 0;
}
