#include <cassert>
#include <cstdlib>
#include <iostream>
// #include <unistd.h>

#include <Inventor/SbBox.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoRotationXYZ.h>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/interaction/So3DSeparator.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/workspace/SoContextManagerKit.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>
#include <stbapi/interaction/SoButtonReleaseGesture.h>

#include <stbapi/vc/SoVCSubPanelKit.h>
#include <stbapi/vc/SoVCMainPanelKit.h>
#include <stbapi/vc/SoVCBarKit.h>
#include <stbapi/vc/SoVCKit.h>

#include <stbapi/vc/globaldef.h>

using namespace std;

#define FRAMES_PER_SEC 50
#define ANIMATION_STEPS_INTERVALL 20

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoVCSubPanelKit);

void
SoVCSubPanelKit::initClass()
{
    SO_BEHAVIOR_INIT_CLASS(SoVCSubPanelKit, SoBehaviorKit, "SoBehaviorKit");
}

SoVCSubPanelKit::SoVCSubPanelKit()
:
    active(FALSE),
    parent(NULL)
{
    SO_BEHAVIOR_CONSTRUCTOR(SoVCSubPanelKit);
    
    ADD(partSwitch,      SoSwitch,      FALSE, base, geometry, FALSE);
    ADD(transform,       SoTransform,   TRUE,  base, partSwitch, FALSE);

    ADD(barSwitch,       SoSwitch,      FALSE, partSwitch,   \0, FALSE);
    ADD(windowBar,       SoVCBarKit,    TRUE,  barSwitch,    \0, FALSE);
    ADD(widgetSwitch,    SoSwitch,      FALSE, partSwitch,   \0, FALSE);
    ADD(widgetRoot,      SoSeparator,   FALSE, widgetSwitch, \0, FALSE);
    ADD(widgetTransform, SoTransform,   TRUE,  widgetRoot, \0, FALSE);

    ADD(widgets,     SoSwitch,   FALSE,    widgetRoot,   \0,  FALSE);

    SO_BEHAVIOR_ADD_FIELD(widgetPressed, (false));
    SO_BEHAVIOR_ADD_FIELD(windowBarChanged, (false));
    
    SO_BEHAVIOR_ADD_FIELD(title, ("default"));

    SO_BEHAVIOR_ADD_FIELD(spParts, (SP_FULL)); 
    SO_NODE_DEFINE_ENUM_VALUE(SPPartMask, SP_WINDOWBAR);
    SO_NODE_DEFINE_ENUM_VALUE(SPPartMask, SP_BODY);
    SO_NODE_DEFINE_ENUM_VALUE(SPPartMask, SP_MENUWIDGETS);
    SO_NODE_DEFINE_ENUM_VALUE(SPPartMask, SP_FULL);
    SO_NODE_SET_SF_ENUM_TYPE(spParts, SPPartMask);

    SO_BEHAVIOR_ADD_FIELD(actionStartGestures, (new SoButtonPressGesture()));
    SO_BEHAVIOR_ADD_FIELD(actionEndGestures, (new SoButtonReleaseGesture()));
    SO_BEHAVIOR_ADD_FIELD(actionEnterGestures, (new SoEnterGesture()));
    SO_BEHAVIOR_ADD_FIELD(actionExitGestures, (new SoExitGesture()));
    SO_BEHAVIOR_ADD_FIELD(actionMoveGestures, (new SoMoveGesture()));

    SO_BEHAVIOR_ADD_FIELD(on, (false) );
    SO_BEHAVIOR_ADD_FIELD(highlighted, (false) );
    SO_BEHAVIOR_ADD_FIELD(maximized, (false) );

    SO_BEHAVIOR_INIT_INSTANCE();

    SO_GET_ANY_PART(this, "barSwitch", SoSwitch)->whichChild = -3; // show bar
    SO_GET_ANY_PART(this, "widgetSwitch", SoSwitch)->whichChild = -3; // show widgets
    SO_GET_ANY_PART(this, "partSwitch", SoSwitch)->whichChild = -3; // show parts
    SO_GET_ANY_PART(this, "widgets", SoSwitch)->whichChild = -3; // show parts

    widgetPressed = false;
    windowBarChanged = false;
    sizedState = SoVCSubPanelKit::NORMAL;

    setSearchingChildren(TRUE);

    widgetPressedSensor.setFunction( SoVCSubPanelKit::sensorWidgetPressedCB );
    widgetPressedSensor.setData( this );
    widgetPressedSensor.attach( &widgetPressed );
    widgetPressedSensor.setPriority( 1 );

    windowBarChangedSensor.setFunction( SoVCSubPanelKit::sensorWindowBarCB);
    windowBarChangedSensor.setData( this );
    windowBarChangedSensor.attach( &windowBarChanged);
    windowBarChangedSensor.setPriority(1);

    currentWidgetKit = NULL;
    handleHierarchyUp = false;

    on = false;
    highlighted = false;

    getGeometrySwitch()->whichChild = -1; // OFF

    inspector = new StbEventInspector(this);

    onSensor.setFunction( SoVCSubPanelKit::stateChangedCB );
    onSensor.setData( this );
    onSensor.attach( &on );
    onSensor.setPriority( 1 );
    hlSensor.setFunction( SoVCSubPanelKit::stateChangedCB );
    hlSensor.setData( this );
    hlSensor.attach( &highlighted );
    hlSensor.setPriority( 1 );

    animStepsCount = 0;
}

