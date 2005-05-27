 /* ======================================================================= */
/** header file for API definitions for the application dll
  *
  * @author Gerhard Reitmayr
  *
  * $Id: tabletop.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#ifdef TABLETOP_EXPORTS
#define TABLETOP_API __declspec(dllexport)
#else
#define TABLETOP_API __declspec(dllimport)
#endif
#else
#define TABLETOP_API
#endif

