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
//  CONTENT:    So3DDial - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw              Hermann Wurnig
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:28  gh      last modification
// ===========================================================================
#ifndef __SO3DDIAL_H__
#define __SO3DDIAL_H__

#include <stbapi/widgets/SoBase3DDial.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DDial;

typedef void So3DDialCB(void *userData, So3DDial *button);

/**
 * Class: So3DDial
 * Contains all functions for handling a 3D-DialButton. Derived
 * from SoBase3DDial. Any float-position between minValue and 
 * maxValue can be taken. 
 * Documented at
 * http://www.studierstube.org/stbapi/So3DDial.html
 * @ingroup oldwidgets
 * @ingroup deprecated
 */
class STBAPI_API So3DDial : public SoBase3DDial {

	SO_KIT_HEADER(So3DDial);

  public:
		// member-variables
	SoSFFloat minValue;
	SoSFFloat maxValue;
	SoSFFloat currentValue;

		// member-methods
	static void initClass();	
	void		initFields(float radius, float height, float pos, 
						   float min, float max);

				So3DDial();				
				So3DDial(float radius, float height);
				So3DDial(float radius, float height, float min, 
						 float max, float position);

	virtual void		setPosition(float newValue);
    virtual void        setPositionUpdate(float newValue);
	float		getPosition();
	virtual void		setCurrentValue(float newValue, SbBool forceUpdate=FALSE);
	float		getCurrentValue();
	virtual void		setMinMax(float min, float max);

	void		addDialChangeCallback(So3DDialCB *funcCB, void *userData);
	SbBool		removeDialChangeCallback(So3DDialCB *funcCB);
    virtual void        updateFromNetwork(float newValue);
    static void snapBack(void *data,
		SoSensor *sensor
		);

  protected:
		// member-variables
	static int initClassAt;
		// member-methods
	static void positionChangeCB(void *userData, SoSensor *);
	static void minmaxChangeCB(void *userData, SoSensor *);
	virtual	   ~So3DDial();	

	SoFieldSensor *positionSensor;
    float currentSetValue;
	SbBool validFlag;

  private:
		// member-methods

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

