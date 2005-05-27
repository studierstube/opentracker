// ===========================================================================
//  (C) 2000-2003 Vienna University of Technology
// ===========================================================================
//  NAME:       SoCurveKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Construct3d
//  CONTENT:    Curves implementation
//  VERSION:    1.x
// ===========================================================================
//  This is closed source
// ===========================================================================
//  AUTHOR:    kl      Klaus Feiler
//			   hk	   Hannes Kaufmann
// ===========================================================================
//  HISTORY:   Time of development: Dezember 2002 - 
//
// ===========================================================================

#include "SoCurveKit.h"
#include "transf.hxx"

#include "MaterialConstants.h"

SO_KIT_SOURCE(SoCurveKit);

void SoCurveKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoCurveKit, Object3dKit, "Object3dKit");
} 

SoCurveKit::SoCurveKit() 
{ 
  initSoCurveKit();
  
  Build();  // Build Curve
  if (s_return.ok() || (CurveOp.getValue() == CURVE_INTERSECTION))
  {
    CreateSensors(); 
    setUpConnections(TRUE,TRUE);
  }
}

SoCurveKit::SoCurveKit(int number, std::vector<Object3dKit*> points, enumCurveOp type, int numofPoints, int Degree, int userID)
{
  initSoCurveKit();
  
  childNumber.setValue(number);
  lineThickness.setValue(3);
  setNodeName("C", number);
  CurveOp.setValue(type); 
  this->userID.setValue(userID);
  MaterialConstants::defineLineMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);
  
  Points = points;
  NumofPoints = numofPoints;
  degree = Degree;
  
  pointsName.setNum(NumofPoints.getValue());
  
  for(int i=0; i<NumofPoints.getValue(); i++)
  {
    pointsName.set1(i,Points[i]->getName().getString());
  }
  
  Build();  // Build Curve
  if (s_return.ok() || (CurveOp.getValue() == CURVE_INTERSECTION))
  {
    CreateSensors(); 
    setUpConnections(TRUE,TRUE);
  } 
}

SoCurveKit::SoCurveKit(int number, std::vector<int> objectList, enumCurveOp type, int Degree, int userID)
{
  initSoCurveKit();
  
  childNumber.setValue(number);
  lineThickness.setValue(3);
  setNodeName("C", number);
  CurveOp.setValue(type); 
  this->userID.setValue(userID);
  MaterialConstants::defineLineMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);
  
  s_return = outcome(-1,NULL); 
  
  unsigned int pointcount = 0;
  unsigned int curvecount = 0;
  unsigned int objectcount = 0;
  unsigned int linecount = 0;
  int linepos = -1;
  while  ( (objectcount < objectList.size()) &&
    ((getObject(objectList[objectcount]))->isOfType(SoCurveKit::getClassTypeId()) || 
    (getObject(objectList[objectcount]))->isOfType(SoPointKit::getClassTypeId()) || 
    ((getObject(objectList[objectcount]))->isOfType(SoLineKit::getClassTypeId()) && (type == CIRCLE))) )
  {
    if( (getObject(objectList[objectcount]))->isOfType(SoCurveKit::getClassTypeId()) )
      curvecount++;
    if( (getObject(objectList[objectcount]))->isOfType(SoPointKit::getClassTypeId()) )
      pointcount++;
    if( (getObject(objectList[objectcount]))->isOfType(SoLineKit::getClassTypeId()) )
    {
      linecount++;
      if (linepos == -1)
        linepos = objectcount; //save the position of line object
    }
    objectcount++;
  }

  // circle define by on line as normal and two point. 
  // first point is the midpoint second is the radius  
  // or circle define by on line as normal and one point. 
  if ((linecount > 0) && (type == CIRCLE) && (pointcount >= 1)) 
  {                                                       
    LineName.setValue(getObject(objectList[linepos])->getName());
    if (pointcount > 2)
      NumofPoints.setValue(3);
    else 
      NumofPoints.setValue(pointcount + 1);
    
    degree = Degree;
    pointsName.setNum(NumofPoints.getValue() - 1);
    
    int counter = 0;
    int pos=0;
    while (counter < NumofPoints.getValue())
    {
      if (counter != linepos) //jump over the line
      {
        pointsName.set1(pos,getObject(objectList[counter])->getName().getString());
        pos++;
      }
      counter++;
    }
    
    Build();  // Build Curve
    if (s_return.ok())
    {
      //only deselect points which are really involved in the curve
      addDependentObjects(objectList, NumofPoints.getValue());
      deselectObjects(objectList,NumofPoints.getValue());
      createObjectList(objectList, NumofPoints.getValue());
      
      CreateSensors();
      setUpConnections(TRUE,TRUE);
    } 
  }
  else if (pointcount >= 2)
  {
    //circle and ellipse need only the first three points
    if ((type != CURVE_POINTS_ON) && (type != CURVE_BEZIER))
    {
      //check if two points in a plane
      SoPointKit *obj1 = (SoPointKit*)getObject(objectList[0]);
      SoPointKit *obj2 = (SoPointKit*)getObject(objectList[1]);
	  SbName planeName;

	  // check if a plane is selected already
	  // if a plane is selected, we check if both points lie in the selected
	  // plane. If they do, we will use this plane as the base plane for the
	  // circle.
	  if (objectList.size() > 2)
	  {
		int i;
		for (i = 0; i < objectList.size(); i++)
		{
			if (getObject(objectList[i])->isOfType(SoPlaneKit::getClassTypeId()))
			{
				// check if points lie on plane. could also use the ACIS
				// function is_on_plane for the check with a probably smaller epsilon
				PLANE* plane1 = ((SoPlaneKit*)getObject(objectList[i]))->a_Plane;
				double dist1 = distance_to_plane(obj1->a_Point, plane1->root_point(), plane1->normal());
				double dist2 = distance_to_plane(obj2->a_Point, plane1->root_point(), plane1->normal());
				if ((dist1 < eps) && (dist2 < eps))
				{
					// important to assign both variables. only PlaneName is "global"
					PlaneName = getObject(objectList[i])->getName();
					planeName = getObject(objectList[i])->getName();
//					printf("PlaneName %s \n",planeName.getString());
				}
				else
					printf("Circle: Points do not lie in the selected plane.\n");
			}
		}
	  }
	  else
	  {
		planeName = ComparetwoObjectsNameList((obj1->NamesofDependentObjects("Plane")), (obj2->NamesofDependentObjects("Plane")));
		PlaneName.setValue(planeName);
	  }
/*	  printf("planeName %s \n",planeName.getString());*/
      if (planeName != "")
      {
        if (pointcount == 2)
          NumofPoints.setValue(2);
        else if (pointcount >= 3)       // there can also be 3 or more points in a plane
          NumofPoints.setValue(3);
      }
      else
      {
        if (pointcount == 2)
          return;		// do not increase pointcount and do not continue. 
        // example: If only 2 points are drawn and selected, 
        // a circle which is not in a plane cannot be generated. 3 points would be required.
        else
          NumofPoints.setValue(3);   // only 3 points are needed for a circle and ellipse in general 
      }
    }
    else
      NumofPoints = pointcount;
    
    degree = Degree;
    pointsName.setNum(NumofPoints.getValue());
    
    for(int i=0; i < NumofPoints.getValue(); i++)
    {
      pointsName.set1(i,getObject(objectList[i])->getName().getString());
    }
    
    Build();  // Build Curve
    if (s_return.ok())
    {
      //only deselect points which are really involved in the curve
      addDependentObjects(objectList, NumofPoints.getValue());
      deselectObjects(objectList,NumofPoints.getValue());
      createObjectList(objectList, NumofPoints.getValue());
      
      CreateSensors();
      setUpConnections(TRUE,TRUE);
    } 
  }
}

