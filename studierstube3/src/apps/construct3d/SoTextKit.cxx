// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Cnd.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//  AUTHOR:    hk      Hannes Kaufmann
// ===========================================================================
//  HISTORY:   Time of development: Dezember 1999 - 
//
// ===========================================================================

#include "SoTextKit.h"
#include "SoPointKit.h"
#include "SoLineKit.h"
#include <Inventor/nodes/SoFontStyle.h>
#include <stbapi/misc/SoBillboardKit.h>
#include <Inventor/nodes/SoFont.h>

SO_KIT_SOURCE(SoTextKit);

void SoTextKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoTextKit, Object3dKit, "Object3dKit");
} 

SoTextKit::SoTextKit() 
{ 
  initSoTextKit();
  createSensors();
  setUpConnections(TRUE, TRUE);
}

/*void
SoTextKit::setNameSensors()
{
  objectOneSensor = new SoFieldSensor(nameUpdate, this);
  objectOneSensor->setPriority(0);
  objectOneSensor->attach(&p);
  
  objectTwoSensor = new SoFieldSensor(nameUpdate, this);
  objectTwoSensor->setPriority(0);
  objectTwoSensor->attach(&q);
}
*/

void SoTextKit::createSensors()
{
	objectOneSensor = new SoFieldSensor(nameUpdate, this);
	objectOneSensor->setPriority(0);
//	objectOneSensor->attach(&p);

	objectTwoSensor = new SoFieldSensor(nameUpdate, this);
	objectTwoSensor->setPriority(0);
//	objectTwoSensor->attach(&q);

	if (s_return.ok())
	{
		objectOneSensor = new SoFieldSensor(nameUpdate, this);
		objectOneSensor->setPriority(0);

		objectTwoSensor = new SoFieldSensor(nameUpdate, this);
		objectTwoSensor->setPriority(0);

		if(LineOneName.getValue() != "")
		{ 
			SbName Obj1Name = LineOneName.getValue();
			Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(Obj1Name);
			if(obj1)
				obj1->connectChild(this);
		}
		if(LineTwoName.getValue() != "")
		{ 
			SbName Obj2Name = LineTwoName.getValue();
			Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
			if(obj2)
				obj2->connectChild(this);
		}
	}

	for (int i=0; i < NumofPoints.getValue(); i++)
	{
		SbString ObjName;
		pointsName.get1(i,ObjName);
		Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
		if(obj)
		{
			printf("Creating sensor %d \n",i);
			ObjectSensor[i] = new SoFieldSensor(nameUpdate, this);
			ObjectSensor[i]->setPriority(0);
			obj->connectChild(this);
		}
	}
}

SoTextKit::SoTextKit(int number, SbVec3f t)
{
  initSoTextKit();
  createSensors();
  setUpConnections(TRUE, TRUE);
//  setNameSensors();
  
  childNumber.setValue(number);
  textSize.setValue(defaultTextSize);
  setNodeName("T", number);
  
  textCoord.setValue(t);
}
/*
void 
SoTextKit::calcText(SoNode* objectOne, SoNode* objectTwo)
{
  calc = new SoCalculator();
  // distance between Points A and B
  calc->A.connectFrom(&(((SoPointKit*)objectOne)->pCoord));
  calc->B.connectFrom(&(((SoPointKit*)objectTwo)->pCoord));
  // outputs rounded distance in cm
  calc->expression.set1Value(0, "oa = ceil(length(B-A)*1000)/10");
  // text displayed at mid point, translation - vec3f(0.01,0,0)
  calc->expression.set1Value(1, "oA = (A+B)/2- vec3f(0.01,0,0)");
  
  textCoord.connectFrom(&(calc->oA));
  distance.connectFrom(&(calc->oa));
  //    char coordString[12];    
  //    sprintf(coordString, "d = %.2f", distance.getValue());
  
  //    SoConcatenate* conc = new SoConcatenate(SoMFString::getClassTypeId());
  //    conc->input[0]->set("d = ");
  //    conc->input[1]->connectFrom(&(calc->oa));
  //    conc->input[2]->set("cm");
}
*/

//  SoTextKit(
//    int number,
//    SoNode* objectOne, 
//    SoNode* objectTwo
//    );

