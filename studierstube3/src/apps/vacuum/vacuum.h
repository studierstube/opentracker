/* ===========================================================================
    NAME:       vacuum
    TYPE:       c++ header
    PROJECT:    Studierstube Applications - vacuum
    CONTENT:    Defines the entry point for the DLL application.
   ===========================================================================
    AUTHORS:    ib,tf      Istvan Barakonyi, Tamer Fahmy
   ===========================================================================
    HISTORY:

    15-Aug-03 01:55:42  ib      created
=========================================================================== */

#ifdef WIN32
#ifdef VACUUM_EXPORTS
#define VACUUM_API __declspec(dllexport)
#else
#define VACUUM_API __declspec(dllimport)
#endif
#else
#define VACUUM_API
#endif

/* ===========================================================================
    End of vacuum.h
=========================================================================== */
