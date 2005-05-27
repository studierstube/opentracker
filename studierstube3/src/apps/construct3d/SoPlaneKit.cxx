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
//  UPDATE:    hk      Klaus Feiler 2002
// ===========================================================================
//  HISTORY:   Time of development: Dezember 1999 - 
//
// ===========================================================================

#include "SoPlaneKit.h"
#include "SoSphereKit.h"
#include "SoCurveKit.h"
#include "entwray.hxx"
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <time.h>

#include "MaterialConstants.h"

SO_KIT_SOURCE(SoPlaneKit);

void SoPlaneKit::initClass() 
{ 
	SO_KIT_INIT_CLASS(SoPlaneKit, Object3dKit, "Object3dKit");
} 

void
SoPlaneKit::CreateSensors()
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
		
		objectSixSensor = new SoFieldSensor(nameUpdate, this);
		objectSixSensor->setPriority(0);

		objectSevenSensor = new SoFieldSensor(nameUpdate, this);
		objectSevenSensor->setPriority(0);

		if(PointOneName.getValue() != "")
		{
			SbName Obj1Name = PointOneName.getValue();
			Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(Obj1Name);
			if(obj1)
				obj1->connectChild(this);
		}  
		if(PointTwoName.getValue() != "")
		{
			SbName Obj2Name = PointTwoName.getValue();
			Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
			if(obj2)
				obj2->connectChild(this);
		}
		if(PointThreeName.getValue() != "")
		{ 
			SbName Obj3Name = PointThreeName.getValue();
			Object3dKit *obj3 = (Object3dKit*)SoNode::getByName(Obj3Name);
			if(obj3)
				obj3->connectChild(this);
		}
		if(ObjectName.getValue() != "")
		{ 
			SbName Obj4Name = ObjectName.getValue();
			Object3dKit *obj4 = (Object3dKit*)SoNode::getByName(Obj4Name);
			if(obj4)
			{
				obj4->connectChild(this);
				printf("sensor to plane created\n");
			}
		}
		if(SphereOneName.getValue() != "")
		{ 
			SbName Obj5Name = SphereOneName.getValue();
			Object3dKit *obj5 = (Object3dKit*)SoNode::getByName(Obj5Name);
			if(obj5)
			{
				obj5->connectChild(this);
				printf("sensor to first sphere created\n");
			}
		}
		if(SphereTwoName.getValue() != "")
		{ 
			SbName Obj6Name = SphereTwoName.getValue();
			Object3dKit *obj6 = (Object3dKit*)SoNode::getByName(Obj6Name);
			if(obj6)
			{
				obj6->connectChild(this);
				printf("sensor to second sphere created\n");
			}
		}
		if(LineOneName.getValue() != "")
		{ 
			SbName Obj7Name = LineOneName.getValue();
			Object3dKit *obj7 = (Object3dKit*)SoNode::getByName(Obj7Name);
			if(obj7)
			{
				obj7->connectChild(this);
				printf("sensor to first line created\n");
			}
		}
		if(LineTwoName.getValue() != "")
		{ 
			SbName Obj8Name = LineTwoName.getValue();
			Object3dKit *obj8 = (Object3dKit*)SoNode::getByName(Obj8Name);
			if(obj8)
			{
				obj8->connectChild(this);
				printf("sensor to second line created\n");
			}
		}
	}
}

SoPlaneKit::SoPlaneKit() 
{ 
	initSoPlaneKit();
	CreateSensors();
	setUpConnections(TRUE, TRUE);
}

SoPlaneKit::SoPlaneKit(int number, std::vector<int> objectList, int userID)
{
	initSoPlaneKit();
	
	childNumber.setValue(number);
	this->objSum = objSum;
	setNodeName("E", number);
	this->userID.setValue(userID);
	MaterialConstants::definePlaneMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
	SoMaterial* mat = (SoMaterial*) getPart("material", false);
	mat->copyFieldValues(W);
	
	MaterialConstants::changePlaneTexture(this, 0, userID);
	//changePlaneTexture(0, userID);
	
	s_return = outcome(-1,NULL); 
	
	bool rightObjects=false;
	int num = 0;
	
	std::cout << "objectList.size(): " << objectList.size() << std::endl;

	if(objectList.size() > 2) //3 points
	{
		if( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
			(getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) &&
			(getObject(objectList[2])->isOfType(SoPointKit::getClassTypeId())) )
		{
			PointOneName = getObject(objectList[0])->getName();
			PointTwoName = getObject(objectList[1])->getName();
			PointThreeName = getObject(objectList[2])->getName();
			
			num = 3;
			rightObjects = true;
      size3Points.setValue(TRUE);
		}
	}
	
	if (rightObjects)
		UpdateAcisObject();
	if (rightObjects && s_return.ok())
	{
		addDependentObjects(objectList, num);
		deselectObjects(objectList, num);
		createObjectList(objectList, num);
		
		CreateSensors();
		setUpConnections(TRUE, TRUE);    
	}
}


