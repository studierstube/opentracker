 /* ======================================================================= */
/** header file for API definitions for the application dll
  *
  * @author Gerhard Reitmayr
  *
  * $Id: ARlibrary.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @defgroup arlibrary ARLibrary
 * This application demonstrates the use of AR for retrieving books in a library.
 * @ingroup apps
 */

#ifdef WIN32
#ifdef ARLIBRARY_EXPORTS
#define ARLIBRARY_API __declspec(dllexport)
#else
#define ARLIBRARY_API __declspec(dllimport)
#endif
#else
#define ARLIBRARY_API
#endif

