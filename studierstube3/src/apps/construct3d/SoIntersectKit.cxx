// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Cnd.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//  AUTHOR:    rs      Klaus Feiler
// ===========================================================================
//  HISTORY:   Time of development: January 2002 - 
//
// ===========================================================================

#include "SoIntersectKit.h"
#include "transf.hxx"
#include <Inventor/nodes/SoShapeHints.h> 

#include "SoPointKit.h"
#include "SoCubeKit.h"
#include "SoCurveKit.h"
#include "SoCylinderKit.h"
#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include "SoSphereKit.h"
#include "SoSurfaceKit.h"
#include "SoBoolKit.h"

SO_KIT_SOURCE(SoIntersectKit);

void SoIntersectKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoIntersectKit, Object3dKit, "Object3dKit");
} 

SoIntersectKit::SoIntersectKit() 
{ 
	printf("IntersectKit:: Distribution - calling this constructor1\n");
	initSoIntersectKit();
//    Build();  // Build Intersection
    CreateSensors();
    setUpConnections(TRUE, TRUE);
}


// ATTENTION: This constructor is only needed for addSlice in c3d.cxx
// addSlice should be rewritten very similar to addIntersect
// this is the only constructor which is allowed to have no userID passed to it
// the userID is set in SetIntersectKit which is called in c3d.cxx right after
// initializing an empty intersectionKit with this Constructor
SoIntersectKit::SoIntersectKit(int number) 
{ 
  initSoIntersectKit();
  
  childNumber.setValue(number);
//  transparencyValue.setValue(0.1f);
  //	color.setValue(intersectDefaultColor);
  setNodeName("I", number);
  //CreateSensors();
  //setUpConnections(TRUE, TRUE);  
  //Build();  // Build Intersection
  // take care that s_return is false by default. 
  // Otherwise it is always true and new intersection objects are generated even if 
  // no valid objects or no objects at all are selected
  s_return = outcome(-1,NULL);
}


// initializing an empty intersectionKit with this Constructor
SoIntersectKit::SoIntersectKit(std::vector<int> objectList, 
							   SoSelection* selectionRoot, 
							   SoSFInt32* objSum, 
							   int userID) 
{ 
	initSoIntersectKit();

	childNumber.setValue(objSum->getValue());
	setNodeName("I", objSum->getValue());
	s_return = outcome(-1,NULL);
	// do NOT add the IntersectionKit to the scene graph here
	// this would trigger a setUpConnections on the distributed slave side
	// BEFORE the dependant object names are known, therefore the connections
	// cannot be set up correctly
	/*selectionRoot->addChild(this);*/
	SetIntersectKit(objectList, selectionRoot, objSum, userID);
}

void SoIntersectKit::SetIntersectKitSlice(std::vector<int> objectList, SoSelection* selectionRoot, SoSFInt32* objSum, int userID)
{
  std::cout << "SoIntersectKit SetIntersectKitSlice: " << objSum->getValue() << std::endl;
  this->objSum = objSum;
  this->selectionRoot = selectionRoot;
  thisobjectName = Get_ObjectName("I_", (objSum->getValue()));
  this->userID.setValue(userID);
  
  bool rightObjects=false;
  if (objectList.size() >= 2)
  {
    // SLICING BODY
    if ( (isObject(objectList[0])) &&
      (isObject(objectList[1])) )
    {
      Object1Name.setValue(getObject(objectList[0])->getName());
      Object2Name.setValue(getObject(objectList[1])->getName());
    
      intersectionType.setValue(SLICE_BODY);
      rightObjects=true;      
    }
  }

    if (rightObjects)
  {
    std::cout << "right Objects intersectionkit" << std::endl;
    addDependentObjects(objectList, 2);
    deselectObjects(objectList,2);
    createObjectList(objectList, 2);
    
    CreateSensors();
    setUpConnections(TRUE, TRUE);
  }
}

void SoIntersectKit::SetIntersectKit(std::vector<int> objectList, SoSelection* selectionRoot, SoSFInt32* objSum, int userID)
{
  std::cout << "SoIntersectKit SetIntersectKit: " << objSum->getValue() << std::endl;
  this->objSum = objSum;
  this->selectionRoot = selectionRoot;
  thisobjectName = Get_ObjectName("I_", (objSum->getValue()));
  this->userID.setValue(userID);
  
  bool rightObjects=false;
  
  if (objectList.size() >= 2)
  {
    // PLANE_PLANE_INTERSECTION
    if ( (getObject(objectList[0])->isOfType(SoPlaneKit::getClassTypeId())) &&
      (getObject(objectList[1])->isOfType(SoPlaneKit::getClassTypeId())) )  
    {
      Object1Name = getObject(objectList[0])->getName();
      Object2Name = getObject(objectList[1])->getName();
      
      intersectionType.setValue(PLANE_PLANE);
      rightObjects=true;
    } else
      // EDGE_EDGE_INTERSECTION
    if ( (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) &&
      (getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) )  
    {
      Object1Name = getObject(objectList[0])->getName();
      Object2Name = getObject(objectList[1])->getName();
      
      intersectionType.setValue(EDGE_EDGE);
      rightObjects=true;
    } else
    if ( (getObject(objectList[0])->isOfType(SoCurveKit::getClassTypeId())) &&
      (getObject(objectList[1])->isOfType(SoCurveKit::getClassTypeId())) )  
    {
      Object1Name = getObject(objectList[0])->getName();
      Object2Name = getObject(objectList[1])->getName();
      
      intersectionType.setValue(EDGE_EDGE);
      rightObjects=true;
    } else
    if ( (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) &&
      (getObject(objectList[1])->isOfType(SoCurveKit::getClassTypeId())) )  
    {
      Object1Name = getObject(objectList[0])->getName();
      Object2Name = getObject(objectList[1])->getName();
      
      intersectionType.setValue(EDGE_EDGE);
      rightObjects=true;
    } else
    if ( (getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) &&
      (getObject(objectList[0])->isOfType(SoCurveKit::getClassTypeId())) )  
    {
      Object1Name = getObject(objectList[1])->getName();
      Object2Name = getObject(objectList[0])->getName();
      
      intersectionType.setValue(EDGE_EDGE);
      rightObjects=true;
    } else
    //BODY_PLANE_INTERSECTION
    if ( (getObject(objectList[0])->isOfType(SoCurveKit::getClassTypeId())) && 
      (getObject(objectList[1])->isOfType(SoPlaneKit::getClassTypeId())) )
    {
      Object1Name = getObject(objectList[0])->getName();
      Object2Name = getObject(objectList[1])->getName();
      
      intersectionType.setValue(CURVE_OBJECT);
      rightObjects=true;      
    } else
    if ( (getObject(objectList[1])->isOfType(SoCurveKit::getClassTypeId())) && 
      (getObject(objectList[0])->isOfType(SoPlaneKit::getClassTypeId())) )
    {
      Object1Name = getObject(objectList[1])->getName();
      Object2Name = getObject(objectList[0])->getName();
      
      intersectionType.setValue(CURVE_OBJECT);
      rightObjects=true;      
    } else
    if ( (isObject(objectList[0]) ) && 
      (getObject(objectList[1])->isOfType(SoPlaneKit::getClassTypeId())) )
    {
      Object1Name = getObject(objectList[0])->getName();
      Object2Name = getObject(objectList[1])->getName();
      
      intersectionType.setValue(BODY_PLANE);
      rightObjects=true;      
    } else
    if ( (isObject(objectList[1]) )  && 
      (getObject(objectList[0])->isOfType(SoPlaneKit::getClassTypeId())) )
    {
      Object1Name = getObject(objectList[1])->getName();
      Object2Name = getObject(objectList[0])->getName();
      
      intersectionType.setValue(BODY_PLANE);
      rightObjects=true;      
    } else
      // CURVE_OBJECT_INTERSECTION
    if ( (isObject(objectList[0])) &&
      (getObject(objectList[1])->isOfType(SoCurveKit::getClassTypeId())))
    {
      Object1Name = getObject(objectList[1])->getName();
      Object2Name = getObject(objectList[0])->getName();
      
      intersectionType.setValue(CURVE_OBJECT);
      rightObjects=true;      
    } else
    if ( (isObject(objectList[1])) &&
      (getObject(objectList[0])->isOfType(SoCurveKit::getClassTypeId())))
    {
      Object1Name = getObject(objectList[0])->getName();
      Object2Name = getObject(objectList[1])->getName();
      
      intersectionType.setValue(CURVE_OBJECT);
      rightObjects=true;      
    } else
      // LINE_OBJECT_INTERSECTION
    if ( (isObject(objectList[0])) &&
      (getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())))
    {
      Object1Name = getObject(objectList[1])->getName();
      Object2Name = getObject(objectList[0])->getName();
      
      intersectionType.setValue(LINE_OBJECT);
      rightObjects=true;      
    } else
    if ( (isObject(objectList[1])) &&
      (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())))
    {
      Object1Name = getObject(objectList[0])->getName();
      Object2Name = getObject(objectList[1])->getName();
      
      intersectionType.setValue(LINE_OBJECT);
      rightObjects=true;      
    } else
    // OBJECT_OBJECT_INTERSECTION
    if ( (isObject(objectList[0])) &&
      (isObject(objectList[1])) )
    {
//      Object1Name = getObject(objectList[0])->getName();
//      Object2Name = getObject(objectList[1])->getName();
      Object1Name.setValue(getObject(objectList[0])->getName());
      Object2Name.setValue(getObject(objectList[1])->getName());
//printf("Intersectkit: setting object names -----------------------------------\n");
      intersectionType.setValue(OBJECT_OBJECT);
      rightObjects=true;      
    }
  }
  
  if (rightObjects)
  {
    std::cout << "right Objects intersectkit" << std::endl;
	// by adding the IntersectionKit to the scene graph AFTER the object
	// names have been set, apparently setUpConnections is called in the
	// distribution automatically AFTER the object names are known and 
	// therefore the connections to the intersecting objects can be set up
	// correctly
	selectionRoot->addChild(this);
    addDependentObjects(objectList, 2);
    deselectObjects(objectList,2);
    createObjectList(objectList, 2);
    
    CreateSensors();
	//printf("SetIntersectKit: sensors created, calling setupconnections now\n");
    setUpConnections(TRUE, TRUE);
    //Build();  // Build Intersection
  }
}


