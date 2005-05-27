#include <iostream>

#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/SoLocaleManagerKit.h>
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/resource/StbSkinLoader.h>

#include <stbapi/vc/SoVCManager.h>
#include <stbapi/vc/SoVCBarKit.h>
#include <stbapi/vc/SoVCKit.h>
#include <stbapi/vc/globaldef.h>
#include <stbapi/vc/SoVCWidgetKit.h>

#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoRotationXYZ.h>

SO_NODE_SOURCE(SoVCManager);

void
SoVCManager::initClass()
{
   SO_NODE_INIT_CLASS(SoVCManager, SoNode, "Node");
}

SoVCManager::SoVCManager()
{
    SO_NODE_CONSTRUCTOR(SoVCManager);

    pinVC = false;
}

SoVCManager::~SoVCManager()
{
}

void
SoVCManager::createResources(SbAppID appID)
{
  virtualCockpit->createResources(appID);
}

void
SoVCManager::useResources(SbAppID appID)
{
  virtualCockpit->useResources(appID);
}

void
SoVCManager::freeResources(SbAppID appID)
{
  virtualCockpit->freeResources(appID);
}

void
SoVCManager::setupBars()
{
  SbString texfilename = "";
  SbString dirName = virtualCockpit->skinDirectory.getValue();
  SbString name = virtualCockpit->skinDirectory.getValue();
  name += virtualCockpit->skinFileName.getValue();    
  StbSkinLoader *skin = StbSkinLoader::getSkin(name.getString());
  
  int maxElements = 1;
  SbVec3f size;
  
  // *** setup TASK BAR *** //

  // create task bar and setup transform    
  UAGETNODE(maxTaskElements_,SoInfo,"VC_TASK_BAR_MAX_ELEMENTS",skin);
  maxElements = atoi(maxTaskElements_->string.getValue().getString());
  SoVCBarKit* taskBar = new SoVCBarKit(maxElements, skin, "VC_TASK_BAR_");
  UAGETNODE(startTask_, SoTranslation, "VC_TASK_BAR_START_TRANSLATION", skin);
  UAGETNODE(sizeTask_, SoScale, "VC_TASK_BAR_SIZE", skin);
  size = sizeTask_->scaleFactor.getValue();
  
  taskBar->setupTransform(startTask_->translation.getValue(), size, virtualCockpit->vcSize.getValue());
  taskBar->ref();
  virtualCockpit->setTaskBar(taskBar);

  // *** setup SYSTEM BAR *** //

  // create system bar and setup transform
  UAGETNODE(maxSystemElements_,SoInfo,"VC_SYSTEM_BAR_MAX_ELEMENTS",skin);
  maxElements = atoi(maxSystemElements_->string.getValue().getString());
  SoVCBarKit* systemBar = new SoVCBarKit(maxElements, skin, "VC_SYSTEM_BAR_");
  UAGETNODE(startSystem_, SoTranslation, "VC_SYSTEM_BAR_START_TRANSLATION", skin);
  UAGETNODE(sizeSystem_, SoScale, "VC_SYSTEM_BAR_SIZE", skin);
  size = sizeSystem_->scaleFactor.getValue();
  systemBar->setupTransform(startSystem_->translation.getValue(), size, virtualCockpit->vcSize.getValue());
  // add system bar elements
  systemBar->addBarElement(VC_OPEN_APP,      "VC_OPEN_APP",      NULL, systemBarCB, this);
  systemBar->addBarElement(VC_CLOSE_APP,     "VC_CLOSE_APP",     NULL, systemBarCB, this);
  systemBar->addBarElement(VC_SAVE_APP,      "VC_SAVE_APP",      NULL, systemBarCB, this);

  // set system bar elements' content
  SoSeparator *openCont = new SoSeparator();
  SoMaterial *openContMat = new SoMaterial();
  openContMat->diffuseColor.setValue(1.0, 1.0, 1.0);
  openCont->addChild(openContMat);
  SoRotationXYZ *openContRot = new SoRotationXYZ();
  openContRot->angle.setValue(-M_PI/2.0);
  openContRot->axis = SoRotationXYZ::X;
  openCont->addChild(openContRot);
  SoTransform *openContTransf = new SoTransform();
  openContTransf->scaleFactor.setValue(0.6, 0.6, 0.6);
  openCont->addChild(openContTransf);
  SoTexture2 * openContTex = new SoTexture2();
  texfilename = "";
  texfilename += dirName;
  texfilename += SbString("open.jpg");
  openContTex->filename = texfilename;
  openCont->addChild(openContTex);
  SoCube *openContGeom = new SoCube();
  openContGeom->width = 1.0;
  openContGeom->height = 1.0;
  openContGeom->depth = 0.01;
  openCont->addChild(openContGeom);
  openCont->ref();
  
  systemBar->getElement(VC_OPEN_APP)->setContent(openCont);

  SoSeparator *closeCont = new SoSeparator();
  SoMaterial *closeContMat = new SoMaterial();
  closeContMat->diffuseColor.setValue(1.0, 1.0, 1.0);
  closeCont->addChild(closeContMat);
  SoRotationXYZ *closeContRot = new SoRotationXYZ();
  closeContRot->angle.setValue(-M_PI/2.0);
  closeContRot->axis = SoRotationXYZ::X;
  closeCont->addChild(closeContRot);
  SoTransform *closeContTransf = new SoTransform();
  closeContTransf->scaleFactor.setValue(0.6, 0.6, 0.6);
  closeCont->addChild(closeContTransf);
  SoTexture2 * closeContTex = new SoTexture2();
  texfilename = "";
  texfilename += dirName;
  texfilename += SbString("close.jpg");
  closeContTex->filename = texfilename;
  closeCont->addChild(closeContTex);
  SoCube *closeContGeom = new SoCube();
  closeContGeom->width = 1.0;
  closeContGeom->height = 1.0;
  closeContGeom->depth = 0.01;
  closeCont->addChild(closeContGeom);
  closeCont->ref();

  systemBar->getElement(VC_CLOSE_APP)->setContent(closeCont);

  SoSeparator *saveCont = new SoSeparator();
  SoMaterial *saveContMat = new SoMaterial();
  saveContMat->diffuseColor.setValue(1.0, 1.0, 1.0);
  saveCont->addChild(saveContMat);
  SoRotationXYZ *saveContRot = new SoRotationXYZ();
  saveContRot->angle.setValue(-M_PI/2.0);
  saveContRot->axis = SoRotationXYZ::X;
  saveCont->addChild(saveContRot);
  SoTransform *saveContTransf = new SoTransform();
  saveContTransf->scaleFactor.setValue(0.6, 0.6, 0.6);
  saveCont->addChild(saveContTransf);
  SoTexture2 * saveContTex = new SoTexture2();
  texfilename = "";
  texfilename += dirName;
  texfilename += SbString("save.jpg");
  saveContTex->filename = texfilename;
  saveCont->addChild(saveContTex);
  SoCube *saveContGeom = new SoCube();
  saveContGeom->width = 1.0;
  saveContGeom->height = 1.0;
  saveContGeom->depth = 0.01;
  saveCont->addChild(saveContGeom);
  saveCont->ref();

  systemBar->getElement(VC_SAVE_APP)->setContent(saveCont);  

  // set bar in vc
  systemBar->ref();
  virtualCockpit->setSystemBar(systemBar);


  // *** setup WINDOW BAR *** //

  // create window bar and setup transform
  UAGETNODE(maxWindowElements_,SoInfo,"VC_WINDOW_BAR_MAX_ELEMENTS",skin);
  maxElements = atoi(maxWindowElements_->string.getValue().getString());
  SoVCBarKit* windowBar = new SoVCBarKit(maxElements, skin, "VC_WINDOW_BAR_");
  UAGETNODE(startWindow_, SoTranslation, "VC_WINDOW_BAR_START_TRANSLATION", skin);
  UAGETNODE(sizeWindow_, SoScale, "VC_WINDOW_BAR_SIZE", skin);
  size = sizeWindow_->scaleFactor.getValue();
  windowBar->setupTransform(startWindow_->translation.getValue(), size, virtualCockpit->vcSize.getValue());

  // set window bar elements' content (MAX WIDGET)
  SoSeparator *maxCont = new SoSeparator();
  SoMaterial *maxContMaterial = new SoMaterial();
  SoRotationXYZ *maxContRot = new SoRotationXYZ();
  maxContRot->axis = SoRotationXYZ::X;
  maxContRot->angle = -M_PI / 2.0;
  SoCone *maxContShape = new SoCone();
  maxContShape->bottomRadius = 0.2;
  maxContShape->height = 0.4;
  maxContMaterial->diffuseColor = SbVec3f(0.0, 1.0, 0.0);
  maxContMaterial->transparency = 0.0;
  maxCont->addChild(maxContRot);
  maxCont->addChild(maxContMaterial);
  maxCont->addChild(maxContShape);

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

  // set window bar elements' content (CLOSE WIDGET) => no content

  // add window bar elements
  windowBar->addBarElement(VC_PIN_VC,   "VC_PIN_VC",   NULL, windowBarCB, this);

  SoSeparator *pinCont = new SoSeparator();
  SoMaterial *pinContMat = new SoMaterial();
  pinContMat->diffuseColor.setValue(1.0, 1.0, 1.0);
  pinCont->addChild(pinContMat);
  SoRotationXYZ *pinContRot = new SoRotationXYZ();
  pinContRot->angle.setValue(-M_PI/2.0);
  pinContRot->axis = SoRotationXYZ::X;
  pinCont->addChild(pinContRot);
  SoTransform *pinContTransf = new SoTransform();
  pinContTransf->scaleFactor.setValue(0.6, 0.6, 0.6);
  pinCont->addChild(pinContTransf);
  SoTexture2 * pinContTex = new SoTexture2();
  texfilename = "";
  texfilename += dirName;
  texfilename += SbString("stick.jpg");
  pinContTex->filename = texfilename;
  pinCont->addChild(pinContTex);
  SoCube *pinContGeom = new SoCube();
  pinContGeom->width = 1.0;
  pinContGeom->height = 1.0;
  pinContGeom->depth = 0.01;
  pinCont->addChild(pinContGeom);
  pinCont->ref();

  windowBar->getElement(VC_PIN_VC)->setContent(pinCont);
  windowBar->addBarElement(VC_CLOSE_VC, "VC_CLOSE_VC", NULL, windowBarCB, this);
  windowBar->addBarElement(VC_MIN_VC,   "VC_MIN_VC",   minCont, windowBarCB, this, SbVec3f(0,0,0),false);
  windowBar->addBarElement(VC_MAX_VC,   "VC_MAX_VC",   maxCont, windowBarCB, this, SbVec3f(0,0,0),false);

  // set bar in vc
  windowBar->ref();
  virtualCockpit->setWindowBar(windowBar);
}

