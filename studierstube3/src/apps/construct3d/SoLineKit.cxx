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

#include "SoLineKit.h"
#include "SoCurveKit.h"
#include "entwray.hxx"

#include "MaterialConstants.h"

SO_KIT_SOURCE(SoLineKit);

void SoLineKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoLineKit, Object3dKit, "Object3dKit");
} 

SoLineKit::SoLineKit() 
{ 
  initSoLineKit();
  CreateSensors();
  setUpConnections(TRUE, TRUE);
}

SoLineKit::SoLineKit(int number, int userID) 
{ 
  initSoLineKit();
  childNumber.setValue(number);
  lineThickness.setValue(3);
  setNodeName("L", number);
  this->userID.setValue(userID);
  
  CreateSensors();
  setUpConnections(TRUE, TRUE);  
}
/*
SoLineKit::SoLineKit(int number, std::vector<int> objectList, int userID) 
{ 
  s_return = outcome(-1,NULL); 
  
  if (checkObjects(objectList) != -1)
  {
    s_return = outcome(); 
    initSoLineKit();
    childNumber.setValue(number);
    lineThickness.setValue(3);
    setNodeName("L", number);
    this->userID.setValue(userID);
    
    setUpConnections(TRUE, TRUE);  
  }
  CreateSensors(); 
}
*/
SoLineKit::SoLineKit(int number, int userID, SbName intersectObjectName) 
{ 
  initSoLineKit();
  
  childNumber.setValue(number);
  lineThickness.setValue(3);
  setNodeName("L", number);
  this->userID.setValue(userID);
  MaterialConstants::defineLineMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);
  
  this->intersectObjectName.setValue(intersectObjectName);
  this->objSum.setValue(number);
  
  SoConcatenate* conc = new SoConcatenate(SoMFVec3f::getClassTypeId());
  conc->input[0]->connectFrom(&(pCoord));
  conc->input[1]->connectFrom(&(qCoord));
  
  SoCoordinate3* coordinate = (SoCoordinate3*) getPart("coord", false);
  coordinate->point.connectFrom(conc->output);
  
//  Build();
  CreateSensors();
  setUpConnections(TRUE, TRUE);
}

void
SoLineKit::CreateSensors()
{
  objectOneSensor = new SoFieldSensor(nameUpdate, this);
  objectOneSensor->setPriority(0);
  //objectOneSensor->attach(&p);
  
  objectTwoSensor = new SoFieldSensor(nameUpdate, this);
  objectTwoSensor->setPriority(0);
  //objectTwoSensor->attach(&q);
  objectThreeSensor = new SoFieldSensor(nameUpdate, this);
  objectThreeSensor->setPriority(0);
  objectFourSensor = new SoFieldSensor(nameUpdate, this);
  objectFourSensor->setPriority(0);

  if(p.getValue() != "")
  {
    SbName Obj1Name = p.getValue();
	Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(Obj1Name);
    if(obj1)
      obj1->connectChild(this);
  }  
  if(q.getValue() != "")
  {
    SbName Obj2Name = q.getValue();
	Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
    if(obj2)
      obj2->connectChild(this);
  }    
  if (TangentPointOne.getValue() != "")
  {
    objectThreeSensor->attach(&TangentPointOne);
    
    SoPointKit *obj = (SoPointKit*)SoNode::getByName(TangentPointOne.getValue());
    if(obj)
      obj->connectChild(this);
  }
  
  if (TangentPointTwo.getValue() != "")
  {
    objectFourSensor->attach(&TangentPointTwo);
    
    SoPointKit *obj = (SoPointKit*)SoNode::getByName(TangentPointTwo.getValue());
    if(obj)
      obj->connectChild(this);
  }
  
  if (intersectObjectName.getValue() != "")
  { 
    objectThreeSensor->attach(&intersectObjectName);
    
    SoIntersectKit *obj = (SoIntersectKit*)SoNode::getByName(intersectObjectName.getValue());
    if(obj)
	{
		printf("Line connected to intersect object \n");
      obj->connectChild(this);
	}
  }

  if (intersectionPointName.getValue() != "")
  { 
    objectThreeSensor->attach(&intersectionPointName);
    
    SoPointKit *obj = (SoPointKit*)SoNode::getByName(intersectionPointName.getValue());
    if(obj)
      obj->connectChild(this);
  }
}

SoLineKit::SoLineKit(int number, std::vector<int> objectList, int userID, 
					 SoSFInt32* objSum, SbBool angleBisector)
{
  initSoLineKit();
  
  s_return = outcome(-1,NULL); 
  
  if (checkObjects(objectList) != -1)
  {
    lineThickness.setValue(3);
    setNodeName("L", number + checkObjects(objectList));
    childNumber.setValue(number + checkObjects(objectList));
    
    this->userID.setValue(userID);
    MaterialConstants::defineLineMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
    SoMaterial* mat = (SoMaterial*) getPart("material", false);
    mat->copyFieldValues(W);
    this->objSum.setValue(objSum->getValue());
    
	// multiples of 2 points could be selected to generate multiple lines
	if (!angleBisector)
		if (objectList.size() >= 2)
		{
			p.setValue(getObject(objectList[0])->getName());
			q.setValue(getObject(objectList[1])->getName());

			addDependentObjects(objectList, 2);
			deselectObjects(objectList,2);
			createObjectList(objectList, 2);

			SoConcatenate* conc = new SoConcatenate(SoMFVec3f::getClassTypeId());
			conc->input[0]->connectFrom(&(pCoord));
			conc->input[1]->connectFrom(&(qCoord));

			SoCoordinate3* coordinate = (SoCoordinate3*) getPart("coord", false);
			coordinate->point.connectFrom(conc->output);
			
			// do not Build() before sensors are created !!! 
//			Build(); 
			CreateSensors();
			setUpConnections(TRUE, TRUE);
		}

	// 3 points are selected for generating an angle bisector
	if (angleBisector)
		if (objectList.size() >= 3) 
		{
			p.setValue(getObject(objectList[0])->getName());
			q.setValue(getObject(objectList[2])->getName());
			intersectionPointName.setValue(getObject(objectList[1])->getName());

			addDependentObjects(objectList, 3);
			deselectObjects(objectList,3);
			createObjectList(objectList, 3);

			SoConcatenate* conc = new SoConcatenate(SoMFVec3f::getClassTypeId());
			conc->input[0]->connectFrom(&(pCoord));
			conc->input[1]->connectFrom(&(qCoord));

			SoCoordinate3* coordinate = (SoCoordinate3*) getPart("coord", false);
			coordinate->point.connectFrom(conc->output);

			// do not Build() before sensors are created !!! 
//			Build(); 
			CreateSensors();
			setUpConnections(TRUE, TRUE);
		} 
  }
}
/*
void SoLineKit::SetLineKit(std::vector<int> objectList, SoSFInt32* objSum)
{
  //initSoLineKit();
  
  s_return = outcome(-1,NULL); 
  
  if (checkObjects(objectList) != -1)
  {    
    SoLineSet* line = (SoLineSet*) getPart("object", false);
    line->numVertices.setValue(2);
    line->startIndex.setValue(0);
    
    MaterialConstants::defineLineMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
    SoMaterial* mat = (SoMaterial*) getPart("material", false);
    mat->copyFieldValues(W);
    this->objSum.setValue(objSum->getValue());
    
    if (objectList.size() >= 2)
    {
      p.setValue(getObject(objectList[0])->getName());
      q.setValue(getObject(objectList[1])->getName());

      addDependentObjects(objectList, 2);
      deselectObjects(objectList,2);
      createObjectList(objectList, 2);
    }
    
    CreateSensors();
    setUpConnections(TRUE, TRUE);
  }
}
*/