void
SoIntersectKit::CreateSensors()
{
  //if (s_return.ok())
//  {
    std::cout << "SoIntersectionKit: create sensors" << std::endl;
    std::cout << "object 1 Name: " << Object1Name.getValue().getString() << "  object 2 name: " << Object2Name.getValue().getString() << std::endl;
    Object3dKit *obj1 = NULL;
    Object3dKit *obj2 = NULL;
    objectOneSensor = new SoFieldSensor(nameUpdate, this);
    objectOneSensor->setPriority(0);
    
    objectTwoSensor = new SoFieldSensor(nameUpdate, this);
    objectTwoSensor->setPriority(0);
    
	SensorsCreated = TRUE;

//	if(Object1Name.getValue() == "") 
//		return;
	if(Object1Name.getValue() != "") 
	{
	obj1 = (Object3dKit*)SoNode::getByName(Object1Name.getValue());
    std::cout << "obj1 childnumber: " << obj1->childNumber.getValue() << " name: " << obj1->getName().getString() << std::endl;
    if(obj1)
      obj1->connectChild(this);
	}
    
//	if(Object2Name.getValue() == "") 
//		return;
	if(Object2Name.getValue() != "") 
	{
    obj2 = (Object3dKit*)SoNode::getByName(Object2Name.getValue());
    std::cout << "obj2 childnumber: " << obj2->childNumber.getValue() << " name: " << obj2->getName().getString() << std::endl;
    if(obj2)
      obj2->connectChild(this);
    }
  else
  std::cout << "SoIntersectionKit: can't create sensors" << std::endl;
}

// init all parts and fields
void
SoIntersectKit::initSoIntersectKit()
{
  SO_KIT_CONSTRUCTOR(SoIntersectKit); 
  // Add the catalog parts:
  //-----------------------name--|---type------|nuldef|  par   |rite|public
  //SO_KIT_ADD_CATALOG_ENTRY(objectVertexProperty,SoVertexProperty,FALSE,switchNode,\x0,TRUE);
  //SO_KIT_ADD_CATALOG_ENTRY(objectShapeHints,SoShapeHints,FALSE,switchNode,\x0,TRUE);
  //SO_KIT_ADD_CATALOG_ENTRY(objectFaceSet,SoFaceSet,   FALSE,switchNode,\x0,TRUE);
  
  //fields
  SO_NODE_ADD_FIELD(Object1Name, (""));
  SO_NODE_ADD_FIELD(Object2Name, (""));
  SO_NODE_ADD_FIELD(intersectionType, (0));
  SO_NODE_ADD_FIELD(objectNames, (""));  //for EDGE object intersection
  SO_NODE_ADD_FIELD(numofObjectNames, (0));//for EDGE object intersection
  SO_NODE_ADD_FIELD(pCoord, (0.0f, 0.0f, 0.0f));//for PLANE_PLANE intersection, save first coord of line 
  SO_NODE_ADD_FIELD(qCoord, (0.0f, 0.0f, 0.0f));//for PLANE_PLANE intersection, save second coord of line
  
  // Initialize this SoIntersectKit
  SO_KIT_INIT_INSTANCE();
  //turn dragging temporarily off
  translationOn.setValue(FALSE);
  rotationOn.setValue(FALSE);
  
  a_Shape = NULL;
  sliceResultOne = NULL;
  sliceResultTwo = NULL;
  objSum = NULL;
  objectOneSensor = NULL;
  objectTwoSensor = NULL;
  
  name.setValue("Intersect");
  SensorsCreated = FALSE;
}

// Destructor
SoIntersectKit::~SoIntersectKit()
{
printf("IntersectionKit Destructor called!!!!\n");

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

//  RemoveAllCallbacks();
/*  if (intersectionType.getValue() == OBJECT_OBJECT)
  {
	SoCurveKit *obj = (SoCurveKit*) getObject(this->childNumber.getValue()+1);
	if(obj)
	{
	   printf("Disconnecting intersection curve \n");
	   this->disconnectChild(obj);
	}
  }
*/
}

// Disadvantage	: Radius point on Sphere can be selected 
// because points have a priority 
float
SoIntersectKit::getDistance(SbVec3f penSPAposition)
{
  return maxDistance;
}

void
SoIntersectKit::nameUpdate(void *data, SoSensor *sensor)
{
  std::cout << "SoIntersectKit::nameUpdate()" << std::endl;
  SoIntersectKit *self=(SoIntersectKit*)data;
  self->triggerAllowed = TRUE;
  //self->Build();
  self->triggerChange();
}