SoPlaneKit::SoPlaneKit(int number, std::vector<int> objectList, SbBool normalPlane, SbBool symmetryPlane, 
					   int userID, SoSFInt32* objSum)
{
	initSoPlaneKit();
	
	childNumber.setValue(number);
	this->objSum = objSum;
	setNodeName("E", number);
	normal.setValue(normalPlane);
	symmetry.setValue(symmetryPlane);
	this->userID.setValue(userID);
	MaterialConstants::definePlaneMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
	SoMaterial* mat = (SoMaterial*) getPart("material", false);
	mat->copyFieldValues(W);
	
	MaterialConstants::changePlaneTexture(this, 0, userID);
	//changePlaneTexture(0, userID);
	
	s_return = outcome(-1,NULL); 
	
	bool rightObjects=false;
	int num = 0;
	
	std::cout << "objectList.size(): " << objectList.size() << std::endl;
	
	if(symmetryPlane) //symetrieplane
	{
		if(objectList.size() > 1) //symetrieplane 2 points
		{
			if( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
				(getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )
			{
				PointTwoName = getObject(objectList[0])->getName();
				PointThreeName = getObject(objectList[1])->getName();
				
				num = 2;
				rightObjects = true;
			}
		}
		if (objectList.size() > 0) //symetrieplane 1 line
		{
			if((getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())))
			{
				PointTwoName = ((SoLineKit*)getObject(objectList[0]))->p;
				PointThreeName = ((SoLineKit*)getObject(objectList[0]))->q;
				
				num = 1;
				rightObjects = true;
			}
		}
	}
	if (!normalPlane && !symmetryPlane) //plane
	{
		if(objectList.size() > 2) //3 points
		{
			if( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
				(getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) &&
				(getObject(objectList[2])->isOfType(SoPointKit::getClassTypeId())) )
			{
				PointOneName = getObject(objectList[0])->getName();
				PointTwoName = getObject(objectList[1])->getName();
				PointThreeName = getObject(objectList[2])->getName();
				
				num = 3;
				rightObjects = true;
			}
		}
		if(objectList.size() > 1) //1 points, 1 line
		{
			if( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
				(getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) )
			{
				PointOneName = getObject(objectList[0])->getName();
				PointTwoName = ((SoLineKit*)getObject(objectList[1]))->p;
				PointThreeName = ((SoLineKit*)getObject(objectList[1]))->q;
				
				num = 2;
				rightObjects = true;
			}
			if( (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) &&
				(getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) )
			{
				PointOneName = getObject(objectList[1])->getName();
				PointTwoName = ((SoLineKit*)getObject(objectList[0]))->p;
				PointThreeName = ((SoLineKit*)getObject(objectList[0]))->q;
				
				num = 2;
				rightObjects = true;
			}
		}
	}
	if (normalPlane && !symmetryPlane) //normal plane
	{
		if(objectList.size() > 1) //1 points, 1 line
		{
			if( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
				(getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) )
			{
				PointOneName = getObject(objectList[0])->getName();
				LineOneName = getObject(objectList[1])->getName();
//				PointTwoName = ((SoLineKit*)getObject(objectList[1]))->p;
//				PointThreeName = ((SoLineKit*)getObject(objectList[1]))->q;
				
				num = 2;
				rightObjects = true;
			}
			if( (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) &&
				(getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) )
			{
				PointOneName = getObject(objectList[1])->getName();
				LineOneName = getObject(objectList[0])->getName();

//				PointTwoName = ((SoLineKit*)getObject(objectList[0]))->p;
//				PointThreeName = ((SoLineKit*)getObject(objectList[0]))->q;
				
				num = 2;
				rightObjects = true;
			}
		}    
	}
	if (!normalPlane && !symmetryPlane) //normal plane
	{
		if(objectList.size() > 1) //1 object, 1 point
		{
			if( (isObject(objectList[0])) &&
				(getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) )
			{
				//point on given object
				point_containment pc;
				api_point_in_body(((SoPointKit*)getObject(objectList[1]))->a_Point, 
									getObject(objectList[0])->getBody(), pc, FALSE, NULL);
				
				if (pc == point_boundary)
				{
					ObjectName = getObject(objectList[0])->getName();
					PointTwoName = ((SoPointKit*)getObject(objectList[1]))->getName();
					
					num = 2;
					rightObjects = true;
				}
			}
			if( (isObject(objectList[1])) &&
				(getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) )
			{
				//point fixed on given object
				//point on given object
				point_containment pc;
				api_point_in_body(((SoPointKit*)getObject(objectList[0]))->a_Point, 
									getObject(objectList[1])->getBody(), pc, FALSE, NULL);
				
				if (pc == point_boundary)
				{
					ObjectName = getObject(objectList[1])->getName();
					PointTwoName = ((SoPointKit*)getObject(objectList[0]))->getName();
					
					num = 2;
					rightObjects = true;
				}
			}
		} 
		if((objectList.size() == 3)  //tangent plan between two spheres
			&& ((getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())))
			&& ((getObject(objectList[1])->isOfType(SoSphereKit::getClassTypeId())))
			&& ((getObject(objectList[2])->isOfType(SoSphereKit::getClassTypeId()))) )
		{	
			PointOneName  = getObject(objectList[0])->getName();
			SphereOneName = getObject(objectList[1])->getName();
			SphereTwoName = getObject(objectList[2])->getName();	
			
			num = 3;
			rightObjects = true;
			childNumber.setValue(number + 2);
			setNodeName("E", number + 2);
		}
		if((objectList.size() == 3)  //tangent plan between two spheres
			&& ((getObject(objectList[0])->isOfType(SoSphereKit::getClassTypeId())))
			&& ((getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())))
			&& ((getObject(objectList[2])->isOfType(SoSphereKit::getClassTypeId()))) )
		{	
			SphereOneName = getObject(objectList[0])->getName();
			PointOneName  = getObject(objectList[1])->getName();
			SphereTwoName = getObject(objectList[2])->getName();	
			
			num = 3;
			rightObjects = true;
			childNumber.setValue(number + 2);
			setNodeName("E", number + 2);
		}
		if((objectList.size() == 3)  //tangent plan between two spheres
			&& ((getObject(objectList[0])->isOfType(SoSphereKit::getClassTypeId())))
			&& ((getObject(objectList[1])->isOfType(SoSphereKit::getClassTypeId())))
			&& ((getObject(objectList[2])->isOfType(SoPointKit::getClassTypeId()))) )
		{	
			SphereOneName = getObject(objectList[0])->getName();
			SphereTwoName = getObject(objectList[1])->getName();
			PointOneName  = getObject(objectList[2])->getName();
			
			num = 3;
			rightObjects = true;
			childNumber.setValue(number + 2);
			setNodeName("E", number + 2);
		}
		if((objectList.size() == 2) //plane defined by two line
			&& ((getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())))
			&& ((getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId()))) )
		{	
			LineOneName = getObject(objectList[0])->getName();
			LineTwoName = getObject(objectList[1])->getName();
			
			num = 2;
			rightObjects = true;
		}
	}  
	
	if (rightObjects)
		UpdateAcisObject();
	if (rightObjects && s_return.ok())
	{
		addDependentObjects(objectList, num);
		deselectObjects(objectList, num);
		createObjectList(objectList, num);
		
		CreateSensors();
		setUpConnections(TRUE, TRUE);    
	}
}

