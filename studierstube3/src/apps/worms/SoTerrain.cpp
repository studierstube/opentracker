// ===========================================================================
//  (C) 2002 Vienna University of Technology
// ===========================================================================
//  NAME:       SoTerrain.cpp
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    A Node that displays height maps
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    dw      Daniel Wagner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================


#include "SoTerrain.h"

#include <assert.h>


SO_NODE_SOURCE(SoTerrain);


static const double heightFactor = 0.3,
					centerX = 0.5,
					centerY = 0.0,
					centerZ = 0.5;


// This initializes the SoTerrain class.
//
void
SoTerrain::initClass()
{
	// Initialize type id variables.
	SO_NODE_INIT_CLASS(SoTerrain, SoSeparator, "Separator");
}


// Constructor
//
SoTerrain::SoTerrain() : texture(NULL), mesh(NULL), vp(NULL), transform(NULL),
						 lightModel(NULL), complexity(NULL), shapeHints(NULL), drawStyle(NULL),
						 positionSensor(NULL), orientationSensor(NULL),
						 heightMapNameSensor(NULL), textureNameSensor(NULL),
						 heightData(NULL)
{
	SO_NODE_CONSTRUCTOR(SoTerrain);

	createFieldsAndSensors();
	createNodes();

	mapSize = 0;
	numVertices = numIndices = 0;
	sizeX = sizeZ = 1;


	/////////////////////////////////////////////////////////////////
	// just for testing
	/*textureName = "../apps/worms/data/TerrainTexSmall.jpg";
	texture->filename.setValue(textureName.getValue());

	heightMapName = "../apps/worms/data/Heightmap.raw";
	createFromHeightmapFile(heightMapName.getValue().getString());*/
	// testing end
	/////////////////////////////////////////////////////////////////
}


// Destructor
//
SoTerrain::~SoTerrain()
{
}


void
SoTerrain::positionCB(void* data, SoSensor* sensor)
{
	SoTerrain* self = (SoTerrain*)data;

	if(self->transform)
		self->transform->translation.setValue(self->position.getValue());
}


void
SoTerrain::orientationCB(void* data, SoSensor* sensor)
{
	SoTerrain* self = (SoTerrain*)data;

	if(self->transform)
		self->transform->rotation.setValue(self->orientation.getValue());
}


void
SoTerrain::heightmapNameCB(void* data, SoSensor* sensor)
{
	SoTerrain* self = (SoTerrain*)data;

	self->createFromHeightmapFile(self->heightMapName.getValue().getString());
}


void
SoTerrain::textureNameCB(void* data, SoSensor* sensor)
{
	SoTerrain* self = (SoTerrain*)data;

	self->texture->filename.setValue(self->textureName.getValue());
}


void
SoTerrain::createFieldsAndSensors()
{
    SO_NODE_ADD_FIELD(position, (0,0,0));
    SO_NODE_ADD_FIELD(orientation, (SbRotation::identity()));
	SO_NODE_ADD_FIELD(heightMapName,(""));
	SO_NODE_ADD_FIELD(textureName,(""));

    positionSensor = new SoFieldSensor(positionCB, this);
    orientationSensor = new SoFieldSensor(orientationCB, this);
	heightMapNameSensor = new SoFieldSensor(heightmapNameCB, this);
	textureNameSensor = new SoFieldSensor(textureNameCB, this);

    positionSensor->attach(&position);
    orientationSensor->attach(&orientation);
	heightMapNameSensor->attach(&heightMapName);
	textureNameSensor->attach(&textureName);
}

void
SoTerrain::createNodes()
{
	transform = new SoTransform;
	addChild(transform);

	lightModel = new SoLightModel;
	lightModel->model = SoLightModel::BASE_COLOR;
	addChild(lightModel);

	complexity = new SoComplexity;
	complexity->textureQuality = 1.0;
	addChild(complexity);

	shapeHints = new SoShapeHints;
	shapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
	shapeHints->shapeType = SoShapeHints::SOLID;
	shapeHints->faceType = SoShapeHints::CONVEX;
	addChild(shapeHints);

	texture = new SoTexture2;
	addChild(texture);


	//SoDrawStyle* drawStyle = new SoDrawStyle;
	//drawStyle->style = SoDrawStyle::LINES;
	//addChild(drawStyle);


	vp = new SoVertexProperty;
	mesh = new SoIndexedTriangleStripSet;
	mesh->vertexProperty = vp;
	addChild(mesh);
}