SbBool SoIntersectKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
  if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    Object3dKit::setUpConnections(onOff, doItAlways);
    
    // do your own special setup here !
    // connect fields
    //translation.connectFrom(&(pCoord));
    SbName OneName = Object1Name.getValue();
    SbName TwoName = Object2Name.getValue();
    SoNode *objectOne = NULL;
    SoNode *objectTwo = NULL;
    SbBool validObject = FALSE;

	if (!SensorsCreated)
		CreateSensors();
	printf("IntersectKit::setUpConnections OneName %s \n",OneName.getString());
	printf("IntersectKit::setUpConnections TwoName %s \n",TwoName.getString());
    if(OneName != "")
    {
      objectOne = SoNode::getByName(OneName);
      objectOneSensor->attach(&((Object3dKit*)objectOne)->ChangeObject);
      validObject = TRUE;
    }
    if(TwoName != "")
    {
      objectTwo = SoNode::getByName(TwoName);
      objectTwoSensor->attach(&((Object3dKit*)objectTwo)->ChangeObject);
      validObject = TRUE;
    }
    if (validObject)
        Build();
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
    
  }
  return !(connectionsSetUp = onOff); 
}

void
SoIntersectKit::OnChange()
{
  if (triggerAllowed)
  {
    printf("Updating IntersectionKit, onchange\n");
    Build();
  }
  triggerAllowed = FALSE;
}

void
SoIntersectKit::Build()
{
  //std::cout << "make Intersection:"<< intersectionType.getValue() << std::endl;
  
  SbName Obj1Name = Object1Name.getValue();
  SbName Obj2Name = Object2Name.getValue();
  
  if(Obj1Name != "" && Obj2Name != "")
  {
    if (intersectionType.getValue() == SLICE_BODY)
    {
      SlicingBODY(Obj1Name, Obj2Name); 
    }
    if (intersectionType.getValue() == OBJECT_OBJECT)
    {
      Object_Object_int(Obj1Name, Obj2Name); 
    }
    if (intersectionType.getValue() == LINE_OBJECT)
    {
      SoLineKit *obj1 = (SoLineKit*)SoNode::getByName(Obj1Name);
      Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
      
      //stretching of the line
      // robert's opinion: this does NOT look very good. who guarantees that we stay in 100*length?!
      EDGE* line;
      SPAposition pos1 =  obj1->a_Line->start_pos() + 100*(obj1->a_Line->end_pos() - obj1->a_Line->start_pos());
      SPAposition pos2 =  obj1->a_Line->end_pos() + 100*(obj1->a_Line->start_pos() - obj1->a_Line->end_pos());
      s_return = api_mk_ed_line(pos1, pos2, line);
      if (!s_return.ok()) line = obj1->a_Line;
      
      if (obj2->name.getValue() == "Plane")
      {
        std::cout << "line Plane Intersect" << std::endl;
        EGDE_FACE_int(line, ((SoPlaneKit*)obj2)->GetPlaneFace(0));
        //EGDE_FACE_int(line, ((SoPlaneKit*)obj2)->GetPlaneFace(-1));
      }
      else if (obj2->name.getValue() == "Surface")
      {
        EGDE_FACE_int(line, ((SoSurfaceKit*)obj2)->getFace());
      }
      else
        EGDE_FACE_int(line, obj2->a_Shape->lump()->shell()->face());
    }	
    if (intersectionType.getValue() == CURVE_OBJECT)
    {
      SoCurveKit *obj1 = (SoCurveKit*)SoNode::getByName(Obj1Name);
      Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
      
      if (obj1->CurveOp.getValue() != CURVE_INTERSECTION)
      {
        if (obj2->name.getValue() == "Plane")
        {
          EGDE_FACE_int(obj1->a_Curve, ((SoPlaneKit*)obj2)->GetPlaneFace(0));
          //EGDE_FACE_int(obj1->a_Curve, ((SoPlaneKit*)obj2)->GetPlaneFace(-1));
        }
        else if (obj2->name.getValue() == "Surface")
        {
          EGDE_FACE_int(obj1->a_Curve, ((SoSurfaceKit*)obj2)->getFace());
        }
        else
          EGDE_FACE_int(obj1->a_Curve, obj2->a_Shape->lump()->shell()->face());
      }
      else
      {
        if (obj2->name.getValue() == "Plane")
        {
          EGDE_FACE_int(obj1->a_Curve, ((SoPlaneKit*)obj2)->GetPlaneFace(0));
          //EGDE_FACE_int(obj1->a_Curve, ((SoPlaneKit*)obj2)->GetPlaneFace(-1));
        }
        else if (obj2->name.getValue() == "Surface")
        {
          EGDE_FACE_int(obj1->a_intCurve, ((SoSurfaceKit*)obj2)->getFace());
        }
        else
          EGDE_FACE_int(obj1->a_intCurve, obj2->a_Shape->lump()->shell()->face());
      }
    }	
    if (intersectionType.getValue() == PLANE_PLANE)
    {
      SoPlaneKit *obj1 = (SoPlaneKit*)SoNode::getByName(Obj1Name);
      SoPlaneKit *obj2 = (SoPlaneKit*)SoNode::getByName(Obj2Name);
      
	  // Update both planes here for distribution (!!!)
	  // Otherwise the plane values in the distribution are not correct 
	  // from the beginning and will not be updated until both planes have 
	  // been moved for the first time
	  // The problem was found when intersecting two planes of symmetry
	  obj1->UpdateAcisObject();
	  obj2->UpdateAcisObject();
      PLANE_PLANE_int(obj1->a_Plane, obj2->a_Plane);    
    }
    if (intersectionType.getValue() == EDGE_EDGE)
    {
      Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(Obj1Name);
      Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
      if ((obj1->name.getValue() == "Curve") && (obj2->name.getValue() == "Curve"))
      {
        if ( (((SoCurveKit*)obj1)->CurveOp.getValue() == CURVE_INTERSECTION) && ((SoCurveKit*)obj2)->CurveOp.getValue() == CURVE_INTERSECTION)
          BODY_BODY_int((((SoCurveKit*)obj1)->a_intCurve), (((SoCurveKit*)obj2)->a_intCurve));
        else
          EDGE_EDGE_int(((SoCurveKit*)obj1)->a_Curve, ((SoCurveKit*)obj2)->a_Curve);
      }
      else if ((obj1->name.getValue() == "Line") && (obj2->name.getValue() == "Curve"))
        EDGE_EDGE_int(((SoLineKit*)obj1)->a_Line, ((SoCurveKit*)obj2)->a_Curve);
      else
        EDGE_EDGE_int(((SoLineKit*)obj1)->a_Line, ((SoLineKit*)obj2)->a_Line);
    }
    if (intersectionType.getValue() == BODY_PLANE)
    {
      BODY_PLANE_int(Obj2Name, Obj1Name);
    }
  }
  else
    std::cout << "SoIntersectKit Error: no Objects found" << std::endl;
  
}

