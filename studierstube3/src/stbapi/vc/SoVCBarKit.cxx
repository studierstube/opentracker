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

#include <stbapi/vc/SoVCBarKit.h>
#include <stbapi/vc/SoVCWidgetKit.h>
#include <stbapi/vc/SoVCPushWidgetKit.h>
#include <stbapi/vc/SoVCDialWidgetKit.h>

#include <Inventor/nodes/SoTransparencyType.h>
#include <Inventor/actions/SoGLRenderAction.h>

#ifndef min
#define min(a,b) (a<b)?a:b
#endif
#ifndef max
#define max(a,b) (a>b)?a:b
#endif

#define ADD SO_KIT_ADD_CATALOG_ENTRY

SO_KIT_SOURCE(SoVCBarKit);

void
SoVCBarKit::initClass()
{
    SO_KIT_INIT_CLASS(SoVCBarKit, SoBaseKit, "BaseKit");
}

SoVCBarKit::SoVCBarKit(int maxElements, StbSkinLoader *skin, char *prefix)
  : skin(skin), prefix(prefix), maxElements(maxElements)
{
    SO_KIT_CONSTRUCTOR(SoVCBarKit);

    ADD(barRoot,        SoSeparator,         FALSE,   this,        \0,   FALSE);
    ADD(barTransp,      SoTransparencyType,  FALSE,   barRoot,     \0,   FALSE);
    ADD(barTransform,   SoTransform,         TRUE,    barRoot,     \0,   FALSE);
    ADD(barSwitch,      SoSwitch,            FALSE,   barRoot,     \0,   FALSE);

    SO_KIT_ADD_CATALOG_LIST_ENTRY(barWidgets,     SoSeparator,   FALSE,    barSwitch,   \0,  SoVCWidgetKit,  FALSE);

    SO_KIT_ADD_FIELD(removeElement, (false) );

    SO_KIT_INIT_INSTANCE();

    SO_GET_ANY_PART(this, "barTransp", SoTransparencyType)->value = SoGLRenderAction::SORTED_OBJECT_BLEND;

    removeElement = false;

    elementToRemove = NULL;

    removeElemSensor.setFunction( SoVCBarKit::stateChangedCB );
    removeElemSensor.setData( this );
    removeElemSensor.attach( &removeElement );
    removeElemSensor.setPriority( 1 );

    SO_GET_ANY_PART(this,"barSwitch",SoSwitch)->whichChild = 0;

    setSearchingChildren(TRUE);
}


SoVCBarKit::~SoVCBarKit()
{
  Stb3DEventGenerator::releaseAllGrabbers(this);
}


void
SoVCBarKit::removeBarElementCB(void *userData, SoVCWidgetKit* widgetKit)
{
  SoVCBarKit *barKit = (SoVCBarKit*)userData;
  
  if(widgetKit != NULL)
    {
      barKit->getWidgets()->removeChild(widgetKit);    
    }
}


void 
SoVCBarKit::setElementToRemove(SoVCWidgetKit *widgetKit)
{
  elementToRemove = widgetKit;
}

void
SoVCBarKit::addBarElement(int widgetID, const char* widgetName, SoSeparator* content, SoVCWidgetKitCB* cbFunc, void* cbData, SbVec3f numSpacers, bool scaleContent)
{
  SoVCPushWidgetKit* pwk = new SoVCPushWidgetKit();
  
  pwk->useAutoScale(scaleContent);

  pwk->setWidgetID(widgetID);

  pwk->addReleaseCallback(cbFunc, cbData);
  pwk->setWidgetName(widgetName);
  pwk->setContent(content);

  // setup transform for widget
  PUAGETNODE(refWidgetTranslation_, SoTranslation, prefix, "SPACING", skin);
  float tx,ty,tz;
  refWidgetTranslation_->translation.getValue().getValue(tx,ty,tz);

  float transX, transY, transZ;
  transX = transY = transZ = 0;
  float factor = getWidgets()->getNumChildren();
  if(tx > 0)
    transX = factor + factor*tx;
  if(ty > 0)
    transY = factor + factor*ty;
  if(tz > 0)
    transZ = factor + factor*tz;
  
  pwk->setTranslation(SbVec3f(numSpacers[0]+transX, numSpacers[1]+transY, numSpacers[2]+transZ)); // switched y,z cause of rotation

  getWidgets()->addChild(pwk);
}

SoNodeKitListPart *
SoVCBarKit::getWidgets()
{
  return SO_GET_ANY_PART(this, "barWidgets", SoNodeKitListPart);
}

