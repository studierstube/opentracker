// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Cnd.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//  AUTHOR:    rs      Reinhard Steiner
// ===========================================================================
//  HISTORY:   Time of development: January 2002 - 
//
// ===========================================================================

#include "SoBoolKit.h"
#include "SoIntersectKit.h"
#include "transf.hxx"
#include <Inventor/nodes/SoShapeHints.h> 
#include <Inventor/nodes/SoComplexity.h> 

#include "MaterialConstants.h"

SO_KIT_SOURCE(SoBoolKit);

void SoBoolKit::initClass() 
{ 
	SO_KIT_INIT_CLASS(SoBoolKit, Object3dKit, "Object3dKit");
} 

SoBoolKit::SoBoolKit() 
{ 
	initSoBoolKit();
	
	Build(bool_high);  // Build shape
	CreateSensors();
	setUpConnections(TRUE, TRUE);
}

SoBoolKit::SoBoolKit(int number, std::vector<int> objectList, enumBoolOp boolop, int userID)
{
	initSoBoolKit();
	
	childNumber.setValue(number);
	setNodeName("B", number);
	this->userID.setValue(userID);
	MaterialConstants::defineBoolMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
	SoMaterial* mat = (SoMaterial*) getPart("material", false);
	mat->copyFieldValues(W);
	
	s_return = outcome(-1,NULL); 
	
	if ((objectList.size() >= 2) && (isObject(objectList[0])) && isObject(objectList[1]))
	{
		m_BoolOp = boolop;
		
		m_Object1Name = getObject(objectList[0])->getName();
		m_Object2Name = getObject(objectList[1])->getName();
		
		Build(bool_high);
		CreateSensors();
		
		if (s_return.ok())
		{
			setUpConnections(TRUE, TRUE);
			addDependentObjects(objectList, 2);
			deselectObjects(objectList,2);
			createObjectList(objectList, 2);
		}
	}
}

SoBoolKit::SoBoolKit(int number, SbName ObjectOneName, BODY* BodyOne, BODY* BodyTwo, int slicePart, std::vector<int> objectList, int userID)
{
	initSoBoolKit();
	
	childNumber.setValue(number);
	//	transparencyValue.setValue(boolObjectDefaultTransparency);
	setNodeName("B", number);
	this->userID.setValue(userID);
	MaterialConstants::defineBoolMaterials(*W, *S, *D, *N, *L, this->userID.getValue());
	SoMaterial* mat = (SoMaterial*) getPart("material", false);
	mat->copyFieldValues(W);
	
	s_return = outcome(-1,NULL); 
	
	slicingPart.setValue(slicePart);
	m_BoolOp = BOOL_CUT;
	
	m_Object1Name = ObjectOneName;
	
	Build(BodyOne, BodyTwo, bool_high);
	
	if (s_return.ok())
	{
		CreateSensors();
		setUpConnections(TRUE, TRUE);
		addDependentObjects(objectList, 1);
		createObjectList(objectList, 1);
	}
}

void
SoBoolKit::CreateSensors()
{
	if (s_return.ok())
	{
		objectOneSensor = new SoFieldSensor(nameUpdate, this);
		objectOneSensor->setPriority(0);
		
		objectTwoSensor = new SoFieldSensor(nameUpdate, this);
		objectTwoSensor->setPriority(0);
		
		if(m_Object1Name.getValue() == "") return;
		Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(m_Object1Name.getValue());
		if(obj1)
			obj1->connectChild(this);
		else std::cout << "no object found" << std::endl;
		
		if(m_Object2Name.getValue() == "") return;
		Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(m_Object2Name.getValue());
		if(obj2)
			obj2->connectChild(this);
	}
}

