 /* ======================================================================= */
/** header file for API definitions for the application dll
  *
  * $Id: billiard3d.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#ifdef BILLIARD3D_EXPORTS
#define BILLIARD3D_API __declspec(dllexport)
#else
#define BILLIARD3D_API __declspec(dllimport)
#endif
#else
#define BILLIARD3D_API
#endif

