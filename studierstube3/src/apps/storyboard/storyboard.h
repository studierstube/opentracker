
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the STORYBOARD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// STORYBOARD_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WIN32
#ifdef STORYBOARD_EXPORTS
#define STORYBOARD_API __declspec(dllexport)
#else
#define STORYBOARD_API __declspec(dllimport)
#endif
#else
#define STORYBOARD_API
#endif