// init all parts and fields
void
SoBoolKit::initSoBoolKit()
{
	m_BoolOp = BOOL_UNINITIALIZED;
	
	SO_KIT_CONSTRUCTOR(SoBoolKit); 
	// Add the catalog parts:
	//-----------------------name--|---type------|nuldef|  par   |rite|public
	//  SO_KIT_ADD_CATALOG_ENTRY(objectVertexProperty,SoVertexProperty,FALSE,switchNode,\x0,TRUE);
	//  SO_KIT_ADD_CATALOG_ENTRY(objectShapeHints,SoShapeHints,FALSE,switchNode,\x0,TRUE);
	//  SO_KIT_ADD_CATALOG_ENTRY(objectFaceSet,SoFaceSet,   FALSE,switchNode,\x0,TRUE);
	
	SO_KIT_ADD_CATALOG_ENTRY(objectVertexProperty,SoVertexProperty,FALSE,objectGroup,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(objectShapeHints,SoShapeHints,FALSE,objectGroup,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(objectFaceSet,SoFaceSet,   FALSE,objectGroup,\x0,TRUE);
	
	//fields
	SO_NODE_ADD_FIELD(m_Object1Name, (""));
	SO_NODE_ADD_FIELD(m_Object2Name, (""));
	SO_NODE_ADD_FIELD(m_BoolOp, (BOOL_UNINITIALIZED));
	
	// Initialize this SoBoolKit
	SO_KIT_INIT_INSTANCE();
	
	//turn dragging temporarily off
	translationOn.setValue(FALSE);
	rotationOn.setValue(FALSE);
	
	name.setValue("Bool");
	a_Shape = NULL;
	
	// connect fields
	m_UpdateBoolObject = false;
	
	SoShapeHints *sh = (SoShapeHints*)getPart("objectShapeHints", false);
	sh->shapeType.setValue(SoShapeHints::SOLID);
	sh->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
	sh->creaseAngle.setValue(1.0);
	SoComplexity* comp = new SoComplexity;
	comp->value = 0.01;
	
	//  SoGroup* objGrp = (SoGroup*) objectGroup.getValue();
	//  objGrp->addChild(cone);
	SoGroup* nearGrp = (SoGroup*) nearestGroup.getValue();
	nearGrp->addChild(objectVertexProperty.getValue());
	nearGrp->addChild(comp);
	nearGrp->addChild(objectShapeHints.getValue());
	nearGrp->addChild(objectFaceSet.getValue());
	//  nearGrp->addChild(cone);
	
	
	objectOneSensor=NULL;
	objectTwoSensor=NULL;
	
	slicingPart.setValue(0);
}

// Destructor
SoBoolKit::~SoBoolKit()
{
	if(objectOneSensor)
		delete objectOneSensor;
	if(objectTwoSensor)
		delete objectTwoSensor;
}

// Disadvantage	: Radius point on Sphere can be selected 
// because points have a priority 
float
SoBoolKit::getDistance(SbVec3f penPosition)
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
		
		api_entity_point_distance((ENTITY*)a_Shape->lump()->shell(), pen_point, Point, min_distance, ent_info);
	}
	
	return(float(min_distance));
}


void
SoBoolKit::nameUpdate(void *data, SoSensor *sensor)
{
	std::cout << "SoBoolKit::nameUpdate()" << std::endl;
	SoBoolKit *self=(SoBoolKit*)data;
	self->Build(bool_low);
	self->triggerChange();
	self->m_UpdateBoolObject = false;
}


void
SoBoolKit::OnChange()
{
	m_UpdateBoolObject = true;
}


void 
SoBoolKit::OnMove()
{
	Build(bool_low);
	m_UpdateBoolObject = true;
}


void 
SoBoolKit::OnRelease()
{
	//  if(m_UpdateBoolObject == true)
	//  {
    Build(bool_high);
    m_UpdateBoolObject = false;
	//  }
}



