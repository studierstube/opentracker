// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Cnd.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//  AUTHOR:    kf Klaus Feiler
// ===========================================================================
//  HISTORY:   Time of development: Dezember 2002 - 
//
// ===========================================================================

#include "SoSurfaceKit.h"
#include "SoCurveKit.h"
#include "transf.hxx"
#include <Inventor/nodes/SoShapeHints.h>

#include "MaterialConstants.h"

SO_KIT_SOURCE(SoSurfaceKit);

void SoSurfaceKit::initClass() 
{ 
  SO_KIT_INIT_CLASS(SoSurfaceKit, Object3dKit, "Object3dKit");
} 

SoSurfaceKit::SoSurfaceKit() 
{ 
  initSoSurfaceKit(); 
  
  Build(surface_high);  // Build Surface
  CreateSensors();
  setUpConnections(TRUE, TRUE);
}

SoSurfaceKit::SoSurfaceKit(SoSFInt32* objSum, std::vector<int> objectList, enumSurfaceOp type, SoSFInt32 numberX, SoSFInt32 numberY, SoSFInt32 degreeX,  SoSFInt32 degreeY, SoSelection* selectionRoot, int userID)
{
  initSoSurfaceKit();
  
  childNumber.setValue((objSum->getValue() + numberX.getValue() * numberY.getValue() - 4));
  lineThickness.setValue(3.0);
  setNodeName("O", (objSum->getValue() + numberX.getValue() * numberY.getValue() - 4));
  SurfaceOp.setValue(type); 
  this->userID.setValue(userID);
  MaterialConstants::defineBoolMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);
  
  s_return = outcome(-1,NULL); 
  
  if (objectList.size() >= 4)
  {
    if ( (getObject(objectList[0])->isOfType(SoPointKit::getClassTypeId())) &&
      (getObject(objectList[1])->isOfType(SoPointKit::getClassTypeId())) &&  
      (getObject(objectList[2])->isOfType(SoPointKit::getClassTypeId())) &&  
      (getObject(objectList[3])->isOfType(SoPointKit::getClassTypeId()))   )  
    {
      NumofPointsX = numberX;
      NumofPointsY = numberY;
      ObjSum = objSum;
      SelectionRoot = selectionRoot;
      degree_X = degreeX;
      degree_Y = degreeY;
      getPoints(objSum, selectionRoot, objectList);
      
      Build(surface_high);
      if (s_return.ok())
      {
        CreateSensors();
        addDependentObjects(objectList, 4);
        deselectObjects(objectList,4);
        createObjectList(objectList, 4);
        setUpConnections(TRUE, TRUE);
      }
    }
  }
}

//constructor for sweepsurface
SoSurfaceKit::SoSurfaceKit(SoSFInt32* objSum, std::vector<int> objectList, int userID)
{
  initSoSurfaceKit();
  
  childNumber.setValue(objSum->getValue());
  lineThickness.setValue(3.0);
  setNodeName("O", objSum->getValue());
  SurfaceOp.setValue(SURFACE_SWEEP); 
  this->userID.setValue(userID);
  MaterialConstants::defineBoolMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
  SoMaterial* mat = (SoMaterial*) getPart("material", false);
  mat->copyFieldValues(W);
  
  s_return = outcome(-1,NULL);  
  
  if (objectList.size() >= 2)
  {
    if ( ((getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId())) &&
      (getObject(objectList[1])->isOfType(SoCurveKit::getClassTypeId()))) || 
      ((getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) &&
      (getObject(objectList[0])->isOfType(SoCurveKit::getClassTypeId()))) || 
      ((getObject(objectList[1])->isOfType(SoLineKit::getClassTypeId())) &&
      (getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId()))) )
    {
      if(getObject(objectList[0])->isOfType(SoLineKit::getClassTypeId()))
      {  
        sweepAxisName.setValue(getObject(objectList[0])->getName());
        sweepObjectName.setValue(getObject(objectList[1])->getName());
      }
      else
      {
        sweepAxisName.setValue(getObject(objectList[1])->getName());
        sweepObjectName.setValue(getObject(objectList[0])->getName());
      }
      
      Build(surface_high);
      if (s_return.ok())
      {
        //CreateSensors();
        addDependentObjects(objectList, 2);
        deselectObjects(objectList,2);
        createObjectList(objectList, 2);
        setUpConnections(TRUE, TRUE);
      }
    }
  }
}

