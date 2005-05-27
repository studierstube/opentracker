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

#include <Inventor/actions/SoGetMatrixAction.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <Inventor/nodes/SoShapeHints.h>

#include "Object3dKit.h"
#include "SoUndoRedoListKit.h"
#include "SoBoolKit.h"
#include "SoConeKit.h"
#include "SoCubeKit.h"
#include "SoSphereKit.h"
#include "SoSurfaceKit.h"
#include "SoCylinderKit.h"
#include "SoCurveKit.h"
#include "MaterialConstants.h"

SO_KIT_SOURCE(Object3dKit);

void Object3dKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(Object3dKit, SoDragKit, "SoDragKit"); 
} 

Object3dKit::Object3dKit() 
{ 
  initObject3d();
}

void
Object3dKit::initObject3d()
{    
  SO_KIT_CONSTRUCTOR(Object3dKit);
  
  // SoDragKit provides base node, transform and content node
  //-----------------------name-----|-type-------|nuldef| par |rite|public
  //    SO_KIT_ADD_CATALOG_ENTRY(material,  SoMaterial,  FALSE, base, transform, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(switchNode,SoSwitch,   FALSE, content,\x0, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(style,SoDrawStyle,  FALSE,switchNode,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(nearestSwitch, SoSwitch, FALSE, switchNode, \x0, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(objectGroup, SoGroup, FALSE, nearestSwitch, \x0, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(nearestGroup, SoGroup, FALSE, nearestSwitch, \x0, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(nearestMaterial, SoMaterial, FALSE, nearestGroup, \x0, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(nearestDrawStyle, SoDrawStyle, FALSE, nearestGroup, \x0, TRUE);

  //fields
  SO_NODE_ADD_FIELD(colorState, (WORK));
  SO_NODE_ADD_FIELD(childNumber, (0));
  SO_NODE_ADD_FIELD(name, (""));
  SO_NODE_ADD_FIELD(visible, (SO_SWITCH_ALL));
  SO_NODE_ADD_FIELD(showLayer, (SO_SWITCH_ALL));
//  SO_NODE_ADD_FIELD(transparencyValue, (0.0f));
  SO_NODE_ADD_FIELD(layer, (0));
  SO_NODE_ADD_FIELD(ChangeObject, (0));
  SO_NODE_ADD_FIELD(dependentObjectsName, (""));
  SO_NODE_ADD_FIELD(counterDependentObjectsName, (""));
  SO_NODE_ADD_FIELD(highlighted, (FALSE));
  SO_NODE_ADD_FIELD(dragBoxIntrusion, (FALSE));
  SO_NODE_ADD_FIELD(objectdeleted, (FALSE));
  SO_NODE_ADD_FIELD(objectList,(""));
  SO_NODE_ADD_FIELD(userID,(-1)); 
  // Initialize this SoPointKit
  SO_KIT_INIT_INSTANCE();
  
  setSearchingChildren(TRUE);
  
  colorState.setValue(WORK);
//  transparencyValue.setValue(0.0);
  visible.setValue(SO_SWITCH_ALL);
  // always show object when generated because no object can be generated into 
  // a non-visible layer
  showLayer.setValue(SO_SWITCH_ALL);

  // done in setUpConnections:
  // combine visible and showLayer:
  // if both are set to SO_SWITCH_ALL (= -3) then show object, else do not (= -1)
  
//  SoMaterial* mat = (SoMaterial*) getPart("material", false);
//  mat->transparency.connectFrom(&transparencyValue);

  W = new SoMaterial;
  S = new SoMaterial;
  D = new SoMaterial;
  N = new SoMaterial;
  L = new SoMaterial; 
  userID.setValue(0);
  MaterialConstants::definePointMaterials(*W, *S, *D, *N, *L, userID.getValue());

  SoGroup* nearGp = (SoGroup*)nearestGroup.getValue();
  // Material of nearest color grid
  SoMaterial* mat3 = (SoMaterial*) nearestMaterial.getValue();
  mat3->copyFieldValues(N);
  SoDrawStyle* wireframe = (SoDrawStyle*) nearestDrawStyle.getValue();
  wireframe->style.setValue(SoDrawStyle::LINES);
  //wireframe->lineWidth.setValue(3);
  
  SoSwitch* nearSwitch = (SoSwitch*) getAnyPart("nearestSwitch", false);
  nearSwitch->whichChild.setValue(0);

  isLocked = FALSE;

  CreateSensors();
  setUpConnections(TRUE, TRUE);
  triggerAllowed = FALSE;
}

// Destructor
Object3dKit::~Object3dKit()
{

	if(highlightedSensor)
	{
		highlightedSensor->detach();
	}
	if (dragBoxIntrusionSensor)
	{
		dragBoxIntrusionSensor->detach();
	}
	if (colorChangeSensor)
	{
		colorChangeSensor->detach();
	}

	api_del_entity(a_Shape);
}