// check the objects and return the number of tangentpoints
// if point curve tangent one point must be added => return 1
// if curve curve tangent two point must be added => return 2
int SoLineKit::checkObjects(std::vector<int> objectList)
{
  int number = -1;
  
  if (objectList.size() >= 2)
  {
    if ((getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )    
      number = 0;
    else if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoPlaneKit::getClassTypeId())) )    
      number = 0;
    else if ( (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[0])->isOfType(SoPlaneKit::getClassTypeId())) )    
      number = 0;
    else if ( (getObject(objectList[0])->isOfType(SoCurveKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )    
      number = 1;
    else if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoCurveKit::getClassTypeId())) )    
      number = 1;
    else if ( (getObject(objectList[0])->isOfType(SoCurveKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoCurveKit::getClassTypeId())) )    
      number = 0;
    else if ((isObject(objectList[0])) 
      && (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )    
      number = 0;
    else if ((isObject(objectList[1])) 
      && (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) )    
      number = 0;
    else if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) )    
      number = 0;
    else if ( (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) )    
      number = 0;
  }
  else if (objectList.size() >= 3)
  {
    if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[2])->isOfType(SoPointKit::getClassTypeId())))    
      number = 0;
  }
  
  return number;
}

// init all parts and fields
void
SoLineKit::initSoLineKit()
{
  SO_KIT_CONSTRUCTOR(SoLineKit); 
  // Add the catalog parts:
  //-----------------------name--|---type------|nuldef|  par   |rite|public
  SO_KIT_ADD_CATALOG_ENTRY(coord,SoCoordinate3,FALSE,switchNode,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(object,SoLineSet,   FALSE,switchNode,\x0,TRUE);
  
  //fields
  SO_NODE_ADD_FIELD(lineThickness, (3));
  SO_NODE_ADD_FIELD(normal, (FALSE));
  SO_NODE_ADD_FIELD(pCoord, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(qCoord, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(p, (""));
  SO_NODE_ADD_FIELD(q, (""));
  SO_NODE_ADD_FIELD(intersectionPointName, (""));
  SO_NODE_ADD_FIELD(TangentPointOne, (""));
  SO_NODE_ADD_FIELD(TangentPointTwo, (""));
  SO_NODE_ADD_FIELD(intersectObjectName, (""));
  
  // Initialize this SoLineKit
  SO_KIT_INIT_INSTANCE();
  
  //turn dragging temporarily off
  translationOn.setValue(FALSE);
  rotationOn.setValue(FALSE);
  
  name.setValue("Line");
  
  normal.setValue(FALSE);
  // connect fields
  
  //SoLineSet* line = (SoLineSet*) getPart("object", false);
  //line->numVertices.setValue(2);
  //line->startIndex.setValue(0);
  
  objectOneSensor = NULL;
  objectTwoSensor = NULL;
  objectThreeSensor = NULL;
  objectFourSensor = NULL;
  
  triggerAllowed = FALSE;
}

// Destructor
SoLineKit::~SoLineKit()
{
  pCoord.disconnect();
  qCoord.disconnect();
  
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
  RemoveAllCallbacks();
}

float
SoLineKit::getDistance(SbVec3f penPosition)
{
  double min_distance = 1000;
  if (s_return.ok())
  {
    SPAposition pen_point;
    pen_point.set_x(penPosition.getValue()[0]);
    pen_point.set_y(penPosition.getValue()[1]);
    pen_point.set_z(penPosition.getValue()[2]);
    
    SPAposition Point;
    param_info& ent_info=*(param_info*)NULL_REF;
    
    api_entity_point_distance(a_Line, pen_point, Point, min_distance, ent_info);
  }
  
  return(float(min_distance-IntersectCurvePriorityEpsilon));
}


// input types:
// line:
//  2 points
// line normal to plane:
//  1 point (on line), 1 plane
void
SoLineKit::nameUpdate(void *data, SoSensor *sensor)
{
  SoLineKit *self=(SoLineKit*)data;
  self->triggerAllowed = TRUE;
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
    self->pCoord.connectFrom(&(((SoPointKit*)objectOne)->pCoord));
    if(objectTwo->isOfType(SoPointKit::getClassTypeId()))
    self->qCoord.connectFrom(&(((SoPointKit*)objectTwo)->pCoord));
    }
    
      if(self->intersectObjectName.getValue() != "")
      {
      SoNode* obj = SoNode::getByName(self->intersectObjectName.getValue());
      self->pCoord.connectFrom(&(((SoIntersectKit*)obj)->pCoord));
      self->qCoord.connectFrom(&(((SoIntersectKit*)obj)->qCoord));
      }
  */
  self->triggerChange();
}

void SoLineKit::Build()
{
  if ((p.getValue() != "") && (q.getValue() != "") && (intersectionPointName.getValue() != ""))
  {
    //angle bisector
    if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId()))
      && (getObject(objectList[2])->isOfType(SoPointKit::getClassTypeId())))  
    {
      calculateAngleBisector();
    }
  }
  else if ((p.getValue() != "") && (q.getValue() != ""))
  {
    SbName Obj1Name = p.getValue();
    SbName Obj2Name = q.getValue();

    //line between to points
    if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )   
    {
      SoPointKit *Point1 = (SoPointKit*)SoNode::getByName(Obj1Name);
      SoPointKit *Point2 = (SoPointKit*)SoNode::getByName(Obj2Name);
      pCoord.connectFrom(&(Point1->pCoord));
      qCoord.connectFrom(&(Point2->pCoord));
      position_One = Point1->a_Point;
      position_Two = Point2->a_Point;
      
      s_return = api_mk_ed_line(position_One, position_Two, a_Line);
    }
    
    //tangent min. one object is a curve
    else if ( (getObject(objectList[0])->isOfType(SoCurveKit::getClassTypeId())) 
      || (getObject(objectList[1])->isOfType(SoCurveKit::getClassTypeId())) )   
    {
      if (PointsinaPlane(objectList[0], objectList[1]))
	  {
		  printf("Points are in a plane, calc tangent\n");
        calculateTangent(); 
	  }
      else 
      {
        //delete line from screen
        pCoord.setValue(0,0,0);
        qCoord.setValue(0,0,0);
        std::cout << "SoLineKit: objects not in a Plane" << std::endl;
      }
    }
    
    //line normal to a plane
    else if ( (getObject(objectList[0])->isOfType(SoPlaneKit::getClassTypeId())) 
      || (getObject(objectList[1])->isOfType(SoPlaneKit::getClassTypeId())) )   
    {
      calculateNormalLine(); 
    }
    
    //line normal to an object
    else if ((isObject(objectList[0])) || (isObject(objectList[1])))  
    {
      calculateNormaltoObject(); 
    }
    
    //parallel line
    else if ( ((getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) 
      && (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId()))) || 
      ((getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) 
      && (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId()))) )  
    {
      calculateParallelLine();
    }
  } 
  else if (intersectObjectName.getValue() != "") //line is result of plane plane intersection
  { 
    SoIntersectKit *obj = (SoIntersectKit*)SoNode::getByName(intersectObjectName.getValue());
    printf("intersectkit name im linekit %s \n",intersectObjectName.getValue().getString());    
    pCoord.setValue(obj->pCoord.getValue()[0], obj->pCoord.getValue()[1], obj->pCoord.getValue()[2]);
    qCoord.setValue(obj->qCoord.getValue()[0], obj->qCoord.getValue()[1], obj->qCoord.getValue()[2]);
    
	printf("Linekit INTERSECTION pcoord %f %f %f \n",pCoord.getValue()[0],pCoord.getValue()[1],pCoord.getValue()[2]);
	printf("Linekit INTERSECTION qcoord %f %f %f \n",qCoord.getValue()[0],qCoord.getValue()[1],qCoord.getValue()[2]);
    s_return = api_mk_ed_line(convertSoSFVec3f(obj->pCoord.getValue()), convertSoSFVec3f(obj->qCoord.getValue()), a_Line);
    if (!s_return.ok()) std::cout << "Error can't create Acis Line";
  }
  else
  {
    pCoord = convertPosition(position_One);
    qCoord = convertPosition(position_Two);
    
    s_return = api_mk_ed_line(position_One, position_Two, a_Line);
  }
  
  if (!s_return.ok()) 
  {  
    std::cout << "Error can't create Acis Line" << std::endl;
    this->visible.setValue(SO_SWITCH_NONE);
  }
  else
    this->visible.setValue(SO_SWITCH_ALL);
}

