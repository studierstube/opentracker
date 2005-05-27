/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
//  PROJECT:    Studierstube
//  CONTENT:    Class definition of SoTrakEngine
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    alf     Anton Fuhrmann
//              ds      Dieter Schmalstieg
//              az      Andreas Zajic
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _SOTRAKENGINE_H_
#define _SOTRAKENGINE_H_

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFRotation.h>

#include <stbapi/stbapi.h>
//#include <stbapi/event3d/Stb3DEventGenerator.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * @ingroup tracking
 */
class STBAPI_API SoTrakEngine 
:
    public SoEngine 
{
    SO_ENGINE_HEADER(SoTrakEngine);

public:

//--Input:
    //SoSFTime        timeIn;         // time trigger
    SoSFInt32       station;        // tracker station
    SoSFVec3f       translationIn;  // input translation
    SoSFRotation    rotationIn;     // input rotation
    SoSFBool        buttonIn0;      // input button 0
    SoSFBool        buttonIn1;      // input button 1
    SoSFBool        buttonIn2;      // input button 2
    SoSFBool        buttonIn3;      // input button 3
    SoSFBool        buttonIn4;      // input button 4
    SoSFBool        buttonIn5;      // input button 5
    SoSFBool        buttonIn6;      // input button 6
    SoSFBool        buttonIn7;      // input button 7

//--Output:
    SoEngineOutput  translation;    // (SoSFVec3f) tracker translation
    SoEngineOutput  rotation;       // (SoSFRotation) tracker rotation
    SoEngineOutput  button0;        // (SoSFBool) tracker button 0 value
    SoEngineOutput  button1;        // (SoSFBool) tracker button 1 value
    SoEngineOutput  button2;        // (SoSFBool) tracker button 2 value
    SoEngineOutput  button3;        // (SoSFBool) tracker button 3 value
    SoEngineOutput  button4;        // (SoSFBool) tracker button 4 value
    SoEngineOutput  button5;        // (SoSFBool) tracker button 5 value
    SoEngineOutput  button6;        // (SoSFBool) tracker button 6 value
    SoEngineOutput  button7;        // (SoSFBool) tracker button 7 value

    static void initClass(          // call before first use
        );
    SoTrakEngine(                   // constructor 
        );
    
    /** SoTrakEngine overrides the standard SoEngine writing to avoid problems with 
      * global fields. It destroys the incoming connections and rebuilds them after
      * writing. This avoids writing out the global fields and furthermore related
      * problems in reading them in again. 
      */
    virtual void writeInstance( SoOutput * out );

private:
    virtual ~SoTrakEngine(          // destructor
        );
    virtual void evaluate(          // engine's evaluate method
        );
    void connectFields(             // connect fields for given station
        int i                       // station number
        );
    virtual void inputChanged(      // react to input changes
        SoField* whichField         // field that has changed
        );

}; // class SoTrakEngine

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOTRAKENGINE_H_