SoCurveKit::SoCurveKit(int number, BODY* intCurve, BODY* intCurve_todraw, SbName transObjName, SoNode* interObj, SbName intObjName, int userID)
{
  initSoCurveKit();
  
  childNumber.setValue(number);
  lineThickness.setValue(4.0);
  setNodeName("C", number);
  this->userID.setValue(userID);
  //MaterialConstants::defineIntersectionCurveMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  MaterialConstants::defineLineMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);
  
  CurveOp.setValue(CURVE_INTERSECTION); 
  
  intersectObjectName.setValue(intObjName);
  intersectObject = interObj;
  
  translationObjectName.setValue(transObjName);
  
  a_intCurve = intCurve;
  a_intCurve_todraw = intCurve_todraw;
  Facet_intCurve();
  //Build();  // Build Curve
  if (s_return.ok() || (CurveOp.getValue() == CURVE_INTERSECTION))
  {
    CreateSensors(); 
    setUpConnections(TRUE,TRUE);
  }
}

void
SoCurveKit::CreateSensors()
{
  if (CurveOp.getValue() == CURVE_INTERSECTION)
  {
    ObjectSensor[0] = new SoFieldSensor(nameUpdate, this);
    ObjectSensor[0]->setPriority(0);
    SensorsCreated = TRUE;
    
    SoIntersectKit *obj = (SoIntersectKit*)SoNode::getByName(intersectObjectName.getValue()); //intersectObject;
    printf("CurveKit: Createsensors: intersectobjectname %s \n",intersectObjectName.getValue().getString());
    if(obj)
    {
      std::cout << "curvekit create sensors; int obj ist da" << std::endl;
      obj->connectChild(this);
    }
    else
      std::cout << "curvekit create sensors; noch kein int obj" << std::endl;
    
  }
  else if(LineName.getValue() != "")
  {
    if (!SensorsCreated)
    {
      ObjectSensor[0] = new SoFieldSensor(nameUpdate, this);
      ObjectSensor[0]->setPriority(0);
      SensorsCreated = TRUE;
      
      getObject(LineName.getValue())->connectChild(this);
      
      for (int i=1; i < NumofPoints.getValue(); i++)
      {
        //        SbString ObjName;
        //        pointsName.get1(i,ObjName);
        Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i-1]);
        if(obj)
        {
          printf("Creating sensor %d \n",i);
          ObjectSensor[i] = new SoFieldSensor(nameUpdate, this);
          ObjectSensor[i]->setPriority(0);
          SensorsCreated = TRUE;
          obj->connectChild(this);
        }
      }
    }
  }
  else
  {
    if (!SensorsCreated)
    {
      if (PlaneName.getValue() != "")
      {
		PlaneSensor = new SoFieldSensor(nameUpdate, this);
        PlaneSensor->setPriority(0);
        SensorsCreated = TRUE;
        
        getObject(PlaneName.getValue())->connectChild(this);
      }
      for (int i=0; i < NumofPoints.getValue(); i++)
      {
        //        SbString ObjName;
        //        pointsName.get1(i,ObjName);
        Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
        if(obj)
        {
          printf("Creating sensor %d \n",i);
          ObjectSensor[i] = new SoFieldSensor(nameUpdate, this);
          ObjectSensor[i]->setPriority(0);
          SensorsCreated = TRUE;
          obj->connectChild(this);
        }
      }
    }
  }
}
// init all parts and fields
void
SoCurveKit::initSoCurveKit()
{
  SO_KIT_CONSTRUCTOR(SoCurveKit); 
  // Add the catalog parts:
  //-----------------------name--|---type------|nuldef|  par   |rite|public
  SO_KIT_ADD_CATALOG_ENTRY(objectVertexProperty,SoVertexProperty,FALSE,switchNode,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(object,SoLineSet,   FALSE,switchNode,\x0,TRUE);
  
  // for the line between the controlpoints 
  SO_KIT_ADD_CATALOG_ENTRY(objectVertexProperty_c,SoVertexProperty,FALSE,switchNode,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(style_c,SoDrawStyle,  FALSE,switchNode,\x0,TRUE);  // not needed anymore
  SO_KIT_ADD_CATALOG_ENTRY(object_c,SoLineSet,   FALSE,switchNode,\x0,TRUE);  
  
  //fields
  SO_NODE_ADD_FIELD(pointsName, (""));
  SO_NODE_ADD_FIELD(intersectObjectName, (""));
  SO_NODE_ADD_FIELD(translationObjectName, (""));
  SO_NODE_ADD_FIELD(LineName, (""));
  SO_NODE_ADD_FIELD(PlaneName, (""));
  SO_NODE_ADD_FIELD(NumofPoints, (0));
  SO_NODE_ADD_FIELD(degree, (0));
  SO_NODE_ADD_FIELD(CurveOp, (0));
  SO_NODE_ADD_FIELD(lineThickness, (3.0));
  
  // Initialize this SoCurveKit
  SO_KIT_INIT_INSTANCE();
  
  //turn dragging temporarily off
  translationOn.setValue(FALSE);
  rotationOn.setValue(FALSE);
  
  name.setValue("Curve");
  
  ObjectSensor.reserve(maxNumofPoints);
  
  a_Curve = NULL;
  a_intCurve = NULL;
  a_intCurve_todraw = NULL;
  PlaneSensor = NULL;
  
  // connect fields in setUpConnections()
  
  UpdateCurveObject = false;
  SensorsCreated = FALSE;
  triggerAllowed = FALSE;
}

// Destructor
SoCurveKit::~SoCurveKit()
{
  printf("DESTRUCTOR CURVE---------------------------\n");
  if (CurveOp.getValue() == CURVE_INTERSECTION)
  {
    if (ObjectSensor[0])
      ObjectSensor[0]->detach();
  }
  else if (LineName.getValue() != "")
  {
    for(int i=0; i < NumofPoints.getValue() - 1; i++)
    {
      if (pointsName[i] != "")
      {
        Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
        if(obj)
        {
          obj->disconnectChild(this);
        }
        // if s_return was FALSE, no sensors were created and deleting sensors 
        // would crash here
        if (SensorsCreated)
          if (ObjectSensor[i + 1])
          {
            ObjectSensor[i + 1]->detach();
            printf("removing sensor %d \n",i);
          }
      }
    }
    if (getObject(LineName.getValue()))
      getObject(LineName.getValue())->disconnectChild(this);
    if (SensorsCreated)
      if (ObjectSensor[0])
        ObjectSensor[0]->detach();
  }
  else
  {
    if (PlaneName.getValue() != "")
    {
      if (getObject(PlaneName.getValue()))
        getObject(PlaneName.getValue())->disconnectChild(this);
      if(PlaneSensor)
        PlaneSensor->detach();
    }
    
    for(int i=0; i<NumofPoints.getValue(); i++)
    {
      if (pointsName[i] != "")
      {
        Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
        if(obj)
        {
          obj->disconnectChild(this);
        }
        // if s_return was FALSE, no sensors were created and deleting sensors 
        // would crash here
        if (SensorsCreated)
          if (ObjectSensor[i])
          {
            ObjectSensor[i]->detach();
            printf("removing sensor %d \n",i);
          }
      }
    }
  }
  RemoveAllCallbacks();
  api_del_entity(a_Curve);
}

// Disadvantage	: Radius point on Sphere can be selected 
// because points have a priority 
float
SoCurveKit::getDistance(SbVec3f penPosition)
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
    ENTITY* entity = (ENTITY*)getEntity();
    
    api_entity_point_distance(entity, pen_point, Point, min_distance, ent_info);
    if (CurveOp.getValue() == CURVE_INTERSECTION)
      //      min_distance = min_distance/10;
      min_distance = min_distance - IntersectCurvePriorityEpsilon;
  }
  //std::cout << "curve: "  << min_distance << std::endl;
  return float(min_distance - IntersectCurvePriorityEpsilon);
}