//calulates a line through a point parallel to a other line
void SoLineKit::calculateParallelLine()
{
  std::cout << "SoLineKit: creat a parallel line" << std::endl;
  SPAposition point;
  EDGE* line;
  if (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId()))
  {
    line = ((SoLineKit*)getObject(objectList[0]))->a_Line;
    point = ((SoPointKit*)getObject(objectList[1]))->a_Point;
  }
  else
  {
    line = ((SoLineKit*)getObject(objectList[1]))->a_Line;
    point = ((SoPointKit*)getObject(objectList[0]))->a_Point;
  }
  
  SPAposition pos1 = point - normalise(line->start_pos() - line->end_pos()) * 0.2;
  SPAposition pos2 = point - normalise(line->end_pos() - line->start_pos()) * 0.2;
  
  s_return = api_mk_ed_line(pos1, pos2, a_Line);
  
  if (s_return.ok()) 
  {
    pCoord = convertPosition(pos1);
    qCoord = convertPosition(pos2);
  }
  else
    std::cout << "Error can't create a parallel line";
}

// Calculates tangent from a point to a curve; in a point on a curve; and the common tangent to 2 curves
// Tangent to 2 curves is not very stable and may crash!!! Possible cause: As soon as start and end points
// gets swapped this could cause problems
void SoLineKit::calculateTangent()
{
  ENTITY *entity1=NULL, *entity2=NULL;
  SPAposition* position_1 = NULL; SPAposition* position_2 = NULL;
  entity_with_ray* entity_with_ray_1 = NULL; entity_with_ray* entity_with_ray_2 = NULL;
  bool pointonCurve = false;
  
  //if first object is a curve create entity_with_ray_1 for cal. tangent point on curve
  //intersection curve has no point on curve => can't create entity_with_ray
  //only tangent with point on intersection curve is possible
  if  ((getObject(objectList[0])->isOfType(SoCurveKit::getClassTypeId()))) 
  {
    if (((SoCurveKit*)getObject(objectList[0]))->CurveOp.getValue() != CURVE_INTERSECTION)
    {
      if (TangentPointOne.getValue() != "")  //for update
      {
        SoPointKit *Point = (SoPointKit*)SoNode::getByName(TangentPointOne.getValue());
        entity_with_ray_1 = &entity_with_ray(((SoCurveKit*)getObject(objectList[0]))->getEntity(),Point->a_Point, SPAunit_vector(1,1,1));
      }
      else //first time
      {
        SoPointKit *Point;
		if ( ( (SoCurveKit*)(getObject(objectList[0])) )->pointsName.getNum() > 1)
			Point = (SoPointKit*)SoNode::getByName(((SoCurveKit*)(getObject(objectList[0])))->pointsName[1]); //get one point of curve
		else 
			Point = (SoPointKit*)SoNode::getByName(((SoCurveKit*)(getObject(objectList[0])))->pointsName[0]); //get one point of curve
        printf("Calc tangent here\n");
		entity_with_ray_1 = &entity_with_ray(((SoCurveKit*)getObject(objectList[0]))->getEntity(),Point->a_Point, SPAunit_vector(1,1,1));
      }
    }
    else
    {
      SPAposition pos;
      double dis_point_axis;
      param_info& ent_info=*(param_info*)NULL_REF;
      api_entity_point_distance(((SoCurveKit*)getObject(objectList[0]))->getEntity(), SPAposition(0,0,0), pos, dis_point_axis, ent_info);
            std::cout << "pos1: " << pos.x() << " , " << pos.y() << " , " << pos.z() << std::endl;
      entity_with_ray_1 = &entity_with_ray(((SoCurveKit*)getObject(objectList[0]))->getEntity(),pos, SPAunit_vector(1,1,1));
    }
  }
  
  //if second object is a curve create entity_with_ray_2 for cal. tangent point on curve
  //intersection curve has no point on curve => can't create entity_with_ray
  //only tangent with point on intersection curve is possible
  if  ((getObject(objectList[1])->isOfType(SoCurveKit::getClassTypeId()))) 
  {
    if (((SoCurveKit*)getObject(objectList[1]))->CurveOp.getValue() != CURVE_INTERSECTION)
    {
      if (TangentPointTwo.getValue() != "") // for update
      {
        SoPointKit *Point = (SoPointKit*)SoNode::getByName(TangentPointTwo.getValue()); 
        entity_with_ray_2 = &entity_with_ray(((SoCurveKit*)getObject(objectList[1]))->getEntity(),Point->a_Point, SPAunit_vector(0,0,0));
      }        
      else //first time 
      {
        SoPointKit *Point;
		if ( ( (SoCurveKit*)(getObject(objectList[1])) )->pointsName.getNum() > 1)
			Point = (SoPointKit*)SoNode::getByName(((SoCurveKit*)(getObject(objectList[1])))->pointsName[1]); //get one point of curve
		else 
			Point = (SoPointKit*)SoNode::getByName(((SoCurveKit*)(getObject(objectList[1])))->pointsName[0]); //get one point of curve
        entity_with_ray_2 = &entity_with_ray(((SoCurveKit*)getObject(objectList[1]))->getEntity(),Point->a_Point, SPAunit_vector(0,0,0));
      }
    }
    else
    {
      SPAposition pos;
      double dis_point_axis;
      param_info& ent_info=*(param_info*)NULL_REF;
      api_entity_point_distance(((SoCurveKit*)getObject(objectList[1]))->getEntity(), SPAposition(0,0,0), pos, dis_point_axis, ent_info);
      //      std::cout << "pos2: " << pos.x() << " , " << pos.y() << " , " << pos.z() << std::endl;      
      entity_with_ray_2 = &entity_with_ray(((SoCurveKit*)getObject(objectList[1]))->getEntity(),pos, SPAunit_vector(1,1,1));
    }
  }
  
  //if no entity_with_ray_1 => no first curve => line between position_One and curve2 
  if (entity_with_ray_1 == NULL)
  {
    position_One = ((SoPointKit*)getObject(objectList[0]))->a_Point;
    position_1 = &position_One;    
  }
  //if no entity_with_ray_2 => no second curve => line between position_Two and curve1 
  if (entity_with_ray_2 == NULL)
  {
    position_Two = ((SoPointKit*)getObject(objectList[1]))->a_Point;
    position_2 = &position_Two;
  }
  
  //look if the first point lie on the selected curve
  if ((position_1 != NULL) && (entity_with_ray_2 != NULL))
  {
    if (((SoCurveKit*)getObject(objectList[1]))->isPointonCurve(((SoPointKit*)getObject(objectList[0]))->a_Point))
      pointonCurve = true;
  }
  
  //look if the second point lie on the selected curve
  if ((position_2 != NULL) && (entity_with_ray_1 != NULL))
  {
    if (((SoCurveKit*)getObject(objectList[0]))->isPointonCurve(((SoPointKit*)getObject(objectList[1]))->a_Point))
      pointonCurve = true;
	printf("Point on curve\n");
  }
  
  //calc tangent
  a_Line = NULL;
  if (!pointonCurve)
  {
	  printf("HERE 1\n");
    s_return = api_curve_line_tangent(position_1, entity_with_ray_1, position_2, entity_with_ray_2, a_Line, NULL);
  }
  else
  {
	  printf("HERE 2\n");
    if (entity_with_ray_1)
    {
      SPAunit_vector vec = ((SoCurveKit*)getObject(objectList[0]))->pointDirection(*position_2);
      s_return = api_mk_ed_line(*position_2 + (vec / 5), *position_2 - (vec / 5), a_Line);
	  printf("calc tangent 1\n");
      SoPointKit* tangentPoint = (SoPointKit*)getObject(objectList[0]);
	  printf("calc tangent 1a\n");
	  // adding this sensor caused problems in example Beispiel_4_fertig.iv
      objectThreeSensor->attach(&(tangentPoint->ChangeObject));  
	  printf("calc tangent 1b\n");
    } 
    if (entity_with_ray_2)
    {
      SPAunit_vector vec = ((SoCurveKit*)getObject(objectList[1]))->pointDirection(*position_1);
      s_return = api_mk_ed_line(*position_1 + (vec / 5), *position_1 - (vec / 5), a_Line);
      SoPointKit* tangentPoint = (SoPointKit*)getObject(objectList[1]);
      objectThreeSensor->attach(&(tangentPoint->ChangeObject));  
	  printf("calc tangent 2\n");
    }
  }
  
  //set pCoord and qCoord
  if (a_Line)
  {
    pCoord = convertPosition(a_Line->start_pos());
	qCoord = convertPosition(a_Line->end_pos());
  }
  
  if (s_return.ok())
  {
    //create a tangent point on curve one
    if ((entity_with_ray_1) && (TangentPointOne.getValue() == "") && (!pointonCurve))
    {
      SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
      SoPointKit *P = new SoPointKit(objSum.getValue()+1, pCoord.getValue(), (getObject(objectList[0]))->getName(), this->userID.getValue());
      P->addMovetoRedoUndoList = TRUE;
      P->addDependentObjectsName(this->getName());
      selectionRoot->addChild(P);
      TangentPointOne.setValue(P->getName().getString());
      objSum.setValue(objSum.getValue()+1);
      
      // very important block to make all connections correct between
      // TangentPointOne and pCoord of line
      SoNode* objectThree = SoNode::getByName(TangentPointOne.getValue());
      objectThreeSensor->attach(&(((SoPointKit*)objectThree)->ChangeObject));  
      SoPointKit *obj = (SoPointKit*)SoNode::getByName(TangentPointOne.getValue());
      if(obj)
        obj->connectChild(this);
    }
    else if ((entity_with_ray_1) && (TangentPointOne.getValue() != ""))
    {
      SoPointKit* Point = (SoPointKit*)SoNode::getByName(TangentPointOne.getValue());
      SbVec3f startpoint = convertPosition(a_Line->start_pos()).getValue();
      // check if TangentPointOne is already up to date. If yes, further update
      // would cause infinite loop.
      if (!Point->pCoord.getValue().equals(startpoint,0.00001))
        Point->Update(convertPosition(a_Line->start_pos()).getValue());
    }
    
    //create a tangent point on curve two
    if ((entity_with_ray_2) && (TangentPointTwo.getValue() == "") && (!pointonCurve))
    {
      SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
      SoPointKit *P = new SoPointKit(objSum.getValue()+1, qCoord.getValue(), (getObject(objectList[1]))->getName(), this->userID.getValue());
      P->addMovetoRedoUndoList=true;
      P->addDependentObjectsName(this->getName());
      selectionRoot->addChild(P);
      TangentPointTwo.setValue(P->getName().getString());
      objSum.setValue(objSum.getValue()+1);
      
      // very important paragraph to make all connections correct between
      // TangentPointTwo and qCoord of line
	  objectFourSensor->attach(&P->ChangeObject);
	  P->connectChild(this);
//      SoPointKit* objectFour = (SoPointKit*)SoNode::getByName(TangentPointTwo.getValue());
//      objectFourSensor->attach(&(objectFour)->ChangeObject);  
//      //SoPointKit *obj = (SoPointKit*)SoNode::getByName(TangentPointTwo.getValue());
//      if(obj)
//        obj->connectChild(this);
    }
    else if ((entity_with_ray_2) && (TangentPointTwo.getValue() != ""))
    {
      SoPointKit* Point = (SoPointKit*)SoNode::getByName(TangentPointTwo.getValue());
      SbVec3f endpoint = convertPosition(a_Line->end_pos()).getValue();
      // check if TangentPointTwo is already up to date. If yes, further update
      // would cause infinite loop.
      if (!Point->pCoord.getValue().equals(endpoint,0.00001))
        Point->Update(convertPosition(a_Line->end_pos()).getValue());
    }
  }	  
  if (!a_Line)
    std::cout << "SoLineKit: can't calculate Tangent" << std::endl;
}

