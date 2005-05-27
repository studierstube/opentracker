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
//	UPDATE	   kf	   Klaus Feiler 2002
// ===========================================================================
//  HISTORY:   Time of development: Dezember 1999 - 
//
// ===========================================================================

#include "SoCylinderKit.h"
#include <Inventor/nodes/SoShapeHints.h>

#include "MaterialConstants.h"

SO_KIT_SOURCE(SoCylinderKit);

void SoCylinderKit::initClass() 
{ 
	SO_KIT_INIT_CLASS(SoCylinderKit, Object3dKit, "Object3dKit");
} 

SoCylinderKit::SoCylinderKit() 
{ 
  initSoCylinderKit();
  CreateSensors();
  setUpConnections(TRUE, TRUE);
}

void
SoCylinderKit::CreateSensors()
{
  if (s_return.ok())
  {
    objectOneSensor = new SoFieldSensor(nameUpdate, this);
    objectOneSensor->setPriority(0);

    objectTwoSensor = new SoFieldSensor(nameUpdate, this);
    objectTwoSensor->setPriority(0);
        
    objectThreeSensor = new SoFieldSensor(nameUpdate, this);
    objectThreeSensor->setPriority(0);

    objectFourSensor = new SoFieldSensor(nameUpdate, this);
    objectFourSensor->setPriority(0);

    objectFiveSensor = new SoFieldSensor(nameUpdate, this);
    objectFiveSensor->setPriority(0);

    if(ObjectOneName.getValue() != "")
    {
      SbName Obj1Name = ObjectOneName.getValue();
      Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(Obj1Name);
      if(obj1)
        obj1->connectChild(this);
    }  
    if(ObjectTwoName.getValue() != "")
    {
      SbName Obj2Name = ObjectTwoName.getValue();
      Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
      if(obj2)
        obj2->connectChild(this);
    }
    if(ObjectThreeName.getValue() != "")
    { 
      SbName Obj3Name = ObjectThreeName.getValue();
      Object3dKit *obj3 = (Object3dKit*)SoNode::getByName(Obj3Name);
      if(obj3)
        obj3->connectChild(this);
    }
    if(LineOneName.getValue() != "")
    { 
      SbName Obj4Name = LineOneName.getValue();
      Object3dKit *obj4 = (Object3dKit*)SoNode::getByName(Obj4Name);
      if(obj4)
        obj4->connectChild(this);
    }
    if(LineTwoName.getValue() != "")
    { 
      SbName Obj5Name = LineTwoName.getValue();
      Object3dKit *obj5 = (Object3dKit*)SoNode::getByName(Obj5Name);
      if(obj5)
        obj5->connectChild(this);
    }
  }
}

SoCylinderKit::SoCylinderKit(int number, std::vector<int> objectList, int userID)
{
  initSoCylinderKit();
  
  childNumber.setValue(number);
  setNodeName("Z", number);
  this->userID.setValue(userID);
  MaterialConstants::defineCylinderMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);

  s_return = outcome(-1,NULL); 
  
  bool rightObjects=false;
  int num = 0;

  if (objectList.size() > 2)
  {
    if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
         (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) &&
         (getObject(objectList[2])->isOfType(SoPointKit::getClassTypeId())) )  
    {
      ObjectOneName.setValue(getObject(objectList[0])->getName());
	    ObjectTwoName.setValue(getObject(objectList[1])->getName());
      ObjectThreeName.setValue(getObject(objectList[2])->getName());

      num=3;
      rightObjects = true;
    }
  }
  if (objectList.size() > 1)
  {
    if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
         (getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) ) 
    {
      ObjectThreeName.setValue(getObject(objectList[0])->getName());
	    LineOneName.setValue(getObject(objectList[1])->getName());

      num=2;
      rightObjects = true;
    }
    if ( (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) &&
         (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) ) 
    {
      ObjectThreeName.setValue(getObject(objectList[1])->getName());
	    LineOneName.setValue(getObject(objectList[0])->getName());

      num=2;
      rightObjects = true;
    }
    if ( (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) &&
         (getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) ) 
    {
      LineOneName.setValue(getObject(objectList[0])->getName());
	    LineTwoName.setValue(getObject(objectList[1])->getName());

      num=2;
      rightObjects = true;
    }
  }

  if (rightObjects)
  {
    UpdateAcisObject(); 
    CreateSensors();
	setUpConnections(TRUE, TRUE);
	if (s_return.ok()) // only deselect objects when cylinder is ok
	{
		addDependentObjects(objectList, num);
		deselectObjects(objectList,num);
		createObjectList(objectList, num);
	}
  }
}