void
SoCurveKit::nameUpdate(void *data, SoSensor *sensor)
{
  std::cout << "SoCurveKit::nameUpdate(), but no new building of Curve" << std::endl;
  SoCurveKit *self=(SoCurveKit*)data;
  //  self->Build();
  // only allow trigger of update through nameUpdate, not through direct callbacks 
  // of points/objects that are connected to this object
  self->triggerAllowed = TRUE;
  self->triggerChange();
  self->UpdateCurveObject = false;
}

void
SoCurveKit::OnChange()
{
  if (triggerAllowed)
  {
    printf("Curve on change, building new curve\n");
    UpdateAcisObject();
  }
  UpdateCurveObject = true;
  triggerAllowed = FALSE;
}

void
SoCurveKit::Build()
{
  s_return = outcome(-1,NULL); 
  if (CurveOp.getValue() == CIRCLE)
  {
    createCircle();
  }
  if (CurveOp.getValue() == ELLIPSE_CURVE)
  {
    createEllipse();
  }
  if (CurveOp.getValue() == CURVE_BEZIER)
  {
    createCurveBezier(); 
  }  
  if (CurveOp.getValue() == CURVE_POINTS_ON)
  {
    createCurvePointson();
  }
  if (CurveOp.getValue() == CURVE_INTERSECTION)
  {
    createCurveIntersection();
  }
  
  if (s_return.ok() && (CurveOp.getValue() != CURVE_INTERSECTION))
  {
    Facet_Curve();
    add_lines_between_Controlpoints();
  }
  
}

void SoCurveKit::createCircle()
{
  std::cout << "SoCurveKit::createCircle build new curve" << std::endl;
  if(LineName.getValue() != "")
  {
    if (NumofPoints.getValue() == 2)
      createCircledefinebyoneLineandonePoints();
    if (NumofPoints.getValue() >=  3)
      createCircledefinebyoneLineandtwoPoints();
  }
  else if (NumofPoints.getValue() == 2)
  {
    SoPointKit *obj1 = (SoPointKit*)SoNode::getByName(pointsName[0]);
    SoPointKit *obj2 = (SoPointKit*)SoNode::getByName(pointsName[1]);
    
	SbName planeName;
//	printf("createcircle: PlaneName %s \n",PlaneName.getValue().getString());
	// it is difficult to detect here if 2 points lie in a specific plane
	// they do not necessarily have the plane in their list of dependant objects
	// if they do, this plane is detected automatically and does not have to
	// be selected
	// if they don't, we did a check earlier if the points lie in a selected plane
	if (PlaneName.getValue().getString() == "")
	{
		printf("No plane set yet!\n");
		planeName = ComparetwoObjectsNameList((obj1->NamesofDependentObjects("Plane")), (obj2->NamesofDependentObjects("Plane")));
	}
	// if a PlaneName was already set (in the case both points lie in
	// the same plane) we keep this name and use the plane.
	else  
		planeName = PlaneName.getValue();

    if (planeName.getString() != "")
    {
	  PlaneName.setValue(planeName);
      SoPlaneKit *objplane = (SoPlaneKit*)SoNode::getByName(planeName);
      SPAvector majorAxis;
      majorAxis = obj2->a_Point - obj1->a_Point;
//      std::cout << "majorAcis: " << majorAxis.x() << " , " << majorAxis.y() << " , " << majorAxis.z() << std::endl;
//	  printf("Plane Normal %f %f %f \n",objplane->a_Plane->normal().x(), objplane->a_Plane->normal().y(), objplane->a_Plane->normal().z());

	  // ACIS reported in api_mk_ed_ellipse that the orthogonal vectors
	  // objplane->a_Plane->normal() and majorAxis were not orthogonal
	  // - probably numerical problems
	  // therefore we just generate any orthogonal vector to objplane->a_Plane->normal()
	  // and use the length of majorAxis (which is the radius) to trim it
	  // to the correct length
      //s_return = api_mk_ed_ellipse(obj1->a_Point, objplane->a_Plane->normal(), majorAxis,1.0,0,0,a_Curve,NULL);
	  s_return = api_mk_ed_ellipse(obj1->a_Point, objplane->a_Plane->normal(), 
								objplane->a_Plane->normal().orthogonal()*majorAxis.len(),
								1.0,0,0,a_Curve,NULL);
      if (!s_return.ok())
		  printf("SoCurveKit::createCircle ACIS error %s\n",s_return.get_error_info()->error_message());
    }
    else 
    { 
      std::cout << "can't create circle"<< std::endl;
    }
    
  }
  else if (NumofPoints.getValue() >= 3)
  {
    SPAposition* ctrlpts = ACIS_NEW SPAposition[NumofPoints.getValue()];
    for (int i=0; i < NumofPoints.getValue(); i++)
    {	
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
      if(obj)
      {
        ctrlpts[i] = ((SoPointKit*)obj)->a_Point;
      }
    }
    
    SPAposition centre = GetcentrePoint(ctrlpts[0],ctrlpts[1],ctrlpts[2]);
    SPAvector majorAxis;
    majorAxis =  centre - ctrlpts[0];
    
    //calc the normal of plane
    SPAvector norm = GetPlaneNormal(ctrlpts[0], ctrlpts[1], ctrlpts[2]);
    
    s_return = api_mk_ed_ellipse(centre, normalise(norm), majorAxis,1.0,0,0,a_Curve,NULL);
    ACIS_DELETE [] ctrlpts;
  }
}

