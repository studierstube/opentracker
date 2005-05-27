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
/** source file for SoSimpleSlider class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoSimpleSlider.cxx 4196 2005-05-07 19:02:54Z kaufmann $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodes/SoCube.h>

#include <stbapi/widgets/SoSimpleSlider.h>
#include <stbapi/widgets/SoLabelKit.h>


//#define _DEBUG_SIMPLE_SLIDER

#if defined(_DEBUG_SIMPLE_SLIDER) && defined(_DEBUG)
#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif



#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoSimpleSlider);


/**
 * the open inventor initialization
 */
void SoSimpleSlider::initClass()
{
    SO_BEHAVIOR_INIT_CLASS(SoSimpleSlider, SoSlider, "SoSlider");
}



/**
 * the constructor inits all fields
 */
SoSimpleSlider::SoSimpleSlider()
{
  
  SO_BEHAVIOR_CONSTRUCTOR(SoSimpleSlider);
  
  // Add the body parts to the catalog...
  //-----------------------+------------+------+---------------+-------+---------------
  //                       |            |      |               | right |
  //                       |            | null |               | Sib-  | is
  //                       |            | By   |               | ling  | public
  //         name          | className  | Def. |  parent Name  | Name  | part
  //-----------------------+------------+------+---------------+-------+---------------

  // add fields
  SO_BEHAVIOR_ADD_FIELD(valueIn, (0.0)); 
  SO_BEHAVIOR_ADD_FIELD(alphaIn, (0.0));
  SO_BEHAVIOR_ADD_FIELD(widthDragger, (0.05));
  SO_BEHAVIOR_ADD_FIELD(scaleDraggerPath, (0.8));

  SO_BEHAVIOR_INIT_INSTANCE();
  
  // set up default geometry for dragger part

  setPart("normalGeometry",		 getGeometryFromString(normalString) );
  setPart("highlightedGeometry", getGeometryFromString(highlightedString) );
  setPart("draggedGeometry",	 getGeometryFromString(highlightedString) );
  setPart("disabledGeometry",	 getGeometryFromString(disabledString)  );
  setPart("staticGeometry",				getGeometryFromString(staticString) );
  setPart("staticGeometryHighlight",	getGeometryFromString(staticString) );
  setPart("staticGeometryDisabled",		getGeometryFromString(staticString) );
  
  /*
  // set up default geometry for static part
  SoShapeKit *sBase = SO_GET_PART(this, "staticBase", SoShapeKit);
  assert(sBase != NULL);
  SoCube *sBaseCube = new SoCube();
  sBaseCube->width.setValue(1);
  sBaseCube->depth.setValue(1);
  sBaseCube->height.setValue(.2);
  
  sBase->setPart("shape", sBaseCube);
  SoTransform	*sTransform = SO_GET_PART(sBase, "localTransform", SoTransform);
  assert(sTransform != NULL);
  sTransform->translation.setValue(0,.1,0);
  
  SoShapeKit *sSlide  = SO_GET_PART(this, "staticSlide", SoShapeKit);
  assert(sSlide != NULL);
  SoCube *sSlideCube = new SoCube();

  sSlideCube->width.setValue(1);
  sSlideCube->height.setValue(.1);
  sSlideCube->depth.setValue(.1);
  sSlide->setPart("shape", sSlideCube);

  sTransform = SO_GET_PART(sSlide, "localTransform", SoTransform);
  assert(sTransform != NULL);
  sTransform->translation.setValue(0,.25,0);

  SoMaterial *sMaterial = SO_GET_PART(sSlide, "material", SoMaterial);
  assert(sMaterial!= NULL);
  sMaterial->diffuseColor.setValue(.2, .2, .8);
*/
  
  /*
  SbRotation rot1(SbVec3f(1,0,0), -1.57);
  SbRotation rot2(SbVec3f(0,1,0), 1.57);
  SbRotation rot = rot1 * rot2;

  SbVec3f axVec;
  float ang;

  rot.getValue(axVec, ang);
  char buffer[200];
  float x,y,z;
  axVec.getValue(x,y,z);
  sprintf (buffer, "Transform {translation 0 0.51 7  rotation %f %f %f %f }", x,y,z,ang);
  setPart("label.transform" ,getGeometryFromString(buffer));
*/
  // down side of slider
 // setPart("label.transform" ,getGeometryFromString(" Transform {translation 0 0.51 7  rotation 1 0 0 -1.57 }"));
  // left side of slider
  //if (getPart("label.transform", FALSE) != NULL) setPart("label.transform" ,getGeometryFromString(" Transform {translation -3 0.51 0  rotation -0.5775 0.5775 0.5775  2.0935 }"));

  // setting up the connection between the value and the
  // valueChanged callback

  valueInSensor.setData(this);
  valueInSensor.setFunction(valueInChangedCB);

  alphaInSensor.setData(this);
  alphaInSensor.setFunction(alphaInChangedCB);

//  widthSensor.setData(this);
//  widthSensor.setFunction(widthChangedCB);

  setUpConnections(TRUE, TRUE);
}


