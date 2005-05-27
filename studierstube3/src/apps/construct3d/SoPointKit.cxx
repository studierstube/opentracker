/* =======================================================================
* (C) 2001 Vienna University of Technology
* =======================================================================
* PROJECT: Studierstube
* ======================================================================= */
/** SoPointKit.h containts the implementation of SoPointKit.
* SoPointKit is the point class of Construct3d.
* All points constructed with Construct3d are SoPointKits.
*
* @see contextcnd2 Construct3d - Geometric Construction in n Dimensions 
*
* @author  Hannes Kaufmann
*
* $Id: SoPointKit.cxx 4017 2004-11-17 11:57:24Z kaufmann $
* @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoComplexity.h>

#include "SoPointKit.h"

#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include "SoSphereKit.h"
#include "SoCylinderKit.h"
#include "SoConeKit.h"
#include "SoBoolKit.h"
#include "SoCurveKit.h"
#include "SoSurfaceKit.h"
#include "SoUndoRedoListKit.h"

#include "MaterialConstants.h"

#include <stbapi/util/ivio.h>

SO_KIT_SOURCE(SoPointKit);

void SoPointKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoPointKit, Object3dKit, "Object3dKit");
} 

SoPointKit::SoPointKit() 
{ 
  initSoPointKit();
  CreateSensors();
  setUpConnections(TRUE, TRUE);  
  UpdateAcisObject();
}

SoPointKit::SoPointKit(int number, SbVec3f t, int userID)
{
  initSoPointKit();
  
  childNumber.setValue(number);
  pointSize.setValue(pSize);
  setNodeName("P", number);
  pCoord.setValue(t);
  this->userID.setValue(userID);
  MaterialConstants::definePointMaterials(*W, *S, *D, *N, *L, userID);
  
  UpdateAcisObject();
  CreateSensors();
  setUpConnections(TRUE, TRUE);
}

SoPointKit::SoPointKit(int number, SbVec3f t, SbName objectName, int userID)
{
  initSoPointKit();
  
  childNumber.setValue(number);
  pointSize.setValue(pSize);
  setNodeName("P", number);
  //ObjectName.setValue(object->getName());
  pCoord.setValue(t);
  this->userID.setValue(userID);
  MaterialConstants::definePointMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  // absolutely necessary to update the material here because a Point and also an
  // intersection point like this is most likely, is always constructed with default
  // userID 0 and the material is not updated later on.
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);

  ObjectName.setValue(objectName.getString());
  
  UpdateAcisObject();
  CreateSensors();
  setUpConnections(TRUE, TRUE);
}

SoPointKit::SoPointKit(int number, std::vector<int> objectList, SbVec3f evenPos, int userID)
{
  initSoPointKit();
  
  childNumber.setValue(number);
  pointSize.setValue(pSize);
  setNodeName("P", number);
  pCoord.setValue(evenPos);
  this->userID.setValue(userID);
  MaterialConstants::definePointMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  
  if ( (objectList.size() > 0) &&
    ( (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId()))  ||  
      (getObject(objectList[0])->isOfType(SoPlaneKit::getClassTypeId())) ||  
      (isObject(objectList[0])) ) )
  {
    std::cout << "point on object: " << std::endl; 
    ObjectName.setValue(getObject(objectList[0])->getName());
    
    addDependentObjects(objectList, 1);
    deselectObjects(objectList,1);
    createObjectList(objectList, 1);
  }
  addMovetoRedoUndoList = TRUE;
  UpdateAcisObject(); 
  CreateSensors();
  setUpConnections(TRUE, TRUE);
}

SoPointKit::SoPointKit(int number, std::vector<int> objectList, int userID)
{
  initSoPointKit();
  
  childNumber.setValue(number);
  pointSize.setValue(pSize);
  setNodeName("P", number);
  this->userID.setValue(userID);
  this->translationOn.setValue(FALSE);
  this->rotationOn.setValue(FALSE);
  MaterialConstants::definePointMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  // in CalccentreofLine wird s_return auf OK gesetzt wenn alles funktioniert
  s_return = outcome(-1,NULL); 

  if ( (objectList.size() > 0) && (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) )
  {
    std::cout << "middle point of line: " << std::endl; 
    centreofLine = true;
    ObjectName.setValue(getObject(objectList[0])->getName());
    
    addDependentObjects(objectList, 1);
    deselectObjects(objectList,1);
    createObjectList(objectList, 1);
  
    addMovetoRedoUndoList = FALSE;
    UpdateAcisObject(); 
    CreateSensors();
    setUpConnections(TRUE, TRUE);
  }
}

void
SoPointKit::CreateSensors()
{
  objectOneSensor = new SoFieldSensor(nameUpdate, this);
  objectOneSensor->setPriority(0);
  
  objectTwoSensor = new SoFieldSensor(nameUpdate, this);
  objectTwoSensor->setPriority(0);
  
  if (ObjectName.getValue() != "")
  {
    Object3dKit *obj = (Object3dKit*)SoNode::getByName(ObjectName.getValue());
    if(obj)
    {
      std::cout << "connect to Object: " << ObjectName.getValue().getString() << std::endl;
      obj->connectChild(this);
    }
  }
}

// init all parts and fields
void
SoPointKit::initSoPointKit()
{
  SO_KIT_CONSTRUCTOR(SoPointKit);
  //fields
  SO_NODE_ADD_FIELD(pointSize, (pSize));
  SO_NODE_ADD_FIELD(pCoord, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(PointPosition, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(p, (""));
  SO_NODE_ADD_FIELD(q, (""));
  SO_NODE_ADD_FIELD(ObjectName, (""));
  SO_NODE_ADD_FIELD(StartPosition, (0.0f, 0.0f, 0.0f));
  StartFlag = false;
  MoveFlag = false;
  addMovetoRedoUndoList = false;
  centreofLine = false;
  
  // Initialize this SoPointKit
  SO_KIT_INIT_INSTANCE();
  
  name.setValue("Point");
  ObjectName.setValue("");
  
  // connect fields - pCoord and translation
  // done in setupconnections
  
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);

  SoSphere* sphere1 = new SoSphere;
  sphere1->radius.connectFrom(&(pointSize));
  
  SoComplexity* comp = new SoComplexity;
  comp->value = 0.3;
  SoShapeHints* sh = new SoShapeHints;
  sh->shapeType.setValue(SoShapeHints::SOLID);
  sh->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
  
  SoGroup* nearGp = (SoGroup*) nearestGroup.getValue();//(SoGroup*) getPart("nearestGroup", false);
  nearGp->addChild(comp);
  nearGp->addChild(sh);
  SoSphere* sphere2 = (SoSphere*)sphere1->copy();
  // increase bigger sphere by a small delta = 0.03mm to present a nice 
  // wireframe grid over the point
  sphere2->radius.setValue(float(pointSize.getValue()+0.0003));
  nearGp->addChild(sphere2);
  
  SoMaterial* mat2 = new SoMaterial;
  mat2->transparency = 1;
  
  //SoCube* cube2 = (SoCube*) getPart("transparentobject", false);
  SoCube* cube2 = new SoCube;
  cube2->height = pointDraggingBoxSize;
  cube2->width = pointDraggingBoxSize;
  cube2->depth = pointDraggingBoxSize;
  
  SoGroup* objGrp = (SoGroup*) objectGroup.getValue();//(SoGroup*) getPart("objectGroup", false);
  objGrp->addChild(sphere1);
  objGrp->addChild(mat2);
  objGrp->addChild(cube2);
  
  CreateSensors();
  triggerAllowed = FALSE;
}


// Destructor
SoPointKit::~SoPointKit()
{
    pCoord.disconnect();
    translation.disconnect();

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
}


float
SoPointKit::getDistance(SbVec3f penPosition)
{
  float distance = (penPosition - pCoord.getValue()).length();
  // give point a priority
  // point is always 'nearer' by PointPriorityEpsilon
  // if distance < PointPriorityEpsilon, then distance = 0
  // guarantees better selection of points on surfaces of objects
  if (distance < PointPriorityEpsilon)
    distance = 0;
  else
    distance = distance - PointPriorityEpsilon;
  // printf( "Pointdist: \t%i %.3f \n", childNumber.getValue() , distance);
  return distance;
}

void
SoPointKit::nameUpdate(void *data, SoSensor *sensor)
{
  std::cout << "Point nameupdate "<< std::endl;
  SoPointKit *self=(SoPointKit*)data;
  self->triggerAllowed = TRUE;
  //self->UpdateAcisObject();
  self->triggerChange();
}


SbBool SoPointKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{ 
  //std::cout << "Point setUpConnections "<< std::endl;
  
  if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    //SoBaseKit::setUpConnections(onOff, doItAlways);
    Object3dKit::setUpConnections(onOff, doItAlways);
    
    // connect fields    
    pCoord.connectFrom(&(translation));
    translation.connectFrom(&(pCoord));
  
    SbName Name = ObjectName.getValue();
    SoNode *object = NULL;
    
    if(Name != "")
    {
      object = SoNode::getByName(Name);
      objectOneSensor->attach(&((Object3dKit*)object)->ChangeObject);
      //std::cout << "Name: "<< Name.getString() << std::endl;
    }
	else
	    objectTwoSensor->attach(&pCoord);
    
    UpdateAcisObject();
  }
  else 
  {
    Object3dKit::setUpConnections(onOff, doItAlways);
   
    pCoord.disconnect();
    translation.disconnect();

	if (objectOneSensor)
		objectOneSensor->detach();
	if (objectTwoSensor)
		objectTwoSensor->detach();
  }
  return !(connectionsSetUp = onOff);
}

void SoPointKit::UpdateAcisObject()
{
  if (centreofLine)
  {
    CalcentreofLine();
  }
  else
  {
    //std::cout << "SoPointKit::UpdateAcisObject" << std::endl;
    SPAposition point_on_object, pen_point;
    pen_point.set_x(pCoord.getValue()[0]);
    pen_point.set_y(pCoord.getValue()[1]);
    pen_point.set_z(pCoord.getValue()[2]);
  
  
    bool fixonobject = FALSE;
    SbString string = ObjectName.getValue().getString(); //point dependent on intersection
    if ((ObjectName.getValue() != "") && (string.getSubString(0,0) != "I"))
    {
      fixonobject = TRUE;
      Object3dKit *object = (Object3dKit*)SoNode::getByName(ObjectName.getValue());
      double dis_point_axis;
      ENTITY* entity;
      param_info& ent_info=*(param_info*)NULL_REF;
    
      if (object->name.getValue() == "Curve")
      {
        entity = (ENTITY*)((SoCurveKit*)object)->getEntity();
        s_return = api_entity_point_distance(entity, pen_point, a_Point, dis_point_axis, ent_info);
      }
      else if(object->name.getValue() == "Line")
      {
        fixonobject = TRUE;
        double dis;
        EDGE* edge = ((SoLineKit*)object)->a_Line;
        Line_Plane_Intersect(pen_point, edge->start_pos(), edge->end_pos(), a_Point, dis);
      } else if (object->name.getValue() == "Plane")
      {
        PLANE* pl = ((SoPlaneKit*)object)->a_Plane;
        double para;
        intersect_line_plane(pen_point, (pl->normal()*(-1)), pl->root_point(), pl->normal(), para, a_Point);
      } else if ((object->name.getValue() == "Surface") && (((SoSurfaceKit*)object)->SurfaceOp.getValue() != SURFACE_SWEEP) )
      {
        std::cout << "Point on Surface: " << ObjectName.getValue().getString() << std::endl;
        FACE* face = ((SoSurfaceKit*)object)->a_Surface;
        ENTITY* entity = (ENTITY*)face;
        s_return = api_entity_point_distance(entity, pen_point, a_Point, dis_point_axis, ent_info);
      }
      else  
      {
        std::cout << "Point on Object: " << ObjectName.getValue().getString() << std::endl;
        if (((Object3dKit*)object)->a_Shape != NULL) 
        {
          SHELL* shell = ((Object3dKit*)object)->a_Shape->lump()->shell();
          ENTITY* entity = (ENTITY*)shell;
          s_return = api_entity_point_distance(entity, pen_point, a_Point, dis_point_axis, ent_info);
        }
      }
    }
  
    if (!fixonobject)
      a_Point = SPAposition(pCoord.getValue()[0],pCoord.getValue()[1],pCoord.getValue()[2]);
    else
    {
	  // in case the point is fixed on an object, a objectOnesensor points to ChangeObject 
	  // (of the object it belongs to) and we have to remove objectTwoSensor
	  if (objectTwoSensor)
		  objectTwoSensor->detach();
      pCoord = convertPosition(a_Point);
    }

      //printf("pcoord_x %f \n",pCoord.getValue()[0]);
    //std::cout << "*************** Acis Point: " << a_Point.x() <<" "<< a_Point.y() <<" "<< a_Point.z() << std::endl;
  }

  if (s_return.ok())
    this->visible.setValue(SO_SWITCH_ALL);
  else
    this->visible.setValue(SO_SWITCH_NONE);
}

void SoPointKit::CalcentreofLine()
{
  Object3dKit *object = (Object3dKit*)SoNode::getByName(ObjectName.getValue());
  if(object->name.getValue() == "Line")
  {
    a_Point = ((SoLineKit*)object)->a_Line->start_pos() + (((SoLineKit*)object)->a_Line->end_pos() - ((SoLineKit*)object)->a_Line->start_pos())/2;
    if (objectTwoSensor)
		  objectTwoSensor->detach();
    pCoord = convertPosition(a_Point);
	s_return = outcome(API_SUCCESS,NULL);
  }
}

void
SoPointKit::OnMove()
{
  std::cout << "POINT OnMove, not updating point: " << this->getName().getString() << std::endl;
  // if point is on another object, then we must update the point position while it is moved
  // otherwise it does not "stick" to the object
  SbString ObjNameString = ObjectName.getValue().getString();
  //printf("ObjectName in OnMove in PointKit: %s \n",ObjNameString);
  if (ObjectName.getValue() != "")
	  if (ObjNameString.getSubString(0,0) != "I")
	  {
		  triggerAllowed = TRUE;
		  triggerChange();
	  }
  if (StartFlag)
  {
    MoveFlag = true;
  }
//  UpdateAcisObject();
}


void
SoPointKit::OnPress()
{
  MoveFlag = false;
  std::cout << "POINT Press: " << this->getName().getString() << std::endl;
  if (!StartFlag)
  {
    StartFlag = true;
    StartPosition = convertPosition(a_Point);
  }
}

void
SoPointKit::OnRelease()
{
  triggerChange();
  if (((pCoord != StartPosition) && StartFlag) && (addMovetoRedoUndoList && MoveFlag))
  {
    SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
    list->addCommandtoUndoReduList("move", this->getName(), StartPosition.getValue() , pCoord.getValue());
    UpdateAcisObject();
  }
  StartFlag = false;
  MoveFlag = false;
}

void
SoPointKit::OnChange()
{  
  if (triggerAllowed)
  {
	  std::cout << "Point OnChange, updating point" << std::endl;
	  UpdateAcisObject();
  }
  triggerAllowed = FALSE;
}

SoMFName* SoPointKit::NamesofDependentObjects(SbName ObjectType)
{
  SoMFName* names = new SoMFName;
  int a=1;
  
  if (ObjectName.getValue() != "")
  {
    Object3dKit *obj = (Object3dKit*)SoNode::getByName(ObjectName.getValue());
    if (obj->name.getValue().getString() == ObjectType.getString())
    {
      names->setNum(a);
      names->set(ObjectName.getValue().getString());
      a++;
    }
  }
  
  for (int i=1; i < dependentObjectsName.getNum(); i++)
  {
    if (dependentObjectsName[i] != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(dependentObjectsName[i]);

      if (obj->name.getValue().getString() == ObjectType.getString())
      {
        names->setNum(a);
        names->set(dependentObjectsName[i].getString());
        a++;
      }
      else if (obj->name.getValue() == "Intersect")
      {
        if (getObject(((SoIntersectKit*)obj)->Object1Name.getValue())->name.getValue() == ObjectType)
        {
          names->setNum(a);
          names->set(((SoIntersectKit*)obj)->Object1Name.getValue());
          a++;
        }
        if (getObject(((SoIntersectKit*)obj)->Object2Name.getValue())->name.getValue() == ObjectType)
        {
          names->setNum(a);
          names->set(((SoIntersectKit*)obj)->Object2Name.getValue());
          a++;
        }
      }
    }
  }
  return names;
}

// The following two methods are used in the IntersectionKit for updating line-face intersections
void SoPointKit::Update(SbVec3f coord)
{
  SbBool detached = FALSE;
/*  if (objectTwoSensor)
  {
	objectTwoSensor->detach();
	detached = TRUE;
  }
*/  pCoord.setValue(coord);
//  if (detached)
//	objectTwoSensor->attach(&pCoord);
//  UpdateAcisObject();
  triggerAllowed = TRUE;
  triggerChange();
}

void SoPointKit::Update(float x, float y, float z)
{
  SbBool detached = FALSE;
  if (objectTwoSensor)
  {
	objectTwoSensor->detach();
	detached = TRUE;
  }
  pCoord.setValue(x,y,z);
  if (detached)
	objectTwoSensor->attach(&pCoord);
  UpdateAcisObject();
  triggerAllowed = TRUE;
  triggerChange();
}

// ONLY in this implmentation of setHighlightMaterial, the dragging Material is set too!!!
// In all other Objects, setHighlightMaterial sets only the HIGHLIGHT color and NEAREST color.
// Setting the dragging material here is used in Object3dKit::isInterested(). 
// The reason is that e.g. when user 0 comes near a Point drawn by user 1, we want that
// the Point gets the drag color of user 0 and not 
// the "native" one (of user 1 who constructed the point) 
void 
SoPointKit::setHighlightMaterial(int currentUserID)
{
  SoMaterial* dummy1 = new SoMaterial;
  SoMaterial* dummy2 = new SoMaterial;
  SoMaterial* dummy3 = new SoMaterial;
  MaterialConstants::definePointMaterials(*W, *dummy2, *D, *N, *dummy3, currentUserID);
}