void SoCurveKit::createEllipse()
{
  if (NumofPoints.getValue() >= 3)
  {
    SPAposition* ctrlpts = ACIS_NEW SPAposition[NumofPoints.getValue()];
    for (int i=0; i < NumofPoints.getValue(); i++)
    {	
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
      if(obj)
      {
        ctrlpts[i] = ((SoPointKit*)obj)->a_Point;
      }
    }
    
    
    SPAvector majorAxis;
    majorAxis =  ctrlpts[1] - ctrlpts[0];
    
    //calc the normal of plane
    SPAvector norm = GetPlaneNormal(ctrlpts[0], ctrlpts[1], ctrlpts[2]);
    
    //calc the relation between axis a and b
    double a = distance_to_point(ctrlpts[0],ctrlpts[1]);
    SPAvector vec = ctrlpts[0] - ctrlpts[1];
    double x = distance_to_line(ctrlpts[2],ctrlpts[0], normalise(vec));
    double y = distance_to_plane(ctrlpts[2], ctrlpts[0], normalise(vec));
    double b = sqrt((a*a*x*x)/(a*a-y*y));
    double relation = b/a;
    if (relation <= 1.0)
    {
      s_return = api_mk_ed_ellipse(ctrlpts[0], normalise(norm), majorAxis, relation,0,0,a_Curve,NULL);
    }
    else
    {
      relation = a/b;
      SPAposition pos;
      SPAunit_vector uvec;
      intersect_plane_plane(ctrlpts[0], normalise(norm), ctrlpts[0], normalise(majorAxis), pos, uvec);
      uvec.set_x(uvec.x() * b);
      uvec.set_y(uvec.y() * b);
      uvec.set_z(uvec.z() * b);
      s_return = api_mk_ed_ellipse(ctrlpts[0], normalise(norm), uvec, relation,0,0,a_Curve,NULL); 
    }
    
    if (!s_return.ok()) 
    {
      std::cout << "SoCurveKit: Error: create Ellipse" <<std::endl;
      visible.setValue(SO_SWITCH_NONE);
    }
    else
      visible.setValue(SO_SWITCH_ALL);
    
    ACIS_DELETE [] ctrlpts;
  }
}

void SoCurveKit::createCurveBezier()
{
  int i;
  SPAposition* ctrlpts = ACIS_NEW SPAposition[NumofPoints.getValue()];
  for (i=0; i < NumofPoints.getValue(); i++)
  {	
    Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
    if(obj)
    {
      ctrlpts[i] = ((SoPointKit*)obj)->a_Point;
    }
  }
  
  int rational=FALSE;
  int closed=FALSE;
  int periodic = FALSE;
  int num_crtlpts=NumofPoints.getValue();
  
  double weights[1] = {NULL};
  double point_tol = SPAresabs;
  double knots[maxNumofCurvePoints + 2 * maxDegree];
  int num_knots = degree.getValue() + NumofPoints.getValue();
  for (i = 0; i < degree.getValue(); i++)
  {
    knots[i] = 0;
    knots[num_knots - (i+1)] = 1;
  }
  
  double d =  (num_knots - 2 * degree.getValue() + 1);
  d = 1/d;
  int m = 1;
  for (i = degree.getValue(); i < (num_knots - degree.getValue()); i++)
  {
    knots[i] = (d * m);
    m++;
  }
  /*
  std::cout << "num of knots:" << num_knots << " [";
  for (i = 0; i < num_knots; i++)
  {
  std::cout << knots[i] << " , ";
  }
  std::cout << "]" << std::endl;
  */
  double knot_tol = SPAresabs;
  
  s_return = api_mk_ed_int_ctrlpts(degree.getValue(),rational,closed,periodic,num_crtlpts,ctrlpts,weights,point_tol,num_knots,knots,knot_tol,a_Curve);
  if (!s_return.ok()) 
    std::cout << "Error: create B-Spline Curve" <<std::endl;
  ACIS_DELETE [] ctrlpts;
}

void SoCurveKit::createCurvePointson()
{
  SPAposition* ctrlpts = ACIS_NEW SPAposition[NumofPoints.getValue()];
  for (int i=0; i < NumofPoints.getValue(); i++)
  {
    Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
    if(obj)
    {
      ctrlpts[i] = ((SoPointKit*)obj)->a_Point;
    }
  }
  
  SPAunit_vector vec(0,0,0);
  
  s_return = api_mk_ed_cubic(NumofPoints.getValue(),ctrlpts, vec, vec, 0, a_Curve, NULL);
  if (!s_return.ok()) 
    std::cout << "Error: create cubic curve" <<std::endl;
  ACIS_DELETE [] ctrlpts;  
}

void SoCurveKit::createCurveIntersection()
{
  SoIntersectKit *obj = (SoIntersectKit*)SoNode::getByName(intersectObjectName.getValue());
  if (obj)
  {
    a_intCurve = NULL;
    a_intCurve_todraw = NULL;
    a_intCurve = obj->a_Shape;
    a_intCurve_todraw = obj->a_Shape_todraw;
    if (obj->a_Shape != NULL && a_intCurve_todraw != NULL)
    {
      std::cout << "build int curve" << std::endl;
      //visible.setValue(SO_SWITCH_ALL);
      setvisible();
      Facet_intCurve();
    }
    else
    {
      setinvisible();
      //visible.setValue(SO_SWITCH_NONE);
      std::cout << "build curve no shape " << CurveOp.getValue() << std::endl;
    }
  } 
  else
    std::cout << "build curve no object " << CurveOp.getValue() << std::endl;
}