SoSimpleSlider::~SoSimpleSlider()
{
}

// --------------------------------------------------------------------------

SbBool SoSimpleSlider::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoSlider::setUpConnections(onOff, doItAlways);
        // setup the widget
		
		valueInSensor.attach(&valueIn);
		alphaInSensor.attach(&alphaIn); 
//		widthSensor.attach(&width);
//		widthSensor.schedule();
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.

		valueInSensor.detach();
		alphaInSensor.detach();  
//		widthSensor.detach(); 

	    SoSlider::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);

}

// --------------------------------------------------------------------------
void SoSimpleSlider::resizeWidgetCB()
{
	DEBUG_CODE(printf("SoSimpleSlider::resizeWidgetCB\n"););
	SoBehaviorKit::resizeWidgetCB();

//	SoSimpleSlider *self = (SoSimpleSlider *) data;
//	assert(self!=NULL);
	float widthV = widgetDim[0];
//	float widthV = width.getValue();
	// +/- .5 as the relative width is 1
	// +/- 0.05 for the width of the dragger part
	float widthDraggerV = widthDragger.getValue();
	float scaleDraggerPathV = scaleDraggerPath.getValue();

//	float minx = (-.5f + widthDraggerV ) * scaleDraggerPathV * widthV;
//	float maxx = (+.5f - widthDraggerV ) * scaleDraggerPathV * widthV;
    // set slider by default to left value and 
    // not into the middle of the slider bar
    float minx = (0.0f + widthDraggerV ) * scaleDraggerPathV * widthV;
    float maxx = (1.0f - widthDraggerV ) * scaleDraggerPathV * widthV;
    //widgetMove[0] - widthV /2.0 +
	minVector.setValue( minx,0,0);
	maxVector.setValue(maxx,0,0);

}
/*
void SoSimpleSlider::widthChangedCB(void *data, SoSensor *)
{
}
  */
// --------------------------------------------------------------------------

void SoSimpleSlider::valueInChangedCB(void *data, SoSensor *)
{
	SoSimpleSlider *self = (SoSimpleSlider *) data;
	// if valueIn and value have the same value, do not update !!
	if (self->valueIn.getValue() == self->value.getValue()) return;
	float min = self->minValue.getValue() ;
	float scale = self->maxValue.getValue() - min;
	float value = self->valueIn.getValue() ;

 	self->setAlpha((value-min)/scale);
//	self->valueInSensor.detach();
//	self->valueIn.setValue(self->value.getValue());
//	self->valueInSensor.attach(&self->valueIn);

//    printf("valueInChangedCB %f\n",value);
}
// --------------------------------------------------------------------------

void SoSimpleSlider::alphaInChangedCB(void *data, SoSensor *)
{
  SoSimpleSlider *self = (SoSimpleSlider*)data;
  if (self->alpha.getValue() == self->alphaIn.getValue()) 
	return;

  self->setAlpha(self->alphaIn.getValue());
  //self->alphaInSensor.detach();
  //self->alphaIn.setValue(self->alpha.getValue());
  //self->alphaInSensor.attach(&self->alphaIn);
  // printf("alphaChangedCB %f\n", self->alphaOutput.getValue());
}


	
// --------------------------------------------------------------------------

