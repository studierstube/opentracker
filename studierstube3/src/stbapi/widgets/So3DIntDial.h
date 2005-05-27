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
//  CONTENT:    So3DIntDial - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw              Hermann Wurnig
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:28  gh      last modification
// ===========================================================================
#ifndef __SO3DINTDIAL_H__
#define __SO3DINTDIAL_H__

#include <stbapi/widgets/SoBase3DDial.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So3DIntDial;

typedef void So3DIntDialCB(void *userData, So3DIntDial *button);

/**
 * Contains all functions for handling a 3D-DialButton. Derived
 * from SoBase3DDial. This dial can only be set to a given nuber 
 * of discrete positions (currentValue can be every integer between
 * minValue and maxValue.
 * Documented at
 * http://www.studierstube.org/stbapi/So3DIntDial.html
 * @ingroup oldwidgets
 * @ingroup deprecated
 */
class STBAPI_API So3DIntDial : public SoBase3DDial {

	SO_KIT_HEADER(So3DIntDial);

  public:
		// member-variables
	SoSFInt32 minValue;
	SoSFInt32 maxValue;
	SoSFInt32 currentValue;

		// member-methods
	static void initClass();	
	void		initFields(float radius, float height, int pos, 
						   int min, int max);

				So3DIntDial();				
				So3DIntDial(float radius, float height);
				So3DIntDial(float radius, float height, int min, 
						 int max, int position);

	virtual void		setPosition(float newValue);
    virtual void        setPositionUpdate(float newValue);
	float		getPosition();
	virtual void		setCurrentValue(int newValue, SbBool forceUpdate=FALSE);
	int			getCurrentValue();
	virtual void		setMinMax(int min, int max);

	void		addDialChangeCallback(So3DIntDialCB *funcCB, void *userData);
	SbBool		removeDialChangeCallback(So3DIntDialCB *funcCB);
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
	virtual	   ~So3DIntDial();	

	SoFieldSensor *positionSensor;
    int currentSetValue;
	SbBool validFlag;

  private:
		// member-methods

	  float oldPos;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
