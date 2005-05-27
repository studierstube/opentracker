#ifndef _BIN2IV_H_
#define _BIN2IV_H_

#include <string.h>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoPointSet.h>

#include "../../../tools/avstool/Geom2SerIVProtocol.h"
#include "stbcomm.h"


/******************************************************************************
Class:                  CBuildTree
Purpose:                Build an Inventor subgraph out of our binary IFF
Public members: SoSeparator *B2S(char *, int);
******************************************************************************/
class CBuildTree {
public:
void setSeparator(SoSeparator *mySep);

int Parse(char *);

static SoNode *NewLine(const SbVec3f *from,const SbVec3f *to,SbColor  *col);
static SoNode *NewPoint(const SbVec3f *pos,SbColor  *col);
static void setLinePos(SoSeparator *line,const SbVec3f *from,const SbVec3f *to);
static void setPointPos(SoSeparator *point,const SbVec3f *pos);
static void addRing(int steps, float radius,SbVec3f P,SbVec3f start,SbVec3f normal, SbColor *col,SoVertexProperty *VP);
static SoNode *CBuildTree::NewPolyLine(float radius, int numsides, int numVertices, SbVec3f *vertexPositions,SoBaseColor *col);


private:

SoCoordinate3 *NewCoordinate3(char *);
SoNormal *NewNormal(char *);
SoNormalBinding *NewNormalBinding(char *);
SoMaterialBinding *NewMaterialBinding(char *);
SoSphere *NewSphere(char *);
SoFont *NewFont(char *);
SoTranslation *NewTranslation(char *);
SoDrawStyle *NewDrawStyle(char *);
SoText2 *NewText2(char *);
SoBaseColor *NewBaseColor(char *);
//static SoIndexedFaceSet *NewIndexedLineSet(char *);
SoNode *CBuildTree::NewPointSet(char *buf);
SoNode *NewIndexedLineSet(char *);
SoTriangleStripSet *NewTriangleStripSet(char *);
SoNode *NewLineSet(char *);
SoQuadMesh *NewQuadMesh(char *);
SoMaterial *NewMaterial(char *);
static SoSeparator *getSepBevore(SoSeparator *ssep,SoNode *toFind);

static char* toValidName(char *buf);

SoCoordinate3 *lastSoCoordinate3;
SoNormal *lastSoNormal;
SoBaseColor   *lastSoBaseColor;
SoSeparator *addSep;
SoSeparator *rootSep;
};

/* convenience functions */


int GetInt(char *);
float GetFloat(char *);
unsigned char GetByte(char *);

#endif