void SoIntersectKit::BODY_BODY_int(BODY* bodyOne, BODY* bodyTwo)
{
  std::cout<< "BODY_BODY_int intersect two intersection curves" << std::endl;
  
  SoMFName objectNames_of_todeletingPoint;
  int numof_todeletingPoint = 0;
  
  ENTITY_LIST *intPs = new ENTITY_LIST;
  
  //FILE *output = fopen("a_intersect_objectBody.txt","w");
  //debug_entity(Objectface,output);
  //fclose(output);

  int numofIntersection = 0;
  int a = 1;
  std::vector<SPAposition> coords(100);
  int numofPoints = 0;
  curve_curve_int* intersects=NULL;

  if ((bodyOne != NULL) && (bodyTwo != NULL))
    for (LUMP *lumOne = bodyOne->lump();lumOne != NULL; lumOne = lumOne->next())
    {
      for (SHELL *shelOne = lumOne->shell();shelOne != NULL; shelOne = shelOne->next())
      {
        for (WIRE *wirOne = shelOne->wire();wirOne != NULL; wirOne= wirOne->next())
        {
          COEDGE* st_coedgOne = wirOne->coedge();
          COEDGE* coedgOne = st_coedgOne;
          do
          { 
            for (LUMP *lumTwo = bodyTwo->lump();lumTwo != NULL; lumTwo = lumTwo->next())
            {
              for (SHELL *shelTwo = lumTwo->shell();shelTwo != NULL; shelTwo = shelTwo->next())
              {
                for (WIRE *wirTwo = shelTwo->wire();wirTwo != NULL; wirTwo= wirTwo->next())
                {
                  COEDGE* st_coedgTwo = wirTwo->coedge();
                  COEDGE* coedgTwo = st_coedgTwo;
                  do
                  { 
                    api_intersect_curves(coedgOne->edge(), coedgTwo->edge(), TRUE, intersects, NULL);
                    while (intersects)
                    {    
                      coords[numofPoints] = intersects->int_point;
                      numofPoints++;  
                      numofIntersection++;
                      intersects = intersects->next;
                    }
                    coedgTwo = coedgTwo->next();
                  }
                  while (coedgTwo != st_coedgTwo && coedgTwo != NULL);
                }
              }
            }
            coedgOne = coedgOne->next();
          }
          while (coedgOne != st_coedgOne && coedgOne != NULL);
        }
      }
    }
  
  SoPointKit *nearestObject;
  int nearestCoord;
  
  for (int i=0; i < numofObjectNames.getValue(); i++)
  {
    nearestObject = NULL; 
    SoPointKit *pointObj = (SoPointKit*)SoNode::getByName(objectNames[i]);
    
    double dis=maxPointDistance;
    for(int l = 0; l < numofPoints; l++)
    {
      if (dis > distance_to_point(coords[l], pointObj->a_Point))
      {
        dis = distance_to_point(coords[l], pointObj->a_Point);
        nearestObject = pointObj;
        nearestCoord = l;
      }  
    }
    
    if (nearestObject)
    {
      nearestObject->Update(convertPosition(coords[nearestCoord]).getValue());
      if (pointObj->visible.getValue() == SO_SWITCH_NONE)
      {
        pointObj->setvisible();
      }
      
      numofPoints--;
      for (int a=nearestCoord; a < numofPoints; a++)
        coords[a] = coords[a+1];
    }
    else 
    {
      objectNames_of_todeletingPoint.setNum(++numof_todeletingPoint);
      objectNames_of_todeletingPoint.set1(numof_todeletingPoint-1,objectNames[i].getString());
    }
    
  }
  
  for (i=0; i < numof_todeletingPoint; i++)
  {
    SoPointKit *pointObj = (SoPointKit*)SoNode::getByName(objectNames_of_todeletingPoint[i]);
    if (pointObj->visible.getValue() == SO_SWITCH_ALL)
      pointObj->setinvisible();
  }    
  
  if (objSum != NULL)
  {
    for(int l = 0; l < numofPoints; l++)
    {
      SbVec3f coord(convertPosition(coords[l]).getValue());
      SoPointKit *P1 = new SoPointKit(objSum->getValue()+1, coord, this->userID.getValue());
      P1->ObjectName = "";
      P1->translationOn.setValue(FALSE);
      P1->rotationOn.setValue(FALSE);
      P1->isLocked.setValue(TRUE);
//      P1->layer.setValue(intersectionObjectLayer);
      selectionRoot->addChild(P1);
      objSum->setValue(objSum->getValue()+1);
      numofObjectNames.setValue(numofObjectNames.getValue()+1);
      objectNames.setNum(numofObjectNames.getValue());
      objectNames.set1(numofObjectNames.getValue()-1,P1->getName().getString());
    }
  }
  ACIS_DELETE(intPs);

  if (numofIntersection > 0)
    s_return = outcome();
  
  if (!s_return.ok())
    std::cout << "SoIntersectionKit::BODY_BODY_int: Error: can't intersect BODY BODY" << std::endl;

}

EDGE* SoIntersectKit::getallEdges(BODY* body)
{
  EDGE* newedge;
  EDGE* edge1;
  ENTITY_LIST edge_list;
  int counter = 0;
  
  for (LUMP *lum = body->lump();lum != NULL; lum = lum->next())
  {
    for (SHELL *shel = lum->shell();shel != NULL; shel = shel->next())
    {
      for (WIRE *wir = shel->wire();wir != NULL; wir= wir->next())
      {
        COEDGE* st_coedg = wir->coedge();
        COEDGE* coedg = st_coedg;
        do
        { 
          if (counter == 0)
            edge1 = coedg->edge();
          else
          {
            s_return = api_combine_edges(edge1, coedg->edge(), newedge, NULL, NULL);
            if (!s_return.ok())
              assert(0);
            api_copy_entity((ENTITY*)newedge, ((ENTITY*&)edge1), NULL);
            if (!s_return.ok())
              assert(0);
          }
          coedg = coedg->next();
          counter++;
        }
        while (coedg != st_coedg && coedg != NULL);
      }
    }
  }
  
/*  
  api_get_edges((ENTITY*)body, edge_list, PAT_CAN_CREATE,NULL);

  ENTITY* my_list_item;
  edge_list.init();
  while ((my_list_item=edge_list.next()) != NULL)
  {
 	  if (my_list_item != NULL)
    {

    }
 	}
*/
  std::cout << "SoIntersectKit getallEdges: counter:" << counter << std::endl;
  return newedge;
}

