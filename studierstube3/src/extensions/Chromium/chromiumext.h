/**
 * @defgroup chromium Chromium
 * @ingroup extensions
 *
 * The Chromium extension allows to render the stream of a Chromium network
 * in OpenInventor. It provides a single additional node SoChromium that 
 * can be used in the scene graph similar to a shape node.
 *
 * @author Matthias Grumet, Peter Rautek, Gerhard Reitmayr
 */

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHROMIUM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHROMIUM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef WIN32
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#pragma warning(disable:4305)
#pragma warning(disable:4786)
    #ifdef CHROMIUM_STATIC
        #define CHROMIUM_API
        #define CHROMIUM_EXTERN extern
    #else
        #ifdef CHROMIUM_EXPORTS
            #define CHROMIUM_API __declspec(dllexport)
            #define CHROMIUM_EXTERN
        #else
            #define CHROMIUM_API __declspec(dllimport)
            #define CHROMIUM_EXTERN extern
        #endif
    #endif
#else
    #define CHROMIUM_API
#endif