void
Object3dKit::CreateSensors()
{
  highlightedSensor = new SoFieldSensor(highlightUpdate, this);
  highlightedSensor->setPriority(0);

  dragBoxIntrusionSensor = new SoFieldSensor(dragableUpdate, this);
  dragBoxIntrusionSensor->setPriority(0);

  colorChangeSensor = new SoFieldSensor(colorChange, this);
  // do not set Priority to 0 here! causes distribution to crash!
  //  colorChangeSensor->setPriority(0);
}

void
Object3dKit::colorChange(void *data, SoSensor *sensor)
{
  Object3dKit *self=(Object3dKit*)data;
  SoMaterial* mat = (SoMaterial*) self->getPart("material", false);
  switch(self->colorState.getValue())
    {
    case 0:
        {
            //printf("Color work\n");
			self->setHighlightMaterial(self->userID.getValue());
            mat->copyFieldValues(self->W);
			if ((self->name.getValue() != "Line") && (self->name.getValue() != "Curve")) // for all 3D objects
			{
				SoSwitch* nearestSwitch = (SoSwitch*) self->getAnyPart("nearestSwitch", false);
				nearestSwitch->whichChild.setValue(0);
			}
            if (self->name.getValue() == "Plane")
            {
                SoPlaneKit* plane = (SoPlaneKit*) self;
				MaterialConstants::changePlaneTexture(plane, 0, self->userID.getValue());
            }
            if (self->name.getValue() == "Cube")
            {
                SoCubeKit* cube = (SoCubeKit*) self;
				MaterialConstants::changeCubeTexture(cube, 0, self->userID.getValue());
            }
            break;
        }
    case 1:
        {
            //printf("Color highlight\n");
			if ((self->name.getValue() != "Line") && (self->name.getValue() != "Curve"))
			{
				mat->copyFieldValues(self->W);
				SoMaterial* mat3 = (SoMaterial*) self->nearestMaterial.getValue();
				mat3->copyFieldValues(self->N);
				SoSwitch* nearestSwitch = (SoSwitch*) self->getAnyPart("nearestSwitch", false);
				nearestSwitch->whichChild.setValue(SO_SWITCH_ALL);
			}
			else
				mat->copyFieldValues(self->N);
            if (self->name.getValue() == "Plane" || self->name.getValue() == "Cube")
            {
				SoMaterial* mat3 = (SoMaterial*) self->nearestMaterial.getValue();
				mat3->copyFieldValues(self->N);
                // texture with working color is already set in SoCnDKit::highlightNearestObject()
            }
			break;
        }
    case 2:
        {
            //printf("Color drag\n");
            mat->copyFieldValues(self->D);

			SoSwitch* nearestSwitch = (SoSwitch*) self->getAnyPart("nearestSwitch", false);
			nearestSwitch->whichChild.setValue(0);
            
			break;
        }
    case 3:
        {
            //printf("Color select\n");
			self->setHighlightMaterial(self->userID.getValue());
            mat->copyFieldValues(self->S);
			if ((self->name.getValue() != "Line") && (self->name.getValue() != "Curve"))
			{
				SoSwitch* nearestSwitch = (SoSwitch*) self->getAnyPart("nearestSwitch", false);
				nearestSwitch->whichChild.setValue(0);
			}
            if (self->name.getValue() == "Plane")
            {
                SoPlaneKit* plane = (SoPlaneKit*) self;
				MaterialConstants::changePlaneTexture(plane, 1, self->userID.getValue());
                //plane->changePlaneTexture(1, self->userID.getValue());
            }
            if (self->name.getValue() == "Cube")
            {
                SoCubeKit* cube = (SoCubeKit*) self;
				MaterialConstants::changeCubeTexture(cube, 1, self->userID.getValue());
            }
            break;
        }
    case 4:
        {
            //printf("Color LAYER_INACTIVE_WORK\n");
            mat->copyFieldValues(self->L);
            if (self->name.getValue() == "Plane")
            {
                SoPlaneKit* plane = (SoPlaneKit*) self;
				MaterialConstants::changePlaneTexture(plane, 2, self->userID.getValue());
                //plane->changePlaneTexture(2, self->userID.getValue());
            }
            if (self->name.getValue() == "Cube")
            {
                SoCubeKit* cube = (SoCubeKit*) self;
				MaterialConstants::changeCubeTexture(cube, 2, self->userID.getValue());
            }
            break;
        }
    case 5:
        {
            printf("Color LAYER_INACTIVE_HIGHLIGHT\n");
            mat->copyFieldValues(self->L);
            break;
        }
    case 6:
        {
            printf("Color LAYER_INACTIVE_DRAG\n");
            mat->copyFieldValues(self->D);
            break;
        }
    case 7:
        {
            printf("Color LAYER_INACTIVE_SELECT\n");
            mat->copyFieldValues(self->L);
            if (self->name.getValue() == "Plane")
            {
                SoPlaneKit* plane = (SoPlaneKit*) self;
				MaterialConstants::changePlaneTexture(plane, 2, self->userID.getValue());
                //plane->changePlaneTexture(2, self->userID.getValue());
            }
            if (self->name.getValue() == "Cube")
            {
                SoCubeKit* cube = (SoCubeKit*) self;
				MaterialConstants::changeCubeTexture(cube, 2, self->userID.getValue());
            }
            break;
        }
  }
}