/*
// handles 2 cases:
// case 1: objOne = axis of Cylinder, objTwo = a Point on the Cylinder
// case 2: objOne = axis of Cylinder, objTwo = tangent to Cylinder
// no safety measure implemented to avoid parallel lines 
SoCylinderKit::SoCylinderKit(int number, SoNode* objOne, SoNode* objTwo)
{
	initSoCylinderKit();
	
	childNumber.setValue(number);
	transparencyValue.setValue(0.5);
//	color.setValue(cylinderDefaultColor);
	setNodeName("Z", number);
	
	
	if(objOne->isOfType(SoLineKit::getClassTypeId()))
	{
    LineOneName.setValue(objOne->getName());
	}
  else
  {	
	  ObjectOneName.setValue(objOne->getName());
	  ObjectTwoName.setValue(objTwo->getName());
  }
	if(objTwo->isOfType(SoPointKit::getClassTypeId()))
	{
		ObjectThreeName.setValue(objTwo->getName());
	}
	if(objTwo->isOfType(SoLineKit::getClassTypeId()))
	{
		LineTwoName.setValue(objTwo->getName());
	}
	
	UpdateAcisObject();

  CreateSensors();
  
  s_return = outcome(-1,NULL); 
}
*/

// init all parts and fields
void
SoCylinderKit::initSoCylinderKit()
{
	SO_KIT_CONSTRUCTOR(SoCylinderKit); 
	// Add the catalog parts:
	//-----------------------name--|---type------|nuldef|  par   |rite|public
	//SO_KIT_ADD_CATALOG_ENTRY(object,SoCylinder,   FALSE,switchNode,\x0,TRUE);
	//	SO_KIT_ADD_CATALOG_ENTRY(object,SoSwitch,   FALSE,switchNode,\x0,TRUE);
	//	SO_KIT_ADD_CATALOG_ENTRY(cylinder,SoCylinder,   TRUE,object,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(cylinder,SoCylinder,   FALSE,objectGroup,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(sh,SoShapeHints,   FALSE,nearestGroup,\x0,TRUE);
	
	SO_NODE_ADD_FIELD(cylinderRotation, (0.0f, 0.0f, 0.0f, 0.0f));
	SO_NODE_ADD_FIELD(cylinderRadius, (0));
	SO_NODE_ADD_FIELD(cylinderHeight, (0));
	SO_NODE_ADD_FIELD(cylinderCenter, (0.0f, 0.0f, 0.0f));
	
	// fields OpenCascade
	SO_NODE_ADD_FIELD(ObjectOneName, (""));
	SO_NODE_ADD_FIELD(ObjectTwoName, (""));
	SO_NODE_ADD_FIELD(ObjectThreeName, (""));
	SO_NODE_ADD_FIELD(LineOneName, (""));
	SO_NODE_ADD_FIELD(LineTwoName, (""));
	
	
	// Initialize this SoCylinderKit
	SO_KIT_INIT_INSTANCE();
	
	//turn dragging temporarily off
	translationOn.setValue(FALSE);
	rotationOn.setValue(FALSE);
	
	name.setValue("Cylinder");
	
	// do all connections in setUpConnections
	// add children to nearestGroup (preview Objects) in setupConnections after
	// connections have been set!
	
	// getPart here in order to create empty Part
	// anyway, distribution crashes if this line is missing
	SoCylinder* cylinder =  (SoCylinder*) getPart("cylinder", false);

	objectOneSensor=NULL;
	objectTwoSensor=NULL;
	objectThreeSensor=NULL;
	objectFourSensor=NULL;
	objectFiveSensor=NULL;
}

// Destructor
SoCylinderKit::~SoCylinderKit()
{
	if(objectOneSensor)
	{
		objectOneSensor->detach();
		delete objectOneSensor;
	}
	if(objectTwoSensor)
	{
		objectTwoSensor->detach();
		delete objectTwoSensor;
	}
	if(objectThreeSensor)
	{
		objectThreeSensor->detach();
		delete objectThreeSensor;
	}
	if(objectFourSensor)
	{
		objectFourSensor->detach();
		delete objectFourSensor;
	}
	if(objectFiveSensor)
	{
		objectFiveSensor->detach();
		delete objectFiveSensor;
	}
}

