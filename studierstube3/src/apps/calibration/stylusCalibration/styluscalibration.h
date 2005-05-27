/* ===========================================================================
    NAME:       styluscalibration
    TYPE:       c++ header
    PROJECT:    Studierstube Applications - styluscalibration
    CONTENT:    API definition for the DLL application
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    05-Feb-01 11:28:24  jp      last modification
    05-Feb-01 11:25:50  jp      created
=========================================================================== */

#ifdef WIN32
#ifdef STYLUSCALIBRATION_EXPORTS
#define STYLUSCALIBRATION_API __declspec(dllexport)
#else
#define STYLUSCALIBRATION_API __declspec(dllimport)
#endif
#else
#define STYLUSCALIBRATION_API
#endif

/* ===========================================================================
    End of styluscalibration.h
=========================================================================== */
