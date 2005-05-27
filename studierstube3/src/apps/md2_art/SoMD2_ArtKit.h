// ===========================================================================
//  (C) 2003 Vienna University of Technology
// ===========================================================================
//  NAME:       SoMD2_ArtKit.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    MD2 on ARToolkit Artifact Example Application
//  VERSION:    1.0
// ===========================================================================
//  AUTHORS:    dw      Daniel Wagner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _SoMD2_ArtKit_H_
#define _SoMD2_ArtKit_H_

#include "md2_art.h"

#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/So3DSlider.h>
#include <stbapi/misc/SoMD2NodeKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif



class MD2_ART_API SoMD2_ArtKit : public SoContextKit
{
    SO_KIT_HEADER(SoMD2_ArtKit);

public:
    static void initClass();             //static class initialization

    SoMD2_ArtKit();
    SoContextKit* factory();             //override this to create new context of a specific type
    ~SoMD2_ArtKit();

	static void MD2ButtonCB(void* data, So3DButton* button);

protected:
    SbBool checkWindowGeometry();

	void checkPipMasterMode(    //master/slave mode for pip sheet
        SoNode*	 pipSheet,      //override this to implement your
        SbBool masterMode       //behavior
        );               

	SoMD2NodeKit*	md2;


	static SoNode* myFindNode(SoNode* root, SbString name);

}; //class SoMD2_ArtKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SoMD2_ArtKit_H_