SoVCSubPanelKit::~SoVCSubPanelKit()
{
    // nil
}

void
SoVCSubPanelKit::activateSubPanel(SoVCMainPanelKit *parent)
{
  if(!active)
    SO_GET_ANY_PART(this, "widgets", SoSwitch)->whichChild = -3; // LATEST

  // store pointer to parent of this kit
  SoVCSubPanelKit::parent = parent;

  SbString texfilename = "";
  SbString dirName = parent->getSkinDirectory();
  SbString name = parent->getSkinDirectory();
  name += parent->getSkinFileName();    
  StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());

  int maxElements = 1;
  SbVec3f size;

  // *** setup WINDOW BAR *** //

  // create window bar and setup transform
  UAGETNODE(maxWindowElements_,SoInfo,"SP_WINDOW_BAR_MAX_ELEMENTS",skin);
  maxElements = atoi(maxWindowElements_->string.getValue().getString());
  SoVCBarKit* windowBar = new SoVCBarKit(maxElements, skin, "SP_WINDOW_BAR_");
  UAGETNODE(startWindow_, SoTranslation, "SP_WINDOW_BAR_START_TRANSLATION", skin);
  UAGETNODE(sizeWindow_, SoScale, "SP_WINDOW_BAR_SIZE", skin);
  size = sizeWindow_->scaleFactor.getValue();
  float panelWidth = 1.0;
  UAGETNODE(panelWidth_,SoInfo,"SP_WIDTH",skin);
  sscanf(panelWidth_->string.getValue().getString(),"%f", &panelWidth);
  float mpX,mpY,mpZ;
  parent->mpSize.getValue().getValue(mpX,mpY,mpZ);
  windowBar->setupTransform(startWindow_->translation.getValue(), size, SbVec3f(mpX,mpY,mpZ));

  // set window bar elements' content (MAX WIDGET)
  SoSeparator *maxCont = new SoSeparator();
  SoMaterial *maxContMaterial = new SoMaterial();
  SoRotationXYZ *maxContRot = new SoRotationXYZ();
  maxContRot->axis = SoRotationXYZ::X;
  maxContRot->angle = - M_PI / 2.0;
  SoCone *maxContShape = new SoCone();
  maxContShape->bottomRadius = 0.2;
  maxContShape->height = 0.4;
  maxContMaterial->diffuseColor = SbVec3f(0.0, 1.0, 0.0);
  maxContMaterial->transparency = 0.0;
  maxCont->addChild(maxContRot);
  maxCont->addChild(maxContMaterial);
  maxCont->addChild(maxContShape);
  maxCont->ref();

  // set window bar elements' content (MIN WIDGET)
  SoSeparator *minCont = new SoSeparator();
  SoMaterial *minContMaterial = new SoMaterial();
  SoRotationXYZ *minContRot = new SoRotationXYZ();
  minContRot->axis = SoRotationXYZ::X;
  minContRot->angle = M_PI / 2.0;
  SoCone *minContShape = new SoCone();
  minContShape->bottomRadius = 0.2;
  minContShape->height = 0.4;
  minContMaterial->diffuseColor = SbVec3f(1.0, 1.0, 0.0);
  minContMaterial->transparency = 0.0;
  minCont->addChild(minContRot);
  minCont->addChild(minContMaterial);
  minCont->addChild(minContShape);
  minCont->ref();

  // set window bar elements' content (CLOSE WIDGET) => no content
  // ...

  // set window bar elements' content (OVERVIEW WIDGET)
  SoSeparator *ovwCont = new SoSeparator();
  SoMaterial *ovwContMat = new SoMaterial();
  ovwContMat->diffuseColor.setValue(1.0, 1.0, 1.0);
  ovwCont->addChild(ovwContMat);
  SoRotationXYZ *ovwContRot = new SoRotationXYZ();
  ovwContRot->angle.setValue(-M_PI/2.0);
  ovwContRot->axis = SoRotationXYZ::X;
  ovwCont->addChild(ovwContRot);
  SoTexture2 * ovwContTex = new SoTexture2();
  texfilename = "";
  texfilename += dirName;
  texfilename += SbString("ovw.jpg");
  ovwContTex->filename = texfilename;
  ovwCont->addChild(ovwContTex);
  SoCube *ovwContGeom = new SoCube();
  ovwContGeom->width = 1.0;
  ovwContGeom->height = 1.0;
  ovwContGeom->depth = 0.01;
  ovwCont->addChild(ovwContGeom);
  ovwCont->ref();

  // set window bar elements' content (BACK WIDGET)
  SoSeparator *backCont = new SoSeparator();
  SoMaterial *backContMat = new SoMaterial();
  backContMat->diffuseColor.setValue(1.0, 1.0, 1.0);
  backCont->addChild(backContMat);
  SoRotationXYZ *backContRot = new SoRotationXYZ();
  backContRot->angle.setValue(-M_PI/2.0);
  backContRot->axis = SoRotationXYZ::X;
  backCont->addChild(backContRot);
  SoTexture2 * backContTex = new SoTexture2();
  texfilename = "";
  texfilename += dirName;
  texfilename += SbString("back.jpg");
  backContTex->filename = texfilename;
  backCont->addChild(backContTex);
  SoCube *backContGeom = new SoCube();
  backContGeom->width = 1.0;
  backContGeom->height = 1.0;
  backContGeom->depth = 0.01;
  backCont->addChild(backContGeom);
  backCont->ref();

  // add window bar elements
  //windowBar->addBarElement(SP_CLOSE_SP, "SP_CLOSE_SP", NULL, windowBarCB, this);
  windowBar->addBarElement(SP_OVERVIEW_SP, "SP_OVERVIEW_SP", ovwCont, windowBarCB, this);
  windowBar->addBarElement(SP_BACK_SP, "SP_BACK_SP",  backCont, windowBarCB, this, SbVec3f(1,0,0));
  windowBar->addBarElement(SP_MIN_SP,   "SP_MIN_SP",   minCont, windowBarCB, this, SbVec3f(1,0,0), false);
  windowBar->addBarElement(SP_MAX_SP,   "SP_MAX_SP",   maxCont, windowBarCB, this, SbVec3f(1,0,0), false);

  // set bar in vc
  windowBar->ref();
  setAnyPart("windowBar", windowBar);
    
  for(int i=0; i<getWidgets()->getNumChildren(); i++)
    {
      // set first callback ever :) to be called, to handle zoom properbly
      activateWidget((SoVCWidgetKit*)getWidgets()->getChild(i), NULL);
    }
  
  active = TRUE;

  alignWidgets();
  updateSP();
}

