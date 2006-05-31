

#ifndef __STRINGALIGNER_HEADERFILE__
#define __STRINGALIGNER_HEADERFILE__

#include <string.h>

class StringAligner
{
public:
   StringAligner(const char* nPtr) : ptr(nPtr), myptr(0)
   {
#define USE_64_BIT
#ifdef USE_64_BIT
       if((reinterpret_cast<unsigned long>(nPtr) & 0x03f) != 0)
#else
       if((reinterpret_cast<unsigned int>(nPtr) & 0x03f) != 0)
#endif
           strcpy(myptr = new char[strlen(nPtr)+1], nPtr);
   }

   const char* get() const
   {
       return myptr ? myptr : ptr;
   }

   ~StringAligner()
   {
       if(myptr)
           delete myptr;
   }

protected:
   const char* ptr;
   char* myptr;
};

#ifdef _WIN32_WCE
  #define ALIGN_STRING(str)  StringAligner(str).get()
#else
  #define ALIGN_STRING(str) str
#endif //_WIN32_WCE


#endif //__STRINGALIGNER_HEADERFILE__