// init all parts and fields
void
SoPlaneKit::initSoPlaneKit()
{
	SO_KIT_CONSTRUCTOR(SoPlaneKit); 
	// Add the catalog parts:
	//-----------------------name--|---type------|nuldef|  par   |rite|public
	//  SO_KIT_ADD_CATALOG_ENTRY(objectVertexProperty,SoVertexProperty,FALSE,switchNode,\x0,TRUE);
	//  SO_KIT_ADD_CATALOG_ENTRY(object,SoFaceSet,   FALSE,switchNode,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(texture,SoTexture2,FALSE,objectGroup,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(objectVertexProperty,SoVertexProperty,FALSE,objectGroup,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(object,SoFaceSet,   FALSE,objectGroup,\x0,TRUE);
    
	//fields
	//  SO_NODE_ADD_FIELD(backColor, (0.2f, 0.2f, 0.2f));
	SO_NODE_ADD_FIELD(normal, (FALSE));
	SO_NODE_ADD_FIELD(symmetry, (FALSE));
  SO_NODE_ADD_FIELD(size3Points, (FALSE));
	SO_NODE_ADD_FIELD(PointOneName, (""));
	SO_NODE_ADD_FIELD(PointTwoName, (""));
	SO_NODE_ADD_FIELD(PointThreeName, (""));
	SO_NODE_ADD_FIELD(ObjectName, (""));
	SO_NODE_ADD_FIELD(SphereOneName, (""));
	SO_NODE_ADD_FIELD(SphereTwoName, (""));
    SO_NODE_ADD_FIELD(LineOneName, (""));
    SO_NODE_ADD_FIELD(LineTwoName, (""));
	SO_NODE_ADD_FIELD(TangentPointOneName, (""));
	SO_NODE_ADD_FIELD(TangentPointTwoName, (""));
	
    // Initialize this SoPlaneKit
	SO_KIT_INIT_INSTANCE();
	
	//turn dragging temporarily off
	translationOn.setValue(FALSE);
	rotationOn.setValue(FALSE);
	
	//    normal.setValue(FALSE);
	name.setValue("Plane");
	
	// connect fields
	//  transparencyValue.setValue(0.5);
	
	SoDrawStyle* wireframe = (SoDrawStyle*) nearestDrawStyle.getValue();
	wireframe->lineWidth.setValue(2);
	
	SoGroup* nearGrp = (SoGroup*) nearestGroup.getValue();
	// insert empty texture node which just "turns on material" again
	// by setting a SoTexture2::MODULATE state
	// No Coin Bug - Texture Model state is preserved during traversal of Group nodes
	SoTexture2* neartex = new SoTexture2;
	neartex->model.setValue(SoTexture2::MODULATE);
	nearGrp->addChild(neartex);
	nearGrp->addChild(objectVertexProperty.getValue());
	nearGrp->addChild(object.getValue());
	
	objectOneSensor = NULL;
	objectTwoSensor = NULL;
	objectThreeSensor = NULL;
	objectFourSensor = NULL;
	objectFiveSensor = NULL;
	objectSixSensor = NULL;
	objectSevenSensor = NULL;
	
	//translation.connectFrom(&(movePlane)); 
}

// Destructor
SoPlaneKit::~SoPlaneKit()
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
	if(objectSixSensor)
	{
		objectSixSensor->detach();
		delete objectSixSensor;
	}
	if(objectSevenSensor)
	{
		objectSevenSensor->detach();
		delete objectSevenSensor;
	}

	api_del_entity(a_Plane);
}


float
SoPlaneKit::getDistance(SbVec3f penPosition)
{
	double distance=999.99;
	
	SPAposition pen;
	pen.set_x(penPosition.getValue()[0]);
	pen.set_y(penPosition.getValue()[1]);
	pen.set_z(penPosition.getValue()[2]);
	
	param_info& ent_info=*(param_info*)NULL_REF;
	if (s_return.ok())
		distance = distance_to_plane(pen, a_Plane->root_point(), a_Plane->normal());

	//std::cout << "plane distance: " << (distance + 0.01) << std::endl;
	return float(distance + 0.01);
}



void
SoPlaneKit::nameUpdate(void *data, SoSensor *sensor)
{
	printf("Plane: nameUpdate \n");
	SoPlaneKit *self=(SoPlaneKit*)data;
	self->triggerAllowed = TRUE;
	self->triggerChange();
}

void
SoPlaneKit::UpdateAcisObject()
{
	api_del_entity(a_Plane);
	std::cout << "planekit update acis object" << std::endl;
	//if ( !(getObject(PointOneName.getValue())->isOfType(SoPointKit::getClassTypeId())))
	
	if ((SphereOneName.getValue() != "") && (SphereTwoName.getValue() != ""))
		caltangentPlanebetweentwoSphere();
	
	else 
		if ((LineOneName.getValue() !="") && (LineTwoName.getValue()!=""))
		{
			if (((getObject(LineOneName.getValue()))->isOfType(SoLineKit::getClassTypeId())) 
				&& ((getObject(LineTwoName.getValue()))->isOfType(SoLineKit::getClassTypeId())) )
				calPlanedefinedbytwoLine();
		}
		
		else if (ObjectName.getValue() != "") 
			caltangentPlane();
		else if (size3Points.getValue() == TRUE)
			calPlanesize3Points();
	else
	{
		//plane def. by three points
		SbBool validPoint = FALSE;
		SoPointKit *Point1 = NULL;
		SoPointKit *Point2 = NULL;
		SoPointKit *Point3 = NULL;
		SoLineKit *Line1 = NULL;
		
		if (PointOneName.getValue() != "")
		{ 
			SbName Point1Name = PointOneName.getValue();
			Point1 = (SoPointKit*)SoNode::getByName(Point1Name);
			validPoint = TRUE;
		}
		if (PointTwoName.getValue() != "")
		{ 
			SbName Point2Name = PointTwoName.getValue();
			Point2 = (SoPointKit*)SoNode::getByName(Point2Name);
			validPoint = TRUE;
		}
		if (PointThreeName.getValue() != "")
		{ 
			SbName Point3Name = PointThreeName.getValue();
			Point3 = (SoPointKit*)SoNode::getByName(Point3Name);
			validPoint = TRUE;
		}
		if (LineOneName.getValue() != "")
		{ 
			Line1 = (SoLineKit*)SoNode::getByName(LineOneName.getValue());
			validPoint = TRUE;
		}

		// points invalid in case of distribution if UpdateAcisObject is called 
		// (in SetUpConnections for instance)
		if (validPoint)
		{
			SPAvector norm;
			SoSFVec3f centre;
			double planeSize=0.0;
			
			//if not normal calc normal of three points
			if (!normal.getValue())
			{
				//calc the normal of plane
				SPAvector a = Point1->a_Point - Point3->a_Point;
				SPAvector b = Point2->a_Point - Point3->a_Point;
				norm.set_x(a.y()*b.z() - a.z()*b.y());
				norm.set_y(a.z()*b.x() - a.x()*b.z());
				norm.set_z(a.x()*b.y() - a.y()*b.x());
				
				// get centre of the 3 Points
				centre = GetcentrePoint(Point1->a_Point,Point2->a_Point,Point3->a_Point);
				
				planeSize = (2 * GetmaxDistancePointtoPoints(centre,Point1->a_Point,Point2->a_Point,Point3->a_Point) + distancePlanepointToEdge);
			}
			else //the normal is given by two points or by a line (generated by intersection)
			{
				if ((PointTwoName.getValue() != "") && (PointThreeName.getValue() != ""))
				{
					// should work if Point2 and Point3 are correctly defined - p and q
					//set normal of plane
					norm = normalise(Point2->a_Point - Point3->a_Point);
				}
				else
				{
					SbVec3f line_dir = Line1->pCoord.getValue() - Line1->qCoord.getValue();
					SPAvector line_dir_acis (line_dir[0], line_dir[1], line_dir[2]);
					norm = normalise(line_dir_acis);
					//					SoLineKit* line = (SoLineKit*)getObject(LineOneName.getValue());
					//					norm = normalise(line->a_Line->end_pos() - line->a_Line->start_pos());
				}

				// If the plane is not a plane of symmetry, it is a normal plane
				if(!symmetry.getValue())
				{
					SPAposition point_on_line;
					double dis_point_axis;

					if ((PointTwoName.getValue() != "") && (PointThreeName.getValue() != ""))
						Line_Plane_Intersect(Point1->a_Point,
											Point2->a_Point,
											Point3->a_Point,
											point_on_line,
											dis_point_axis);
					else
					{
						Line_Plane_Intersect(Point1->a_Point,
											convertSoSFVec3f(Line1->pCoord.getValue()),
											convertSoSFVec3f(Line1->qCoord.getValue()),
											point_on_line,
											dis_point_axis);
					}
					
					planeSize = 2*dis_point_axis + distancePlanepointToEdge;
					
					//set the center of the plane
					centre = convertPosition(point_on_line);
				}
				else
				{
					//set the center of the plane
					//the middle of the two given points
					// a line without points on it (generated by intersection)
					// cannot be used for calculating a plane of symmetry,
					// because there is no start and end point
					centre.setValue(float(Point2->a_Point.x()+Point3->a_Point.x())/2, 
									float(Point2->a_Point.y()+Point3->a_Point.y())/2, 
									float(Point2->a_Point.z()+Point3->a_Point.z())/2);
				}
				
			}
			
			a_Plane = new PLANE(convertSoSFVec3f(centre.getValue()), normalise(norm));
			
			SbVec3f *VertexArray = 0;
			SPAposition reference_point;
			if (PointTwoName.getValue() != "")
				reference_point = Point2->a_Point;
			else
				reference_point = convertSoSFVec3f(Line1->pCoord.getValue());
			int NumVertices = FacePlane(planeSize, a_Plane->normal(), reference_point, VertexArray);
			//FacePlane(a_Plane->normal(), Point1->a_Point, Point2->a_Point, Point3->a_Point, VertexArray);

			if (s_return.ok())
			{
				// search the point of VertexArray who has max distance
				SoSFVec3f maxPoint = PointmaxDistance(VertexArray, NumVertices, reference_point);
				
				//move the plane (centre - maxpoint)
				translation.setValue((centre.getValue() - maxPoint.getValue()) - 
										((convertPosition(reference_point).getValue() 
											- maxPoint.getValue())/2));
			}
			else
				std::cout << "can't create plane" << std::endl;
			
			if(VertexArray)
				delete[](VertexArray);
		}
	}

  if (s_return.ok())
	  if (objectdeleted.getValue() == false)
		this->visible.setValue(SO_SWITCH_ALL);
  else
    this->visible.setValue(SO_SWITCH_NONE);
}

