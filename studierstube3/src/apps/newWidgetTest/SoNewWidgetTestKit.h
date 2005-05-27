// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoNewWidgetTestKit.h
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

#ifndef _SONEWWIDGETTESTKIT_H_
#define _SONEWWIDGETTESTKIT_H_

#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/workspace/SoContextKit.h>
#include "newwidgettest.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class NEWWIDGETTEST_API SoNewWidgetTestKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoNewWidgetTestKit);

public:
    static void initClass(             //static class initialization
        );
    SoNewWidgetTestKit(                   //constructor
        );
    SoContextKit* factory(             //override this to create new context
        );                             //of a specific type
    ~SoNewWidgetTestKit(
        );
    static void colorButtonCB(         //callback for color buttons
        void* data,                    //user data
        So3DButton* button             //pointer to pressed button
        );

private:
    SoNode* createPipSheet(       //create new pip sheet
        int uid                        //user assigned to sheet
        );
    SoWindowKit* createWindowGeometry( //create window and content
        int index                      //index of window
        );
    virtual void setSheetMasterMode(   //master/slave mode for pip sheet
        SoNode* pipSheet,              //override this to implement your
        SbBool masterMode              //behavior
        );               
}; //class SoNewWidgetTestKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SONEWWIDGETTESTKIT_H_

