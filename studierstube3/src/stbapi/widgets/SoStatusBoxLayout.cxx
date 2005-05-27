/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** source file for SoStatusBox class.
  *
  * @author Valérie Maquil
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoFaceSet.h>


#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoAppearanceKit.h>

#include <Inventor/actions/SoSearchAction.h> 

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>

#include <stbapi/interaction/SoBehaviorKit.h>
//#include <stbapi/widgets/SoButtonKit.h>
#include <stbapi/widgets/SoStatusBox.h>
#include <stbapi/widgets/SoStatusBoxLayout.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/misc/SoAbortGroup.h>


#include <assert.h>

#define _DEBUG_STATUSBOXLAYOUT

#if defined( _DEBUG_STATUSBOXLAYOUT) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif


SO_KIT_SOURCE(SoStatusBoxLayout);

// ------------------------------------------------------------------------------------------

void SoStatusBoxLayout::initClass()
{
	SO_KIT_INIT_CLASS(SoStatusBoxLayout, SoBaseKit, "BaseKit");
}

// ------------------------------------------------------------------------------------------

SoStatusBoxLayout::SoStatusBoxLayout()
{
	SO_KIT_CONSTRUCTOR(SoStatusBoxLayout);

	//---------------------------------------+------------+------+---------------+-------+---------------
	//                                       |            |      |               | right |
	//                                       |            | null |               | Sib-  | is
	//                                       |            | By   |               | ling  | public
	//                         name          | className  | Def. |  parent Name  | Name  | part
	//---------------------------------------+------------+------+---------------+-------+---------------
	SO_KIT_ADD_CATALOG_ENTRY(base,			  SoSeparator,	FALSE, this,			    \x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(layout,		  SoSeparator , FALSE, base,			    \x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(hiddenGeometry,  SoAbortGroup, FALSE, base,			    \x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(statusBox,		  SoStatusBox,	FALSE, hiddenGeometry,	    \x0,    TRUE);
	SO_KIT_ADD_CATALOG_LIST_ENTRY(widgets,	  SoGroup,		FALSE, hiddenGeometry,      \x0, SoBehaviorKit, TRUE);

	SO_KIT_INIT_INSTANCE();

	SoAbortGroup *abortGrp = (SoAbortGroup *)readNodeFromMemory("SoAbortGroup { abortActions (RENDER | BBOX | EVENT3D) }");
	assert( abortGrp != NULL );
	assert( setPart("hiddenGeometry", abortGrp ) );


	layoutSensor.setData(this);
	layoutSensor.schedule();
	layoutSensor.setFunction(layoutCB);

	setUpConnections(TRUE, TRUE);

}

// ------------------------------------------------------------------------------------------

SoStatusBoxLayout::~SoStatusBoxLayout()
{

}

// ------------------------------------------------------------------------------------------

SbBool SoStatusBoxLayout::setUpConnections( SbBool onOff, SbBool doItAlways )
{
 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoBaseKit::setUpConnections(onOff, doItAlways);

	layoutSensor.attach(layout.getValue());

	/*SoSeparator *layoutElements = (SoSeparator *)layout.getValue();
	SoNodeKitListPart *widgetList = (SoNodeKitListPart *)widgets.getValue();
	int numChildren = widgetList->getNumChildren();
	for (int i=0; i<numChildren; i++) widgetList->removeChild(i);

	findWidgets(layoutElements);
	
	numChildren = widgetList->getNumChildren();

	for (int i=0; i<numChildren; i++)
	{
		SoBehaviorKit *widget = (SoBehaviorKit *)widgetList->getChild(i);
		focusSensor[i].setData(this);
		focusSensor[i].setFunction(focusCB);
		focusSensor[i].attach(&(widget->inside));
	}*/
  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
	SoNodeKitListPart *widgetList = (SoNodeKitListPart *)widgets.getValue();
	int numChildren = widgetList->getNumChildren();

	/*for (int i=0; i<numChildren; i++)
	{
		SoBehaviorKit *widget = (SoBehaviorKit *)widgetList->getChild(i);
		focusSensor[i].detach();
	}*/

	for (int i= 0 ; i<focusSensors.getLength(); i++)
	{
		((SoFieldSensor*)focusSensors[i])->detach();
		delete (SoFieldSensor*)focusSensors[i];
	}

    SoBaseKit::setUpConnections(onOff, doItAlways);
  }
  return !(connectionsSetUp = onOff);
}

// ------------------------------------------------------------------------------------------
void SoStatusBoxLayout::focusCB(void *data, SoSensor *sensor)
{
	//DEBUG_CODE(printf("######### focusCB.\n"));
	SoStatusBoxLayout *self = (SoStatusBoxLayout*)data;
	assert(self);

	SoNodeKitListPart *widgetList = (SoNodeKitListPart *)self->widgets.getValue();
	SoFieldSensor *fSensor = (SoFieldSensor *)sensor;
	SoBehaviorKit *curWidget = (SoBehaviorKit *)fSensor->getAttachedField()->getContainer();
	SoStatusBox *statusBox = (SoStatusBox *)self->statusBox.getValue();

	if (curWidget->inside.getValue())
	{	
		statusBox->setLabel(&(curWidget->statusBoxText));
		statusBox->show.setValue(true);
	}
	else
	{
		statusBox->show.setValue(false);
	}
}

// ------------------------------------------------------------------------------------------

void SoStatusBoxLayout::layoutCB(void *data, SoSensor *sensor)
{
	DEBUG_CODE(printf("######### layoutCB.\n"));
	SoStatusBoxLayout *self = (SoStatusBoxLayout*)data;
	assert(self);
	self->layoutSensor.detach();


	SoSeparator *layoutElements = (SoSeparator *)self->layout.getValue();
	SoNodeKitListPart *widgetList = (SoNodeKitListPart *)self->widgets.getValue();
	int numChildren = widgetList->getNumChildren();
	for (int i=0; i<numChildren; i++) widgetList->removeChild(i);

	self->findWidgets(layoutElements);
	
	widgetList = (SoNodeKitListPart *)self->widgets.getValue();
	numChildren = widgetList->getNumChildren();


	/*for (int i=0; i<numChildren; i++)
	{
		SoBehaviorKit *widget = (SoBehaviorKit *)widgetList->getChild(i);
		self->focusSensor[i].setData(self);
		self->focusSensor[i].setFunction(self->focusCB);
		self->focusSensor[i].attach(&(widget->inside));
	}*/

	self->focusSensors.truncate(0);

	for (int i=0; i<numChildren; i++)
	{
		SoBehaviorKit *widget = (SoBehaviorKit *)widgetList->getChild(i);
		SoFieldSensor *triggerSensor = new SoFieldSensor();
		triggerSensor->setData(self);
		triggerSensor->setFunction( self->focusCB );
		triggerSensor->attach(&(widget->inside));
		self->focusSensors.append(triggerSensor);
	}


	//writeFile("pucLayoutGeom.iv",self,FALSE);



}

// ------------------------------------------------------------------------------------------

void SoStatusBoxLayout::findWidgets(SoNode *start)
{
	SoNodeKitListPart *widgList = (SoNodeKitListPart *)widgets.getValue();

	SoSearchAction sa1;
    sa1.setType(SoBehaviorKit::getClassTypeId());
    sa1.setInterest(SoSearchAction::ALL);
    sa1.setSearchingAll(TRUE);
    sa1.apply(start);
    SoPathList & pl = sa1.getPaths();

    for (int i = 0; i < pl.getLength(); i++) {
      SoFullPath * p = (SoFullPath*) pl[i];
      if (p->getTail()->isOfType(SoBehaviorKit::getClassTypeId())) {
       widgList->addChild((SoBehaviorKit *)p->getTail());
      }
    }
    sa1.reset();

	SoSearchAction sa2;
    sa2.setType(SoStatusBox::getClassTypeId());
    sa2.setInterest(SoSearchAction::FIRST);
    sa2.setSearchingAll(TRUE);
    sa2.apply(start);

	SoFullPath * path = (SoFullPath *) sa2.getPath();
    if (path->getTail()->isOfType(SoStatusBox::getClassTypeId())) {
		SoStatusBox *sb = (SoStatusBox *)path->getTail();
       setPart("statusBox", sb);
	   sb->hideLabel.touch();
      }
    sa2.reset();


}

// ------------------------------------------------------------------------------------------