void
SoPlaneKit::calPlanesize3Points()
{
	//plane def. by three points
	SbBool validPoint = FALSE;
	SoPointKit *Point1 = NULL;
	SoPointKit *Point2 = NULL;
	SoPointKit *Point3 = NULL;
	
	if (PointOneName.getValue() != "")
	{ 
		SbName Point1Name = PointOneName.getValue();
		Point1 = (SoPointKit*)SoNode::getByName(Point1Name);
		validPoint = TRUE;
	}
	if (PointTwoName.getValue() != "")
	{ 
		SbName Point2Name = PointTwoName.getValue();
		Point2 = (SoPointKit*)SoNode::getByName(Point2Name);
		validPoint = TRUE;
	}
	if (PointThreeName.getValue() != "")
	{ 
		SbName Point3Name = PointThreeName.getValue();
		Point3 = (SoPointKit*)SoNode::getByName(Point3Name);
		validPoint = TRUE;
	}
	
	// points invalid in case of distribution if UpdateAcisObject is called 
	// (in SetUpConnections for instance)
	if (validPoint)
	{
		SPAvector norm;
		SoSFVec3f centre;
		double planeSize=0.0;
		
		//calc the normal of plane
		SPAvector a = Point1->a_Point - Point3->a_Point;
		SPAvector b = Point2->a_Point - Point3->a_Point;
		norm.set_x(a.y()*b.z() - a.z()*b.y());
		norm.set_y(a.z()*b.x() - a.x()*b.z());
		norm.set_z(a.x()*b.y() - a.y()*b.x());
		
		a_Plane = new PLANE(Point1->a_Point, normalise(norm));
		
		SbVec3f *VertexArray = 0;
		FacePlane(a_Plane->normal(), Point1->a_Point, Point2->a_Point, Point3->a_Point, VertexArray);

		if(VertexArray)
			delete[](VertexArray);
	}

  if (s_return.ok())
	if (objectdeleted.getValue() == false)
		this->visible.setValue(SO_SWITCH_ALL);
  else
    this->visible.setValue(SO_SWITCH_NONE);
}

void SoPlaneKit::caltangentPlane()
{
  //cal tangent plane
	point_containment pc;
	BODY* body = getObject(ObjectName.getValue())->getBody();
	SPAposition pos = ((SoPointKit*)getObject(PointTwoName.getValue()))->a_Point;
	s_return = api_point_in_body(pos, body, pc, FALSE, NULL);
	if ((pc == point_boundary) && s_return.ok())
	{
		if (body != NULL)
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
			  
			  //create the plane
			  a_Plane = new PLANE(pos, norm);
			  
			  SbVec3f *VertexArray = 0;
			  int NumVertices = FacePlane(tangentplaneSize, a_Plane->normal(), pos, VertexArray);
			  
			  if (s_return.ok())
			  {
				  // search the point of VertexArray who has max distance
				  SoSFVec3f maxPoint = PointmaxDistance(VertexArray, NumVertices, pos);
				  
				  //move the plane (centre - maxpoint)
				  translation.setValue((convertPosition(pos).getValue() - maxPoint.getValue()) 
					  - ((convertPosition(pos).getValue() - maxPoint.getValue())/2));
			  }
			  else
				  std::cout << "can't create plane" << std::endl;
			  
			  if(VertexArray)
				  delete[](VertexArray);
		  }
		  else
			  std::cout << "can't create plane" << std::endl;
    }
		
	}
	else
		std::cout << "can't create plane" << std::endl;
}

