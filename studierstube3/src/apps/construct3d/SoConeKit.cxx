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

#include "SoConeKit.h"
#include <Inventor/nodes/SoShapeHints.h>

#include "MaterialConstants.h"

SO_KIT_SOURCE(SoConeKit);

void SoConeKit::initClass() 
{ 
	SO_KIT_INIT_CLASS(SoConeKit, Object3dKit, "Object3dKit");
} 

SoConeKit::SoConeKit() 
{ 
	initSoConeKit();
	CreateSensors();
	setUpConnections(TRUE, TRUE);
}

void
SoConeKit::CreateSensors()
{
  objectOneSensor = new SoFieldSensor(nameUpdate, this);
  objectOneSensor->setPriority(0);

  objectTwoSensor = new SoFieldSensor(nameUpdate, this);
  objectTwoSensor->setPriority(0);
        
  objectThreeSensor = new SoFieldSensor(nameUpdate, this);
  objectThreeSensor->setPriority(0);

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
}

// Input: 3 points: 1st = Midpoint, 2nd = apex = endpoint of axis
//					3rd = point P on cone
// works for all cases of Point P
SoConeKit::SoConeKit(int number, std::vector<int> objectList, int userID)
{
	initSoConeKit();
	
	childNumber.setValue(number);
//	transparencyValue.setValue(0.5);
	setNodeName("K", number);
	this->userID.setValue(userID);
	MaterialConstants::defineConeMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
	SoMaterial* mat = (SoMaterial*) getPart("material", false);
	mat->copyFieldValues(W);
	s_return = outcome(-1,NULL); 
	
	if (objectList.size() > 2)
	{
		if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
			(getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) &&  
			(getObject(objectList[2])->isOfType(SoPointKit::getClassTypeId()))  )  
		{
			ObjectOneName = getObject(objectList[0])->getName();
			ObjectTwoName = getObject(objectList[1])->getName();
			ObjectThreeName = getObject(objectList[2])->getName();
			
			UpdateAcisObject(); 
			
			if (s_return.ok())
			{
				addDependentObjects(objectList, 3);
				deselectObjects(objectList,3);
				createObjectList(objectList, 3);
				CreateSensors();
				setUpConnections(TRUE, TRUE);
			}
		}
	}
}

// init all parts and fields
void
SoConeKit::initSoConeKit()
{
	SO_KIT_CONSTRUCTOR(SoConeKit); 
	// Add the catalog parts:
	//-----------------------name--|---type------|nuldef|  par   |rite|public
//	SO_KIT_ADD_CATALOG_ENTRY(object,SoCone,   FALSE,switchNode,\x0,TRUE);
	
	//fields
  SO_NODE_ADD_FIELD(coneRotation, (0.0f, 0.0f, 0.0f, 0.0f));
	SO_NODE_ADD_FIELD(coneCenter, (0.0f, 0.0f, 0.0f));
	SO_NODE_ADD_FIELD(coneRadius, (0));
	SO_NODE_ADD_FIELD(coneHeight, (0));

	SO_NODE_ADD_FIELD(ObjectOneName, (""));
	SO_NODE_ADD_FIELD(ObjectTwoName, (""));
	SO_NODE_ADD_FIELD(ObjectThreeName, (""));
	
	// Initialize this SoConeKit
	SO_KIT_INIT_INSTANCE();
	
	//turn dragging temporarily off
	translationOn.setValue(FALSE);
	rotationOn.setValue(FALSE);
	
	name.setValue("Cone");
	
	// connect all fields in setUpConnections

	objectOneSensor=NULL;
	objectTwoSensor=NULL;
	objectThreeSensor=NULL;
}

// Destructor
SoConeKit::~SoConeKit()
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
}

// mCoord and aCoord (=axis) must be defined to get a correct result!!!
float
SoConeKit::getDistance(SbVec3f penSPAposition)
{
  double distance;
  
  SPAposition pen;
  pen.set_x(penSPAposition.getValue()[0]);
  pen.set_y(penSPAposition.getValue()[1]);
  pen.set_z(penSPAposition.getValue()[2]);
  
  param_info& ent_info=*(param_info*)NULL_REF;
  if (s_return.ok())
    api_entity_point_distance((ENTITY*)a_Shape->lump()->shell(), pen, pen, distance, ent_info);

	return float(distance);
}


// input types:
// cone:
//  3 points (midpoint, axis endpoint, point on cylinder)
void
SoConeKit::nameUpdate(void *data, SoSensor *sensor)
{
	std::cout << "SoConeKit::nameUpdate()" << std::endl;
	SoConeKit *self=(SoConeKit*)data;
	self->triggerAllowed = TRUE;
	self->triggerChange();
}

