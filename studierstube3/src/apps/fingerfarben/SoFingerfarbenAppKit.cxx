/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
 /** 
  * Implements SoFingerfarbenAppKit, a Studierstube ContextKit that represents an 
  * application for rapid prototyping of shapes.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoFingerfarbenAppKit.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSphere.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFPlane.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include <Inventor/SoPickedPoint.h>

#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/interaction/So3DSensor.h> 
#include <stbapi/window3d/SoWindowKit.h>

#include <stbapi/tracker/TrackerOT.h> 

#include <core/Context.h>
#include <common/StaticTransformation.h>

#include "SoFingerfarbenAppKit.h"

#include "ConvertToSFPlane.h"
#include "SoCalibrationKit.h"
#include "SoDisplayCalibrationKit.h"
#include "SoDropTarget.h"
#include "SoDropItem.h"
#include "SoTemplateKit.h"
#include "SoSaveKit.h"
#include "SoPrintKit.h"

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoFingerfarbenAppKit);

//--initialization of context in Studierstube
CONTEXT_APPLICATION(SoFingerfarbenAppKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void SoFingerfarbenAppKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoFingerfarbenAppKit, SoContextKit, "SoContextKit");

	ConvertToSFPlane::initClass();
    SoCalibrationKit::initClass();
    SoDisplayCalibrationKit::initClass();

    SoDropTarget::initClass();
    SoDropItem::initClass();

    SoTemplateKit::initClass();
    SoSaveKit::initClass();
    SoPrintKit::initClass();

}

//----------------------------------------------------------------------------
// virtual constructor
//----------------------------------------------------------------------------

SoContextKit* SoFingerfarbenAppKit::factory()
{
    return new SoFingerfarbenAppKit;
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoFingerfarbenAppKit::SoFingerfarbenAppKit()
{
    SO_KIT_CONSTRUCTOR(SoFingerfarbenAppKit);
    SO_KIT_INIT_INSTANCE();

	//inCalibration = false;
	inBuild = false;
	newSurface = false;

	currentSurface = NULL;
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoFingerfarbenAppKit::~SoFingerfarbenAppKit()
{
//    destructor();
}

//----------------------------------------------------------------------------
// we just need this to get the user id and the pen
//----------------------------------------------------------------------------

SbBool SoFingerfarbenAppKit::checkPipGeometry()
{
	return TRUE;
}

void SoFingerfarbenAppKit::checkPipMasterMode(SoNode* pipSheet, SbBool masterMode) 
{

}

void SoFingerfarbenAppKit::checkPipConnections( int uid, SoNode * pipSheetContents )
{

}

SoNode* SoFingerfarbenAppKit::myFindNode(SoNode* root, SbString name)
{
    SoSearchAction sa;
	sa.setSearchingAll(TRUE);
    sa.setName(name);
    sa.apply(root);
    SoFullPath* path = (SoFullPath*)sa.getPath();
    if (path == NULL) return NULL;
    return path->getTail();
}


//----------------------------------------------------------------------------
// createWindowGeometry
// create window and attach scene graph to it.
//----------------------------------------------------------------------------

SbBool SoFingerfarbenAppKit::checkWindowGeometry()
{
	myWindow = getWindow(0);
	
    if (myWindow == NULL)
	{
        myWindow = new SoWindowKit();
        myWindow->size = SbVec3f(0.5,0.5,0.5);

        SoSeparator* clientVolume = myWindow->getClientVolume();

		modelGeometry = new SoGroup;
		clientVolume->addChild(clientVolume);

		((SoGroup*)windowGroup.getValue())->addChild(myWindow);

	}
	else {
		modelGeometry = (SoGroup*)myFindNode(myWindow, "modelGeometry");
        
        printf("%s\n",modelGeometry->getName().getString());

		if (modelGeometry == NULL){
			modelGeometry = new SoGroup;
			myWindow->getClientVolume()->addChild(modelGeometry);
		}
        else {
            printf("model  found!\n");
        }
	}

    leftCursor = new SoSeparator;
    leftCursor->ref();
    leftCursor->addChild(new SoTranslation);
    SoMaterial* cursorMat = new SoMaterial;
    cursorMat->diffuseColor.setValue(1.0, 1.0, 1.0);
    leftCursor->addChild(cursorMat);

    SoCube* cursorGeom = new SoCube;
    cursorGeom->width.setValue(0.005f);
    cursorGeom->height.setValue(0.005f);
    cursorGeom->depth.setValue(0.005f);
    leftCursor->addChild(cursorGeom);

    modelGeometry->addChild(leftCursor);

    rightCursor = (SoSeparator*)leftCursor->copy();
    rightCursor->ref();

    modelGeometry->addChild(rightCursor);

    SoNodeSensor* geometrySensor = new SoNodeSensor();
    geometrySensor->setDeleteCallback(geometrySensorCB, this);
    geometrySensor->attach(modelGeometry);

    return TRUE;
}

//----------------------------------------------------------------------------
// process 3d window events
//----------------------------------------------------------------------------

SbBool SoFingerfarbenAppKit::windowEventCB(void* data, int message, SoWindowKit* window, int uid, So3DEvent* event, SoPath* rootPath)
{
    
	if (message != WM_EVENT) return FALSE;

    //only master processes events
    if (!isMaster) return false;
    if (uid < 0) return false;

    if (modelGeometry == NULL) {
        printf("calling checkWindowGeometry() from eventCB...\n");
        checkWindowGeometry();
		inBuild = false;
    }

    SoUserManagerKit* usrMgr = SoUserManagerKit::getInstance();

	pPenTrans = usrMgr->getUserKit(uid)->getPen()->getMover();
	pSecondTrans = ((SoTrackedArtifactKit*)(findNode(usrMgr->getUserKit(uid)->getAuxGeometry(),"secondHand")))->getMover();

    SoMaterial* userMaterial = NULL;
    userMaterial = (SoMaterial*) myFindNode(usrMgr->getUserKit(uid)->getPen(), "pen_material");

    if (userMaterial == NULL) {
        //printf("FingerfarbenAppKit: userMaterial == NULL! \n");
        userMaterial = new SoMaterial;
    }

    //printf("Event Received. Station:%d Button0:%d Button1:%d Button2:%d\n", event->getStation(),event->getButton(So3DEvent::BUTTON0),event->getButton(So3DEvent::BUTTON1),event->getButton(So3DEvent::BUTTON2));

    //find model

    SoSearchAction sa;
    sa.setNode(modelGeometry);
    sa.setSearchingAll(TRUE);
    rootPath->ref();
    sa.apply(((SoFullPath*)rootPath)->getTail());

    SoFullPath* pathToModel = (SoFullPath*) rootPath->copy();
    pathToModel->ref();
    pathToModel->append(sa.getPath());

    SbViewportRegion vp;
    SoGetMatrixAction ma(vp);
    ma.apply(pathToModel);
    SbMatrix modelMatrix = ma.getMatrix();

    SbVec3f modelTrans,s; SbRotation modelRot, so;
    modelMatrix.getTransform( modelTrans, modelRot, s, so );

	//printf("modelPos: [%f, %f, %f]\n",modelTrans[0],modelTrans[1],modelTrans[2]);

	SbVec3f penPos = pPenTrans->translation.getValue();
	SbVec3f secondPos = pSecondTrans->translation.getValue();

	//printf("penPos: [%f, %f, %f] secondPos: [%f, %f, %f]\n",penPos[0],penPos[1],penPos[2],secondPos[0],secondPos[1],secondPos[2]);

	SbVec3f start = penPos - modelTrans;
	SbVec3f end = secondPos - modelTrans;

	modelRot.invert();
	modelRot.multVec(start, start);
	modelRot.multVec(end, end);

    //snap(start);
    //snap(end);

    raySnap(start);
    raySnap(end);

    ((SoTranslation*)(leftCursor->getChild(0)))->translation = start;
    ((SoTranslation*)(rightCursor->getChild(0)))->translation = end;

    if (event->getButton(So3DEvent::BUTTON0)) {

		if (inBuild){
			if (newSurface) {
				modelGeometry->removeChild(tempLine);
			}
			extendSurface(currentSurface, start, end);
			newSurface = false;
			printf("extended surface...\n");
		}
		else {
            //give the new surface the color selected by the user
            modelGeometry->addChild(userMaterial->copy());

			tempLine = makeLine(start, end);
			modelGeometry->addChild(tempLine);

			currentSurface = makeSurface(start, end);
			modelGeometry->addChild(currentSurface);

			inBuild = true;
			newSurface = true;
		}
	}
	else if (event->getButton(So3DEvent::BUTTON1)) {
		//button2 click
		printf("starting new surface...\n");

		//no surface built -> remove surface, only the line stays
		if (inBuild && newSurface) {
			modelGeometry->removeChild(currentSurface);
		}

		inBuild = false;
	}
	else {
		return FALSE;
	}

    return TRUE;
}

void SoFingerfarbenAppKit::geometrySensorCB(void* data, SoSensor* sensor){
    printf("SoFingerfarbenAppKit: geometry sensor CB!\n");
    ((SoNodeSensor*)sensor)->detach();
    ((SoFingerfarbenAppKit*)data)->modelGeometry = NULL;
    //((SoFingerfarbenAppKit*)data)->checkWindowGeometry();
}

void SoFingerfarbenAppKit::raySnap(SbVec3f &point){

    SoSearchAction sAction;
    sAction.setType(SoCoordinate3::getClassTypeId());
    sAction.setSearchingAll(TRUE);
    sAction.setInterest(SoSearchAction::ALL);
    sAction.apply(modelGeometry);
    SoPathList paths = sAction.getPaths();

    SbViewportRegion vp;
    SoRayPickAction rpa(vp);
    rpa.setRay(SbVec3f(0,0,0), point);
    rpa.setRadius(15);
    rpa.apply(paths, TRUE);

    SoPickedPoint* pp = rpa.getPickedPoint();
    if (pp != NULL) {
        point = rpa.getPickedPoint()->getPoint();
    }
}




void SoFingerfarbenAppKit::snap(SbVec3f &point){

    SoSearchAction sAction;
    sAction.setType(SoIndexedFaceSet::getClassTypeId());
    sAction.setSearchingAll(TRUE);
    sAction.setInterest(SoSearchAction::ALL);
    sAction.apply(modelGeometry);
    SoPathList paths = sAction.getPaths();

    float snapRadius = 0.04f;

    SbBox3f bounds(point[0]-snapRadius, point[1]-snapRadius, point[2]-snapRadius, point[0]+snapRadius, point[1]+snapRadius, point[2]+snapRadius); 

    float minSquare = snapRadius*snapRadius;
    SbVec3f snapPoint = point;

    for (int i=0; i<paths.getLength(); i++){
	    SoCoordinate3* coord = (SoCoordinate3*)(((SoFullPath*)paths[i])->getTail());
        for (int j= 0; j<coord->point.getNum(); j++){
            if (bounds.intersect(coord->point[j])){
                SbVec3f cpoint = coord->point[j];
                float square = (point[0]-cpoint[0])*(point[0]-cpoint[0])+(point[1]-cpoint[1])*(point[1]-cpoint[1])+(point[2]-cpoint[2])*(point[2]-cpoint[2]);
                if (square < minSquare){
                    minSquare = square;
                    snapPoint = cpoint;
                }
            }
        }
    }

    point = snapPoint;

}

void SoFingerfarbenAppKit::setMaterial(SoMaterial* material){
	modelGeometry->addChild(material);
}

SoSeparator* SoFingerfarbenAppKit::makeLine(SbVec3f &start, SbVec3f &end){
	SoSeparator* mySep = new SoSeparator;
	mySep->ref();

	SoDrawStyle *style = new SoDrawStyle;
	style->lineWidth.setValue(2);
	mySep->addChild(style);

	SoCoordinate3 *coord = new SoCoordinate3;
	coord->point.set1Value(0, start);
	coord->point.set1Value(1, end);
	mySep->addChild(coord);

	int indices[] = {0,1,-1};
	SoIndexedLineSet *myLineSet = new SoIndexedLineSet;
	myLineSet->coordIndex.setValues(0, 3, indices);

	mySep->addChild(myLineSet);

	mySep->unrefNoDelete();

	return mySep;
}


SoSeparator* SoFingerfarbenAppKit::makeSurface(SbVec3f &start, SbVec3f &end){
	SoSeparator* mySep = new SoSeparator;
	mySep->ref();

	SoCoordinate3 *coord = new SoCoordinate3;
	coord->point.set1Value(0, start);
	coord->point.set1Value(1, end);
	mySep->addChild(coord);

	SoIndexedFaceSet *myFaceSet = new SoIndexedFaceSet;

	mySep->addChild(myFaceSet);

	mySep->unrefNoDelete();

	return mySep;
}


void SoFingerfarbenAppKit::extendSurface(SoSeparator* surface, SbVec3f &start, SbVec3f &end){

	// search for SoCoordinate3 nodes
    SoSearchAction sAction;
    sAction.setType(SoCoordinate3::getClassTypeId());
    //sAction.setSearchingAll(TRUE);
    sAction.setInterest(SoSearchAction::FIRST);
    sAction.apply(surface);
    SoPath* path = sAction.getPath();

	SoCoordinate3* coord = (SoCoordinate3*)(path->getTail());

	coord->point.set1Value(coord->point.getNum(), start);
	coord->point.set1Value(coord->point.getNum(), end);

	sAction.reset();
    sAction.setType(SoIndexedFaceSet::getClassTypeId());
    sAction.apply(surface);
    path = sAction.getPath();

	SoIndexedFaceSet* faceSet = (SoIndexedFaceSet*)(path->getTail());

	faceSet->coordIndex.set1Value(faceSet->coordIndex.getNum(), coord->point.getNum()-2);
	faceSet->coordIndex.set1Value(faceSet->coordIndex.getNum(), coord->point.getNum()-3);
	faceSet->coordIndex.set1Value(faceSet->coordIndex.getNum(), coord->point.getNum()-4);
	faceSet->coordIndex.set1Value(faceSet->coordIndex.getNum(), SO_END_FACE_INDEX );

	faceSet->coordIndex.set1Value(faceSet->coordIndex.getNum(), coord->point.getNum()-1);
	faceSet->coordIndex.set1Value(faceSet->coordIndex.getNum(), coord->point.getNum()-2);
	faceSet->coordIndex.set1Value(faceSet->coordIndex.getNum(), coord->point.getNum()-3);
	faceSet->coordIndex.set1Value(faceSet->coordIndex.getNum(), SO_END_FACE_INDEX );

}

