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

#include <Inventor/nodes/SoShapeHints.h>
#include "SoCubeKit.h"
#include <Inventor/nodes/SoTexture2.h>

#include "MaterialConstants.h"

SO_KIT_SOURCE(SoCubeKit);

void SoCubeKit::initClass() 
{ 
	SO_KIT_INIT_CLASS(SoCubeKit, Object3dKit, "Object3dKit");
} 

SoCubeKit::SoCubeKit() 
{ 
	initSoCubeKit();
	CreateSensors();
	setUpConnections(TRUE, TRUE);
}

void
SoCubeKit::CreateSensors()
{
	if (s_return.ok())
	{
		objectOneSensor = new SoFieldSensor(nameUpdate, this);
		objectOneSensor->setPriority(0);
        
		objectTwoSensor = new SoFieldSensor(nameUpdate, this);
		objectTwoSensor->setPriority(0);
		
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
	}
}

SoCubeKit::SoCubeKit(int number, std::vector<int> objectList, int userID)
{
	initSoCubeKit();
	
	childNumber.setValue(number);
	setNodeName("Q", number);
	this->userID.setValue(userID);
	MaterialConstants::defineCubeMaterials(*W, *S, *D, *N, *L, this->userID.getValue());

	SoMaterial* mat = (SoMaterial*) getPart("material", false);
	mat->copyFieldValues(W);

	MaterialConstants::changeCubeTexture(this, 0, userID);
	
	s_return = outcome(-1,NULL); 
	
	if (objectList.size() > 1)
	{
		if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) 
			&& (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )  
		{
			ObjectOneName.setValue(getObject(objectList[0])->getName());
			ObjectTwoName.setValue(getObject(objectList[1])->getName());
			
			UpdateAcisObject(); 
			CreateSensors();
			setUpConnections(TRUE, TRUE);
			if (s_return.ok())
			{
				addDependentObjects(objectList, 2);
				deselectObjects(objectList,2);
				createObjectList(objectList, 2);
			}
		}
	}
}

// init all parts and fields
void
SoCubeKit::initSoCubeKit()
{
	SO_KIT_CONSTRUCTOR(SoCubeKit); 
	// Add the catalog parts:
	//-----------------------name--|---type------|nuldef|  par   |rite|public
	SO_KIT_ADD_CATALOG_ENTRY(texture,SoTexture2,FALSE,objectGroup,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(object, SoCube,   FALSE,objectGroup,\x0,TRUE);
	//fields
	SO_NODE_ADD_FIELD(boxCenter, (0.0f, 0.0f, 0.0f));
	
	SO_NODE_ADD_FIELD(boxWidth, (0.0f));
	SO_NODE_ADD_FIELD(boxHeight, (0.0f));
	SO_NODE_ADD_FIELD(boxDepth, (0.0f));
	
	// fields OpenCascade
	SO_NODE_ADD_FIELD(ObjectOneName, (""));
	SO_NODE_ADD_FIELD(ObjectTwoName, (""));
	
	// Initialize this SoCubeKit
	SO_KIT_INIT_INSTANCE();
	
	//turn dragging temporarily off
	translationOn.setValue(FALSE);
	rotationOn.setValue(FALSE);
	
	name.setValue("Cube");
	
	// connect fields in setUpConnections
	
	SoMaterial* mat3 = (SoMaterial*) nearestMaterial.getValue();
	//mat3->diffuseColor.setValue(0, 0, 0);
    mat3->emissiveColor.setValue(0, 0.315, 0.7);

	SoDrawStyle* wireframe = (SoDrawStyle*) nearestDrawStyle.getValue();
	wireframe->lineWidth.setValue(1.5);

	SoCube* box = (SoCube*) getPart("object", false);

//	SoGroup* objGrp = (SoGroup*) objectGroup.getValue();
//	objGrp->addChild(box);

	SoShapeHints* sh = new SoShapeHints;
    sh->shapeType.setValue(SoShapeHints::SOLID);
    sh->vertexOrdering.setValue(SoShapeHints::CLOCKWISE);

	SoGroup* nearGrp = (SoGroup*) nearestGroup.getValue();
	// insert empty texture node which just "turns on material" again
	// by setting a SoTexture2::MODULATE state
	// No Coin Bug - Texture Model state is preserved during traversal of Group nodes
	SoTexture2* neartex = new SoTexture2;
	neartex->model.setValue(SoTexture2::MODULATE);
	nearGrp->addChild(neartex);
    nearGrp->addChild(sh);
    nearGrp->addChild(box);
	
	objectOneSensor=NULL;
	objectTwoSensor=NULL;
	triggerAllowed = FALSE;
}

// Destructor
SoCubeKit::~SoCubeKit()
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
}


