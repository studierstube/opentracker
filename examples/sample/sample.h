 /* ======================================================================= */
/** header file for API definitions for the application dll
  *
  * @author Gerhard Reitmayr
  *
  * $Header$
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#ifdef SAMPLE_EXPORTS
#define SAMPLE_API __declspec(dllexport)
#else
#define SAMPLE_API __declspec(dllimport)
#endif
#else
#define SAMPLE_API
#endif