// Builds the shape, and tolerance it
void SoBoolKit::Build(float tolerance)
{
	std::cout << "Build Boolobject" <<std::endl;
	SbName Obj1Name = m_Object1Name.getValue();
	SbName Obj2Name = m_Object2Name.getValue();
	
	if(Obj1Name != "" && Obj2Name != "")
	{    
		Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(Obj1Name);
		Object3dKit *obj2 = (Object3dKit*)SoNode::getByName(Obj2Name);
		if(obj1 && obj2 && obj1->s_return.ok() && obj2->s_return.ok())
		{
			if (slicingPart.getValue() == 0)
				Build(obj1->a_Shape, obj2->a_Shape, tolerance);
		}
		else
			std::cout << "can't add booloen object" << std::endl;
	}
	else if(Obj1Name != "" && Obj2Name == "")
	{
		Object3dKit *obj1 = (Object3dKit*)SoNode::getByName(Obj1Name);
		if (obj1 && obj1->s_return.ok())
		{
			if (slicingPart.getValue() == 1) //this is result of a slice
			{
				std::cout << "add booloen object 1" << std::endl;
				Build(((SoIntersectKit*)obj1)->sliceResultOne, ((SoIntersectKit*)obj1)->a_Shape, tolerance);
			}
			if (slicingPart.getValue() == 2) //this is result of a slice
			{
				std::cout << "add booloen object 2" << std::endl;
				Build(((SoIntersectKit*)obj1)->sliceResultTwo, ((SoIntersectKit*)obj1)->a_Shape, tolerance);  
			}
		}
	}
}

void SoBoolKit::Build(BODY* bodyOne, BODY* bodyTwo, float tolerance)
{
	s_return = api_del_entity(a_Shape);
	std::cout << "Build 2" <<std::endl;
	BODY *Shape;
	a_return = api_copy_body(bodyOne, Shape); 
	if (!a_return.ok()) std::cout << "Error: can't copy body object one" <<std::endl;
	
	
	a_return = api_copy_body(bodyTwo, a_Shape); 
	if (!a_return.ok()) std::cout << "Error: can't copy body object two" <<std::endl;
	/*  
    FILE* f = fopen("boolen_2.txt", "w");
    debug_entity((ENTITY*)bodyTwo, f);
    fclose(f);
    f = fopen("boolen_1.txt", "w");
    debug_entity((ENTITY*)bodyOne, f);
    fclose(f);
	*/
	switch(m_BoolOp.getValue())
	{
	case BOOL_UNION:
		{
			s_return = api_boolean(Shape,a_Shape,UNION);
			//s_return = api_boolean_chop_body(Shape,a_Shape,TRUE,UNION);
			if (!s_return.ok()) std::cout << "Error can't create Acis UNION" <<std::endl;
		}
		break;
	case BOOL_CUT:
		{
            // BOOL_CUT is also called by the SLICE operation
            // for slicing the reverse order of objects is needed: 
            // Second minus first object
            // in regular BOOL_CUT cases we want first minus second object
            if (slicingPart.getValue() == 0)
            {
                BODY* Shape2;
                a_return = api_copy_body(bodyOne, a_Shape); 
                if (!a_return.ok()) std::cout << "Error: can't copy body object one" <<std::endl;

                a_return = api_copy_body(bodyTwo, Shape2); 
                if (!a_return.ok()) std::cout << "Error: can't copy body object two" <<std::endl;

			    //s_return = api_boolean(Shape,a_Shape,SUBTRACTION);
                s_return = api_boolean(Shape2,a_Shape,SUBTRACTION);
			    if (!s_return.ok()) std::cout << "Error can't create Acis SUBTRACT" <<std::endl;
            }
            else
            {
                s_return = api_boolean(Shape,a_Shape,SUBTRACTION);
                if (!s_return.ok()) std::cout << "Error can't create Acis SUBTRACT" <<std::endl;
            }
		}
		break;
	case BOOL_INTERSECTION:
		{
			s_return = api_boolean(Shape,a_Shape,INTERSECTION);
			if (!s_return.ok()) std::cout << "Error can't create Acis UNION" <<std::endl;
		}
		break;
	}
	
	if (s_return.ok())
	{
    
		//FILE* f = fopen("boolen.txt", "w");
		//debug_entity((ENTITY*)a_Shape, f);
		//fclose(f); 
		int NumOfTriangles = Triangulate(a_Shape, tolerance);  
		//std::cout << "num of Triangles: " << NumOfTriangles <<std::endl;
		int NumVertices = NumOfTriangles * 3;
		//std::cout << "num of Vertices: " << NumVertices <<std::endl;
		if(NumVertices > 0)
		{
			int *FaceDescriptor = new int[NumOfTriangles];
			for(int i = 0; i < NumOfTriangles; ++i)
				FaceDescriptor[i] = 3;
			
			SoFaceSet *myFaceSet = (SoFaceSet*)getPart("objectFaceSet", true);
			DISABLEDIVNODE(myFaceSet);
			myFaceSet->numVertices.deleteValues(0);
			myFaceSet->numVertices.setValues(0, NumOfTriangles, FaceDescriptor);
			ENABLEDIVNODE(myFaceSet);
			
			delete[](FaceDescriptor);
			std::cout << "SoBoolKit (" << getName().getString() << ") build with tolerance " << tolerance <<std::endl <<std::endl;        
		}
		else 
		{
			s_return = outcome(-1,NULL); 
			std::cout << "can't add booloen object" << std::endl;
		}
		
		DISABLEDIVFIELD(visible);
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
		ENABLEDIVFIELD(visible);
	}
	else 
	{
		s_return = outcome(-1,NULL); 
		std::cout << "SoBoolKit::build can't add booloen object" << std::endl;
	}
}

