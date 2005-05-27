// includes vertex, triangle class for defining the topology of the face

#ifndef _TOPOLOGY_H_
#define _TOPOLOGY_H_

#include <math.h>

class CVertex{

protected:
    
    float m_x, m_y, m_z;    // 3D space coordinates
    float m_u, m_v;     // texture coordinate

public:

    CVertex();
    CVertex(float x,float y,float z);
    CVertex(const CVertex &vertex);  // copy constructor
    CVertex & operator= (const CVertex &vertex);
    CVertex operator* (float c);
    CVertex operator/ (float c);
    CVertex operator+ (CVertex &vertex);
    CVertex operator- (CVertex &vertex);
    void operator+= (const CVertex &vertex);
    void operator-= (const CVertex &vertex);

    float& X() {return m_x;}
    float& Y() {return m_y;}
    float& Z() {return m_z;}
    float& U() {return m_u;}
    float& V() {return m_v;}

    void Set(float x,float y,float z);
    void Transform(CVertex &vector,float amount);
    void Normalize();
};

//----------------------------------------------------------------------

class CTriangle {

protected:

    int m_v0,m_v1,m_v2;

public:
    
    CTriangle();
    void Set(int v0,int v1,int v2);
    int GetVertexIndex(short index);
};

#endif