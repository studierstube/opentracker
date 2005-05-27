// ===========================================================================
//	(C) 2000 Vienna University of Technology
// ===========================================================================
//	NAME:		Cnd.cxx
//	TYPE:		c++ source code
//	PROJECT:	Studierstube Workspace
//	CONTENT:	Geometric Construction in n Dimensions
//	VERSION:	1.3
// ===========================================================================
//	AUTHOR:    hk	   Hannes Kaufmann
//	UPDATE	   kf	   Klaus Feiler 2002
// ===========================================================================
//	HISTORY:   Time of development: Dezember 1999 - 
//
// ===========================================================================


#include "SoSphereKit.h"
#include <Inventor/nodes/SoShapeHints.h>

#include "MaterialConstants.h"

SO_KIT_SOURCE(SoSphereKit);

void SoSphereKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoSphereKit, Object3dKit, "Object3dKit");
} 

SoSphereKit::SoSphereKit() 
{ 
  initSoSphereKit();
  CreateSensors();
  setUpConnections(TRUE, TRUE);
}

void
SoSphereKit::CreateSensors()
{
  if (s_return.ok())
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
}

SoSphereKit::SoSphereKit(int number, std::vector<int> objectList, int userID)
{
  initSoSphereKit();

  childNumber.setValue(number);
  setNodeName("S", number);
  this->userID.setValue(userID);
  MaterialConstants::defineSphereMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);
  s_return = outcome(-1,NULL); 

  bool rightObjects=false;
  int num = 0;

  if (objectList.size() > 1)
  {
    if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
         (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )  
    {
      ObjectOneName = getObject(objectList[0])->getName();
	    ObjectTwoName = getObject(objectList[1])->getName();
      
      num = 2;
      rightObjects=true;
    }
  }
  if (objectList.size() > 2)
  {
    if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
         (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) &&  
         (getObject(objectList[2])->isOfType(SoPointKit::getClassTypeId()))  )  
    {
      ObjectOneName = getObject(objectList[0])->getName();
	    ObjectTwoName = getObject(objectList[1])->getName();
      ObjectThreeName = getObject(objectList[2])->getName();
      
      num = 3;
      rightObjects=true;
    }
  }

  if (rightObjects)
  {
    // sphere calls each point: "hello parent, i'm your child, take me into your list of children"
    // at same time this sphere puts each point into its own parent list
    addDependentObjects(objectList, num);
    deselectObjects(objectList,num);
    createObjectList(objectList, num);

    UpdateAcisObject(); 
    CreateSensors();
    setUpConnections(TRUE, TRUE);
  }
}

// init all parts and fields
void
SoSphereKit::initSoSphereKit()
{
  SO_KIT_CONSTRUCTOR(SoSphereKit); 
  // Add the catalog parts:
  //-----------------------name--|---type------|nuldef|  par   |rite|public
//  SO_KIT_ADD_CATALOG_ENTRY(shapehint,SoShapeHints,  FALSE,switchNode,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(sphere,SoSphere,   FALSE,objectGroup,\x0,TRUE);
  
  //fields
  SO_NODE_ADD_FIELD(radius, (0.0f));
  SO_NODE_ADD_FIELD(sphereCenter, (0.0f, 0.0f, 0.0f));
  
  // fields OpenCascade
  SO_NODE_ADD_FIELD(ObjectOneName, (""));
  SO_NODE_ADD_FIELD(ObjectTwoName, (""));
  SO_NODE_ADD_FIELD(ObjectThreeName, (""));

  // robert has no idea why the following line should be necessary
  // so its disabled for now. if somebody thinks the same: please delete it
  // SO_NODE_ADD_FIELD(dependentObjectsName, (""));
  
  // Initialize this SoSphereKit
  SO_KIT_INIT_INSTANCE();
  
  //turn dragging temporarily off
  translationOn.setValue(FALSE);
  rotationOn.setValue(FALSE);
  
  name.setValue("Sphere");

/*  SoShapeHints* shapeHint = (SoShapeHints*) getPart("shapehint", false);
  shapeHint->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
  shapeHint->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
*/
  // connect fields
  MaterialConstants::defineSphereMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);
  
  SoSphere* sphere = (SoSphere*) getPart("sphere", false);
/*  //sphere = new SoSphere;
  sphere->radius.connectFrom(&(radius));

  translation.connectFrom(&(sphereCenter));
*/
//  SoShapeHints* sh = new SoShapeHints;
//  sh->shapeType.setValue(SoShapeHints::SOLID);
//  sh->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);

//  SoGroup* objGrp = (SoGroup*) objectGroup.getValue();
//  objGrp->addChild(sphere);

//  SoGroup* nearGrp = (SoGroup*) nearestGroup.getValue();
//  nearGrp->addChild(sh);
//  nearGrp->addChild(sphere);
//printf("Added child sphere-------------------------------------------\n");

  objectOneSensor=NULL;
  objectTwoSensor=NULL;
  objectThreeSensor=NULL;
}

