// CFaceModel.h : interface of the CFaceModel class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _FACEMODEL_H_
#define _FACEMODEL_H_

#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <math.h>
#include "Topology.h"
#include "utils.h"
#include "linkedlist.h"

class CFaceModel {

protected:

    // *** topology and color data
    CVertex   m_vertex[MAX_VERTEX];         // *** original vertex positions
    CVertex   m_conformation[MAX_VERTEX];   // *** conformation vertex positions
    CVertex   m_workVertex[MAX_VERTEX];     // *** transformed vertex positions
    CTriangle m_triangle[MAX_TRIANGLE];     // *** original link information
    CVertex   m_normalVector[MAX_VERTEX];   // *** normal vector of vertices

    float m_min[3],m_max[3]; // *** for bounding box calculation

    // *** face indices
    int m_vertexIndex[MAX_VERTEX];      // *** face index of vertices
    int m_triangleIndex[MAX_TRIANGLE];  // *** face index of triangles

    // *** topology help variables
    LinkedList<int> m_normalVectorList[MAX_FACE_VERTEX];
    bool m_smoothNormalEnabled[MAX_FACE_VERTEX];
    
    // *** texture image
    //AUX_RGBImageRec *m_TextureImage;

    // *** initialization flag
    bool m_conformationValid;
    bool m_initialized;

    // *** eye parameters
    CVertex m_leftEyeCenter,m_rightEyeCenter;

    // *** conformation parameters
    bool m_glasses;
    int  m_transparency;

public: 
    
    CFaceModel();
    ~CFaceModel();

    // *** init
    bool Init(char *path);

    // *** render face
    bool Draw(CVertex* deform,float *transform,float *color,bool smoothEnabled,bool coloringEnabled);
    void DrawFace(float *transform,float *color,bool smoothEnabled,bool coloringEnabled);
    void DrawTest(float zoom);
    void DrawAuxiliary(float *transform,float *color,bool coloringEnabled);
    void DrawAccessories(float *transform,float *color,bool coloringEnabled);
    
    // *** load init data from file
    bool LoadVertices(char *fileName);
    bool LoadTriangles(char *fileName);

    // *** set conformation parameters
    void SetConformation(CVertex *vertex,bool glasses,int transparency);

    // *** helper functions
    CVertex NormalVector(CTriangle *triangle);
    float* GetColors(float *color);
    void GetMinMax(float *min,float *max);
};

#endif