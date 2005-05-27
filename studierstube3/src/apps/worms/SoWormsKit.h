// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoWormsKit.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Small simple game...
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    dw		Daniel Wagner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef __SOWORMSKIT_HEADERFILE__
#define __SOWORMSKIT_HEADERFILE__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoSphere.h>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/So3DSlider.h>
#include <stbapi/workspace/SoContextKit.h>

#include "worms.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//#include "SoMD2NodeKit.h"
//#include "SoTerrain.h"
#include "SoWormsScene.h"


class WORMS_API SoWormsKit : public SoContextKit
{
    SO_KIT_HEADER(SoWormsKit);

public:
    static void initClass();             //static class initialization

    SoWormsKit();

    SoContextKit* factory();             //override this to create new context of a specific type

    ~SoWormsKit();

	static void MD2ButtonCB(void* data, So3DButton* button);

protected:
    SbBool checkPipGeometry();

    SbBool checkWindowGeometry();


	void checkPipMasterMode(SoNode*	 pipSheet, SbBool masterMode);               

    SbBool windowEventCB(void* data, int messageId, SoWindowKit* win, int uid, So3DEvent* event, SoPath* path);

    So3DSlider* speedSlider;     // slider for ball/brush size

    static SoSeparator* sheet;			// pointer to pip sheet


	SoWormsScene* theScene;
	//SoMD2NodeKit*	md2NodeKit;
	//SoTerrain*		terrain;

}; //class SoWormsKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//__SOWORMSKIT_HEADERFILE__

