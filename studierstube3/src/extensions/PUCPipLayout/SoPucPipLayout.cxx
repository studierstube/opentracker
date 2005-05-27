/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
/** Source file defines the entry point for the application dll.
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucPipLayout.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucPipLayout.h"

#include "SoWidgetLayoutGroup.h"
#include "SoPanelGroup.h"
#include "SoPanel.h"
#include <Inventor/nodes/SoCube.h>

#include <Inventor/nodes/SoSeparator.h>

#include <iostream>

using namespace std;

#ifdef _DEBUG_PUC
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucPipLayout);

//----------------------------------------------------------------------------

void SoPucPipLayout::initClass()
{
//    SO_KIT_INIT_CLASS(SoPucPipLayout, SoBaseKit, "BaseKit");
   SO_KIT_INIT_CLASS(SoPucPipLayout, SoLayoutKit, "SoLayoutKit");

	SoWidgetLayoutGroup::initClass();
	SoPanelGroup::initClass();
	SoPanel::initClass();

}

//----------------------------------------------------------------------------

SoPucPipLayout::SoPucPipLayout()//:
//layoutAction()
{
	 SO_KIT_CONSTRUCTOR(SoPucPipLayout);
    
    //--- partName, partClassName, nullByDefault, parentName, rightName, isPublicPart

   //---                     partName, partClassName, nullByDefault, parentName, rightName, isPublicPart
	SO_KIT_ADD_CATALOG_ENTRY(pucAppliances, SoPucServer,  TRUE, this, \x0, TRUE);

			
	SO_NODE_ADD_FIELD(units, (5));
	SO_NODE_ADD_FIELD(columns, (2));
	SO_NODE_ADD_FIELD(layoutUpdated, ());
	SO_NODE_DEFINE_ENUM_VALUE(Style, PUC);
	SO_NODE_DEFINE_ENUM_VALUE(Style, STB);
	SO_NODE_DEFINE_ENUM_VALUE(Style, PUCEXT);

	
	SO_NODE_SET_SF_ENUM_TYPE(style, Style);
	
	SO_NODE_ADD_FIELD( style, (PUC));

	SO_KIT_INIT_INSTANCE();	

	nodeSensor.setData(this);
	nodeSensor.setFunction(nodeSensorCB);
	nodeSensor.setPriority(0);
	nodeSensor.setTriggerPathFlag(TRUE);

	setUpConnections(TRUE, TRUE);


}

//----------------------------------------------------------------------------

SoPucPipLayout::~SoPucPipLayout()
{

}

//----------------------------------------------------------------------------

//static 
void SoPucPipLayout::nodeSensorCB( void *data, SoSensor *sen)
{

	//std::cout << "SoPucPipLayout::nodeSensorCB" << endl;
	((SoPucPipLayout*)data)->doLayout();

}


//----------------------------------------------------------------------------

void SoPucPipLayout::doLayout()
{
	nodeSensor.detach();

	//SoSeparator *sep = initLayoutAction();
	SoBuildPanelAction::initClass();
	SoBuildPanelAction panelAction;
	panelAction.setDimensions(width.getValue(), height.getValue(), depth.getValue());
	panelAction.setStyle(style.getValue());
	panelAction.setColumns(columns.getValue());
	panelAction.setUnits(units.getValue());
	((SoPucServer*)(pucAppliances.getValue()))->stopNodeSensor();
	panelAction.apply(pucAppliances.getValue());

	SoSeparator *sep = panelAction.getPipLayout();

	std::cout << "layoutAction got " << sep<< endl;
	// remove old geometry
	((SoSeparator *)geometry.getValue())->removeAllChildren();
	// add new geometry
	((SoSeparator *)geometry.getValue())->addChild( sep );

	//writeFile("pucLayoutGeom.iv",geometry.getValue(),FALSE);
	
	layoutUpdated.setValue();

	// TODO activate later 
	((SoPucServer*)(pucAppliances.getValue()))->startNodeSensor();

}
#pragma
//----------------------------------------------------------------------------

SbBool SoPucPipLayout::setUpConnections( SbBool onOff, SbBool doItAlways )
{
 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    //SoBaseKit::setUpConnections(onOff, doItAlways);
	SoLayoutKit::setUpConnections(onOff, doItAlways);
    // do your own special setup here !
	if (pucAppliances.getValue() != NULL) 
	{
		nodeSensor.attach(pucAppliances.getValue());
		nodeSensor.schedule();
	}
  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
	nodeSensor.detach();
//    SoBaseKit::setUpConnections(onOff, doItAlways);
	SoLayoutKit::setUpConnections(onOff, doItAlways);
  }
  return !(connectionsSetUp = onOff);
}

//----------------------------------------------------------------------------

//virtual 
void SoPucPipLayout::resizeCB()
{
	// stupid to do .... 
	// but there is no ?!? other solution ?!?
	SoScale *scale = (SoScale *) this->scale.getValue();
	assert(scale != NULL);
	scale->scaleFactor.setValue(1, 1, 1);
	// have to keep scale to identity else it will mess up the whole layout.
}						

//----------------------------------------------------------------------------

//virtual 
void SoPucPipLayout::doStyleAction(SoAction *action)
{
	SoSeparator *geom= SO_GET_PART(this, "geometry", SoSeparator);
	action->traverse(geom);

}

//----------------------------------------------------------------------------
