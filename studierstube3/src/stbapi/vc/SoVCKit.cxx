#include <iostream>
#include <cassert>
#include <cstdlib>

#include <Inventor/SbBox.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>

#include <stbapi/vc/SoVCMainPanelKit.h>
#include <stbapi/vc/SoVCKit.h>
#include <stbapi/vc/SoVCBarKit.h>

#include <stbapi/widgets/SoDial.h>

#include <stbapi/util/ivio.h>

#define ADD SO_KIT_ADD_CATALOG_ENTRY

const int VC_INFO = true;

SO_KIT_SOURCE(SoVCKit);


void
SoVCKit::initClass()
{
    SO_KIT_INIT_CLASS(SoVCKit, SoTrackedArtifactKit, "SoTrackedArtifactKit");
}


SoVCKit::SoVCKit()
:
currentMainPanelKit(NULL)
{
    SO_KIT_CONSTRUCTOR(SoVCKit);

    ADD(transform,  SoTransform, TRUE, onOffSwitch, \0, FALSE);
    ADD(taskBar,    SoVCBarKit, TRUE, onOffSwitch,  \0, FALSE);
    ADD(systemBar,  SoVCBarKit, TRUE, onOffSwitch,  \0, FALSE);
    ADD(windowBar,  SoVCBarKit, TRUE, onOffSwitch,  \0, FALSE);
    
    SO_KIT_ADD_CATALOG_LIST_ENTRY(mainPanels, SoSwitch ,FALSE, onOffSwitch, \0, SoVCMainPanelKit, TRUE);

    // add field
    SO_KIT_ADD_FIELD(vcSize, (1.0, 1.0, 1.0));
    SO_KIT_ADD_FIELD(vcAreaUpperLeft, (0.0,0.0));
    SO_KIT_ADD_FIELD(vcAreaLowerRight, (1.0,1.0));
    SO_KIT_ADD_FIELD(skinFileName, ("skinVC.iv"));
    SO_KIT_ADD_FIELD(skinDirectory, ("./graphix/"));

    SO_KIT_ADD_FIELD(distFromEye, (6.0));

    SO_KIT_ADD_FIELD(vcParts, (VC_FULL)); 

    SO_KIT_ADD_FIELD(pinned, (false));

    SO_NODE_DEFINE_ENUM_VALUE(VCPartMask, VC_SYSTEMBAR);
    SO_NODE_DEFINE_ENUM_VALUE(VCPartMask, VC_WINDOWBAR);
    SO_NODE_DEFINE_ENUM_VALUE(VCPartMask, VC_MAINPANEL);
    SO_NODE_DEFINE_ENUM_VALUE(VCPartMask, VC_TASKBAR);
    SO_NODE_DEFINE_ENUM_VALUE(VCPartMask, VC_FULL);

    SO_NODE_SET_SF_ENUM_TYPE(vcParts, VCPartMask);

    SO_KIT_INIT_INSTANCE();

    actionMinMax = false;

    //    pinned = false;

    setSearchingChildren(TRUE);

    setEventMask(So3DSeparator::BM_ALL_EVENTS);
}

SoVCKit::~SoVCKit()
{
    Stb3DEventGenerator::releaseAllGrabbers(this);
}

void
SoVCKit::createResources(SbAppID appID)
{
  if (getMainPanelKit(appID) == NULL)
    {
      SO_GET_ANY_PART(this, "transform", SoTransform)->scaleFactor.setValue(vcSize.getValue()[0],vcSize.getValue()[1], vcSize.getValue()[2]);
      SoVCMainPanelKit *mpk = new SoVCMainPanelKit(); 
      mpk->appID = appID; 
      mpk->parent = this; 
      getMainPanels()->addChild(mpk);
      if (VC_INFO)
	printf("INFO(SoVCKit): created resource (appID: %d)\n",
	       appID);
    }
  else
    {
      printf("ERROR(SoVCKit): can't create resource (appID: %d)\n",
	     appID);
    }
}

void
SoVCKit::freeResources(SbAppID appID)
{
  for (int i = 0; i<getMainPanels()->getNumChildren(); i++)
    if (((SoVCMainPanelKit *)getMainPanels()->getChild(i))->appID == appID)
      {
	Stb3DEventGenerator::releaseAllGrabbers(getMainPanels()->getChild(i));
	if (VC_INFO)
	  printf("INFO(SoVCKit): freed resource (appID: %d)\n",appID);
	if (getMainPanels()->getChild(i) == currentMainPanelKit)
	  currentMainPanelKit = NULL;
	getMainPanels()->removeChild(i);
	i--;
      }
}