// Destructor
SoSphereKit::~SoSphereKit()
{
  if(objectOneSensor)
    delete objectOneSensor;
  if(objectTwoSensor)
    delete objectTwoSensor;
  if(objectThreeSensor)
    delete objectThreeSensor;
}

// Disadvantage : Radius point on Sphere can be selected 
// because points have a priority 
float
SoSphereKit::getDistance(SbVec3f penPosition)
{
  float distance = (float)fabs((penPosition - sphereCenter.getValue()).length() - radius.getValue());
  //	printf( "Spheredist: \t%.3f \n", distance);
  return distance;
}

// input types:
// sphere:
//	2 points (midpoint, point on sphere)
//	3 points (midpoint, 2 points determining radius of sphere)
void
SoSphereKit::nameUpdate(void *data, SoSensor *sensor)
{
	SoSphereKit *self=(SoSphereKit*)data;
	std::cout << "SoSphereKit::nameUpdate()" << std::endl;
	self->triggerAllowed = TRUE;  
	self->triggerChange();
}

void SoSphereKit::UpdateAcisObject()
{
  SbName Obj1Name = ObjectOneName.getValue();
  SbName Obj2Name = ObjectTwoName.getValue();
  
  SoPointKit *Point1 = (SoPointKit*)SoNode::getByName(Obj1Name);
  SoPointKit *Point2 = (SoPointKit*)SoNode::getByName(Obj2Name);

  double a_r = 0.0;
  
  if (ObjectThreeName.getValue() == "")
  {
    if((ObjectTwoName.getValue() != "") & (ObjectOneName.getValue() != ""))
    {
      a_r =  distance_to_point(Point1->a_Point,Point2->a_Point);
    }
  }
  else
  {
    SbName Obj3Name = ObjectThreeName.getValue();
    SoPointKit *Point3 = (SoPointKit*)SoNode::getByName(Obj3Name);
    a_r =  distance_to_point(Point2->a_Point,Point3->a_Point);
  }
 
  s_return = api_del_entity(a_Shape);
//  if(!s_return.ok())
//	  printf("Sphere was NOT deleted\n");
  s_return = api_solid_sphere(Point1->a_Point, a_r, a_Shape);
  if (!s_return.ok()) std::cout << "Error can't create Acis Sphere" << std::endl;
  
//  if (s_return.ok()) std::cout << "Acis Sphere with m: " << Point1->a_Point.x() <<" "<< Point1->a_Point.y() <<" "<< Point1->a_Point.z()
//    << " r: " << radius.getValue() << " A: " << a_r << std::endl;

  if (s_return.ok())
  {    
    radius.setValue(float(a_r));
    sphereCenter = convertPosition(Point1->a_Point);
  }

}

void
SoSphereKit::OnChange()
{
	if (triggerAllowed)
	{
		printf("SphereKit: onchange called, updating Sphere\n");
		triggerAllowed = FALSE;
		UpdateAcisObject();
	}
	triggerAllowed = FALSE;
}

SbBool SoSphereKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) {
		// We connect AFTER base class.
		//SoBaseKit::setUpConnections(onOff, doItAlways);
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		SoSphere* sphere = (SoSphere*) getPart("sphere", false);
		sphere->radius.connectFrom(&(radius));
		
		translation.connectFrom(&(sphereCenter));
		
		SoShapeHints* sh = new SoShapeHints;
		sh->shapeType.setValue(SoShapeHints::SOLID);
		sh->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
		
		SoGroup* nearGrp = (SoGroup*) nearestGroup.getValue();
		nearGrp->addChild(sh);
		nearGrp->addChild(sphere);
		
		// do your own special setup here !
		// connect fields
		//translation.connectFrom(&(pCoord));
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
      if (objectOneSensor) {
			  objectOneSensor->attach(&((SoPointKit*)objectOne)->ChangeObject);
      }
			valid = TRUE;
		}
		if(TwoName != "")
		{
			objectTwo = SoNode::getByName(TwoName);
      if (objectOneSensor) {
        objectTwoSensor->attach(&((SoPointKit*)objectTwo)->ChangeObject);
      }
			valid = TRUE;
		} 
		if(ThreeName != "")
		{
			objectThree = SoNode::getByName(ThreeName);
      if (objectOneSensor) {
			  objectThreeSensor->attach(&((SoPointKit*)objectThree)->ChangeObject);
      }
			valid = TRUE;
		}
		
		if (valid)
			UpdateAcisObject();
	}
	else {
		// We disconnect BEFORE base class.
		// Disconnect the field sensors.
		//translation.disconnect();
		
		//SoBaseKit::setUpConnections(onOff, doItAlways);
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		SoSphere* sphere = (SoSphere*) getPart("sphere", false);
		sphere->radius.disconnect();
		
		translation.disconnect();
		
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
SoSphereKit::setHighlightMaterial(int currentUserID)
{
	SoMaterial* dummy1 = new SoMaterial;
	SoMaterial* dummy2 = new SoMaterial;
	SoMaterial* dummy3 = new SoMaterial;
	SoMaterial* dummy4 = new SoMaterial;
	MaterialConstants::defineSphereMaterials(*W, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}
