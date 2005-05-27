/* ===========================================================================
    NAME:       trainer
    TYPE:       c++ header
    PROJECT:    Studierstube Applications - trainer
    CONTENT:    API definition for the DLL application
   ===========================================================================
    AUTHORS:    bs      Benjamin A. Schmit
=========================================================================== */

#ifdef WIN32
#ifdef TRAINER_EXPORTS
#define TRAINER_API __declspec(dllexport)
#else
#define TRAINER_API __declspec(dllimport)
#endif
#else
#define TRAINER_API
#endif

/* ===========================================================================
    End of trainer_win32.h
=========================================================================== */
