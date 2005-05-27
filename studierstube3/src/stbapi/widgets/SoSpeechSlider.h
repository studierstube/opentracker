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
/** class definition of SoSpeechSlider
  *
  * @author  Reinhard Steiner
  *
  * $Id: SoSpeechSlider.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_SPEECHSLIDER_
#define _SO_SPEECHSLIDER_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/engines/SoInterpolate.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFName.h>
#include <stbapi/interaction/SoDraggerKit.h>
#include <stbapi/widgets/SoSlider.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * @ingroup widgets
 */
class STBAPI_API SoSpeechSlider : public SoSlider
{
	SO_BEHAVIOR_HEADER(SoSpeechSlider);

  SoMFNode actionUpGestures;
  SoMFNode actionDownGestures;
  SoSFFloat step;

public:

	static void initClass();

	SoSpeechSlider();
	virtual ~SoSpeechSlider();
  
  float getStep()
  {
     return step.getValue();  
  };

	/**
	 * the slider's state-machine
	 */
	void handle3DEvent(SoHandle3DEventAction *h3a);

private:


};

#endif

