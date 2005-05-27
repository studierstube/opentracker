// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       lasso
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of the lasso tool for the pip
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


#include <stdio.h>
#include <assert.h>

#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSphere.h>

#include <stbapi/interaction/So3DSensor.h>
#include <stbapi/misc/wstudierstube.h>
#include "piputil.h"
#include "lasso.h"

//-----------------------------------------------------------------------------

SoCoordinate3* lassoCoord; //outline of the lasso polygon drawn on pip
SoLineSet* lassoLine;      //line set to visualize lasso
SoSeparator* markerSep;    //separator containing markers for lassoed artifacts

//-----------------------------------------------------------------------------
// add a marker to the set of lassoed artifacts
// vector: offset of marker on the pip

void
addMarker(SbVec3f vector)
{
    NEWNODE(sep,SoSeparator);
    NEWNODE(t,SoTranslation);
    AGETNODE(crossHair,SoSeparator,"CROSS_HAIR");
    t->translation = vector;
    sep->addChild(t);
    sep->addChild(crossHair);
    markerSep->addChild(sep);
}

//-----------------------------------------------------------------------------
// press callback for lasso sensor - start new lasso

void
cbLassoPress(void*, So3DSensor*)
{
    lassoCoord->point.setNum(0);
    lassoLine->numVertices = -1;
//--remove makers
    //for(int i=markerSep->getNumChildren()-1; i>=0; i--) 
    //    markerSep->removeChild(i);
    markerSep->removeAllChildren();
}

//-----------------------------------------------------------------------------
// callback while drawing lasso - if moved sufficiently much from last point,
// add another point to lasso polyline
// sensor: lasso sensor on pip

void
cbLassoMove(void*, So3DSensor* sensor)
{
    const float epsilon = 0.25; //quarter of an inch
    // get matrix to convert world space to lasso (=pip) space
    SbMatrix matrix;
    SbVec3f p, q;
    if(!getMatrix(lassoCoord,matrix))
    {
        printf("warning: could not get matrix for lasso coords\n");
        return;
    }
    p = sensor->translation.getValue();   //get new point
    matrix.inverse().multVecMatrix(p,q);  //convert new point
    q[1]=0;                               //new point always on pip
    int num = lassoCoord->point.getNum();
    // add point only of sufficiently far away
    if(num>0 && (q-lassoCoord->point[num-1]).length() < epsilon) return;
    lassoCoord->point.set1Value(num,q); //add new point
    lassoLine->numVertices = num+1;
}

//-----------------------------------------------------------------------------
// release callback for lasso sensor on pip - close the polyline, find the
// objects that have been lassoed
// sensor: lasso sensor on pip

void
cbLassoRelease(void*, So3DSensor* sensor)
{
    // find matrix to convert artifacts' position to pip space
    SbMatrix matrix;
    SbVec3f eyeW,eyeP,isectP; //W=world,P=PIP
    if(!getMatrix(lassoCoord,matrix))
    {
        printf("warning: could not get matrix for pip coords\n");
        return;
    }
    eyeW = WStudierstube::getMiddleEyePos();
    matrix.inverse().multVecMatrix(eyeW,eyeP);

    // for all artifact see if they are inside the lasso
    for(int i=0; i<artifacts->getNumChildren(); i++)
    {
        GETCHILD(artifact,SoArtifactKit,artifacts,i);
        if(projectArtifactToPip(artifact,eyeP,matrix,lassoCoord,isectP))
        {
            addMarker(isectP);         //add a maker for lassoed artifact
            artifact->selected = TRUE; //select lassoed artifact
        }
    }
}

//-----------------------------------------------------------------------------
// setup lasso stuff

void
setupLasso()
{
    AGETNODE(lassoSensor,So3DSensor,"LASSO_SENSOR");
    lassoSensor->setPressCallback(cbLassoPress,NULL);
    lassoSensor->setMoveCallback(cbLassoMove,NULL);
    lassoSensor->setReleaseCallback(cbLassoRelease,NULL);
    AVGETNODE(lassoCoord,SoCoordinate3,"LASSO_COORD");
    AVGETNODE(lassoLine,SoLineSet,"LASSO_LINE");
    AVGETNODE(markerSep,SoSeparator,"MARKER_SEP");
}

//-----------------------------------------------------------------------------
// reset the lasso

void
resetLasso()
{
    cbLassoPress(NULL,NULL);
}

//-----------------------------------------------------------------------------
//eof
