#include <cassert>
#include <iostream>

#include <Inventor/SbBox.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/misc/SoChildList.h>
#include <stbapi/vc/SoVCDraggerKit.h>
#include <stbapi/interaction/SoButtonPressGesture.h>
#include <stbapi/interaction/SoButtonReleaseGesture.h>

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY 

SO_BEHAVIOR_SOURCE(SoVCDraggerKit);

void 
SoVCDraggerKit::initClass(void)
{
    SO_BEHAVIOR_INIT_CLASS(SoVCDraggerKit, SoVCWidgetKit, "SoVCWidgetKit");
}


SoVCDraggerKit::SoVCDraggerKit()
:
    startCallback(NULL),
    finishCallback(NULL),
    moveCallback(NULL),
    startCallbackData(NULL),
    finishCallbackData(NULL),
    moveCallbackData(NULL)
{
    SO_BEHAVIOR_CONSTRUCTOR(SoVCDraggerKit);
    
    SO_KIT_DEFINE_ENUM_VALUE(DragPoint, PICKPOINT);
    SO_KIT_DEFINE_ENUM_VALUE(DragPoint, ORIGIN);
    SO_KIT_SET_SF_ENUM_TYPE(dragPoint, DragPoint);

    SO_BEHAVIOR_ADD_FIELD(translation, (0,0,0));
    SO_BEHAVIOR_ADD_FIELD(rotation, (0,0,0,1));
    SO_BEHAVIOR_ADD_FIELD(dragButton, (0));
    SO_BEHAVIOR_ADD_FIELD(dragPoint, (ORIGIN));

    SO_BEHAVIOR_INIT_INSTANCE();

    setSearchingChildren(TRUE);

    constrainedTransform = new SoTransform();

    globalToLocal.makeIdentity();   
}

SoVCDraggerKit::~SoVCDraggerKit()
{
}

void SoVCDraggerKit::connectConstraint()
{
  /*

  // create and connect constraint input to translation and rotation
  constraint->translationIn.connectFrom(&translation);
  constraint->rotationIn.connectFrom(&rotation);
  
  // connect constraint output to transform
  //SoTransform* constrainedTrafo = constrainedTransform;
  SoTransform* constrainedTrafo = (SoTransform*)constrainedTransform.getValue();
  constrainedTrafo->translation.connectFrom(&constraint->translationOut);
  constrainedTrafo->rotation.connectFrom(&constraint->rotationOut);

  */
}

void 
SoVCDraggerKit::handle3DEvent(SoHandle3DEventAction* h3a)
{
  /*
  cout << "DRAAAAAAAAAAAAAAGER KIT" << endl;

  SoButtonPressGesture buttonPress(dragButton.getValue());
  SoButtonReleaseGesture buttonRelease(dragButton.getValue());

  if( animating )
    return;

  if ( isInside(h3a))//inspector->detect(&enter) )
    {
      highlighted = true;
    } 
  else
    highlighted = false;

  if (highlighted.getValue() && inspector->detect(&buttonPress))
    {		
      on = true;

      inspector->setGrabber(h3a);
      
      kitInitialTransL = translation.getValue();
      kitInitialRotL   = rotation.getValue();

      penInitialTransG = inspector->getPenTranslation();
      penInitialRotG   = inspector->getPenRotation();
        
      // determine the pickOffset

      if (dragPoint.getValue() == PICKPOINT)
        {
	  SbVec3f penInitialTransL;
	  inspector->getGlobalToLocalMatrix().multVecMatrix(penInitialTransG, penInitialTransL);

	  pickOffsetL = penInitialTransL - kitInitialTransL;
        }
      else if (dragPoint.getValue() == ORIGIN)
        {
	  pickOffsetL.setValue(0,0,0);
        }

      // fire the startCallback
      if(startCallback) startCallback(startCallbackData, this);
    }
  else if(on.getValue() && inspector->detect(&buttonRelease) )
    {      
      if (!isInside(h3a)) highlighted = false;
      on = false;

      inspector->releaseGrabber(h3a);

      // fire the finishCallback
      if(finishCallback) finishCallback(finishCallbackData, this);
    }
  else if(on.getValue() && detectAction(actionMoveGestures))//inspector->detect(&move) )
    {
      inspector->alignWithPen(   rotation, 
				 translation,
				 kitInitialRotL,
				 penInitialRotG,
				 pickOffsetL);

      // fire the move callback
      if(moveCallback) moveCallback(moveCallbackData, this);
    }
  //else if (!isInside(h3a))//inspector->detect(&exit))
    //  {
  //  highlighted = false;
  // }
*/
}

void 
SoVCDraggerKit::setStartCallback(SoVCDraggerKitCB* cb, void* userData)
{
    startCallback = cb;
    startCallbackData = userData;
}

void 
SoVCDraggerKit::setFinishCallback(SoVCDraggerKitCB* cb, void* userData)
{
    finishCallback = cb;
    finishCallbackData = userData;
}

void 
SoVCDraggerKit::setMoveCallback(SoVCDraggerKitCB* cb, void* userData)
{
    moveCallback = cb;
    moveCallbackData = userData;
}

void 
SoVCDraggerKit::removeStartCallback()
{
    setStartCallback(NULL);
}

void 
SoVCDraggerKit::removeFinishCallback()
{
    setFinishCallback(NULL);
}

void 
SoVCDraggerKit::removeMoveCallback()
{
    setMoveCallback(NULL);
}

SbBool 
SoVCDraggerKit::isInside(const SoHandle3DEventAction *h3a)
{
    return inspector->isInside();
}

SbBool 
SoVCDraggerKit::isInterested()
{
  return inspector->isInterested();
}




