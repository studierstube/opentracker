// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       MediDeskKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================

#include "MediDeskKit.h"


//global variables
voAppearance      *curAppearance;        //current volume appearance node
voGeometry        *curGeometry;          //current volume geometry node
SoClipPlane       *fixedClipPlane = NULL;
voStatus          *vstatus;
SoTransform       *volumeTmpTrans = NULL;
SoTrakEngine      *hmdEngine = NULL;
SoTrakEngine      *pipEngine = NULL;
SoTrakEngine      *penEngine = NULL;
SoSeparator       *clientVolume = NULL;
SoNode            *pipRoot = NULL;

#define ASSIGNTEXT(buttonName,textureName) \
           AVGETNODE(button,SoBase3DButton,buttonName); \
           AVGETNODE(texture,SoTexture2,textureName); \
           button->setupTexture(texture,0,0)

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(MediDeskKit);

//--initialization of context in Studierstube
CONTEXT_APPLICATION(MediDeskKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void
MediDeskKit::initClass(void)
{
    SO_KIT_INIT_CLASS(MediDeskKit, SoContextKit, "SoContextKit");
    //vstatus = new voStatus();

    voAppearanceElement::initClass();
    voAppearance::initClass(vstatus);
    voGeometry::initClass(vstatus);
    MySoSketchKit::initClass();
}


//----------------------------------------------------------------------------
// virtual constructor
//----------------------------------------------------------------------------

SoContextKit*
MediDeskKit::factory()
{
    return new MediDeskKit;
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

MediDeskKit::MediDeskKit()
{
    SO_KIT_CONSTRUCTOR(MediDeskKit);
    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

MediDeskKit::~MediDeskKit()
{
    destructor();
}

//----------------------------------------------------------------------------
// create drag separator
//----------------------------------------------------------------------------
SoDragSeparator* createDragSep(SoNode* content)
{
    //cout << "run create DragSep ..." << endl;
    NEWNODE(ds, SoDragSeparator);
    NEWNODE(t, SoTransform);
    t->translation.connectFrom(&ds->translation);
    t->rotation.connectFrom(&ds->rotation);
    ds->addChild(t);
    ds->addChild(content);
    return ds;
}

//----------------------------------------------------------------------------
// Callback functions 
//----------------------------------------------------------------------------

//
// VOLUME DISPLAY
//

//-----------------------------------------------------------------------------
// callback for setting new lookup table center value

void cbSetLutCenter(void*, So3DLogSlider* slider)
{
   int value = slider->currentValue.getValue();

   AVGETNODE(curAppearance,voAppearance,"VO_APPEARANCE");
   curAppearance->lutCenter.setValue(value);
}


//-----------------------------------------------------------------------------
// callback for setting new lookup table width value

void cbSetLutWidth(void*, So3DLogSlider* slider)
{
   int value = slider->currentValue.getValue();

   AVGETNODE(curAppearance,voAppearance,"VO_APPEARANCE");
   curAppearance->lutWidth.setValue(value);
}


//-----------------------------------------------------------------------------
// callback to specify display (use lut or not)

void cbUseLut(void*, So3DButton* )
{

   //cout << "useLut pressed ..." << endl;

   // set texture
   SoSeparator* sep = readFile("buttontextures.iv");
   SoBase3DButton* button;
   SoTexture2* texture;

   // set lut flag
   AVGETNODE(curAppearance,voAppearance,"VO_APPEARANCE");
   curAppearance->setUseLutStatus();

   if (vstatus->getUseLutStatus()) {
      ASSIGNTEXT("LUT_BUTTON","LUT_TEXTURE");
   } else {
      ASSIGNTEXT("LUT_BUTTON","NO_LUT_TEXTURE");
   }

}


//-----------------------------------------------------------------------------
// callback to specify the volume size

void cbSetVoScale(void*, So3DLogSlider* slider )
{
   float value = slider->currentValue.getValue();

   AVGETNODE(curAppearance,voAppearance,"VO_APPEARANCE");
   curAppearance->voScale = value;
}


//-----------------------------------------------------------------------------
// callback to use MIP mapping

void cbUseMip(void*, So3DButton* )
{

   //cout << "useMIP pressed ..." << endl;

   // set texture
   SoSeparator* sep = readFile("buttontextures.iv");
   SoBase3DButton* button;
   SoTexture2* texture;

   // set mip flag
   AVGETNODE(curAppearance,voAppearance,"VO_APPEARANCE");
   curAppearance->setUseMipStatus();

   vstatus->setMipStatus(!vstatus->getMipStatus());
   
   if (vstatus->getMipStatus()) {
     ASSIGNTEXT("MIP_BUTTON","MIP_TEXTURE");
   } else {
     ASSIGNTEXT("MIP_BUTTON","OVER_TEXTURE");
   }

}


//-----------------------------------------------------------------------------
// callback to use ATTENUATE mapping

void cbUseAttenuate(void*, So3DButton* )
{

   //cout << "useAttenuate pressed ..." << endl;

   // set texture
   SoSeparator* sep = readFile("buttontextures.iv");
   SoBase3DButton* button;
   SoTexture2* texture;

   vstatus->setAttenuateStatus(! vstatus->getAttenuateStatus());
   if (vstatus->getAttenuateStatus()) {
     ASSIGNTEXT("ATTENUATE_BUTTON","NO_BLUE_TEXTURE");
          
   } else {
     ASSIGNTEXT("ATTENUATE_BUTTON","BLUE_TEXTURE");

   }

}

//-----------------------------------------------------------------------------
// callback to use Volume of interest mapping

void cbUseVoi(void*, So3DButton* )
{

   //cout << "useVOI pressed ..." << endl;

   // set texture
   SoSeparator* sep = readFile("buttontextures.iv");
   SoBase3DButton* button;
   SoTexture2* texture;

   // set mip flag
   vstatus->setVOIStatus(! vstatus->getVOIStatus());
   if (vstatus->getVOIStatus()) {
      ASSIGNTEXT("VOI_BUTTON","VOI_TEXTURE");
   } else {
      ASSIGNTEXT("VOI_BUTTON","NO_VOI_TEXTURE");
   }


}

//-----------------------------------------------------------------------------
// callback to use Mirror mapping

void cbUseMirror(void*, So3DButton* )
{

   //cout << "useMirror pressed ..." << endl;

   // set texture
   SoSeparator* sep = readFile("buttontextures.iv");
   SoBase3DButton* button;
   SoTexture2* texture;

//jt
// We currently don't use the mirror feature, so this button
// is currently used for the orthogonal clipping planes mode
   vstatus->setOrthoStatus(! vstatus->getOrthoStatus());
   if (vstatus->getOrthoStatus()) {
      ASSIGNTEXT("MIRROR_BUTTON","MIRROR_TEXTURE");
   } else {
      ASSIGNTEXT("MIRROR_BUTTON","NO_MIRROR_TEXTURE");
   }
/*
   cout << "SORRY : This Mirror switch is not working in this version." << endl;
   cout << "        If you want to use the mirror functionality please start program with voStbMirror." << endl; 
       
   ASSIGNTEXT("MIRROR_BUTTON","NO_MIRROR_TEXTURE");
*/
}

//-----------------------------------------------------------------------------
// callback to use Slice mapping

void cbUseSlice(void*, So3DButton* )
{

   //cout << "useSlice pressed ..." << endl;

   // set texture
   SoSeparator* sep = readFile("buttontextures.iv");
   SoBase3DButton* button;
   SoTexture2* texture;

   //cout << "SORRY : Slicing unfortuately does not work in this version." << endl;
   
   vstatus->setSliceStatus(! vstatus->getSliceStatus());

   if (vstatus->getSliceStatus()) {
      ASSIGNTEXT("SLICE_BUTTON","SLICE_TEXTURE");
   } else {
      ASSIGNTEXT("SLICE_BUTTON","NO_SLICE_TEXTURE");
   }

}



//
// VOLUME
//


//-----------------------------------------------------------------------------
// callback 

void cbVolumeDragPress(void*, SoDragKit* s )
{
   //cout << "cbVolumeSensorPress ..." << endl;

   s->translation.connectFrom(&(s->translation));
   s->rotation.connectFrom(&(s->rotation));

   vstatus->setDragStatus(true);
}


//-----------------------------------------------------------------------------
// callback 

void cbVolumeDragRelease(void*, SoDragKit* s)
{
   //cout << "cbVolumeSensorRelease ..." << endl;

   s->translation.disconnect();
   s->rotation.disconnect();

   vstatus->setDragStatus(false);

}

//
// CLIP
//


//-----------------------------------------------------------------------------
// freezePlane
void freezeClipPlane(SoClipPlane *fcp) {
   //cout << "freeze clipping plane ..." << endl;

   SbVec3f p0, p1, p2;
   SbVec3f tp0, tp1, tp2;
   SbMatrix transMat, invTransMat;

   AGETNODE(freezeGroup,SoGroup,"FREEZE_CLIP_GROUP");
   AGETNODE(clipPlane,SoClipPlane,"CLIP_PLANE");

   SoXtViewer* viewer = StbWorkspace::getInstance()->getUserData(0).viewer;
   SbViewportRegion vpr = viewer->getViewportRegion(); 
   SoSearchAction* sa = new SoSearchAction();
   SoPath* fcpPath = new SoPath();
   SoGetMatrixAction* ma = new SoGetMatrixAction(vpr);
   SbMatrix* m = new SbMatrix();
   SbVec3f curPip0;
   SbVec3f curPip1;
   SbVec3f curPip2;
   sa->setNode(freezeGroup);
   sa->apply(StbWorkspace::getInstance()->getUserData(0).userRoot);
   fcpPath = sa->getPath();
   ma->apply(fcpPath);
   *m = ma->getInverse();
   vstatus->getClipPlaneVecs(p0,p1,p2);
   m->multVecMatrix(p0, tp0);
   m->multVecMatrix(p1, tp1);
   m->multVecMatrix(p2, tp2);

   SbPlane p(tp0, tp2, tp1);
   //p.offset(vstatus->getClipPlaneOffset());

   fcp->plane.setValue(p);
   fcp->on = true;

}

//-----------------------------------------------------------------------------
// unFreezePlane
void unFreezeClipOnePlane(SoClipPlane *fcp) {
   AGETNODE(fcpGroup,SoGroup,"FREEZE_CLIP_GROUP");
   fcpGroup->removeChild(fcp);
}

//-----------------------------------------------------------------------------
// unFreezePlane
void unFreezeAllClipPlane() {

   AGETNODE(fcpGroup,SoGroup,"FREEZE_CLIP_GROUP");
   if (fcpGroup->getNumChildren() == 0) return;
   fcpGroup->removeAllChildren();
}


//-----------------------------------------------------------------------------
// callback

void cbClipSensorPress(void*, So3DSensor* s )
{
   //cout << "cbClipSensorPress ..." << endl;
   
   int fcpCount;

   AGETNODE(fcpGroup,SoGroup,"FREEZE_CLIP_GROUP");

   //---------------------
   // slice texture of current clipping position 
   if (vstatus->getSliceStatus() && vstatus->getPipFlipStatus()) {

      // clipping planes needs to be frozen first because it is not 
      // possible to onScreenRender the scene including SoClipPlane
      SoClipPlane *fcp = new SoClipPlane();
      freezeClipPlane(fcp);      
      fcpGroup->addChild(fcp);

      // find needed nodes in the scene graph
      AGETNODE(volumeSep,SoSeparator,"VOLUME_DRAGGER");
      AGETNODE(sliceGroup,SoGroup,"SLICE_GROUP"); 
      AGETNODE(sd,SoDragKit,"SD");
      AGETNODE(sliceContent,SoSeparator,"SLICE_SEP");
      
      //remove old slice from pip
      AGETNODE(pipSlice,SoSeparator,"PIP_SLICE");
      pipSlice->removeAllChildren();

      //remove former created slices
      sliceContent->removeAllChildren();

      // create scene to render
      SoSeparator* bigScene = new SoSeparator;
      bigScene->ref();

      SoTrakEngine* trakSource = new SoTrakEngine();
      trakSource = vstatus->getPipEngine();
      SbVec3f cp0, cp1, cp2, e1, e2, n, m, pos;
      vstatus->getClipPlaneVecs(cp0, cp1, cp2);
      e1=cp1-cp0;
      e2=cp2-cp0;
      n=e2.cross(e1);
      n.normalize();
      m=n+cp0;

      NEWNODE(cam,SoPerspectiveCamera);
      pos=cp0-(0.2*n);
      cam->position = pos;
      cam->pointAt(m);
      cam->nearDistance = 0.001;
      cam->farDistance = 10;
      cam->heightAngle = M_PI/3.0; // 60 degrees vertical fisheye
      cam->viewportMapping = SoCamera::LEAVE_ALONE;

      NEWNODE(light,SoPointLight);
      light->location.connectFrom(&trakSource->translation);

      bigScene->addChild(cam);
      bigScene->addChild(light);
      bigScene->addChild(volumeSep);

      // create slice subgraph
      SoSeparator* sliceSep = new SoSeparator;
      SoTexture2* sliceTex = new SoTexture2;     

      SoMaterial* sliceMat = new SoMaterial;
      //sliceMat->emissiveColor.setValue(1.0, 1.0, 1.0);
      sliceMat->shininess.setValue(0.1);

      SoCube* slice = new SoCube;
      slice->width=0.256;
      slice->height=0.256;
      slice->depth=0.02;

      SoTransform* sliceTx = new SoTransform();
      sliceTx->translation.setValue(0.0,0.0,0.02);

      SoCube* frame = new SoCube;
      frame->width=0.276;
      frame->height=0.276;
      frame->depth=0.03;

      sliceSep->addChild(frame);
      sliceSep->addChild(sliceTx);
      sliceSep->addChild(sliceMat);
      sliceSep->addChild(sliceTex);
      sliceSep->addChild(slice);

      sliceContent->addChild(sliceSep);

      //add current slice to pip
      
      pipSlice->addChild(sliceMat);
      pipSlice->addChild(sliceTex);
      pipSlice->addChild(slice);

      // onscreen render scene
      SoXtViewer* viewer = StbWorkspace::getInstance()->getUserData(0).viewer;
      SbViewportRegion vpr = viewer->getViewportRegion(); 

      //const int size = 128;
      //const int size = 480;
      const int size = 256;
      //onScreenRenderScene(bigScene, vpr, sliceTex, size, size, GL_RGB);
      onScreenRenderScene(bigScene, vpr, sliceTex, size, size);

      fcpCount=fcpGroup->getNumChildren(); 
      if (fcpCount == 0) return;
   
      unFreezeClipOnePlane((SoClipPlane *)fcpGroup->getChild(fcpCount-1));
      
   }


   //---------------------
   // freeze clipping plane
   if ((vstatus->getVOIStatus()) && (vstatus->getPipFlipStatus())) {

      //cout << "FREEZE clipplane!!! " << endl;

      SoClipPlane *fcp = new SoClipPlane();
      freezeClipPlane(fcp);      
   
      fcpGroup->addChild(fcp);

   //---------------------
   // UN freeze LAST clipping plane
   } else if ((vstatus->getVOIStatus()) && !(vstatus->getPipFlipStatus())) {

      //cout << "UN-FREEZE clipplane!!! " << endl;
   
      fcpCount=fcpGroup->getNumChildren(); 
      if (fcpCount == 0) return;
   
      unFreezeClipOnePlane((SoClipPlane *)fcpGroup->getChild(fcpCount-1));

   }


}

//-----------------------------------------------------------------------------
// callback

void cbClipSensorRelease(void*, So3DSensor* s)
{
   //cout << "cbClipSensorRelease ..." << endl;

   //---------------------
   // slice texture of current clipping position 
   if (vstatus->getSliceStatus() && vstatus->getPipFlipStatus()) {

   }
}


//----------------------------------------------------------------------------
// Reads the pip sheet and sets the button callbacks
//----------------------------------------------------------------------------

SoSeparator*
MediDeskKit::createPipSheet(int uid)
{
//--set hmdEngine
    hmdEngine = comm->getSoTrakEngine(comm->getHmdFromUser(uid));
    vstatus->setHmdEngine(hmdEngine);

//--set pipEngine
    pipEngine = comm->getSoTrakEngine(comm->getPipFromUser(uid));
    vstatus->setPipEngine(pipEngine);

//--set penEngine
    penEngine = comm->getSoTrakEngine(comm->getPenFromUser(uid));
    vstatus->setPenEngine(penEngine);

//--pip cube is not shown in this application
    comm->showPipBody(uid, FALSE);

//--read pip sheet from file "sheet.iv"
    return readFile("sheet.iv",comm->workingDirectory);
}

//----------------------------------------------------------------------------
// createWindowGeometry
// create window and attach scene graph to it. 
//----------------------------------------------------------------------------

SoWindowKit*
MediDeskKit::createWindowGeometry(int index)
{
//--create a default window
    WindowCreate wc;
    SoWindowKit::defaultWindow(wc);
    SoWindowKit* windowKit = comm->createWindow(NULL, &wc, NULL, NULL);
//--need to start application maximized to get clipping planes.
//--open inventors clipping planes are limited to 6
    windowKit->maximize();
//--build scene graph composed of a material and a sphere node
//--and place it window's client area
    //SoSeparator* clientVolume = windowKit->getClientVolumeSeparator();
    clientVolume = windowKit->getClientVolumeSeparator();
    clientVolume->addChild(readFile("volume.iv",comm->workingDirectory));

    return windowKit;
}

//----------------------------------------------------------------------------

void
MediDeskKit::setSheetMasterMode(SoNode* pipSheet, SbBool masterMode)
{
//--find pointers to the red and blue color buttons
        
    //cout << "run setSheetMasterMode() ..." << endl;

    SoSeparator* sep = readFile("buttontextures.iv");
    SoBase3DButton* button;
    SoTexture2* texture;

    ASSIGNTEXT("LUT_BUTTON","NO_LUT_TEXTURE");
    ASSIGNTEXT("MIP_BUTTON","OVER_TEXTURE");
    ASSIGNTEXT("ATTENUATE_BUTTON","BLUE_TEXTURE");
    ASSIGNTEXT("VOI_BUTTON","NO_VOI_TEXTURE");
    ASSIGNTEXT("MIRROR_BUTTON","NO_MIRROR_TEXTURE");
    ASSIGNTEXT("SLICE_BUTTON","NO_SLICE_TEXTURE");

    pipRoot = pipSheet;

    So3DButton* bLut =         (So3DButton*)findNode(pipSheet,"LUT_BUTTON");
    So3DButton* bMip =         (So3DButton*)findNode(pipSheet,"MIP_BUTTON");
    So3DButton* bAttenuate =   (So3DButton*)findNode(pipSheet,"ATTENUATE_BUTTON");
    So3DButton* bVoi =         (So3DButton*)findNode(pipSheet,"VOI_BUTTON");
    So3DButton* bMirror =      (So3DButton*)findNode(pipSheet,"MIRROR_BUTTON");
    So3DButton* bSlice =       (So3DButton*)findNode(pipSheet,"SLICE_BUTTON");

    So3DLogSlider* slLutC =    (So3DLogSlider*)findNode(pipSheet,"LUT_CENTER_SLIDER");
    So3DLogSlider* slLutW =    (So3DLogSlider*)findNode(pipSheet,"LUT_WIDTH_SLIDER");
    So3DLogSlider* slScale =   (So3DLogSlider*)findNode(pipSheet,"SCALE_SLIDER");

    SoDragKit* dVolume =       (SoDragKit*)findNode(clientVolume,"VOLUME_DRAGGER");
    So3DSensor* sClip =        (So3DSensor*)findNode(clientVolume,"CLIP_SENSOR");

    if(masterMode) // if context is master -> set callbacks
    {
        //cout << "setSheetMasterMode:masterMode" << endl;
        
        this->forceFocusChange();

        dVolume->setPressCallback(cbVolumeDragPress,NULL);
        dVolume->setReleaseCallback(cbVolumeDragRelease,NULL);        
        sClip->setPressCallback(cbClipSensorPress,NULL);
        sClip->setReleaseCallback(cbClipSensorRelease,NULL);
        slLutC->addPositionChangeCallback(cbSetLutCenter,NULL);
        slLutW->addPositionChangeCallback(cbSetLutWidth,NULL);
        slScale->addPositionChangeCallback(cbSetVoScale,NULL);

        bLut->addReleaseCallback(cbUseLut,this);
        bMip->addReleaseCallback(cbUseMip,this);
        bAttenuate->addReleaseCallback(cbUseAttenuate,this);
        bVoi->addReleaseCallback(cbUseVoi,this);
        bMirror->addReleaseCallback(cbUseMirror,this);
        bSlice->addReleaseCallback(cbUseSlice,this);

    }
    else // if context is slave -> remove callbacks
    {
        //cout << "setSheetMasterMode:slaveMode" << endl;
        dVolume->removePressCallback(cbVolumeDragPress,NULL);
        dVolume->removeReleaseCallback(cbVolumeDragRelease,NULL);
        sClip->removePressCallback();
        sClip->removeReleaseCallback();
        slLutC->removePositionChangeCallback(cbSetLutCenter);
        slLutW->removePositionChangeCallback(cbSetLutWidth);
        slScale->removePositionChangeCallback(cbSetVoScale);

        bMip->removeReleaseCallback(cbUseMip);
        bLut->removeReleaseCallback(cbUseLut);
        bAttenuate->removeReleaseCallback(cbUseAttenuate);
        bVoi->removeReleaseCallback(cbUseVoi);
        bMirror->removeReleaseCallback(cbUseMirror);
        bSlice->removeReleaseCallback(cbUseSlice);

    }
    
}