void SoLineKit::calculateNormalLine()
{
  SoPointKit *point = NULL;
  SoPlaneKit *plane = NULL;
  
  printf("Calculate normal line called\n");
  //first object in list is a plane
  if ( (getObject(objectList[0])->isOfType(SoPlaneKit::getClassTypeId())) 
    && (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )   
  {
    point = (SoPointKit*)(getObject(objectList[1]));
    plane = (SoPlaneKit*)(getObject(objectList[0]));
  }
  //second object in list is a plane
  if ( (getObject(objectList[1])->isOfType(SoPlaneKit::getClassTypeId())) 
    && (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) )   
  {
    point = (SoPointKit*)(getObject(objectList[0]));
    plane = (SoPlaneKit*)(getObject(objectList[1]));
  }
  
  if (plane && point)
  {
	SPAposition midP = convertSoSFVec3f(midpoint.getValue());
//	printf("Calc normal line Point coord %f %f %f\n",midP.x(),midP.y(),midP.z() );
	SPAunit_vector norm = normalise(plane->a_Plane->normal());
	pCoord = convertPosition(midP + norm * normalLineLength);
	qCoord = convertPosition(midP - norm * normalLineLength);
	s_return = api_mk_ed_line(midP + norm * normalLineLength, midP - norm * normalLineLength, a_Line);
  }
}

void SoLineKit::calculateNormaltoObject()
{
  SoPointKit *point = NULL;
  Object3dKit *object = NULL;
  
  //first object in list is a object
  if ( (isObject(objectList[0])) 
    && (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )   
  {
    point = (SoPointKit*)(getObject(objectList[1]));
    object = (Object3dKit*)(getObject(objectList[0]));
  }
  //second object in list is a plane
  if ( (isObject(objectList[1])) 
    && (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) )   
  {
    point = (SoPointKit*)(getObject(objectList[0]));
    object = (Object3dKit*)(getObject(objectList[1]));
  }
  
  if (object && point)
  {
    point_containment pc;
    BODY* body = object->getBody();
    SPAposition pos = point->a_Point;
    api_point_in_body(pos, body, pc, FALSE, NULL);
    if (pc == point_boundary)
    {
      FACE* objectFace = body->lump()->shell()->face();
      surface* transObjectSurface = NULL;
      SPAtransf trans;
      
      //for SoSurfaceKit, there is now transformation
      if (body->transform())
        trans = body->transform()->transform();
      else 
        trans = SPAtransf();
      
      bool onFace = false;
      int counter = 0;
      
      while (objectFace && !onFace)
      {
        counter++;
        std::cout << "count " << counter << std::endl;
        //transform the face with the translation of the body. to move the face in the right position
        transObjectSurface = objectFace->geometry()->trans_surface(trans);
        //check with face is the face on with the point lie
        if (transObjectSurface->test_point_tol(pos, EPS, *(SPApar_pos*)NULL_REF, *(SPApar_pos*)NULL_REF ))
          onFace = true;        
        else
          objectFace = objectFace->next();
      }
      if (onFace)
      {
        //cal the normal
        SPAunit_vector norm = transObjectSurface->point_normal(pos, *(SPApar_pos*)NULL_REF);
        
        SPAposition position_One;
        position_One = point->a_Point + 0.3 * norm;
        SPAposition position_Two;
        position_Two = point->a_Point - 0.3 * norm;
        
        pCoord = convertPosition(position_One);
        qCoord = convertPosition(position_Two);
        
        s_return = api_mk_ed_line(position_One, position_Two, a_Line);
      }
      else
        std::cout << "can't create line normal to object" << std::endl;
      
    }
    else
    {
		    std::cout << "can't create line normal to object" << std::endl;
        pCoord.setValue(0,0,0);
        qCoord.setValue(0,0,0);
    }
  }
}

void SoLineKit::calculateAngleBisector()
{
  std::cout << "create AngleBisector" << std::endl;
  /*
  EDGE* edgeOne = ((SoLineKit*)getObject(objectList[0]))->a_Line;
  EDGE* edgeTwo = ((SoLineKit*)getObject(objectList[1]))->a_Line;
  curve_curve_int* intersects=NULL;
  
  s_return = api_intersect_curves(edgeOne, edgeTwo, FALSE, intersects, NULL);

  SPAposition intPoint;
  if (intersects)
  {
    intPoint = intersects->int_point;

    SPAvector angelBisector = (normalise(edgeOne->start_pos() - edgeOne->end_pos()) + normalise(edgeTwo->start_pos() - edgeTwo->end_pos())); 
    SPAposition positionOne = (intPoint - angelBisector * 0.05); 
    SPAposition positionTwo = (intPoint + angelBisector * 0.2);
   
    s_return = api_mk_ed_line(positionOne, positionTwo, a_Line);
    if (s_return.ok())
    { 
      this->visible.setValue(SO_SWITCH_ALL);
      pCoord = convertPosition(positionOne);
      qCoord = convertPosition(positionTwo);
		}
		else
    {
		  std::cout << "SoLineKit: Error: can't create angel bisector" << std::endl;
      this->visible.setValue(SO_SWITCH_NONE);
    }
  }
  else 
  {
    std::cout << "SoLineKit: Error: can't creat angle bisector" << std::endl;
    this->visible.setValue(SO_SWITCH_NONE);
  }
  */
  SPAposition posA = ((SoPointKit*)getObject(objectList[0]))->a_Point;
  SPAposition posB = ((SoPointKit*)getObject(objectList[2]))->a_Point;
  SPAposition posI = ((SoPointKit*)getObject(objectList[1]))->a_Point;

  SPAvector angelBisector = (normalise(posA - posI) + normalise(posB - posI)); 
  SPAposition positionOne = (posI - angelBisector * 0.05); 
  SPAposition positionTwo = (posI + angelBisector * 0.2);
 
  s_return = api_mk_ed_line(positionOne, positionTwo, a_Line);
  if (s_return.ok())
  { 
    this->visible.setValue(SO_SWITCH_ALL);
    pCoord = convertPosition(positionOne);
    qCoord = convertPosition(positionTwo);
	}
	else
  {
		std::cout << "SoLineKit: Error: can't create angel bisector" << std::endl;
    this->visible.setValue(SO_SWITCH_NONE);
  }
}

void SoLineKit::OnChange()
{
  if (triggerAllowed)
  {
    std::cout << "SoLineKit OnChange, updated" << std::endl;
    Build();
  }
  triggerAllowed = FALSE;
}

SbBool SoLineKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
  
  if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    Object3dKit::setUpConnections(onOff, doItAlways);
    
    // do your own special setup here !
    // connect fields
    SoDrawStyle* drawstyle = (SoDrawStyle*) getPart("style", false);
    drawstyle->lineWidth.connectFrom(&(lineThickness));
    
    SoConcatenate* conc = new SoConcatenate(SoMFVec3f::getClassTypeId());
    conc->input[0]->connectFrom(&(pCoord));
    conc->input[1]->connectFrom(&(qCoord));
    
    SoCoordinate3* coordinate = (SoCoordinate3*) getPart("coord", false);
    coordinate->point.connectFrom(conc->output);
    
    SbName pName = p.getValue();
    SbName qName = q.getValue();
    SoNode *objectOne = NULL;
    SoNode *objectTwo = NULL;
    SoNode *objectThree = NULL;
    SoNode *objectFour = NULL;
    SbBool validObject = FALSE;
    
    //std::cout<< "line upConnection pName: " << pName.getString() << std::endl;
    if(intersectObjectName.getValue() != "")
    {
      printf("Intersectobjectname in linekit setupconn %s \n",intersectObjectName.getValue().getString());
      objectThree = SoNode::getByName(intersectObjectName.getValue());    
      objectThreeSensor->attach(&(((SoIntersectKit*)objectThree)->ChangeObject));
      pCoord.connectFrom(&(((SoIntersectKit*)objectThree)->pCoord));
      qCoord.connectFrom(&(((SoIntersectKit*)objectThree)->qCoord));
      validObject = TRUE;
    } 
    else
    {
      if(pName != "")
      {
        objectOne = SoNode::getByName(pName);
        if (objectOne->isOfType(SoPointKit::getClassTypeId()))
        {
          objectOneSensor->attach(&(((SoPointKit*)objectOne)->pCoord));
          pCoord.connectFrom(&(((SoPointKit*)objectOne)->pCoord));
		
		  // special case: 2nd object is a Plane, this means our Line is
		  // a line normal to a plane through a given point
		  // we must CONNECT the point (which is the mid point of the normal line)
		  // with our line
		  // The midpoint is only used in calculateNormalLine()
		  // since pName != "", we assume qName != "" as well
		  objectTwo = SoNode::getByName(qName);
		  if (objectTwo->isOfType(SoPlaneKit::getClassTypeId()))
			  midpoint.connectFrom(&(((SoPointKit*)objectOne)->pCoord));
        }
        else
		  objectOneSensor->attach(&(((Object3dKit*)objectOne)->ChangeObject));

        validObject = TRUE;
      }
      if(qName != "")
      {
        objectTwo = SoNode::getByName(qName);
        if (objectTwo->isOfType(SoPointKit::getClassTypeId()))
        {
          objectTwoSensor->attach(&(((SoPointKit*)objectTwo)->pCoord));  
          qCoord.connectFrom(&(((SoPointKit*)objectTwo)->pCoord));

		  // special case: 1st object is a Plane, this means our Line is
		  // a line normal to a plane through a given point
		  // we must CONNECT the point (which is the mid point of the normal line)
		  // with our line
		  // The midpoint is only used in calculateNormalLine()
		  // since qName != "", we assume pName != "" as well
		  objectOne = SoNode::getByName(pName);
		  if (objectOne->isOfType(SoPlaneKit::getClassTypeId()))
			  midpoint.connectFrom(&(((SoPointKit*)objectTwo)->pCoord));
        }
		else 
		  objectTwoSensor->attach(&(((Object3dKit*)objectTwo)->ChangeObject));

        validObject = TRUE;
      }
      if(intersectionPointName.getValue() != "")
      {
        objectThree = SoNode::getByName(intersectionPointName.getValue());    
        objectThreeSensor->attach(&(((SoPointKit*)objectThree)->ChangeObject));  
        validObject = TRUE;
      } 
      if(TangentPointOne.getValue() != "")
      {
        objectThree = SoNode::getByName(TangentPointOne.getValue());    
        objectThreeSensor->attach(&(((SoPointKit*)objectThree)->ChangeObject));  
        validObject = TRUE;
      } 
      if(TangentPointTwo.getValue() != "")
      {
        objectFour = SoNode::getByName(TangentPointTwo.getValue());    
        objectFourSensor->attach(&(((SoPointKit*)objectFour)->ChangeObject));  
        validObject = TRUE;
        
        //        objectTwoSensor->attach(&(((SoPointKit*)objectFour)->pCoord));  
        //        qCoord.connectFrom(&(((SoPointKit*)objectFour)->pCoord));
      }
    }
    if (validObject)
      Build();
  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
    Object3dKit::setUpConnections(onOff, doItAlways);
    
    SoDrawStyle* drawstyle = (SoDrawStyle*) getPart("style", false);
    drawstyle->lineWidth.disconnect();
    
    pCoord.disconnect();
    qCoord.disconnect();
    SoCoordinate3* coordinate = (SoCoordinate3*) getPart("coord", false);
    coordinate->point.disconnect();
    
    if (objectOneSensor)
      objectOneSensor->detach();
    if (objectTwoSensor)
      objectTwoSensor->detach();
    if (objectThreeSensor)
      objectThreeSensor->detach();
    if(objectFourSensor)
      objectFourSensor->detach();
  } 
  return !(connectionsSetUp = onOff);
  
}


