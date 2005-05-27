/* ===========================================================================
    NAME:       @sampleapp@
    TYPE:       c++ header
    PROJECT:    Studierstube Applications - @sampleapp@
    CONTENT:    API definition for the DLL application
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    05-Feb-01 11:28:24  jp      last modification
    05-Feb-01 11:25:50  jp      created
=========================================================================== */

#ifdef WIN32
#ifdef VIEWER_EXPORTS
#define VIEWER_API __declspec(dllexport)
#else
#define VIEWER_API __declspec(dllimport)
#endif
#else
#define VIEWER_API
#endif

/* ===========================================================================
    End of @sampleapp@.h
=========================================================================== */