void SoIntersectKit::Object_Object_int(SbName Obj1Name, SbName Obj2Name)
{
  std::cout << "OBJECT_OBJECT_INT" << std::endl;
  Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(Obj1Name);
  Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
  if(obj1 && obj2 && obj1->s_return.ok() && obj2->s_return.ok())
  {
    BODY *shape1, *shape2;
    SbName transName = "";
    
    if ((obj1->name.getValue() == "Surface") && (((SoSurfaceKit*)obj1)->SurfaceOp.getValue() != SURFACE_SWEEP)) 
      shape1 = ((SoSurfaceKit*)obj1)->getBody();
    else
    {
      transName = Obj1Name;
      shape1 = obj1->a_Shape;
    }
    if ((obj2->name.getValue() == "Surface") && (((SoSurfaceKit*)obj2)->SurfaceOp.getValue() != SURFACE_SWEEP)) 
      shape2 = ((SoSurfaceKit*)obj2)->getBody();
    else
    {
      if (transName == "")
        transName = Obj2Name;
      shape2 = obj2->a_Shape;
    }
    
    SPAposition p1 = SPAposition(0,0,0); 
    SPAposition p2 = SPAposition(0,0,0);
    BODY *shape1Offset, *shape2Offset;
   

    api_copy_body(shape1, shape1Offset, NULL);
    api_copy_body(shape2, shape2Offset, NULL);
    api_offset_body(shape1Offset, 0.001, p1, p2, NULL);
    api_offset_body(shape2Offset, 0.001, p1, p2, NULL);
    s_return = api_slice(shape1Offset, shape2Offset, *(SPAunit_vector*)NULL, a_Shape_todraw);
    s_return = api_slice_complete(shape1Offset, shape2Offset, *(SPAunit_vector*)NULL, a_Shape_todraw, NULL);
    s_return = api_clean_wire(a_Shape_todraw, NULL);

    s_return = api_slice(shape1, shape2, *(SPAunit_vector*)NULL, a_Shape);
    s_return = api_slice_complete(shape1, shape2, *(SPAunit_vector*)NULL, a_Shape, NULL);
    s_return = api_clean_wire(a_Shape, NULL);

    if (!s_return.ok()) 
    {
      std::cout << "Error: intersect objects" << std::endl;
      a_Shape = NULL;
    }
    else
    {
      if ((numofObjectNames.getValue() == 0) && (objSum != NULL)) //objSum != NULL bei verteilung
      {
        std::cout << "add new curve" << std::endl;
        
        SoCurveKit *C = new SoCurveKit((objSum->getValue() + 1) , a_Shape, a_Shape_todraw, transName, this, thisobjectName, userID.getValue());
        C->translationOn.setValue(FALSE);
        C->rotationOn.setValue(FALSE);
        C->isLocked.setValue(TRUE);
//        C->layer.setValue(intersectionObjectLayer);
        selectionRoot->addChild(C);
        objSum->setValue(objSum->getValue()+1);
        numofObjectNames.setValue(numofObjectNames.getValue()+1);
        objectNames.setNum(numofObjectNames.getValue());
        objectNames.set1(numofObjectNames.getValue()-1,C->getName().getString());
		// Connecting Curve to IntersectionKit - all callbacks are set to it
		// IF we set this connection in the CurveKit, it does not work with the distribution
		// because under distribution the CurveKit arrives first at the slave and the connection
		// to the - at this moment - nonexisting IntersectionKit cannot be set up
		//this->connectChild(C);
      }
	  else
		  printf("OBJECT_OBJECT_INT numofObjectNames %d\n", numofObjectNames.getValue());
    }
    if (!s_return.ok()) std::cout << "Error can't create intersection" <<std::endl;
  }
  else
	  printf("OBJECT_OBJECT_INT: Nothing done, no valid objects\n");
}

void SoIntersectKit::EGDE_FACE_int(EDGE* Objectedge, FACE* Objectface)
{
  std::cout<< "intersect line with face" << std::endl;
  
  SoMFName objectNames_of_todeletingPoint;
  int numof_todeletingPoint = 0;
  
  ENTITY_LIST *intPs = new ENTITY_LIST;
  intPs->init();
   
  //FILE *output = fopen("a_intersect_object.txt","w");
  //debug_entity(Objectface,output);
  //fclose(output);
  
  int num = 0;
  while (Objectface)
  {
    s_return = api_edfa_int(Objectedge, Objectface, intPs);
    printf("Edge Face int s_return %d \n",s_return.error_number());
    num = num + intPs->count();
    //num = intPs->iteration_count();
    Objectface = Objectface->next();
  }
//printf("EGDE_FACE_int number of intersection points original: %d \n",num);
  std::vector<SPAposition> coords(100);
  int numofPoints = 0;
  int i;
  for (i = intPs->count()-1; i>=0; i--)
  {
    if ((*intPs)[i]->identity(VERTEX_LEVEL)==VERTEX_TYPE)
    {
      APOINT *point;
      ENTITY *pe = (*intPs)[i];
      point = ((VERTEX*)pe)->geometry();
      
      //coords->set1Value(a, point->coords().x(),point->coords().y(),point->coords().z());
      coords[numofPoints] = point->coords();
      numofPoints++;       
    }
  }

  SoPointKit *nearestObject;
  int nearestCoord;
  
  for (i=0; i < numofObjectNames.getValue(); i++)
  {
    nearestObject = NULL; 
    SoPointKit *pointObj = (SoPointKit*)SoNode::getByName(objectNames[i]);
	// important to check for NULL pointer because of distribution
	// when distributing the whole scene at once it happens that pointObj is NULL
	if (pointObj != NULL)  
	{
		double dis=maxPointDistance;
		for(int l = 0; l < numofPoints; l++)
		{
			if (dis > distance_to_point(coords[l], pointObj->a_Point))
			{
				dis = distance_to_point(coords[l], pointObj->a_Point);
				nearestObject = pointObj;
				nearestCoord = l;
			}  
		}

		if (nearestObject)
		{
			nearestObject->Update(convertPosition(coords[nearestCoord]).getValue());
			if (pointObj->visible.getValue() == SO_SWITCH_NONE)
			{
				pointObj->setvisible();
			}

			numofPoints--;
			for (int a=nearestCoord; a < numofPoints; a++)
				coords[a] = coords[a+1];
		}
		else 
		{
			objectNames_of_todeletingPoint.setNum(++numof_todeletingPoint);
			objectNames_of_todeletingPoint.set1(numof_todeletingPoint-1,objectNames[i].getString());
		}
	}
  }

  //delete points
  for (i=0; i < numof_todeletingPoint; i++)
  {  
    SoPointKit *pointObj = (SoPointKit*)SoNode::getByName(objectNames_of_todeletingPoint[i]);
    if (pointObj->visible.getValue() == SO_SWITCH_ALL)
      pointObj->setinvisible();
  }    

  //create new points
  if (objSum != NULL) //für verteilung
  {
  for(int l = 0; l < numofPoints; l++)
  {
	printf("setting new intersection point\n");
    SbVec3f coord(convertPosition(coords[l]).getValue());
	thisobjectName = this->getName();
    SoPointKit *P1 = new SoPointKit(objSum->getValue() + 1, coord, thisobjectName, this->userID.getValue());
//	std::cout << "intersection: " << thisobjectName.getString() << std::endl;
    P1->ObjectName = "";
    P1->translationOn.setValue(FALSE);
    P1->rotationOn.setValue(FALSE);
    P1->isLocked.setValue(TRUE);

    // TODO: 
    //   - robert thinks, dependency should be other way. but what's the name of this point?
    //   - anyway, we can make a workaround using the Point's translationOn property.
    //addDependentObjectsName(...P1's name...);
    P1->addDependentObjectsName(this->getName().getString());
//    P1->layer.setValue(intersectionObjectLayer);
    selectionRoot->addChild(P1);
    objSum->setValue(objSum->getValue()+1);
    numofObjectNames.setValue(numofObjectNames.getValue()+1);
    objectNames.setNum(numofObjectNames.getValue());
    objectNames.set1(numofObjectNames.getValue()-1,P1->getName().getString());
  }
  }
  std::cout<< "EGDE_FACE_int number of intersection points: "<< intPs->count() << std::endl;
  ACIS_DELETE(intPs);
}