void
Object3dKit::highlightUpdate(void *data, SoSensor *sensor)
{
  Object3dKit *self=(Object3dKit*)data;
  int colorValue = self->colorState.getValue();
  // highlighted was set to TRUE
  if (self->highlighted.getValue() == TRUE)
  {
      if (colorValue > 3) // layer inactive
        self->colorState.setValue(LAYER_INACTIVE_HIGHLIGHT);
      else
        self->colorState.setValue(HIGHLIGHT);
  }
  else
  {
      SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
      if (selectionRoot->isSelected(self))
      {
          if (colorValue > 3) // layer inactive
              self->colorState.setValue(LAYER_INACTIVE_SELECT);
          else
              self->colorState.setValue(SELECT);
      }
      else
      {
          if (colorValue > 3) // layer inactive
              self->colorState.setValue(LAYER_INACTIVE_WORK);
          else
              self->colorState.setValue(WORK);
      }
  }
}

void
Object3dKit::dragableUpdate(void *data, SoSensor *sensor)
{
  Object3dKit *self=(Object3dKit*)data;
  int colorValue = self->colorState.getValue();
  // dragBoxIntrusion was set to TRUE
  if (self->dragBoxIntrusion.getValue() == TRUE)
  {
	  // avoid too many color state changes - check if already in drag Color
	  if (colorValue != DRAG && colorValue != LAYER_INACTIVE_DRAG)
	   if (colorValue > 3) // layer inactive
	      self->colorState.setValue(LAYER_INACTIVE_DRAG);
	   else
          self->colorState.setValue(DRAG);
  }
  else
      // switch back from dragging state to no_dragging color only if dragging was
      // previously activated
      if (colorValue == DRAG || colorValue == LAYER_INACTIVE_DRAG)
    {
        SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
        if (self->highlighted.getValue())
        {
            if (colorValue > 3) // layer inactive
                self->colorState.setValue(LAYER_INACTIVE_HIGHLIGHT);
            else
                self->colorState.setValue(HIGHLIGHT);
        }
        else
            if (selectionRoot->isSelected(self))
            {
                if (colorValue > 3) // layer inactive
                    self->colorState.setValue(LAYER_INACTIVE_SELECT);
                else
                    self->colorState.setValue(SELECT);
            }
            else 
            {
                if (colorValue > 3) // layer inactive
                    self->colorState.setValue(LAYER_INACTIVE_WORK);
                else
                    self->colorState.setValue(WORK);
            }
  }
}


SbBool Object3dKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
//  std::cout << "Object3dKit setUpConnections "<< std::endl;
  if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoBaseKit::setUpConnections(onOff, doItAlways);
    
    // combine visible and showLayer:
    // if both are set to SO_SWITCH_ALL (= -3) then show object, else do not (= -1)
	SoCalculator* combine = new SoCalculator();
	combine->a.connectFrom(&visible);
	combine->b.connectFrom(&showLayer);
	combine->expression.set1Value(0, "oa = ((a==-3)&&(b==-3)) ? -3 : -1 ");
	
	SoSwitch* sw = (SoSwitch*) getAnyPart("switchNode", false);
    sw->whichChild.connectFrom(&(combine->oa));

    // do your own special setup here !
    highlightedSensor->attach(&highlighted);
    dragBoxIntrusionSensor->attach(&dragBoxIntrusion);
    colorChangeSensor->attach(&colorState);
  }
  else {
	SoSwitch* sw = (SoSwitch*) switchNode.getValue();
    sw->whichChild.disconnect();

	if(highlightedSensor)
	{
		highlightedSensor->detach();
	}
	if (dragBoxIntrusionSensor)
	{
		dragBoxIntrusionSensor->detach();
	}
	if (colorChangeSensor)
	{
		colorChangeSensor->detach();
	}

    SoBaseKit::setUpConnections(onOff, doItAlways);
  }
  return !(connectionsSetUp = onOff);
}


float
Object3dKit::getDistance(SbVec3f penSPAposition)
{
  return maxDistance;
}

void 
Object3dKit::setTransparency(float transparency)
{
//  transparencyValue.setValue(transparency);
  //    printf("transp %.3f \n",transparency);
}

