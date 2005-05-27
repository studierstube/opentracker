// ===========================================================================
//  (C) 2002 Vienna University of Technology
// ===========================================================================
//  NAME:       SoTerrain.cpp
//  TYPE:       c++ header file
//  PROJECT:    Studierstube
//  CONTENT:    A Node that displays height maps
//  VERSION:    0.1
// ===========================================================================
//  AUTHORS:    dw      Daniel Wagner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================


#ifndef __SOTERRAIN_HEADERFILE__
#define __SOTERRAIN_HEADERFILE__


#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/sensors/SoFieldSensor.h>


class SoTerrain : public SoSeparator
{
	SO_NODE_HEADER(SoTerrain);

public:
	// Initializes this class.
	static void initClass();

	// Constructor
	SoTerrain();

	bool getHeight(double nXPos, double nZPos, double& nHeight) const;


    SoSFVec3f		position;
    SoSFRotation	orientation;
	SoSFString		heightMapName;
	SoSFString		textureName;


private:
	// Destructor
	virtual ~SoTerrain();

	void createFieldsAndSensors();
	void createNodes();
	bool createFromHeightmapFile(const char* nFileName);
	bool loadHeightmapFile(const char* nFileName);

    static void positionCB(void* data, SoSensor* sensor);
    static void orientationCB(void* data, SoSensor* sensor);
	static void heightmapNameCB(void* data, SoSensor* sensor);
	static void textureNameCB(void* data, SoSensor* sensor);


    SoFieldSensor* positionSensor;
    SoFieldSensor* orientationSensor;
	SoFieldSensor* heightMapNameSensor;
	SoFieldSensor* textureNameSensor;


	SoLightModel*				lightModel;
	SoComplexity*				complexity;
	SoShapeHints*				shapeHints;
	SoDrawStyle*				drawStyle;
	SoTexture2*					texture;
	SoIndexedTriangleStripSet*	mesh;
	SoVertexProperty*			vp;
	SoTransform*				transform;

	int				mapSize,
					numVertices, numIndices;

	unsigned char	*heightData;

	double			sizeX,sizeZ;
};


#endif //__SOTERRAIN_HEADERFILE__

