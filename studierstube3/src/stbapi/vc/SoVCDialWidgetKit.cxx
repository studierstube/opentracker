#include <iostream>

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/misc/SoChildList.h>
#include <stbapi/util/ivio.h>

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>

#include <stbapi/vc/SoVCDialWidgetKit.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>
#include <stbapi/interaction/SoButtonReleaseGesture.h>
#include <stbapi/misc/SoConstraint.h>

#include <Inventor/SbBox.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SoPath.h>

#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText3.h>

#include <stbapi/vc/dialwidgetGeometry/onString.h>
#include <stbapi/vc/dialwidgetGeometry/offString.h>
#include <stbapi/vc/dialwidgetGeometry/highlightedOnString.h>
#include <stbapi/vc/dialwidgetGeometry/highlightedOffString.h>

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoVCDialWidgetKit)

void SoVCDialWidgetKit::initClass()
{
  SO_BEHAVIOR_INIT_CLASS(SoVCDialWidgetKit, SoVCDraggerKit, "SoVCDraggerKit");
}

// callback, that reports, whenever a value has changed
static void 
valueChangedCB(void *data, SoSensor *)
{
}


SoVCDialWidgetKit::SoVCDialWidgetKit()
{
  SO_BEHAVIOR_CONSTRUCTOR(SoVCDialWidgetKit);

  ADD(dialValueContent,          SoSwitch,       FALSE, base,    geometry,    TRUE);
  
  SO_BEHAVIOR_ADD_FIELD(minValue, (-10.0) ); 
  SO_BEHAVIOR_ADD_FIELD(maxValue, (30.0) ); 
  SO_BEHAVIOR_ADD_FIELD(axis, (0, 1, 0) );
  SO_BEHAVIOR_ADD_FIELD(startAngle, (0.0) );
  SO_BEHAVIOR_ADD_FIELD(endAngle, (4*M_PI) );
  SO_BEHAVIOR_ADD_FIELD(value, (0.0) ); 

  SO_BEHAVIOR_INIT_INSTANCE();

  // setting up the connection between the value and the
  // valueChanged callback
  valueChangedSensor = new SoFieldSensor(valueChangedCB, &value);
  valueChangedSensor->attach(&value);
  currentAngle = startAngle.getValue();

  // set the default geometry
  setPart("onGeometry",             getGeometryFromString(widgetOnString) );
  setPart("offGeometry",            getGeometryFromString(widgetOffString) );
  setPart("highlightedOnGeometry",  getGeometryFromString(widgetHighlightedOnString) );
  setPart("highlightedOffGeometry", getGeometryFromString(widgetHighlightedOffString) );

  animating = false;
  on = false;
  highlighted = false;
  hideWhenLeaf = false;

  getGeometrySwitch()->whichChild = OFF;
  SO_GET_ANY_PART(this, "dialValueContent", SoSwitch)->whichChild = SO_SWITCH_NONE;
}

SoVCDialWidgetKit::~SoVCDialWidgetKit()
{
}


SoTransform *
SoVCDialWidgetKit::getTransform()
{
  return constrainedTransform;
}

