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
  * @author Zsolt Marx
  *
  * $Id: SoSlider.cxx 4047 2004-11-25 12:34:27Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSwitch.h>

#include <stbapi/widgets/SoSlider.h>
#include <stbapi/widgets/sliderGeometry/normalString.h>
#include <stbapi/widgets/sliderGeometry/highlightedString.h>
#include <stbapi/widgets/sliderGeometry/disabledString.h>
#include <stbapi/widgets/sliderGeometry/staticString.h>

#include <stbapi/misc/SoLineConstraint.h>
#include <stbapi/misc/SoConstraint.h>

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoSlider);


/**
 * the open inventor initialization
 */
void SoSlider::initClass()
{
    SO_BEHAVIOR_INIT_CLASS(SoSlider, SoDraggerKit, "SoDraggerKit");
}

/**
 * the constructor inits all fields
 */
SoSlider::SoSlider()
{
  
  SO_BEHAVIOR_CONSTRUCTOR(SoSlider);
  
  // Add the body parts to the catalog...
  //-----------------------+------------+------+---------------+-------+---------------
  //                       |            |      |               | right |
  //                       |            | null |               | Sib-  | is
  //                       |            | By   |               | ling  | public
  //         name          | className  | Def. |  parent Name  | Name  | part
  //-----------------------+------------+------+---------------+-------+---------------
  ADD(staticSeparator,		  SoSeparator,		FALSE,  base,			constrainedTransform, TRUE);
  ADD(staticScale,			  SoScale,			FALSE,  staticSeparator, \x0,   TRUE);
  ADD(staticSwitch,			  SoSwitch,			FALSE,	staticSeparator, \x0,   TRUE);
  ADD(staticGeometry,		  SoSeparator,		FALSE,  staticSwitch,    \x0,   TRUE);
  ADD(staticGeometryHighlight,SoSeparator,		FALSE,  staticSwitch,    \x0,   TRUE);
  ADD(staticGeometryDisabled, SoSeparator,		FALSE,  staticSwitch,    \x0,   TRUE);

  // add fields 
  
  SO_BEHAVIOR_ADD_FIELD(minValue, (0.0) ); 
  SO_BEHAVIOR_ADD_FIELD(maxValue, (20.0) ); 
  SO_BEHAVIOR_ADD_FIELD(minVector, (0.0, 0.0, -5.2)); 
  SO_BEHAVIOR_ADD_FIELD(maxVector, (0.0, 0.0, 5.2 )); 
  SO_BEHAVIOR_ADD_FIELD(value, (0.0)); 
  SO_BEHAVIOR_ADD_FIELD(valueOutput, (0.0)); 
  SO_BEHAVIOR_ADD_FIELD(alpha, (0.0));
  SO_BEHAVIOR_ADD_FIELD(alphaOutput, (0.0));
  SO_BEHAVIOR_ADD_FIELD(valueVec3f, (0,0,0));
  
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
  
  // make the static scale and the dragger scale use the same scale
  
  // set up connections
  
  
  // set up default geometry
  setPart("normalGeometry", getGeometryFromString(normalString) );
  setPart("highlightedGeometry", getGeometryFromString(highlightedString) );
  setPart("draggedGeometry", getGeometryFromString(highlightedString) );
  setPart("disabledGeometry", getGeometryFromString(disabledString)  );

  setPart("staticGeometry",			 getGeometryFromString(staticString) );
  setPart("staticGeometryHighlight", getGeometryFromString(staticString) );
  setPart("staticGeometryDisabled",  getGeometryFromString(staticString) );

  // valueChanged callback

  valueSensor.setData(this);
  valueSensor.setFunction(valueChangedCB);

  alphaSensor.setData(this);
  alphaSensor.setFunction(alphaChangedCB);
  
  setUpConnections(TRUE,TRUE);

  setAlpha(alpha.getValue(), true);

}


SoSlider::~SoSlider()
{
}


// --------------------------------------------------------------------------

