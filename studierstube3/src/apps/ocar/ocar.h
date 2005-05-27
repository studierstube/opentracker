
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OCAR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OCAR_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WIN32
#ifdef OCAR_EXPORTS
#define OCAR_API __declspec(dllexport)
#else
#define OCAR_API __declspec(dllimport)
#endif
#else
#define OCAR_API
#endif

/** @defgroup ocar Outdoor Collaborative Augmented Reality
 * This group contains classes (mostly application classes) for the OCAR project.
 * @ingroup apps
 */