SoSwitch *
SoVCSubPanelKit::getWidgets()
{
  return SO_GET_ANY_PART(this,"widgets",SoSwitch);
}


void
SoVCSubPanelKit::adaptHierarchyLevel()
{
  SoVCWidgetKit *widgetKit = currentWidgetKit;
  int i;

  if(widgetKit == NULL)
    {
      return;
    }

  if(handleHierarchyUp)
    {
      // begin - save information for pre-animation settings
      SbViewportRegion vp;
      SoGetBoundingBoxAction bba(vp);      

      bba.apply(currentWidgetKit);
      animSaveCurrWidgetPreBBox = bba.getXfBoundingBox().project();
      // end - save information for post-animation settings

      SoVCWidgetKit* widgetsParent = widgetKit->getWidgetParent();	
      if(widgetsParent != NULL)
	{	 
	  widgetKit->getWidgetChildren()->whichChild = SO_SWITCH_NONE;
	  currentWidgetKit->setVisibility(true);

	  // begin - save information for post-animation settings  
	  bba.apply(currentWidgetKit);
	  animSaveCurrWidgetPostBBox = bba.getXfBoundingBox().project();
	  // end - save information for post-animation settings	  

	  widgetKit->getWidgetChildren()->whichChild = SO_SWITCH_ALL;
	  currentWidgetKit->setVisibility(false);

	  // begin animation sensor setup
	  animStepsCount = 0;
	  SoTimerSensor *animationHUpSensor = new SoTimerSensor(animationHUpSensorCB, this);
	  animationHUpSensor->setBaseTime(SbTime::getTimeOfDay());
	  animationHUpSensor->setInterval(SbTime(1.0 / FRAMES_PER_SEC)); //scheduling interval
	  animationHUpSensor->schedule();
	  // end animation sensor setup	  
	}
      else
	{
	  widgetKit->getWidgetChildren()->whichChild = SO_SWITCH_NONE;
	  currentWidgetKit->setVisibility(true);

	  // begin - save information for post-animation settings  
	  bba.apply(currentWidgetKit);
	  animSaveCurrWidgetPostBBox = bba.getXfBoundingBox().project();
	  // end - save information for post-animation settings	  

	  widgetKit->getWidgetChildren()->whichChild = SO_SWITCH_ALL;
	  currentWidgetKit->setVisibility(false);

	  // begin animation sensor setup
	  animStepsCount = 0;
	  SoTimerSensor *animationHUpSensor = new SoTimerSensor(animationHUpSensorCB, this);
	  animationHUpSensor->setBaseTime(SbTime::getTimeOfDay());
	  animationHUpSensor->setInterval(SbTime(1.0 / FRAMES_PER_SEC)); //scheduling interval
	  animationHUpSensor->schedule();
	  // end animation sensor setup
	}      
    }
  else
    {
      if(widgetKit->getWidgetChildren()->getNumChildren() > 0)
	{	 
	  SoVCWidgetKit* widgetsParent = widgetKit->getWidgetParent();	  

	  SbVec3f oldScale = getTransform()->scaleFactor.getValue();
	  getTransform()->scaleFactor.setValue(1.0,1.0,1.0);
	  
	  // begin - save information for post-animation settings
	  SbViewportRegion vp;
	  SoGetBoundingBoxAction bba(vp);
  
	  bba.apply(currentWidgetKit);
	  animSaveCurrWidgetPreBBox = bba.getXfBoundingBox().project();
	  // end - save information for post-animation settings
	  getTransform()->scaleFactor.setValue(oldScale[0], oldScale[1], oldScale[2]);
  
	  widgetKit->setVisibility(false);
	  widgetKit->getWidgetChildren()->whichChild.setValue(SO_SWITCH_ALL);	

	  if(widgetsParent != NULL)
	    {	 
	      int currWidgetIdx = 0;
	      for(i=0; i<widgetsParent->getWidgetChildren()->getNumChildren(); i++)
		{
		  if((SoVCWidgetKit*)widgetsParent->getWidgetChildren()->getChild(i) == widgetKit)
		    {
		      currWidgetIdx = i;
		      break;
		    }
		}
	      widgetsParent->getWidgetChildren()->whichChild.setValue(currWidgetIdx);
	      // } // fuckin' brace fucked up countless time, line above was never executed with correct idx !
	    }
	  else
	    {
	      int currWidgetIdx = 0;
	      for(i=0; i<getWidgets()->getNumChildren(); i++)
		{	       
		  if((SoVCWidgetKit*)getWidgets()->getChild(i) == widgetKit)
		    {
		      currWidgetIdx = i;
		      break;
		    }
		}
	      getWidgets()->whichChild.setValue( currWidgetIdx );
	    }
	  
	  // begin - save information for post-animation settings  
	  bba.apply(currentWidgetKit);
	  animSaveCurrWidgetPostBBox = bba.getXfBoundingBox().project();
	  // end - save information for post-animation settings	  

	  widgetKit->getWidgetChildren()->whichChild.setValue(SO_SWITCH_NONE);	

	  // begin animation sensor setup
	  animStepsCount = 0;
	  SoTimerSensor *animationHDownSensor = new SoTimerSensor(animationHDownSensorCB, this);
	  animationHDownSensor->setBaseTime(SbTime::getTimeOfDay());
	  animationHDownSensor->setInterval(SbTime(1.0 / FRAMES_PER_SEC)); //scheduling interval
	  animationHDownSensor->schedule();
	  // end animation sensor setup

	  parent->updateStatusBar(currentWidgetKit, this);
	}
      else
	{		   
	  if(currentWidgetKit->isHideWhenLeaf())
	    {
	      currentWidgetKit = oldWidgetKit;
	      parent->getVC()->setActionMinMax(true);
	      parent->getVC()->setVCPartVisibility(SoVCKit::VC_WINDOWBAR);	  	  
	    }
	  else
	      currentWidgetKit = oldWidgetKit;

	  parent->updateStatusBar(currentWidgetKit, this);
	}
    }  
}