void SoIntersectKit::EGDE_FACE_int(BODY* Objectbody, FACE* Objectface)
{
  std::cout<< "EGDE_FACE_int intersect curve with object" << std::endl;
  
  SoMFName objectNames_of_todeletingPoint;
  int numof_todeletingPoint = 0;
  
  ENTITY_LIST *intPs = new ENTITY_LIST;
  
  //FILE *output = fopen("a_intersect_objectBody.txt","w");
  //debug_entity(Objectface,output);
  //fclose(output);

  int num = 0;
  int a = 1;
  while (Objectface)
  {
    for (LUMP *lum = Objectbody->lump();lum != NULL; lum = lum->next())
    {
      for (SHELL *shel = lum->shell();shel != NULL; shel = shel->next())
      {
        for (WIRE *wir = shel->wire();wir != NULL; wir= wir->next())
        {
          COEDGE* st_coedg = wir->coedge();
          COEDGE* coedg = st_coedg;
          do
          { 
            api_edfa_int(coedg->edge(), Objectface, intPs);
            num = num + intPs->count();
            coedg = coedg->next();
          }
          while (coedg != st_coedg && coedg != NULL);
        }
      }
    }
    Objectface = Objectface->next();
  }
  
  std::vector<SPAposition> coords(100);
  int numofPoints = 0;
  int i;
  for (i = intPs->count()-1; i>=0; i--)
  {
    if ((*intPs)[i]->identity(VERTEX_LEVEL)==VERTEX_TYPE)
    {
      APOINT *point;
      ENTITY *pe = (*intPs)[i];
      point = ((VERTEX*)pe)->geometry();
      
      //coords->set1Value(a, point->coords().x(),point->coords().y(),point->coords().z());
      coords[numofPoints] = point->coords();
      numofPoints++;       
    }
  }
  
  SoPointKit *nearestObject;
  int nearestCoord;
  
  for (i=0; i < numofObjectNames.getValue(); i++)
  {
    nearestObject = NULL; 
    SoPointKit *pointObj = (SoPointKit*)SoNode::getByName(objectNames[i]);
    
    double dis=maxPointDistance;
    for(int l = 0; l < numofPoints; l++)
    {
      if (dis > distance_to_point(coords[l], pointObj->a_Point))
      {
        dis = distance_to_point(coords[l], pointObj->a_Point);
        nearestObject = pointObj;
        nearestCoord = l;
      }  
    }
    
    if (nearestObject)
    {
      nearestObject->Update(convertPosition(coords[nearestCoord]).getValue());
      if (pointObj->visible.getValue() == SO_SWITCH_NONE)
      {
        pointObj->setvisible();
      }
      
      numofPoints--;
      for (int a=nearestCoord; a < numofPoints; a++)
        coords[a] = coords[a+1];
    }
    else 
    {
      objectNames_of_todeletingPoint.setNum(++numof_todeletingPoint);
      objectNames_of_todeletingPoint.set1(numof_todeletingPoint-1,objectNames[i].getString());
    }
    
  }
  
  for (i=0; i < numof_todeletingPoint; i++)
  {
    SoPointKit *pointObj = (SoPointKit*)SoNode::getByName(objectNames_of_todeletingPoint[i]);
    if (pointObj->visible.getValue() == SO_SWITCH_ALL)
      pointObj->setinvisible();
  }    
  
  if (objSum != NULL)
  {
    for(int l = 0; l < numofPoints; l++)
    {
      SbVec3f coord(convertPosition(coords[l]).getValue());
      SoPointKit *P1 = new SoPointKit(objSum->getValue()+1, coord, this->userID.getValue());
      P1->ObjectName = "";
      P1->translationOn.setValue(FALSE);
      P1->rotationOn.setValue(FALSE);
      P1->isLocked.setValue(TRUE);
      P1->addDependentObjectsName(this->getName().getString());
//      P1->layer.setValue(intersectionObjectLayer);
      selectionRoot->addChild(P1);
      objSum->setValue(objSum->getValue()+1);
      numofObjectNames.setValue(numofObjectNames.getValue()+1);
      objectNames.setNum(numofObjectNames.getValue());
      objectNames.set1(numofObjectNames.getValue()-1,P1->getName().getString());
    }
  }
  ACIS_DELETE(intPs);
}

void SoIntersectKit::PLANE_PLANE_int(PLANE* plane_One, PLANE* plane_Two)
{
  std::cout << "PLANE_PLANE_intersection" << std::endl;
  SPAposition line_pt;
  SPAunit_vector line_dir;
//  printf("Plane 1 root point %f %f %f \n",plane_One->root_point().x(),plane_One->root_point().y(),plane_One->root_point().z());
//  printf("Plane 2 root point %f %f %f \n",plane_Two->root_point().x(),plane_Two->root_point().y(),plane_Two->root_point().z());
  int returnvalue = intersect_plane_plane(plane_One->root_point(), plane_One->normal(), 
											plane_Two->root_point(), plane_Two->normal(), 
											line_pt, line_dir);
  
  if (returnvalue == 1)
  {
//	  printf("Returnvalue ==1, line_pt %f %f %f \n",line_pt.x(),line_pt.y(),line_pt.z());
    // generate a positive s_return value since all methods must set an s_return value
    s_return = outcome(API_SUCCESS,NULL);
    if (numofObjectNames.getValue() == 1)
    {
      //SoLineKit *lineObj = (SoLineKit*)SoNode::getByName(objectNames[numofObjectNames.getValue()-1]);
      //lineObj->Update(line_pt + (line_dir/5), line_pt - (line_dir/5));
      pCoord = convertPosition(line_pt + (line_dir/5));
      qCoord = convertPosition(line_pt - (line_dir/5));
    }
    else
    {
      if (objSum != NULL) //für verteilung
      {
        pCoord = convertPosition(line_pt + (line_dir/5));
        qCoord = convertPosition(line_pt - (line_dir/5));
        SoLineKit *L = new SoLineKit(objSum->getValue()+1, userID.getValue(), this->getName());
        //L->color.setValue(intersectDefaultColor);
        L->Update(line_pt + (line_dir), line_pt - (line_dir));
        L->translationOn.setValue(FALSE);
        L->rotationOn.setValue(FALSE);
        L->isLocked.setValue(TRUE);
        if (L->s_return.ok())
        {
//		  L->addDependentObjectsName(this->getName().getString());
          selectionRoot->addChild(L);
          objSum->setValue(objSum->getValue()+1); 
          numofObjectNames.setValue(numofObjectNames.getValue()+1);
          objectNames.setNum(numofObjectNames.getValue());
          objectNames.set1(numofObjectNames.getValue()-1,L->getName().getString());
        }
      }
    }
  }
  else
  {
    std::cout << "Error: planes are parallel" << std::endl;
    // generate a negative s_return "s_return = FALSE"
    s_return = outcome(-1,NULL);
  }
}