void SoPlaneKit::calPlanedefinedbytwoLine()
{
  std::cout << "creat Plane defined by two line" << std::endl;
  EDGE* edgeOne = ((SoLineKit*)getObject(LineOneName.getValue()))->a_Line;
  EDGE* edgeTwo = ((SoLineKit*)getObject(LineTwoName.getValue()))->a_Line;
  curve_curve_int* intersects=NULL;
  
  s_return = api_intersect_curves(edgeOne, edgeTwo, FALSE, intersects, NULL);

  SPAposition intPoint;
  if (intersects)
  {
    intPoint = intersects->int_point;
    
    //calc normal
    SPAvector norm, a, b;

    if (intPoint != edgeOne->start_pos())
    {
      a = edgeOne->start_pos() - intPoint;
    }
    else
    {
      a = edgeOne->end_pos() - intPoint;
    }
    if (intPoint != edgeTwo->start_pos())
    {
      b = edgeTwo->start_pos() - intPoint;
    }
    else
    {
      b = edgeTwo->end_pos() - intPoint;
    }

    norm.set_x(a.y()*b.z() - a.z()*b.y());
    norm.set_y(a.z()*b.x() - a.x()*b.z());
    norm.set_z(a.x()*b.y() - a.y()*b.x());

    a_Plane = new PLANE(intPoint, normalise(norm));

    SbVec3f *VertexArray = 0;
		int NumVertices = FacePlane(minPlaneSize, a_Plane->normal(), edgeOne->start_pos(), VertexArray);
		
		if (s_return.ok())
		{

      // search the point of VertexArray who has max distance
			SoSFVec3f maxPoint = PointmaxDistance(VertexArray, NumVertices, edgeOne->start_pos());
			
			//move the plane (centre - maxpoint)
			translation.setValue((convertPosition(intPoint).getValue() - maxPoint.getValue()) - 
        ((convertPosition(edgeOne->start_pos()).getValue() - maxPoint.getValue())/2));

			if (objectdeleted.getValue() == false)
				this->visible.setValue(SO_SWITCH_ALL);
		}
		else
    {
		  std::cout << "can't create plane" << std::endl;
      this->visible.setValue(SO_SWITCH_NONE);
    }
  }
  else 
  {
    std::cout << "SoPlaneKit: Error can't creat plane defined by two line" << std::endl;
    this->visible.setValue(SO_SWITCH_NONE);
  }


}

SoSFVec3f 
SoPlaneKit::PointmaxDistance(SbVec3f* VertexArray, int NumVertices, SPAposition Point)
{
	// search the point of VertexArray who has max distance
	float maxdistance = 0.0;
	SoSFVec3f maxPoint;
	for(int i = 0; i < NumVertices;)
	{
		SPAposition pos(VertexArray[i].getValue()[0], VertexArray[i].getValue()[1], VertexArray[i].getValue()[2]);
		if (maxdistance < distance_to_point(Point, pos))
		{
			maxdistance = float(distance_to_point(Point, pos));
			maxPoint.setValue(VertexArray[i].getValue()[0], 
							VertexArray[i].getValue()[1], 
							VertexArray[i].getValue()[2]);
		}
		i++;
	}
	return maxPoint;
}

SoSFVec3f 
SoPlaneKit::GetcentrePoint(SPAposition Point1, SPAposition Point2, SPAposition Point3)
{
	// get centre of the 3 Points
	SPAvector centrePoint;
	centrePoint.set_x(Point1.x() + Point2.x() + Point3.x());
	centrePoint.set_y(Point1.y() + Point2.y() + Point3.y());
	centrePoint.set_z(Point1.z() + Point2.z() + Point3.z());
	centrePoint = centrePoint / 3;
	SoSFVec3f centre;
	centre.setValue(float(centrePoint.x()), float(centrePoint.y()), float(centrePoint.z()));
	return centre;
}

float 
SoPlaneKit::GetmaxDistancePointtoPoints(SoSFVec3f Center, SPAposition Point1, SPAposition Point2, SPAposition Point3)
{
	double distance;
	
	SPAposition center;
	center.set_x(Center.getValue()[0]);
	center.set_y(Center.getValue()[1]);
	center.set_z(Center.getValue()[2]);
	distance = distance_to_point(center, Point1);
	if (distance < distance_to_point(center, Point2)) 
		distance = distance_to_point(center, Point2);
	if (distance < distance_to_point(center, Point3)) 
		distance = distance_to_point(center, Point3);
	
	//std::cout << "distance: " << distance << std::endl;
	return float(distance);
}

int 
SoPlaneKit::FacePlane(double planeSize, SPAvector normalvector, SPAposition Point, SbVec3f* &VertexArray)
{
	FACE* a_Faces;
	int NumVertices = 0;
	
	if (planeSize < minPlaneSize) 
		planeSize=minPlaneSize;
	
	s_return = api_face_plane(Point,planeSize,planeSize,&normalvector,a_Faces,NULL);
	
	if (s_return.ok())
	{
		// SbVec3f *VertexArray = 0;
		int NumOfTriangles = Triangulate(a_Faces, VertexArray);
		
		//    std::cout << "num of Triangles: " << NumOfTriangles <<std::endl;
		NumVertices = NumOfTriangles * 3;
		//    std::cout << "num of Vertices: " << NumVertices <<std::endl;
		if(VertexArray && NumVertices > 0)
		{
			int *FaceDescriptor = new int[NumOfTriangles];
			for(int i = 0; i < NumOfTriangles; ++i)
				FaceDescriptor[i] = 3;
			
			SoVertexProperty *VertexProperty = (SoVertexProperty*)getPart("objectVertexProperty", true);
			VertexProperty->vertex.setValues(0, NumVertices, VertexArray);
			
			SoFaceSet *myFaceSet = (SoFaceSet*)getPart("object", true);
			myFaceSet->numVertices.deleteValues(0);
			myFaceSet->numVertices.setValues(0, NumOfTriangles, FaceDescriptor);
			delete[](FaceDescriptor);
		}
	}
	else 
		SoVertexProperty *VertexProperty = (SoVertexProperty*)getPart("objectVertexProperty", true);
	
	return NumVertices;
}

