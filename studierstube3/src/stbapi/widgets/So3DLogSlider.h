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
//  CONTENT:    So3DLogSlider - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw              Hermann Wurnig
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:28  gh      last modification
// ===========================================================================
#ifndef __SO3DLOGSLIDER_H__
#define __SO3DLOGSLIDER_H__

#include <stbapi/widgets/So3DSlider.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DLogSlider;

typedef void So3DLogSliderCB(void *userData, So3DLogSlider *slider);

/**
 * Contains all functions for handling a 3D-Slider. Derived from
 * So3DLogSlider. Every position between minValue and maxValue 
 * can be taken. The values between minValue and maxValue are
 * not intepolated linearly, but logarithmically
 * Documented at
 * http://www.studierstube.org/stbapi/So3DLogSlider.html
 * @ingroup oldwidgets
 * @ingroup deprecated
 */
class STBAPI_API So3DLogSlider : public So3DSlider {

	SO_KIT_HEADER(So3DLogSlider);

  public:
	
		// member-variables  

		// member-methods
	static void	initClass();	
	SoNode	   *copy(SbBool copyConnections) const;	
				So3DLogSlider();		
				So3DLogSlider(float width, float radius,
						   float position=0);		
				So3DLogSlider(float width, float radius,
						   float min, float max, float position=0);		

	virtual void setPosition(float newValue);
	float		getPosition();
    virtual void setPositionUpdate(float newValue);
	virtual void setCurrentValue(float newValue, SbBool forceUpdate=FALSE);
	float		getCurrentValue();
	virtual void setMinMax(float min, float max);

	void		addPositionChangeCallback(So3DLogSliderCB *funcCB, void *userData);
	SbBool		removePositionChangeCallback(So3DLogSliderCB *funcCB);
    static void snapBack(void *data,
		SoSensor *sensor
		);

  protected:
		// member-variables
	static int initClassAtLog;
		// member-functions
	static void positionChangeCB(void *userData, SoSensor *);


  private:
		// member-functions
	virtual	   ~So3DLogSlider();	
	void		initFields(float width, float radius,
						   float pos, float min, float max);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