void SoCurveKit::createCircledefinebyoneLineandonePoints()
{
  EDGE* edge = ((SoLineKit*)getObject(LineName.getValue()))->a_Line;
  SPAposition pos1 = ((SoPointKit*)getObject(pointsName[0]))->a_Point;
  
  //cal the major Axis
  SPAposition pointonEdge;
  double distance;
  EDGE* helpedge1;
  s_return = api_mk_ed_line(edge->start_pos() + normalise(edge->start_pos() - edge->end_pos()) * 100, edge->end_pos() - normalise(edge->start_pos() - edge->end_pos()) * 100, helpedge1);
  if (s_return.ok())
    s_return = api_entity_point_distance(helpedge1, pos1, pointonEdge, distance, *(param_info*)NULL_REF);
  
  if (s_return.ok())
    s_return = api_mk_ed_ellipse(pointonEdge, normalise(edge->end_pos() - edge->start_pos()), pos1 - pointonEdge ,1.0,0,0,a_Curve,NULL);
  
  if (!s_return.ok())
    std::cout << "SoCurveKit: Error can't create circle define by one line and one points" << std::endl;
}

void SoCurveKit::createCircledefinebyoneLineandtwoPoints()
{
  EDGE* edge = ((SoLineKit*)getObject(LineName.getValue()))->a_Line;
  SPAposition pos1 = ((SoPointKit*)getObject(pointsName[0]))->a_Point;
  SPAposition pos2 = ((SoPointKit*)getObject(pointsName[1]))->a_Point;
  
  //cal the major Axis
  SPAposition pointonEdge;
  double distance;
  EDGE* helpedge1;
  EDGE* helpedge2;
  s_return = api_mk_ed_line(edge->start_pos() + normalise(edge->start_pos() - edge->end_pos()) * 100, edge->end_pos() - normalise(edge->start_pos() - edge->end_pos()) * 100, helpedge1);
  if (s_return.ok())
    s_return = api_entity_point_distance(helpedge1, pos1, pointonEdge, distance, *(param_info*)NULL_REF);
  if (s_return.ok())
    s_return = api_mk_ed_line(pos1 + normalise(pos1 - pointonEdge) * 100, pointonEdge - normalise(pos1 - pointonEdge) * 100, helpedge2);
  if (s_return.ok())
    s_return = api_entity_point_distance(helpedge2, pos2, pointonEdge, distance, *(param_info*)NULL_REF);  
  
  if (s_return.ok())
    s_return = api_mk_ed_ellipse(pos1, normalise(edge->end_pos() - edge->start_pos()), normalise(pointonEdge - pos1) *  SPAvector(pos2 - pos1).len() ,1.0,0,0,a_Curve,NULL);
  
  if (!s_return.ok())
    std::cout << "SoCurveKit: Error can't create circle define by one line and two points" << std::endl;
}


void SoCurveKit::Facet_Curve()
{
  if (a_Curve)
  {
    int npts;
    int nmax = maxNumofCurvePoints;
    SPAposition pos[maxNumofCurvePoints];
    double t[maxNumofCurvePoints];
    
    //move the intersection curve
    s_return = api_facet_curve(a_Curve->geometry()->equation(),a_Curve->start_param(),a_Curve->end_param(),0.0005,nmax,(int&)npts,pos,t);
    
    if (!s_return.ok()) std::cout << "Error: create facets from B-Spline Curve" <<std::endl;
    
    if (s_return.ok()) 
    {
      
      SoVertexProperty *VertexProperty = (SoVertexProperty*)getPart("objectVertexProperty", false);
      DISABLEDIVNODE( VertexProperty );
      VertexProperty->vertex.startEditing();
      for (int i=0; i < npts; i++)
      {
        //std::cout << " Point " << i << " x: " << pos[i].x() << " y: " << pos[i].y() << " z: " << pos[i].z()  << " param: " << t[i] << std::endl;
        VertexProperty->vertex.set1Value(i,SbVec3f((float)pos[i].x(),(float)pos[i].y(),(float)pos[i].z()));
      }
      VertexProperty->vertex.finishEditing();
      ENABLEDIVNODE( VertexProperty );
      
      SoLineSet* line = (SoLineSet*) getPart("object", false);
      DISABLEDIVNODE( line );
      line->startIndex.setValue(0);
      line->numVertices.setValue(npts);
      ENABLEDIVNODE( line );
      
      SoVertexProperty *VertexProperty_c = (SoVertexProperty*)getPart("objectVertexProperty_c", false);
      SoLineSet* line_c = (SoLineSet*) getPart("object_c", false);
      DISABLEDIVNODE( line_c );
      line_c->startIndex.setValue(0);
      line_c->numVertices.setValue(0);
      ENABLEDIVNODE( line_c );
    }
  }
}

void SoCurveKit::Facet_intCurve()
{
  BODY* todraw_Body = NULL;
  api_copy_body(a_intCurve_todraw, todraw_Body, NULL);
  
  
  FILE* f = fopen("curve.txt", "w");
  debug_entity((ENTITY*)todraw_Body, f);
  fclose(f);
  
  if ((translationObjectName.getValue() != ""))
  {
    Object3dKit *obj = (Object3dKit*)SoNode::getByName(translationObjectName.getValue());
    if (obj->name.getValue() != "Surface")
    {
      api_apply_transf(todraw_Body,obj->a_Shape->transform()->transform(),NULL);
      api_change_body_trans(todraw_Body, obj->a_Shape->transform(),FALSE,NULL);
    }
  }
  int counter = 0;  
  int start = 0;
  int sum = 0;
  int c = 0;
  if (todraw_Body != NULL)
  {    
    SoVertexProperty *VertexProperty = (SoVertexProperty*)getPart("objectVertexProperty", false);   
    SoLineSet* line = (SoLineSet*) getPart("object", false);
    DISABLEDIVNODE( line );
    line->numVertices.deleteValues(0, -1);
    
    DISABLEDIVNODE( VertexProperty );
    VertexProperty->vertex.startEditing();
    for (LUMP *lum = todraw_Body->lump();lum != NULL; lum = lum->next())
    {
      for (SHELL *shel = lum->shell();shel != NULL; shel = shel->next())
      {
        for (WIRE *wir = shel->wire();wir != NULL; wir= wir->next())
        {
          COEDGE* st_coedg = wir->coedge();
          COEDGE* coedg = st_coedg;
          do
          {  
            int npts;
            int nmax = maxNumofCurvePoints;
            SPAposition pos[maxNumofCurvePoints];
            double t[maxNumofCurvePoints];
            
            if (coedg->edge()->geometry() != NULL)
              s_return = api_facet_curve(coedg->edge()->geometry()->equation(),coedg->edge()->start_param(),coedg->edge()->end_param(),0.0005,nmax,(int&)npts,pos,t);

            sum = sum + npts;
            if (!s_return.ok()) std::cout << "Error: create facets from intersection Curve" <<std::endl;
            
            if (s_return.ok()) 
            {
              int i;
              for (i=0; i < npts; i++)
              {
                //std::cout << " Point " << i+start << " x: " << pos[i].x() << " y: " << pos[i].y() << " z: " << pos[i].z()  << " param: " << t[i] << std::endl;
                VertexProperty->vertex.set1Value(i+start,SbVec3f((float)pos[i].x(),(float)pos[i].y(),(float)pos[i].z()));				
              }
              line->numVertices.set1Value(counter,npts);
              counter++;
              start += i;
            }
            coedg = coedg->next();
            //coedg = coedg->previous();
            c++;
            std::cout << "Curve kit while" << std::endl;
          }
          //while (coedg != st_coedg && coedg != NULL);
          while (coedg != st_coedg && coedg != NULL && c < 10); //c < 10 for sweep intersection
        }
      }
    }
    
    VertexProperty->vertex.finishEditing();
    ENABLEDIVNODE( VertexProperty );
    ENABLEDIVNODE( line );    
    
    SoVertexProperty *VertexProperty_c = (SoVertexProperty*)getPart("objectVertexProperty_c", false);
    SoLineSet* line_c = (SoLineSet*) getPart("object_c", false);
    DISABLEDIVNODE( line_c );
    line_c->startIndex.setValue(0);
    line_c->numVertices.setValue(0);
    ENABLEDIVNODE( line_c );
  }
  else
    std::cout << "Error; can't face intersection curve"<< std::endl;
  
  //std::cout << "num of intCurve: "<< counter << "  num of edges; " << Count_Edges(todraw_Body) << " sum: " << sum  << std::endl;
}

