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
/* MiniView.cxx contains the implementation of MiniView
 *
 * @author Istvan Barakonyi
 *
 * $Id: MiniView.cxx 4209 2005-05-19 09:06:39Z bara $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoCoordinate3.h"
#include "Inventor/nodes/SoMaterial.h"
#include "Inventor/nodes/SoMaterialBinding.h"
#include "Inventor/nodes/SoQuadMesh.h"
#include "Inventor/nodes/SoDrawStyle.h"
#include "Inventor/nodes/SoPointSet.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// own classes
#include "MiniView.h"
#include "SceneManager.h"

SO_KIT_SOURCE(MiniView);

void MiniView::initClass()
{
   SO_KIT_INIT_CLASS(MiniView, SoBaseKit, "BaseKit");
}

MiniView::MiniView()
{
    SO_KIT_CONSTRUCTOR(MiniView);

    SO_KIT_ADD_CATALOG_ENTRY(tileRoot, SoSeparator, FALSE, this, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(coordinateMesh, SoCoordinate3, FALSE, tileRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(materialBindingMesh, SoMaterialBinding, FALSE, tileRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(materialMesh, SoMaterial, TRUE, tileRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(mesh, SoQuadMesh, FALSE, tileRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(playerRoot, SoSeparator, FALSE, this, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(materialBindingPoint, SoMaterialBinding, FALSE, playerRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(materialPoint, SoMaterial, TRUE, playerRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(coordinatePoint, SoCoordinate3, FALSE, playerRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(drawStylePoint, SoDrawStyle, FALSE, playerRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(pointSet, SoPointSet, FALSE, playerRoot, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(pathRoot, SoSeparator, FALSE, this, \0, FALSE);

    SO_KIT_ADD_FIELD(maxCoordinate,  (0.0f,0.0f));
    SO_KIT_ADD_FIELD(tileSize,  (0.0f));
    SO_KIT_ADD_FIELD(globalScale,  (1.0f));
    SO_KIT_ADD_FIELD(defaultTileColor,  (1.0f,1.0f,1.0f));
    SO_KIT_ADD_FIELD(userTileColor,  (1.0f,1.0f,1.0f));
    SO_KIT_ADD_FIELD(fixTileColor,  (1.0f,1.0f,1.0f));
    SO_KIT_ADD_FIELD(positionColor,  (1.0f,1.0f,1.0f));
    SO_KIT_ADD_FIELD(pathColor,  (1.0f,1.0f,1.0f)); // TODO: implement!
    SO_KIT_ADD_FIELD(transparency,  (0.0f));
    SO_KIT_ADD_FIELD(occupationInfo,  (SceneManager::INVALID,SceneManager::INVALID));
    SO_KIT_ADD_FIELD(currentPosition,  (SceneManager::INVALID,SceneManager::INVALID));
    SO_KIT_ADD_FIELD(nextPosition,  (SceneManager::INVALID,SceneManager::INVALID));

    SO_KIT_INIT_INSTANCE();

    ((SoMaterialBinding*)(SO_GET_ANY_PART(this,"materialBindingMesh",SoMaterialBinding)))->value.setValue(SoMaterialBinding::PER_FACE);
    ((SoMaterialBinding*)(SO_GET_ANY_PART(this,"materialBindingPoint",SoMaterialBinding)))->value.setValue(SoMaterialBinding::PER_PART);
    ((SoDrawStyle*)(SO_GET_ANY_PART(this,"drawStylePoint",SoDrawStyle)))->pointSize.setValue(7.0f);

    maxCoordinateSensor=new SoFieldSensor(buildGridCB,this);
    tileSizeSensor=new SoFieldSensor(buildGridCB,this);
    occupationInfoSensor=new SoFieldSensor(occupationInfoCB,this);
    currentPositionSensor=new SoFieldSensor(userPositionCB,this);
    nextPositionSensor=new SoFieldSensor(userPositionCB,this);

    setUpConnections(TRUE,TRUE);
}

MiniView::~MiniView()
{
    delete maxCoordinateSensor;
    delete tileSizeSensor;
    delete occupationInfoSensor;
    delete currentPositionSensor;
    delete nextPositionSensor;
}

SbBool MiniView::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    
    if (onOff) {
        maxCoordinateSensor->attach(&maxCoordinate);
        tileSizeSensor->attach(&tileSize);
        occupationInfoSensor->attach(&occupationInfo);
        currentPositionSensor->attach(&currentPosition);
        nextPositionSensor->attach(&nextPosition);

        // connect external transparency parameter to material's transpareny field
        SO_GET_ANY_PART(this,"materialMesh",SoMaterial)->transparency.connectFrom(&transparency);
        
        buildGridCB(this,NULL);
        occupationInfoCB(this,NULL);
        userPositionCB(this,NULL);
    }
    else {
        maxCoordinateSensor->detach();
        tileSizeSensor->detach();
        occupationInfoSensor->detach();
        currentPositionSensor->detach();
        nextPositionSensor->detach();
    }

    return !(connectionsSetUp = onOff);
}

void MiniView::buildGridCB(void *data,SoSensor *sensor)
{
    MiniView *mv=(MiniView*)data;
    SoQuadMesh *qm=SO_GET_ANY_PART(mv,"mesh",SoQuadMesh);
    SoCoordinate3 *coord=SO_GET_ANY_PART(mv,"coordinateMesh",SoCoordinate3);
    SoMaterial *mat=SO_GET_ANY_PART(mv,"materialMesh",SoMaterial);
    SbVec2f mc=mv->maxCoordinate.getValue();

    if (mc[0]<0 || mc[1]<0){
        printf("MINIVIEW: error: negative maximum coordinate value\n");
        return;
    }
    if (mv->tileSize.getValue()<0){
        printf("MINIVIEW: error: negative tile size\n");
        return;
    }
    
    // set number of mesh points
    coord->point.setNum((int)((mc[0]+1)*(mc[1]+1)));

    qm->verticesPerRow=(int)mc[0]+1;
    qm->verticesPerColumn=(int)mc[1]+1;
    
    for (int i=0;i<=mc[1];i++){
        for (int j=0;j<=mc[0];j++){
            coord->point.set1Value(
                i*(int)(mc[0]+1)+j,
                (j-mc[0]/2.0f)*mv->tileSize.getValue()*mv->globalScale.getValue(),
                (mc[1]/2.0f-i)*mv->tileSize.getValue()*mv->globalScale.getValue(),
                0
            );
            // initialize color to default color
            int index=i*(int)(mc[0])+j;
            mat->diffuseColor.set1Value(index,mv->defaultTileColor.getValue());
        }
    }
}

void MiniView::occupationInfoCB(void *data,SoSensor *sensor)
{
    MiniView *mv=(MiniView*)data;
    SoMaterial *mat=SO_GET_ANY_PART(mv,"materialMesh",SoMaterial);
    SbVec2f mc=mv->maxCoordinate.getValue();

    for (int i=0;i<mc[1];i++){
        for (int j=0;j<mc[0];j++){
            int index=i*(int)(mc[0])+j;
            if (index>=mv->occupationInfo.getNum()) return; // there is not enough occupation data --> exit
            SbVec2f info=mv->occupationInfo[index];
            int userID=(int)(info[0]);
            // if user ID is valid, set appropriate user color
            if (userID>=0) {
                mat->diffuseColor.set1Value(
                    index,
                    userID<mv->userTileColor.getNum() ? mv->userTileColor[userID] : SbColor(0,0,0)
                );
            }
            // if user ID is invalid: if tile type is valid, set fixed tile color, otherwise set default (water) color
            else {
                if (info[1]>=0.0f) mat->diffuseColor.set1Value(index,mv->fixTileColor.getValue());
                else mat->diffuseColor.set1Value(index,mv->defaultTileColor.getValue());
            }            
        }
    }
}

void MiniView::userPositionCB(void *data,SoSensor *sensor)
{
    MiniView *mv=(MiniView*)data;
    SoPointSet *ps=SO_GET_ANY_PART(mv,"pointSet",SoPointSet);
    SoCoordinate3 *coord=SO_GET_ANY_PART(mv,"coordinatePoint",SoCoordinate3);
    SoMaterial *mat=SO_GET_ANY_PART(mv,"materialPoint",SoMaterial);
    SbVec2f mc=mv->maxCoordinate.getValue();

    // set number of player points ( multiply by 2 because of next user position dots)
    ps->numPoints=(mv->currentPosition.getNum()==1 && mv->currentPosition[0][0]<0) ? 0 : mv->currentPosition.getNum()*2;

    // set point coordinates
    for (int i=0;i<ps->numPoints.getValue()/2;i++){
        SbVec2f curPos=mv->currentPosition[i];
        if (curPos[0]>=0 || curPos[1]>=0){
            coord->point.set1Value(
                i,
                (curPos[0]-mc[0]/2.0f+0.5f)*mv->tileSize.getValue()*mv->globalScale.getValue(),
                (mc[1]/2.0f-curPos[1]-0.5f)*mv->tileSize.getValue()*mv->globalScale.getValue(),
                0.01f*mv->globalScale.getValue()
            );
            mat->diffuseColor.set1Value(i,i<mv->positionColor.getNum() ? mv->positionColor[i] : 
                (mv->positionColor.getNum() ? mv->positionColor[mv->positionColor.getNum()-1] : SbColor(0,0,0))
            );
        }
        /*SbVec2f nextPos=mv->nextPosition[i];
        if (nextPos[0]>=0 || nextPos[1]>=0){
            coord->point.set1Value(
                i*2,
                (nextPos[0]-mc[0]/2.0f+0.5f)*mv->tileSize.getValue(),
                (mc[1]/2.0f-nextPos[1]-0.5f)*mv->tileSize.getValue(),
                0.01f
            );
            mat->diffuseColor.set1Value(i*2,SbColor(1,1,0));
        }*/
    }
}