void 
SoVCDialWidgetKit::handle3DEvent(SoHandle3DEventAction *h3a)
{
  actionStartGestures = new SoButtonPressGesture(dragButton.getValue());
  actionEndGestures = new SoButtonReleaseGesture(dragButton.getValue());

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

  if(on.getValue() == false && detectAction(actionStartGestures))
    {     
      on = true;
      inspector->setGrabber(h3a);
      if(startCallback) startCallback(startCallbackData, this);

      useAutoScale(false);
      getContent()->whichChild = SO_SWITCH_NONE;
      autoScaleValueContent();
      SO_GET_ANY_PART(this, "dialValueContent", SoSwitch)->whichChild = SO_SWITCH_ALL;
    }
  else if(on.getValue() == true && detectAction(actionEndGestures))
    {     
      if (!isInside(h3a)) highlighted = false;

      on = false;
      inspector->releaseGrabber(h3a);
      
      // fire the finishCallback
      if(finishCallback) finishCallback(finishCallbackData, this);

      SO_GET_ANY_PART(this, "dialValueContent", SoSwitch)->whichChild = SO_SWITCH_NONE;
      useAutoScale(true);
      getContent()->whichChild = SO_SWITCH_ALL;
    }
  else if(on.getValue() && detectAction(actionMoveGestures))
    {
      SbMatrix widgetTransfMat;
      SoTransform *widgetTransf = SO_GET_ANY_PART(this, "widgetTransform", SoTransform);
      widgetTransfMat.setTransform(widgetTransf->translation.getValue(),
				   widgetTransf->rotation.getValue(),
				   widgetTransf->scaleFactor.getValue(),
				   widgetTransf->scaleOrientation.getValue(),
				   widgetTransf->center.getValue());
      currentAngle += inspector->getAxisRotationAngle(axis.getValue(), widgetTransfMat );

      if (currentAngle < startAngle.getValue()) currentAngle = startAngle.getValue();
      else if (currentAngle > endAngle.getValue()) currentAngle = endAngle.getValue();

      // apply it to the current rotation
      getTransform()->rotation.setValue( SbRotation(-axis.getValue(), currentAngle) );
      
      // apply rotation do geometry
      SbString searchName = "indicatorTransform";
      SoTransform *indicatorTransform = (SoTransform*)findNode(getPart("highlightedOnGeometry", false), searchName);
      indicatorTransform->rotation.setValue( SbRotation(-axis.getValue(), currentAngle));     
      
      indicatorTransform = (SoTransform*)findNode(getPart("highlightedOffGeometry", false), searchName);
      indicatorTransform->rotation.setValue( SbRotation(-axis.getValue(), currentAngle));     

      indicatorTransform = (SoTransform*)findNode(getPart("offGeometry", false), searchName);
      indicatorTransform->rotation.setValue( SbRotation(-axis.getValue(), currentAngle));     

      indicatorTransform = (SoTransform*)findNode(getPart("onGeometry", false), searchName);
      indicatorTransform->rotation.setValue( SbRotation(-axis.getValue(), currentAngle));     

      // reflect the current angle in the dial's value
      float start = startAngle.getValue();
      float end   = endAngle.getValue();
      float minVal = minValue.getValue();
      float maxVal = maxValue.getValue();

      value.setValue( (currentAngle-start)/(end-start) * (maxVal - minVal) + minVal);

      autoScaleValueContent();      

      // fire the move callback
      if(moveCallback) moveCallback(moveCallbackData, this);
    }
}

SbBool 
SoVCDialWidgetKit::isInside(const SoHandle3DEventAction *h3a)
{  
    return inspector->isInside();
}


SbBool 
SoVCDialWidgetKit::isInterested()
{
  return inspector->isInterested();
}

void 
SoVCDialWidgetKit::autoScaleContent()
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

      // reduce size, cause toggleIndicatorSphere is nearly 1/5 of regular sphere
      sizeFull[0] *= 0.55;//0.6,
      sizeFull[1] *= 0.55;//0.6;
      sizeFull[2] *= 0.55;//0.6;

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

