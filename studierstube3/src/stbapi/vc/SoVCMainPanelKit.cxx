#include <iostream>
#include <cassert>
#include <cstdlib>

#include <Inventor/SbBox.h>
#include <Inventor/SbString.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoAsciiText.h>
#include <Inventor/nodes/SoTransparencyType.h>
#include <Inventor/nodes/SoAsciiText.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/interaction/So3DSeparator.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCone.h>

#include <stbapi/vc/SoVCMainPanelKit.h>
#include <stbapi/vc/SoVCKit.h>
#include <stbapi/vc/SoVCBarKit.h>
#include <stbapi/vc/SoVCSubPanelKit.h>
#include <stbapi/vc/SoVCStatusBarKit.h>

#define ADD SO_KIT_ADD_CATALOG_ENTRY

SO_KIT_SOURCE(SoVCMainPanelKit);

void
SoVCMainPanelKit::initClass()
{
    SO_KIT_INIT_CLASS(SoVCMainPanelKit, SoBaseKit, "BaseKit");
}

SoVCMainPanelKit::SoVCMainPanelKit()
:
    appID(-1), maxSubPanels(4), active(FALSE),
    parent(NULL)
{
    SO_KIT_CONSTRUCTOR(SoVCMainPanelKit);

    ADD(mainPanelRoot,   So3DSeparator,    FALSE, this,            \0, FALSE);
    ADD(partSwitch,      SoSwitch,         FALSE, mainPanelRoot,   \0, FALSE);
    ADD(barSwitch,       SoSwitch,         FALSE, partSwitch,      \0, FALSE);
    ADD(trayBar,         SoVCBarKit,       TRUE,  barSwitch,       \0, FALSE);
    ADD(quickBar,        SoVCBarKit,       TRUE,  barSwitch,       \0, FALSE);
    ADD(statusBar,       SoVCStatusBarKit, FALSE, barSwitch,       \0, FALSE);

    ADD(subPanelRoot,    So3DSeparator, FALSE, partSwitch,      \0, FALSE);
    ADD(transform,       SoTransform,   FALSE, subPanelRoot,    \0, FALSE);    

    ADD(silkPanelRoot,   SoSeparator,   FALSE, subPanelRoot,    \0, FALSE);
    ADD(silkPanelSwitch, SoSwitch,      FALSE, silkPanelRoot,   \0, FALSE);
    ADD(silkPanelTransf, SoTransform,   FALSE, silkPanelSwitch, \0, FALSE);
    ADD(silkPanelMat,    SoMaterial,    FALSE, silkPanelSwitch, \0, FALSE);
    ADD(silkPanelTransp, SoTransparencyType,  FALSE, silkPanelSwitch, \0, FALSE);

    ADD(silkPanelGeom,   SoCube,        FALSE, silkPanelSwitch, \0, FALSE);

    ADD(subPanelSwitch,  SoSwitch,      FALSE, subPanelRoot,    silkPanelRoot, FALSE);
    ADD(subPanels, SoSwitch, FALSE, subPanelSwitch, \0, FALSE);

    SO_KIT_ADD_FIELD(mpParts, (MP_FULL)); 
    SO_KIT_ADD_FIELD(mpAreaUpperLeft, (0.0,0.0));
    SO_KIT_ADD_FIELD(mpAreaLowerRight, (1.0,1.0));
    SO_KIT_ADD_FIELD(mpSize, (1.0, 1.0, 0.005));

    SO_KIT_ADD_FIELD(trayBarChanged, (false));

    SO_NODE_DEFINE_ENUM_VALUE(MPPartMask, MP_TRAYBAR);
    SO_NODE_DEFINE_ENUM_VALUE(MPPartMask, MP_QUICKBAR);
    SO_NODE_DEFINE_ENUM_VALUE(MPPartMask, MP_STATUSBAR);
    SO_NODE_DEFINE_ENUM_VALUE(MPPartMask, MP_SUBPANELS);
    SO_NODE_DEFINE_ENUM_VALUE(MPPartMask, MP_SILKPANEL);
    SO_NODE_DEFINE_ENUM_VALUE(MPPartMask, MP_FULL);

    SO_NODE_SET_SF_ENUM_TYPE(mpParts, MPPartMask);

    SO_KIT_INIT_INSTANCE();

    trayBarChanged = false;

    SO_GET_ANY_PART(this, "barSwitch", SoSwitch)->whichChild = -3; // show bars
    SO_GET_ANY_PART(this, "subPanelSwitch", SoSwitch)->whichChild = -3; // show subPanels
    SO_GET_ANY_PART(this, "partSwitch", SoSwitch)->whichChild = -3; // show parts

    SO_GET_ANY_PART(this, "silkPanelSwitch", SoSwitch)->whichChild = -3; // show parts
  
    SO_GET_ANY_PART(this, "silkPanelGeom", SoCube)->width = 1.0;
    SO_GET_ANY_PART(this, "silkPanelGeom", SoCube)->height = 1.0;
    SO_GET_ANY_PART(this, "silkPanelGeom", SoCube)->depth = 0.001;
    SO_GET_ANY_PART(this, "silkPanelTransp", SoTransparencyType)->value = SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND;
    SO_GET_ANY_PART(this, "silkPanelMat", SoMaterial)->transparency.setValue(0.6);
    SO_GET_ANY_PART(this, "silkPanelMat", SoMaterial)->ambientColor.setValue(0.70392468, 0.72155223, 0.72155349);
    SO_GET_ANY_PART(this, "silkPanelMat", SoMaterial)->diffuseColor.setValue(0.557827, 0.571796, 0.571797);
    SO_GET_ANY_PART(this, "silkPanelMat", SoMaterial)->specularColor.setValue(0.708205, 0.480594, 0.458724);
    SO_GET_ANY_PART(this, "silkPanelMat", SoMaterial)->emissiveColor.setValue(0.480594, 0.480594, 0.458724);
    SO_GET_ANY_PART(this, "silkPanelMat", SoMaterial)->shininess = 0.236364;
    SO_GET_ANY_PART(this, "silkPanelTransf", SoTransform)->translation.setValue(0, 0 , -0.3); 

    SO_GET_ANY_PART(this, "mainPanelRoot", So3DSeparator)->boundingBoxCheck = FALSE;
    SO_GET_ANY_PART(this, "subPanelRoot", So3DSeparator)->boundingBoxCheck = FALSE;

    trayBarChangedSensor.setFunction( SoVCMainPanelKit::sensorTrayBarCB);
    trayBarChangedSensor.setData( this );
    trayBarChangedSensor.attach( &trayBarChanged);
    trayBarChangedSensor.setPriority(1);

    hasMaxedSP = false;

    setSearchingChildren(TRUE);
}