int SoBoolKit::Triangulate(BODY* p_Shape, float tolerance)
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


SbBool SoBoolKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) {
		// We connect AFTER base class.
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		// do your own special setup here !
		// connect fields
		SoNode *objectOne = NULL;
		SoNode *objectTwo = NULL;
		
		if(m_Object1Name.getValue() != "")
		{
			objectOne = SoNode::getByName(m_Object1Name.getValue());
			//pCoord.connectFrom(&(((Object3dKit*)objectOne)->ChangeObject));
			objectOneSensor->attach(&((Object3dKit*)objectOne)->ChangeObject);
		}  
		
		if(m_Object2Name.getValue() != "")
		{
			objectTwo = SoNode::getByName(m_Object2Name.getValue());
			//qCoord.connectFrom(&(((SoPointKit*)objectTwo)->pCoord));
			objectTwoSensor->attach(&((Object3dKit*)objectTwo)->ChangeObject);
		}
		
		Build(bool_high);
		
	}
	else 
	{
		Object3dKit::setUpConnections(onOff, doItAlways);
		
		if (objectOneSensor)
			objectOneSensor->detach();
		if (objectTwoSensor)
			objectTwoSensor->detach();
	}
	return !(connectionsSetUp = onOff); 
}

void SoBoolKit::Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum)
{
	RemoveAllCallbacks();
	
	if(m_Object2Name.getValue() != "")
	{
		Object3dKit* obj = (Object3dKit*)SoNode::getByName(m_Object2Name.getValue());
		obj->undodelete();
	}  
	//  SoSeparator *looknode = (SoSeparator*) SelectionRoot->getChild(childNumber.getValue());
	//SelectionRoot->removeChild(looknode); // removes whole seperator node
	SelectionRoot->removeChild(this); // removes whole kit
	ObjSum->setValue(ObjSum->getValue()-1); 
}

void 
SoBoolKit::setHighlightMaterial(int currentUserID)
{
	SoMaterial* dummy1 = new SoMaterial;
	SoMaterial* dummy2 = new SoMaterial;
	SoMaterial* dummy3 = new SoMaterial;
	SoMaterial* dummy4 = new SoMaterial;
	MaterialConstants::defineBoolMaterials(*W, *dummy2, *dummy3, *N, *dummy4, currentUserID);
}

