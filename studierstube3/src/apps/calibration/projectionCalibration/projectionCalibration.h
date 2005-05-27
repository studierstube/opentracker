/* ===========================================================================
    NAME:       PROJECTIONCALIBRATION
    TYPE:       c++ header
    PROJECT:    Studierstube Applications - PROJECTIONCALIBRATION
    CONTENT:    API definition for the DLL application
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    05-Feb-01 11:28:24  jp      last modification
    05-Feb-01 11:25:50  jp      created
=========================================================================== */

#ifdef WIN32
#ifdef PROJECTIONCALIBRATION_EXPORTS
#define PROJECTIONCALIBRATION_API __declspec(dllexport)
#else
#define PROJECTIONCALIBRATION_API __declspec(dllimport)
#endif
#else
#define PROJECTIONCALIBRATION_API
#endif

/* ===========================================================================
    End of PROJECTIONCALIBRATION.h
=========================================================================== */
