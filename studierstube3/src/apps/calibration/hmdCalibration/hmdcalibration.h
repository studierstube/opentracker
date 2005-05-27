/* ===========================================================================
    NAME:       hmdcalibration
    TYPE:       c++ header
    PROJECT:    Studierstube Applications - hmdcalibration
    CONTENT:    API definition for the DLL application
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    05-Feb-01 11:28:24  jp      last modification
    05-Feb-01 11:25:50  jp      created
=========================================================================== */

#ifdef WIN32
#ifdef HMDCALIBRATION_EXPORTS
#define HMDCALIBRATION_API __declspec(dllexport)
#else
#define HMDCALIBRATION_API __declspec(dllimport)
#endif
#else
#define HMDCALIBRATION_API
#endif

/* ===========================================================================
    End of hmdcalibration.h
=========================================================================== */