char *SoSimpleSlider::normalString=
"Separator {"
"           Transform{ translation 0 0.45 0 rotation 0 1 0 1.57} "
"           Material {"
"                    diffuseColor 0.82813 0.8125 0.78125"
"           }"
"            Coordinate3 {"
"                    point ["
"                        -0.37501 -0.20967 0.05001,"
"                        0.37498 -0.20967 0.05001,"
"                        0.37498 0.45616 0.03725,"
"                        -0.37501 0.45616 0.03725,"
"                        0.37498 0.55032 0.03131,"
"                        -0.37501 0.55032 0.03131,"
"                        0.37498 0.55032 0.01232,"
"                        -0.37501 0.55032 0.01232,"
"                        -0.37501 0.50100 0.00639,"
"                        0.37498 0.50100 0.00639,"
"                        0.37498 0.50100 -0.00607,"
"                        -0.37501 0.50100 -0.00607,"
"                        0.37498 0.54361 -0.01289,"
"                        -0.37501 0.54361 -0.01289,"
"                        0.37498 0.54361 -0.03069,"
"                        -0.37501 0.54361 -0.03069,"
"                        0.37498 0.45393 -0.03782,"
"                        -0.37501 0.45393 -0.03782,"
"                        0.37498 -0.20745 -0.04998,"
"                        -0.37501 -0.20745 -0.04998"
"                        ]"
"                }"
"           IndexedFaceSet {"
"                coordIndex ["
"                    8,19,0,-1,"
"                    7,8,0,-1,"
"                    3,5,7,-1,"
"                    0,3,7,-1,"
"                    17,8,11,-1,"
"                    17,19,8,-1,"
"                    11,15,17,-1,"
"                    11,13,15,-1,"
"                    18,9,1,-1,"
"                    9,6,1,-1,"
"                    4,2,6,-1,"
"                    2,1,6,-1,"
"                    9,16,10,-1,"
"                    18,16,9,-1,"
"                    14,10,16,-1,"
"                    12,10,14,-1,"
"                    0,19,1,-1,"
"                    19,18,1,-1,"
"                    19,17,18,-1,"
"                    17,16,18,-1,"
"                    17,15,16,-1,"
"                    15,14,16,-1,"
"                    15,13,14,-1,"
"                    13,12,14,-1,"
"                    13,11,12,-1,"
"                    11,10,12,-1,"
"                    11,8,10,-1,"
"                    8,9,10,-1,"
"                    8,6,9,-1,"
"                    7,6,8,-1,"
"                    7,5,6,-1,"
"                    5,4,6,-1,"
"                    5,3,4,-1,"
"                    3,2,4,-1,"
"                    3,0,2,-1,"
"                    0,1,2,-1"
"                    ]"
"            }"
"}";


// --------------------------------------------------------------------------

char *SoSimpleSlider::highlightedString=
"Separator {"
"           Transform{ translation 0 0.45 0 rotation 0 1 0 1.57 }"
"           Material {"
"                    diffuseColor .776 1 0"
"           }"
"            Coordinate3 {"
"                    point ["
"                        -0.37501 -0.20967 0.05001,"
"                        0.37498 -0.20967 0.05001,"
"                        0.37498 0.45616 0.03725,"
"                        -0.37501 0.45616 0.03725,"
"                        0.37498 0.55032 0.03131,"
"                        -0.37501 0.55032 0.03131,"
"                        0.37498 0.55032 0.01232,"
"                        -0.37501 0.55032 0.01232,"
"                        -0.37501 0.50100 0.00639,"
"                        0.37498 0.50100 0.00639,"
"                        0.37498 0.50100 -0.00607,"
"                        -0.37501 0.50100 -0.00607,"
"                        0.37498 0.54361 -0.01289,"
"                        -0.37501 0.54361 -0.01289,"
"                        0.37498 0.54361 -0.03069,"
"                        -0.37501 0.54361 -0.03069,"
"                        0.37498 0.45393 -0.03782,"
"                        -0.37501 0.45393 -0.03782,"
"                        0.37498 -0.20745 -0.04998,"
"                        -0.37501 -0.20745 -0.04998"
"                        ]"
"                }"
"           IndexedFaceSet {"
"                coordIndex ["
"                    8,19,0,-1,"
"                    7,8,0,-1,"
"                    3,5,7,-1,"
"                    0,3,7,-1,"
"                    17,8,11,-1,"
"                    17,19,8,-1,"
"                    11,15,17,-1,"
"                    11,13,15,-1,"
"                    18,9,1,-1,"
"                    9,6,1,-1,"
"                    4,2,6,-1,"
"                    2,1,6,-1,"
"                    9,16,10,-1,"
"                    18,16,9,-1,"
"                    14,10,16,-1,"
"                    12,10,14,-1,"
"                    0,19,1,-1,"
"                    19,18,1,-1,"
"                    19,17,18,-1,"
"                    17,16,18,-1,"
"                    17,15,16,-1,"
"                    15,14,16,-1,"
"                    15,13,14,-1,"
"                    13,12,14,-1,"
"                    13,11,12,-1,"
"                    11,10,12,-1,"
"                    11,8,10,-1,"
"                    8,9,10,-1,"
"                    8,6,9,-1,"
"                    7,6,8,-1,"
"                    7,5,6,-1,"
"                    5,4,6,-1,"
"                    5,3,4,-1,"
"                    3,2,4,-1,"
"                    3,0,2,-1,"
"                    0,1,2,-1"
"                    ]"
"            }"
"}";

// --------------------------------------------------------------------------

