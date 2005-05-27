#include <cassert>
#include <iostream>

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoAsciiText.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>
#include <stbapi/interaction/SoButtonReleaseGesture.h>

#include <stbapi/vc/SoVCWidgetKit.h>

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoVCWidgetKit);

void 
SoVCWidgetKit::initClass()
{
  SO_BEHAVIOR_INIT_CLASS(SoVCWidgetKit, SoBehaviorKit, "SoBehaviorKit");
}

SoVCWidgetKit::SoVCWidgetKit()
  :widgetParent(NULL)
{
  SO_BEHAVIOR_CONSTRUCTOR(SoVCWidgetKit);

  ADD(widgetContent,          SoSwitch,       FALSE, base, geometry,    FALSE);  // TODO: change this to some switch
  ADD(widgetTransform,        SoTransform,    FALSE, base,          widgetContent,     FALSE);      
  ADD(invisibleDraw,          SoDrawStyle,    FALSE, base,          childWidgets,   FALSE);
  ADD(onGeometry,             SoSeparator,    FALSE, geometrySwitch, \x0,    TRUE);
  ADD(offGeometry,            SoSeparator,    FALSE, geometrySwitch, \x0,    TRUE);
  ADD(highlightedOnGeometry,  SoSeparator,    FALSE, geometrySwitch, \x0,    TRUE);
  ADD(highlightedOffGeometry, SoSeparator,    FALSE, geometrySwitch, \x0,    TRUE);
  ADD(childWidgets,           SoSwitch,       FALSE, base,           widgetTransform,    FALSE); 

  SO_BEHAVIOR_ADD_FIELD(actionStartGestures, (new SoButtonPressGesture())); 
  SO_BEHAVIOR_ADD_FIELD(actionEndGestures, (new SoButtonReleaseGesture()));
  SO_BEHAVIOR_ADD_FIELD(actionEnterGestures, (new SoEnterGesture()));
  SO_BEHAVIOR_ADD_FIELD(actionExitGestures, (new SoExitGesture()));
  SO_BEHAVIOR_ADD_FIELD(actionMoveGestures, (new SoMoveGesture()));
  
  SO_BEHAVIOR_ADD_FIELD(on, (false) );
  SO_BEHAVIOR_ADD_FIELD(highlighted, (false) );
  SO_BEHAVIOR_ADD_FIELD(noGeometry, (false) );

  SO_BEHAVIOR_ADD_FIELD(widgetID, (1));
  SO_BEHAVIOR_ADD_FIELD(widgetName, ("default"));
  SO_BEHAVIOR_ADD_FIELD(scaleContent, (true));
  SO_BEHAVIOR_ADD_FIELD(hideWhenLeaf, (true)); 

  SO_BEHAVIOR_INIT_INSTANCE();
  
  SO_GET_ANY_PART(this, "widgetContent", SoSwitch)->whichChild = -3;
  SO_GET_ANY_PART(this, "childWidgets", SoSwitch)->whichChild = -3;

  setSearchingChildren(TRUE);

  animating = false;

  activeWidgetChildrenChanged = false;
  highlighted = false;
  on = false;
  noGeometry = false;
  adaptGeometryToState();

  inspector = new StbEventInspector(this);

  pressCBList = new SbPList();
  releaseCBList = new SbPList();
	
  onSensor.setFunction( SoVCWidgetKit::stateChangedCB );
  onSensor.setData( this );
  onSensor.attach( &on );
  onSensor.setPriority( 1 );
  hlSensor.setFunction( SoVCWidgetKit::stateChangedCB );
  hlSensor.setData( this );
  hlSensor.attach( &highlighted );
  hlSensor.setPriority( 1 );
  noGeomSensor.setFunction( SoVCWidgetKit::stateChangedCB );
  noGeomSensor.setData( this );
  noGeomSensor.attach( &noGeometry );
  noGeomSensor.setPriority( 1 );

  activeWidgetChildrenSensor.setFunction( SoVCWidgetKit::sensorActiveWidgetChildrenCB );
  activeWidgetChildrenSensor.setData( this );
  activeWidgetChildrenSensor.attach( &activeWidgetChildrenChanged );
  activeWidgetChildrenSensor.setPriority( 1 );


  rescale = true;
  activeWidgetChildrenId = -1;

  SoDrawStyle *invDraw = new SoDrawStyle();
  invDraw->style = SoDrawStyle::FILLED;
  setPart("invisibleDraw", invDraw);  
}