void SoPlaneKit::FacePlane(SPAvector normalvector, SPAposition Point1, SPAposition Point2, SPAposition Point3,  SbVec3f* &VertexArray)
{
	FACE* a_Faces;
	int NumVertices = 3;
  int NumOfTriangles = 1;
  VertexArray = new SbVec3f[3];
	
	s_return = api_face_plane(Point1,minPlaneSize,minPlaneSize,&normalvector,a_Faces,NULL);
	
	if (s_return.ok())
	{
		if(VertexArray)
		{
			int *FaceDescriptor = new int[NumOfTriangles];
			FaceDescriptor[0] = 3;
			
      VertexArray[0].setValue((float)Point1.x(), (float)Point1.y(), (float)Point1.z());
      VertexArray[1].setValue((float)Point2.x(), (float)Point2.y(), (float)Point2.z());
      VertexArray[2].setValue((float)Point3.x(), (float)Point3.y(), (float)Point3.z());

			SoVertexProperty *VertexProperty = (SoVertexProperty*)getPart("objectVertexProperty", true);
			VertexProperty->vertex.setValues(0, 3, VertexArray);
			
			SoFaceSet *myFaceSet = (SoFaceSet*)getPart("object", true);
			myFaceSet->numVertices.deleteValues(0);
			myFaceSet->numVertices.setValues(0, NumOfTriangles, FaceDescriptor);
			delete[](FaceDescriptor);
		}
	}
	else 
		SoVertexProperty *VertexProperty = (SoVertexProperty*)getPart("objectVertexProperty", true);
}

FACE* SoPlaneKit::GetPlaneFace(int normal)
{
	/*
	param_info& ent_info=*(param_info*)NULL_REF;
	double line_param;
	SPAposition point(-100,-100,-100);
	SPAposition intPoint;
	EDGE *line;
	api_mk_ed_line(point, point + normalise(a_Plane->normal())*100, line);
  
	int param = intersect_line_plane(point,normalise(a_Plane->normal()), a_Plane->root_point(), normalise(a_Plane->normal()), line_param, intPoint);
	  
	FACE* Faces;
	ENTITY_LIST *intPs = new ENTITY_LIST;
	intPs->init();

	s_return = api_face_plane(intPoint,200,200,&normalise(a_Plane->normal()),Faces,NULL);	

	if (s_return.ok())
		return Faces;
	else
		return NULL;
  */

	//fastest method, just set PLANE (derived from SURFACE) as the FACE geometry
  PLANE* Plane = new PLANE(a_Plane->root_point(),a_Plane->normal());
	FACE* Faces = new FACE;
	Faces->set_geometry(Plane); 

	return Faces;
}

void
SoPlaneKit::OnChange()
{
	if (triggerAllowed)
	{
		printf("Updating plane \n");
		UpdateAcisObject();
	}
	triggerAllowed = FALSE;
}

SbBool SoPlaneKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) {
		// We connect AFTER base class.
		//SoBaseKit::setUpConnections(onOff, doItAlways);
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		// do your own special setup here !
		// connect fields
		SbName OneName = PointOneName.getValue();
		SbName TwoName = PointTwoName.getValue();
		SbName ThreeName = PointThreeName.getValue();
		SoNode *objectOne = NULL;
		SoNode *objectTwo = NULL;
		SoNode *objectThree = NULL;
		SoNode *objectFour = NULL;
		SoNode *objectFive = NULL;
		SoNode *objectSix = NULL;
		SoNode *objectSeven = NULL;
		SbBool validObject = FALSE;
		
		if(OneName != "")
		{
			objectOne = SoNode::getByName(OneName);
			objectOneSensor->attach(&((SoPointKit*)objectOne)->pCoord);
			validObject = TRUE;
		}
		if(TwoName != "")
		{
			objectTwo = SoNode::getByName(TwoName);
			objectTwoSensor->attach(&((SoPointKit*)objectTwo)->pCoord);
			validObject = TRUE;
		}
		if(ThreeName != "")
		{
			objectThree = SoNode::getByName(ThreeName);
			objectThreeSensor->attach(&((SoPointKit*)objectThree)->pCoord);
			validObject = TRUE;
		}
		if(SphereOneName.getValue() != "")
		{
			objectFour = SoNode::getByName(SphereOneName.getValue());
			objectFourSensor->attach(&((SoSphereKit*)objectFour)->ChangeObject);
			validObject = TRUE;
		}
		if(SphereTwoName.getValue() != "")
		{
			objectFive = SoNode::getByName(SphereTwoName.getValue());
			objectFiveSensor->attach(&((SoSphereKit*)objectFive)->ChangeObject);
			validObject = TRUE;
		}
		if(LineOneName.getValue() != "")
		{
			objectSix = SoNode::getByName(LineOneName.getValue());
			objectSixSensor->attach(&((SoLineKit*)objectSix)->ChangeObject);
			validObject = TRUE;
		}
		if(LineTwoName.getValue() != "")
		{
			objectSeven = SoNode::getByName(LineTwoName.getValue());
			objectSevenSensor->attach(&((SoLineKit*)objectSeven)->ChangeObject);
			validObject = TRUE;
		}
    
		if (validObject)
			UpdateAcisObject();
	}
	else {
		// We disconnect BEFORE base class.
		// Disconnect the field sensors.
		Object3dKit::setUpConnections(onOff, doItAlways);
		
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
		if (objectSixSensor)
			objectSixSensor->detach();
		if (objectSevenSensor)
			objectSevenSensor->detach();
	}
	return !(connectionsSetUp = onOff); 
}

int SoPlaneKit::Triangulate(FACE* p_Face, SbVec3f* &p_VertexArray)
{
	
	int NumTriangle = 0;
	int ArrayPos = 0;
	
	REFINEMENT *ref = new REFINEMENT;
	ref->set_surf_mode(AF_SURF_ALL);
	ref->set_adjust_mode(AF_ADJUST_NONE);
	ref->set_triang_mode(AF_TRIANG_ALL);
	ref->set_surface_tol(25);
	ref->set_normal_tol(25);
	api_set_default_refinement(ref);
	
	parameter_token ptoken[2];
	ptoken[0] = POSITION_TOKEN;
	ptoken[1] = NORMAL_TOKEN;
	
	while (p_Face)
	{
		POLYGON_POINT_MESH *facets = (POLYGON_POINT_MESH *)NULL;
		a_return = api_facet_face(p_Face);
		a_return = api_get_face_facets(p_Face,facets);
		if (!a_return.ok()) 
		{
			std::cout << "Error: no Boolean Object" <<std::endl;
			p_Face = NULL;
		}
		
		POLYGON *poly;
		POLYGON_VERTEX *poly_vtx;
		SPAposition vtx_pos;
		
		for(poly=facets->first(); poly != NULL; poly = poly->next())
		{
			NumTriangle++;
			
			// get new array
			SbVec3f *oldArray = p_VertexArray;
			p_VertexArray = new SbVec3f[(NumTriangle)*3];
			if(!p_VertexArray)
				return(0);
			
			// copy old array to new one
			for(int i = 0; i < (NumTriangle-1) * 3;)
			{
				p_VertexArray[i] = oldArray[i];
				i++;
			}
			if(oldArray)
				delete[](oldArray);
			
			for(poly_vtx = poly->first(); poly_vtx != NULL; poly_vtx = poly_vtx->next())
			{
				poly_vtx->point(vtx_pos);
				// Fill our vertex array
				p_VertexArray[ArrayPos++].setValue((float)vtx_pos.x(), (float)vtx_pos.y(), (float)vtx_pos.z());
			}
		}
		ACIS_DELETE(facets);
		ACIS_DELETE(poly);
//		poly_vtx->destroy(NULL);

		p_Face = p_Face->next_in_list();
	}
	return(NumTriangle);
}

