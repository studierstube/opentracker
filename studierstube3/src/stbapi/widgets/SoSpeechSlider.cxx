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
/** source file for SoSlider class.
  *
  * @author Reinhard Steiner
  *
  * $Id: SoSpeechSlider.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#include <stbapi/widgets/SoSpeechSlider.h>


#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoSpeechSlider);


/**
 * the open inventor initialization
 */
void SoSpeechSlider::initClass()
{
    SO_BEHAVIOR_INIT_CLASS(SoSpeechSlider, SoSlider, "SoSlider");
}


/**
 * the constructor inits all fields
 */
SoSpeechSlider::SoSpeechSlider()
{
  
  SO_BEHAVIOR_CONSTRUCTOR(SoSpeechSlider);
    
  // add fields
  SO_BEHAVIOR_ADD_FIELD(actionUpGestures, (NULL) ); 
  SO_BEHAVIOR_ADD_FIELD(actionDownGestures, (NULL) ); 
  SO_BEHAVIOR_ADD_FIELD(step, (0.1) ); 
  
  SO_BEHAVIOR_INIT_INSTANCE();
  
  // setting up the connections:
  //              ,----------,            ,-----------,
  // translation->|          |---->alpha->|           |
  //    rotation->|constraint|  minValue->|interpolate|->value
  //              |          |  maxValue->|           |
  //              |          |            `-----------´
  //              |          |-------------------------->[trafo->trans]
  //              |          |-------------------------->[trafo->rot]
  //              `----------´                
}


SoSpeechSlider::~SoSpeechSlider()
{
}


void SoSpeechSlider::handle3DEvent(SoHandle3DEventAction *h3a)
{
	// to prevent the button's geometry to switch abruptly
	// between different geometry states, while this method
	// is executed, we set the state variables (on,highlighted)
	// first and adapt the geometry finally.

	// treat the button's highlight state

	if(detectAction(actionUpGestures))
	{
    float temp = alpha.getValue() + step.getValue();

    if(temp > 1.0f)
      setAlpha(1.0f);
    else
      setAlpha(temp);
	}
	else if(detectAction(actionDownGestures))
	{
    float temp = alpha.getValue() - step.getValue();

    if(temp < 0.0f)
      setAlpha(0.0f);
    else
      setAlpha(temp);
	}
  else
    SoDraggerKit::handle3DEvent(h3a);
}
