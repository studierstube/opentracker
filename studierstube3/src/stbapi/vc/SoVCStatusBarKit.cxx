#include <cassert>
#include <cstdlib>
#include <iostream>

#include <Inventor/SbBox.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSphere.h>

#include <stbapi/misc/ivmacros.h>
#include <stbapi/util/ivio.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/workspace/SoContextManagerKit.h>

#include <stbapi/vc/SoVCStatusBarKit.h>

#include <Inventor/nodes/SoTransparencyType.h>
#include <Inventor/actions/SoGLRenderAction.h>

#define ADD SO_KIT_ADD_CATALOG_ENTRY

using namespace std;

SO_KIT_SOURCE(SoVCStatusBarKit);

void
SoVCStatusBarKit::initClass()
{
    SO_KIT_INIT_CLASS(SoVCStatusBarKit, SoBaseKit, "BaseKit");
}

SoVCStatusBarKit::SoVCStatusBarKit(StbSkinLoader *skin, char *prefix)
  : skin(skin), prefix(prefix)
{
    SO_KIT_CONSTRUCTOR(SoVCStatusBarKit);

    ADD(barRoot,     SoSeparator,         FALSE, this,      \0, FALSE);
    ADD(barTransp,   SoTransparencyType,  FALSE, barRoot,   \0, FALSE);
    ADD(barTransf,   SoTransform,         FALSE, barRoot,   \0, FALSE);
    ADD(barGeomRoot, SoSeparator,         FALSE, barRoot,   \0, FALSE);
    ADD(barGeomTransf, SoTransform,       FALSE, barGeomRoot, \0, FALSE);
    ADD(barGeomMat,    SoMaterial,        FALSE, barGeomRoot, \0, FALSE);
    ADD(barGeomShape,  SoCube,            FALSE, barGeomRoot, \0, FALSE);
    ADD(barElements,   SoSwitch,          FALSE, barRoot,   \0, FALSE);

    SO_KIT_ADD_FIELD(updateElemContent, (false) );

    SO_KIT_INIT_INSTANCE();

    updateElemsContentSensor.setFunction( SoVCStatusBarKit::stateChangedCB );
    updateElemsContentSensor.setData( this );
    updateElemsContentSensor.attach( &updateElemContent );
    updateElemsContentSensor.setPriority( 1 );

    updateElemContent = false;
    updatedElementsContent = NULL;
    updatedElementsID = -1;
 

    SO_GET_ANY_PART(this, "barElements",SoSwitch)->whichChild.setValue(SO_SWITCH_ALL);

    SO_GET_ANY_PART(this, "barTransp", SoTransparencyType)->value = SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND;

    SO_GET_ANY_PART(this, "barGeomTransf", SoTransform)->translation.setValue(0.5, 0, 0);//0.5, 0.5);
    SO_GET_ANY_PART(this, "barGeomShape", SoCube)->width.setValue(1.0);
    SO_GET_ANY_PART(this, "barGeomShape", SoCube)->height.setValue(1.0);
    SO_GET_ANY_PART(this, "barGeomShape", SoCube)->depth.setValue(1.0);
    SO_GET_ANY_PART(this, "barGeomMat", SoMaterial)->diffuseColor = SbVec3f(0.0, 1.0, 0.0);
    SO_GET_ANY_PART(this, "barGeomMat", SoMaterial)->transparency.setValue(0.6);

    setSearchingChildren(TRUE);

    usedTotalSize = 0.0;
    idCount = 0;
}

SoVCStatusBarKit::~SoVCStatusBarKit()
{
  Stb3DEventGenerator::releaseAllGrabbers(this);
}