SoVCMainPanelKit::~SoVCMainPanelKit()
{
    // nil
}

void
SoVCMainPanelKit::activateMainPanel(SoVCKit *parent)
{
  if(!active)
    getSubPanels()->whichChild = -3;

  // store pointer to parent of this kit
  SoVCMainPanelKit::parent = parent;
  
  SbString name = parent->skinDirectory.getValue();
  name += parent->skinFileName.getValue();    
  StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());

  // calculate current MP area
  SbVec2f resultUL,resultLR;
  resultUL = SbVec2f(0,0);
  resultLR = SbVec2f(1,1);

  // merge with bars
  UAGETNODE(mergeArea_1_,SoVCMainPanelKit,"MP_TRAY_BAR_AREA",skin);
  mergeMPAreas(mergeArea_1_,resultUL,resultLR);
  
  UAGETNODE(mergeArea_2_,SoVCMainPanelKit,"MP_QUICK_BAR_AREA",skin);
  mergeMPAreas(mergeArea_2_,resultUL,resultLR);

  UAGETNODE(mergeArea_3_,SoVCMainPanelKit,"MP_STATUS_BAR_AREA",skin);
  mergeMPAreas(mergeArea_3_,resultUL,resultLR);
  // --------------------------------------------
  
  float mpaWidth,mpaHeight;
  SbVec2f mpaCenter;
  
  mpaWidth = resultLR[0] - resultUL[0];
  mpaHeight = resultLR[1] - resultUL[1];
  mpaCenter = SbVec2f(resultUL[0]+mpaWidth/2.0,resultUL[1]+mpaHeight/2.0);

  SoVCMainPanelKit::mpaWidth = mpaWidth;
  SoVCMainPanelKit::mpaHeight = mpaHeight;
  SoVCMainPanelKit::mpaCenter = mpaCenter;

  int maxElements = 1;
  SbVec3f size;
  
  // create tray bar and setup transform    
  UAGETNODE(maxTrayElements_,SoInfo,"MP_TRAY_BAR_MAX_ELEMENTS",skin);
  maxElements = atoi(maxTrayElements_->string.getValue().getString());
  SoVCBarKit *trayBar = new SoVCBarKit(maxElements, skin, "MP_TRAY_BAR_");
  UAGETNODE(startTray_, SoTranslation, "MP_TRAY_BAR_START_TRANSLATION", skin);
  UAGETNODE(sizeTray_, SoScale, "MP_TRAY_BAR_SIZE", skin);
  size = sizeTray_->scaleFactor.getValue();
  trayBar->setupTransform(startTray_->translation.getValue(), size, parent->vcSize.getValue());
  trayBar->ref();
  setAnyPart("trayBar", trayBar);

  /*
  // create quickstart bar and setup transform
  UAGETNODE(maxQuickElements_,SoInfo,"MP_QUICK_BAR_MAX_ELEMENTS",skin);
  maxElements = atoi(maxQuickElements_->string.getValue().getString());
  SoVCBarKit *quickBar = new SoVCBarKit(maxElements, skin, "MP_QUICK_BAR_");
  UAGETNODE(startQuick_, SoTranslation, "MP_QUICK_BAR_START_TRANSLATION", skin);
  UAGETNODE(sizeQuick_, SoScale, "MP_QUICK_BAR_SIZE", skin);
  size = sizeQuick_->scaleFactor.getValue();
  quickBar->setupTransform(startQuick_->translation.getValue(), size, parent->vcSize.getValue());
  quickBar->ref();
  setAnyPart("quickBar", quickBar);
  */

  // create status bar and setup transform
  SoVCStatusBarKit *statusBar = new SoVCStatusBarKit(skin, "MP_STATUS_BAR_");
  UAGETNODE(startStatus_, SoTranslation, "MP_STATUS_BAR_START_TRANSLATION", skin);
  UAGETNODE(sizeStatus_, SoScale, "MP_STATUS_BAR_SIZE", skin);
  statusBar->setupTransform(startStatus_->translation.getValue(), sizeStatus_->scaleFactor.getValue(), parent->vcSize.getValue());
  // test here with add elements !
  SoSeparator *menueInfoSep = new SoSeparator();
  
  SoBaseColor *textColor = new SoBaseColor();
  textColor->rgb.setValue(1,0,0);
  SoText3 *text = new SoText3();
  text->justification.setValue(SoText3::CENTER);
  text->string.setValue("MAIN");
  SoFont *textFont = new SoFont();
  textFont->size.setValue(0.3);
  textFont->name.setValue("Arial:Bold");
  SoRotationXYZ *textRot = new SoRotationXYZ();
  textRot->angle.setValue(-M_PI/2);
  textRot->axis = SoRotationXYZ::X;
  
  menueInfoSep->addChild(textRot);
  menueInfoSep->addChild(textColor);
  menueInfoSep->addChild(textFont);
  menueInfoSep->addChild(text);
  
  int elemId = statusBar->addBarElement(menueInfoSep, 0.5);

  statusBar->ref();
  setAnyPart("statusBar", statusBar);
  
  SbString setString = "eventMask ";
  setString += SbString(So3DSeparator::BM_ALL_EVENTS);    
  for(int i=0; i<getSubPanels()->getNumChildren(); i++)
    {
      ((SoVCSubPanelKit*)getSubPanels()->getChild(i))->activateSubPanel(this);	
    }
  
  active = TRUE;
  updateMP();
}