void 
SoVCDialWidgetKit::autoScaleValueContent()
{
  SoSeparator *dialValueRoot = new SoSeparator();

  // ---------- CURRENT VALUE -----------------
  SoSeparator *currentValueRoot = new SoSeparator();
  SoSeparator *dialValueContent = new SoSeparator();

  SoMaterial *textMat = new SoMaterial();
  textMat->diffuseColor = SbVec3f(0.0, 0.0, 0.0);
  
  SoText3 *text = new SoText3();
  text->justification.setValue(SoText3::CENTER);
  SbString valueString;
  valueString.sprintf("%+04.1f", value.getValue());
  text->string.setValue(valueString.getString());
  SoFont *textFont = new SoFont();
  textFont->size.setValue(0.2);
  textFont->name.setValue("Arial:Bold");
  SoRotationXYZ *textRot = new SoRotationXYZ();
  textRot->angle.setValue(-M_PI/2);
  textRot->axis = SoRotationXYZ::X;
 
  dialValueContent->addChild(textRot);
  dialValueContent->addChild(textMat);
  dialValueContent->addChild(textFont);
  dialValueContent->addChild(text);

  dialValueContent->ref();

  SbViewportRegion vp;
  SoGetBoundingBoxAction bba(vp);

  bba.apply(dialValueContent);
  SbBox3f bboxUSC = bba.getXfBoundingBox().project();
  
  SbVec3f sizeFullUSC;
  bboxUSC.getSize(sizeFullUSC[0], sizeFullUSC[1], sizeFullUSC[2]);
  
  dialValueContent->unrefNoDelete();
  
  bba.apply(this);
  SbBox3f bbox = bba.getXfBoundingBox().project();
  
  SbVec3f sizeFull;
  bbox.getSize(sizeFull[0], sizeFull[1], sizeFull[2]);
  
  // reduce size, cause toggleIndicatorSphere is nearly 1/5 of regular sphere
  sizeFull[0] *= 0.3;//0.55;
  sizeFull[1] *= 0.3;//0.55;
  sizeFull[2] *= 0.3;//0.55;
  
  // diameter of sphere = a_OuterBox;
  // with diameterSphere = diagInnerBox and diagInnerBox = 3 * a_InnerBox * a_InnerBox we are done
  
  float diameterSphere = sizeFull[0];
  float aInnerBox = sqrt((diameterSphere / 3.0f));
  
  // compute scale factors
  
  float sfX = aInnerBox / sizeFullUSC[0];
  float sfY = aInnerBox / sizeFullUSC[1];
  float sfZ = aInnerBox / sizeFullUSC[2];
  
  SbVec3f bboxCenter = bbox.getCenter();
  SbVec3f uscCenter = bboxUSC.getCenter();

  SoTransform *contTransf = new SoTransform();
  contTransf->scaleFactor = SbVec3f(sfX, sfX, sfZ);
  contTransf->translation = SbVec3f(0, 0, 0.1);
  currentValueRoot->addChild(contTransf);
  currentValueRoot->addChild(dialValueContent);
  // --------------------------------------


  // ---------- MAX VALUE -----------------
  SoSeparator *maxValueRoot = new SoSeparator();
  SoSeparator *maxValueContent = new SoSeparator();

  SoMaterial *maxTextMat = new SoMaterial();
  maxTextMat->diffuseColor = SbVec3f(0.24, 0.0, 0.0);
  
  SoText3 *maxText = new SoText3();
  maxText->justification.setValue(SoText3::CENTER);
  SbString maxValueString;
  maxValueString.sprintf("%+04.1f", maxValue.getValue());
  maxText->string.setValue(maxValueString.getString());
  SoFont *maxTextFont = new SoFont();
  maxTextFont->size.setValue(0.2);
  maxTextFont->name.setValue("Arial:Bold");
  SoRotationXYZ *maxTextRot = new SoRotationXYZ();
  maxTextRot->angle.setValue(-M_PI/2);
  maxTextRot->axis = SoRotationXYZ::X;
 
  maxValueContent->addChild(maxTextRot);
  maxValueContent->addChild(maxTextMat);
  maxValueContent->addChild(maxTextFont);
  maxValueContent->addChild(maxText);

  maxValueContent->ref();

  bba.apply(maxValueContent);
  bboxUSC = bba.getXfBoundingBox().project();
  
  bboxUSC.getSize(sizeFullUSC[0], sizeFullUSC[1], sizeFullUSC[2]);
  
  maxValueContent->unrefNoDelete();
  
  bba.apply(this);
  bbox = bba.getXfBoundingBox().project();
  
  bbox.getSize(sizeFull[0], sizeFull[1], sizeFull[2]);
  
  // reduce size, cause toggleIndicatorSphere is nearly 1/5 of regular sphere
  sizeFull[0] *= 0.1;
  sizeFull[1] *= 0.1;
  sizeFull[2] *= 0.1;
  
  // diameter of sphere = a_OuterBox;
  // with diameterSphere = diagInnerBox and diagInnerBox = 3 * a_InnerBox * a_InnerBox we are done
  
  diameterSphere = sizeFull[0];
  aInnerBox = sqrt((diameterSphere / 3.0f));
  
  // compute scale factors
  
  sfX = aInnerBox / sizeFullUSC[0];
  sfY = aInnerBox / sizeFullUSC[1];
  sfZ = aInnerBox / sizeFullUSC[2];
  
  bboxCenter = bbox.getCenter();
  uscCenter = bboxUSC.getCenter();

  SoTransform *maxContTransf = new SoTransform();
  maxContTransf->scaleFactor = SbVec3f(sfX, sfX, sfZ);
  maxContTransf->translation = SbVec3f(0, 0, -0.17);
  maxValueRoot->addChild(maxContTransf);
  maxValueRoot->addChild(maxValueContent);

  // --------------------------------------


  // ---------- MIN VALUE -----------------
  SoSeparator *minValueRoot = new SoSeparator();
  SoSeparator *minValueContent = new SoSeparator();

  SoMaterial *minTextMat = new SoMaterial();
  minTextMat->diffuseColor = SbVec3f(0.0, 0.24, 0.0);
  
  SoText3 *minText = new SoText3();
  minText->justification.setValue(SoText3::CENTER);
  SbString minValueString;
  minValueString.sprintf("%+04.1f", minValue.getValue());
  minText->string.setValue(minValueString.getString());
  SoFont *minTextFont = new SoFont();
  minTextFont->size.setValue(0.2);
  minTextFont->name.setValue("Arial:Bold");
  SoRotationXYZ *minTextRot = new SoRotationXYZ();
  minTextRot->angle.setValue(-M_PI/2);
  minTextRot->axis = SoRotationXYZ::X;
 
  minValueContent->addChild(minTextRot);
  minValueContent->addChild(minTextMat);
  minValueContent->addChild(minTextFont);
  minValueContent->addChild(minText);

  minValueContent->ref();

  bba.apply(minValueContent);
  bboxUSC = bba.getXfBoundingBox().project();
  
  bboxUSC.getSize(sizeFullUSC[0], sizeFullUSC[1], sizeFullUSC[2]);
  
  minValueContent->unrefNoDelete();
  
  bba.apply(this);
  bbox = bba.getXfBoundingBox().project();
  
  bbox.getSize(sizeFull[0], sizeFull[1], sizeFull[2]);
  
  // reduce size, cause toggleIndicatorSphere is nearly 1/5 of regular sphere
  sizeFull[0] *= 0.1;
  sizeFull[1] *= 0.1;
  sizeFull[2] *= 0.1;
  
  // diameter of sphere = a_OuterBox;
  // with diameterSphere = diagInnerBox and diagInnerBox = 3 * a_InnerBox * a_InnerBox we are done
  
  diameterSphere = sizeFull[0];
  aInnerBox = sqrt((diameterSphere / 3.0f));
  
  // compute scale factors
  
  sfX = aInnerBox / sizeFullUSC[0];
  sfY = aInnerBox / sizeFullUSC[1];
  sfZ = aInnerBox / sizeFullUSC[2];
  
  bboxCenter = bbox.getCenter();
  uscCenter = bboxUSC.getCenter();

  SoTransform *minContTransf = new SoTransform();
  minContTransf->scaleFactor = SbVec3f(sfX, sfX, sfZ);
  minContTransf->translation = SbVec3f(0, 0, 0.27);
  minValueRoot->addChild(minContTransf);
  minValueRoot->addChild(minValueContent);

  // --------------------------------------


  dialValueRoot->addChild(maxValueRoot);
  dialValueRoot->addChild(currentValueRoot);
  dialValueRoot->addChild(minValueRoot);
  
  SO_GET_ANY_PART(this, "dialValueContent", SoSwitch)->removeAllChildren();
  SO_GET_ANY_PART(this, "dialValueContent", SoSwitch)->addChild(dialValueRoot);        
}

float
SoVCDialWidgetKit::getAngle()
{
  return currentAngle;
}

float
SoVCDialWidgetKit::getValue()
{
  return value.getValue();
}

void 
SoVCDialWidgetKit::addValueCallback(SoVCWidgetKitCB* cb, void* userData)
{
  valueCBList->append((void*)cb);
  valueCBList->append(userData);
}

bool 
SoVCDialWidgetKit::removeValueCallback(SoVCWidgetKitCB* cb)
{
  int i;	

  i = valueCBList->find((void*)cb);       
  if (i==-1) 
    return FALSE;
	
  valueCBList->remove(i);
  valueCBList->remove(i);
  return TRUE;
}
