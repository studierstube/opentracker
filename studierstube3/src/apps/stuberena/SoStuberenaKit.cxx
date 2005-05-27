// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoStuberenaKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Terrain application for workspace
//  VERSION:    1.0
// ===========================================================================
//  AUTHORS:    ge      Gottfried Eibner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <stbapi/workspace/StbWorkspace.h>

#include <Inventor/nodes/SoSeparator.h>

#include "SoStuberenaKit.h"

SO_KIT_SOURCE(SoStuberenaKit);

//----------------------------------------------------------------------------
void SoStuberenaKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoStuberenaKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------

SoContextKit* SoStuberenaKit::factory()
{
    return new SoStuberenaKit();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoStuberenaKit::SoStuberenaKit()
{
    SO_KIT_CONSTRUCTOR(SoStuberenaKit);

    SO_KIT_ADD_CATALOG_ENTRY(postScene, SoSeparator, TRUE, this, \0 ,TRUE);

    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
SbBool SoStuberenaKit::checkWindowGeometry()
{
    if (getMasterMode())
    {
        SoSeparator *postRoot = StbWorkspace::getInstance()->postRoot;
        postRoot->addChild(SO_GET_PART(this,"postScene",SoSeparator));
    }

    return TRUE;
}

//----------------------------------------------------------------------------
//eof