void
SoVCMainPanelKit::addSubPanel(SoVCSubPanelKit* subPanel)
{
  if ((getSubPanels()->getNumChildren()) < maxSubPanels)
    {
      // add geometry
      getSubPanels()->addChild(subPanel);
    }
}

void
SoVCMainPanelKit::removeSubPanel(SoVCSubPanelKit* subPanel)
{
  if ((getSubPanels()->findChild(subPanel)) != -1)
    {
      // remove geometry
      if (active) {
	// release any grabbers in the geometry
	Stb3DEventGenerator::releaseAllGrabbers(subPanel);
      }      
      getSubPanels()->removeChild(subPanel);
    }
}

SoSwitch*
SoVCMainPanelKit::getSubPanels()
{
  return SO_GET_ANY_PART(this,"subPanels",SoSwitch);
}

void
SoVCMainPanelKit::showMainPanel(bool show)
{
  if(show)
    SO_GET_ANY_PART(this,"partSwitch",SoSwitch)->whichChild = -3;
  else
    SO_GET_ANY_PART(this,"partSwitch",SoSwitch)->whichChild = -1;
}

void
SoVCMainPanelKit::updateMP()
{
  showMPBars(mpParts.getValue() & SoVCMainPanelKit::MP_TRAYBAR, 
             mpParts.getValue() & SoVCMainPanelKit::MP_QUICKBAR,
             mpParts.getValue() & SoVCMainPanelKit::MP_STATUSBAR);

  if(mpParts.getValue() & SoVCMainPanelKit::MP_SUBPANELS)
    {
      SO_GET_ANY_PART(this, "subPanelSwitch", SoSwitch)->whichChild = -3; // show
    }
  else
    {
      SO_GET_ANY_PART(this, "subPanelSwitch", SoSwitch)->whichChild = -1;
    }

  if(mpParts.getValue() & SoVCMainPanelKit::MP_SILKPANEL)
    { 
      SO_GET_ANY_PART(this, "silkPanelSwitch", SoSwitch)->whichChild = -3; // show
    }
  else
    {
      SO_GET_ANY_PART(this, "silkPanelSwitch", SoSwitch)->whichChild = -1;
    }
}