bool
SoTerrain::createFromHeightmapFile(const char* nHeightmapFile)
{
	int x,y;

	if(!loadHeightmapFile(nHeightmapFile))
		return false;

	numVertices = mapSize*mapSize;
	numIndices = 0;


	// setup the vertex property
	//
	SbVec3f*  vPos = new SbVec3f[numVertices];
	SbVec2f*  vTex = new SbVec2f[numVertices];
	uint32_t* vCol = new uint32_t[numVertices];
	SbVec3f*  vNorm = new SbVec3f[numVertices];


	for(y=0; y<mapSize; y++)
		for(x=0; x<mapSize; x++)
		{
			double	px = (double)x/(double)mapSize,
					py = (double)y/(double)mapSize;
			int		idx = x+y*mapSize;
			int		c = 255; //colorData ? colorData[idx] : 255;

			assert(idx<numVertices);

			vPos[idx].setValue(sizeX*px - centerX,
							   heightFactor*(double)heightData[idx]/255.0 - centerY,
							   centerZ - sizeZ*py);
			vTex[idx].setValue(px,1.0-py);
			vCol[idx] = ((c) | (c<<8) | (c<<16) | (c<<24));
			vNorm[idx].setValue(0, 0, 1);
		}

	vp->vertex.setValues(0, numVertices, vPos);
	vp->texCoord.setValues(0, numVertices, vTex);
	vp->orderedRGBA.setValues(0, numVertices, vCol);
	vp->normal.setValues(0, numVertices, vNorm);

	vp->materialBinding = SoVertexProperty::PER_VERTEX_INDEXED;

	delete vPos;
	delete vTex;
	delete vCol;
	delete vNorm;


	// setup the tri-mesh
	//
	int* vIdx = new int[mapSize*(mapSize+1)*2+1];

	for(y=0; y<mapSize-1; y++)
	{
		if(numIndices>0)							// add degenerated triangles to merge strips
		{
			vIdx[numIndices++] = vIdx[numIndices-1];
			vIdx[numIndices++] = (y+1)*mapSize;
		}

		for(x=0; x<mapSize; x++)
		{
			vIdx[numIndices++] = (y+1)*mapSize + x;
			vIdx[numIndices++] = y*mapSize + x;
		}
	}

	vIdx[numIndices] = -1;

	mesh->coordIndex.setValues(0, numIndices+1, vIdx);
	mesh->textureCoordIndex.setValues(0, numIndices+1, vIdx);
	delete vIdx;

	mesh->materialIndex.setNum(1);
	mesh->materialIndex.set1Value(0, -1);

	printf("INFO: terrain created from heightmap file '%s'\n", nHeightmapFile);
	return true;
}


bool
SoTerrain::loadHeightmapFile(const char* nFileName)
{
	FILE*	fp=fopen(nFileName, "rb");
	int		size, read;


	if(fp==NULL)
	{
		printf("ERROR: can not load heightmap file '%s': file not found\n", nFileName);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	mapSize = (int)sqrt((float)size);
	if(mapSize*mapSize != size)
	{
		printf("ERROR: heightmap file '%s': is not of squared size\n", nFileName);
		fclose(fp);
		return false;
	}

	if(heightData)
		delete heightData;
	heightData = new unsigned char[size];

	read = fread(heightData, 1, size, fp);

	if(read!=size)
	{
		printf("ERROR: loading raw image '%s' with %d x %d pixels (%d bytes) failed: file not large enough\n", nFileName, mapSize, mapSize, size);
		delete heightData;
		heightData = NULL;
		fclose(fp);
		return NULL;
	}

	printf("raw image '%s' (%dx%d) loaded\n", nFileName, mapSize, mapSize);
	return true;
}


bool
SoTerrain::getHeight(double nXPos, double nZPos, double& nHeight) const
{
	int x = (int)((nXPos+centerX) * mapSize/sizeX),
		z = (int)((centerZ-nZPos) * mapSize/sizeZ),
		idx = x+z*mapSize;

	if(x<0 || x>=mapSize || z<0 || z>=mapSize)
		return false;

	assert(idx>=0 && idx<mapSize*mapSize);

	nHeight = heightFactor*(double)heightData[idx]/255.0 - centerY;
	return true;
}




//////////////////////////////////////////////////////////////////////////////////////////////

	/*SoNormalBinding *normalBinding = new SoNormalBinding;
	normalBinding->value = SoNormalBinding::PER_VERTEX;
	addChild(normalBinding);

	// create vertex & texture coordinates and normals
	//
	coords = new SoCoordinate3;
	addChild(coords);

	texCoords = new SoTextureCoordinate2;
	addChild(texCoords);

	normals = new SoNormal;
	addChild(normals);

	SbVec3f* vPos = new SbVec3f[numVertices];
	SbVec2f* vTex = new SbVec2f[numVertices];
	SbVec3f* vNorm = new SbVec3f[numVertices];

	for(y=0; y<sizeZ; y++)
		for(x=0; x<sizeX; x++)
		{
			double	px = (double)x/(double)sizeX,
					py = (double)y/(double)sizeZ;
			int		idx = x+y*sizeX;

			assert(idx<numVertices);

			vPos[idx].setValue(px, (double)heightData[x+y*sizeX]/255.0, py);
			vTex[idx].setValue(px,py);
			vNorm[idx].setValue(0, 1, 0);
		}

	coords->point.setValues(0, numVertices, vPos);
	texCoords->point.setValues(0, numVertices, vTex);
	normals->vector.setValues(0, numVertices, vNorm);

	delete vPos;
	delete vTex;
	delete vNorm;*/
