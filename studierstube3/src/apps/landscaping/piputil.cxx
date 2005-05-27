// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       piputil
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Auxilliary function for landscaping demo
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:17:53  gh      last modification
//  17-sep-98 21:47:52  ds      created
// ===========================================================================
//piputil.cxx

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <stbapi/misc/wstudierstube.h>
#include "piputil.h"

//-----------------------------------------------------------------------------
// get the positon of a given node
// node: node to find positon for
// returns: position of that node

SbVec3f
getNodePosition(SoNode* node)
{
    static SbViewportRegion vp;
    static SoGetBoundingBoxAction bba(vp);
    bba.apply(node);
    SbMatrix matrix = bba.getXfBoundingBox().getTransform();
    SbVec3f t, s; SbRotation r, so;
    matrix.getTransform(t,r,s,so);
    return t;
}

//-----------------------------------------------------------------------------
// line segment (a-b) vs. (c-d) intersection test (points given in xz plane)
// assumes that c-d is horizontal
// returns: true if intersection

SbBool
intersectLineSegments(const SbVec3f a, const SbVec3f b, 
                      const SbVec3f c, const SbVec3f d)
{
    float t,u;
    float g0 = b[0]-a[0]; 
    float g2 = b[2]-a[2]; 
    float k0 = d[0]-c[0]; //assuming horizontal line, always large number 
    float k2 = d[2]-c[2]; //assuming horizontal line, always 0
    float x0 = c[0]-a[0];
    float x2 = c[2]-a[2]; 
    if(g2==0.0) return FALSE; //parallel horizontal lines
    u = (x2*g0/g2-x0)/k0;
    t = (x2+u*k2)/g2;
    return 0<t && t<1 && 0<u && u<1; //if t and u in interval [0,1]->intersect
}

//-----------------------------------------------------------------------------
// non-zero winding number rule point-in-polygon test in the xz-plane
// point: point to test
// poly: polygon to test
// returns: true if point in polygon

SbBool
isPointInPolygon(const SbVec3f point, const SoCoordinate3* poly)
{
    int num = poly->point.getNum();
    SbVec3f point2 = point + SbVec3f(1000,0,0); //large offset in x
    int count = 0;
    for(int i=0; i<num; i++)
        if(intersectLineSegments(
            poly->point[i],poly->point[(i+1)%num],point,point2)) count++;
    return count % 2;
}

//-----------------------------------------------------------------------------
//compute projection of an artifact's center to the plane defined by the 
//poly points. returns true if projection inisde poly and projection point
//as the passed reference parameter

SbBool                         //returns: true if point inside polygon
projectArtifactToPip(
    SoArtifactKit* artifact,   //pointer to the artifact
    SbVec3f eye,               //eye point in pip space
    SbMatrix pipMatrix,        //matrix from world to pip space
    SoCoordinate3* poly,       //polygon
    SbVec3f& intersectionPoint //return value
    )
{
    SbVec3f aposW,aposP; //W=world,P=PIP
    aposW = getNodePosition(artifact);              //find position of artifact
    pipMatrix.inverse().multVecMatrix(aposW,aposP); //in pip space
    SbLine eyeLine(aposP,eye);                      //line eye->artifact
    static SbPlane xzplane(SbVec3f(0,1,0),0);       //XZ plane
    if(xzplane.intersect(eyeLine,intersectionPoint))//if intersect with XZplane
    {
        return isPointInPolygon(intersectionPoint,poly); //true if in polygon
    }
    else
    {
        printf("warning: no eyeline-pip intersection\n"); //may rarely happen
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
// get trafo matrix for a given node
// node: node to find matrix for
// matrix: returns the computed matrix
// returns: true if matrix could be found

SbBool
getMatrix(SoNode* node, SbMatrix& matrix)
{
    assert(node);
    static SoSearchAction sa;
    sa.setNode(node);
    sa.apply(WStudierstube::root);
    SoPath* path;
    if(!(path=sa.getPath())) return FALSE; //warning: finds only first path!
    SoGetMatrixAction ma(WStudierstube::getCurrentViewportRegion());
    ma.apply(path);
    matrix = ma.getMatrix(); //get matrix for given path
    return TRUE;
} 

//-----------------------------------------------------------------------------
// get trafo matrix for a given node
// node: node to find matrix for
// matrix: returns the computed matrix
// returns: true if matrix could be found

SbBool
getPipMatrix(SoNode* node, SbMatrix& matrix)
{
    assert(node);
    static SoSearchAction sa;
    sa.setNode(node);
    sa.apply(WStudierstube::pipSep);
    SoPath* path;
    if(!(path=sa.getPath())) return FALSE; //warning: finds only first path!
    SoGetMatrixAction ma(WStudierstube::getCurrentViewportRegion());
    ma.apply(path);
    matrix = ma.getMatrix(); //get matrix for given path
    return TRUE;
} 

//-----------------------------------------------------------------------------
// get the pen's positon
// returns: pen's position

SbVec3f
getPenPosition()
{
    static SoTransform* penTrafo;
    AVGETNODE(penTrafo,SoTransform,"PEN_TRAFO");
    return penTrafo->translation.getValue();
}

//-----------------------------------------------------------------------------
//eof