void
SoVCMainPanelKit::showMPBars(bool tray, bool quick, bool status)
{
  if(tray)
    {
      if (SO_CHECK_ANY_PART(this,"trayBar",SoVCBarKit))
	SO_CHECK_ANY_PART(this,"trayBar",SoVCBarKit)->showElements(TRUE);
    }
  else 
      {
	if (SO_CHECK_ANY_PART(this,"trayBar",SoVCBarKit))
	  SO_CHECK_ANY_PART(this,"trayBar",SoVCBarKit)->showElements(FALSE);
      }
  
  if(quick)
    {
      if (SO_CHECK_ANY_PART(this,"quickBar",SoVCBarKit))
	SO_CHECK_ANY_PART(this,"quickBar",SoVCBarKit)->showElements(TRUE);
    }
  else 
    {
	if (SO_CHECK_ANY_PART(this,"quickBar",SoVCBarKit))
	  SO_CHECK_ANY_PART(this,"quickBar",SoVCBarKit)->showElements(FALSE);
    }

  if(status)
    {
      if (SO_CHECK_ANY_PART(this,"statusBar",SoVCStatusBarKit))
	SO_CHECK_ANY_PART(this,"statusBar",SoVCStatusBarKit)->showElements(TRUE);
    }
  else 
    {
	if (SO_CHECK_ANY_PART(this,"statusBar",SoVCStatusBarKit))
	  SO_CHECK_ANY_PART(this,"statusBar",SoVCStatusBarKit)->showElements(FALSE);
    }
}

SbString
SoVCMainPanelKit::getSkinDirectory()
{
  return parent->skinDirectory.getValue();
}

SbString
SoVCMainPanelKit::getSkinFileName()
{
  return parent->skinFileName.getValue();
}

void
SoVCMainPanelKit::setupTransform(SbVec2f vcAreaUL, SbVec2f vcAreaLR)
{
  SbString name = parent->skinDirectory.getValue();
  name += parent->skinFileName.getValue();    
  StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());

  // calculate current MP area
  SbVec2f resultUL,resultLR;
  resultUL = vcAreaUL;
  resultLR = vcAreaLR;

  // merge with bars  
  UAGETNODE(mergeArea_1_,SoVCMainPanelKit,"MP_TRAY_BAR_AREA",skin);
  mergeMPAreas(mergeArea_1_,resultUL,resultLR);
  
  UAGETNODE(mergeArea_2_,SoVCMainPanelKit,"MP_QUICK_BAR_AREA",skin);
  mergeMPAreas(mergeArea_2_,resultUL,resultLR);

  UAGETNODE(mergeArea_3_,SoVCMainPanelKit,"MP_STATUS_BAR_AREA",skin);
  mergeMPAreas(mergeArea_3_,resultUL,resultLR);
  // --------------------------------------------

  float vcX,vcY,vcZ;
  float mpaWidth,mpaHeight;
  SbVec2f mpaCenter;

  vcX = parent->vcSize.getValue()[0];
  vcY = parent->vcSize.getValue()[1];
  vcZ = parent->vcSize.getValue()[2];
  mpaWidth = resultLR[0] - resultUL[0];
  mpaHeight = resultLR[1] - resultUL[1];
  mpaCenter = SbVec2f(resultUL[0]+mpaWidth/2.0,resultUL[1]+mpaHeight/2.0);

  // scale mainPanel
  // autoscaling

  float sfx = vcX * mpaWidth;
  float sfy = vcY * mpaHeight;
  float sfs = (sfx < sfy) ? sfx : sfy;
  
  SO_GET_ANY_PART(this, "transform", SoTransform)->scaleFactor.setValue(sfs,sfs,sfs);

  SoVCMainPanelKit::mpaWidth = mpaWidth;
  SoVCMainPanelKit::mpaHeight = mpaHeight;
  SoVCMainPanelKit::mpaCenter = mpaCenter;
  
  if(!hasMaxedSP) // new, due to disappearing of SP when moving pen into apps window first time
    {
      for(int i=0; i<getSubPanels()->getNumChildren(); i++)
	{
	  ((SoVCSubPanelKit*)getSubPanels()->getChild(i))->setupTransform();
	}
      
      alignSubPanels(mpaWidth, mpaHeight, mpaCenter);
    }
}