void
SoVCManager::systemBarCB(void* userData, SoVCWidgetKit* widgetKit)
{
  SoUserKit* uk = ((SoVCManager*)userData)->userKit;
  SoVCKit* vc = ((SoVCManager*)userData)->virtualCockpit;

  if(widgetKit->getWidgetID() == VC_OPEN_APP)
    {
      // show fileBrowser
      printf("***OPEN_APP called****\n");
    }
  else if(widgetKit->getWidgetID() == VC_CLOSE_APP)
    {
      SoContextManagerKit::getInstance()->stopApplication(uk->getCurrentApp());
    }
  else if(widgetKit->getWidgetID() == VC_SAVE_APP)
    {
      SoContextManagerKit::getInstance()->saveApplication(uk->getCurrentApp());
    }
  else if(widgetKit->getWidgetID() == VC_SAVE_AS_APP)
    {
      SoContextManagerKit::getInstance()->saveApplicationAs(uk->getCurrentApp());
    }
  else if(widgetKit->getWidgetID() == VC_SAVE_ALL_APPS)
    {
      SoContextManagerKit::getInstance()->saveAll();
    }
  
  vc->updateVC();
}

void
SoVCManager::windowBarCB(void *userData, SoVCWidgetKit *widgetKit)
{
  SoVCKit* vc = ((SoVCManager*)userData)->virtualCockpit;

  if(widgetKit->getWidgetID() == VC_CLOSE_VC)
    {
      SoGui::exitMainLoop();
    }
  else if(widgetKit->getWidgetID() == VC_MIN_VC)
    {
      // minimize VC
      vc->setActionMinMax(true);
      vc->setVCPartVisibility(SoVCKit::VC_WINDOWBAR);
    }
  else if(widgetKit->getWidgetID() == VC_MAX_VC)
    {
      // maximize VC
      vc->setActionMinMax(true);
      vc->setVCPartVisibility(SoVCKit::VC_FULL);
    
    }
  else if(widgetKit->getWidgetID() == VC_PIN_VC)
    {
      if(vc->isPinned())
	{
	  vc->pinMe(false);
	}
      else
	{
	  vc->pinMe(true);
	}
    }
}

void
SoVCManager::taskBarCB(void *userData, SoVCWidgetKit* widgetKit)
{
}

void
SoVCManager::setMainPanelGeometry(SbAppID appID, SoNode* mpGeom)
{
    virtualCockpit->setMainPanelGeometry(appID, mpGeom);
}
