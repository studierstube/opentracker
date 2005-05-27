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
//  CONTENT:    So3DTouchSlider - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    jp      Jan Prikryl
// ===========================================================================
//  HISTORY:
//
//  29-Sep-00 11:11:48  jp      copied template from So3DSlider.h
// ===========================================================================

#ifndef __So3DTouchSlider_H__
#define __So3DTouchSlider_H__

#include <stbapi/widgets/SoBase3DTouchSlider.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DTouchSlider;

typedef void So3DTouchSliderCB(void *userData, So3DTouchSlider *slider);

////////////////////////////////////////////////////////////////////
//
//  Class: So3DTouchSlider
//
//  Contains all functions for handling a 3D-Slider. Derived from
//  SoBase3DTouchSlider. Every position between minValue and maxValue 
//  can be taken.
//
////////////////////////////////////////////////////////////////////
/**
 * @ingroup oldwidgets
 * @ingroup deprecated
 */
class STBAPI_API So3DTouchSlider : public SoBase3DTouchSlider {
    
    SO_KIT_HEADER(So3DTouchSlider);
    
public:
    
    // member-variables  
    SoSFFloat   minValue;
    SoSFFloat   maxValue;
    SoSFFloat   currentValue;
    SoSFFloat   increment;
    
    // member-methods
    static void initClass();    
    
    // constructors
    So3DTouchSlider();      
    So3DTouchSlider(float width, float radius, float position=0);       
    So3DTouchSlider(float width, float radius, 
                    float min, float max, float position=0);        
    
    virtual void setPosition(float newValue);
    float        getPosition();
    virtual void setCurrentValue(float newValue);
    float        getCurrentValue();
    virtual void setMinMax(float min, float max);
    
    void   setIncrement(float value);
    float  getIncrement();
    float  getNormalizedIncrement();
    
    void   addPositionChangeCallback(So3DTouchSliderCB *funcCB, void *userData);
    SbBool removePositionChangeCallback(So3DTouchSliderCB *funcCB);
   
protected:
    // member-variables
    static int  initClassAttemptsSlider;
    
    // member-functions
    static void positionChangeCB(void *userData, SoSensor *);
    static void minmaxChangeCB(void *userData, SoSensor *);
    virtual    ~So3DTouchSlider();  
    
    SoFieldSensor *positionSensor;
    
private:
    // member-functions
    void initFields(float width, float radius,
                    float pos, float min, float max);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