void
SoVCMainPanelKit::alignSubPanels(float w, float h, SbVec2f c)
{
  static SbViewportRegion vp;
  static SoGetBoundingBoxAction bbAction(vp);

  float incrX, incrY;
  incrX = incrY = 0;

  setSilkPanelSize(0);

  for(int i=0; i<getSubPanels()->getNumChildren(); i++)
    {
      SoVCSubPanelKit* spk =((SoVCSubPanelKit*)getSubPanels()->getChild(i));
      bbAction.apply(spk);
      SbBox3f spBox = bbAction.getBoundingBox();
      
      if (!spBox.isEmpty()) 
	{	
	  float spSizeX, spSizeY, spSizeZ;
	  spBox.getSize(spSizeX, spSizeY, spSizeZ);

	  float minX, minY, minZ, maxX, maxY, maxZ;
	  spBox.getBounds(minX, minY, minZ, maxX, maxY, maxZ);

	  float minXs, minYs, minZs, maxXs, maxYs, maxZs;
	  bbAction.apply(SO_GET_ANY_PART(this, "silkPanelRoot", SoSeparator));
	  SbBox3f silkpBox = bbAction.getBoundingBox();
	  silkpBox.getBounds(minXs, minYs, minZs, maxXs, maxYs, maxZs);

	  float transX, transY, transZ;	  
	  transZ = 0;
	  transX = minXs - minX + incrX;
	  transY = maxYs - maxY + incrY;
	  
	  SbVec3f transOld = spk->getTransform()->translation.getValue();
	  spk->getTransform()->translation.setValue(transX + transOld[0], transY + transOld[1], transOld[2]);
	  incrY -= spSizeY;	    
      	}
    }
  
  setSilkPanelSize(1);  
}

void
SoVCMainPanelKit::mergeMPAreas(SoVCMainPanelKit *mergeArea, SbVec2f &resultAreaUL, SbVec2f &resultAreaLR)
{
  resultAreaUL[0] = (resultAreaUL[0] >
		     mergeArea->mpAreaUpperLeft.getValue()[0]) ?
    resultAreaUL[0] :
    mergeArea->mpAreaUpperLeft.getValue()[0];
  
  resultAreaUL[1] = (resultAreaUL[1] >
		   mergeArea->mpAreaUpperLeft.getValue()[1]) ?
    resultAreaUL[1] :
    mergeArea->mpAreaUpperLeft.getValue()[1];
 
  resultAreaLR[0] = (resultAreaLR[0] <
		     mergeArea->mpAreaLowerRight.getValue()[0]) ?
    resultAreaLR[0] :
    mergeArea->mpAreaLowerRight.getValue()[0];
  
  resultAreaLR[1] = (resultAreaLR[1] <
		     mergeArea->mpAreaLowerRight.getValue()[1]) ?
    resultAreaLR[1] :
    mergeArea->mpAreaLowerRight.getValue()[1];
}

void
SoVCMainPanelKit::setTrayBarSPKit(SoVCSubPanelKit* spKit)
{
  trayBarSPKit = spKit;
}