void
SoVCSubPanelKit::sensorWidgetPressedCB(void *userData, SoSensor *sensor)
{
  assert( userData != NULL );
  SoVCSubPanelKit * self = (SoVCSubPanelKit *) userData;
  self->setHandleHierarchyUp(false);
  self->adaptHierarchyLevel();
}

void
SoVCSubPanelKit::widgetPressedCB(void* userData, SoVCWidgetKit* widgetKit)
{
  SoVCSubPanelKit *spk = (SoVCSubPanelKit*)userData;

  spk->setCurrentWidgetKitName(widgetKit->getWidgetName());
  spk->setCurrentWidgetKit(widgetKit);

  spk->widgetPressed = true;
}

void
SoVCSubPanelKit::activateWidget(SoVCWidgetKit* widgetKit, SoVCWidgetKit* parent)
{
  widgetKit->showChildren(false);
  widgetKit->showContent(true);
  widgetKit->setWidgetParent(parent);
  widgetKit->addPressCallback(widgetPressedCB, this);
  for(int i=0; i<widgetKit->getWidgetChildren()->getNumChildren(); i++)
    {    
      activateWidget(((SoVCWidgetKit*)widgetKit->getWidgetChildren()->getChild(i)), widgetKit);
    }
}


void
SoVCSubPanelKit::adaptSubPanelToState()
{
  if(windowWidgetPressed == SP_OVERVIEW_SP)
    {
      sizedState = SoVCSubPanelKit::NORMAL;
      getParent()->updateSPState(this);
    } 
  else if(windowWidgetPressed == SP_CLOSE_SP)
    {
      // todo: call to mainpanel and remove subpanel -> then realign subpanels !? confusing for user?
    }
  else if(windowWidgetPressed == SP_MIN_SP)
    {
      sizedState = SoVCSubPanelKit::MINIMIZED;
      setSPPartVisibility( 0 );
      getParent()->updateSPState(this);
    }
  else if(windowWidgetPressed == SP_MAX_SP)
    {
      alignWidgets();
      setupTransform();
      parent->enlargeSubPanel(this);
    }
  else if(windowWidgetPressed == SP_BACK_SP)
    {
      handleHierarchyUp = true;
      adaptHierarchyLevel();
    }

  updateSP();
}

void
SoVCSubPanelKit::sensorWindowBarCB(void *userData, SoSensor *sensor)
{
  assert( userData != NULL );
  SoVCSubPanelKit * self = (SoVCSubPanelKit *) userData;
  self->adaptSubPanelToState();
}

void
SoVCSubPanelKit::setWindowWidgetPressed(int state)
{
  windowWidgetPressed = state;
}

void
SoVCSubPanelKit::windowBarCB(void *userData, SoVCWidgetKit *widgetKit)
{
  SoVCSubPanelKit* spk = (SoVCSubPanelKit*)userData;

  if(widgetKit->getWidgetID() == SP_OVERVIEW_SP)
    {
      spk->setWindowWidgetPressed(SP_OVERVIEW_SP);
    }
  else if(widgetKit->getWidgetID() == SP_CLOSE_SP)
    {
      spk->setWindowWidgetPressed(SP_CLOSE_SP);
    }
  else if(widgetKit->getWidgetID() == SP_MIN_SP)
    {
      spk->setWindowWidgetPressed(SP_MIN_SP);
    }
  else if(widgetKit->getWidgetID() == SP_MAX_SP)
    {
      spk->setWindowWidgetPressed(SP_MAX_SP);
    }
  else if(widgetKit->getWidgetID() == SP_BACK_SP)
    {
      spk->setWindowWidgetPressed(SP_BACK_SP);
    }

  spk->windowBarChanged = true;
}