void SoIntersectKit::EDGE_EDGE_int(EDGE* edge_One, EDGE* edge_Two)
{
//  std::cout << "EDGE_EDGE_int" << std::endl;
  
  SoMFName objectNames_of_todeletingPoint;
  int numof_todeletingPoint = 0;
  
  curve_curve_int* intersects=NULL;
  s_return = api_intersect_curves(edge_One, edge_Two, FALSE, intersects, NULL);
  //s_return = api_inter_ed_ed(edge_One, edge_Two, intersects, NULL);
  
  std::vector<SPAposition> coords(100);
  int numofPoints = 0;
  int numofIntersection = 0;
  while (intersects)
  {
    coords[numofPoints] = intersects->int_point;
    numofPoints++;  
    numofIntersection++;
    intersects = intersects->next;
  }
  
  SoPointKit *nearestObject;
  int nearestCoord;
  int i;
  for (i=0; i < numofObjectNames.getValue(); i++)
  {
    nearestObject = NULL; 
    SoPointKit *pointObj = (SoPointKit*)SoNode::getByName(objectNames[i]);
    
    double dis=maxPointDistance;
	double calcDistance=maxPointDistance+1.0; //just to make it bigger
    for(int l = 0; l < numofPoints; l++)
    {
	  if (pointObj)
		calcDistance = distance_to_point(coords[l], pointObj->a_Point);
      if (dis > calcDistance)
      {
        dis = calcDistance;
        nearestObject = pointObj;
        nearestCoord = l;
      }  
    }
    
    if (nearestObject)
    {
      nearestObject->Update(convertPosition(coords[nearestCoord]).getValue());
      if (pointObj->visible.getValue() == SO_SWITCH_NONE)
      {
        pointObj->setvisible();
      }
      
      numofPoints--;
      for (int a=nearestCoord; a < numofPoints; a++)
        coords[a] = coords[a+1];
    }
    else 
    {
      objectNames_of_todeletingPoint.setNum(++numof_todeletingPoint);
      objectNames_of_todeletingPoint.set1(numof_todeletingPoint-1,objectNames[i].getString());
    }
    
  }

  for (i=0; i < numof_todeletingPoint; i++)
  {
    SoPointKit *pointObj = (SoPointKit*)SoNode::getByName(objectNames_of_todeletingPoint[i]);
	if (pointObj)
		if (pointObj->visible.getValue() == SO_SWITCH_ALL)
			pointObj->setinvisible();
  }    
  
  if (objSum!=NULL) //für Verteilung
  {
    for(int l = 0; l < numofPoints; l++)
    {
      
      //    std::cout<< "add point x="<< coords[l].x() << " y=" << coords[l].y() << " z=" << coords[l].z() << std::endl;
      SbVec3f coord = convertPosition(coords[l]).getValue();
      SoPointKit *P1 = new SoPointKit(objSum->getValue()+1, coord, this->userID.getValue());
      P1->ObjectName = "";
      P1->translationOn.setValue(FALSE);
      P1->rotationOn.setValue(FALSE);
      P1->isLocked.setValue(TRUE);
      P1->addDependentObjectsName(this->getName().getString());
//      P1->layer.setValue(intersectionObjectLayer);
      selectionRoot->addChild(P1);
      objSum->setValue(objSum->getValue()+1);
      numofObjectNames.setValue(numofObjectNames.getValue()+1);
      objectNames.setNum(numofObjectNames.getValue());
      objectNames.set1(numofObjectNames.getValue()-1,P1->getName().getString());
    }
    //s_return = outcome();
  }
  
  std::cout<< "EDGE_EDGE_int number of intersection points: "<< numofIntersection << std::endl;
  ACIS_DELETE(intersects);
}

void SoIntersectKit::BODY_PLANE_int(SbName object1Name, SbName object2Name)
{
  std::cout << "BODY_PLANE_int" << std::endl;
  
  Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(object1Name);
  Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(object2Name);
  PLANE* plane;
  BODY* body=NULL;
  SbName transObjName = "";
  if ((obj1->name.getValue() == "Plane"))       
  {
    plane = ((SoPlaneKit*)obj1)->a_Plane;
    if ((obj2->name.getValue() == "Surface")) 
      body = ((SoSurfaceKit*)obj2)->getBody();
    else 
    {
      body = obj2->a_Shape;
      transObjName = obj2->getName();
    }
  }
  else
  {
    plane = ((SoPlaneKit*)obj2)->a_Plane;  
    if ((obj1->name.getValue() == "Surface")) 
      body = ((SoSurfaceKit*)obj1)->getBody();
    else 
    {
      body = obj1->a_Shape;
      transObjName = obj1->getName();
    }
  }
  
  BODY* shapeOffset;
  SPAposition p1 = SPAposition(0,0,0); 
  SPAposition p2 = SPAposition(0,0,0); 
  api_copy_body(body, shapeOffset, NULL);
  api_offset_body(shapeOffset, 0.001, p1, p2, NULL);
  s_return = api_planar_slice(shapeOffset, plane->root_point() + plane->normal()*0.001, plane->normal(), a_Shape_todraw, NULL);
  
  s_return = api_planar_slice(body, plane->root_point(), plane->normal(), a_Shape, NULL);
  
  if (!s_return.ok()) std::cout << "Error: can't intersect plane with object" << std::endl;
  
  if (!s_return.ok())
  {
    std::cout << "Error: no intersect objects" << std::endl;
    a_Shape=NULL;
  }
  else
  {
    if ((numofObjectNames.getValue()) == 0 && (objSum != NULL))
    {
      std::cout << "add new curve" << std::endl;

      //      std::cout << "intersectionkit transform: obj "<< Obj1Name.getString() <<std::endl;
      SoCurveKit *C = new SoCurveKit(objSum->getValue()+1, a_Shape, a_Shape_todraw, transObjName, this, thisobjectName, userID.getValue());
      C->translationOn.setValue(FALSE);
      C->rotationOn.setValue(FALSE);
      C->isLocked.setValue(TRUE);
//      C->layer.setValue(intersectionObjectLayer);
      selectionRoot->addChild(C);
      objSum->setValue(objSum->getValue()+1);
      numofObjectNames.setValue(numofObjectNames.getValue()+1);
      objectNames.setNum(numofObjectNames.getValue());
      objectNames.set1(numofObjectNames.getValue()-1,C->getName().getString());
    }
  }
  
  if (!s_return.ok()) std::cout << "Error can't create intersection" <<std::endl;
}