void SoConeKit::UpdateAcisObject()
{
	
	SbName Obj1Name = ObjectOneName.getValue();
	SbName Obj2Name = ObjectTwoName.getValue();
	SbName Obj3Name = ObjectThreeName.getValue();
	
	
	SoPointKit *Point1 = (SoPointKit*)SoNode::getByName(Obj1Name);
	SoPointKit *Point2 = (SoPointKit*)SoNode::getByName(Obj2Name);
	SoPointKit *Point3 = (SoPointKit*)SoNode::getByName(Obj3Name);
	
  SPAunit_vector direction_n = normalise(Point2->a_Point - Point1->a_Point);

  SPAposition point_on_line;
  double dis_point_axis; //= distance_to_line(Point3->a_Point, Point1->a_Point, direction_n); 
  
  Line_Plane_Intersect(Point1->a_Point,Point2->a_Point, Point3->a_Point, point_on_line,dis_point_axis);


  double dis_bottom_top = distance_to_point(Point2->a_Point, Point1->a_Point);
  double dis_point_on_line_top = distance_to_point(point_on_line, Point2->a_Point);
  double a_r = ((dis_point_axis * dis_bottom_top) / dis_point_on_line_top);
	
	s_return = api_solid_cylinder_cone(Point1->a_Point, Point2->a_Point, a_r, a_r, 0.0, NULL, a_Shape);
  if (!s_return.ok()) 
    std::cout << "Error can't create Acis Cone" <<  std::endl;
  else
  {
	  // set Inventor      
	  SoComposeRotationFromTo* comprot = new SoComposeRotationFromTo();
	  SbVec3f ydir(0.,1.,0.);
	  comprot->from = ydir;
	  comprot->to.setValue(float(direction_n.x()), float(direction_n.y()), float(direction_n.z()));
	  coneRotation.connectFrom(&(comprot->rotation));
	  coneHeight.setValue(float(dis_bottom_top));
	  coneRadius.setValue(float(a_r));

	  coneCenter = convertPosition(Point1->a_Point + (Point2->a_Point - Point1->a_Point)/2);
  }
}

void
SoConeKit::OnChange()
{
	if (triggerAllowed)
	{
		printf("Updating Cone\n");
		UpdateAcisObject();
	}
	triggerAllowed = FALSE;
}

SbBool SoConeKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) {
		// We connect AFTER base class.
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		// do your own special setup here !
		// connect fields
		
		translation.connectFrom(&(coneCenter));
		rotation.connectFrom(&(coneRotation));
		cone = new SoCone;
		cone->bottomRadius.connectFrom(&(coneRadius));
		cone->height.connectFrom(&(coneHeight));
		
		SoShapeHints* sh = new SoShapeHints;
		sh->shapeType.setValue(SoShapeHints::SOLID);
		sh->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
		
		SoGroup* objGrp = (SoGroup*) objectGroup.getValue();
		objGrp->addChild(cone);
		// add children to nearestGroup (preview Objects) in setupConnections after
		// connections have been set!
		SoGroup* nearGrp = (SoGroup*) nearestGroup.getValue();
		nearGrp->addChild(sh);
		nearGrp->addChild(cone);
		
		SbName OneName = ObjectOneName.getValue();
		SbName TwoName = ObjectTwoName.getValue();
		SbName ThreeName = ObjectThreeName.getValue();
		SoNode *objectOne = NULL;
		SoNode *objectTwo = NULL;
		SoNode *objectThree = NULL;
		SbBool valid = FALSE;
		
		if(OneName != "")
		{
			objectOne = SoNode::getByName(OneName);
			objectOneSensor->attach(&((SoPointKit*)objectOne)->pCoord);
			valid = TRUE;
		}
		if(TwoName != "")
		{
			objectTwo = SoNode::getByName(TwoName);
			objectTwoSensor->attach(&((SoPointKit*)objectTwo)->pCoord);
			valid = TRUE;
		}
		if(ThreeName != "")
		{
			objectThree = SoNode::getByName(ThreeName);
			objectThreeSensor->attach(&((SoPointKit*)objectThree)->pCoord);
			valid = TRUE;
		}
		
		if (valid)
			UpdateAcisObject();
		
	}
	else {
		// We disconnect BEFORE base class.
		// Disconnect the field sensors.
		//translation.disconnect();
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		if (objectOneSensor)
			objectOneSensor->detach();
		if (objectTwoSensor)
			objectTwoSensor->detach();
		if (objectThreeSensor)
			objectThreeSensor->detach();
	}
	return !(connectionsSetUp = onOff); 
}

void 
SoConeKit::setHighlightMaterial(int currentUserID)
{
	SoMaterial* dummy1 = new SoMaterial;
	SoMaterial* dummy2 = new SoMaterial;
	SoMaterial* dummy3 = new SoMaterial;
	SoMaterial* dummy4 = new SoMaterial;
	MaterialConstants::defineConeMaterials(*W, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}
