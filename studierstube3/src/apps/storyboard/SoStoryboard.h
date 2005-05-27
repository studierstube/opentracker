// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoStoryboard.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    Demo application for Studierstube foundation class
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds        Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _SOSTORYBOARD_H_
#define _SOSTORYBOARD_H_

#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextKit.h>
#include "SoSorterKit.h"
#include "storyboard.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//----------------------------------------------------------------------------
// SoStoryboard - sample multi-context application for Studierstube
// this application is derived from the Studierstube 2.0 foundation class
// SoContextKit. It can thus manage multiple contexts (working windows),
// and work in both master and slave mode in Studierstube.
// It allows to manage multiple "scenes", in which players, props, and
// a camera can be manipulated.
//----------------------------------------------------------------------------

class STORYBOARD_API SoStoryboard
:
    public SoContextKit
{
    SO_KIT_HEADER(SoStoryboard);

public:
    SoSFInt32 thisInstanceNum;               //number of this instance

    static void initClass(             //static class initialization
        );                             //call before first use
    SoStoryboard(
        );
    ~SoStoryboard(
        );
    SoContextKit* factory(             //override this to create new context
        );                             //of a specific type
    static void createWindowCB(
        void* data,
        SoSensor* 
        );
    static void deleteWindowCB(
        void* data,
        SoSensor* 
        );
    static void createWindowButtonCB(
        void* data,
        So3DButton* 
        );
    static void deleteWindowButtonCB(
        void* data,
        So3DButton* button
        );
    static void checkBoxCB(
        void* userData,
        So3DCheckBox* button
        );
    static void sliderCB(
        void* userData,
        So3DIntSlider* slider
        );
    static void updateTexture(
        void* userData, 
        SoDragKit* drag
        );
    int getNumWindows(
        );
    void closeWindow(                   //close window with index i
        int i
        );

private:
    SoNode* createPipSheet(
        int uid
        );
    SoWindowKit* createWindowGeometry(
        int index
        );
    void setMasterMode(
        SbBool masterMode
        );
    void setSheetMasterMode(           //master/slave mode for pip sheet
        SoNode* pipSheet,              //override this to implement your
        SbBool masterMode              //behavior
        );
    void focusChange(
        int uid
        );
    SoWindowKit* createFirstWindow(
        );
    SoWindowKit* createSecondWindow(
        );
    void createIcon(
        );
   void createCameraScene(
        );
    SbString getIconId(
        );

    static int instanceNum;            //running index of instances
    static SoWindowKit* staticWindow;  //pointer to single static window  
    static SoSeparator* sceneSource;   //pointer to scene template
    static SoSorterKit* sorter;        //pointer to slide sorter
    //static SoSorterKit* sorterBig;
    static SoTexture2*  silverScreenTexture;

    SoSeparator* bigScene;             //pointer to special big scene graph
    SoPointLight* bigSceneLight;
    SoPerspectiveCamera* bigSceneCam;

    SoSwitch* sceneSwitch;             //switch for changing scenes
    SoSeparator* figSep;               //root of all figures
    SoSeparator* camSep;               //root of all camera stuff
    //SoTexture2*  texture;              //pointer to instance's texture image

    SoDragKit* cam1;
    SoSwitch* player0Switch;
    SoSwitch* player1Switch;
    SoSwitch* player2Switch;
    SoSwitch* player3Switch;
}; //class SoStoryboard

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOSTORYBOARD_H_

