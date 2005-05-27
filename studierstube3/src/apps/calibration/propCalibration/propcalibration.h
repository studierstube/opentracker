/* ===========================================================================
    NAME:       propcalibration
    TYPE:       c++ header
    PROJECT:    Studierstube Applications - propcalibration
    CONTENT:    API definition for the DLL application
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    05-Feb-01 11:28:24  jp      last modification
    05-Feb-01 11:25:50  jp      created
=========================================================================== */

#ifdef WIN32
#ifdef PROPCALIBRATION_EXPORTS
#define PROPCALIBRATION_API __declspec(dllexport)
#else
#define PROPCALIBRATION_API __declspec(dllimport)
#endif
#else
#define PROPCALIBRATION_API
#endif

/* ===========================================================================
    End of propcalibration.h
=========================================================================== */
