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
/** SoBase3DTouchSlider - implementation
  *
  * @author  Jan Prikryl
  *
  * $Id: SoBase3DTouchSlider.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif
 
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/misc/SoChildList.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stbapi/widgets/SoBase3DTouchSlider.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>

int  SoBase3DTouchSlider::baseInitClassAttempts=0;

SO_KIT_SOURCE(SoBase3DTouchSlider);

BASE3D_SOURCE(SoBase3DTouchSlider);

void
SoBase3DTouchSlider::getBoundingBox(SoGetBoundingBoxAction *action)
{
  int numIndices;
  const int *indices;
  

  switch (action->getPathCode(numIndices, indices))
  {
    case SoAction::IN_PATH:
    {
      if (children!=NULL)
        children->traverse(action);
    }
	break;

	case SoAction::OFF_PATH:
    {
		if (children!=NULL)
         children->traverse(action,0);
    }
	break;

    case SoAction::BELOW_PATH:
    case SoAction::NO_PATH:
    { 
	  SbViewportRegion vp;
	  SoGetBoundingBoxAction bba(vp);
	  SoGetBoundingBoxAction bbb(vp);

      //condom Hack: if its a virgin make sure that it has the right geometry 
      //by setting the correct geometry before
      if (firstTime) 
      {
        this->enableNotify(FALSE);	//to make the Windows Inventor happy
        setupSliderMesh(this,NULL);    
        firstTime=FALSE;		//
        this->enableNotify(TRUE);  //to make the Windows Inventor happy
      }

      //extend boundingBox with existing bodyGeometry
      if (bodyGeometry.getValue()!=NULL)
        bba.apply(bodyGeometry.getValue());
      //extend boundingBox with existing sliderGeometry
      if (sliderGeometry.getValue()!=NULL)
        bbb.apply(sliderGeometry.getValue());
	  switch (action->getWhatReset())
	  {
        case SoGetBoundingBoxAction::ALL:
		{
          action->extendBy(bba.getXfBoundingBox()); 
          action->extendBy(bbb.getXfBoundingBox()); 
		}
        break;
        case SoGetBoundingBoxAction::BBOX:
		{
          action->extendBy(bba.getBoundingBox()); 
          action->extendBy(bbb.getBoundingBox()); 
		}
        break;
        case SoGetBoundingBoxAction::TRANSFORM:
        break;
	  }
    }
    break;
  }	

}

void 
SoBase3DTouchSlider::setupSliderMesh(void *data, SoSensor *)
{
	int i;
	
	SoBase3DTouchSlider *slider = (SoBase3DTouchSlider *) data;
		
	float width = slider->width.getValue();
	float sWidth = 0.18f*width;
	float height = slider->radius.getValue()*0.7f;
	float depth  = slider->radius.getValue();

	float vertex1Data[12][3]=
		{{-sWidth/2,-height/2,-depth/2}, 
		 {-sWidth/2,0,-depth/2},
		 {-sWidth/5,height/2,-depth/2},
		 {sWidth/5,height/2,-depth/2},
		 {sWidth/2,0,-depth/2},
		 {sWidth/2,-height/2,-depth/2}, 

		 {-sWidth/2,-height/2,depth/2}, 
		 {-sWidth/2,0,depth/2},
		 {-sWidth/5,height/2,depth/2},
		 {sWidth/5,height/2,depth/2},
		 {sWidth/2,0,depth/2},
		 {sWidth/2,-height/2,depth/2}}; 

	float vertex2Data[12][3]=
		{{sWidth/5,height/2,depth/2},
		 {sWidth/2,0,depth/2},
		 {sWidth/2,-height/2,depth/2}, 
		 {sWidth/2,-height/2,-depth/2}, 
		 {sWidth/2,0,-depth/2},
		 {sWidth/5,height/2,-depth/2},

		 {-sWidth/5,height/2,depth/2},
		 {-sWidth/2,0,depth/2},
		 {-sWidth/2,-height/2,depth/2}, 
		 {-sWidth/2,-height/2,-depth/2}, 
		 {-sWidth/2,0,-depth/2},
		 {-sWidth/5,height/2,-depth/2}};


	SoGroup *bGroup = new SoGroup;
	SoDrawStyle *bStyle = new SoDrawStyle;
	bStyle->style.setValue(SoDrawStyle::INVISIBLE); 
	bGroup->addChild(bStyle);

	SoCube *bCube = new SoCube;
	bCube->width=width/100;
	bCube->height=height;
	bCube->depth=depth;
	bGroup->addChild(bCube);

	bStyle = new SoDrawStyle;
	bStyle->style.setValue(SoDrawStyle::FILLED);
	bGroup->addChild(bStyle);
	
	SoTranslation *bTrans=new SoTranslation;
	bTrans->translation.setValue(0,-height/2+height/14+height/100,0);
	bGroup->addChild(bTrans);		

	bCube = new SoCube;
	bCube->width=width;
	bCube->height=height/7;
	bCube->depth=depth/3;
	bGroup->addChild(bCube);

	slider->setBodyGeometry(bGroup, setupSliderMesh);

	bCube = new SoCube;
	bCube->width=width*1.15f;
	bCube->height=height*1.5f;
	bCube->depth=depth*1.15f;

	slider->setBodySurrounding(bCube);

	bGroup = new SoGroup;	

	SoCoordinate3 *bCoord1 = new SoCoordinate3;
	for (i=0;i<12;i++)
		bCoord1->point.set1Value(i,vertex1Data[i]);
	bGroup->addChild(bCoord1);
	
	SoQuadMesh *bMesh1 = new SoQuadMesh;
	bMesh1->verticesPerRow=6;
	bMesh1->verticesPerColumn=2;
	bGroup->addChild(bMesh1);

	SoCoordinate3 *bCoord2 = new SoCoordinate3;
	for (i=0;i<12;i++)
		bCoord2->point.set1Value(i,vertex2Data[i]);
	bGroup->addChild(bCoord2);
	
	SoQuadMesh *bMesh2 = new SoQuadMesh;
	bMesh2->verticesPerRow=6;
	bMesh2->verticesPerColumn=2;
	bGroup->addChild(bMesh2);

	slider->setSliderGeometry(bGroup);

	bCube = new SoCube;
	bCube->width = sWidth*1.15f;
	bCube->height = height*1.5f;
	bCube->depth = depth*1.15f;
	slider->setSliderSurrounding(bCube);

	slider->startPoint.setValue(-0.4f*width,0,0);
	slider->endPoint.setValue(+0.4f*width,0,0);
	
	
}


//###################################################################
//## FUNCTIONS FOR HANDLING ADDITIONAL GEOMETRY
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setAdditionalGeometry
// Purpose:		This function inserts the given node geometry in the
//				dial-graph for use as new additional geomerty
// Parameters:	geometry:	pointer to the geometry-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DTouchSlider::setAdditionalGeometry(SoNode *geometry)
{		// replace old with new geometry
	SoGroup *aSep = (SoGroup *) additionalGeometry.getValue();
		
	if (aSep->getNumChildren() == 0)
		aSep->addChild(geometry);
	else
		aSep->replaceChild(0,geometry); 

}

/////////////////////////////////////////////////////////////////////
// Name:		getAdditionalGeometry
// Purpose:		This function returns a pointer to the graph of the
//				dials geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the dials geometry
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DTouchSlider::getAdditionalGeometry()
{		// return pointer to geometry
	SoGroup *aSep = (SoGroup *) additionalGeometry.getValue();
		
	if (aSep->getNumChildren() == 0)
		return NULL;
	else
		return aSep->getChild(0); 
}


//###################################################################
//## FUNCTIONS FOR HANDLING THE SLIDER-SURROUNDING
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setBodySurrounding
// Purpose:		This function inserts the given node newBody in the
//				slider-graph for use as body-surrounding
// Parameters:	newBody:	pointer to the geometry-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DTouchSlider::setBodySurrounding(SoNode *newBody)
{		// replace old with new node
	((SoSeparator *) bodySurrounding.getValue())->replaceChild(0,newBody);
}

/////////////////////////////////////////////////////////////////////
// Name:		setSliderSurrounding
// Purpose:		This function inserts the given node newSlider in the
//				slider-graph for use as slider-surrounding
// Parameters:	newSlider:	pointer to the geometry-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DTouchSlider::setSliderSurrounding(SoNode *newSlider)
{		// replace old with new node
	((SoSeparator *) sliderSurrounding.getValue())->replaceChild(0,newSlider);
}

/////////////////////////////////////////////////////////////////////
// Name:		getBodySurrounding
// Purpose:		This function returns a pointer to the graph of the
//				sliders body-surrounding-geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the bodys-surrounding
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DTouchSlider::getBodySurrounding()
{		// find node and return it
	return ((SoSeparator *)bodySurrounding.getValue())->getChild(0);
}

/////////////////////////////////////////////////////////////////////
// Name:		getSliderSurrounding
// Purpose:		This function returns a pointer to the graph of the
//				slider-surrounding-geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the slider-surrounding
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DTouchSlider::getSliderSurrounding()
{		// find node and return it
	return ((SoSeparator *)sliderSurrounding.getValue())->getChild(0);
}


//###################################################################
//## FUNCTIONS FOR HANDLING THE SLIDER-GEOMETRY
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setBodyGeometry
// Purpose:		This function inserts the given node newBody in the
//				slider-graph for use as slider-body
// Parameters:	newBody:	pointer to the geometry-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DTouchSlider::setBodyGeometry(SoNode *newBody, SoSensorCB *callback)
{		// replace old with new node and change sensor-CB
	radiusSensor->detach();
	widthSensor->detach();

	if (callback != NULL)
	{
		radiusSensor->attach(&radius);
		widthSensor->attach(&width);
		radiusSensor->setFunction(callback);
		widthSensor->setFunction(callback);
	}

	((SoSeparator *) bodyGeometry.getValue())->replaceChild(0,newBody);
}

/////////////////////////////////////////////////////////////////////
// Name:		setSliderGeometry
// Purpose:		This function inserts the given node newSlider in the
//				slider-graph for use as slider
// Parameters:	newSlider:	pointer to the geometry-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DTouchSlider::setSliderGeometry(SoNode *newSlider)
{		// replace old with new node
	((SoSeparator *) sliderGeometry.getValue())->replaceChild(0,newSlider);
}

/////////////////////////////////////////////////////////////////////
// Name:		getBodyGeometry
// Purpose:		This function returns a pointer to the graph of the
//				sliders body-geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the body-geometry
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DTouchSlider::getBodyGeometry()
{		// find node and return it
	return ((SoSeparator *)bodyGeometry.getValue())->getChild(0);
}

/////////////////////////////////////////////////////////////////////
// Name:		getSliderGeometry
// Purpose:		This function returns a pointer to the graph of the
//				slider-geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the slider-geometry
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DTouchSlider::getSliderGeometry()
{		// find node and return it
	return ((SoSeparator *)sliderGeometry.getValue())->getChild(0);
}


//###################################################################
//## SETTING AND READING HIGHLIGHTING-STATUS
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		disableHighlighting
// Purpose:		sets the field disableHL with the value of the
//				parameter disHigh. If disalbeHL is TRUE, no
//				highlighting is preformed
// Parameters:	disHigh:	TRUE for disabling highlighting,
//							FALSE for enabling it
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DTouchSlider::disableHighlighting(SbBool disHigh)
{
		// set new value for disableHL
	disableHL = disHigh;
	if (disHigh == TRUE) 
	{		// if highlighting disabled then set normal colors
		sliderHL.setValue(0);
		bodyHL.setValue(0);	
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		isSliderHighlighted
// Purpose:		returns FALSE, if slider is not highlighted (then
//				highlighted == 0), else TRUE
// Parameters:	---
// Returnvalue: FALSE, when highlighted == 0, else TRUE
/////////////////////////////////////////////////////////////////////
SbBool 
SoBase3DTouchSlider::isSliderHighlighted()
{
	if (sliderHL.getValue() == 0) return FALSE;
	else return TRUE;
}

/////////////////////////////////////////////////////////////////////
// Name:		setSliderHighlighted
// Purpose:		This function sets the field highlighted to 1, if 
//				the parameter highlightB is TRUE, else to 0
// Parameters:	highlightB	: if TRUE -> highlighted=1 
//							  else highlighted=0
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DTouchSlider::setSliderHighlighted(SbBool highlightB)
{
    if (highlightB == TRUE)
    {	
        sliderHL.setValue(1);
    }
    else
    {
        sliderHL.setValue(0);		
    }
}

/////////////////////////////////////////////////////////////////////
// Name:		isBodyHighlighted
// Purpose:		returns FALSE, if body of slider is not highlighted 
//				(then highlighted == 0), else TRUE
// Parameters:	---
// Returnvalue: FALSE, when highlighted == 0, else TRUE
/////////////////////////////////////////////////////////////////////
SbBool 
SoBase3DTouchSlider::isBodyHighlighted()
{
	if (bodyHL.getValue() == 0) return FALSE;
	else return TRUE;
}

/////////////////////////////////////////////////////////////////////
// Name:		setBodyHighlighted
// Purpose:		This function sets the field highlighted to 1, if 
//				the parameter highlightB is TRUE, else to 0
// Parameters:	highlightB	: if TRUE -> highlighted=1 
//							  else highlighted=0
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DTouchSlider::setBodyHighlighted(SbBool highlightB)
{		// only change value when necessary

    if (highlightB == TRUE)
    {
        bodyHL.setValue(1);
    }
    else
    {
        bodyHL.setValue(0);		
    }
}


//###################################################################
//## SETTING THE POSITION OF THE SLIDER
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setPosition
// Purpose:		This function sets the movable part of the slider 
//				to a given position
// Parameters:	newValue:	new position of slider
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DTouchSlider::setPosition(float )
{
}


float
SoBase3DTouchSlider::getNormalizedIncrement(void)
{
	return 0;
}

//###################################################################
//## INITIALIZATION FUNCTIONS 
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		initClass
// Purpose:		sets up type- and file-format name-information for
//				the class
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DTouchSlider::initClass()
{
	if (SoBase3DTouchSlider::baseInitClassAttempts==0)
	{
		SoBase3DTouchSlider::baseInitClassAttempts=1;

		// use initialization-macro
		SO_KIT_INIT_CLASS(SoBase3DTouchSlider, SoBaseKit, "BaseKit");
	
		BASE3D_INIT_CLASS(SoBase3DTouchSlider);
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		createSliderGraph
// Purpose:		Builds up the default slider-geometry and the 
//				field-connections
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DTouchSlider::createSliderGraph()
{
		// for all calculations (translation out of 
		// start- and end-point and the value of position, which
		// has to be between 0 and 1, width, height...)
		//we use a calculator-engine

	SoCalculator *sCalc = new SoCalculator;
	sCalc->a.connectFrom(&position);
	sCalc->A.connectFrom(&startPoint);
	sCalc->B.connectFrom(&endPoint);
	sCalc->expression.set1Value(0,"oA[0]=(a-0.5)*(B[0]-A[0])");
	sCalc->expression.set1Value(1,"oA[1]=(a-0.5)*(B[1]-A[1])");
	sCalc->expression.set1Value(2,"oA[2]=(a-0.5)*(B[2]-A[2])");
//	sCalc->expression.set1Value(0,"oA=(0-0.5)*(B-A)");

		// there we need a swich-node for color-highlighting
	SoSwitch *sSwitch = (SoSwitch *) bodySwitch.getValue();

	SoMaterial *sMat = new SoMaterial;
	sMat->diffuseColor.connectFrom(&bodyColor);
	sSwitch->addChild(sMat);

	sMat = new SoMaterial;
	sMat->diffuseColor.connectFrom(&bodyHLColor);
	sSwitch->addChild(sMat);

	sSwitch->whichChild.connectFrom(&bodyHL);

		// then we change the material to fully transparent
	SoDrawStyle *bStyle = 
		(SoDrawStyle *) bodyDrawStyle.getValue();
	bStyle->style.setValue(SoDrawStyle::INVISIBLE);

		// after the body we need a translation for moving the
		// slider along the body
	SoTranslation *sTrans = (SoTranslation *) sliderTranslation.getValue(); 

	sTrans->translation.connectFrom(&sCalc->oA);	

		// then we add the slider-part
		// it has the same construction as the body-part
		// first a switch-node for highlighting
	sSwitch = (SoSwitch *) sliderSwitch.getValue();

	sMat = new SoMaterial;
	sMat->diffuseColor.connectFrom(&sliderColor);
	sSwitch->addChild(sMat);

	sMat = new SoMaterial;
	sMat->diffuseColor.connectFrom(&sliderHLColor);
	sSwitch->addChild(sMat);

	sMat = new SoMaterial;
	sMat->diffuseColor.connectFrom(&sliderDragColor);
	sSwitch->addChild(sMat);

	sSwitch->whichChild.connectFrom(&sliderHL);

		// we change to fully transparent
	SoDrawStyle *sStyle = 
		(SoDrawStyle *) sliderDrawStyle.getValue();
	sStyle->style.setValue(SoDrawStyle::INVISIBLE);


	SoGroup *bGr = new SoGroup;
	((SoSeparator *) bodyGeometry.getValue())->addChild(bGr);
	((SoSeparator *) sliderGeometry.getValue())->addChild(bGr);
	((SoSeparator *) bodySurrounding.getValue())->addChild(bGr);
	((SoSeparator *) sliderSurrounding.getValue())->addChild(bGr);

	radiusSensor= new SoFieldSensor(setupSliderMesh, this);
	radiusSensor->attach(&radius);
	widthSensor = new SoFieldSensor(setupSliderMesh, this);
	widthSensor->attach(&width);

	setupSliderMesh(this,NULL);

}

/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		does all constructor-initialization
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DTouchSlider::initFields(float width, float radius, float pos)
{
	if (pos > 1) pos = 1; 
	else if (pos < 0) pos = 0;
 	
	SO_KIT_CONSTRUCTOR(SoBase3DTouchSlider);

	SO_KIT_DEFINE_ENUM_VALUE(CBPolicy, ON_RELEASE);
	SO_KIT_DEFINE_ENUM_VALUE(CBPolicy, ALWAYS);
	SO_KIT_SET_SF_ENUM_TYPE(callback,CBPolicy);
	SO_KIT_ADD_FIELD(callback,		(ON_RELEASE));
	callback=ON_RELEASE;

		// declare fields and their default-values
	SO_KIT_ADD_FIELD(startPoint,	(-0.4f*width,0,0));
	SO_KIT_ADD_FIELD(endPoint,		(+0.4f*width,0,0));
	SO_KIT_ADD_FIELD(bodyColor,		(0.7,0.7,0.7));
	SO_KIT_ADD_FIELD(bodyHLColor,	(1.0,1.0,0.2));
	SO_KIT_ADD_FIELD(sliderColor,	(0.4,0.4,0.4));
	SO_KIT_ADD_FIELD(sliderHLColor,	(0.8,0.8,0.2));
	SO_KIT_ADD_FIELD(sliderDragColor,(1.0,0.25,0.25));
	SO_KIT_ADD_FIELD(position,		(pos));
	SO_KIT_ADD_FIELD(triggerButton,	(STB_PRIMARY_BUTTON));
	SO_KIT_ADD_FIELD(bodyHL,		(0));
	SO_KIT_ADD_FIELD(sliderHL,		(0));
	SO_KIT_ADD_FIELD(disableHL,		(FALSE));
	SO_KIT_ADD_FIELD(radius,		(radius));
	SO_KIT_ADD_FIELD(width,			(width));
	SO_KIT_ADD_FIELD(nodeId,		(""));

	SO_KIT_ADD_CATALOG_ENTRY(kitSeparator,SoSeparator,FALSE,this,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(additionalGeometry,SoGroup,FALSE,kitSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,SoSeparator,FALSE,kitSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(bodySeparator,SoSeparator,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(bodySwitch,SoSwitch,FALSE,bodySeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(bodyGeometry,SoSeparator,FALSE,bodySeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(bodyDrawStyle,SoDrawStyle,FALSE,bodySeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(bodySurrounding,SoSeparator,FALSE,bodySeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(sliderTranslation,SoTranslation,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(sliderSeparator,SoSeparator,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(sliderSwitch,SoSwitch,FALSE,sliderSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(sliderGeometry,SoSeparator,FALSE,sliderSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(sliderDrawStyle,SoDrawStyle,FALSE,sliderSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(sliderSurrounding,SoSeparator,FALSE,sliderSeparator,\x0,TRUE);


	SO_KIT_INIT_INSTANCE();

	BASE3D_CONSTRUCTOR;
		// initialize non-field member-variables
	funcList=new SbPList;
	dragging = FALSE;
}


/////////////////////////////////////////////////////////////////////
// Name:		SoBase3DTouchSlider
// Purpose:		Default-Constructor of class SoBase3DTouchSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoBase3DTouchSlider::SoBase3DTouchSlider()
{
    firstTime=TRUE;
	initFields(50,3,0);
		// build up the default button-geometry
	createSliderGraph();
}

/////////////////////////////////////////////////////////////////////
// Name:		~SoBase3DTouchSlider
// Purpose:		Destructor of class So3DBSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoBase3DTouchSlider::~SoBase3DTouchSlider()
{
}


//###################################################################
//## 3D-EVENT-HANDLING
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		isInterested
// Purpose:		returns TRUE if the handle3DEvent-method has to be
//				called ( i.e. the slider is interested in the event)
// Parameters:	h3a:	event-handle
// Returnvalue: TRUE if interested, FALSE if not
/////////////////////////////////////////////////////////////////////
SbBool
SoBase3DTouchSlider::isInterested(SoHandle3DEventAction *h3a)
{
	So3DEvent *ev = (So3DEvent*) h3a->getEvent();

	const SoPath *rootPath = h3a->getPathFromRoot();

	SoPath *currPath = rootPath->copy();
	currPath->ref();
	currPath->append(kitSeparator.getValue());
	currPath->append(topSeparator.getValue());

	static SbViewportRegion vp;

		// calculate axis-aligned bounding-box
	static SoGetBoundingBoxAction *bba=new SoGetBoundingBoxAction(vp);
	bba->apply((SoFullPath*)currPath);

//### GH July 22 1999
//orig: 	SbBox3f box=bba->getBoundingBox();
	SbXfBox3f box=bba->getXfBoundingBox();
		// if pen outside then
	if (box.intersect(ev->getTranslation())==FALSE) 
	 		// event is not interesting
		return FALSE;

		// get path to slider-geometry
	currPath->append(sliderSeparator.getValue());
		
		// get boundingBox of slider
	static SoGetBoundingBoxAction *bba2=new SoGetBoundingBoxAction(vp);
	bba2->apply(currPath);

	currPath->unref();

	SbXfBox3f xfbox=bba2->getXfBoundingBox();
		// if pen is insider slider-geometry then
	if(xfbox.intersect(ev->getTranslation())==TRUE)
	{		// handle3DEvent has to be called for slider
		h3a->setGrabber(this);
		h3a->setHandled();
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////
// Name:		handle3DEvent
// Purpose:		this method is called whenever the event is
//				interesting for the slider
// Parameters:	h3a:	event-handle
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DTouchSlider::handle3DEvent(SoHandle3DEventAction *h3a)
{
    So3DEvent *ev = (So3DEvent*) h3a->getEvent();
    
    const SoPath *rootPath = h3a->getPathFromRoot();
    
    static SbViewportRegion vp;
    
    SoPath *sliderPath = rootPath->copy();
    sliderPath->ref();
    sliderPath->append(kitSeparator.getValue());
    sliderPath->append(topSeparator.getValue());
    sliderPath->append(sliderSeparator.getValue());
    
    // get boundingBox of slider
    static SoGetBoundingBoxAction *bba2=new SoGetBoundingBoxAction(vp);
    bba2->apply(sliderPath);
    
    sliderPath->unref();
    
    SbXfBox3f xfbox=bba2->getXfBoundingBox();
    // if pen is inside slider-geometry then
    if (xfbox.intersect(ev->getTranslation()) == TRUE) 
    {
        if (!dragging)
        {
            setSliderHighlighted(TRUE);
            dragging = TRUE;
        }
        
        SoPath *currPath = rootPath->copy();
        currPath->ref();
        currPath->append(kitSeparator.getValue());
        currPath->append(topSeparator.getValue());
        
        SbVec3f sliderPoint, transformedPoint, vec1, vec2;
        float  value;
        
        //calculate slider-position out of current pen-position
        static SoGetMatrixAction *gma2=new SoGetMatrixAction(vp);
        gma2->apply((SoFullPath *)currPath);
        currPath->unref();
        
        SbMatrix inv_m2=gma2->getInverse();
        
        sliderPoint = ev->getTranslation();
        inv_m2.multVecMatrix(sliderPoint, transformedPoint); 
        
        // calculate the new position of the slider
        // using the dot-product
        vec1 = endPoint.getValue() - startPoint.getValue();
        vec2 = transformedPoint - startPoint.getValue();
        
        value = vec1.dot(vec2)/(vec1.length()*vec1.length());         
        setPosition(value);
    }
    else
    {
        // completely outside, so release grabber again
        setSliderHighlighted(FALSE);
        dragging = FALSE;
        h3a->releaseGrabber();
        h3a->setHandled();
    }
}