void SoLineKit::Update(SPAposition pos1, SPAposition pos2)
{
  position_One = pos1;
  position_Two = pos2;
  Build();
  this->triggerChange();
}

void 
SoLineKit::setHighlightMaterial(int currentUserID)
{
  SoMaterial* dummy1 = new SoMaterial;
  SoMaterial* dummy2 = new SoMaterial;
  SoMaterial* dummy3 = new SoMaterial;
  SoMaterial* dummy4 = new SoMaterial;
  MaterialConstants::defineLineMaterials(*W, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}

void SoLineKit::Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum)
{
  int counter = 0; 
  int objNumber = childNumber.getValue();
  
  if (TangentPointTwo.getValue() != "")
  {
    counter++;
    SoPointKit *object = (SoPointKit*)SoNode::getByName(TangentPointTwo.getValue());
    object->disconnectChild(this);
    if (object->ObjectName.getValue() != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(object->ObjectName.getValue());
      if(obj)
      {
        obj->disconnectChild(object);
      }
    }
    // an object->Undo would decrease the ObjSum. This would lead to a false objectcount
    // since the point was created by the LineKit
    object->RemoveAllCallbacks();
    SelectionRoot->removeChild(object); // removes this node
    //    object->Undo(SelectionRoot, ObjSum);
    //    ObjSum->setValue(ObjSum->getValue()+1); 
  } 
  
  if (TangentPointOne.getValue() != "")
  {
    counter++;
    SoPointKit *object = (SoPointKit*)SoNode::getByName(TangentPointOne.getValue());
    object->disconnectChild(this);
    if (object->ObjectName.getValue() != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(object->ObjectName.getValue());
      if(obj)
      {
        obj->disconnectChild(object);
      }
    }
    // an object->Undo would decrease the ObjSum. This would lead to a false objectcount
    // since the point was created by the LineKit
    object->RemoveAllCallbacks();
    SelectionRoot->removeChild(object); // removes this node
    //    object->Undo(SelectionRoot, ObjSum);
    //    ObjSum->setValue(ObjSum->getValue()+1); 
  }
  
  RemoveAllCallbacks();
  
  //SoSeparator *looknode = (SoSeparator*) SelectionRoot->getChild(objNumber);
  //SelectionRoot->removeChild((SoSeparator*)looknode);// removes whole seperator node
  SelectionRoot->removeChild(this);// removes whole LineKit
  ObjSum->setValue(ObjSum->getValue() - 1);
}

