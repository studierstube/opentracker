

#ifndef __STRINGALIGNER_HEADERFILE__
#define __STRINGALIGNER_HEADERFILE__

#include <string.h>

class StringAligner
{
public:
   StringAligner(const char* nPtr) : ptr(nPtr), myptr(0)
   {
       if((reinterpret_cast<unsigned int>(nPtr) & 0x03f) != 0)
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