void SoCurveKit::add_lines_between_Controlpoints()
{
  if ((CurveOp.getValue() == CURVE_POINTS_ON) || (CurveOp.getValue() == CURVE_BEZIER)) 
  {
    SoVertexProperty *VertexProperty_c = (SoVertexProperty*)getPart("objectVertexProperty_c", false);
    DISABLEDIVNODE( VertexProperty_c );
    VertexProperty_c->vertex.startEditing();
    for (int i=0; i < NumofPoints.getValue(); i++)
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
      //std::cout << " Point " << i << " x: " << pos[i].x() << " y: " << pos[i].y() << " z: " << pos[i].z()  << " param: " << t[i] << std::endl;
      VertexProperty_c->vertex.set1Value(i, convertPosition(((SoPointKit*)obj)->a_Point).getValue());
    }
    VertexProperty_c->vertex.finishEditing();
    ENABLEDIVNODE( VertexProperty_c );
    
    SoLineSet* line_c = (SoLineSet*) getPart("object_c", false);
    DISABLEDIVNODE( line_c );
    line_c->startIndex.setValue(0);
    line_c->numVertices.setValue(NumofPoints.getValue());
    ENABLEDIVNODE( line_c );
  }
}

void SoCurveKit::UpdateAcisObject()
{
  Build();
}

SPAposition
SoCurveKit::GetcentrePoint(SPAposition Point1, SPAposition Point2, SPAposition Point3)
{
  // get centre of the 3 Points
  
  SPAposition centre;
  SPAvector norm = GetPlaneNormal(Point1, Point2, Point3);
  
  SPAposition linept;
  SPAunit_vector linedir;
  SPAposition p1;
  p1.set_x(((Point2.x() - Point1.x())/2) + Point1.x());
  p1.set_y(((Point2.y() - Point1.y())/2) + Point1.y());
  p1.set_z(((Point2.z() - Point1.z())/2) + Point1.z());
  SPAposition p2;
  p2.set_x(((Point3.x() - Point1.x())/2) + Point1.x());
  p2.set_y(((Point3.y() - Point1.y())/2) + Point1.y());
  p2.set_z(((Point3.z() - Point1.z())/2) + Point1.z());
  intersect_plane_plane(p1, normalise(Point2-Point1), p2, normalise(Point3-Point1),linept, linedir);
  double d;
  intersect_line_plane(linept, linedir, Point1, normalise(norm), d, centre);
  
  return centre;
}

SPAvector SoCurveKit::GetPlaneNormal(SPAposition Point1, SPAposition Point2, SPAposition Point3)
{
  //calc the normal of plane
  SPAvector norm;
  SPAvector a = Point1 - Point3;
  SPAvector b = Point2 - Point3;
  norm.set_x(a.y()*b.z() - a.z()*b.y());
  norm.set_y(a.z()*b.x() - a.x()*b.z());
  norm.set_z(a.x()*b.y() - a.y()*b.x());
  
  return norm;
}

SbBool SoCurveKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
  //	std::cout << "setupconnections Curve" << std::endl;
  if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    //SoBaseKit::setUpConnections(onOff, doItAlways);
    Object3dKit::setUpConnections(onOff, doItAlways);
    
    // do your own special setup here !
    // connect fields
    
    SoDrawStyle* drawstyle = (SoDrawStyle*) getPart("style", false);
    drawstyle->lineWidth.connectFrom(&(lineThickness));
    
    //SoNode *object;
    Object3dKit* obj;
    
    if (CurveOp.getValue() == CURVE_INTERSECTION)
    {
      if (!SensorsCreated)
      {
        printf("Sensor not initialized yet! \n");
        CreateSensors();
      }
      //object = SoNode::getByName(intersectObjectName.getValue());
      obj = (Object3dKit*)getByName(intersectObjectName.getValue());
      printf("CurveKit: intersectobjectname %s \n",intersectObjectName.getValue().getString());
      
      if (ObjectSensor[0] && obj)
      {
        ObjectSensor[0]->attach(&((SoIntersectKit*)obj)->ChangeObject);
      }
    }
    else if(LineName.getValue() != "")
    {
      if (!SensorsCreated)
      {
        printf("Sensor not initialized yet! \n");
        CreateSensors();
      }
      //object = SoNode::getByName(intersectObjectName.getValue());
      obj = (Object3dKit*)getByName(LineName.getValue());
      
      if (ObjectSensor[0] && obj)
      {
        ObjectSensor[0]->attach(&((SoLineKit*)obj)->ChangeObject);
      }      
      for(int i=0; i < NumofPoints.getValue()-1; i++)
      {
        if (pointsName[i] != "")
        {
          //object = SoNode::getByName(pointsName[i]);
          obj = (Object3dKit*)getByName(pointsName[i]);
          if (ObjectSensor[i + 1])
          {
            ObjectSensor[i + 1]->attach(&((SoPointKit*)obj)->pCoord);
          }
        }
      }
    }
    else
    {
      if (PlaneName.getValue() != "")
      { 
		  if(getObject(PlaneName.getValue()) && PlaneSensor) 
          PlaneSensor->attach(&((SoPlaneKit*)getObject(PlaneName.getValue()))->ChangeObject);
      }      
      
      if (NumofPoints.getValue() != 0)
      { 
        if (!SensorsCreated)
        {
          printf("Sensor not initialized yet! \n");
          CreateSensors();
        }
        for(int i=0; i<NumofPoints.getValue(); i++)
        {
          if (pointsName[i] != "")
          {
            //object = SoNode::getByName(pointsName[i]);
            obj = (Object3dKit*)getByName(pointsName[i]);
            if (ObjectSensor[i])
            {
              ObjectSensor[i]->attach(&((SoPointKit*)obj)->pCoord);
            }
          }
        }
      }
    }
    
    //    SoMaterial* mat = (SoMaterial*) getPart("material", false);
    //    mat->diffuseColor.connectFrom(&(color));
    
    //NumofPoints = *((SoSFInt32*) getPart("NumofPoints",false));
    
    UpdateAcisObject();
    
  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
    //translation.disconnect();
    Object3dKit::setUpConnections(onOff, doItAlways);
    
    SoDrawStyle* drawstyle = (SoDrawStyle*) getPart("style", false);
    drawstyle->lineWidth.disconnect();
    
    int i;
    for(i=0; i<NumofPoints.getValue(); i++)
    {
      if (ObjectSensor[i])
        ObjectSensor[i]->detach();
    }
    if (CurveOp.getValue() == CURVE_INTERSECTION)
    {
      if (ObjectSensor[i])
        ObjectSensor[0]->detach();    
    }
    if (PlaneName.getValue() != "")
      if(PlaneSensor)
        PlaneSensor->detach();
      
  }
  return !(connectionsSetUp = onOff);
}