SoTextKit::SoTextKit(int number, std::vector<int> objectList, int userID)
{
  initSoTextKit();
  
  childNumber.setValue(number);
  textSize.setValue(defaultTextSize);
  setNodeName("T", number);
  s_return = outcome(-1,NULL); 
  
//  std::cout << "object: " << getObject(objectList[0])->getName().getString() << std::endl;
//  std::cout << "object: " << getObject(objectList[1])->getName().getString() << std::endl;

  unsigned int pointcount = 0;
  unsigned int linecount = 0;
  unsigned int objectcount = 0;
  while  ( (objectcount < objectList.size()) && 
			((getObject(objectList[objectcount]))->isOfType(SoPointKit::getClassTypeId()) ||
			(getObject(objectList[objectcount]))->isOfType(SoLineKit::getClassTypeId()))) 
  {
    if( (getObject(objectList[objectcount]))->isOfType(SoPointKit::getClassTypeId()) )
      pointcount++;
	if( (getObject(objectList[objectcount]))->isOfType(SoLineKit::getClassTypeId()) )
	  linecount++;
    objectcount++;
  }

  // FIXME: we assume that only lines or only points are passed, 
  // but not lines AND points
  if (linecount == 2)
  {
	  s_return = outcome();
	  std::cout << "line1: " << getObject(objectList[0])->getName().getString() << std::endl;
	  std::cout << "line2: " << getObject(objectList[1])->getName().getString() << std::endl;
	  LineOneName = getObject(objectList[0])->getName();
	  LineTwoName = getObject(objectList[1])->getName();
	  Build();
	  deselectObjects(objectList,linecount);
	  createSensors();
	  setUpConnections(TRUE, TRUE);
  } else
	  if (pointcount > 1)
	  {
		  NumofPoints.setValue(pointcount);
		  s_return = outcome();
		  for(int i=0; i < NumofPoints.getValue(); i++)
		  {
			  pointsName.set1(i,getObject(objectList[i])->getName().getString());
			  std::cout << "point: " << getObject(objectList[i])->getName().getString() << std::endl;
		  }
		  Build();
		  deselectObjects(objectList,pointcount);
		  createSensors();
		  setUpConnections(TRUE, TRUE);
	  } 
}

