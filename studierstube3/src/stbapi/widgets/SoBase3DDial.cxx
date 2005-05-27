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
/** SoBase3DDial - implementation
  *
  * @author  Hermann Wurnig, Gerd Hesina
  *
  * $Id: SoBase3DDial.cxx 3781 2004-09-28 11:19:51Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>

#include <Inventor/misc/SoChildList.h>

#include <stbapi/widgets/SoBase3DDial.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/workspace/StbWorkspace.h>

int  SoBase3DDial::baseInitClassAt=0;

SO_KIT_SOURCE(SoBase3DDial);

BASE3D_SOURCE(SoBase3DDial);

void
SoBase3DDial::getBoundingBox(SoGetBoundingBoxAction *action)
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

	  //condom Hack: if its a virgin make sure that it has the right geometry 
      //by setting the correct geometry before
      if (firstTime) 
      {
        this->enableNotify(FALSE);	//to make the Windows Inventor happy
        setupDialMesh(this,NULL);    
        firstTime=FALSE;		//
        this->enableNotify(TRUE);  //to make the Windows Inventor happy
      }
      //extend boundingBox with existing Geometry
      if (geoSeparator.getValue()!=NULL)
        bba.apply(geoSeparator.getValue());
	  switch (action->getWhatReset())
	  {
        case SoGetBoundingBoxAction::ALL:
		{   
          action->extendBy(bba.getXfBoundingBox()); 
		}
        break;
        case SoGetBoundingBoxAction::BBOX:
		{
          action->extendBy(bba.getBoundingBox()); 
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
SoBase3DDial::setupDialMesh(void *data, SoSensor *)
{
	SoBase3DDial *dial = (SoBase3DDial *) data;
	
	SoSeparator *bSep = new SoSeparator;

	SoCylinder *dCyl = new SoCylinder;
	dCyl->height=dial->height.getValue();
	dCyl->radius=dial->radius.getValue();
	bSep->addChild(dCyl);

	SoMaterial *bMaterial = new SoMaterial;
	bMaterial->diffuseColor.connectFrom(&dial->positionColor);
	bSep->addChild(bMaterial);

	SoTranslation *bTrans = new SoTranslation;
	bTrans->translation.setValue(0,dial->height.getValue()/2, 
								 -dial->radius.getValue()/2);
	bSep->addChild(bTrans);

	SoCube *bCube = new SoCube;
	bCube->width = dial->radius.getValue()/10;
	bCube->height= dial->height.getValue()/10;
	bCube->depth = dial->radius.getValue()*0.8;
	bSep->addChild(bCube);

	dial->setGeometry(bSep,setupDialMesh);

	dCyl = new SoCylinder;
	dCyl->height = dial->height.getValue()*1.5;
	dCyl->radius = dial->radius.getValue()*1.15;

	dial->setSurrounding(dCyl);

}

//###################################################################
//## SETTING AND READING THE POSITION OF THE DIAL
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setPosition
// Purpose:		This function sets the slider to a new position
// Parameters:	newValue:	new position of slider
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DDial::setPosition(float )
{		// newValue has to be between 0 and 1
}

void
SoBase3DDial::setPositionUpdate(float )
{
	fprintf(stderr,"ATTENTION: abstract SoBase3DDial::setPositionUpdate called!!! Please fix\n");
	assert(0);
}

/////////////////////////////////////////////////////////////////////
// Name:		getPosition
// Purpose:		This function returns the dials current position
// Parameters:	---
// Returnvalue: position of dial
/////////////////////////////////////////////////////////////////////
float
SoBase3DDial::getPosition()
{		
	return position.getValue();
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
SoBase3DDial::disableHighlighting(SbBool disHigh)
{
		// set new value for disableHL
	disableHL = disHigh;
	if (disHigh == TRUE) 
	{		// if highlighting disabled then set normal colors
		highlighted.setValue(0);
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
SoBase3DDial::isHighlighted()
{
	if (highlightCount > 0) return TRUE;
	else return FALSE;
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
SoBase3DDial::setHighlighted(SbBool highlightB)
{
	if (highlightB == FALSE) 
	{
		highlightCount--;
		if (highlightCount == 0) highlighted = 0;
	}
	else	// else turn it on when no one is dragging 
	{
		if (highlighted.getValue() != 2) highlighted = 1;
		highlightCount++;
	}
}


//###################################################################
//## FUNCTIONS FOR HANDLING THE DIAL-GEOMETRY
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setGeometry
// Purpose:		This function inserts the given node geometry in the
//				dial-graph for use as new dial geomerty
// Parameters:	geometry:	pointer to the geometry-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DDial::setGeometry(SoNode *geometry, SoSensorCB *callback)
{		// replace old with new geometry and change sensors
	radiusSensor->detach();
	heightSensor->detach();

	if (callback != NULL)
	{
		radiusSensor->attach(&radius);
		heightSensor->attach(&height);
		radiusSensor->setFunction(callback);
		heightSensor->setFunction(callback);
	}
				
	((SoSeparator *)geoSeparator.getValue())->
		replaceChild(0,geometry);
}


/////////////////////////////////////////////////////////////////////
// Name:		getGeometry
// Purpose:		This function returns a pointer to the graph of the
//				dials geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the dials geometry
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DDial::getGeometry()
{		// return pointer to geometry
	return ((SoSeparator *) geoSeparator.getValue())->
		getChild(0);
}


//###################################################################
//## FUNCTIONS FOR HANDLING THE DIAL-SURROUNDING
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setSurrounding
// Purpose:		This function inserts the given node in the
//				dial-graph for use as surrounding geometry
// Parameters:	geometry:	pointer to the surrounding-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DDial::setSurrounding(SoNode *geometry)
{		// replace old with new geometry
	((SoSeparator *)surrSeparator.getValue())->
		replaceChild(0,geometry);
}

/////////////////////////////////////////////////////////////////////
// Name:		getSurrounding
// Purpose:		This function returns a pointer to the dials
//				surrounding-geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the surrounding-geomerty
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DDial::getSurrounding()
{		// return pointer to surrounding
	return ((SoSeparator *) surrSeparator.getValue())->
		getChild(0);
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
SoBase3DDial::setAdditionalGeometry(SoNode *geometry)
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
SoBase3DDial::getAdditionalGeometry()
{		// return pointer to geometry
	SoGroup *aSep = (SoGroup *) additionalGeometry.getValue();
		
	if (aSep->getNumChildren() == 0)
		return NULL;
	else
		return aSep->getChild(0); 
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
SoBase3DDial::initClass()
{

	if (SoBase3DDial::baseInitClassAt==0)
	{
		SoBase3DDial::baseInitClassAt=1;

		// use initialization-macro
		SO_KIT_INIT_CLASS(SoBase3DDial, SoBaseKit, "BaseKit");

		BASE3D_INIT_CLASS(SoBase3DDial);
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		defines the fields used by SoBase3DDial
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DDial::initFields()
{
	SO_KIT_CONSTRUCTOR(SoBase3DDial);

	SO_KIT_DEFINE_ENUM_VALUE(CBPolicy, ON_RELEASE);
	SO_KIT_DEFINE_ENUM_VALUE(CBPolicy, ALWAYS);
	SO_KIT_SET_SF_ENUM_TYPE(callback,CBPolicy);
	SO_KIT_ADD_FIELD(callback,	(ON_RELEASE));

	callback=ON_RELEASE;

	SO_KIT_ADD_CATALOG_ENTRY(kitSeparator,SoSeparator,FALSE,this,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(additionalGeometry,SoGroup,FALSE,kitSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,SoSeparator,FALSE,kitSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(rotation,SoRotationXYZ,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(colorSwitch,SoSwitch,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(geoSeparator,SoSeparator,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(surroundingDrawStyle,SoDrawStyle,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(surrSeparator,SoSeparator,FALSE,topSeparator,\x0,TRUE);
	
		// declare fields and their default-values
	SO_KIT_ADD_FIELD(dialColor,			(0.5,0.5,0.5));
	SO_KIT_ADD_FIELD(highlightColor,	(1.0,1.0,0.4));
	SO_KIT_ADD_FIELD(dragColor,			(1.0,0.35,0.35));
	SO_KIT_ADD_FIELD(positionColor,		(0.2,0.2,0.2));
	SO_KIT_ADD_FIELD(highlighted,		(0));
	SO_KIT_ADD_FIELD(triggerButton,		(STB_PRIMARY_BUTTON));
	SO_KIT_ADD_FIELD(disableHL,			(FALSE));
	SO_KIT_ADD_FIELD(startAngle,		(-2*M_PI/3));
	SO_KIT_ADD_FIELD(endAngle,			(+2*M_PI/3));
	SO_KIT_ADD_FIELD(position,			(0));
	SO_KIT_ADD_FIELD(radius,			(0));
	SO_KIT_ADD_FIELD(height,			(0));
	SO_KIT_ADD_FIELD(nodeId,			(""));

	SO_KIT_INIT_INSTANCE();

	BASE3D_CONSTRUCTOR;

		// initialize non-field member-variables
	funcList=new SbPList;
	dragging=FALSE;
	highlightCount = 0;
    snapBackSensor=new SoAlarmSensor(NULL,NULL);
}

/////////////////////////////////////////////////////////////////////
// Name:		setupDialGeometry
// Purpose:		Builds up the default dial-geometry and the 
//				field-connections
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DDial::setupDialGeometry(float height, float radius)
{
	this->height=height;
	this->radius=radius;

	SoRotationXYZ *bRot = (SoRotationXYZ *) rotation.getValue();
	bRot->axis = SoRotationXYZ::Y;
	
	SoCalculator *sCalc = new SoCalculator;
	sCalc->a.connectFrom(&position);
	sCalc->b.connectFrom(&startAngle);
	sCalc->c.connectFrom(&endAngle);
	sCalc->expression.set1Value(0,"oa=-b-a*(c-b)");
	bRot->angle.connectFrom(&sCalc->oa);	

		// switch-child 0 is the normal dial-color
	SoMaterial *bMaterial = new SoMaterial;
	bMaterial->diffuseColor.connectFrom(&dialColor);
	((SoSwitch *) colorSwitch.getValue())->addChild(bMaterial);

		// switch-child 1 is the highlight-color
	bMaterial = new SoMaterial;
	bMaterial->diffuseColor.connectFrom(&highlightColor);
	((SoSwitch *) colorSwitch.getValue())->addChild(bMaterial);

		// switch-child 2 is the dragging-color
	bMaterial = new SoMaterial;
	bMaterial->diffuseColor.connectFrom(&dragColor);
	((SoSwitch *) colorSwitch.getValue())->addChild(bMaterial);

	((SoSwitch *) colorSwitch.getValue())->whichChild.connectFrom(&highlighted);
	
	SoDrawStyle *bStyle = 
		(SoDrawStyle *) surroundingDrawStyle.getValue();
	bStyle->style.setValue(SoDrawStyle::INVISIBLE);

	SoGroup *bGr = new SoGroup;

	((SoSeparator *) geoSeparator.getValue())->addChild(bGr);
	((SoSeparator *) surrSeparator.getValue())->addChild(bGr);

	heightSensor= new SoFieldSensor(setupDialMesh, this);
	heightSensor->attach(&this->height);
	radiusSensor = new SoFieldSensor(setupDialMesh, this);
	radiusSensor->attach(&this->radius);

	setupDialMesh(this, NULL);
}


//###################################################################
//## CONSTRUCTORS/DESTRUCTOR
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		SoBase3DDial
// Purpose:		Default-Constructor of class SoBase3DDial
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoBase3DDial::SoBase3DDial()
{		// init fields
	firstTime=TRUE;
	initFields();
		// and create slider
	setupDialGeometry(3,5);
}

/////////////////////////////////////////////////////////////////////
// Name:		~SoBase3DDial
// Purpose:		Destructor of class SoBase3DDial
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoBase3DDial::~SoBase3DDial()
{
    if(snapBackSensor)
		delete snapBackSensor;
}


//###################################################################
//## 3D-EVENT-HANDLING
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		isInterested
// Purpose:		returns TRUE if the SoHandle3DEvent-method has to be
//				called ( i.e. the dial is interested in the event)
// Parameters:	h3a:	event-handle
// Returnvalue: TRUE if interested, FALSE if not
/////////////////////////////////////////////////////////////////////
SbBool 
SoBase3DDial::isInterested(SoHandle3DEventAction *h3a)
{
	DISABLEDIVNODE(this);
	// get underlying event
	So3DEvent *ev = (So3DEvent*) h3a->getEvent();
		// get path to dial

	const SoPath *rootPath = h3a->getPathFromRoot();

	SoPath *currPath = rootPath->copy();
	currPath->ref();
	currPath->append(kitSeparator.getValue());
	currPath->append(topSeparator.getValue());
	currPath->append(geoSeparator.getValue());

	SbViewportRegion vp;
	
	SoGetBoundingBoxAction bba(vp);
	bba.apply((SoFullPath*)currPath);
	currPath->unref();
	
	SbXfBox3f xfbox=bba.getXfBoundingBox();

	// if position of pen is inside, highlight and set grabber
	if(xfbox.intersect(ev->getTranslation())==TRUE)
	{		// then not interested
		setHighlighted(TRUE);
		h3a->setGrabber(this);
		h3a->setHandled();
		ENABLEDIVNODE(this);
		return TRUE;
	}
		// else the pen is outside the dials bounding box 
	ENABLEDIVNODE(this);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////
// Name:	getSign
// Purpose:	returns 1 if number >= 0, else -1
/////////////////////////////////////////////////////////////////////
int getSign(float number)
{
	if (number < 0) return -1; else return 1;
}
	
/////////////////////////////////////////////////////////////////////
// Name:		SoHandle3DEvent
// Purpose:		this method is called whenever the event is
//				interesting for the dial
// Parameters:	h3a:	event-handle
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DDial::handle3DEvent(SoHandle3DEventAction *h3a)
{
	DISABLEDIVNODE(this);
	// get underlying event
	So3DEvent *ev = (So3DEvent*) h3a->getEvent();
		// get path to dial
	const SoPath *rootPath = h3a->getPathFromRoot();

	SoPath *currPath = rootPath->copy();
	currPath->ref();
	currPath->append(kitSeparator.getValue());
	currPath->append(topSeparator.getValue());

	static SbViewportRegion vp;

	SbVec3f dialPoint, transformedPoint;
	float  x,y,z,value;

		//calculate slider-position out of current pen-position
	static SoGetMatrixAction *gma2=new SoGetMatrixAction(vp);
	gma2->apply((SoFullPath *)currPath);
	SbMatrix inv_m2=gma2->getInverse();

	dialPoint = ev->getTranslation();
	inv_m2.multVecMatrix(dialPoint, transformedPoint); 
		// calculate the new position of the slider
		// using the dot-product
	transformedPoint.getValue(x,y,z);
	transformedPoint.setValue(x,0,z);

	if (transformedPoint.length() == 0)
		value = 10;
	else 
		value = asin(z/transformedPoint.length()); 


	if ((dragging == TRUE) && (dragStation == ev->getStation()))
	{
		if (value == 10)
		{
            currPath->unref();
			ENABLEDIVNODE(this);
			return;
		}
			// if the angle has changed
		if (oldAngle != value) 
		{		// then set new position 
			if (getSign(oldDistance) == getSign(x))
			{		// 1. and 3. quadrant
				setPosition(getPosition()-getSign(x)*(value-oldAngle)/
					(-endAngle.getValue()+startAngle.getValue()));
			}
			else
			{		// 2. and 4. quadrant
				if (value < 0)
					setPosition(getPosition()-getSign(x)*(M_PI+value+oldAngle)/
						(-endAngle.getValue()+startAngle.getValue()));
				else
					setPosition(getPosition()+getSign(x)*(M_PI-value-oldAngle)/
						(-endAngle.getValue()+startAngle.getValue()));
			}
			oldAngle=value;oldDistance=x;
		}

		if ((ev->getType() == triggerButton.getValue()+1) &&
			(ev->getButton(triggerButton.getValue()) == FALSE))
		{
			dragging = FALSE;
			highlighted = 1;
			ENABLEDIVNODE(this);
//### GH July 23 1999
			setPositionUpdate(getPosition());
		}
	}

	if ((dragging == FALSE) || ((dragging == TRUE) && (dragStation != ev->getStation())))
	{
		SoPath *dialPath = currPath->copy();
		dialPath->ref();
		dialPath->append(geoSeparator.getValue());
	
		static SoGetBoundingBoxAction *bba=new SoGetBoundingBoxAction(vp);
		bba->apply(dialPath);
	
		dialPath->unref();

		SbXfBox3f xfbox=bba->getXfBoundingBox();

			// if position of pen is outside
		if(xfbox.intersect(ev->getTranslation()) == FALSE)
		{		// then turn off highlighting
			setHighlighted(FALSE);
			h3a->releaseGrabber();
			h3a->setHandled();
	
		}
		else
		{
			if ((ev->getType() == triggerButton.getValue()+1) &&
				(ev->getButton(triggerButton.getValue()) == TRUE) &&
				(dragging == FALSE))
			{
                if( snapBackSensor->isScheduled())
                    snapBackSensor->unschedule();
				dragging = TRUE;
				dragStation = ev->getStation();
				highlighted = 2;
				oldAngle = value;
				oldDistance = x;
			}

		}

	}
	currPath->unref();
	ENABLEDIVNODE(this);
}

void
SoBase3DDial::updateFromNetwork(float newValue)
{
	fprintf(stderr,"ATTENTION: abstract SoBase3DDial::updateFromNetwork called!!! Please fix\n");
	assert(0);
}