void 
SoVCMainPanelKit::updateSPState(SoVCSubPanelKit *spk)
{
  if(spk->getSizedState() == SoVCSubPanelKit::MINIMIZED)
    {
      ((SoVCBarKit*)getAnyPart("trayBar", false))->addBarElement(1,spk->getTitle().getString(),NULL, SoVCBarKit::removeBarElementCB, 
								 ((SoVCBarKit*)getAnyPart("trayBar", false)));
      ((SoVCBarKit*)getAnyPart("trayBar", false))->getElement(spk->getTitle().getString())->addReleaseCallback(trayBarCB, spk);
      
      if(spk->getCurrentWidgetKit() == NULL)
	((SoVCBarKit*)getAnyPart("trayBar", false))->getElement(spk->getTitle().getString())->setContent(spk->getTitle().getString(), SbVec3f(1.0,0.0,0.0));
      else
	((SoVCBarKit*)getAnyPart("trayBar", false))->getElement(spk->getTitle().getString())->setContent((SoSeparator*)spk->getCurrentWidgetKit()->getContent()->getChild(0));
      
      getSubPanels()->whichChild = -3;
      
      for(int i=0; i<getSubPanels()->getNumChildren(); i++)
	{       
	  if(((SoVCSubPanelKit*)getSubPanels()->getChild(i))->getSizedState() != SoVCSubPanelKit::MINIMIZED)
	    {	 
	      ((SoVCSubPanelKit*)getSubPanels()->getChild(i))->setSPPartVisibility(SoVCSubPanelKit::SP_FULL);
	    }
	}
      
      alignSubPanels(mpaWidth, mpaHeight, mpaCenter);            
      updateStatusBar(NULL, NULL);
    }
  else if(spk->getSizedState() == SoVCSubPanelKit::NORMAL)
    {
      for(int i=0; i<getSubPanels()->getNumChildren(); i++)
	{       
	  if(((SoVCSubPanelKit*)getSubPanels()->getChild(i))->getSizedState() != SoVCSubPanelKit::MINIMIZED)
	    {	 
	      ((SoVCSubPanelKit*)getSubPanels()->getChild(i))->setSPPartVisibility(SoVCSubPanelKit::SP_FULL);
	      ((SoVCSubPanelKit*)getSubPanels()->getChild(i))->setupTransform();    
	    }
	}
      hasMaxedSP = false;
      alignSubPanels(mpaWidth, mpaHeight, mpaCenter);     
      
      getSubPanels()->whichChild = -3;
      
      updateStatusBar(NULL, NULL);
    }
}

void
SoVCMainPanelKit::trayBarCB(void *userData, SoVCWidgetKit* widgetKit)
{
  SoVCSubPanelKit *spk = (SoVCSubPanelKit*)userData;
  SoVCMainPanelKit *mpk = spk->getParent();
  
  mpk->setTrayBarSPKit(spk);
  mpk->trayBarChanged = true;
}

void
SoVCMainPanelKit::sensorTrayBarCB(void *userData, SoSensor *sensor)
{
  assert( userData != NULL );
  SoVCMainPanelKit * self = (SoVCMainPanelKit *) userData;
  self->adaptTrayBarState();
}


void
SoVCMainPanelKit::adaptTrayBarState()
{
  trayBarSPKit->setSPPartVisibility(SoVCSubPanelKit::SP_FULL);
  updateStatusBar(trayBarSPKit->getCurrentWidgetKit(), trayBarSPKit);
  
  enlargeSubPanel(trayBarSPKit);
}


void
SoVCMainPanelKit::enlargeSubPanel(SoVCSubPanelKit* spk)
{
  int idx = getSubPanels()->findChild(spk);
  
  getSubPanels()->whichChild = idx;
  
  for(int i=0; i<getSubPanels()->getNumChildren(); i++)
    {
      if(((SoVCSubPanelKit*)getSubPanels()->getChild(i)) != spk)
	{
	  ((SoVCSubPanelKit*)getSubPanels()->getChild(i))->setSPPartVisibility(0);
	}
      else
	{
	  ((SoVCSubPanelKit*)getSubPanels()->getChild(i))->getTransform()->scaleFactor.setValue(1.0,1.0,1.0);
	  alignSubPanels(SoVCMainPanelKit::mpaWidth, SoVCMainPanelKit::mpaHeight, SoVCMainPanelKit::mpaCenter);
	}
    }
  spk->sizedState = SoVCSubPanelKit::MAXIMIZED;
  hasMaxedSP = true;
  updateStatusBar(spk->getCurrentWidgetKit(), spk);
}


bool
SoVCMainPanelKit::isActive()
{
  return active;
}