void
SoSurfaceKit::CreateSensors()
{
	if (s_return.ok())
	{
		if (sweepObjectName.getValue() != "")
		{
			if(getObject(sweepObjectName.getValue()))
			{
				sweepObjectSensor = new SoFieldSensor(nameUpdate, this);
				sweepObjectSensor->setPriority(0);
				SensorsCreated = TRUE;
				getObject(sweepObjectName.getValue())->connectChild(this);
			}      
			if(getObject(sweepAxisName.getValue()))
			{
				sweepAxisSensor = new SoFieldSensor(nameUpdate, this);
				sweepAxisSensor->setPriority(0);
				SensorsCreated = TRUE;
				getObject(sweepAxisName.getValue())->connectChild(this);
			}
		}
		else
		{
			if (SurfaceOp.getValue() != SURFACE_SWEEP)
			{
				Object3dKit *obj;
				for (int i=0; i < NumofPointsX.getValue()*NumofPointsY.getValue(); i++)
				{
					obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
					if(obj)
					{
						ObjectSensor[i] = new SoFieldSensor(nameUpdate, this);
						ObjectSensor[i]->setPriority(0);
						SensorsCreated = TRUE;
						obj->connectChild(this);
					}
				}
			}
		}
	}
}

// init all parts and fields
void
SoSurfaceKit::initSoSurfaceKit()
{
  
  SO_KIT_CONSTRUCTOR(SoSurfaceKit); 
  // Add the catalog parts:
  //-----------------------name--|---type------|nuldef|  par   |rite|public
  SO_KIT_ADD_CATALOG_ENTRY(objectVertexProperty,SoVertexProperty,FALSE,objectGroup,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(objectShapeHints,SoShapeHints,FALSE,objectGroup,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(objectFaceSet,SoFaceSet,   FALSE,objectGroup,\x0,TRUE);
  
  //fields
  SO_NODE_ADD_FIELD(pointsName, (""));
  SO_NODE_ADD_FIELD(sweepObjectName, (""));
  SO_NODE_ADD_FIELD(sweepAxisName, (""));
  SO_NODE_ADD_FIELD(NumofPointsX, (0));
  SO_NODE_ADD_FIELD(NumofPointsY, (0));
  SO_NODE_ADD_FIELD(SurfaceOp, (0));
  
  // Initialize this SoCurveKit
  SO_KIT_INIT_INSTANCE();
  //turn dragging temporarily off
  translationOn.setValue(FALSE);
  rotationOn.setValue(FALSE);
  
  name.setValue("Surface");
  a_Surface = NULL;
  
  ObjectSensor.reserve(maxNumofSurfacePoints);
  sweepObjectSensor = NULL;
  sweepAxisSensor = NULL;
  
  // connect fields
  SoShapeHints *sh = (SoShapeHints*)getPart("objectShapeHints", false);
  sh->vertexOrdering.setValue(SoShapeHintsElement::CLOCKWISE);
  sh->creaseAngle.setValue(1.5);
  SoDrawStyle* wireframe = (SoDrawStyle*) nearestDrawStyle.getValue();
  wireframe->lineWidth.setValue(1.5);
  
  SoGroup* nearGrp = (SoGroup*) nearestGroup.getValue();
  nearGrp->addChild(objectVertexProperty.getValue());
  nearGrp->addChild(objectShapeHints.getValue());
  nearGrp->addChild(objectFaceSet.getValue());
  
  updateSurfaceObject = false;
  buildfirst = true;  
  SensorsCreated = FALSE;
}

// Destructor
SoSurfaceKit::~SoSurfaceKit()
{
	// printf("DESTRUCTOR SURFACE---------------------------\n");
	// int num = NumofPointsX.getValue() * NumofPointsY.getValue() - 4;
	if (SurfaceOp.getValue() != SURFACE_SWEEP)
	{
		for(int i=0; i<NumofPointsX.getValue()*NumofPointsY.getValue(); i++)
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
					delete(ObjectSensor[i]);
				}
			}
		}
	}
	else
	{	
		if (sweepAxisSensor)
		{
			if (sweepAxisName.getValue() != "")
				if (getObject(sweepAxisName.getValue()))
          getObject(sweepAxisName.getValue())->disconnectChild(this);
			sweepAxisSensor->detach();
			delete(sweepAxisSensor);
		}
		if (sweepObjectSensor)
		{
			if (sweepObjectName.getValue() != "")
				if (getObject(sweepObjectName.getValue()))
          getObject(sweepObjectName.getValue())->disconnectChild(this);
			printf("detach sweep object sensor\n");
			sweepObjectSensor->detach();
			delete(sweepObjectSensor);
		}
	}	
	RemoveAllCallbacks();
}