int
SoVCStatusBarKit::addBarElement(SoSeparator* content, double usedSize)
{ 
  if((usedTotalSize + usedSize) > 1.5)
    return -1;

  UAGETNODE(sizeStatus_, SoScale, "MP_STATUS_BAR_SIZE", skin);
  SbVec3f statusBarSize = sizeStatus_->scaleFactor.getValue();

  SbViewportRegion vp;
  SoGetBoundingBoxAction bba(vp);

  content->ref();
  bba.apply(content);
  SbBox3f contentBBox = bba.getXfBoundingBox().project();

  this->ref();
  bba.apply(this);
  SbBox3f barBBox = bba.getXfBoundingBox().project();

  SbVec3f sizeContentBBox;
  contentBBox.getSize(sizeContentBBox[0], sizeContentBBox[1], sizeContentBBox[2]);
  SbVec3f sizeBarBBox;
  barBBox.getSize(sizeBarBBox[0], sizeBarBBox[1], sizeBarBBox[2]);

  // compute new scale
  double newScaleX, newScaleY, newScaleZ;
  newScaleX = newScaleY = newScaleZ = 1.0;

  if(sizeContentBBox[0] > 1)
    newScaleX = 1.0 / sizeContentBBox[0];

  if(sizeContentBBox[1] > 1)
    newScaleY = 1.0 / sizeContentBBox[1];

  if(sizeContentBBox[2] > 1)
    newScaleZ = 1.0 / sizeContentBBox[2];

  newScaleX *= usedSize;

  double finalScaleX = newScaleX;
  double finalScaleY = newScaleY;
  double finalScaleZ = newScaleZ;

  if(newScaleX < 1 || newScaleY < 1 || newScaleZ < 1)
    {
      if(newScaleY < newScaleX)
	finalScaleX = finalScaleY = newScaleY;
      else
	finalScaleX = finalScaleY = newScaleX;
    }

  // compute new translation
  SbVec3f centerBarBBox = barBBox.getCenter();
  SbVec3f centerContentBBox = contentBBox.getCenter();
  double distCentersX = centerBarBBox[0] - centerContentBBox[0] * finalScaleX  + usedTotalSize/statusBarSize[0] +( usedSize/statusBarSize[0])/2.0;
  double distCentersY = centerBarBBox[1] - centerContentBBox[1] * finalScaleY;
  double distCentersZ = centerBarBBox[2] - centerContentBBox[2] * finalScaleZ;

  SoTransform* contentTransf = new SoTransform();
  contentTransf->scaleFactor.setValue(finalScaleX, finalScaleY, finalScaleZ);
  contentTransf->translation.setValue(distCentersX, distCentersY, distCentersZ);

  SoSeparator* newSep = new SoSeparator();
  newSep->addChild(contentTransf);
  newSep->addChild(content);

  SoSwitch* newSwitch = new SoSwitch();
  newSwitch->whichChild.setValue(SO_SWITCH_ALL);
  newSwitch->addChild(newSep);
  
  SO_GET_ANY_PART(this, "barElements", SoSwitch)->addChild(newSwitch);

  pair<double, double> elem;
  elem.first = usedSize;
  elem.second = usedTotalSize;
  elementSizes.push_back(elem);

  usedTotalSize += usedSize;
  idCount++;

  return idCount;
}

void
SoVCStatusBarKit::showElements(bool show)
{
  if(show)
    {
      SO_GET_ANY_PART(this, "barElements", SoSwitch)->whichChild = -3; // show
    }
  else
    {     
      SO_GET_ANY_PART(this, "barElements", SoSwitch)->whichChild = -1; // don't show
    }
}

void
SoVCStatusBarKit::showElement(int elementID, bool show)
{
  if(show)
    {
      ((SoSwitch*)SO_GET_ANY_PART(this, "barElements", SoSwitch)->getChild(elementID))->whichChild  = -3; // show
    }
  else
    {     
      ((SoSwitch*)SO_GET_ANY_PART(this, "barElements", SoSwitch)->getChild(elementID))->whichChild = -1; // don't show
    }
}

void
SoVCStatusBarKit::setupTransform(SbVec3f start, SbVec3f barSize, SbVec3f vcSize) 
{
  if (barSize[0] == 0.0 || barSize[1] == 0.0 || barSize[2] == 0.0)
    return;

  // get size of vc
  float vcX, vcY, vcZ;
  vcX = vcSize.getValue()[0]; 
  vcY = vcSize.getValue()[1];
  vcZ = vcSize.getValue()[2];

  // calculate scale factor for bar
  if (start[0] < 0.0 || start[0] >= 1.0)
    start[0] = 0.0;
  float sfX = vcX * barSize[0];

  if (start[1] < 0.0 || start[1] >= 1.0)
    start[1] = 0.0;
  float sfZ = vcY * barSize[1];

  if (start[2] < 0.0 || start[2] >= 1.0)
    start[2] = 0.0;
  float sfY = vcZ * barSize[2];

  float sfs = (sfX < sfY) ? sfX : sfY;

  // set transform node for bar
     
  UAGETNODE(statusBarDistance_,SoInfo,"MP_STATUS_BAR_DISTANCE",skin);
  float statusBarDistance;
  sscanf(statusBarDistance_->string.getValue().getString(),"%f", &statusBarDistance);
  
  float distanceScaleFact = 0.2333;
  float distanceScale = 1.0 + fabs(distanceScaleFact * statusBarDistance);
  distanceScale = 1.07;

  SO_GET_ANY_PART(this, "barTransf", SoTransform)->translation.setValue((-vcX/2.0+start[0])*distanceScale,
	 								(vcY/2.0-start[1] - 0.5*sfZ)*distanceScale,
									statusBarDistance);     
  SO_GET_ANY_PART(this, "barTransf", SoTransform)->scaleFactor.setValue(sfX*distanceScale,
	 								sfY*distanceScale,
									sfZ*distanceScale);      
  SO_GET_ANY_PART(this, "barTransf", SoTransform)->rotation.setValue(SbRotation(SbVec3f(1,0,0),M_PI/2.0));     
}

