#include <iostream>

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/misc/SoChildList.h>

#include <stbapi/vc/SoVCPushWidgetKit.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>

#include <Inventor/SbBox.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SoPath.h>

#include <stbapi/vc/pushwidgetGeometry/onString.h>
#include <stbapi/vc/pushwidgetGeometry/offString.h>
#include <stbapi/vc/pushwidgetGeometry/highlightedOnString.h>
#include <stbapi/vc/pushwidgetGeometry/highlightedOffString.h>

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoVCPushWidgetKit);

void 
SoVCPushWidgetKit::initClass()
{
  SO_BEHAVIOR_INIT_CLASS(SoVCPushWidgetKit, SoVCWidgetKit, "SoVCWidgetKit");
}

SoVCPushWidgetKit::SoVCPushWidgetKit()
{
  SO_BEHAVIOR_CONSTRUCTOR(SoVCPushWidgetKit);
  
  SO_BEHAVIOR_INIT_INSTANCE();

  // set the default geometry
  setPart("onGeometry",             getGeometryFromString(widgetOnString) );
  setPart("offGeometry",            getGeometryFromString(widgetOffString) );
  setPart("highlightedOnGeometry",  getGeometryFromString(widgetHighlightedOnString) );
  setPart("highlightedOffGeometry", getGeometryFromString(widgetHighlightedOffString) );

  animating = false;

  on = false;
  highlighted = false;

  getGeometrySwitch()->whichChild = OFF;
}

SoVCPushWidgetKit::~SoVCPushWidgetKit()
{
}

void 
SoVCPushWidgetKit::handle3DEvent(SoHandle3DEventAction *h3a)
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
	  inspector->releaseGrabber(h3a);
	}
    }
}

SbBool SoVCPushWidgetKit::isInside(const SoHandle3DEventAction *h3a)
{
  return inspector->isInside();
}


SbBool SoVCPushWidgetKit::isInterested()
{
  return inspector->isInterested();
}


void SoVCPushWidgetKit::autoScaleContent()
{

  if(!rescale)
    return;

  SoNode *unscaledContent = NULL;

  if(SO_GET_ANY_PART(this, "widgetContent", SoSwitch)->getNumChildren() > 0)
    unscaledContent = (SoNode*)SO_GET_ANY_PART(this, "widgetContent", SoSwitch)->getChild(0);

  if(unscaledContent != NULL)
    {
      unscaledContent->ref();

      SbViewportRegion vp;
      SoGetBoundingBoxAction bba(vp);

      bba.apply(unscaledContent);
      SbBox3f bboxUSC = bba.getXfBoundingBox().project();

      SbVec3f sizeFullUSC;
      bboxUSC.getSize(sizeFullUSC[0], sizeFullUSC[1], sizeFullUSC[2]);

      unscaledContent->unrefNoDelete();

      bba.apply(this);
      SbBox3f bbox = bba.getXfBoundingBox().project();

      SbVec3f sizeFull;
      bbox.getSize(sizeFull[0], sizeFull[1], sizeFull[2]);

      sizeFull[0] *= 0.95;
      sizeFull[1] *= 0.95;
      sizeFull[2] *= 0.95;

      // diameter of sphere = a_OuterBox;
      // with diameterSphere = diagInnerBox and diagInnerBox = 3 * a_InnerBox * a_InnerBox we are done

      float diameterSphere = sizeFull[0];
      float aInnerBox = sqrt((diameterSphere / 3.0f));

      // compute scale factors

      float sfX = aInnerBox / sizeFullUSC[0];
      float sfY = aInnerBox / sizeFullUSC[1];
      float sfZ = aInnerBox / sizeFullUSC[2];

      SoTransform *contTransf = new SoTransform();
      contTransf->scaleFactor = SbVec3f(sfX, sfX, sfZ);
      rescale = false;

      ((SoGroup*)unscaledContent)->insertChild(contTransf, 0);
    }        
}