void
Object3dKit::setNodeName(char id[1], int number)
{
  char name[6];
  char objectNumber[3];
  sprintf(objectNumber, "%d", number);
  strcpy(name,id);
  strncat(name, "_", 1);
  strncat(name, objectNumber, 3);
  //	printf(name);
  this->setName(name);
}

void
Object3dKit::select(SoSelection* selectionRoot)
{
    int colorValue = colorState.getValue();
    if (selectionRoot->isSelected(this))
    {
        selectionRoot->deselect(this);
        if (colorValue > 3) // layer inactive
            colorState.setValue(LAYER_INACTIVE_WORK);
        else
            colorState.setValue(WORK);
    }
    else
    {
        selectionRoot->select(this);
        if (colorValue > 3) // layer inactive
            colorState.setValue(LAYER_INACTIVE_SELECT);
        else
            colorState.setValue(SELECT);
    }
}

void
Object3dKit::deselect(SoSelection* selectionRoot)
{
  if (selectionRoot->isSelected(this))
  {
    selectionRoot->deselect(this);
    if (colorState.getValue() > 3) // layer inactive
      colorState.setValue(LAYER_INACTIVE_WORK);
    else
      colorState.setValue(WORK);
  }
}

void 
Object3dKit::setHighlightMaterial(int currentUserID)
{
	SoMaterial* dummy1 = new SoMaterial;
	SoMaterial* dummy2 = new SoMaterial;
	SoMaterial* dummy3 = new SoMaterial;
	SoMaterial* dummy4 = new SoMaterial;
	MaterialConstants::definePointMaterials(*dummy1, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}

void 
Object3dKit::deleteObject()
{
  visible.setValue(SO_SWITCH_NONE);
  objectdeleted = true;
  std::cout << "delete object: " << this->getName().getString() << std::endl;
}

void 
Object3dKit::undodelete()
{
  visible.setValue(SO_SWITCH_ALL);
  objectdeleted = false;
}

// overload isInterested function from SoDragKit
// if dragging is turned off => translationOn and rotationOn = FALSE, then
// "let the event pass and don't take it" -> return FALSE
SbBool 
Object3dKit::isInterested(SoHandle3DEventAction *h3a)
{
  if ((translationOn.getValue() || rotationOn.getValue()) && h3a->getGrabber() == NULL)
  {
	  So3DEvent* ev = (So3DEvent*) h3a->getEvent(); // get underlying event
      SbViewportRegion vp;
      SoGetBoundingBoxAction bba(vp);
      bba.apply((SoFullPath*)h3a->getPathFromRoot());
      SbXfBox3f xfbox=bba.getXfBoundingBox();
      SbBool intersect = xfbox.intersect(ev->getTranslation());
      // set dragBoxIntrusion to TRUE or FALSE depending if pen is in dragGeometry
	  // Multiuser support for dragGeometry highlighting:
	  // Remember which station sent the highlighting event
	  // Turn highlighting only off if the same station gets 
	  // out of the bounding box again
      if (intersect)
	  {
		  // ONLY in this case setHighlightMaterial is used to set the DRAG Material
		  // setting the DRAG material in setHighlightMaterial is ONLY implemented for the
		  // SoPointKit. In all other Objekts setHighlightMaterial is only implemented to
		  // set the HIGHLIGHT color!!!
		  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
		  //printf("User %d \n", uk->getUserFromStation(ev->getStation()));
		  this->setHighlightMaterial(uk->getUserFromStation(ev->getStation()));
		  
		  eventInStation = ev->getStation();
		  this->dragBoxIntrusion.setValue(intersect);
	  }
	  else
		  if (eventInStation.getValue() == ev->getStation())
			  if (intersect != this->dragBoxIntrusion.getValue())
				this->dragBoxIntrusion.setValue(intersect);


      // only return TRUE if a Button Event is recieved, 
      // otherwise event is not handled, therefore
      // return FALSE on a move event - otherwise SoDragKit will not work
      if ((ev->getType() == dragButton.getValue()+1) &&
        (ev->getButton(dragButton.getValue())))
          if (intersect) 
              return TRUE;
  }
  return FALSE; 
}


void 
Object3dKit::handle3DEvent(SoHandle3DEventAction* h3a)
{
  const int DRAGGER_RELEASE = 0;
  const int DRAGGER_PRESS   = 1;
  const int DRAGGER_MOVE    = 2;
  
  DISABLEDIVNODE(this); 
  SoSwitch* sw = (SoSwitch*)highlightSwitch.getValue();
  
  // compute event code
  So3DEvent* ev = (So3DEvent*)h3a->getEvent();
  int evType = -1; // default = no event
  if(ev->getType() == 0) 
    evType = DRAGGER_MOVE;
  else if(ev->getType() == dragButton.getValue()+1) // button event
    evType = ev->getButton(dragButton.getValue());

  if (!dragging.getValue() && evType == DRAGGER_PRESS)
  {
    if (snapBackSensor->isScheduled())
        snapBackSensor->unschedule();
    dragging.setValue(TRUE);
    trans0L  = translation.getValue();
    rot0L    = rotation.getValue();
    startTransG  = ev->getTranslation();
    startRotG    = ev->getRotation();
    SbViewportRegion vp;
    SoGetMatrixAction ma(vp);

    h3a->getPathFromRoot()->ref();
    ma.apply((SoFullPath*)h3a->getPathFromRoot());
    h3a->getPathFromRoot()->unrefNoDelete();

    globalToLocal = ma.getInverse();
    SbVec3f startTransL;
    globalToLocal.multVecMatrix(startTransG,startTransL);
    pickOffsetL = trans0L-startTransL;
    h3a->setGrabber(this);
	// do not set handled to give event to the application
	//h3a->setHandled();
    ENABLEDIVNODE(this);
    //if(pressCallback) pressCallback(pressCallbackData, this);
    printDependentObjectsName();
    //Object3dKit *object = (Object3dKit*)SoNode::getByName(getDependentObjectName(1));
    //std::cout << "objectname: " << object->getName().getString() << std::endl;
    triggerPressCallback();
  }
  else 
  {
	// ENABLEDIVNODE(this); // not necessary to enable it again
	SoDragKit::handle3DEvent( h3a );
  }
   
}

//-----------------------------------------------------------------------------

void
Object3dKit::setAllCallbacks(Object3dKit *DependentDragKit)
{
  setPressCallback(DependentDragKit->staticTriggerPressCallback, DependentDragKit);
  setReleaseCallback(DependentDragKit->staticTriggerReleaseCallback, DependentDragKit);
  setMoveCallback(DependentDragKit->staticTriggerMoveCallback, DependentDragKit);
}

//-----------------------------------------------------------------------------

void
Object3dKit::removeAllCallbacks(Object3dKit *DependentDragKit)
{
  removePressCallback(DependentDragKit->staticTriggerPressCallback, DependentDragKit);
  removeReleaseCallback(DependentDragKit->staticTriggerReleaseCallback, DependentDragKit);
  removeMoveCallback(DependentDragKit->staticTriggerMoveCallback, DependentDragKit);
  removeChangeCallback(DependentDragKit->staticTriggerChangeCallback, DependentDragKit);
}

void
Object3dKit::removeChangeCallback(Object3dKitCB* cb, void* userData)
{
  for(unsigned int i = 0; i < changeCallback.size(); ++i)
    if(cb == changeCallback[i] && userData == changeCallbackData[i])
    {
      changeCallback[i] = changeCallback.back();
      changeCallbackData[i] = changeCallbackData.back();
      changeCallback.pop_back();
      changeCallbackData.pop_back();
    }
}

void Object3dKit::RemoveAllCallbacks()
{
  std::cout << this->getName().getString() << " "; printDependentObjectsName();
  std::cout << " end " << std::endl;
  int i=1;
  while ( (this->getDependentObjectName(i)) != "null" && (this->getDependentObjectName(i) != "") )
  {
    std::cout << this->getName().getString() << " removeCB von " << this->getDependentObjectName(i).getString() << std::endl;
    Object3dKit* obj = (Object3dKit*)SoNode::getByName(this->getDependentObjectName(i));
    obj->removeAllCallbacks((Object3dKit*)this);
    i++;
  }
}

// Connect childs with Change, Move, Release and Press Callbacks
void
Object3dKit::connectChild(Object3dKit *DependentObject3dKit)
{
  setChangeCallback(DependentObject3dKit->staticTriggerChangeCallback, DependentObject3dKit);
  setAllCallbacks(DependentObject3dKit);
}

// Disconnects a specific child with Change, Move, Release and Press Callbacks
void
Object3dKit::disconnectChild(Object3dKit *DependentObject3dKit)
{
  std::cout << this->getName().getString() << " entfernt callback zu " << DependentObject3dKit->getName().getString() << std::endl;
  removeAllCallbacks(DependentObject3dKit);
}


void
Object3dKit::setChangeCallback(Object3dKitCB* cb, void* userData)
{
  changeCallback.push_back(cb);
  changeCallbackData.push_back(userData);
}


void
Object3dKit::triggerChange()
{
  OnChange();
  for(unsigned int i = 0; i < changeCallback.size(); ++i)
    //for(int i = (changeCallback.size()-1); i >= 0; --i)
    changeCallback[i](changeCallbackData[i], this);
  ChangeObject.setValue((ChangeObject.getValue())+1);
}

int Object3dKit::Line_Plane_Intersect(SPAposition point_on_Plane, SPAposition point_on_Line1, SPAposition point_on_Line2, SPAposition &intersect_Point, double &distance_Point_Line)
{
  param_info& ent_info=*(param_info*)NULL_REF;
  double line_param;
  EDGE *Line;
  api_mk_ed_line(point_on_Line1, point_on_Line2, Line);
  
  int param = intersect_line_plane(point_on_Line1,normalise(point_on_Line1 - point_on_Line2), point_on_Plane, normalise(point_on_Line1 - point_on_Line2), line_param, intersect_Point);
  distance_Point_Line = distance_to_line(point_on_Plane, point_on_Line1, normalise(point_on_Line1 - point_on_Line2));
  
  return param;
}

/**
  * this method is called at creation of any instance subclassed from Object3dKit
  *   - calls the addDependentObjectsName() method of its parents saying:
  *     "hello parent, i'm your child, take me into your list of children"
  *   - calls its own addCounterDependentObjectsName() saying:
  *     "this is my parent, i put into my list of parent for later access"
  * 
  * parameters: 
  *   std::vector<int> objectList             list of parents
  *   int number                              number of parents
  */
void Object3dKit::addDependentObjects(std::vector<int> objectList, int number)
{
  for (int i= 0; i < number; i++)
  {
    // debug output:
	  // printf("adding dependent object: %s to %s \n",this->getName().getString(), getObject(objectList[i])->getName().getString());

    // "hello parent, i'm your child, take me into your list of children"
    getObject(objectList[i])->addDependentObjectsName(this->getName());
    // append parent objectList[i] to own list of parents
    this->addCounterDependentObjectsName(getObject(objectList[i])->getName());
  }
}

void Object3dKit::addDependentObjectsName(SbName ObjectName)
{
  int number = dependentObjectsName.getNum();
  dependentObjectsName.set1Value(number, ObjectName.getString());
}

void Object3dKit::addCounterDependentObjectsName(SbName ObjectName)
{
  int number = counterDependentObjectsName.getNum();
  counterDependentObjectsName.set1Value(number, ObjectName.getString());
}

SoMFName Object3dKit::getAllDependentObjectsName()
{
  return dependentObjectsName;
}

SbName Object3dKit::getDependentObjectName(int num)
{
  if ((num >= 0) && (num < dependentObjectsName.getNum()))
    return dependentObjectsName[num];
  else
    return "null";
}

bool Object3dKit::isDependentObjectsName(SbName ObjectName)
{
  bool isinList = false;
  SbString Name;
  for (int i=0; i < dependentObjectsName.getNum(); i++)
  {
    std::cout << "isinList: " << dependentObjectsName[i].getString() << " == " << ObjectName.getString() << std::endl;
    if ( ObjectName.getString() == dependentObjectsName[i] )
      isinList = true;
  }

  return isinList;
}

int Object3dKit::areAllDependentObjectsSelected(SoSelection* selectionRoot)
{
  for (int i=1; i < dependentObjectsName.getNum(); i++)
  {
    if (dependentObjectsName[i] != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(dependentObjectsName[i]);
      if (!obj->areAllDependentObjectsSelected(selectionRoot))
        return 0;
    }
  }
  return selectionRoot->isSelected(this);
}

void Object3dKit::selectAllDependentObjects(SoSelection* selectionRoot)
{
  if (!selectionRoot->isSelected(this))
    this->select(selectionRoot);
  for (int i=1; i < dependentObjectsName.getNum(); i++)
  {
    if (dependentObjectsName[i] != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(dependentObjectsName[i]);
      obj->selectAllDependentObjects(selectionRoot);
    }
  }
}

void Object3dKit::deselectAllDependentObjects(SoSelection* selectionRoot)
{
  if (selectionRoot->isSelected(this))
    this->select(selectionRoot);
  for (int i=1; i < dependentObjectsName.getNum(); i++)
  {
    if (dependentObjectsName[i] != "")
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(dependentObjectsName[i]);
      obj->deselectAllDependentObjects(selectionRoot);
    }
  }
}