void
SoVCStatusBarKit::updateElementsContent(int elementID, SoSeparator *newContent)
{
  updatedElementsContent = newContent;
  updatedElementsID = elementID;
  
  updateElemContent = true;
}

void 
SoVCStatusBarKit::stateChangedCB( void * userdata, SoSensor * sensor )
{
  assert( userdata != NULL );
  SoVCStatusBarKit * self = (SoVCStatusBarKit *) userdata;
  self->adaptBarToState();
}


void
SoVCStatusBarKit::adaptBarToState()
{
  if(updateElemContent.getValue())
    {
      updateContent();
    }
}

void
SoVCStatusBarKit::updateContent()
{
  double usedSize = (elementSizes[updatedElementsID]).first;
  double usedTotalSize = (elementSizes[updatedElementsID]).second;

  UAGETNODE(sizeStatus_, SoScale, "MP_STATUS_BAR_SIZE", skin);
  SbVec3f statusBarSize = sizeStatus_->scaleFactor.getValue();

  SbViewportRegion vp;
  SoGetBoundingBoxAction bba(vp);

  updatedElementsContent->ref();
  bba.apply(updatedElementsContent);
  SbBox3f contentBBox = bba.getXfBoundingBox().project();

  bba.apply(this);
  SbBox3f barBBox = bba.getXfBoundingBox().project();

  SbVec3f sizeContentBBox;
  contentBBox.getSize(sizeContentBBox[0], sizeContentBBox[1], sizeContentBBox[2]);
  SbVec3f sizeBarBBox;
  barBBox.getSize(sizeBarBBox[0], sizeBarBBox[1], sizeBarBBox[2]);
  
  // compute new scale
  double newScaleX, newScaleY, newScaleZ;
  newScaleX = newScaleY = newScaleZ = 1.0;

  if(sizeContentBBox[0] > 1)
    newScaleX = 1.0 / sizeContentBBox[0];

  if(sizeContentBBox[1] > 1)
    newScaleY = 1.0 / sizeContentBBox[1];

  if(sizeContentBBox[2] > 1)
    newScaleZ = 1.0 / sizeContentBBox[2];

  newScaleX *= usedSize;

  double finalScaleX = newScaleX;
  double finalScaleY = newScaleY;
  double finalScaleZ = newScaleZ;

  if(newScaleX < 1 || newScaleY < 1 || newScaleZ < 1)
    {
      if(newScaleY < newScaleX)
	finalScaleX = finalScaleY = newScaleY;
      else
	finalScaleX = finalScaleY = newScaleX;
    }

  // compute new translation
  SbVec3f centerBarBBox = barBBox.getCenter();
  SbVec3f centerContentBBox = contentBBox.getCenter();
  double distCentersX = centerBarBBox[0] - centerContentBBox[0] * finalScaleX  + usedTotalSize/statusBarSize[0] +( usedSize/statusBarSize[0])/2.0;
  double distCentersY = centerBarBBox[1] - centerContentBBox[1] * finalScaleY;
  double distCentersZ = centerBarBBox[2] - centerContentBBox[2] * finalScaleZ;

  /*  
      cout << "*****************STATUS BAR********************" << endl;
      cout << "newScale(x,y,z) = (" << newScaleX << ", " << newScaleY << ", " << newScaleZ << ")" << endl;
      cout << "finalScale(x,y,z) = (" << finalScaleX << ", " << finalScaleY << ", " << finalScaleZ << ")" << endl;
      cout << "sizeBarBBox(x,y,z) = (" << sizeBarBBox[0] << ", " << sizeBarBBox[1] << ", " << sizeBarBBox[2] << ")" << endl;
      cout << "sizeContentBBox(x,y,z) = (" << sizeContentBBox[0] << ", " << sizeContentBBox[1] << ", " << sizeContentBBox[2] << ")" << endl;
      cout << "***********************************************" << endl;  
  */

  SoTransform* contentTransf = new SoTransform();
  contentTransf->scaleFactor.setValue(finalScaleX, finalScaleY, finalScaleZ);
  contentTransf->translation.setValue(distCentersX, distCentersY, distCentersZ);
  
  SoSeparator* newSep = new SoSeparator();
  newSep->addChild(contentTransf);
  newSep->addChild(updatedElementsContent);

  SoSwitch* newSwitch = new SoSwitch();
  newSwitch->whichChild.setValue(SO_SWITCH_ALL);
  newSwitch->addChild(newSep);

  SO_GET_ANY_PART(this, "barElements", SoSwitch)->replaceChild(updatedElementsID, newSwitch);
}