// mCoord and aCoord (=axis) must be defined to get a correct result!!!
float
SoCylinderKit::getDistance(SbVec3f penPosition)
{
  double distance;
  
  SPAposition pen;
  pen.set_x(penPosition.getValue()[0]);
  pen.set_y(penPosition.getValue()[1]);
  pen.set_z(penPosition.getValue()[2]);
  
  param_info& ent_info=*(param_info*)NULL_REF;
  if (s_return.ok())
    api_entity_point_distance((ENTITY*)a_Shape->lump()->shell(), pen, pen, distance, ent_info);

	//std::cout << "cylinder: "  << distance << std::endl;
  return float(distance);
}


// input types:
// cylinder:
//  3 points (midpoint, axis endpoint, point on cylinder)
//  1 Line (objOne=axis of Cylinder), 1 Point (objTwoPoint on the Cylinder)
//  2 Lines (objOne=axis of Cylinder, objTwo = tangent to Cylinder)
void
SoCylinderKit::nameUpdate(void *data, SoSensor *sensor)
{
	std::cout << "SoCylinderKit::nameUpdate()" << std::endl;
	
	SoCylinderKit *self=(SoCylinderKit*)data;
	self->triggerAllowed = TRUE;
	self->triggerChange();
}

void SoCylinderKit::UpdateAcisObject()
{
	SPAposition pos1;
	SPAposition pos2;
	EDGE* Lin1 = NULL;
	EDGE* Lin2 = NULL;
	SPAunit_vector direction_n;
	double a_r = 0.0;
	
	if (LineOneName.getValue() != "") // line axis of Cylinder
	{
		SbName Obj1Name = LineOneName.getValue();
		SoLineKit *Line = (SoLineKit*)SoNode::getByName(Obj1Name);
		Lin1 = Line->a_Line;
		pos1 = Line->a_Line->start_pos();
		pos2 = Line->a_Line->end_pos();
	}
	else
	{
		if ((ObjectOneName.getValue() != "") && (ObjectTwoName.getValue() != "")) // 2 points axis of Cylinder
		{
			SbName Obj1Name = ObjectOneName.getValue();
			SbName Obj2Name = ObjectTwoName.getValue();
			SoPointKit *Point1 = (SoPointKit*)SoNode::getByName(Obj1Name);
			SoPointKit *Point2 = (SoPointKit*)SoNode::getByName(Obj2Name);
			pos1 = Point1->a_Point;
			pos2 = Point2->a_Point;
		}
	}
	
	
	if (ObjectThreeName.getValue() != "") // Point on Cylinder
	{
		SbName Obj3Name = ObjectThreeName.getValue();
		SoPointKit *Point3 = (SoPointKit*)SoNode::getByName(Obj3Name);
		
		direction_n = normalise(pos1 - pos2);
		
		a_r = distance_to_line(Point3->a_Point, pos1, direction_n);  
	}
	if (LineTwoName.getValue() != "") // Tangente on Cylinder
	{
		SPAposition pos3,pos4;
		SbName Line2Name = LineTwoName.getValue(); 
		SoLineKit *Line = (SoLineKit*)SoNode::getByName(Line2Name);
		Lin2 = Line->a_Line;
		
		double returnvalue1, returnvalue2;
		closest_points(Lin1->start_pos(),normalise(Lin1->start_pos()-Lin1->end_pos()),Lin2->start_pos(),normalise(Lin2->start_pos()-Lin2->end_pos()),returnvalue1,pos3,returnvalue2,pos4);
		a_r = distance_to_point(pos3,pos4);
		
		direction_n = normalise(pos1 - pos2);
	} 
    
	s_return = api_del_entity(a_Shape);
	s_return = api_solid_cylinder_cone(pos1, pos2, a_r, a_r, a_r, NULL, a_Shape);
		
	if (s_return.ok())
	{  
		// set Inventor
		SbVec3f ydir(0.,1.,0.);
/*		SoComposeRotationFromTo* comprot = new SoComposeRotationFromTo();		
		comprot->from = ydir;
		comprot->to.setValue(direction_n.x(), direction_n.y(), direction_n.z());
*/		
		cylinderHeight.setValue(float(distance_to_point(pos1,pos2)));
		cylinderRadius.setValue(float(a_r));
		cylinderCenter = convertPosition(pos1+(pos2-pos1)/2);
		//cylinderRotation.connectFrom(&(comprot->rotation));
		cylinderRotation.setValue(SbRotation(ydir, 
											SbVec3f(float(direction_n.x()), 
													float(direction_n.y()), 
													float(direction_n.z()))));
	}
	else
		std::cout << "Error can't create Acis Cylinder";
}

