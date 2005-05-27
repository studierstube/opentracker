#include <iostream>

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoSearchAction.h>

#include <stbapi/vc/SoVCCalibrationWidgetKit.h>
#include <stbapi/vc/SoVCKit.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>

#include <Inventor/SbBox.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SoPath.h>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/resource/StbSkinLoader.h>
#include <stbapi/util/ivio.h>

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoVCCalibrationWidgetKit);

void 
SoVCCalibrationWidgetKit::initClass()
{
  SO_BEHAVIOR_INIT_CLASS(SoVCCalibrationWidgetKit, SoVCPushWidgetKit, "SoVCPushWidgetKit");
}

SoVCCalibrationWidgetKit::SoVCCalibrationWidgetKit()
{
  SO_BEHAVIOR_CONSTRUCTOR(SoVCCalibrationWidgetKit);
  
  SO_BEHAVIOR_INIT_INSTANCE();

  animating = false;

  on = false;
  highlighted = false;

  getGeometrySwitch()->whichChild = OFF;
}

SoVCCalibrationWidgetKit::~SoVCCalibrationWidgetKit()
{
}

void 
SoVCCalibrationWidgetKit::handle3DEvent(SoHandle3DEventAction *h3a)
{
   if(animating)
    {
      return;
    }  

  if(isInside(h3a))
    {
      highlighted = true;
    }
  else
    {
      highlighted = false;
    }   
  
  // determine, if the button has been activated

  void (*function)(void *userData, SoVCWidgetKit *widget);

  if(on.getValue() == FALSE)
    {
      if(detectAction(actionStartGestures))
      {
	on = true;

	for (int i=0; i<pressCBList->getLength(); i+=2)
	{	
	    if((*(pressCBList))[i])
	    {
		function = (void (*)(void *, SoVCWidgetKit *))(*(pressCBList))[i];
		(*function)((*(pressCBList))[i+1],this);
	    }	    
	}
	inspector->setGrabber(h3a);
      }
    }
  else if(on.getValue() == TRUE)
    {
      if(detectAction(actionEndGestures))
	{	  
	  on = false;
	 
	  for (int i=0; i<releaseCBList->getLength(); i+=2)
	  {	
	    if((*(releaseCBList))[i])
	    {
	      function = (void (*)(void *, SoVCWidgetKit *))(*(releaseCBList))[i];
	      (*function)((*(releaseCBList))[i+1],this);
	    }	    
	  }

	  performCalibration(h3a);

	  inspector->releaseGrabber(h3a);
	}
    }
}

void 
SoVCCalibrationWidgetKit::performCalibration(SoHandle3DEventAction *h3a)
{
  const SoPath *rootPath = h3a->getPathFromRoot();
  
  SoSearchAction sa;
  sa.setSearchingAll(TRUE);
  sa.setType(SoVCKit::getClassTypeId());
  rootPath->ref();
  sa.apply((SoFullPath*)rootPath);
  rootPath->unrefNoDelete();
  SoFullPath* path = (SoFullPath*)sa.getPath();
  assert(path != NULL);

  SoVCKit *vcKit = (SoVCKit*)(path->getTail());
  SbString name = vcKit->skinDirectory.getValue();
  name += vcKit->skinFileName.getValue();    
  StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());
  UAGETNODE(localRefPoint_, SoTranslation, "VC_LOCAL_REF_POINT", skin);
  UAGETNODE(globalRefPoint_, SoTranslation, "VC_GLOBAL_REF_POINT", skin);
  SbVec3f localRefPoint = localRefPoint_->translation.getValue();
  SbVec3f globalRefPoint = globalRefPoint_->translation.getValue();
  SbVec3f locToGlobOffset = globalRefPoint - localRefPoint;
  
  SoTransform *vcOffsetTransform = vcKit->getOffset();
  SoTransform *vcMoverTransform = vcKit->getMover();

  SbVec3f localHeadPos = vcMoverTransform->translation.getValue();
  SbVec3f globalHeadPos = localHeadPos + locToGlobOffset;

  SbVec3f localPenPos = inspector->getPenTranslation();
  SbVec3f localAlignedPenPos(localHeadPos[0], localHeadPos[1], localPenPos[2]);
  SbVec3f globalAlignedPenPos = localAlignedPenPos + locToGlobOffset;

  float dEVC = sqrt(pow((globalHeadPos[0]-globalAlignedPenPos[0]),2)+
		    pow((globalHeadPos[1]-globalAlignedPenPos[1]),2)+
		    pow((globalHeadPos[2]-globalAlignedPenPos[2]),2));

  float dEP = sqrt(pow(globalHeadPos[0],2) + pow(globalHeadPos[1],2) + pow(globalHeadPos[2],2));

  UAGETNODE(projectionPanelHeight_, SoInfo, "VC_PROJECTION_PANEL_HEIGHT", skin);
  float hP;
  sscanf(projectionPanelHeight_->string.getValue().getString(),"%f", &hP);

  UAGETNODE(projectionPanelAspect_, SoInfo, "VC_PROJECTION_PANEL_ASPECT", skin);
  float aspectP;
  sscanf(projectionPanelAspect_->string.getValue().getString(),"%f", &aspectP);

  float hVC = (dEVC*hP) / dEP;
  float wVC = hVC * aspectP;

  float transZ = (-1)*localAlignedPenPos[2];
  float scaleFactor = (hVC < wVC) ? hVC : wVC;

  /*  
      cout << "+++++++++++++++ CALIBRATION ++++++++++++++++" << endl;
      cout << "dEVC = " << dEVC << endl;
      cout << "dEP = " << dEP << endl;
      cout << "hVC = " << hVC << endl;
      cout << "wVC = " << wVC << endl;
      cout << "scaleFactor = " << scaleFactor << endl;
      cout << "locToGlobOffset= " << locToGlobOffset[0]  << ", " << locToGlobOffset[1] << ", " << locToGlobOffset[2] << endl;
      cout << "localPenPos = " << localPenPos[0]  << ", " << localPenPos[1] << ", " << localPenPos[2] << endl;
      cout << "translation  = (0, 0, " << transZ << endl;
      cout << "localAlignedPenPos  = " << localAlignedPenPos[0]  << ", " << localAlignedPenPos[1] << ", " << localAlignedPenPos[2] << endl;
      cout << "globalAlignedPenPos = " <<  globalAlignedPenPos[0]  << ", " << globalAlignedPenPos[1] << ", " << globalAlignedPenPos[2] << endl;
      cout << "localHeadPos = " << localHeadPos[0]  << ", " << localHeadPos[1] << ", " << localHeadPos[2] << endl;
      cout << "globalHeadPos = " << globalHeadPos[0]  << ", " << globalHeadPos[1] << ", " << globalHeadPos[2] << endl;
      cout << "++++++++++++++++++++++++++++++++++++++++++++" << endl;
  */
 
  vcOffsetTransform->scaleFactor.setValue(SbVec3f(scaleFactor, scaleFactor, scaleFactor));
  vcOffsetTransform->translation.setValue(SbVec3f(0, 0.3*scaleFactor, transZ));
}