void SoCurveKit::addPoint(SoNode* firstPoint, SoNode* lastPoint, SoNode* middlePoint, enumCurveOp type)
{
  if ( ((CurveOp.getValue() == CURVE_BEZIER) || (CurveOp.getValue() == CURVE_POINTS_ON)) && (CurveOp.getValue() == type))
  {
    SoSFName firstPointName;
    SoSFName lastPointName;
    SoSFName middlePointName; 
    
    if(firstPoint->isOfType(SoPointKit::getClassTypeId()))
      firstPointName.setValue(firstPoint->getName()); 
    if(lastPoint->isOfType(SoPointKit::getClassTypeId()))
      lastPointName.setValue(lastPoint->getName()); 
    if(middlePoint->isOfType(SoPointKit::getClassTypeId()))
      middlePointName.setValue(middlePoint->getName()); 
    
    if (((SoPointKit*)middlePoint)->ObjectName.getValue() != this->getName())
    {
      int counter = 0;
      while((counter < NumofPoints.getValue()) && (firstPointName.getValue() != pointsName[counter])) 
      {
        counter++;
      }
      if ( (counter < NumofPoints.getValue()-1) && ( (lastPointName.getValue() == pointsName[(counter+1)]) ||  (lastPointName.getValue() == pointsName[(counter-1)])) )
      {
        NumofPoints.setValue(NumofPoints.getValue()+1);
        for (int i = (NumofPoints.getValue()-2); i > (counter); i--)
        {
          pointsName.set1(i+1,pointsName[i].getString());
        }
        pointsName.set1((counter+1),middlePointName.getValue().getString());
        /*
        for (int a = 0; a < NumofPoints.getValue(); a++)
        {
        SbString s;
        pointsName.get1(a, s);
        std::cout << "point " << a <<  ": " << s.getString() << std::endl;
        }
        */
        Build(); 
        CreateSensors();
      }
      else 
        std::cout << "can't add point to curve" << std::endl;
    }
    else 
      std::cout << "can't add point to curve: point is point on curve" << std::endl;
  }
}

void SoCurveKit::addPoint(SoNode* endPoint, SoNode* newEndPoint, enumCurveOp type)
{
  if ( ((CurveOp.getValue() == CURVE_BEZIER) || (CurveOp.getValue() == CURVE_POINTS_ON)) && (CurveOp.getValue() == type))
  {
    SoSFName endPointName;
    SoSFName newEndPointName;
    
    if(endPoint->isOfType(SoPointKit::getClassTypeId()))
      endPointName.setValue(endPoint->getName()); 
    if(newEndPoint->isOfType(SoPointKit::getClassTypeId()))
      newEndPointName.setValue(newEndPoint->getName());
    
    if (((SoPointKit*)newEndPoint)->ObjectName.getValue() != this->getName())
    {
      if(pointsName[0] == endPointName.getValue()) 
      {
        std::cout << "Curve add Endpoint" << std::endl;
        NumofPoints.setValue(NumofPoints.getValue()+1);
        for (int i = (NumofPoints.getValue()-2); i >= 0; i--)
        {
          pointsName.set1(i+1,pointsName[i].getString());
        }
        pointsName.set1(0,newEndPointName.getValue().getString());
        
        Build(); 
        CreateSensors();
      }
      else if(pointsName[(NumofPoints.getValue()-1)] == endPointName.getValue()) 
      {
        std::cout << "Curve add Endpoint" << std::endl;
        NumofPoints.setValue(NumofPoints.getValue()+1);
        pointsName.set1( (NumofPoints.getValue()-1) ,newEndPointName.getValue().getString());
        
        Build(); 
        CreateSensors();
      }
      else 
        std::cout << "can't add point to curve" << std::endl;
    }
    else 
      std::cout << "can't add point to curve: point is point on curve" << std::endl;
  }  
}

void SoCurveKit::deletePoint(SoNode* deletePoint, enumCurveOp type)
{
  if ( ((CurveOp.getValue() == CURVE_BEZIER) || (CurveOp.getValue() == CURVE_POINTS_ON)) && (CurveOp.getValue() == type))
  {
    if ( ((NumofPoints.getValue()-1) >= (degree.getValue()+1) ) || (CurveOp.getValue() == CURVE_POINTS_ON))
    {
      SoSFName deletePointName;
      if(deletePoint->isOfType(SoPointKit::getClassTypeId()))
        deletePointName.setValue(deletePoint->getName());
      
      int counter = 0;
      while((counter < NumofPoints.getValue()) && (deletePointName.getValue() != pointsName[counter])) 
      {
        counter++;
      }
      if (counter < NumofPoints.getValue())
      {
        std::cout << "Curve delete Point" << std::endl;
        for (int i = (counter+1); i < NumofPoints.getValue(); i++)
        {
          pointsName.set1(i-1,pointsName[i].getString());
        }
        pointsName.set1((NumofPoints.getValue()-1), "");
        NumofPoints.setValue(NumofPoints.getValue()-1);
        Build(); 
        CreateSensors();
      }
    }
    else
      std::cout << "can't delete point from curve" << std::endl;
  }
}