SoMFName* Object3dKit::NamesofDependentObjects(SbName ObjectType)
{
  SoMFName* names = new SoMFName();
  int a=1;
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
      else 
      {
        if (obj->isOfType(SoIntersectKit::getClassTypeId()))
        {
          if (((SoIntersectKit*)obj)->getinvolvedObjectName(ObjectType) != "")
          {
            names->setNum(a);
            names->set(((SoIntersectKit*)obj)->getinvolvedObjectName(ObjectType).getString());
            a++;
          }
        }
      }
    }
  }
  return names;
}

SbName Object3dKit::ComparetwoObjectsNameList(SoMFName* list1, SoMFName* list2)
{
  SbName name;

  for (int i=0; i < list1->getNum(); i++)
  {
    for (int a=0; a < list2->getNum(); a++)
    {
      SbString ObjName1;
      SbString ObjName2;
      list1->get1(i,ObjName1);
      list2->get1(a,ObjName2);
      if (ObjName1 == ObjName2)
      {
        name = ObjName1.getSubString(1,ObjName1.getLength()-2);    
        return name;
      }
    }
  }
  return name;
}

void Object3dKit::printDependentObjectsName(SoMFName list)
{
  SbString Name;
  for (int i=1; i < list.getNum(); i++)
  {
    list.get1(i, Name);
    std::cout << Name.getString() << " ";
  }
  std::cout << std::endl;
}

