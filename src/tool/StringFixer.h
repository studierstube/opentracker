

#ifndef __STRINGFIXER_HEADERFILE__
#define __STRINGFIXER_HEADERFILE__

#include <string.h>

class StringFixer
{
	enum {
		MAX_LEN = 8192			// 8K should be enough...
	};

public:
   StringFixer(const char* nString, ...)
   {
		str = new char[MAX_LEN];

		va_list marker;
		va_start(marker, nString);
		vsprintf(str, nString, marker);
   }

   /*StringFixer(const unsigned short* nString, ...)
   {
		char* strc = new char[wcslen(nString)+1];
		int len = wcslen(nString);

		for(int i=0; i<=len; i++)
			strc[i] = (char)nString[i];

		str = new char[MAX_LEN];

		va_list marker;
		va_start(marker, nString);
		vsprintf(str, strc, marker);
		delete strc;
   }*/

   const char* get() const
   {
       return str;
   }

   ~StringFixer()
   {
       delete str;
   }

protected:
   char* str;
};

#ifdef _WIN32_WCE
  #define FIX_STRING(X) StringFixer(X).get() 
#else
  #define FIX_STRING(X) X
#endif //_WIN32_WCE


#endif //__STRINGFIXER_HEADERFILE__