// Disadvantage	: Radius point on Sphere can be selected 
// because points have a priority 
float
SoSurfaceKit::getDistance(SbVec3f penPosition)
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
    ENTITY* entity = NULL;
    if(SurfaceOp.getValue() == SURFACE_SWEEP)
      entity = (ENTITY*)a_Shape;
    else
      entity = (ENTITY*)a_Surface;
    
    api_entity_point_distance(entity, pen_point, Point, min_distance, ent_info);
  }
  return(float(min_distance));
}


void
SoSurfaceKit::nameUpdate(void *data, SoSensor *sensor)
{
//  std::cout << "SoSurface::nameUpdate()" << std::endl;
  SoSurfaceKit *self=(SoSurfaceKit*)data;
  //self->Build(surface_high);
  // only allow trigger of update through nameUpdate, not through direct callbacks 
  // of points/objects that are connected to this object
  self->triggerAllowed = TRUE;
  self->triggerChange();
  //self->updateSurfaceObject=false;
  self->updateSurfaceObject = true; // updates surface on release
}

void
SoSurfaceKit::OnChange()
{
// printf("Surfacekit on change called\n");
  if (triggerAllowed)
  {
    printf("Rebuilding surface \n");
    Build(surface_low);
  }
  updateSurfaceObject = true;
  triggerAllowed = FALSE;
}

void SoSurfaceKit::OnRelease()
{
// printf("Surfacekit on release called\n");
  if (updateSurfaceObject)
  {
    Build(surface_high);
    updateSurfaceObject = false;
  }
}

void SoSurfaceKit::OnMove()
{
  //  Build(surface_low);
  updateSurfaceObject = true;
}

void
SoSurfaceKit::Build(float tolerance)
{
  if (SurfaceOp.getValue() == SURFACE_SWEEP)
    BuildSweepSurface(tolerance);
  else
  {
    SPAposition pos[100];
    SoPointKit *obj;
    
    for (int i=0; i < NumofPointsX.getValue() * NumofPointsY.getValue(); i++)
    {
      //std::cout << "point nr=" << i << " name=" << pointsName.getString() << std::endl;
      obj = (SoPointKit*)SoNode::getByName(pointsName[i]);
      pos[i].set_x(obj->a_Point.x());
      pos[i].set_y(obj->a_Point.y());
      pos[i].set_z(obj->a_Point.z()); 
      //std::cout << "point nr=" << i << " name=" << pointsName[i].getString() << " x= " << pos[i].x() << " y=" << pos[i].y() << " z=" << pos[i].z() << std::endl; 
    }
    
    double precision = tolerance / 10000;
    std::cout << "make surface with precision: " << precision << " and tolerance: " << tolerance << std::endl;
    if (SurfaceOp.getValue() == SURFACE_B_SPLINE)
    {
      int rational=FALSE;
      int closed=FALSE;
      int periodic = FALSE;
      int num_crtlpts=NumofPointsX.getValue();  
      
      double weights[1] = {NULL};
      double point_tol = SPAresabs;
      
      double knots_X[maxNumofSurfacePoints + 2 * maxDegree];
      //knots_X = new double[maxNumofSurfacePoints + 2 * maxDegree];
      int num_knots_X = getKnots(degree_X, NumofPointsX, knots_X);
      
      double knots_Y[maxNumofSurfacePoints + 2 * maxDegree];
      int num_knots_Y = getKnots(degree_Y, NumofPointsY, knots_Y);
      
      double knot_tol = SPAresabs;
      s_return = api_mk_fa_spl_ctrlpts(degree_Y.getValue(), rational, 0, 0, NumofPointsY.getValue(), degree_X.getValue(), rational, 0, 0, NumofPointsX.getValue(), pos, weights, knot_tol, num_knots_Y, knots_Y, num_knots_X, knots_X, knot_tol, a_Surface, NULL);
    } 
    if (SurfaceOp.getValue() == SURFACE_POINTS_ON)
    {
      s_return = api_mk_fa_spl_fit(precision,NumofPointsY.getValue(),NumofPointsX.getValue(),pos,NULL,NULL,a_Surface,NULL);
    }
    
    
    if (s_return.ok())
    {
      int NumOfTriangles = Triangulate(a_Surface, tolerance); 
      
      int *FaceDescriptor = new int[NumOfTriangles];
      for(int i = 0; i < NumOfTriangles; ++i)
        FaceDescriptor[i] = 3;
      
      SoFaceSet *myFaceSet = (SoFaceSet*)getPart("objectFaceSet", true);
      DISABLEDIVNODE(myFaceSet);
      myFaceSet->numVertices.deleteValues(0);
      myFaceSet->numVertices.setValues(0, NumOfTriangles, FaceDescriptor);
      ENABLEDIVNODE(myFaceSet);
      
      if (NumOfTriangles == 0)
      {
        visible.setValue(SO_SWITCH_NONE);
        //deleted.setValue(TRUE);
      }
      else
      {
		if (objectdeleted.getValue() == false)
			visible.setValue(SO_SWITCH_ALL);
      }
      buildfirst = false;
    }
    else 
    {
      if (buildfirst)
      {
        int deletenum = ObjSum->getValue() - NumofPointsX.getValue() * NumofPointsY.getValue() + 4;
        int num = NumofPointsX.getValue() * NumofPointsY.getValue() - 4;
        for (int i = 0; i < num; i++) 
        {
          //std::cout << "ObjSum: " << ObjSum->getValue() << " point: " << i <<" deletenum: " << deletenum << std::endl;
          // hk 3.10.2003 - casted to ObjectKit
          //SoSeparator *looknode = (SoSeparator*) SelectionRoot->getChild(deletenum);
          Object3dKit *looknode = (Object3dKit*) SelectionRoot->getChild(deletenum);
          SelectionRoot->removeChild(looknode); // removes whole seperator node
          ObjSum->setValue(ObjSum->getValue()-1); 
        } 
      }
      std::cout << "ERROR: can't create surface" << std::endl;
    }
  }

  if (s_return.ok())
  {
  	  if (objectdeleted.getValue() == false)
		  this->visible.setValue(SO_SWITCH_ALL);
  }
  else
	this->visible.setValue(SO_SWITCH_NONE);
}