void
SoVCKit::useResources(SbAppID appID)
{
  SoVCMainPanelKit *mpk = getMainPanelKit(appID);
  
  if (mpk)
    {
      SbString setString = "whichChild ";
      setString += SbString(getMainPanels()->findChild(mpk));
      getMainPanels()->containerSet(setString.getString());
      
      currentMainPanelKit = mpk;
      updateVC();
      if (VC_INFO)
	printf("INFO(SoVCKit): using resource (appID: %d)\n",
	       appID);
    }
  else
    {
      printf("ERROR(SoVCKit): can't use resource (appID: %d)\n",
	     appID);
    }
}

void
SoVCKit::setMainPanelGeometry(SbAppID appID, SoNode* mpGeom)
{
    SoVCMainPanelKit *mpk = getMainPanelKit(appID);
    if (mpk) 
      {
	if (mpGeom) 
	  {
            SoVCMainPanelKit *mpkNew;
            if (VC_INFO)
	      printf("INFO(SoVCKit): setting MainPanel geo (appID: %d)\n",
                     appID);
            if (mpGeom->isOfType(SoVCMainPanelKit::getClassTypeId()))
	      {
                mpkNew = (SoVCMainPanelKit *)mpGeom;
	      }
            else
	      {
		return;
	      }

            mpkNew->appID = appID;
            
            Stb3DEventGenerator::releaseAllGrabbers(mpk);
            getMainPanels()->replaceChild(mpk,mpkNew);
            
            mpkNew->activateMainPanel(this);

            if (mpk == currentMainPanelKit) 
	      {
                currentMainPanelKit = mpkNew;
                updateVC();
	      }
	  }
      }
    else
      {
	printf("ERROR(SoVCKit): can't set geometry (appID: %d)\n",
	       appID);
      }
}

SoVCMainPanelKit *
SoVCKit::getMainPanelKit(SbAppID appID)
{
  for (int i = 0; i<getMainPanels()->getNumChildren(); i++)
    if (((SoVCMainPanelKit *)getMainPanels()->getChild(i))->appID == appID) 
      {
	return (SoVCMainPanelKit *)getMainPanels()->getChild(i);
      }
  return NULL;
}

SoNodeKitListPart *
SoVCKit::getMainPanels()
{
  return SO_GET_ANY_PART(this, "mainPanels",SoNodeKitListPart);
}

void
SoVCKit::setVCPartVisibility(unsigned int vcpm)
{
  vcParts.setValue(vcpm);
  updateVC();
}

void
SoVCKit::setMPPartVisibility(unsigned int mppm)
{
  if (currentMainPanelKit)
    currentMainPanelKit->mpParts.setValue(mppm);
  currentMainPanelKit->updateMP();
}

void
SoVCKit::showVCBars(bool system, bool window, bool task)
{
  if(system)
    {
      if (SO_CHECK_ANY_PART(this,"systemBar",SoVCBarKit))
	SO_CHECK_ANY_PART(this,"systemBar",SoVCBarKit)->showElements(TRUE);
    }
  else 
    {
      if (SO_CHECK_ANY_PART(this,"systemBar",SoVCBarKit))
	SO_CHECK_ANY_PART(this,"systemBar",SoVCBarKit)->showElements(FALSE);
    }
  
  if(window)
    {
      if (SO_CHECK_ANY_PART(this,"windowBar",SoVCBarKit))
	SO_CHECK_ANY_PART(this,"windowBar",SoVCBarKit)->showElements(TRUE);
    }
  else 
    {
      if (SO_CHECK_ANY_PART(this,"windowBar",SoVCBarKit))
	SO_CHECK_ANY_PART(this,"windowBar",SoVCBarKit)->showElements(FALSE);
    }
  
  if(task)
    {
      if (SO_CHECK_ANY_PART(this,"taskBar",SoVCBarKit))
	SO_CHECK_ANY_PART(this,"taskBar",SoVCBarKit)->showElements(TRUE);
    }
  else 
    {
      if (SO_CHECK_ANY_PART(this,"taskBar",SoVCBarKit))
	SO_CHECK_ANY_PART(this,"taskBar",SoVCBarKit)->showElements(FALSE);
    }
}