void
SoCylinderKit::OnChange()
{
	if (triggerAllowed)
	{
		printf("CylinderKit: onchange called, updating cylinder\n");
		UpdateAcisObject();
	}
	triggerAllowed = FALSE;
}


SbBool SoCylinderKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) {
		// We connect AFTER base class.
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		// do your own special setup here !
		// connect fields
		//translation.connectFrom(&(pCoord));
		
		// do all connections in setUpConnections
		translation.connectFrom(&(cylinderCenter));
		rotation.connectFrom(&(cylinderRotation));
		SoCylinder* cylinder =  (SoCylinder*) getPart("cylinder", false);
		cylinder->radius.connectFrom(&(cylinderRadius));
		cylinder->height.connectFrom(&(cylinderHeight));
		
		SoShapeHints* sh = (SoShapeHints*) getAnyPart("sh", false);
		sh->shapeType.setValue(SoShapeHints::SOLID);
		sh->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);

		// add children to nearestGroup (preview Objects) in setupConnections after
		// connections have been set!
		SoGroup* nearGrp = (SoGroup*) nearestGroup.getValue();
		nearGrp->addChild(cylinder);

		SbName OneName = ObjectOneName.getValue();
		SbName TwoName = ObjectTwoName.getValue();
		SbName ThreeName = ObjectThreeName.getValue();
		SbName FourName = LineOneName.getValue();
		SbName FiveName = LineTwoName.getValue();
		SoNode *objectOne = NULL;
		SoNode *objectTwo = NULL;
		SoNode *objectThree = NULL;
		SoNode *objectFour = NULL;
		SoNode *objectFive = NULL;
		SbBool valid = FALSE;
		
		if(OneName != "")
		{
			objectOne = SoNode::getByName(OneName);
			if (objectOneSensor)
			{
				objectOneSensor->attach(&((SoPointKit*)objectOne)->pCoord);
				valid = TRUE;
			}
		}
		if(TwoName != "")
		{
			objectTwo = SoNode::getByName(TwoName);
			if (objectTwoSensor)
			{
				objectTwoSensor->attach(&((SoPointKit*)objectTwo)->pCoord);
				valid = TRUE;
			}
		}
		if(ThreeName != "")
		{
			objectThree = SoNode::getByName(ThreeName);
			if (objectThreeSensor)
			{
				objectThreeSensor->attach(&((SoPointKit*)objectThree)->pCoord);
				valid = TRUE;
			}
		}
		if(FourName != "")
		{
			objectFour = SoNode::getByName(FourName);
			if (objectFourSensor)
			{
				objectFourSensor->attach(&((Object3dKit*)objectFour)->ChangeObject);
				valid = TRUE;
			}
		}
		if(FiveName != "")
		{
			objectFive = SoNode::getByName(FiveName);
			if (objectFiveSensor)
			{
				objectFiveSensor->attach(&((Object3dKit*)objectFive)->ChangeObject);
				valid=TRUE;
			}
		}
		
		if (valid)
			UpdateAcisObject();
	}
	else {
		// We disconnect BEFORE base class.
		// Disconnect the field sensors.
		//translation.disconnect();
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		translation.disconnect();
		rotation.disconnect();
		SoCylinder* cylinder =  (SoCylinder*) getPart("cylinder", false);
		cylinder->radius.disconnect();
		cylinder->height.disconnect();
		
		if (objectOneSensor)
			objectOneSensor->detach();
		if (objectTwoSensor)
			objectTwoSensor->detach();
		if (objectThreeSensor)
			objectThreeSensor->detach();
		if (objectFourSensor)
			objectFourSensor->detach();
		if (objectFiveSensor)
			objectFiveSensor->detach();
	}
	return !(connectionsSetUp = onOff); 
}

void 
SoCylinderKit::setHighlightMaterial(int currentUserID)
{
	SoMaterial* dummy1 = new SoMaterial;
	SoMaterial* dummy2 = new SoMaterial;
	SoMaterial* dummy3 = new SoMaterial;
	SoMaterial* dummy4 = new SoMaterial;
	MaterialConstants::defineCylinderMaterials(*W, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}