void
SoVCWidgetKit::showChildren(bool state)
{
  if(state)
    SO_GET_ANY_PART(this, "childWidgets", SoSwitch)->whichChild = -3;
  else
    SO_GET_ANY_PART(this, "childWidgets", SoSwitch)->whichChild = -1;
}

void
SoVCWidgetKit::showContent(bool state)
{
  if(state)
    SO_GET_ANY_PART(this, "widgetContent", SoSwitch)->whichChild = -3;
  else
    SO_GET_ANY_PART(this, "widgetContent", SoSwitch)->whichChild = -1;
}

void
SoVCWidgetKit::setWidgetID(int id)
{
  widgetID.setValue(id);
}

void
SoVCWidgetKit::setWidgetName(const char* name)
{
  widgetName.setValue(name);
}

void
SoVCWidgetKit::setContent(SoSeparator* content)
{
  SO_GET_ANY_PART(this, "widgetContent", SoSwitch)->removeAllChildren();;

  if(content!=NULL)
    SO_GET_ANY_PART(this, "widgetContent", SoSwitch)->addChild(content);

}

void
SoVCWidgetKit::autoScaleContent()
{
}


void
SoVCWidgetKit::setContent(const char* description, SbVec3f textColor)
{
  SO_GET_ANY_PART(this, "widgetContent", SoSwitch)->removeAllChildren();;

  SoSeparator *textRoot = new SoSeparator();
  SoSeparator *textContent = new SoSeparator();

  SoMaterial *textMat = new SoMaterial();
  textMat->diffuseColor = textColor;
  
  SoText3 *text = new SoText3();
  text->justification.setValue(SoText3::CENTER);
  SbString valueString;
  text->string.setValue(description);
  SoFont *textFont = new SoFont();
  textFont->size.setValue(0.2);
  textFont->name.setValue("Arial:Bold");
  SoRotationXYZ *textRot = new SoRotationXYZ();
  textRot->angle.setValue(-M_PI/2);
  textRot->axis = SoRotationXYZ::X;
 
  textContent->addChild(textRot);
  textContent->addChild(textMat);
  textContent->addChild(textFont);
  textContent->addChild(text);

  textContent->ref();

  SbViewportRegion vp;
  SoGetBoundingBoxAction bba(vp);

  bba.apply(textContent);
  SbBox3f bboxUSC = bba.getXfBoundingBox().project();
  
  SbVec3f sizeFullUSC;
  bboxUSC.getSize(sizeFullUSC[0], sizeFullUSC[1], sizeFullUSC[2]);
  
  textContent->unrefNoDelete();
  
  bba.apply(this);
  SbBox3f bbox = bba.getXfBoundingBox().project();
  
  SbVec3f sizeFull;
  bbox.getSize(sizeFull[0], sizeFull[1], sizeFull[2]);
  
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
  contTransf->translation = SbVec3f(0, 0, 0.2);//0.15);
  
  textRoot->addChild(contTransf);
  textRoot->addChild(textContent);

  SO_GET_ANY_PART(this, "widgetContent", SoSwitch)->addChild(textRoot);
}

SoVCWidgetKit::~SoVCWidgetKit()
{
}

void 
SoVCWidgetKit::handle3DEvent(SoHandle3DEventAction *h3a)
{
}

void
SoVCWidgetKit::adaptGeometryToState()
{
  if (noGeometry.getValue())
    {
       SO_GET_ANY_PART(this, "invisibleDraw", SoDrawStyle)->style = SoDrawStyle::INVISIBLE;
    }
  else
    {
      SO_GET_ANY_PART(this, "invisibleDraw", SoDrawStyle)->style = SoDrawStyle::FILLED;

      if (on.getValue())
	if (highlighted.getValue()) 
	  getGeometrySwitch()->whichChild = HIGHLIGHTED_ON;
	else 
	  getGeometrySwitch()->whichChild = ON;
      else
	if (highlighted.getValue()) 
	  {
	    getGeometrySwitch()->whichChild = HIGHLIGHTED_OFF;
	  }
	else 
	  getGeometrySwitch()->whichChild = OFF;      
    }

  if(scaleContent.getValue())
    autoScaleContent(); 
}