bool SoCurveKit::rebuild(int deg)
{
  if (CurveOp.getValue() == CURVE_BEZIER)
  {
    if ((deg*2) < (deg + NumofPoints.getValue()))
    {
      degree.setValue(deg);
      Build();
      this->objectList.setNum(1);
      this->objectList.set1(0, this->getName().getString());
      return true;
    }
    else
    {
      std::cout << "can't build curve with degree " << deg << std::endl; 
      return false;
    }
    
  }
  return false;
}

int SoCurveKit::Count_Edges(BODY* body)
{
  int counter = 0;
  if (body != NULL)
  {    
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
            counter++;
            coedg = coedg->next();
          }
          while (coedg != st_coedg && coedg != NULL);
        }
      }
    }    
  }
  return counter;
}

BODY* SoCurveKit::getBody()
{
  if (CurveOp.getValue() != CURVE_INTERSECTION)
  {
    COEDGE* coedge = new COEDGE(a_Curve, NULL,NULL, NULL);
    WIRE *wire = new WIRE(coedge, NULL);
    SHELL *shell = new SHELL();
    shell->set_wire(wire);
    LUMP* lump = new LUMP(shell, NULL);
    BODY* body = new BODY(lump);
    return body;
  }
  else
    return(a_intCurve);
}

ENTITY* SoCurveKit::getEntity()
{
  if (CurveOp.getValue() == CURVE_INTERSECTION)
  {
    return (ENTITY*) a_intCurve;
  }
  else
    return (ENTITY*) a_Curve;
}

void 
SoCurveKit::setHighlightMaterial(int currentUserID)
{
  SoMaterial* dummy1 = new SoMaterial;
  SoMaterial* dummy2 = new SoMaterial;
  SoMaterial* dummy3 = new SoMaterial;
  SoMaterial* dummy4 = new SoMaterial;
  MaterialConstants::defineLineMaterials(*W, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}

SPAposition SoCurveKit::getstartPosition()
{
  SPAposition pos;
  if (CurveOp.getValue() == CURVE_INTERSECTION)
    pos = a_intCurve->lump()->shell()->wire()->coedge()->start_pos();
  else
    pos = a_Curve->start_pos();  
  
  return pos;
}

bool SoCurveKit::isPointonCurve(SPAposition pos)
{
  if (a_Curve)
  {
    const curve& c = a_Curve->geometry()->equation();
    return (c.test_point(pos, *(SPAparameter*)NULL_REF, *(SPAparameter*)NULL_REF));
  }
  
  if (a_intCurve)
  {
    BODY* curve_Body = NULL;
    api_copy_body(a_intCurve, curve_Body, NULL);
    
    if ((translationObjectName.getValue() != ""))
    {
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(translationObjectName.getValue());
      if (obj->name.getValue() != "Surface")
      {
        api_apply_transf(curve_Body,obj->a_Shape->transform()->transform(),NULL);
        api_change_body_trans(curve_Body, obj->a_Shape->transform(),FALSE,NULL);
      }
    }
    
    for (LUMP *lum = curve_Body->lump();lum != NULL; lum = lum->next())
    {
      for (SHELL *shel = lum->shell();shel != NULL; shel = shel->next())
      {
        for (WIRE *wir = shel->wire();wir != NULL; wir= wir->next())
        {
          COEDGE* st_coedg = wir->coedge();
          COEDGE* coedg = st_coedg;
          do
          {            
            if (coedg->edge()->geometry() != NULL)
            {
              const curve& c = coedg->edge()->geometry()->equation();
              if (c.test_point(pos, *(SPAparameter*)NULL_REF, *(SPAparameter*)NULL_REF))
                return true;
            }
            coedg = coedg->next();
          }
          while (coedg != st_coedg && coedg != NULL);
        }
      }
    }
  }
  return false;
}

SPAunit_vector SoCurveKit::pointDirection(SPAposition pos)
{
  if (a_Curve)
  {
    const curve& c = a_Curve->geometry()->equation();
    const SPAparameter  param_guess = NULL;
    return c.point_direction(pos, param_guess);
  }
  else
  {
    if (a_intCurve)
    {
      BODY* curve_Body = NULL;
      api_copy_body(a_intCurve, curve_Body, NULL);
      
      if ((translationObjectName.getValue() != ""))
      {
        Object3dKit *obj = (Object3dKit*)SoNode::getByName(translationObjectName.getValue());
        if (obj->name.getValue() != "Surface")
        {
          api_apply_transf(curve_Body,obj->a_Shape->transform()->transform(),NULL);
          api_change_body_trans(curve_Body, obj->a_Shape->transform(),FALSE,NULL);
        }
      }      
      
      for (LUMP *lum = curve_Body->lump();lum != NULL; lum = lum->next())
      {
        for (SHELL *shel = lum->shell();shel != NULL; shel = shel->next())
        {
          for (WIRE *wir = shel->wire();wir != NULL; wir= wir->next())
          {
            COEDGE* st_coedg = wir->coedge();
            COEDGE* coedg = st_coedg;
            do
            {            
              if (coedg->edge()->geometry() != NULL)
              {
                const curve& c = coedg->edge()->geometry()->equation();
                
                const SPAparameter  param_guess = NULL;
                
                if (c.test_point(pos, *(SPAparameter*)NULL_REF, *(SPAparameter*)NULL_REF)) 
                  return c.point_direction(pos, param_guess);
              }
              coedg = coedg->next();
            }
            while (coedg != st_coedg && coedg != NULL);
          }
        }
      }
    }
  }
  return NULL;
}

void SoCurveKit::Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum)
{
  int counter = 0; 
  int objNumber = childNumber.getValue();
  
  if (CurveOp.getValue() == CURVE_INTERSECTION)
  {
    if (ObjectSensor[0])
      ObjectSensor[0]->detach();
  }
  else if(LineName.getValue() != "")
  {
    if (ObjectSensor[0])
      ObjectSensor[0]->detach();
    
    for(int i=1; i<NumofPoints.getValue(); i++)
    {
      if (pointsName[i-1] != "")
      {
        Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i-1]);
        if(obj)
        {
          obj->disconnectChild(this);
        }
        if (ObjectSensor[i])
        {
          ObjectSensor[i]->detach();
          //printf("removing sensor %d \n",i);
        }
      }
    }
  }
  else
  {
    if (PlaneName.getValue() != "")
      if(PlaneSensor)
        PlaneSensor->detach();
      
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
            //printf("removing sensor %d \n",i);
          }
        }
      }
  }
  RemoveAllCallbacks();
  
  //SoSeparator *looknode = (SoSeparator*) SelectionRoot->getChild(objNumber);
  //SelectionRoot->removeChild((SoSeparator*)looknode);// removes whole seperator node
  SelectionRoot->removeChild(this);// removes whole curve
  ObjSum->setValue(ObjSum->getValue() - 1);
}
