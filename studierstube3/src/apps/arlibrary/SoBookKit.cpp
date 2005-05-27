 /* ======================================================================= */
/** source file for SoBookKit
  *
  * @author Harald Piringer, Eike Umlauf
  *
  * $Id: SoBookKit.cpp 3427 2004-07-14 12:20:49Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/SoDB.h>
#include "SoBookKit.h"

SO_KIT_SOURCE(SoBookKit);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SoBookKit::SoBookKit()
{
	SO_KIT_CONSTRUCTOR(SoBookKit);

	SO_KIT_ADD_CATALOG_ENTRY(contents, SoSeparator, FALSE, this, \x0, TRUE);

	//---- set the fields to dummy values
	SO_NODE_ADD_FIELD(id, (0));
	SO_NODE_ADD_FIELD(authors, ("nobody"));
	SO_NODE_ADD_FIELD(keywords, ("none"));
	SO_NODE_ADD_FIELD(title, ("unnamed"));
	SO_NODE_ADD_FIELD(isbn, ("unnamed"));
	SO_NODE_ADD_FIELD(shelf, ("unnamed"));
	SO_NODE_ADD_FIELD(color, (0.0f, 0.0f, 0.0f));
	SO_NODE_ADD_FIELD(year, (0));

	SO_KIT_INIT_INSTANCE();
}

SoBookKit::~SoBookKit()
{

}

void SoBookKit::initClass()
{
	SO_KIT_INIT_CLASS(SoBookKit, SoBaseKit, "BaseKit");
}

