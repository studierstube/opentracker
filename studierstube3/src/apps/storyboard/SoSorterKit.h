// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoSorterKit.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Header for class SoSorterKit
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds		Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _SOSORTERKIT_H_
#define _SOSORTERKIT_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <stbapi/interaction/SoDragKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoSorterKit
:
    public SoBaseKit
{
    SO_KIT_HEADER(SoSorterKit);
    SO_KIT_CATALOG_ENTRY_HEADER(icons);

public:
    SoSFInt32 rows;
    SoSFFloat xspacing;
    SoSFFloat yspacing;

    static void initClass(             //static class initialization
        );
    SoSorterKit(
		);
    ~SoSorterKit(
        );
	void setMasterMode(
		SbBool masterMode
		);
    void resort(
        );
    void add(
        SbString id,
        SoTexture2* icon,
        int insertAt = -1
        );
    void remove(
        SbString id
        );
    int getNumIcons(
        );
    void select(
        SbString id
        );
    void select(
        SoDragKit* drag
        );
    void unselect(
        );
    SbString getId(
        int index
        );
    static void pressCallback(
        void* userData,
        SoDragKit* drag
        );
    static void moveCallback(
        void* userData,
        SoDragKit* drag
        );
    static void releaseCallback(
        void* userData,
        SoDragKit* drag
        );
    SoTexture2* getTexture(
		SbString id
		);

private:
    SoGroup* getIcons(
        );
    SbVec3f getTranslation(
        int index
        );
    void computeSnapping(
        SoDragKit* drag,
        int& oldIndex,
        int& newIndex,
        SbVec3f& newTranslation,
        SbBool& hasMoved
        );

    SoDragKit* selectedDrag;
    SbBool isMaster;
}; // class SorterKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOSORTERKIT_H_