void Object3dKit::printDependentObjectsName()
{
  for (int i=1; i < dependentObjectsName.getNum(); i++)
  {
    SbString name;
    dependentObjectsName.get1(i, name);
    std::cout << name.getString() << " ";
  }
  std::cout << std::endl;
}

void Object3dKit::deleteDependentObjectsName(SbName objectName)
{
  int i=0;
  while ((i < dependentObjectsName.getNum()) && (dependentObjectsName[i] != objectName))
    i++;

  if (dependentObjectsName[i] == objectName)
  {
    for (int a=i; a < dependentObjectsName.getNum()-1; a++)
      dependentObjectsName.set1(a, dependentObjectsName[a+1].getString());
 
    dependentObjectsName.deleteValues(dependentObjectsName.getNum()-1);
  }
}

/**
  * - traverse depencency-structure upwards (parents, their parents and so forth)
  *   (parent means counter-dependent object)
  * - early termination if locked point is found: we don't use locked structures
  * - search result will be appended to foundObjectNumbersList
  */
// we had this idea of checking for unselected children (strategy A), but we don't need it now
// so 'selectedObjectNumbersList' is not needed
// int Object3dKit::getAllPointsFromAncestors(std::vector<int> foundObjectNumbersList, std::vector<int> selectedObjectNumbersList, SbBool areAllFree) {
int Object3dKit::getAllPointsFromAncestors(std::vector<int>* foundObjectNumbersList, SbBool* areAllFree) {
  // - go thru counter-dependent objects
  //   - if object is locked:
  //     # ask robert for documentation of "test case 2"
  //     # current strategy is "C", but we keep idea "A" for the future
  //     # strategy A:
  //     # (locked means: (1) translationOn==false or (2) has an unselected child)
  //     # strategy C:
  //     # (locked means: (1) translationOn==false)
  //     - set areAllFree false
  //     - stop tree traversal
  //   - if it's a point: add it to the list
  //   - if it's no point:
  //     - call getAllPointsFromAncestors() of this object

  int c1; // counter

  for (c1=1; c1 < counterDependentObjectsName.getNum(); c1++) {
    // if name not empty
    if (counterDependentObjectsName[c1] != "") {
      Object3dKit *counterDependentObject = (Object3dKit*)SoNode::getByName(counterDependentObjectsName[c1]);

      if (counterDependentObject->isLocked.getValue()==TRUE) {
        *areAllFree = FALSE;
        return 1; // return message: early termination
      }
      if (counterDependentObject->getName().getString()[0]=='P') {
        foundObjectNumbersList->push_back(counterDependentObject->childNumber.getValue());
      } // if counterDependentObject is a point
      else {
        counterDependentObject->getAllPointsFromAncestors(foundObjectNumbersList, areAllFree);
      } // if else counterDependentObject is a point
    } // if name not empty
  } // for every counter-dependent object
  return 0; // no error
} // getAllPointsFromAncestors(std::vector<int>*, SbBool*)