// Returns the closest point ON the box to the given point.
// (Returns the point on the center of the Z face if passed the center.)
// only base node contains full scene + translation
float
SoCubeKit::getDistance(SbVec3f penPosition)
{
	double distance;
	
	SPAposition pen;
	pen.set_x(penPosition.getValue()[0]);
	pen.set_y(penPosition.getValue()[1]);
	pen.set_z(penPosition.getValue()[2]);
	
	param_info& ent_info=*(param_info*)NULL_REF;
	api_entity_point_distance((ENTITY*)a_Shape->lump()->shell(), pen, pen, distance, ent_info);
	
	return float(distance);
}


void
SoCubeKit::nameUpdate(void *data, SoSensor *sensor)
{
	std::cout << "SoCubeKit::nameUpdate()" << std::endl;
	
	SoCubeKit *self=(SoCubeKit*)data;
	self->triggerAllowed = TRUE;
	self->triggerChange();
}

void SoCubeKit::UpdateAcisObject()
{
	SbName Obj1Name = ObjectOneName.getValue();
	SbName Obj2Name = ObjectTwoName.getValue();
	
	
	SoPointKit *Point1 = (SoPointKit*)SoNode::getByName(Obj1Name);
	SoPointKit *Point2 = (SoPointKit*)SoNode::getByName(Obj2Name);
	
	s_return = api_del_entity(a_Shape);
	s_return = api_solid_block(Point1->a_Point, Point2->a_Point,a_Shape); 
	
	if (s_return.ok()) 
	{
		boxCenter = convertPosition(Point1->a_Point + (Point2->a_Point - Point1->a_Point)/2);
		boxWidth.setValue(float(Point1->a_Point.x() - Point2->a_Point.x()));  
		boxHeight.setValue(float(Point1->a_Point.y() - Point2->a_Point.y()));  
		boxDepth.setValue(float(Point1->a_Point.z() - Point2->a_Point.z()));  
		
//		std::cout << "Acis Cube from: " << Point1->a_Point.x() <<" "<< Point1->a_Point.y() <<" "<< Point1->a_Point.z()
//			<< " to: "<< Point2->a_Point.x() <<" "<< Point2->a_Point.y() <<" "<< Point2->a_Point.z() << std::endl;
	}
	else
		std::cout << "Error can't create Acis Cube" << std::endl;
	
}

void
SoCubeKit::OnChange()
{
	if (triggerAllowed)
	{
		printf("Updating Cube\n");
		UpdateAcisObject();
	}
	triggerAllowed = FALSE;
}

SbBool SoCubeKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) {
		// We connect AFTER base class.
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		translation.connectFrom(&(boxCenter)); 
		// do your own special setup here !
		// connect fields
		SoCube* box = (SoCube*) getPart("object", false);
		box->width.connectFrom(&(boxWidth));
		box->height.connectFrom(&(boxHeight));
		box->depth.connectFrom(&(boxDepth));

		SbName pName = ObjectOneName.getValue();
		SbName qName = ObjectTwoName.getValue();
		SoNode *objectOne = NULL;
		SoNode *objectTwo = NULL;
		SbBool valid = FALSE;

		if(pName != "")
		{
			objectOne = SoNode::getByName(pName);
			if (objectOneSensor)
			{
				objectOneSensor->attach(&((SoPointKit*)objectOne)->pCoord);
				valid = TRUE;
			}
		}  
		
		if(qName != "")
		{
			objectTwo = SoNode::getByName(qName);
			if (valid)
				valid = FALSE;
			if (objectTwoSensor)
			{
				objectTwoSensor->attach(&((SoPointKit*)objectTwo)->pCoord);
				valid = TRUE;
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
		SoCube* box = (SoCube*) getPart("object", false);
		box->width.disconnect();
		box->height.disconnect();
		box->depth.disconnect();
		
		if (objectOneSensor)
			objectOneSensor->detach();
		if (objectTwoSensor)
			objectTwoSensor->detach();
	}
	return !(connectionsSetUp = onOff); 
}

void 
SoCubeKit::setHighlightMaterial(int currentUserID)
{
	SoMaterial* dummy1 = new SoMaterial;
	SoMaterial* dummy2 = new SoMaterial;
	SoMaterial* dummy3 = new SoMaterial;
	SoMaterial* dummy4 = new SoMaterial;
	MaterialConstants::defineCubeMaterials(*W, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}
