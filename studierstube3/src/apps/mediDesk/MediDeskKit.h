// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       MediDeskKit.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================


#include <Inventor/SbLinear.h>
#include <Inventor/SoPath.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/Xt/viewers/SoXtViewer.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/So3DLogSlider.h>
#include <stbapi/util/ivio.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/SoContextKitSub.h>
#include <stbapi/interaction/SoDragKit.h>
#include <stbapi/interaction/So3DSensor.h>
#include <stbapi/interaction/SoDragSeparator.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/misc/OnScreenRenderer.h>
#include <stbapi/workspace/SoContextKit.h>
#include "MySoSketchKit.h"
#include "voAppearanceElement.h"
#include "voAppearance.h"
#include "voGeometry.h"
#include "voStatus.h"


class MediDeskKit
:
    public SoContextKit
{
    SO_KIT_HEADER(MediDeskKit);

public:
    double          peqn[4];
    double          head;
    float           panelOffset;

    static void     initClass();       //static class initialization        
    MediDeskKit();                     //constructor
    SoContextKit*   factory();         //override this to create new context
                                       //of a specific type
    ~MediDeskKit();
    static void     colorButtonCB(     //callback for color buttons
        void* data,                    //user data
        So3DButton* button             //pointer to pressed button
    );
    void            freezeClipPlane(
        SoClipPlane * fcp
    );
    void            unFreezeClipOnePlane(
        SoClipPlane *fcp
    );
    void            unFreezeAllClipPlane();


private:
    SoSeparator* createPipSheet(       //create new pip sheet
        int uid                        //user assigned to sheet
    );
    SoWindowKit* createWindowGeometry( //create window and content
        int index                      //index of window
    );
    virtual void setSheetMasterMode(   //master/slave mode for pip sheet
        SoNode* pipSheet,              //override this to implement your
        SbBool masterMode              //behavior
    );
}; //class MediDeskKit

