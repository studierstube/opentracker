/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* SoViewingFrustum.h contains the definition of SoViewingFrustum,
 * a nodekit representing and visualizing a viewing frustum
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoViewingFrustum.cxx 3866 2004-10-08 19:36:44Z bara $
 * @file
 */
/* ======================================================================= */

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/elements/SoModelMatrixElement.h>

#include <stbapi/util/math2.h>

#include "SoViewingFrustum.h"

SO_KIT_SOURCE(SoViewingFrustum);

void SoViewingFrustum::initClass()
{
    SO_KIT_INIT_CLASS(SoViewingFrustum, SoBaseKit, "BaseKit");
}

SoViewingFrustum::SoViewingFrustum()
{
    SO_KIT_CONSTRUCTOR(SoViewingFrustum);

    SO_KIT_ADD_CATALOG_ENTRY(root, SoSeparator, FALSE, this, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(offsetTransform, SoTransform, FALSE, root, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(coords, SoCoordinate3, FALSE, root, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(indexedFaceSet, SoIndexedFaceSet, FALSE, root, \0, FALSE);

    SO_KIT_ADD_FIELD(positionOffset,(0,0,0));
    SO_KIT_ADD_FIELD(orientationOffset,(SbRotation::identity()));
    SO_KIT_ADD_FIELD(aspectRatio,(1.0));
    SO_KIT_ADD_FIELD(nearDistance,(1.0));
    SO_KIT_ADD_FIELD(farDistance,(10.0));
    SO_KIT_ADD_FIELD(heightAngle, (float(M_PI)/4.0f)); // 45 degrees

    SO_KIT_INIT_INSTANCE();

    // define the frustum plane in the order of the BB intersection check: near,left,right,bottom,top,far 

    // near plane
    faceIndex[0]=0; faceIndex[1]=1; faceIndex[2]=2; faceIndex[3]=3;
    faceIndex[4]=SO_END_FACE_INDEX;
    // left plane
    faceIndex[5]=5; faceIndex[6]=6; faceIndex[7]=2; faceIndex[8]=1;
    faceIndex[9]=SO_END_FACE_INDEX;
    // right plane
    faceIndex[10]=4; faceIndex[11]=0; faceIndex[12]=3; faceIndex[13]=7;
    faceIndex[14]=SO_END_FACE_INDEX;    
    // bottom plane
    faceIndex[15]=7; faceIndex[16]=3; faceIndex[17]=2; faceIndex[18]=6;
    faceIndex[19]=SO_END_FACE_INDEX;
    // top plane
    faceIndex[20]=5; faceIndex[21]=1; faceIndex[22]=0; faceIndex[23]=4;
    faceIndex[24]=SO_END_FACE_INDEX;    
    // far plane
    faceIndex[25]=7; faceIndex[26]=6; faceIndex[27]=5; faceIndex[28]=4;
    faceIndex[29]=SO_END_FACE_INDEX;

    // indices of the 12 frustum edges
    edgeIndex[0][0]=0; edgeIndex[0][1]=1;
    edgeIndex[1][0]=0; edgeIndex[1][1]=3;
    edgeIndex[2][0]=0; edgeIndex[2][1]=4;
    edgeIndex[3][0]=1; edgeIndex[3][1]=2;
    edgeIndex[4][0]=1; edgeIndex[4][1]=5;
    edgeIndex[5][0]=2; edgeIndex[5][1]=3;
    edgeIndex[6][0]=2; edgeIndex[6][1]=6;
    edgeIndex[7][0]=3; edgeIndex[7][1]=7;
    edgeIndex[8][0]=4; edgeIndex[8][1]=5;
    edgeIndex[9][0]=4; edgeIndex[9][1]=7;
    edgeIndex[10][0]=5; edgeIndex[10][1]=6;
    edgeIndex[11][0]=6; edgeIndex[11][1]=7;

    // indices of the vertices of the 6 BB planes
    bbPlaneIndex[0][0]=0; bbPlaneIndex[0][1]=1; bbPlaneIndex[0][2]=3;    
    bbPlaneIndex[1][0]=1; bbPlaneIndex[1][1]=5; bbPlaneIndex[1][2]=7;    
    bbPlaneIndex[2][0]=4; bbPlaneIndex[2][1]=6; bbPlaneIndex[2][2]=7;    
    bbPlaneIndex[3][0]=2; bbPlaneIndex[3][1]=3; bbPlaneIndex[3][2]=7;
    bbPlaneIndex[4][0]=1; bbPlaneIndex[4][1]=0; bbPlaneIndex[4][2]=4;    
    bbPlaneIndex[5][0]=0; bbPlaneIndex[5][1]=2; bbPlaneIndex[5][2]=6;

    // field sensors
    aspectRatioSensor=new SoFieldSensor(setGeometry,this);
    nearDistanceSensor=new SoFieldSensor(setGeometry,this);
    farDistanceSensor=new SoFieldSensor(setGeometry,this);
    heightAngleSensor=new SoFieldSensor(setGeometry,this);

    // init point to NULL
    point=NULL;

    root=(SoSeparator*)(getPart("root",false));

    setUpConnections(TRUE,TRUE);
}

SoViewingFrustum::~SoViewingFrustum()
{
    // delete sensors
    delete aspectRatioSensor;
    delete nearDistanceSensor;
    delete farDistanceSensor;
    delete heightAngleSensor;
}

SbBool SoViewingFrustum::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;

    if (onOff) {
        // attach sensors
        aspectRatioSensor->attach(&aspectRatio);
        nearDistanceSensor->attach(&nearDistance);
        farDistanceSensor->attach(&farDistance);
        heightAngleSensor->attach(&heightAngle);

        SoTransform *trans=SO_GET_ANY_PART(this,"offsetTransform",SoTransform);
        trans->translation.connectFrom(&positionOffset);
        trans->rotation.connectFrom(&orientationOffset);

        setGeometry(this,NULL);
    }
    else {
        // detach sensors
        aspectRatioSensor->detach();
        nearDistanceSensor->detach();
        farDistanceSensor->detach();
        heightAngleSensor->detach();
    }

    return !(connectionsSetUp = onOff);
}

void SoViewingFrustum::GLRender(SoGLRenderAction *action)
{
    matrix=SoModelMatrixElement::get(action->getState());
    SoBaseKit::GLRender(action);
}

void SoViewingFrustum::setGeometry(void *data,SoSensor *sensor)
{
    SoViewingFrustum *vf=(SoViewingFrustum*)data;

    float ha=vf->heightAngle.getValue();
    if (((SoFieldSensor*)sensor)==vf->heightAngleSensor && (ha<0.0f || ha>M_PI)) {
        printf("VIEWINGFRUSTUM: warning: heightAngle (%f) is out of bounds [0,PI], clamping to bounds\n",ha);
        if (ha<0.0f) vf->heightAngle.setValue(ha=0.0f);
        else if (ha>M_PI) vf->heightAngle.setValue(float(M_PI));
    }

    float ar=vf->aspectRatio.getValue();
    if (((SoFieldSensor*)sensor)==vf->aspectRatioSensor && ar<0.0f) {
        printf("VIEWINGFRUSTUM: warning: aspectRatio (%f) has to be >=0.0f, clamping to 0.0f\n",ar);
            vf->aspectRatio.setValue(0.0f);
            ar=0.0f;
    }

    // TODO: what if nd or fd < 0 ??? --> currently taking the absolute value
    float nd=(float)fabs(vf->nearDistance.getValue());
    float fd=(float)fabs(vf->farDistance.getValue());
    if ((((SoFieldSensor*)sensor)==vf->nearDistanceSensor || ((SoFieldSensor*)sensor)==vf->farDistanceSensor) && nd>fd) {
        printf("VIEWINGFRUSTUM: warning: far distance (%f) should be larger than near distance (%f), swapping near/far\n",nd,fd);
        float tmp=fd;
        vf->farDistance.setValue(fd=nd);
        vf->nearDistance.setValue(nd=tmp);
    }

    float nearYCoord=nd*float(tan(ha/2.0f));
    float farYCoord=fd*float(tan(ha/2.0f));
    float nearXCoord=nearYCoord*ar;
    float farXCoord=farYCoord*ar;

    if (vf->point) {
        delete[](vf->point);
        vf->point=NULL;
    }
    vf->point=new SbVec3f[8];

    vf->point[0].setValue(-nearXCoord,nearYCoord,nd);
    vf->point[1].setValue(nearXCoord,nearYCoord,nd);
    vf->point[2].setValue(nearXCoord,-nearYCoord,nd);
    vf->point[3].setValue(-nearXCoord,-nearYCoord,nd);
    vf->point[4].setValue(-farXCoord,farYCoord,fd);
    vf->point[5].setValue(farXCoord,farYCoord,fd);
    vf->point[6].setValue(farXCoord,-farYCoord,fd);
    vf->point[7].setValue(-farXCoord,-farYCoord,fd);

    for (int i=0;i<6;i++){
        vf->plane[i]=SbPlane(
            vf->point[vf->faceIndex[i*5]],
            vf->point[vf->faceIndex[i*5+1]],
            vf->point[vf->faceIndex[i*5+2]]
        );
    }

    for (int j=0;j<12;j++){
        vf->edge[j].setValue(vf->point[vf->edgeIndex[j][0]],vf->point[vf->edgeIndex[j][1]]);
    }

    SoCoordinate3 *cd= SO_GET_ANY_PART(vf,"coords",SoCoordinate3);
    cd->point.setValues(0,8,vf->point);

    SoIndexedFaceSet *ifs= SO_GET_ANY_PART(vf,"indexedFaceSet",SoIndexedFaceSet);
    ifs->coordIndex.setValues(0,5*6,vf->faceIndex);
}

bool SoViewingFrustum::intersectBB(const SbVec3f &min,const SbVec3f &max)
{
    SbVec3f t,s;
    SbRotation r,so;
    matrix.getTransform(t,r,s,so);

    int i,j,k;
    unsigned short result[8];

    SbVec3f bbpoint[8];
    for (i=0;i<8;i++){
        // derive BB vertices from the min and max point of the BB
        bbpoint[i][0]=(i & 4) ? max[0] : min[0];
        bbpoint[i][1]=(i & 2) ? max[1] : min[1];
        bbpoint[i][2]=(i & 1) ? max[2] : min[2];

        // transform point with inverse transformation of the frustum
        (r*orientationOffset.getValue()).inverse().multVec(bbpoint[i],bbpoint[i]);
        bbpoint[i]-=t+positionOffset.getValue();

        // reset intersection result for each vertex
        result[i]=0;
    }

    unsigned short sum=63; // 63 = 111111 binary

    // 1.) check whether one of the BB vertices is inside the frustum

    // iterate through the plane of the frustum
    for (i=0;i<6;i++){
        // iterate through the point of the BB
        for (j=0;j<8;j++){
            // check whether vertex is inside or outside the currently examined plane
            if (plane[i].isInHalfSpace(bbpoint[j])) result[j] |= (int)(pow(2,i)); // outside
            else sum &= ~(int)(pow(2,i)); // inside
        }
    }
    // none of the vertices are inside the current plane --> trivial reject
    if (sum) return false;
    
    // if there is a vertex that is inside all the frustum plane, it's inside the frustum --> trivial accept
    for (i=0;i<8;i++) if (!result[i]) return true;

    // 2.) test for frustum protrusion through an OBB face by checking each edge in the frustum for intersection with an OBB face
    
    // iterate through the edges of the frustum
    for (i=0;i<12;i++){
        // iterate through the faces of the BB
        for (j=0;j<6;j++){
            // intersection point
            SbVec3f p;
            // check whether the edge intersect the currently examined face of the BB (is intersection point between edge vertices)
            SbPlane bbPlane(bbpoint[bbPlaneIndex[j][0]],bbpoint[bbPlaneIndex[j][1]],bbpoint[bbPlaneIndex[j][2]]);
            if (bbPlane.intersect(edge[i],p)){ // line is not parallel with the plane
                for (k=0;k<3;k++){
                    if (p[k]<m2fmin(point[edgeIndex[i][0]][k],point[edgeIndex[i][1]][k]) ||
                        p[k]>m2fmax(point[edgeIndex[i][0]][k],point[edgeIndex[i][1]][k])) 
                        break;
                }
                if (k==3) return true;
            }
        }
    }

    // 3.) check for containment of the frustum inside the OBB by checking if the origin of the frustum is inside the BB
    SbVec3f frustumCenter(0.0f,0.0f,nearDistance.getValue()+(farDistance.getValue()-nearDistance.getValue())/2.0f);
    (r*orientationOffset.getValue()).multVec(frustumCenter,frustumCenter);
    frustumCenter+=t+positionOffset.getValue();
    for (i=0;i<3;i++) if (frustumCenter[i]<min[i] || frustumCenter[i]>max[i]) break;
    if (i==3) return true;

    // 4.) all tests have failed, reject because OBB does not overlap the viewing frustum
    return false;
}

bool SoViewingFrustum::intersectPoint(const SbVec3f &p)
{
    SbVec3f t,s;
    SbRotation r,so;
    matrix.getTransform(t,r,s,so);

    // transform point with inverse transformation of the frustum
    SbVec3f p_rot;
    (r*orientationOffset.getValue()).inverse().multVec(p,p_rot);
    p_rot-=t+positionOffset.getValue();

    // iterate through the plane of the frustum
    for (int i=0;i<6;i++){
        // check whether vertex is inside or outside the currently examined plane
        if (plane[i].isInHalfSpace(p_rot)) return false;
    }

    return true;
}