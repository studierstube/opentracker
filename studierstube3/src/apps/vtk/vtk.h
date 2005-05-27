 /* ======================================================================= */
/** header file for API definitions for the application dll
  *
  * @author Stefan Bruckner
  *			Rudolf Seemann
  *
  * $Id: vtk.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#ifdef VTK_EXPORTS
#define VTK_API __declspec(dllexport)
#else
#define VTK_API __declspec(dllimport)
#endif
#else
#define VTK_API
#endif

