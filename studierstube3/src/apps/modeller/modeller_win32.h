/* ===========================================================================
    NAME:       modeller_win32
    TYPE:       c++ header
    PROJECT:    Studierstube Applications - modeller
    CONTENT:    API definition for the DLL application
   ===========================================================================
    AUTHORS:    bs      Benjamin A. Schmit
=========================================================================== */

#ifdef WIN32
#ifdef MODELLER_EXPORTS
#define MODELLER_API __declspec(dllexport)
#else
#define MODELLER_API __declspec(dllimport)
#endif
#else
#define MODELLER_API
#endif

/* ===========================================================================
    End of modeller_win32.h
=========================================================================== */
