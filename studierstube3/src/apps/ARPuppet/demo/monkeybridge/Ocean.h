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
/* Ocean.h contains the ocean component
 *
 * This code is based on the program of Carsten Dachsbacher, 2001
 *
 * @author Istvan Barakonyi
 *
 * $Id: Ocean.h 4209 2005-05-19 09:06:39Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _OCEAN_H_
#define _OCEAN_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/actions/SoGLRenderAction.h>

#ifdef __COIN__
#include <Inventor/system/gl.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

// for the DLL export
#include "monkeybridge.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// Data structure for wave parameters
typedef struct
{
	float direction;
	float dirX, dirY;
	float lambda;		// wavelenght
	float k;			// wave number
	float omega;		// angular velocity
	float freq;			// frequency
	float period;		// period
	float amplitude;	
	float phase;
} WAVE;


/** The Ocean node implements a dynamic water surface simulating an ocean
    The algorithm is based on the Pierson-Moskowitz frequency distribution model
    and the Mitsuyasu formula for angular distribution.
    A nice tutorial on the topic is given on the following website: 
    http://www.naturewizard.com/tutorial02.html

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class MONKEYBRIDGE_API Ocean : public SoNode
{
    SO_NODE_HEADER(Ocean);

public:

    /// Scaling ocean model (connected to global game scaling parameter)
    SoSFFloat scale;

    /// Wave generation parameters
    /*@{*/

    SoSFInt32 waveResolution;
    SoSFVec2f meshResolution;
    SoSFFloat windSpeed;
    SoSFFloat windDirection;
    SoSFFloat gravity;
    SoSFFloat alpha;
    SoSFFloat speed;

    /*@}*/

    /// Wave rendering parameters
    /*@{*/

    SoSFColor color;
    SoSFFloat transparency;

    /*@}*/

    Ocean();

SoINTERNAL public:

    static void initClass();

protected:

    SoFieldSensor *meshResolutionSensor;
    SoFieldSensor *waveResolutionSensor;
    SoFieldSensor *windSpeedSensor;
    SoFieldSensor *windDirectionSensor;
    SoFieldSensor *gravitySensor;

    SoTimerSensor *timerSensor;

    /// Indicates whether we have already initialized our component or not
    bool initialized;

    /// Wave generation helper variables
    float *waterHeight;
    SbVec3f *waterNormal;
    WAVE *wave;
    int nWaves;
    float freqPeak;

    virtual ~Ocean();

    /// Rendering loop
    virtual void GLRender(SoGLRenderAction *action);

    /// Static field sensor callback function
    static void initCB(void *data, SoSensor *sensor);

    /// Timer sensor callback function
    static void timerSensorCB(void *data, SoSensor *sensor);
    
    /// Wave animation and rendering cycle
    void animate();

    /// Approximation of the gamma function
    float gamma(float y);

    float directionEnergy(float f,float theta);
    float energy1DFreq(float f);
    float energy2DFreqAngle(float f,float theta);
    float waveAmplitude(float f,float theta,float k);
    
    int ax(int x);
    int ay(int y);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _OCEAN_H_