void
SoPlaneKit::caltangentPlanebetweentwoSphere()
{
	SoSphereKit* SphereOne = (SoSphereKit*)getObject(SphereOneName.getValue());
	SoSphereKit* SphereTwo = (SoSphereKit*)getObject(SphereTwoName.getValue());
	SoPointKit* Point = (SoPointKit*)getObject(PointOneName.getValue());
	
	//calc the normal of plane
	//the centers of the two spheres and the point 
	SPAvector a = SPAvector(convertSoSFVec3f(SphereOne->sphereCenter.getValue()).x(), 
						convertSoSFVec3f(SphereOne->sphereCenter.getValue()).y(), 
						convertSoSFVec3f(SphereOne->sphereCenter.getValue()).z()) - SPAvector(Point->a_Point.x(), 
						Point->a_Point.y(), Point->a_Point.z());
	SPAvector b = SPAvector(convertSoSFVec3f(SphereTwo->sphereCenter.getValue()).x(), 
						convertSoSFVec3f(SphereTwo->sphereCenter.getValue()).z(), 
						convertSoSFVec3f(SphereTwo->sphereCenter.getValue()).z()) - SPAvector(Point->a_Point.x(), 
						Point->a_Point.y(), Point->a_Point.z());
	
	SPAvector norm;
	norm.set_x(a.y()*b.z() - a.z()*b.y());
	norm.set_y(a.z()*b.x() - a.x()*b.z());
	norm.set_z(a.x()*b.y() - a.y()*b.x());
	
	//intersect the spheres with the plane => two circles
	BODY* curveOne = NULL;
	BODY* curveTwo = NULL;
	s_return = api_planar_slice(SphereOne->getBody(), convertSoSFVec3f(SphereOne->sphereCenter.getValue()), 
								normalise(norm), curveOne, NULL);
	s_return = api_planar_slice(SphereTwo->getBody(), convertSoSFVec3f(SphereTwo->sphereCenter.getValue()), 
								normalise(norm), curveTwo, NULL);
	
	if (s_return.ok())
	{    
		//cal the tangent between the two circles => two points each on Sphere
		ENTITY *entity1=NULL, *entity2=NULL;
		SPAposition* position_1 = NULL; SPAposition* position_2 = NULL;
		entity_with_ray* entity_with_ray_1 = NULL; entity_with_ray* entity_with_ray_2 = NULL;
		
		//cal. point on intersectioncurve, 
		SPAposition pos1;
		double dis_point_axis;
		api_entity_point_distance((ENTITY*)curveOne->lump()->shell(), SPAposition(100,100,100), pos1, dis_point_axis, *(param_info*)NULL_REF);
		SPAposition pos2;
		api_entity_point_distance((ENTITY*)curveTwo->lump()->shell(), SPAposition(-100,-100,-100), pos2, dis_point_axis, *(param_info*)NULL_REF);
		
		//create two new circle, because acis can't cal. the tangent between two intersection curve
		EDGE* circleOne;
		s_return = api_mk_ed_ellipse(convertSoSFVec3f(SphereOne->sphereCenter.getValue()), normalise(norm), pos1 - convertSoSFVec3f(SphereOne->sphereCenter.getValue()),1.0,0,0,circleOne,NULL);
		EDGE* circleTwo;
		s_return = api_mk_ed_ellipse(convertSoSFVec3f(SphereTwo->sphereCenter.getValue()), normalise(norm), pos2 - convertSoSFVec3f(SphereTwo->sphereCenter.getValue()),1.0,0,0,circleTwo,NULL);
		
		bool crosswise = false;
		SPAunit_vector unitvecOne(0,0,0);
		SPAunit_vector unitvecTwo(0,0,0);
		EDGE* Line=NULL;
		int index = 0;
		
		srand( (unsigned)time( NULL ) );
		do
		{
			if ((TangentPointOneName.getValue() != "") && (TangentPointTwoName.getValue() != "") && crosswise)  //for update
			{
				SoPointKit *Point_1 = (SoPointKit*)SoNode::getByName(TangentPointOneName.getValue());
				SoPointKit *Point_2 = (SoPointKit*)SoNode::getByName(TangentPointTwoName.getValue());
				entity_with_ray_1 = &entity_with_ray((ENTITY*)circleOne, Point_1->a_Point, normalise(SPAvector(Point_2->a_Point - Point_1->a_Point))); 
				entity_with_ray_2 = &entity_with_ray((ENTITY*)circleTwo, Point_2->a_Point, normalise(SPAvector(Point_2->a_Point - Point_1->a_Point)));
			}
			else //first time
			{
				entity_with_ray_1 = &entity_with_ray((ENTITY*)circleOne, pos1, unitvecOne);
				entity_with_ray_2 = &entity_with_ray((ENTITY*)circleTwo, pos2, unitvecTwo);
			}
			
			s_return = api_curve_line_tangent(position_1, entity_with_ray_1, position_2, entity_with_ray_2, Line, NULL);
			
			SPAposition pointone;
			SPAposition pointtwo;
			double dis;
			//cal nearest point
			Line_Plane_Intersect(Line->start_pos(), convertSoSFVec3f(SphereOne->sphereCenter.getValue()), convertSoSFVec3f(SphereTwo->sphereCenter.getValue()), pointone, dis);
			Line_Plane_Intersect(Line->end_pos(), convertSoSFVec3f(SphereOne->sphereCenter.getValue()), convertSoSFVec3f(SphereTwo->sphereCenter.getValue()), pointtwo, dis);
			//cal vector nearest point tangent point
			SPAvector vecOne(Line->start_pos() - pointone);
			SPAvector vecTwo(Line->end_pos() - pointtwo);
			
			//      std::cout << "pos1: " << pos1.x() << " , " << pos1.y() << " , " << pos1.z() << std::endl;
			//      std::cout << "pos2: " << pos2.x() << " , " << pos2.y() << " , " << pos2.z() << std::endl;
			
			//test if the plane is crosswise
			if ( (((vecOne.y() >= 0) && (vecTwo.y() <= 0)) || ((vecOne.y() <= 0) && (vecTwo.y() >= 0)))
				&& (((vecOne.z() >= 0) && (vecTwo.z() <= 0)) || ((vecOne.z() <= 0) && (vecTwo.z() >= 0))) )
			{
				crosswise = true;
			}
			else
			{
				//if not crosswise
				if ((TangentPointOneName.getValue() != "") && (TangentPointTwoName.getValue() != ""))  //for update
				{
					SoPointKit *Point_1 = (SoPointKit*)SoNode::getByName(TangentPointOneName.getValue());
					SoPointKit *Point_2 = (SoPointKit*)SoNode::getByName(TangentPointTwoName.getValue());
					
					//mirror one point of contact 
					EDGE* axis;
					s_return = api_mk_ed_line(convertSoSFVec3f(SphereOne->sphereCenter.getValue()), convertSoSFVec3f(SphereTwo->sphereCenter.getValue()), axis);
					api_entity_point_distance((ENTITY*)axis, Point_1->a_Point, pointone, dis, *(param_info*)NULL_REF, NULL);
					pos1 = pointone + (pointone - Point_1->a_Point);
					
					//set the vector of entity_with_ray_1 and entity_with_ray_2
					unitvecOne = normalise(SPAvector(convertSoSFVec3f(SphereTwo->sphereCenter.getValue()) - Point_1->a_Point) * rand());
					unitvecTwo = normalise(SPAvector(convertSoSFVec3f(SphereOne->sphereCenter.getValue()) - Point_2->a_Point) * rand());
				}
				crosswise = false;
				index++;
			}
		}
		while(!crosswise && (index < 10));
		
		if (s_return.ok() && Line)
		{
			//cal the normalvector of the tangent plane
			SPAvector tangnorm = convertSoSFVec3f(SphereOne->sphereCenter.getValue()) - Line->start_pos();
			
			//center of the tangential line
			SoSFVec3f lineCenter = convertPosition(Line->start_pos() + (Line->end_pos() - Line->start_pos()) / 2);
			
			//create the plane
			a_Plane = new PLANE(convertSoSFVec3f(lineCenter.getValue()), normalise(tangnorm));
			
			SbVec3f *VertexArray = 0;
			int NumVertices = FacePlane(tangentplaneSize, a_Plane->normal(), convertSoSFVec3f(lineCenter.getValue()), VertexArray);
			
			if (s_return.ok())
			{
				// search the point of VertexArray who has max distance
				SoSFVec3f maxPoint = PointmaxDistance(VertexArray, NumVertices, convertSoSFVec3f(lineCenter.getValue()));
				
				translation.setValue( - (maxPoint.getValue() - (lineCenter.getValue() +  (maxPoint.getValue()-lineCenter.getValue())/2) ));
			}
			
			//create a tangent point on sphere one
			if ((TangentPointOneName.getValue() == ""))
			{
				SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
				SoPointKit *P = new SoPointKit(objSum->getValue(), convertPosition(Line->start_pos()).getValue(), SphereOneName.getValue(), this->userID.getValue());
				P->addDependentObjectsName(this->getName());
				P->translationOn.setValue(FALSE);
				P->rotationOn.setValue(FALSE);
				selectionRoot->addChild(P);
				TangentPointOneName.setValue(P->getName().getString());
				objSum->setValue(objSum->getValue()+1);
			}
			else //update point position
			{
				SoPointKit *Point = (SoPointKit*)SoNode::getByName(TangentPointOneName.getValue());
				if (Line->start_pos() != Point->a_Point)
					Point->Update(convertPosition(Line->start_pos()).getValue());
			}
			
			//create a tangent point on sphere two
			if ((TangentPointTwoName.getValue() == ""))
			{
				SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
				SoPointKit *P = new SoPointKit(objSum->getValue(), convertPosition(Line->end_pos()).getValue(), SphereTwoName.getValue(), this->userID.getValue());
				P->addDependentObjectsName(this->getName());
				P->translationOn.setValue(FALSE);
				P->rotationOn.setValue(FALSE);
				selectionRoot->addChild(P);
				TangentPointTwoName.setValue(P->getName().getString());
				objSum->setValue(objSum->getValue()+1);
			}
			else //update point position
			{
				SoPointKit *Point = (SoPointKit*)SoNode::getByName(TangentPointTwoName.getValue());
				if (Line->end_pos() != Point->a_Point)
					Point->Update(convertPosition(Line->end_pos()).getValue());
			}
		}
		else 
			std::cout << "can't not create tangentplane between two spheres" << std::endl;
  }
  else 
	  std::cout << "can't not create tangentplane between two spheres" << std::endl;
  
}