bool SoLineKit::PointsinaPlane(SbName objectOne, SbName objectTwo)
{
  //both objects are points
  if ((getObject(objectOne)->isOfType(SoPointKit::getClassTypeId())) && 
	  (getObject(objectTwo)->isOfType(SoPointKit::getClassTypeId())) )
    return true;  
  
  bool firstCurveinPlane = false;
  bool secondCurveinPlane = false;
  SPAunit_vector normalOne;
  SPAunit_vector normalTwo;
  SPAposition planePointOne;
  SPAposition planePointTwo;
  
  //check if all points of curveOne are in a plane
  if ((getObject(objectOne)->isOfType(SoCurveKit::getClassTypeId())))
  { 
	  //if objectOne is an intersection curve return TRUE
	  if (((SoCurveKit*)getObject(objectOne))->CurveOp.getValue() == CURVE_INTERSECTION) 
		  return true;
	  else //circle defined by a normal line or in a plane 
		  if ((((SoCurveKit*)getObject(objectOne))->LineName.getValue() != "") ||
			  (((SoCurveKit*)getObject(objectOne))->PlaneName.getValue() != "")) 
		  {
			  EDGE* norm;
			  if (((SoCurveKit*)getObject(objectOne))->LineName.getValue() != "")
				  norm = ((SoLineKit*)getObject(((SoCurveKit*)getObject(objectOne))->LineName.getValue()))->a_Line;
			  SoPointKit* curvepoint = (SoPointKit*)getObject(((SoCurveKit*)getObject(objectOne))->pointsName[0]);
			  SPAposition pos = curvepoint->a_Point;

			  if( (getObject(objectTwo)->isOfType(SoPointKit::getClassTypeId())))
			  {
				  SoPointKit* point = ((SoPointKit*)(getObject(objectTwo)));
				  double distance = 1000; // some value higher than epsilon
				  if (((SoCurveKit*)getObject(objectOne))->LineName.getValue() != "")
					  distance = distance_to_plane(point->a_Point, pos, normalise(norm->start_pos() - norm->end_pos()));
				  if (((SoCurveKit*)getObject(objectOne))->PlaneName.getValue() != "")
					  distance = distance_to_plane(point->a_Point, pos, 
							((SoPlaneKit*)getObject(((SoCurveKit*)getObject(objectOne))->PlaneName.getValue()))->a_Plane->normal());
				  if ((eps > distance) && (-eps < distance))
					  return true;
				  else 
					  return false;
			  }
			  else
				  return false;
		  }
		  else 
			  if (((SoCurveKit*)getObject(objectOne))->pointsName.getNum() == 2) //circle on a plane
			  {
				  SoPointKit* pointOne = (SoPointKit*)getObject(((SoCurveKit*)getObject(objectOne))->pointsName[0]);
				  SoPointKit* pointTwo = (SoPointKit*)getObject(((SoCurveKit*)getObject(objectOne))->pointsName[1]);
				  //get the name of the plane on with the points lie
				  SbName planeName = ComparetwoObjectsNameList((pointOne->NamesofDependentObjects("Plane")), (pointTwo->NamesofDependentObjects("Plane")));
				  if (planeName != "")
				  {
					  //get the plane 
					  SoPlaneKit *plane = (SoPlaneKit*)SoNode::getByName(planeName);
					  firstCurveinPlane = true;
					  normalOne = plane->a_Plane->normal();
					  planePointOne = plane->a_Plane->root_point();
				  }
			  } 
			  else
			  {
				  //create normal of the frist three points of the curve
				  SPAunit_vector norm = getNormal(((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectOne))->pointsName[0])))->a_Point , 
					  ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectOne))->pointsName[1])))->a_Point , 
					  ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectOne))->pointsName[2])))->a_Point );
				  //if there more then 3 points of the curve look if the other lie in the same plane
				  bool allinPlane = true;
				  for (int index = 3; index < ((SoCurveKit*)getObject(objectOne))->pointsName.getNum(); index++)
				  {
					  if ((EPS < distance_to_plane(((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectOne))->pointsName[index])))->a_Point,
						  ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectOne))->pointsName[0])))->a_Point,norm))
						  && (-EPS > distance_to_plane(((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectOne))->pointsName[index])))->a_Point,
						  ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectOne))->pointsName[0])))->a_Point,norm)) )
					  {
						  allinPlane = false;
					  }
				  }
				  if (allinPlane)
				  {
					  firstCurveinPlane = true;
					  normalOne = norm;
					  planePointOne = ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectOne))->pointsName[0])))->a_Point;
				  }
			  }
  }

  //check if all points of curve two are in a plane
  if ( (getObject(objectTwo)->isOfType(SoCurveKit::getClassTypeId())) )
  { 
	  //if objectTwo is a intersection curve return true
	  if (((SoCurveKit*)getObject(objectTwo))->CurveOp.getValue() == CURVE_INTERSECTION) 
		  return true;
	  else //circle defined by a normal line or in a plane  
		  if ((((SoCurveKit*)getObject(objectTwo))->LineName.getValue() != "") ||
			  (((SoCurveKit*)getObject(objectTwo))->PlaneName.getValue() != ""))
	  {
		  EDGE* norm;
		  if (((SoCurveKit*)getObject(objectTwo))->LineName.getValue() != "")
			  norm = ((SoLineKit*)getObject(((SoCurveKit*)getObject(objectTwo))->LineName.getValue()))->a_Line;
		  SoPointKit* curvepoint = (SoPointKit*)getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[0]);
		  SPAposition pos = curvepoint->a_Point;

		  if( (getObject(objectOne)->isOfType(SoPointKit::getClassTypeId())))
		  {
			  SoPointKit* point = ((SoPointKit*)(getObject(objectOne)));
			  double distance = 1000; // some value higher than epsilon
			  // if circle is defined by the normal line, get normal vector from line for distance function
			  if (((SoCurveKit*)getObject(objectTwo))->LineName.getValue() != "")
				  distance = distance_to_plane(point->a_Point, pos, normalise(norm->start_pos() - norm->end_pos()));
			  // if circle is defined in a plane, get normal vector of plane for distance function
			  if (((SoCurveKit*)getObject(objectTwo))->PlaneName.getValue() != "")
				  distance = distance_to_plane(point->a_Point, pos, 
				  ((SoPlaneKit*)getObject(((SoCurveKit*)getObject(objectTwo))->PlaneName.getValue()))->a_Plane->normal());
			  if ((eps > distance) && (-eps < distance))
				  return true;
			  else 
				  return false;
		  }
		  else
			  return false;

	  }
	  else if (((SoCurveKit*)getObject(objectTwo))->pointsName.getNum() == 2) //circle on a plane
	  {
		  SoPointKit* pointOne = (SoPointKit*)getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[0]);
		  SoPointKit* pointTwo = (SoPointKit*)getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[1]);
		  //get the name of the plane on with the points lie
		  SbName planeName = ComparetwoObjectsNameList((pointOne->NamesofDependentObjects("Plane")), (pointTwo->NamesofDependentObjects("Plane")));
		  if (planeName != "")
		  {
			  //get the plane
			  SoPlaneKit *plane = (SoPlaneKit*)SoNode::getByName(planeName);
			  secondCurveinPlane = true;
			  normalTwo = plane->a_Plane->normal();
			  planePointTwo = plane->a_Plane->root_point();
		  }
	  } 
	  else
	  {
		  //create normal of the first three points of the curve
		  SPAunit_vector norm = getNormal(((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[0])))->a_Point , 
			  ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[1])))->a_Point , 
			  ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[2])))->a_Point );
		  //if there more then 3 points of the curve look if the other lie in the same plane
		  bool allinPlane = true;
		  for (int index = 3; index < ((SoCurveKit*)getObject(objectTwo))->pointsName.getNum(); index++)
		  {
			  if ((EPS < distance_to_plane(((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[index])))->a_Point,
				  ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[0])))->a_Point,norm))
				  && (-EPS > distance_to_plane(((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[index])))->a_Point,
				  ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[0])))->a_Point,norm)) )
				  allinPlane = false;
		  }
		  if (allinPlane)
		  {
			  secondCurveinPlane = true;
			  normalTwo = norm;
			  planePointTwo = ((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[0])))->a_Point;
		  }
	  }
  }

  //check if both curves are in a plane
  if (firstCurveinPlane && secondCurveinPlane)
  {
	  SPAposition spapos = SPAposition();
	  SPAunit_vector spaunvec = SPAunit_vector();

	  //check if both planes are parallel and then if one point from curve two lies on the plane of curve one
	  if ( ( 0 == intersect_plane_plane(planePointOne, normalOne, planePointTwo, normalTwo, spapos, spaunvec) )
		  && ((EPS > distance_to_plane(((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[0])))->a_Point,planePointOne,normalOne) )
		  && (-EPS < distance_to_plane(((SoPointKit*)(getObject(((SoCurveKit*)getObject(objectTwo))->pointsName[0])))->a_Point,planePointOne,normalOne)) ) )
		  return true;
  }
  //check if curve one and a point are in a plane
  if ((getObject(objectTwo)->isOfType(SoPointKit::getClassTypeId())))
  {
	  if ( (EPS > distance_to_plane( ((SoPointKit*)getObject(objectTwo))->a_Point,planePointOne,normalOne) )
		  &&(-EPS < distance_to_plane( ((SoPointKit*)getObject(objectTwo))->a_Point,planePointOne,normalOne)))
		  return true;
  }
  //check if curve two and a point are in a plane
  if ((getObject(objectOne)->isOfType(SoPointKit::getClassTypeId())))
  {
	  if ( (EPS > distance_to_plane( ((SoPointKit*)getObject(objectOne))->a_Point,planePointTwo,normalTwo) )
		  && (-EPS < distance_to_plane( ((SoPointKit*)getObject(objectOne))->a_Point,planePointTwo,normalTwo) ))
		  return true;
  }
  return false;
}