/**
  * - traverse depencency-structure upwards (parents, their parents and so forth)
  *   (parent means counter-dependent object)
  * - early termination if locked point is found: we don't use locked structures
  * - search result will be appended to foundObjectNumbersList
  */
int Object3dKit::getAllAncestors(std::vector<int>* foundObjectNumbersList, SbBool* areAllFree) {
  // - go thru counter-dependent objects
  //   - if object is locked:
  //     - set areAllFree false
  //     - stop tree traversal
  //   - add object to the list
  //   - if it's no point:
  //     - call getAllPointsFromAncestors() of this object

  int c1; // counter

  for (c1=1; c1 < counterDependentObjectsName.getNum(); c1++) {
    // if name not empty
    if (counterDependentObjectsName[c1] != "") {
      Object3dKit *counterDependentObject = (Object3dKit*)SoNode::getByName(counterDependentObjectsName[c1]);

      if (counterDependentObject->isLocked.getValue()==TRUE) {
        *areAllFree = FALSE;
        return 1; // return message: early termination
      }
      foundObjectNumbersList->push_back(counterDependentObject->childNumber.getValue());
      if (counterDependentObject->getName().getString()[0]!='P') {
        counterDependentObject->getAllAncestors(foundObjectNumbersList, areAllFree);
      } // if counterDependentObject is not a point
    } // if name not empty
  } // for every counter-dependent object
  return 0; // no error
} // getAllAncestors(std::vector<int>*, SbBool*)

