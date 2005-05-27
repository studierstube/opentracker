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

#include "SoPolygonKit.h"


SO_KIT_SOURCE(SoPolygonKit);

void SoPolygonKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoPolygonKit, Object3dKit, "Object3dKit");
} 

void
SoPolygonKit::CreateSensors()
{
      for (int i=0; i < NumofPoints.getValue(); i++)
      {
        ObjectSensor[i] = new SoFieldSensor(nameUpdate, this);
        ObjectSensor[i]->setPriority(0);
  
        Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
        if(obj)
        {
          obj->connectChild(this);
        }
      }
}

SoPolygonKit::SoPolygonKit() 
{ 
  initSoPolygonKit();
  CreateSensors();
}
/*
SoPolygonKit::SoPolygonKit(int number) 
{ 
  initSoPolygonKit();
  CreateSensors();
  
  childNumber.setValue(number);
  lineThickness.setValue(polygonLineThickness);
//  backColor.setValue(polygonBackColor);
//  color.setValue(polygonDefaultColor);
  setNodeName("Y", number);
}
*/
// skew or regular polygon with n Points
SoPolygonKit::SoPolygonKit(int number, std::vector<Object3dKit*> points, int numofPoints, SbBool constrained)
{
  initSoPolygonKit();
  
  childNumber.setValue(number);
  lineThickness.setValue(polygonLineThickness);
//  backColor.setValue(polygonBackColor);
//  color.setValue(polygonDefaultColor);
  setNodeName("Y", number);  
  
  Points = points;
  NumofPoints = numofPoints;
  pointsName.setNum(NumofPoints.getValue());
//  printf("Number of points in polygon %d \n",NumofPoints.getValue());
  
  for(int i=0; i<NumofPoints.getValue(); i++)
  {
    pointsName.set1(i,Points[i]->getName().getString());
  }

  // constrained==TRUE means that a regular polygon has been drawn. 
  // All points must be constrained to a plane
//  if (constrained)
//  {
//  }

  UpdateAcisObject();

  if (s_return.ok())
	CreateSensors();
}