void
SoVCSubPanelKit::updateSP()
{
  showSPBars(spParts.getValue() & SoVCSubPanelKit::SP_WINDOWBAR);

  if(spParts.getValue() & SoVCSubPanelKit::SP_MENUWIDGETS)
    {
      SO_GET_ANY_PART(this, "widgetSwitch", SoSwitch)->whichChild = -3; // show
    }    
  else
    {
      SO_GET_ANY_PART(this, "widgetSwitch", SoSwitch)->whichChild = -1;
    }

  if(maximized.getValue())
    {
      windowWidgetPressed = SP_MAX_SP;
      windowBarChanged = true;
    }  
}

void
SoVCSubPanelKit::showSPBars(bool window)
{
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
}


void
SoVCSubPanelKit::setSPPartVisibility(unsigned int sppm)
{
  spParts.setValue(sppm);
  updateSP();
}

void
SoVCSubPanelKit::alignWidgets()
{
  SbString name = parent->getSkinDirectory();
  name += parent->getSkinFileName();    
  StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());

  int elementsMaxCol = 1;
  UAGETNODE(elementsMaxCol_,SoInfo,"SP_ELEMENTS_MAX_PER_COL",skin);
  elementsMaxCol = atoi(elementsMaxCol_->string.getValue().getString());
  float tX, tY, tZ;
  UAGETNODE(refWidgetTranslation_, SoTranslation, "SP_ELEMENTS_SPACING", skin);
  refWidgetTranslation_->translation.getValue().getValue(tX, tY, tZ); 
  
  float xFactor, yFactor;
  xFactor = yFactor = 0.0;
  
  if(currentWidgetKit == NULL)
    {
      for(int i=0; i<getWidgets()->getNumChildren(); i++)
	{
	  if(yFactor == elementsMaxCol)
	    {	 
	      yFactor = 0;
	      xFactor = xFactor + 1.0f;
	    }

	  ((SoVCWidgetKit*)getWidgets()->getChild(i))->setTranslation(SbVec3f(xFactor + xFactor*tX, 0, yFactor + yFactor*tY)); // switched y,z cause of rot.
	  
	  yFactor =  yFactor + 1.0f;
	}
    }
  else
    {
      for(int i=0; i<currentWidgetKit->getWidgetChildren()->getNumChildren(); i++)
	{
	  if(yFactor == elementsMaxCol)
	    {	 
	      yFactor = 0;
	      xFactor = xFactor + 1.0f;
	    }
	  
	  ((SoVCWidgetKit*)currentWidgetKit->getWidgetChildren()->getChild(i))->setTranslation(SbVec3f(xFactor + xFactor*tX, 0, yFactor + yFactor*tY));
	  
	  yFactor =  yFactor + 1.0f;
	}
    }
}