void
SoVCKit::setSystemBar(SoVCBarKit *systemBar)
{
  setAnyPart("systemBar",systemBar);
}                                                                               

void
SoVCKit::setTaskBar(SoVCBarKit *taskBar)
{
  setAnyPart("taskBar",taskBar);
}

void
SoVCKit::setWindowBar(SoVCBarKit *windowBar)
{
  setAnyPart("windowBar",windowBar);
}

void
SoVCKit::updateVC()
{
  if(currentMainPanelKit)
    {
      showGeometry(true);
      showVCBars(vcParts.getValue() & SoVCKit::VC_SYSTEMBAR, 
		 vcParts.getValue() & SoVCKit::VC_WINDOWBAR,
		 vcParts.getValue() & SoVCKit::VC_TASKBAR);
      
      if(vcParts.getValue() & SoVCKit::VC_MAINPANEL)
	{
	  currentMainPanelKit->showMainPanel(TRUE);
	}
      else
	{
	  currentMainPanelKit->showMainPanel(FALSE);
	}

      if(actionMinMax)
	{ 
	  actionMinMax = false;
	  return;
	}
      
      SbString name = skinDirectory.getValue();
      name += skinFileName.getValue();
      StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());
      assert(skin);
      
      SbVec2f resultUL,resultLR;
      resultUL = vcAreaUpperLeft.getValue();
      resultLR = vcAreaLowerRight.getValue();
      
      UAGETNODE(mergeArea_2_,SoVCKit,"VC_SYSTEM_BAR_AREA",skin);
      mergeVCAreas(mergeArea_2_,resultUL,resultLR);

      UAGETNODE(mergeArea_3_,SoVCKit,"VC_TASK_BAR_AREA",skin);
      mergeVCAreas(mergeArea_3_,resultUL,resultLR);

      UAGETNODE(mergeArea_4_,SoVCKit,"VC_WINDOW_BAR_AREA",skin);
      mergeVCAreas(mergeArea_4_,resultUL,resultLR);
    
      currentMainPanelKit->setupTransform(resultUL,resultLR);
    }

  if(isPinned())
    {
      pinMe(false);
    }
  else
    {
      pinMe(true);
    }    
}

void
SoVCKit::mergeVCAreas(SoVCKit *mergeArea, SbVec2f &resultAreaUL, SbVec2f &resultAreaLR)
{
  resultAreaUL[0] = (resultAreaUL[0] >
		     mergeArea->vcAreaUpperLeft.getValue()[0]) ?
    resultAreaUL[0] :
    mergeArea->vcAreaUpperLeft.getValue()[0];
  
  resultAreaUL[1] = (resultAreaUL[1] >
		     mergeArea->vcAreaUpperLeft.getValue()[1]) ?
    resultAreaUL[1] :
    mergeArea->vcAreaUpperLeft.getValue()[1];
  
  resultAreaLR[0] = (resultAreaLR[0] <
		     mergeArea->vcAreaLowerRight.getValue()[0]) ?
    resultAreaLR[0] :
    mergeArea->vcAreaLowerRight.getValue()[0];
  
  resultAreaLR[1] = (resultAreaLR[1] <
		     mergeArea->vcAreaLowerRight.getValue()[1]) ?
    resultAreaLR[1] :
    mergeArea->vcAreaLowerRight.getValue()[1];
}

SoVCBarKit*
SoVCKit::getWindowBar()
{
  return (SoVCBarKit*)getAnyPart("windowBar", false);
}

SoVCBarKit*
SoVCKit::getSystemBar()
{
  return (SoVCBarKit*)getAnyPart("systemBar", false);
}

SoVCBarKit*
SoVCKit::getTaskBar()
{
  return (SoVCBarKit*)getAnyPart("taskBar", false);
}

void
SoVCKit::setActionMinMax(bool state)
{
  actionMinMax = state;
}

void
SoVCKit::setUpVCConnections(bool onOff, bool always)
{
  setUpConnections(onOff, always);
}

void
SoVCKit::setMover(SoTransform* newMover)
{
  setPart("mover", newMover);
}

void
SoVCKit::pinMe(bool state)
{
  if(state)
    {
      getMover()->translation.disconnect();
      getMover()->rotation.disconnect();
      pinned = true;
    }
  else
    {
      setUpConnections(true, true);
      pinned = false;
    }
}

bool
SoVCKit::isPinned()
{
  return pinned.getValue();
}