// init all parts and fields
void
SoPolygonKit::initSoPolygonKit()
{
  SO_KIT_CONSTRUCTOR(SoPolygonKit); 
  // Add the catalog parts:
  //-----------------------name--|---type------|nuldef|  par   |rite|public
  SO_KIT_ADD_CATALOG_ENTRY(objectVertexProperty,SoVertexProperty,FALSE,switchNode,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(object,SoFaceSet,   FALSE,switchNode,\x0,TRUE);

  SO_KIT_ADD_CATALOG_ENTRY(object_c,SoLineSet,   FALSE,switchNode,\x0,TRUE);

  //fields
  SO_NODE_ADD_FIELD(backColor, (0.2f, 0.2f, 0.2f));
  SO_NODE_ADD_FIELD(NumofPoints, (0));
  SO_NODE_ADD_FIELD(pointsName, (""));

    // Initialize this SoPolygonKit
  SO_KIT_INIT_INSTANCE();
  
  ObjectSensor.reserve(maxNumofPoints);

  //turn dragging temporarily off
  translationOn.setValue(FALSE);
  rotationOn.setValue(FALSE);
  
  //    normal.setValue(FALSE);
  name.setValue("Polygon");
	
  // connect fields
//  SoMaterial* mat = (SoMaterial*) getPart("material", false);
//  mat->diffuseColor.connectFrom(&(color));
//  mat->emissiveColor.connectFrom(&(backColor));
//  transparencyValue.setValue(0.5);

  SoDrawStyle* drawstyle = (SoDrawStyle*) getPart("style", false);
  drawstyle->lineWidth.connectFrom(&(lineThickness));


  //translation.connectFrom(&(movePlane));
}

// Destructor
SoPolygonKit::~SoPolygonKit()
{
}


float
SoPolygonKit::getDistance(SbVec3f penPosition)
{
  double distance = maxDistance;
  
  SPAposition pen;
  pen.set_x(penPosition.getValue()[0]);
  pen.set_y(penPosition.getValue()[1]);
  pen.set_z(penPosition.getValue()[2]);
  
  SPAposition Point;
  param_info& ent_info=*(param_info*)NULL_REF;
  ENTITY* entity = (ENTITY*)a_Polygon;
    
  api_entity_point_distance(entity, pen, Point, distance, ent_info);

  std::cout << "skew polygon distance: " << distance << std::endl;
  return float(distance);
}


void
SoPolygonKit::nameUpdate(void *data, SoSensor *sensor)
{
  SoPolygonKit *self=(SoPolygonKit*)data;
  self->triggerChange();
}

void
SoPolygonKit::UpdateAcisObject()
{
    s_return = outcome(-1,NULL); 
    int numPoints = NumofPoints.getValue();
    SPAposition* vertex = new SPAposition[numPoints];
    int i;
    for (i=0; i < numPoints; i++)
    {	
      Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
      if(obj)
      {
        vertex[i] = ((SoPointKit*)obj)->a_Point;
      }
    }

	EDGE** line = new EDGE*[numPoints];
    for (i=0; i < numPoints-1; i++)
    {	
		api_mk_ed_line (vertex[i], vertex[i+1], line[i]);
	}
	// close polygon (connect last and first point)
	api_mk_ed_line (vertex[numPoints-1], vertex[0], line[numPoints-1]);
		
	s_return = api_make_ewire (numPoints, line, a_Polygon);

/*    FILE* f = fopen("poly.txt", "w");
    debug_entity((ENTITY*)a_Polygon, f);
    fclose(f); 
	printf("writing out poly");
*/
	if (!s_return.ok()) 
      std::cout << "Error: creating Polygon" <<std::endl;
}

int 
SoPolygonKit::FacePlane(double planeSize, SPAvector* normalvector, SPAposition Point, SbVec3f* &VertexArray)
{
  FACE* a_Faces;
  int NumVertices = 0;

  if (planeSize < minPlaneSize) 
    planeSize=minPlaneSize;

  s_return = api_face_plane(Point,planeSize,planeSize,normalvector,a_Faces,NULL);

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
  
  return NumVertices;
}

/*
FACE* 
SoPolygonKit::GetPlaneFace()
{
  FACE* Faces;
  SoPointKit *Point1;
  SoPointKit *Point2;
  SoPointKit *Point3;

  if (PointOneName.getValue() != "")
  { 
    SbName PointName = PointOneName.getValue();
		Point1 = (SoPointKit*)SoNode::getByName(PointName);
  }
  if (PointTwoName.getValue() != "")
  { 
    SbName PointName = PointTwoName.getValue();
		Point2 = (SoPointKit*)SoNode::getByName(PointName);
  }
  if (PointThreeName.getValue() != "")
  { 
    SbName PointName = PointThreeName.getValue();
		Point3 = (SoPointKit*)SoNode::getByName(PointName);
  }
  SoSFVec3f c = GetcentrePoint(Point1->a_Point, Point2->a_Point, Point3->a_Point);
  SPAposition center(c.getValue()[0],c.getValue()[1],c.getValue()[2]);
  SPAposition point = Point1->a_Point - 100*(Point1->a_Point - center);
  s_return = api_face_plane(point,100.0,100.0,new SPAvector(a_Plane->normal()), Faces,NULL);
  return Faces;
}
*/

void
SoPolygonKit::OnChange()
{
	UpdateAcisObject();
}

SbBool 
SoPolygonKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
  if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    //SoBaseKit::setUpConnections(onOff, doItAlways);
    Object3dKit::setUpConnections(onOff, doItAlways);
    
    // do your own special setup here !
    // connect fields
    SoNode *object;

	if (NumofPoints.getValue() != 0)
      { 
        CreateSensors();
        for(int i=0; i<NumofPoints.getValue(); i++)
        {
          SbString objectName; 
          pointsName.get1(i,objectName);
          if (pointsName[i] != "")
          {
            object = SoNode::getByName(pointsName[i]);
            if (ObjectSensor[i])
            {
              ObjectSensor[i]->attach(&((SoPointKit*)object)->pCoord);
            }
          }
        }
      }

//    SoMaterial* mat = (SoMaterial*) getPart("material", false);
//    mat->diffuseColor.connectFrom(&(color));

    UpdateAcisObject();

  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
    //translation.disconnect();
    
    //SoMaterial* mat = (SoMaterial*) getPart("material", false);
    //mat->diffuseColor.disconnect();
    
    //SoBaseKit::setUpConnections(onOff, doItAlways);
	Object3dKit::setUpConnections(onOff, doItAlways);

    for(int i=0; i<NumofPoints.getValue(); i++)
    {
      ObjectSensor[i]->detach();
    }

  }
  return !(connectionsSetUp = onOff); 
}

int 
SoPolygonKit::Triangulate(FACE* p_Face, SbVec3f* &p_VertexArray)
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
        //std::cout << "position: "<< vtx_pos.x() << " " << vtx_pos.y() << " " << vtx_pos.z() << " " << ArrayPos <<std::endl;
			}
			
		}				
		delete facets;
		p_Face = p_Face->next_in_list();
	}
	
	return(NumTriangle);
}
