// Topology.cpp
//
// includes vertex, triangle class for defining the topology of the face

#include "Topology.h"

CVertex::CVertex() {
	
	m_x=m_y=m_z=0.0f;
	m_u=m_v=0;
}

CVertex::CVertex(float x,float y,float z){

	m_x=x;
	m_y=y;
	m_z=z;
}

// copy constructor
CVertex::CVertex(const CVertex &vertex){
	
	m_x=vertex.m_x;
	m_y=vertex.m_y;
	m_z=vertex.m_z;
}

CVertex & CVertex::operator= (const CVertex &vertex){

	m_x=vertex.m_x;
	m_y=vertex.m_y;
	m_z=vertex.m_z;

	return *this;
}

CVertex CVertex::operator* (float c){
	
	return CVertex(m_x*c,m_y*c,m_z*c);
}

CVertex CVertex::operator/ (float c){
	
	return CVertex(m_x/c,m_y/c,m_z/c);
}

CVertex CVertex::operator+ (CVertex &vertex){

	return CVertex(	m_x+vertex.m_x,m_y+vertex.m_y,m_z+vertex.m_z);
}

CVertex CVertex::operator- (CVertex &vertex){

	return CVertex(	m_x-vertex.m_x,m_y-vertex.m_y,m_z-vertex.m_z);
}

void CVertex::operator+= (const CVertex &vertex){
	
	m_x+=vertex.m_x;
	m_y+=vertex.m_y;
	m_z+=vertex.m_z;
}

void CVertex::operator-= (const CVertex &vertex){
	
	m_x-=vertex.m_x;
	m_y-=vertex.m_y;
	m_z-=vertex.m_z;
}

void CVertex::Set(float x,float y,float z){
	
	m_x=x;
	m_y=y;
	m_z=z;
}

void CVertex::Transform(CVertex &vector,float amount){

	m_x+=vector.X()*amount;
	m_y+=vector.Y()*amount;
	m_z+=vector.Z()*amount;
}

void CVertex::Normalize(){
	
	float length=(float)sqrt(m_x*m_x+m_y*m_y+m_z*m_z);

	if (length==0.0f) length=1.0f;
	else {
		m_x/=length;
		m_y/=length;
		m_z/=length;
	}
}


//----------------------------------------------------------------------

CTriangle::CTriangle() {
	
	m_v0=m_v1=m_v2=-1;
}

void CTriangle::Set(int v0,int v1,int v2){
	
	m_v0=v0;
	m_v1=v1;
	m_v2=v2;
}

int CTriangle::GetVertexIndex(short index){

	switch (index){		
		case 0:
			return m_v0;
		case 1:
			return m_v1;
		case 2:
			return m_v2;
		default:
			return -1;
	}
}