// init all parts and fields
void
SoTextKit::initSoTextKit()
{
	SO_KIT_CONSTRUCTOR(SoTextKit);
	//-----------------------name--|-type--|nuldef| par    |rite|public
	SO_KIT_ADD_CATALOG_ENTRY(font, SoFontStyle, FALSE, switchNode, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(billboard, SoBillboardKit, FALSE, switchNode, \x0, TRUE);
	//SO_KIT_ADD_CATALOG_ENTRY(object, SoText3, FALSE, switchNode, \x0, TRUE);
	
	//fields
	SO_NODE_ADD_FIELD(textSize, (defaultTextSize));
	SO_NODE_ADD_FIELD(distance, (0.0f));
	SO_NODE_ADD_FIELD(textCoord, (0.0f, 0.0f, 0.0f));
//	SO_NODE_ADD_FIELD(p, (""));
//	SO_NODE_ADD_FIELD(q, (""));
	SO_NODE_ADD_FIELD(pointsName, (""));
	SO_NODE_ADD_FIELD(NumofPoints, (0));
	
	// Initialize this SoTextKit
	SO_KIT_INIT_INSTANCE();
	
	name.setValue("Text");
	
	ObjectSensor.reserve(maxNumofPoints);
	for (int i=0; i<maxNumofPoints; i++)
		ObjectSensor[i] = NULL;
	
	// TextKit is not dynamic - 
	// string values do not get updated automatically
	//turn dragging temporarily off
	translationOn.setValue(FALSE);
	rotationOn.setValue(FALSE);
/*	
	// connect fields
	translation.connectFrom(&(textCoord));
	//	textCoord.connectFrom(&(translation));  // causes crash if turned on here
*/
	SoMaterial* mat = (SoMaterial*) getPart("material", false);
	mat->diffuseColor.setValue(1,1,1);

	text = new SoText3;
	text->justification.setValue(SoText3::CENTER);
    
    // using easy to read font of new PIP "Arial Rounded MT Bold"
    SoFont* textFont = new SoFont;
    textFont->name.setValue("Arial Rounded MT Bold");
    textFont->size.setValue(defaultTextSize);
	
    //text = (SoText3*) getPart("object", false);
	//        SoAsciiText* text1 = (SoAsciiText*) getPart("object", false);
	//        SoAnnoText3 text1 = (SoAnnoText3*) getPart("object", false);

    SoBillboardKit* billb = (SoBillboardKit*) getPart("billboard", false);
	SoSeparator* ct = (SoSeparator*) billb->getPart("content", false);
    ct->addChild(textFont);
	ct->addChild(text);
}

float
SoTextKit::getDistance(SbVec3f penPosition)
{
  //float distance = (penPosition - textCoord.getValue()).length();
  //printf("Textdist:%.3f\n",distance);
  return (penPosition - textCoord.getValue()).length();
}

// Destructor
SoTextKit::~SoTextKit()
{
  if(objectOneSensor)
    delete objectOneSensor;
  if(objectTwoSensor)
    delete objectTwoSensor; 

  for(int i=0; i<NumofPoints.getValue(); i++)
  {
    if (pointsName[i] != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
      if(obj)
      {
        obj->disconnectChild(this);
      }
      if (ObjectSensor[i])
      {
        ObjectSensor[i]->detach();
        printf("removing sensor %d \n",i);
      }
    }
  }
  RemoveAllCallbacks();
}


// input types:
// measure point distance:
//  2 points
void
SoTextKit::nameUpdate(void *data, SoSensor *sensor)
{
  SoTextKit *self=(SoTextKit*)data;
/*  SbName pName = self->p.getValue();
  SbName qName = self->q.getValue();
  SoNode *objectOne = NULL;
  SoNode *objectTwo = NULL;
  
  if(pName != "")
    objectOne = SoNode::getByName(pName);
  if(qName != "")
    objectTwo = SoNode::getByName(qName);
  
  if(objectOne && objectTwo)
  {
    self->calcText(objectOne, objectTwo);
    self->text->string.connectFrom(&(self->distance));
  }
 */ 
  self->triggerChange();
}

void
SoTextKit::Build()
{
  SoFontStyle* textFont = (SoFontStyle*) getPart("font", false);
  textFont->size.connectFrom(&(textSize));

  if(LineOneName.getValue() != "" && LineTwoName.getValue() != "")
  {
//	  printf("LineOne %s \n", LineOneName.getValue().getString());
//	  printf("LineTwo %s \n", LineTwoName.getValue().getString());
	  
	  EDGE *Line1 = ((SoLineKit*)getObject(LineOneName.getValue()))->a_Line;
	  EDGE *Line2 = ((SoLineKit*)getObject(LineTwoName.getValue()))->a_Line;

	  // intersect both lines to find out which angle to measure
	  curve_curve_int* intersects=NULL;
	  s_return = api_inter_ed_ed(Line1, Line2, intersects, NULL);
	  SPAposition int_point (1000,1000,1000); // fixed unlikely point
	  // assuming there is only one intersection point
	  // if one exists, then get it
	  if (intersects != NULL)
		int_point = intersects->int_point;
	  
	  SPAvector v1, v2;
	  if (int_point == SPAposition (1000,1000,1000))
	  {
		  printf("SoTextKit::Build No intersection between lines\n");
		  v1 = Line1->end_pos() - Line1->start_pos();
		  v2 = Line2->end_pos() - Line2->start_pos();
	  } else
	  {
		  //printf("intersection point %f %f %f \n",int_point.x(), int_point.y(), int_point.z());	
		  if (int_point != Line1->start_pos())
			v1 = int_point - Line1->start_pos();
		  else
			v1 = int_point - Line1->end_pos();
		  if (int_point != Line2->start_pos())
			v2 = int_point - Line2->start_pos();
		  else
			v2 = int_point - Line2->end_pos();
	  }

	  // ACIS kernel function: 
	  // Pass both vectors and a 3rd vector giving the plane in which the
	  // angle is measured. 3rd vector can be the normalized cross product 
	  // of the two input vectors 
	  SPAvector cross = v1*v2;
	  SPAunit_vector crossunit (0,0,0);
	  if (!cross.is_zero(eps)) //check if length is 0 !
		  crossunit = normalise(cross);
	  double angle = 0;
	  if (!crossunit.is_zero())
		angle = angle_between(v1, v2, crossunit);
	  else
		  angle = 0;
	  char strdis[25];
	  sprintf(strdis, "%.1f°", angle*180/M_PI);
	  text->string.setValue(strdis);
	  SPAposition pos;
	  pos = Line1->mid_pos() + (Line2->mid_pos() - Line1->mid_pos()) / 2;
	  textCoord.setValue(convertPosition(pos).getValue());
	  text->justification.setValue(SoText3::CENTER);
  }
  else
  if (NumofPoints.getValue() == 2)
  {
    double dis = distance_to_point(((SoPointKit*)getObject(pointsName[0]))->a_Point, ((SoPointKit*)getObject(pointsName[1]))->a_Point);
    char strdis[25];
	//show distance in meters with 3 decimals
    sprintf(strdis, "%.3f", dis);
    text->string.setValue(strdis);
    SPAposition pos;
    pos = ((SoPointKit*)getObject(pointsName[0]))->a_Point + (((SoPointKit*)getObject(pointsName[1]))->a_Point - ((SoPointKit*)getObject(pointsName[0]))->a_Point) / 2;
    textCoord.setValue(convertPosition(pos).getValue());
    text->justification.setValue(SoText3::CENTER);
  }
  else if (NumofPoints.getValue() > 2)
  {
    std::string str = "";
    std::string strline = "_";
    double sum = 0.0;
    for (int index=0; index < NumofPoints.getValue()-1; index++)
    {
      sum = sum + distance_to_point(((SoPointKit*)getObject(pointsName[index]))->a_Point, ((SoPointKit*)getObject(pointsName[index + 1]))->a_Point);
      str = str + (((SoPointKit*)getObject(pointsName[index]))->getName().getString());
      strline = strline + "___";
    }
    str = str + (((SoPointKit*)getObject(pointsName[NumofPoints.getValue()-1]))->getName().getString());
    strline = strline + "___";

    char strsum[25];
	//show distance in meters with 3 decimals
    sprintf(strsum, "%.3f", sum);
    str = str + "=" + strsum;

    text->string.set1Value(0,strline.c_str());
    text->string.set1Value(1,str.c_str());
    textCoord.setValue(convertPosition(((SoPointKit*)getObject(pointsName[NumofPoints.getValue()-1]))->a_Point).getValue());
    text->justification.setValue(SoText3::LEFT);
  }
}

SbBool SoTextKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
  //	std::cout << "setupconnections Curve" << std::endl;
  if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    //SoBaseKit::setUpConnections(onOff, doItAlways);
    Object3dKit::setUpConnections(onOff, doItAlways);
    
    // do your own special setup here !
	SbName OneName = LineOneName.getValue();
	SbName TwoName = LineTwoName.getValue();
	SoNode *objectOne = NULL;
	SoNode *objectTwo = NULL;

    SoNode *object = NULL;
	SbBool ok = FALSE;
    
	// connect fields
	translation.connectFrom(&(textCoord));

	if(OneName != "")
	{
		objectOne = SoNode::getByName(OneName);
		if (objectOneSensor)
		{
			objectOneSensor->attach(&((Object3dKit*)objectOne)->ChangeObject);
			ok = TRUE;
		}
	}
	if(TwoName != "")
	{
		objectTwo = SoNode::getByName(TwoName);
		if (objectTwoSensor)
		{
			objectTwoSensor->attach(&((Object3dKit*)objectTwo)->ChangeObject);
			ok = TRUE;
		}
	}
    if (NumofPoints.getValue() != 0)
    { 
      for(int i=0; i<NumofPoints.getValue(); i++)
      {
        if (pointsName[i] != "")
        {
          object = SoNode::getByName(pointsName[i]);
          if (ObjectSensor[i])
          {
			if (ObjectSensor[i]->getAttachedField() == NULL)
			{
				ObjectSensor[i]->attach(&((SoPointKit*)object)->pCoord);
				ok = TRUE;
				std::cout << "attach: " << ((SoPointKit*)object)->getName().getString() << std::endl;
			}
          }
        }
      }
    }
    
	if (ok)
		Build();
  }
  else 
  {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
    //translation.disconnect();
    Object3dKit::setUpConnections(onOff, doItAlways);
    
    SoDrawStyle* drawstyle = (SoDrawStyle*) getPart("style", false);
    drawstyle->lineWidth.disconnect();
    
	if (objectOneSensor)
		objectOneSensor->detach();
	if (objectTwoSensor)
		objectTwoSensor->detach();

    int i;
    for(i=0; i<NumofPoints.getValue(); i++)
    {
      if (ObjectSensor[i])
        ObjectSensor[i]->detach();
    }    
  }
  return !(connectionsSetUp = onOff);
}

void SoTextKit::OnChange()
{
  Build();
}