void 
SoPlaneKit::setHighlightMaterial(int currentUserID)
{
	SoMaterial* dummy1 = new SoMaterial;
	SoMaterial* dummy2 = new SoMaterial;
	SoMaterial* dummy3 = new SoMaterial;
	SoMaterial* dummy4 = new SoMaterial;
	MaterialConstants::definePlaneMaterials(*W, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}

void SoPlaneKit::Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum)
{
	int counter = 0; 
	int objNumber = childNumber.getValue();
	
	if (TangentPointTwoName.getValue() != "")
	{
		counter++;
		SoPointKit *object = (SoPointKit*)SoNode::getByName(TangentPointTwoName.getValue());
		object->disconnectChild(this);
		if (object->ObjectName.getValue() != "")
		{
			Object3dKit *obj = (Object3dKit*)SoNode::getByName(object->ObjectName.getValue());
			if(obj)
			{
				obj->disconnectChild(object);
			}
		}
		object->Undo(SelectionRoot, ObjSum);
	} 
	
	if (TangentPointOneName.getValue() != "")
	{
		counter++;
		SoPointKit *object = (SoPointKit*)SoNode::getByName(TangentPointOneName.getValue());
		object->disconnectChild(this);
		if (object->ObjectName.getValue() != "")
		{
			Object3dKit *obj = (Object3dKit*)SoNode::getByName(object->ObjectName.getValue());
			if(obj)
			{
				obj->disconnectChild(object);
			}
		}
		object->Undo(SelectionRoot, ObjSum);
	}
	
	RemoveAllCallbacks();
	
	//SoSeparator *looknode = (SoSeparator*) SelectionRoot->getChild(objNumber);
	//SelectionRoot->removeChild((SoSeparator*)looknode);// removes whole seperator node
	SelectionRoot->removeChild(this);// removes whole LineKit
	ObjSum->setValue(ObjSum->getValue() - 1);
}