void Object3dKit::Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum)
{
  std::cout << "Undo " << this->getName().getString() << " ObjSum: " << ObjSum->getValue() << " childnumber: " << childNumber.getValue() << std::endl;
  RemoveAllCallbacks();
  
  //SoSeparator *looknode = (SoSeparator*) SelectionRoot->getChild(childNumber.getValue());
  //SelectionRoot->removeChild(looknode); // removes whole seperator node
  // removing THIS instead of a SoSeperator node improves stability a LOT!!! 
  // The Object3dKit is no real SoSeperator node (it is derived from SoBaseKit which is just a SoNode)
  // and this caused somehow unwanted behaviour
  SelectionRoot->removeChild(this); // removes this node
  ObjSum->setValue(ObjSum->getValue()-1); 
}

void Object3dKit::Undo(SoSelection* SelectionRoot, int NodeNumber)
{
  RemoveAllCallbacks();
  
//  SoSeparator *looknode = (SoSeparator*) SelectionRoot->getChild(NodeNumber);
  //std::cout << "Undo " << looknode->getName().getString() << " NodeNumber: " << NodeNumber << " childnumber: " << childNumber.getValue() << std::endl;
  //SelectionRoot->removeChild(looknode); // removes whole seperator node
  SelectionRoot->removeChild(this); // removes this object
}

Object3dKit*
Object3dKit::getObject(int n)
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  Object3dKit *looknode = (Object3dKit*) selectionRoot->getChild(n);
  return looknode;
}

Object3dKit* 
Object3dKit::getObject(SbName name)
{
  Object3dKit* object = (Object3dKit*)SoNode::getByName(name);
  return object;
}

void Object3dKit::deselectObjects(std::vector<int> objectList, int number)
{
//  SoUndoRedoListKit* list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  for (int i= 0; i < number; i++)
  {
    getObject(objectList[i])->select(selectionRoot);
//    list->addCommandtoUndoReduList("select", getObject(objectList[i])->getName());
  }
}

SoMFName* Object3dKit::getObjectList()
{
  return &objectList;
}
  
void Object3dKit::createObjectList(std::vector<int> objectList, int number)
{
  this->objectList.setNum(number);
  for (int i=0; i < number; i++)
  {
    this->objectList.set1(i, getObject(objectList[i])->getName().getString());
  }
/*
  this->objectList.setNum(objectList.size());
  for (int i=0; i < objectList.size(); i++)
  {
    this->objectList.set1(i, getObject(objectList[i])->getName().getString());
  }
*/

}

SoSFVec3f Object3dKit::convertPosition(SPAposition Position)
{
  SoSFVec3f vec;
  vec.setValue(float(Position.x()), float(Position.y()), float(Position.z()));
  return vec;
}

SPAposition Object3dKit::convertSoSFVec3f(const SbVec3f & vec)
{
  SPAposition pos;
  pos.set_x(vec.getValue()[0]);
  pos.set_y(vec.getValue()[1]);
  pos.set_z(vec.getValue()[2]);
  return pos;
}

bool Object3dKit::isObject(int num)
{
  Object3dKit* object = getObject(num);
  if (object->isOfType(SoBoolKit::getClassTypeId()))
    return true;
  else if (object->isOfType(SoConeKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoCubeKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoCylinderKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoSphereKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoSurfaceKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoCurveKit::getClassTypeId()))
    return true; 
  else if (object->isOfType(SoPlaneKit::getClassTypeId()))
    return true; 
  else
    return false;
}

bool Object3dKit::isObject(SbName name)
{
  Object3dKit* object = getObject(name);
  if (object->isOfType(SoBoolKit::getClassTypeId()))
    return true;
  else if (object->isOfType(SoConeKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoCubeKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoCylinderKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoSphereKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoSurfaceKit::getClassTypeId()))
    return true;  
  else if (object->isOfType(SoCurveKit::getClassTypeId()))
    return true; 
  else if (object->isOfType(SoPlaneKit::getClassTypeId()))
    return true; 
  else
    return false;
}

void Object3dKit::setvisible()
{
  if (objectdeleted.getValue() == false)
    visible.setValue(SO_SWITCH_ALL);
}

void Object3dKit::setinvisible()
{
  if (objectdeleted.getValue() == false)
    visible.setValue(SO_SWITCH_NONE);
}

SPAunit_vector Object3dKit::getNormal(SPAposition posOne, SPAposition posTwo, SPAposition posThree)
{
  SPAvector norm;

  //calc the normal of plane
	SPAvector a = posOne - posThree;
	SPAvector b = posTwo - posThree;
	norm.set_x(a.y()*b.z() - a.z()*b.y());
	norm.set_y(a.z()*b.x() - a.x()*b.z());
	norm.set_z(a.x()*b.y() - a.y()*b.x());

  return normalise(norm);
}

BODY* Object3dKit::getBody()
{
  return a_Shape;
}