bool
SoVCMainPanelKit::existsMaxedSP()
{
  return hasMaxedSP;
}


void
SoVCMainPanelKit::setSilkPanelSize(bool full)
{
  if(!full)
    {
      SO_GET_ANY_PART(this, "silkPanelTransf", SoTransform)->translation.setValue(0,0,0);
      SO_GET_ANY_PART(this, "silkPanelTransf", SoTransform)->scaleFactor.setValue(1.0, 1.0, 1.0);
    }   
  else
    {
      float vcX = parent->vcSize.getValue()[0];
      float vcY = parent->vcSize.getValue()[1];
      float vcZ = parent->vcSize.getValue()[2];

      float sfx = vcX * mpaWidth;
      float sfy = vcY * mpaHeight;
      float sfs = (sfx < sfy) ? sfx : sfy;
      float sfc = (vcX < vcY) ? vcX : vcY;            

      SbString name = parent->skinDirectory.getValue();
      name += parent->skinFileName.getValue();    
      StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());
      UAGETNODE(sizeStatus_, SoScale, "MP_STATUS_BAR_SIZE", skin);
      SbVec3f statusBarSize = sizeStatus_->scaleFactor.getValue();
      UAGETNODE(silkPanelDistance_,SoInfo,"MP_SILK_PANEL_DISTANCE",skin);
      float silkPanelDistance;
      sscanf(silkPanelDistance_->string.getValue().getString(),"%f", &silkPanelDistance);
      UAGETNODE(statusBarDistance_,SoInfo,"MP_STATUS_BAR_DISTANCE",skin);
      float statusBarDistance;
      sscanf(statusBarDistance_->string.getValue().getString(),"%f", &statusBarDistance);

      silkPanelDistance /= sfs;

      float distanceScaleFact = 0.2333;
      float distanceScale = 1.0 + fabs(distanceScaleFact * silkPanelDistance);
      float statusBarDistanceScale = 1.0 + fabs(distanceScaleFact * statusBarDistance);

      distanceScale = 1.07;
      
      SO_GET_ANY_PART(this, "silkPanelTransf", SoTransform)->translation.setValue(0,(statusBarSize[1]/2.0)*(1.0/sfs)*distanceScale,silkPanelDistance);     
      
      SO_GET_ANY_PART(this, "silkPanelTransf", SoTransform)->scaleFactor.setValue((1.0/sfs)*distanceScale,
	 									  ((1.0 - statusBarSize[1])/sfs)*distanceScale,
										  (1.0/sfs)*distanceScale);      
      
    }         
}


void 
SoVCMainPanelKit::updateStatusBar(SoVCWidgetKit *widgetKit, SoVCSubPanelKit *spKit)
{
  SbString infoString;
  
  if(widgetKit != NULL && spKit != NULL)
    {
      SbString tmp;
      if(widgetKit->getWidgetParent() == NULL)
	{
	  infoString = spKit->getTitle();
	  infoString += "->";
	}      
      else 
	{ 
	  tmp = widgetKit->getWidgetParent()->getWidgetName().getValue();
	  tmp += "->";
	}
      
      tmp += widgetKit->getWidgetName().getValue();
      infoString += tmp;
    } 
  else if(widgetKit == NULL && spKit != NULL)
    {
      infoString = spKit->getTitle();
    }
  else
    infoString = "MAIN";
  
 SoSeparator *menueInfoSep = new SoSeparator();

 SoMaterial *textMat = new SoMaterial();
 textMat->diffuseColor = SbVec3f(1.0, 0.0, 0.0);

 SoText3 *text = new SoText3();
 text->justification.setValue(SoText3::CENTER);
 text->string.setValue(infoString.getString());

 SoFont *textFont = new SoFont();
 textFont->size.setValue(0.3);
 textFont->name.setValue("Arial:Bold");

 SoRotationXYZ *textRot = new SoRotationXYZ();
 textRot->angle.setValue(-M_PI/2);
 textRot->axis = SoRotationXYZ::X;

 menueInfoSep->addChild(textRot);
 menueInfoSep->addChild(textMat);
 menueInfoSep->addChild(textFont);
 menueInfoSep->addChild(text);

 SO_GET_ANY_PART(this, "statusBar", SoVCStatusBarKit)->updateElementsContent(0, menueInfoSep);
}

SoVCKit*
SoVCMainPanelKit::getVC()
{
  return parent;
}