void
SoVCSubPanelKit::setupTransform()
{
  SbString name = parent->getSkinDirectory();
  name += parent->getSkinFileName();    
  StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());

  int elementsMaxCol = 1;
  UAGETNODE(elementsMaxCol_,SoInfo,"SP_ELEMENTS_MAX_PER_COL",skin);
  elementsMaxCol = atoi(elementsMaxCol_->string.getValue().getString());

  float tX, tY, tZ;
  UAGETNODE(refWidgetTranslation_, SoTranslation, "SP_ELEMENTS_SPACING", skin);
  refWidgetTranslation_->translation.getValue().getValue(tX, tY, tZ); 

  float panelWidth = 1.0;
  UAGETNODE(panelWidth_,SoInfo,"SP_WIDTH",skin);
  sscanf(panelWidth_->string.getValue().getString(),"%f", &panelWidth);

  static SbViewportRegion vp;
  static SoGetBoundingBoxAction bbAction(vp);
  SoVCBarKit* wBar = SO_GET_ANY_PART(this, "windowBar", SoVCBarKit);

  bbAction.apply(wBar);
  SbBox3f box = bbAction.getBoundingBox();
  float sizeX, sizeY, sizeZ;
  if (!box.isEmpty()) 
    {
      box.getSize(sizeX, sizeY, sizeZ);
    }
  
  float spWindowPanelHeight = sizeZ;
  SbVec3f spBodySize;
  spBodySize[0] = 1.0;
  spBodySize[1] = 1.0f - spWindowPanelHeight;
  spBodySize[2] = 1.0;

  int widgetCount = 0;
  if(currentWidgetKit == NULL)
    widgetCount = getWidgets()->getNumChildren();
  else
    widgetCount = currentWidgetKit->getWidgetChildren()->getNumChildren();
    
  int elementsMaxRow = (int)(float(widgetCount/elementsMaxCol) + 0.5);

  SbVec3f sizeContentBBox;
  float radius = 1.0;

  sizeContentBBox[0] = (elementsMaxRow + ((float)elementsMaxRow-1.0f)*tX) * radius;
  sizeContentBBox[1] = (elementsMaxCol + ((float)elementsMaxCol-1.0f)*tY) * radius;
  sizeContentBBox[2] = radius;
  
  float finalScaleX, finalScaleY, finalScaleZ;
  finalScaleX = finalScaleY = finalScaleZ = radius;

  if(sizeContentBBox[0] > max(sizeContentBBox[1], sizeContentBBox[2]) && sizeContentBBox[0] > 1)
    {
      finalScaleX = finalScaleY = finalScaleZ = 1.0/sizeContentBBox[0];
    }
  else if(sizeContentBBox[1] > max(sizeContentBBox[0], sizeContentBBox[2]) && sizeContentBBox[1] > 1)
    {
      finalScaleX = finalScaleY = finalScaleZ = 1.0/sizeContentBBox[1];
    }
  else if(sizeContentBBox[2] > max(sizeContentBBox[0], sizeContentBBox[1]) && sizeContentBBox[2] > 1)
    {
      finalScaleX = finalScaleY = finalScaleZ = 1.0/sizeContentBBox[2];
    }

  float tmpScaleX = finalScaleX;
  if(sizeContentBBox[0]*finalScaleX > spBodySize[0])
    tmpScaleX = (spBodySize[0] / (sizeContentBBox[0]*finalScaleX))*finalScaleX;
  float tmpScaleY = finalScaleY;
  if(sizeContentBBox[1]*finalScaleY > spBodySize[1])
    tmpScaleY = (spBodySize[1] / (sizeContentBBox[1]*finalScaleY))*finalScaleY;
  float tmpScaleZ = finalScaleZ;
  if(sizeContentBBox[2]*finalScaleZ > spBodySize[2])
    tmpScaleZ = (spBodySize[2] / (sizeContentBBox[2]*finalScaleZ))*finalScaleZ;

  float finalScale = min(min(tmpScaleX, tmpScaleY), tmpScaleZ);

 
  SO_GET_ANY_PART(this, "widgetTransform", SoTransform)->scaleFactor.setValue(finalScale, finalScale, finalScale);
  SO_GET_ANY_PART(this, "widgetTransform", SoTransform)->rotation.setValue(SbRotation(SbVec3f(1,0,0),M_PI/2.0));  
  SO_GET_ANY_PART(this, "widgetTransform", SoTransform)->translation.setValue(-spBodySize[0]/2.0 + (radius*finalScale)/2.0,
  									   1.0/2.0 - spWindowPanelHeight - (radius*finalScale)/2.0,
  									   0);
  SO_GET_ANY_PART(this, "transform", SoTransform)->scaleFactor.setValue(panelWidth, panelWidth, panelWidth);
}

SoTransform*
SoVCSubPanelKit::getTransform()
{
  return SO_GET_ANY_PART(this, "transform", SoTransform);
}

SoVCMainPanelKit*
SoVCSubPanelKit::getParent()
{
  return parent;
}

SoSeparator*
SoVCSubPanelKit::getWidgetRoot()
{
  return SO_GET_ANY_PART(this, "widgetRoot", SoSeparator);
}

void
SoVCSubPanelKit::setCurrentWidgetKitName(SoSFString widgetName)
{
  currentWidgetKitName = widgetName;
}

void
SoVCSubPanelKit::setCurrentWidgetKit(SoVCWidgetKit* widgetKit)
{
  oldWidgetKit = currentWidgetKit;
  currentWidgetKit = widgetKit;
}

SoSFString
SoVCSubPanelKit::getCurrentWidgetKitName()
{
  return currentWidgetKitName;
}

SoVCWidgetKit*
SoVCSubPanelKit::getCurrentWidgetKit()
{
  return currentWidgetKit;
}

int
SoVCSubPanelKit::getSizedState()
{
  return sizedState;
}

SbString
SoVCSubPanelKit::getTitle()
{
  return title.getValue();
}

void 
SoVCSubPanelKit::handle3DEvent(SoHandle3DEventAction *h3a)
{
    
}

void SoVCSubPanelKit::stateChangedCB( void * userdata, SoSensor * sensor )
{
  assert( userdata != NULL );
  SoVCSubPanelKit * self = (SoVCSubPanelKit *) userdata;

  self->adaptGeometryToState();
}

void SoVCSubPanelKit::adaptGeometryToState()
{
}

void
SoVCSubPanelKit::scaleSPGeometry()
{
}

void
SoVCSubPanelKit::setHandleHierarchyUp(SbBool state)
{
  handleHierarchyUp = state;
}

// animation stuff