void 
SoVCWidgetKit::useAutoScale(bool use)
{
  if(use)
    scaleContent.set("TRUE");
  else
    scaleContent.set("FALSE");
}


void 
SoVCWidgetKit::turnOff()
{
  on = false;
  if(releaseCallback) releaseCallback(releaseCallbackData, this);
}

void 
SoVCWidgetKit::addPressCallback(SoVCWidgetKitCB* cb, void* userData)
{
  pressCBList->append((void*)cb);
  pressCBList->append(userData);
}

void 
SoVCWidgetKit::addReleaseCallback(SoVCWidgetKitCB* cb, void* userData)
{
  releaseCBList->append((void*)cb);
  releaseCBList->append(userData);
}

bool 
SoVCWidgetKit::removePressCallback(SoVCWidgetKitCB* cb)
{
  int i;	

  i = pressCBList->find((void*)cb);       
  if (i==-1) 
    return FALSE;
	
  pressCBList->remove(i);
  pressCBList->remove(i);
  return TRUE;
}

bool 
SoVCWidgetKit::removeReleaseCallback(SoVCWidgetKitCB* cb)
{
  int i;	

  i = releaseCBList->find((void*)cb);       
  if (i==-1) 
    return FALSE;
	
  releaseCBList->remove(i);
  releaseCBList->remove(i);
  return TRUE;
}

void 
SoVCWidgetKit::stateChangedCB( void * userdata, SoSensor * sensor )
{
  assert( userdata != NULL );
  SoVCWidgetKit * self = (SoVCWidgetKit *) userdata;
  self->adaptGeometryToState();
}

int
SoVCWidgetKit::getWidgetID()
{
  return widgetID.getValue();
}

void
SoVCWidgetKit::setTranslation(SbVec3f translation)
{
  float x,y,z;
  translation.getValue(x,y,z);
  SO_GET_ANY_PART(this, "widgetTransform", SoTransform)->translation.setValue(x,y,z);
}

SoTransform*
SoVCWidgetKit::getTransform()
{
  return SO_GET_ANY_PART(this,"widgetTransform", SoTransform);
}

SoSwitch*
SoVCWidgetKit::getContent()
{
  return SO_GET_ANY_PART(this, "widgetContent", SoSwitch);
}

SoSwitch*
SoVCWidgetKit::getWidgetChildren()
{
  return SO_GET_ANY_PART(this, "childWidgets", SoSwitch);
}

void
SoVCWidgetKit::setActiveWidgetChildren(int switchId)
{
  activeWidgetChildrenId = switchId;
  activeWidgetChildrenChanged = true;
}

void 
SoVCWidgetKit::sensorActiveWidgetChildrenCB( void * userdata, SoSensor * sensor )
{
  assert( userdata != NULL );
  SoVCWidgetKit * self = (SoVCWidgetKit *) userdata;
  self->changeActiveWidgetChildren();
}

void 
SoVCWidgetKit::changeActiveWidgetChildren()
{
  SO_GET_ANY_PART(this, "childWidgets", SoSwitch)->whichChild = activeWidgetChildrenId;  
}

void
SoVCWidgetKit::setWidgetParent(SoVCWidgetKit* parent)
{
   SoVCWidgetKit::widgetParent = parent;
}

SoSFString
SoVCWidgetKit::getWidgetName()
{
  return widgetName;
}

SoVCWidgetKit*
SoVCWidgetKit::getWidgetParent()
{
  return widgetParent;
}

void
SoVCWidgetKit::setVisibility(bool status)
{
  if(status)
    {
      noGeometry = false;
    }
  else
    {
      noGeometry = true;
    }
}

void
SoVCWidgetKit::setAnimationState(bool on)
{
  if(on)
    { 
      animating = true;
      on = false;
      highlighted = false;
    }
  else
    {
      animating = false;
      on = false;
      highlighted = false;
    }
}

bool
SoVCWidgetKit::isHideWhenLeaf()
{
  return hideWhenLeaf.getValue();
}