SbBool SoSlider::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
		    // We connect AFTER base class.
		    SoDraggerKit::setUpConnections(onOff, doItAlways);
		
			constraint = new SoLineConstraint();  
			interpolator = new SoInterpolateFloat();
			interpolatorVec3f = new SoInterpolateVec3f();
	    
		  SoScale* sc = (SoScale*) scale.getValue();
 		  setPart("staticScale", sc);
		
        // setup the widget
		  SoLineConstraint* lineConstraint = (SoLineConstraint*)constraint;
		  lineConstraint->lineStart.connectFrom(&minVector);
		  lineConstraint->lineEnd.connectFrom(&maxVector);
		  lineConstraint->translationIn.connectFrom(&translation);
		  lineConstraint->rotationIn.connectFrom(&rotation);
  
		  // connect constraint output to transform
  
		  SoTransform* constrainedTrafo = (SoTransform*)constrainedTransform.getValue();
		  constrainedTrafo->translation.connectFrom(&lineConstraint->translationOut);
		  constrainedTrafo->rotation.connectFrom(&lineConstraint->rotationOut);
  
		  alphaOutput.connectFrom(&lineConstraint->alpha);

		  interpolator->alpha.connectFrom(&alpha);
		  interpolator->input0.connectFrom(&minValue);
		  interpolator->input1.connectFrom(&maxValue);

		  valueOutput.connectFrom(&interpolator->output);
	
		  interpolatorVec3f->alpha.connectFrom(&alpha);
		  interpolatorVec3f->input0.connectFrom(&minVector);
		  interpolatorVec3f->input1.connectFrom(&maxVector);
  
		  valueVec3f.connectFrom(&interpolatorVec3f->output);
	
		  valueSensor.attach(&value);
		  alphaSensor.attach(&alphaOutput);

		  adaptGeometryToState();
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.

		alpha.disconnect();
		value.disconnect();

        SoTransform* constrainedTrafo = (SoTransform*)constrainedTransform.getValue();
        constrainedTrafo->translation.disconnect();
        constrainedTrafo->rotation.disconnect();

        alphaOutput.disconnect();
        valueOutput.disconnect();
        valueVec3f.disconnect();

        valueSensor.detach();
        alphaSensor.detach();
  
	    SoDraggerKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);

}

void SoSlider::setAlpha(float p_Alpha, bool p_Force)
{
  //printf("setAlpha %f, current alpha = %f\n", p_Alpha, alpha.getValue());

  if(p_Alpha < 0) p_Alpha = 0;
  if(p_Alpha > 1) p_Alpha = 1;

  if(!p_Force)
  {
    // ignore if alpha has not changed
    float diff = alpha.getValue() - p_Alpha;
    if(diff > -0.001 && diff < 0.001)
      return;
  }
  
  alpha.setValue(p_Alpha);
  value.setValue(valueOutput.getValue());

  SbVec3f v = minVector.getValue() + ((maxVector.getValue() - minVector.getValue()) * alpha.getValue());
  translation.setValue(v);

  // fire the move callback 
  if(moveCallback) moveCallback(moveCallbackData, this);


  //float x, y, z;
  //valueVec3f.getValue().getValue(x, y, z);
  //printf("SetAlpha: %f, (%f,%f,%f)\n", p_Alpha, x, y, z);
  //printf("minVector(%f, %f, %f)\n", minVector.getValue()[0], minVector.getValue()[1], minVector.getValue()[2]);
  //printf("maxVector(%f, %f, %f)\n", maxVector.getValue()[0], maxVector.getValue()[1], maxVector.getValue()[2]);
  //printf("v(%f, %f, %f)", v[0], v[1], v[2]);

  //translation.setValue(valueVec3f.getValue());  
}


float SoSlider::getAlpha()
{
  return(alpha.getValue());
}

void SoSlider::valueChangedCB(void *data, SoSensor *)
{
	SoSlider *self = (SoSlider *) data;
	float min = self->minValue.getValue() ;
	float scale = self->maxValue.getValue() - min;
	float value = self->value.getValue();

 	self->setAlpha((value-min)/scale);
    // printf("valueChangedCB\n");
}

void SoSlider::alphaChangedCB(void *data, SoSensor *)
{
  SoSlider *self = (SoSlider*)data;
  self->setAlpha(self->alphaOutput.getValue());
   // printf("alphaChangedCB %f\n", self->alphaOutput.getValue());
}

/**
 * adapts the dragger's geometry to its state
 */
void SoSlider::adaptGeometryToState()
{
	SoDraggerKit::adaptGeometryToState();
	
	SoSwitch *staticSw = (SoSwitch *)staticSwitch.getValue();

	if (staticSw == NULL) return;

	if (enable.getValue() == TRUE)
	{
		if (highlighted.getValue() == TRUE) // highlighted and not dragging
			staticSw->whichChild.setValue(1);
		else
			staticSw->whichChild.setValue(0);
	}
	else
		staticSw->whichChild.setValue(2);
	
}

// -------------------------------------------------------------------------------