void
SoVCSubPanelKit::animationHDownSensorCB(void *userdata, SoSensor *sensor)
{   
  SoVCSubPanelKit *spKit = (SoVCSubPanelKit*)userdata;
  SoVCWidgetKit *currentWKit = spKit->getCurrentWidgetKit();  

  spKit->getParent()->enlargeSubPanel(spKit);

  // it's a fix for the unusual highlighting of all widgets after hierachy down
  for(int i=0; i<currentWKit->getWidgetChildren()->getNumChildren(); i++)
  {
    ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->setAnimationState(true);
  }
   
  // do animation stuff here
  int animSteps = spKit->getAnimStepsCount();

  SbVec3f sizePreBBox;
  spKit->getAnimSaveCurrWidgetPreBBox().getSize(sizePreBBox[0], sizePreBBox[1], sizePreBBox[2]);
  SbVec3f sizePostBBox;
  spKit->getAnimSaveCurrWidgetPostBBox().getSize(sizePostBBox[0], sizePostBBox[1], sizePostBBox[2]);
  
  // compute new scale
  double newScaleX = ((sizePostBBox[0] / (double)ANIMATION_STEPS_INTERVALL) * (double)animSteps) / sizePostBBox[0];
  double newScaleY = ((sizePostBBox[1] / (double)ANIMATION_STEPS_INTERVALL) * (double)animSteps) / sizePostBBox[1];
  double newScaleZ = ((sizePostBBox[2] / (double)ANIMATION_STEPS_INTERVALL) * (double)animSteps) / sizePostBBox[2];

  // compute new translation
  SbVec3f centerPreBBox = spKit->getAnimSaveCurrWidgetPreBBox().getCenter();
  SbVec3f centerPostBBox = spKit->getAnimSaveCurrWidgetPostBBox().getCenter();
  double distCentersX;
  double distCentersY;
  double distCentersZ;
  double newTranslXIncr;
  double newTranslYIncr;
  double newTranslZIncr;

  // set new transformation
  SbString name = spKit->getParent()->getSkinDirectory();
  name += spKit->getParent()->getSkinFileName();    
  StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());

  int elementsMaxCol = 1;
  UAGETNODE(elementsMaxCol_,SoInfo,"SP_ELEMENTS_MAX_PER_COL",skin);
  elementsMaxCol = atoi(elementsMaxCol_->string.getValue().getString());

  float tX, tY, tZ;
  UAGETNODE(refWidgetTranslation_, SoTranslation, "SP_ELEMENTS_SPACING", skin);
  refWidgetTranslation_->translation.getValue().getValue(tX, tY, tZ); 

  float xFactor, yFactor;
  xFactor = yFactor = 0.0;

  for(int i=0; i<currentWKit->getWidgetChildren()->getNumChildren(); i++)
  {	   
    distCentersX = centerPreBBox[0]-(sizePostBBox[0]/2.0 - 0.5)*newScaleX;
    distCentersY = centerPreBBox[1]-(sizePostBBox[1]/2.0 - 0.5)*newScaleY;
    distCentersZ = centerPreBBox[2]-(sizePostBBox[2]/2.0 - 0.5)*newScaleZ;
    newTranslXIncr = (distCentersX / (double)ANIMATION_STEPS_INTERVALL) * (double)animSteps;
    newTranslYIncr = (distCentersY / (double)ANIMATION_STEPS_INTERVALL) * (double)animSteps;
    newTranslZIncr = (distCentersZ / (double)ANIMATION_STEPS_INTERVALL) * (double)animSteps;
    
    ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->getTransform()->scaleFactor.setValue(newScaleX, newScaleZ, newScaleY);
    
    if(yFactor == elementsMaxCol)
      {	 
	yFactor = 0;
	xFactor = xFactor + 1.0;
      }
        
    ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->setTranslation(SbVec3f((xFactor + xFactor*tX) * newScaleX + distCentersX,
											    0 + distCentersY, 
											    (yFactor + yFactor*tY) * newScaleZ + distCentersZ));
      
    yFactor =  yFactor + 1.0;       
      
    SbVec3f oldTranslation = ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->getTransform()->translation.getValue();    
    ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->getTransform()->translation.setValue(oldTranslation[0] - newTranslXIncr,
													  oldTranslation[1] - newTranslYIncr,
													  oldTranslation[2] - newTranslZIncr);                     
  }
  
  // TODO: use elementsSpacing, elementsMaxCol  from skinfile
  
  // check if we are done allready
  if(spKit->getAnimStepsCount() == ANIMATION_STEPS_INTERVALL)
    {
      for(int i=0; i<currentWKit->getWidgetChildren()->getNumChildren(); i++)
	{
	  ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->setAnimationState(false);
	}
      sensor->unschedule();
    }
  
  if(animSteps == 1)
    spKit->displayCurrentWKitChilds(true);

  // increment animcounter
  spKit->inc1AnimStepsCount();
}

void
SoVCSubPanelKit::displayCurrentWKitChilds(bool state)
{
  if(state)
    currentWidgetKit->getWidgetChildren()->whichChild.setValue(SO_SWITCH_ALL);
  else
    currentWidgetKit->getWidgetChildren()->whichChild.setValue(SO_SWITCH_NONE);
}