char *SoSimpleSlider::disabledString=
"Separator {"
"           Transform{ translation 0 0.45 0 rotation 0 1 0 1.57}"
"           Material {"
"                    diffuseColor 0.82813 0.8125 0.78125 transparency .5"
"           }"
"            Coordinate3 {"
"                    point ["
"                        -0.37501 -0.20967 0.05001,"
"                        0.37498 -0.20967 0.05001,"
"                        0.37498 0.45616 0.03725,"
"                        -0.37501 0.45616 0.03725,"
"                        0.37498 0.55032 0.03131,"
"                        -0.37501 0.55032 0.03131,"
"                        0.37498 0.55032 0.01232,"
"                        -0.37501 0.55032 0.01232,"
"                        -0.37501 0.50100 0.00639,"
"                        0.37498 0.50100 0.00639,"
"                        0.37498 0.50100 -0.00607,"
"                        -0.37501 0.50100 -0.00607,"
"                        0.37498 0.54361 -0.01289,"
"                        -0.37501 0.54361 -0.01289,"
"                        0.37498 0.54361 -0.03069,"
"                        -0.37501 0.54361 -0.03069,"
"                        0.37498 0.45393 -0.03782,"
"                        -0.37501 0.45393 -0.03782,"
"                        0.37498 -0.20745 -0.04998,"
"                        -0.37501 -0.20745 -0.04998"
"                        ]"
"                }"
"           IndexedFaceSet {"
"                coordIndex ["
"                    8,19,0,-1,"
"                    7,8,0,-1,"
"                    3,5,7,-1,"
"                    0,3,7,-1,"
"                    17,8,11,-1,"
"                    17,19,8,-1,"
"                    11,15,17,-1,"
"                    11,13,15,-1,"
"                    18,9,1,-1,"
"                    9,6,1,-1,"
"                    4,2,6,-1,"
"                    2,1,6,-1,"
"                    9,16,10,-1,"
"                    18,16,9,-1,"
"                    14,10,16,-1,"
"                    12,10,14,-1,"
"                    0,19,1,-1,"
"                    19,18,1,-1,"
"                    19,17,18,-1,"
"                    17,16,18,-1,"
"                    17,15,16,-1,"
"                    15,14,16,-1,"
"                    15,13,14,-1,"
"                    13,12,14,-1,"
"                    13,11,12,-1,"
"                    11,10,12,-1,"
"                    11,8,10,-1,"
"                    8,9,10,-1,"
"                    8,6,9,-1,"
"                    7,6,8,-1,"
"                    7,5,6,-1,"
"                    5,4,6,-1,"
"                    5,3,4,-1,"
"                    3,2,4,-1,"
"                    3,0,2,-1,"
"                    0,1,2,-1"
"                    ]"
"            }"
"}";

// --------------------------------------------------------------------------

char *SoSimpleSlider::staticString=
"Separator {"
"           Transform{ translation 0 0.66692 0 rotation 0 1 0 1.57}"
"           Material {"
"                    diffuseColor 0.5 0.5 0.5"
"           }"
"            Coordinate3 {"
"                    point ["
"                        -0.5 -0.43692 -0.50000,"
"                        -0.5 -0.66692 -0.50000,"
"                        -0.5 -0.43692 0.49999,"
"                        -0.5 -0.66692 0.49999,"
"                        0.49999 -0.43692 0.49999,"
"                        0.49999 -0.66692 0.49999,"
"                        0.49999 -0.43692 -0.50000,"
"                        0.49999 -0.66692 -0.50000,"
"                        0.05436 -0.43692 -0.43868,"
"                        0.05436 -0.43692 -0.05436,"
"                        -0.05436 -0.43692 0.05436,"
"                        -0.05436 -0.43692 -0.43868,"
"                        -0.05436 -0.43692 0.43868,"
"                        0.05436 -0.43692 0.43868,"
"                        0.05436 -0.60898 -0.43868,"
"                        -0.05436 -0.60898 -0.43868,"
"                        0.05436 -0.60898 0.43868,"
"                        -0.05436 -0.60898 0.43868"
"                        ]"
"                }"
"           IndexedFaceSet {"
"                coordIndex ["
"                    15,11,12,-1,"
"                    17,15,12,-1,"
"                    16,13,8,-1,"
"                    14,16,8,-1,"
"                    15,14,11,-1,"
"                    8,11,14,-1,"
"                    12,13,17,-1,"
"                    16,17,13,-1,"
"                    17,16,15,-1,"
"                    14,15,16,-1,"
"                    12,4,13,-1,"
"                    4,6,13,-1,"
"                    9,13,6,-1,"
"                    10,2,12,-1,"
"                    2,4,12,-1,"
"                    8,0,11,-1,"
"                    0,2,11,-1,"
"                    10,11,2,-1,"
"                    9,6,8,-1,"
"                    6,0,8,-1,"
"                    7,5,1,-1,"
"                    3,1,5,-1,"
"                    1,0,7,-1,"
"                    6,7,0,-1,"
"                    7,6,5,-1,"
"                    4,5,6,-1,"
"                    5,4,3,-1,"
"                    2,3,4,-1,"
"                    3,2,1,-1,"
"                    0,1,2,-1"
"                    ]"
"            }"
"}";


