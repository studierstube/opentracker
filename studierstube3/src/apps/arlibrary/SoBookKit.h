 /* ======================================================================= */
/** header file for SoBookKit
  *
  * @author Harald Piringer, Eike Umlauf
  *
  * $Id: SoBookKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOBOOKKIT_H_
#define _SOBOOKKIT_H_

#include "ARlibrary.h"
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFString.h> 
#include <Inventor/fields/SoSFString.h> 
#include <Inventor/fields/SoSFColor.h> 
#include <Inventor/fields/SoSFInt32.h> 

/**
 * SoBookKit models a single book in the library. It stores information about
 * the book and the geomtry to represent the book.
 *
 * @author Harald Piringer and Eike Umlauf.
 * @ingroup arlibrary
 */
class ARLIBRARY_API SoBookKit : public SoBaseKit  
{
	SO_KIT_HEADER(SoBookKit);

public:
	SO_KIT_CATALOG_ENTRY_HEADER(contents);
	
public:
	SoBookKit();

SoINTERNAL public:
	static void initClass();

public:
    /// multifield of all known authors
	SoMFString	authors;
    /// multifield of all keywords
    SoMFString	keywords;
	SoSFString	title,
				isbn;
    /// stores identifier of the shelf
	SoSFString	shelf;
	SoSFColor	color;
	SoSFInt32	year;
	SoSFInt32	id;

public:
	virtual ~SoBookKit();

};

#endif