void
SoVCSubPanelKit::animationHUpSensorCB(void *userdata, SoSensor *sensor)
{   
  SoVCSubPanelKit *spKit = (SoVCSubPanelKit*)userdata;
  SoVCWidgetKit *currentWKit = spKit->getCurrentWidgetKit();  
  int animSteps = spKit->getAnimStepsCount();

   // it's a fix for the unusual highlighting of all widgets after hierachy down
  for(int i=0; i<currentWKit->getWidgetChildren()->getNumChildren(); i++)
  {
    ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->setAnimationState(true);
  }  

  // do animation stuff here
  SbVec3f sizePreBBox;
  spKit->getAnimSaveCurrWidgetPreBBox().getSize(sizePreBBox[0], sizePreBBox[1], sizePreBBox[2]);
  SbVec3f sizePostBBox;
  spKit->getAnimSaveCurrWidgetPostBBox().getSize(sizePostBBox[0], sizePostBBox[1], sizePostBBox[2]);
  
  // compute new scale
  double newScaleX = ((sizePreBBox[0] / (double)ANIMATION_STEPS_INTERVALL) * ((double)ANIMATION_STEPS_INTERVALL - (double)animSteps)) / sizePreBBox[0];
  double newScaleY = ((sizePreBBox[1] / (double)ANIMATION_STEPS_INTERVALL) * ((double)ANIMATION_STEPS_INTERVALL - (double)animSteps)) / sizePreBBox[1];
  double newScaleZ = ((sizePreBBox[2] / (double)ANIMATION_STEPS_INTERVALL) * ((double)ANIMATION_STEPS_INTERVALL - (double)animSteps)) / sizePreBBox[2];

  // compute new translation
  SbVec3f centerPreBBox = spKit->getAnimSaveCurrWidgetPreBBox().getCenter();
  SbVec3f centerPostBBox = spKit->getAnimSaveCurrWidgetPostBBox().getCenter();
  double distCentersX;
  double distCentersY;
  double distCentersZ;
  double newTranslXIncr;
  double newTranslYIncr;
  double newTranslZIncr;

  // set new transformation
  SbString name = spKit->getParent()->getSkinDirectory();
  name += spKit->getParent()->getSkinFileName();    
  StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());

  int elementsMaxCol = 1;
  UAGETNODE(elementsMaxCol_,SoInfo,"SP_ELEMENTS_MAX_PER_COL",skin);
  elementsMaxCol = atoi(elementsMaxCol_->string.getValue().getString());

  float tX, tY, tZ;
  UAGETNODE(refWidgetTranslation_, SoTranslation, "SP_ELEMENTS_SPACING", skin);
  refWidgetTranslation_->translation.getValue().getValue(tX, tY, tZ); 

  float xFactor, yFactor;
  xFactor = yFactor = 0.0;

  for(int i=0; i<currentWKit->getWidgetChildren()->getNumChildren(); i++)
  {	   
    distCentersX = centerPreBBox[0] - centerPostBBox[0];
    distCentersY = centerPreBBox[1] - centerPostBBox[1];
    distCentersZ = centerPreBBox[2] - centerPostBBox[2];
    newTranslXIncr = (distCentersX / (double)ANIMATION_STEPS_INTERVALL) * (double)animSteps;
    newTranslYIncr = (distCentersY / (double)ANIMATION_STEPS_INTERVALL) * (double)animSteps;
    newTranslZIncr = (distCentersZ / (double)ANIMATION_STEPS_INTERVALL) * (double)animSteps;
    
    ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->getTransform()->scaleFactor.setValue(newScaleX, newScaleZ, newScaleY);
    
    if(yFactor == elementsMaxCol)
      {	 
	yFactor = 0;
	xFactor = xFactor + 1.0;
      }
    
    ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->setTranslation(SbVec3f((xFactor+xFactor*tX)*newScaleX+centerPreBBox[0]-(sizePreBBox[0]/2.0-0.5)*newScaleX,
											    0+centerPreBBox[1]-(sizePreBBox[1]/2.0-0.5)*newScaleY, 
											    (yFactor+yFactor*tY)*newScaleZ+centerPreBBox[2]-(sizePreBBox[2]/2.0-0.5)*newScaleZ));
    
    yFactor =  yFactor + 1.0;      
    
    
    SbVec3f oldTranslation = ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->getTransform()->translation.getValue();    
    ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->getTransform()->translation.setValue(oldTranslation[0] - newTranslXIncr,
													  oldTranslation[1] - newTranslYIncr,
													    oldTranslation[2] - newTranslZIncr);    
    
  }
  
  // check if we are done allready
  if(spKit->getAnimStepsCount() == ANIMATION_STEPS_INTERVALL)
    {
      for(int i=0; i<currentWKit->getWidgetChildren()->getNumChildren(); i++)
	{
	  ((SoVCWidgetKit*)currentWKit->getWidgetChildren()->getChild(i))->setAnimationState(false);
	}

      if(currentWKit->getWidgetParent() != NULL)
	currentWKit->getWidgetParent()->getWidgetChildren()->whichChild = SO_SWITCH_ALL;	
      else
	spKit->getWidgets()->whichChild = SO_SWITCH_ALL;

      currentWKit->setVisibility(true);
      currentWKit->getWidgetChildren()->whichChild = SO_SWITCH_NONE;
      
      spKit->setCurrentWidgetKit(currentWKit->getWidgetParent());
      spKit->setHandleHierarchyUp(false);
      spKit->alignWidgets();
      spKit->setupTransform();
      spKit->getParent()->enlargeSubPanel(spKit);
      spKit->getParent()->updateStatusBar(currentWKit->getWidgetParent(), spKit);
      
      sensor->unschedule();
    }
  
  // increment animcounter
  spKit->inc1AnimStepsCount();
}

int
SoVCSubPanelKit::getAnimStepsCount()
{
  return animStepsCount;
}

void
SoVCSubPanelKit::inc1AnimStepsCount()
{
  animStepsCount++;
}

SbBox3f&
SoVCSubPanelKit::getAnimSaveCurrWidgetPreBBox()
{
  return animSaveCurrWidgetPreBBox;
}

SbBox3f&
SoVCSubPanelKit::getAnimSaveCurrWidgetPostBBox()
{
  return animSaveCurrWidgetPostBBox;
}