void
SoSurfaceKit::BuildSweepSurface(float tolerance)
{
  SoLineKit* line = (SoLineKit*)getObject(sweepAxisName.getValue());  
  sweep_options *opts = new sweep_options();
  opts->set_self_int_test(1);
  opts->set_draft_repair_level(SELF_INTERSECTION);
  ENTITY* helpEntity;
  
  if (getObject(sweepObjectName.getValue())->isOfType(SoCurveKit::getClassTypeId())) 
  {
    SoCurveKit* object = (SoCurveKit*)getObject(sweepObjectName.getValue());
    api_copy_entity (object->a_Curve, helpEntity);
    s_return = api_sweep_with_options(helpEntity, line->a_Line->start_pos(), line->a_Line->end_pos() - line->a_Line->start_pos(), opts, a_Shape, NULL);
  }
  else
  {
    SoLineKit* object = (SoLineKit*)getObject(sweepObjectName.getValue());
    api_copy_entity (object->a_Line, helpEntity);
    s_return = api_sweep_with_options(helpEntity, line->a_Line->start_pos(), line->a_Line->end_pos() - line->a_Line->start_pos(), opts, a_Shape, NULL);
  }
  
  if (s_return.ok())
  {
    int NumOfTriangles = Triangulate(a_Shape , tolerance + sweepTolerance);
    
    int *FaceDescriptor = new int[NumOfTriangles];
    for(int i = 0; i < NumOfTriangles; ++i)
      FaceDescriptor[i] = 3;
    
    SoFaceSet *myFaceSet = (SoFaceSet*)getPart("objectFaceSet", true);
    DISABLEDIVNODE(myFaceSet);
    myFaceSet->numVertices.deleteValues(0);
    myFaceSet->numVertices.setValues(0, NumOfTriangles, FaceDescriptor); 
    ENABLEDIVNODE(myFaceSet);
    
    if (NumOfTriangles == 0)
    {
      visible.setValue(SO_SWITCH_NONE);
      //deleted.setValue(TRUE);
    }
    else
    {
	  if (objectdeleted.getValue() == false)
		visible.setValue(SO_SWITCH_ALL);
    }
  }  
  else 
    std::cout << "SoSurfaceKit Error: can't build sweepsurface" << std::endl;
}

void SoSurfaceKit::UpdateAcisObject()
{
  Build(surface_low);
}

