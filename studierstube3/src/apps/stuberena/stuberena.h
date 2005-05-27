/* ===========================================================================
    NAME:       stuberena
    TYPE:       c++ header
    PROJECT:    Studierstube Applications - stuberena
    CONTENT:    API definition for the DLL application
   ===========================================================================
    AUTHORS:    ge      Gottfried Eibner
   ===========================================================================
    HISTORY:

    12-Jun-03 15:07:32  ge      created
   =========================================================================== */

#ifdef WIN32
#ifdef STUBERENA_EXPORTS
#define STUBERENA_API __declspec(dllexport)
#else
#define STUBERENA_API __declspec(dllimport)
#endif
#else
#define STUBERENA_API
#endif

/* ===========================================================================
    End of stuberena.h
=========================================================================== */