void
SoVCBarKit::showElements(bool show)
{
  if(show)
    {
      SO_GET_ANY_PART(this, "barSwitch", SoSwitch)->whichChild = -3; // show
    }
  else
    {
      SO_GET_ANY_PART(this, "barSwitch", SoSwitch)->whichChild = -1; // don't show
    }
}

void
SoVCBarKit::setupTransform(SbVec3f start, SbVec3f barSize, SbVec3f vcSize) 
{
  if (barSize[0] == 0.0 || barSize[1] == 0.0 || barSize[2] == 0.0)
    return;

  float tX, tY, tZ;
  PUAGETNODE(refWidgetTranslation_, SoTranslation, prefix, "SPACING", skin);
  refWidgetTranslation_->translation.getValue().getValue(tX, tY, tZ); 

  float radius;
  PUAGETNODE(refWidgetDimension_, SoInfo, prefix, "WIDGET_RADIUS_SCALE", skin);
  sscanf(refWidgetDimension_->string.getValue().getString(),"%f", &radius);

  float sfMin = min(min(barSize[0], barSize[1]), barSize[2]);
  SbVec3f sizeContentBBox;
  if(tX > 0)
    {
      sizeContentBBox[0] = (maxElements + ((float)maxElements-1.0)*tX) * radius;
      sizeContentBBox[1] = radius;
      sizeContentBBox[2] = radius;
    }
  else if(tY > 0)
    {
      sizeContentBBox[0] = radius;
      sizeContentBBox[1] = (maxElements + ((float)maxElements-1.0)*tY) * radius;
      sizeContentBBox[2] = radius;
    }
  else if(tZ > 0)
    {
      sizeContentBBox[0] = radius;
      sizeContentBBox[1] = radius;
      sizeContentBBox[2] = (maxElements + ((float)maxElements-1.0)*tZ ) * radius;
    }

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
  if(sizeContentBBox[0]*finalScaleX > barSize[0])
    tmpScaleX = (barSize[0] / (sizeContentBBox[0]*finalScaleX))*finalScaleX;
  float tmpScaleY = finalScaleY;
  if(sizeContentBBox[1]*finalScaleY > barSize[1])
    tmpScaleY = (barSize[1] / (sizeContentBBox[1]*finalScaleY))*finalScaleY;
  float tmpScaleZ = finalScaleZ;
  if(sizeContentBBox[2]*finalScaleZ > barSize[2])
    tmpScaleZ = (barSize[2] / (sizeContentBBox[2]*finalScaleZ))*finalScaleZ;

  float finalScale = min(min(tmpScaleX, tmpScaleY), tmpScaleZ);
  
  // set transform node for bar
  SO_GET_ANY_PART(this, "barTransform", SoTransform)->scaleFactor.setValue(finalScale, finalScale, finalScale);
  SO_GET_ANY_PART(this, "barTransform", SoTransform)->rotation.setValue(SbRotation(SbVec3f(1,0,0),M_PI/2.0));  
  SO_GET_ANY_PART(this, "barTransform", SoTransform)->translation.setValue(-vcSize[0]/2.0 + start[0] + (radius*finalScale)/2.0,
									   vcSize[1]/2.0 - start[1]  - (radius*finalScale)/2.0,
									   0);

}

SoVCWidgetKit*
SoVCBarKit::getElement(int widgetID)
{
  for (int i=0; i<getWidgets()->getNumChildren(); i++)
    if (((SoVCWidgetKit*)getWidgets()->getChild(i))->getWidgetID() == widgetID) 
      return (SoVCWidgetKit*)getWidgets()->getChild(i);
  return NULL;
}

SoVCWidgetKit*
SoVCBarKit::getElement(const char* widgetName)
{
  for (int i=0; i<getWidgets()->getNumChildren(); i++)
    if (((SoVCWidgetKit*)getWidgets()->getChild(i))->getWidgetName().getValue() == SbString(widgetName)) 
      return (SoVCWidgetKit*)getWidgets()->getChild(i);
  return NULL;
}

int
SoVCBarKit::getNumElements()
{
  return getWidgets()->getNumChildren();
}

void 
SoVCBarKit::stateChangedCB( void * userdata, SoSensor * sensor )
{
  assert( userdata != NULL );
  SoVCBarKit * self = (SoVCBarKit *) userdata;
  self->adaptBarToState();
}


void
SoVCBarKit::adaptBarToState()
{
  if(removeElement.getValue())
    {
      if(elementToRemove != NULL)
	{
	  getWidgets()->removeChild(elementToRemove);    
	}
    }
}