SbBool SoSurfaceKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	//	std::cout << "setupconnections Surface" << std::endl;
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) {
		// We connect AFTER base class.
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		if (SurfaceOp.getValue() != SURFACE_SWEEP)
		{
			SoNode *object;
			if (NumofPointsX.getValue()*NumofPointsY.getValue() != 0)
			{ 
				if (!SensorsCreated)
					CreateSensors();
				for(int i=0; i<NumofPointsX.getValue()*NumofPointsY.getValue(); i++)
				{
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
			
			//NumofPoints = *((SoSFInt32*) getPart("NumofPoints",false));
			if (NumofPointsX.getValue()*NumofPointsY.getValue() != 0)
				UpdateAcisObject(); 
		}
		else
		{
			if (sweepObjectName.getValue() != "")
			{
				if (!SensorsCreated)
					CreateSensors();
				if (sweepObjectSensor)
				{
					sweepObjectSensor->attach(&((Object3dKit*)getObject(sweepObjectName.getValue()))->ChangeObject);
				}      
				if (sweepAxisSensor)
				{
					sweepAxisSensor->attach(&((Object3dKit*)getObject(sweepAxisName.getValue()))->ChangeObject);
				}      
			}
		}
	}
	else {
		// We disconnect BEFORE base class.
		// Disconnect the field sensors.
		//translation.disconnect();
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		if (SurfaceOp.getValue() != SURFACE_SWEEP)
		{
			for(int i=0; i<NumofPointsX.getValue()*NumofPointsY.getValue(); i++)
			{
				if (ObjectSensor[i])
					ObjectSensor[i]->detach();
			}
		}
		else
		{
			if (sweepAxisSensor)
				sweepAxisSensor->detach();
			if (sweepObjectSensor)
				sweepObjectSensor->detach();
		}
	}
	return !(connectionsSetUp = onOff);
}

void SoSurfaceKit::getPoints(SoSFInt32* objSum, SoSelection* selectionRoot, std::vector<int> objectList)
{
  std::vector<SPAposition> PointsX_start(100);
  std::vector<SPAposition> PointsX_end(100);
  pointsName.setNum(NumofPointsX.getValue() * NumofPointsY.getValue());
  pointsName.set1(0,getObject(objectList[0])->getName().getString());
  pointsName.set1(NumofPointsX.getValue()-1, getObject(objectList[1])->getName().getString());
  pointsName.set1(((NumofPointsX.getValue() * NumofPointsY.getValue())-NumofPointsX.getValue()),getObject(objectList[3])->getName().getString());
  pointsName.set1(((NumofPointsX.getValue() * NumofPointsY.getValue())-1),getObject(objectList[2])->getName().getString());
  
  // this should never happen as the default values for NumofPointsX and NumofPointsY are 2 !
  if ((NumofPointsX.getValue() == 0) || (NumofPointsY.getValue() == 0))
    assert(0);
  
  PointsX_start[0] = ((SoPointKit*)getObject(objectList[0]))->a_Point; 
  PointsX_start[(NumofPointsX.getValue()-1)] = ((SoPointKit*)getObject(objectList[1]))->a_Point;
  double deltaX = ( ((SoPointKit*)getObject(objectList[1]))->a_Point.x() - ((SoPointKit*)getObject(objectList[0]))->a_Point.x()) / (NumofPointsX.getValue()-1);
  double deltaY = ( ((SoPointKit*)getObject(objectList[1]))->a_Point.y() - ((SoPointKit*)getObject(objectList[0]))->a_Point.y()) / (NumofPointsX.getValue()-1);
  double deltaZ = ( ((SoPointKit*)getObject(objectList[1]))->a_Point.z() - ((SoPointKit*)getObject(objectList[0]))->a_Point.z()) / (NumofPointsX.getValue()-1);
  
  
  //std::cout << "delta x=" << deltaX << " delta y= " << deltaY << " delta Z=" << deltaZ << std::endl;
  //std::cout << "point 0 x=" << PointsX_start[0].x() << " y=" << PointsX_start[0].y() << " z=" << PointsX_start[0].z() << std::endl; 
  
  int i;
  // create points between 1. and 2. selected point
  for (i=1; i < NumofPointsX.getValue()-1; i++)
  {
    PointsX_start[i].set_x(PointsX_start[i-1].x() + deltaX);
    PointsX_start[i].set_y(PointsX_start[i-1].y() + deltaY);
    PointsX_start[i].set_z(PointsX_start[i-1].z() + deltaZ);
    
    SbVec3f eventPos;
    eventPos = convertPosition(PointsX_start[i]).getValue();
    Object3dKit *P = new SoPointKit(objSum->getValue(), eventPos, this->userID.getValue());
    ((SoPointKit*)P)->addMovetoRedoUndoList=true;
    //P->layer.setValue(surfaceBaseObjectLayer);
    P->addDependentObjectsName(this->getName());
    selectionRoot->addChild(P);
    pointsName.set1(i,P->getName().getString());
    objSum->setValue(objSum->getValue()+1);
    //std::cout << "point " << i << " x=" << PointsX_start[i].x() << " y=" << PointsX_start[i].y() << " z=" << PointsX_start[i].z() << " Name: " << P->getName().getString() << std::endl; 
  }
  
  //std::cout << "point " << NumofPointsX.getValue()-1 <<  " x=" << PointsX_start[NumofPointsX.getValue()-1 ].x() << " y=" << PointsX_start[NumofPointsX.getValue()-1 ].y() << " z=" << PointsX_start[NumofPointsX.getValue()-1 ].z()  << std::endl; 
  
  PointsX_end[0] = ((SoPointKit*)getObject(objectList[3]))->a_Point; 
  PointsX_end[(NumofPointsX.getValue()-1)] = ((SoPointKit*)getObject(objectList[2]))->a_Point;
  deltaX = (((SoPointKit*)getObject(objectList[2]))->a_Point.x() - ((SoPointKit*)getObject(objectList[3]))->a_Point.x()) / (NumofPointsX.getValue()-1);
  deltaY = (((SoPointKit*)getObject(objectList[2]))->a_Point.y() - ((SoPointKit*)getObject(objectList[3]))->a_Point.y()) / (NumofPointsX.getValue()-1);
  deltaZ = (((SoPointKit*)getObject(objectList[2]))->a_Point.z() - ((SoPointKit*)getObject(objectList[3]))->a_Point.z()) / (NumofPointsX.getValue()-1);
  
  //std::cout << "delta x=" << deltaX << " delta y= " << deltaY << " delta Z=" << deltaZ << std::endl;
  
  //std::cout << "point 0 x=" << PointsX_end[0].x() << " y=" << PointsX_end[0].y() << " z=" << PointsX_end[0].z() <<  std::endl; 
  
  // creat the points between 3. and 4. select point
  for (i=1; i < NumofPointsX.getValue()-1; i++)
  {
    PointsX_end[i].set_x(PointsX_end[i-1].x() + deltaX);
    PointsX_end[i].set_y(PointsX_end[i-1].y() + deltaY);
    PointsX_end[i].set_z(PointsX_end[i-1].z() + deltaZ);
    
    SbVec3f eventPos;
    eventPos = convertPosition(PointsX_end[i]).getValue();
    Object3dKit *P = new SoPointKit(objSum->getValue(), eventPos, this->userID.getValue());
    ((SoPointKit*)P)->addMovetoRedoUndoList=true;
    //P->layer.setValue(surfaceBaseObjectLayer);
    P->addDependentObjectsName(this->getName());
    selectionRoot->addChild(P);
    pointsName.set1((NumofPointsX.getValue()*(NumofPointsY.getValue()-1)) + i,P->getName().getString());
    objSum->setValue(objSum->getValue()+1);
    //std::cout << "point " << i << " x=" << PointsX_end[i].x() << " y=" << PointsX_end[i].y() << " z=" << PointsX_end[i].z() << " Name: " << P->getName().getString() << std::endl; 
    
  }
  //std::cout << "point " << NumofPointsX.getValue()-1 <<  " x=" << PointsX_end[NumofPointsX.getValue()-1 ].x() << " y=" << PointsX_end[NumofPointsX.getValue()-1 ].y() << " z=" << PointsX_start[NumofPointsX.getValue()-1 ].z() << std::endl; 
  
  for (i=0; i < NumofPointsX.getValue(); i++)
  {
    deltaX = (PointsX_end[i].x() - PointsX_start[i].x()) / (NumofPointsY.getValue()-1);
    deltaY = (PointsX_end[i].y() - PointsX_start[i].y()) / (NumofPointsY.getValue()-1);
    deltaZ = (PointsX_end[i].z() - PointsX_start[i].z()) / (NumofPointsY.getValue()-1);
    
    for (int a=1; a < NumofPointsY.getValue() - 1; a++ )
    {
      SbVec3f eventPos;
      eventPos.setValue(float(PointsX_start[i].x() + (deltaX * a)), 
						float(PointsX_start[i].y() +  (deltaY * a)), 
						float(PointsX_start[i].z() + (deltaZ * a)));
      Object3dKit *P = new SoPointKit(objSum->getValue(), eventPos, this->userID.getValue());
      ((SoPointKit*)P)->addMovetoRedoUndoList=true; 
      //P->layer.setValue(surfaceBaseObjectLayer);
      selectionRoot->addChild(P);
      pointsName.set1((a*NumofPointsX.getValue() + i),P->getName().getString());
      objSum->setValue(objSum->getValue()+1);
      //std::cout << "Y point " << a << " x=" << PointsX_start[i].x() + (deltaX * a) << " y=" << PointsX_start[i].y() + (deltaY * a) << " z=" << PointsX_start[i].z() + (deltaZ * a)<< " Name: " << P->getName().getString() << std::endl; 
    }
  }
}

int SoSurfaceKit::Triangulate(FACE* p_Surface, float tolerance)
{
  int NumTriangle = 0;
  int ArrayPos = 0;
  
  SoVertexProperty *VertexProperty = (SoVertexProperty*)getPart("objectVertexProperty", true);
  
  REFINEMENT *ref = new REFINEMENT;
  ref->set_surf_mode(AF_SURF_ALL);
  ref->set_adjust_mode(AF_ADJUST_NONE);
  ref->set_triang_mode(AF_TRIANG_ALL);
  ref->set_surface_tol(tolerance);
  ref->set_normal_tol(tolerance);
  api_set_default_refinement(ref);
  
  parameter_token ptoken[2];
  ptoken[0] = POSITION_TOKEN;
  ptoken[1] = NORMAL_TOKEN;
  
  while (p_Surface)
  {
    POLYGON_POINT_MESH *facets = (POLYGON_POINT_MESH *)NULL;
    a_return = api_facet_face(p_Surface);
    a_return = api_get_face_facets(p_Surface,facets);
    if (!a_return.ok()) 
    {
      std::cout << "Error: no Surface Object" <<std::endl;
      p_Surface = NULL;
    }
    
    POLYGON *poly;
    POLYGON_VERTEX *poly_vtx;
    SPAposition vtx_pos;
    int NumVertices = 0;
    
    DISABLEDIVNODE( VertexProperty );
    
    // count number of vertices (go through all facets and all polys
    for (poly=facets->first(); poly != NULL; poly = poly->next())
      NumVertices += poly->count();
    VertexProperty->vertex.setNum(NumVertices);
    SbVec3f * points = VertexProperty->vertex.startEditing();
    
    int i=0;
    for(poly=facets->first(); poly != NULL; poly = poly->next())
    {
      for(poly_vtx = poly->first(); poly_vtx != NULL; poly_vtx = poly_vtx->next())
      {
        poly_vtx->point(vtx_pos);
        // Fill vertex array
        points[i][0] = (float)vtx_pos.x();
        points[i][1] = (float)vtx_pos.y();
        points[i][2] = (float)vtx_pos.z();
        i++;
      }
    }
    NumTriangle = i/3; // 3 vertices per triangle
    
    VertexProperty->vertex.finishEditing();
    ENABLEDIVNODE( VertexProperty );
    
    delete facets;
    p_Surface = p_Surface->next_in_list();
  }
  return(NumTriangle);
}

int SoSurfaceKit::Triangulate(BODY* p_Shape, float tolerance)
{
  int NumTriangle = 0;
  int ArrayPos = 0;
  
  SoVertexProperty *VertexProperty = (SoVertexProperty*)getPart("objectVertexProperty", true);
  
  REFINEMENT *ref = new REFINEMENT;
  ref->set_surf_mode(AF_SURF_ALL);
  ref->set_adjust_mode(AF_ADJUST_NONE);
  ref->set_triang_mode(AF_TRIANG_ALL);
  ref->set_surface_tol(tolerance);
  ref->set_normal_tol(tolerance);
  api_set_default_refinement(ref);
  
  parameter_token ptoken[2];
  ptoken[0] = POSITION_TOKEN;
  ptoken[1] = NORMAL_TOKEN;
  
  if (p_Shape != NULL)
  {
    FACE *f;
    for (LUMP *lum = p_Shape->lump();lum != NULL; lum = lum->next())
    {
      for (SHELL *shel = lum->shell();shel != NULL; shel = shel->next())
      {
        f=shel->face_list();
        while (f)
        {
          POLYGON_POINT_MESH *facets;
          a_return = api_facet_face(f);
          a_return = api_get_face_facets(f,facets);
          if (!a_return.ok()) 
          {
            std::cout << "Error: no Bool Object" <<std::endl;
            f = NULL;
          }
          
          POLYGON* poly;
          POLYGON_VERTEX* poly_vtx;
          SPAposition vtx_pos;
          
          DISABLEDIVNODE( VertexProperty );
          VertexProperty->vertex.startEditing();
          for(poly=facets->first(); poly != NULL; poly = poly->next())
          {
            NumTriangle++;
            
            for(poly_vtx = poly->first(); poly_vtx != NULL; poly_vtx = poly_vtx->next())
            {
              poly_vtx->point(vtx_pos);
              //std::cout << "SPAposition: "<< vtx_pos.x() << " " << vtx_pos.y() << " " << vtx_pos.z() << " " << ArrayPos <<std::endl;
              
              // Fill our vertex array
              VertexProperty->vertex.set1Value(ArrayPos++, (float)vtx_pos.x(), (float)vtx_pos.y(), (float)vtx_pos.z());
            }
            
          }
          VertexProperty->vertex.finishEditing();
          ENABLEDIVNODE( VertexProperty );
          
          //delete facets;
          ACIS_DELETE(facets);
          ACIS_DELETE(poly);
          //					poly_vtx->destroy(NULL);
          f = f->next_in_list();
        }
      }
    }
    api_delete_body_facets(p_Shape);
  }
  return(NumTriangle);
}

int SoSurfaceKit::getKnots(SoSFInt32 degree, SoSFInt32 numofPoints, double* knots)
{
  int num_knots = degree.getValue() + numofPoints.getValue();
  int i;
  for (i = 0; i < degree.getValue(); i++)
  {
    knots[i] = 0;
    knots[num_knots - (i+1)] = 1.0;
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
  return num_knots;
}

BODY* SoSurfaceKit::getBody()
{
  BODY* body = NULL;
  if (SurfaceOp.getValue() != SURFACE_SWEEP)
  {
    SHELL *shell = new SHELL(a_Surface, NULL,NULL);
    LUMP* lump = new LUMP(shell, NULL);
    body = new BODY(lump);
  }
  else
    body = a_Shape;

  return(body);
}

FACE* SoSurfaceKit::getFace()
{
  FACE* face = NULL;
  if (SurfaceOp.getValue() == SURFACE_SWEEP)
  {
    //SHELL *shell = new SHELL(a_Surface, NULL,NULL);
    //LUMP* lump = new LUMP(shell, NULL);
    face = a_Shape->lump()->shell()->face();;
  }
  else
    face = a_Surface;

  return(face);
}

void SoSurfaceKit::Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum)
{
	if (SurfaceOp.getValue() != SURFACE_SWEEP)
	{
		int deletenum = ObjSum->getValue() - 1 - NumofPointsX.getValue() * NumofPointsY.getValue() + 4;
		int num = NumofPointsX.getValue() * NumofPointsY.getValue() - 4;
		int i = 0;
		for(i=0; i<NumofPointsX.getValue()*NumofPointsY.getValue(); i++)
		{
			if (pointsName[i] != "")
			{
				//Object3dKit *obj = (Object3dKit*)SoNode::getByName(pointsName[i]);
				SoPointKit *obj = (SoPointKit*)SoNode::getByName(pointsName[i]);
				if(obj)
				{
					obj->disconnectChild(this);
				}
				if (ObjectSensor[i])
				{
					ObjectSensor[i]->detach();
				}
			}
		}
		  	RemoveAllCallbacks();
		
		for (i = 0; i < num; i++) 
		{
			SoPointKit *looknode = (SoPointKit*) SelectionRoot->getChild(deletenum);
			SelectionRoot->removeChild(looknode); // removes whole seperator node
			ObjSum->setValue(ObjSum->getValue()-1); 
		}
	}
	  else 
	    RemoveAllCallbacks();
	
	SelectionRoot->removeChild(this); // removes whole SurfaceKit
	ObjSum->setValue(ObjSum->getValue()-1);  
}

void 
SoSurfaceKit::setHighlightMaterial(int currentUserID)
{
  SoMaterial* dummy1 = new SoMaterial;
  SoMaterial* dummy2 = new SoMaterial;
  SoMaterial* dummy3 = new SoMaterial;
  SoMaterial* dummy4 = new SoMaterial;
  MaterialConstants::defineBoolMaterials(*W, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}

