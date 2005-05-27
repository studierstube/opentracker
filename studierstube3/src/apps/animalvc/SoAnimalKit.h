// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoAnimalKit.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Demo application for Studierstube foundation class
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds		Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _SoAnimalKit_H_
#define _SoAnimalKit_H_


#include <stbapi/vc/SoVCWidgetKit.h>

#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextKit.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/nodes/SoSphere.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/So3DSlider.h>
#include "animal.h"
#include <Inventor/fields/SoSFUInt32.h>
#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class ANIMAL_API SoAnimalKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoAnimalKit);

public:
    static void initClass(             //static class initialization
        );
    SoAnimalKit(
        );
    SoContextKit* factory(             //override this to create new context
        );                             //of a specific type
    ~SoAnimalKit(
        );
    static void resetButtonCB(
        void*, 
        So3DButton* button
        );
    static void colorButtonCB(
        void*, 
        So3DButton* button
        );

    static void colorWidgetCB(
	void*,
	SoVCWidgetKit* widgetKit
	);


protected:
    SbBool checkPipGeometry();
    SbBool checkMainPanelGeometry();

    SbBool checkWindowGeometry();

    SoSFUInt32 packed;          // color of paint (same as ball)
    SoSFFloat sliderSize;       // current value of slider
 
    void checkMainPanelMasterMode(
				  SoNode* mainPanelContents,
				  SbBool masterMode
				  );
   
    void checkPipMasterMode(   //master/slave mode for pip sheet
			    SoNode* pipSheet,              //override this to implement your
			    SbBool masterMode              //behavior
			    );               
    SbBool windowEventCB(              //override this to get your own
        void* data,                    //window message behavior
        int messageId,
        SoWindowKit* win,
        int uid,
        So3DEvent* event, 
        SoPath* path
        );

    SoIndexedTriangleStripSet* ts; //indexed trianglestripset mesh
    SoVertexProperty* vp;     // mesh with animal
    SoTranslation* ballTrafo; // translation of small "cursor" ball
    SoMaterial* ballMat;      // color of ball
    So3DSlider* sizeSlider;   // slider for ball/brush size
    SoSphere* sph;            // sphere representing brush

    static SoAnimalKit* animalFocus; // pointer to instance in focus
    static SoSeparator* sheet;       // pointer to pip sheet

    SoTransform *transi;

}; //class SoAnimalKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SoAnimalKit_H_

