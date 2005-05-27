// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoStuberenaKit.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Stuberena startup application for Studierstube foundation class
//  VERSION:    1.0
// ===========================================================================
//  AUTHORS:    ge      Gottfried Eibner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _SoStuberenaKit_H_
#define _SoStuberenaKit_H_

#include <stbapi/workspace/SoContextKit.h>

#include "stuberena.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class STUBERENA_API SoStuberenaKit : public SoContextKit
{
    SO_KIT_HEADER(SoStuberenaKit);

    SO_KIT_CATALOG_ENTRY_HEADER(postScene);

  public:

    SoStuberenaKit(void);

    static void initClass(void);    //static class initialization

    SoContextKit* factory(void);    //override this to create new context of a specific type

  protected:

    SbBool checkWindowGeometry(void);

}; //class SoStuberenaKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SoStuberenaKit_H_