// For the slicing we slice with a circle and not a plane (example taken from ACIS Book)
// therefore we have to calculate a radius for this cirlce
// We take the root point of the plane and calculate a 2nd point in the plane
// by intersecting the plane with an arbitrary line
// If this intersection point does not exist (e.g. line and plane parallel) then we
// take a different line and intersect with this.
void SoIntersectKit::SlicingBODY(SbName Obj1Name, SbName Obj2Name)
{
  std::cout << "SlicingBODY" << std::endl;
  
  std::vector<int> objectList(10);
  Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(Obj1Name);
  Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
  PLANE* plane;
  BODY* body=NULL;
  SbName objName;
  SbName transObjName = "";
  if ((obj1->name.getValue() == "Plane"))       
  {
    plane = ((SoPlaneKit*)obj1)->a_Plane;
    if ((obj2->name.getValue() == "Surface")) 
      a_return = api_copy_body(((SoSurfaceKit*)obj2)->getBody(), body);
    else 
    {
      a_return = api_copy_body(obj2->a_Shape, body);
      objName = obj2->getName();
      transObjName = obj2->getName();
    }
  }
  else
  {
    plane = ((SoPlaneKit*)obj2)->a_Plane;  
    if ((obj1->name.getValue() == "Surface")) 
      a_return = api_copy_body(((SoSurfaceKit*)obj1)->getBody(), body);
    else 
    {
      a_return = api_copy_body(obj1->a_Shape, body);
      objName = obj1->getName();
      transObjName = obj1->getName();
    }
  }
  
  // For the slicing we slice with a circle and not a plane (example taken from ACIS Book)
  // therefore we have to calculate a radius for this cirlce
  // We take the root point of the plane and calculate a 2nd point in the plane
  // by intersecting the plane with an arbitrary line
  // If this intersection point does not exist (e.g. line and plane parallel) then we
  // take a different line and intersect with this.

  SPAposition pointonPlane;
  double line_param = 0.0;

  int param = intersect_line_plane(SPAposition(0,0,0),normalise(SPAposition(-100,-100,-100) - SPAposition(100,100,100)), plane->root_point(), plane->normal(), line_param, pointonPlane);
  
  // if plane is parallel to line. calc point on plane with other line
  if (same_point(pointonPlane, SPAposition(0,0,0), 1^-20))
    int param = intersect_line_plane(SPAposition(0,0,0),normalise(SPAposition(0,0,0) - SPAposition(100,0,100)), plane->root_point(), plane->normal(), line_param, pointonPlane);

  SPAvector radius = normalise(pointonPlane - plane->root_point()) * 100;
  
  EDGE* slicer_e;
  s_return = api_mk_ed_ellipse(plane->root_point(), plane->normal(), radius, 1, 0, 0, slicer_e);

  //std::cout << "plane norm: " << plane->normal().x() << " , " << plane->normal().y() << " , " << plane->normal().z() << std::endl;
  //std::cout << "plane point on: " << pointonPlane.x() << " , " << pointonPlane.y() << " , " << pointonPlane.z() << std::endl;
  //std::cout << "plane root: " << plane->root_point().x() << " , " << plane->root_point().y() << " , " << plane->root_point().z() << std::endl;
  //std::cout << "radius: " << radius.x() << " , " << radius.y() << " , " << radius.z() << std::endl;

  EDGE* e_array[1];
  e_array[0] = slicer_e;
  BODY* slicer_wb;
  s_return = api_make_ewire(1, e_array, slicer_wb);

  ENTITY_LIST slicer_faces;
  s_return = api_cover_wires(slicer_wb, *(surface*)NULL, slicer_faces);

  if (!s_return.ok())
  {
    std::cout << "Error: can't slice object" << std::endl;
    a_Shape=NULL;
  }
  else
  {
    if ((numofObjectNames.getValue()) == 0 && (objSum != NULL)) //first time create objects
    {
      objectList[0] = objSum->getValue(); 
      a_Shape = body;
      //first part of slice
      sliceResultOne = slicer_wb;

      SoBoolKit *C = new SoBoolKit(objSum->getValue()+1, thisobjectName, slicer_wb, body, 1, objectList, userID.getValue());
      C->translationOn.setValue(FALSE);
      C->rotationOn.setValue(FALSE);
      C->isLocked.setValue(TRUE);
      selectionRoot->addChild(C);
      objSum->setValue(objSum->getValue()+1);
      numofObjectNames.setValue(numofObjectNames.getValue()+1);
      objectNames.setNum(numofObjectNames.getValue());
      objectNames.set1(numofObjectNames.getValue()-1,C->getName().getString());

      //second part of slice
      //reverse face
      FACE* sf =  (FACE*)slicer_faces[0];
      api_reverse_face(sf);
      sliceResultTwo = slicer_wb;

      C = new SoBoolKit(objSum->getValue()+1, thisobjectName, slicer_wb, body, 2, objectList, userID.getValue());
      C->translationOn.setValue(FALSE);
      C->rotationOn.setValue(FALSE);
      C->isLocked.setValue(TRUE);
      selectionRoot->addChild(C);
      objSum->setValue(objSum->getValue()+1);
      numofObjectNames.setValue(numofObjectNames.getValue()+1);
      objectNames.setNum(numofObjectNames.getValue());
      objectNames.set1(numofObjectNames.getValue()-1,C->getName().getString());
    }
    else //for update
    {
      a_Shape = body;
      
      a_return = api_copy_body(slicer_wb, sliceResultOne); 
      //reverse face
      FACE* sf =  (FACE*)slicer_faces[0];
      api_reverse_face(sf);
      a_return = api_copy_body(slicer_wb, sliceResultTwo); 
      std::cout << "update intersect objects" << std::endl;
    }
  }
}

void SoIntersectKit::get_edges (ENTITY_LIST& edge_list, BODY *bod)
{
  FACE* ff = bod->lump()->shell()->face_list();
  while (ff!=NULL)
  {
    for(LOOP *lp = ff->loop(); lp !=NULL; lp = lp->next())
      get_edges(edge_list,lp);
    
    ff = ff->next_in_list();
  }
}

void SoIntersectKit::get_edges(ENTITY_LIST& edge_list, LOOP *lp)
{
  COEDGE* cc = lp->start();
  do
  {
    if(cc->sense() == FORWARD)
      edge_list.add(cc->edge());
    cc = cc->next();
  } 
  while (cc !=lp->start());
}

SbName SoIntersectKit::Get_ObjectName(char* name, int number)
{
  std::string s1, s2, s3;
  char string[25];
  sprintf(string, "%d", number);
  //itoa(number, string, 10);
  s1 = name; s2 = string;
  s3 = s1 + s2;
  SbName objName = s3.c_str();
  return objName;
}

void SoIntersectKit::Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum)
{
  // I have no idea why but "this" gets destroyed after removeChild of 
  // intersection points/elements
  int childnum = childNumber.getValue();
  // RemoveallCallbacks must be done before removing the other objects
  RemoveAllCallbacks();    
printf("number of intersection points %d \n",numofObjectNames.getValue());
  //this->ref();
  for (int i=numofObjectNames.getValue()-1; i >= 0 ; i--)
  {
    if (objectNames[i] != "")
    {
      printf("Removing intersection point %d \n",i);
      Object3dKit *object = (Object3dKit*)SoNode::getByName(objectNames[i]);
      object->Undo(SelectionRoot, objSum);
      //object->Undo(SelectionRoot, ObjSum);
    }
  }

printf("Undo IntersectKit, childnumber %d \n",childnum);  

  //SoSeparator *looknode = (SoSeparator*) SelectionRoot->getChild(childnum);
  //SelectionRoot->removeChild(looknode); // removes whole seperator node
  SelectionRoot->removeChild(this); // removes whole Kit
  ObjSum->setValue(ObjSum->getValue()-1); 
}

void SoIntersectKit::selectAllDependentObjects(SoSelection* selectionRoot)
{
  if (!selectionRoot->isSelected(this))
    this->select(selectionRoot);
  for (int i=numofObjectNames.getValue()-1; i >= 0 ; i--)
  {
    if (objectNames[i] != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(objectNames[i]);
      obj->selectAllDependentObjects(selectionRoot);
    }
  }
}

void SoIntersectKit::deselectAllDependentObjects(SoSelection* selectionRoot)
{
  if (selectionRoot->isSelected(this))
    this->deselect(selectionRoot);  
  for (int i=numofObjectNames.getValue()-1; i >= 0 ; i--)
  {
    if (objectNames[i] != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(objectNames[i]);
      obj->deselectAllDependentObjects(selectionRoot);
    }
  }
}

int SoIntersectKit::areAllDependentObjectsSelected(SoSelection* selectionRoot)
{
  for (int i=numofObjectNames.getValue()-1; i >= 0 ; i--)
  {
    if (dependentObjectsName[i] != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(dependentObjectsName[i]);
      if (!obj->areAllDependentObjectsSelected(selectionRoot))
        return 0;
    }
  }
  return 1;
}

SbName SoIntersectKit::getinvolvedObjectName(SbName ObjectType)
{
  if ((getObject(Object1Name.getValue())->name.getValue().getString() == ObjectType.getString()) && !(getObject(Object2Name.getValue())->name.getValue().getString() == ObjectType.getString()))
    return Object1Name.getValue();
  else if ((getObject(Object2Name.getValue())->name.getValue().getString() == ObjectType.getString()) && !(getObject(Object1Name.getValue())->name.getValue().getString() == ObjectType.getString()))
    return Object2Name.getValue();
  
  return "";
}
