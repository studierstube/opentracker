// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoRedAndBlueKit.h
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

#ifndef _SOREDANDBLUEKIT_H_
#define _SOREDANDBLUEKIT_H_

#include <stbapi/widgets/So3DButton.h>
#include <stbapi/workspace/SoContextKit.h>
#include "redandblue.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class REDANDBLUE_API SoRedAndBlueKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoRedAndBlueKit);

public:
    static void initClass(             //static class initialization
        );
    SoRedAndBlueKit(                   //constructor
        );
    SoContextKit* factory(             //override this to create new context
        );                             //of a specific type
    ~SoRedAndBlueKit(
        );
    static void colorButtonCB(         //callback for color buttons
        void* data,                    //user data
        So3DButton* button             //pointer to pressed button
        );

private:
    SbBool checkPipGeometry();

    SbBool checkWindowGeometry();

    virtual void setSheetMasterMode(   //master/slave mode for pip sheet
        SoNode* pipSheet,              //override this to implement your
        SbBool masterMode              //behavior
        );               
}; //class SoRedAndBlueKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOREDANDBLUEKIT_H_

