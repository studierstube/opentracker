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
/** SoBase3DButton - implementation
  *
  * @author  Hermann Wurnig, Gerd Hesina
  *
  * $Id: SoBase3DButton.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoNormalBinding.h>

#include <cassert>
#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stbapi/widgets/SoBase3DButton.h>
#include <stbapi/workspace/StbWorkspace.h>

int SoBase3DButton::baseInitClassAttempts = 0;

SO_KIT_SOURCE(SoBase3DButton);

BASE3D_SOURCE(SoBase3DButton);

void
SoBase3DButton::getBoundingBox(SoGetBoundingBoxAction *action)
{
  int numIndices;
  const int *indices;

  switch (action->getPathCode(numIndices, indices))
  {
    case SoAction::IN_PATH:
	{
      if (children!=NULL)
         children->traverse(action);
	}
	break;

	case SoAction::OFF_PATH:
    {
		if (children!=NULL)
         children->traverse(action,0);
    }
	break;

    case SoAction::BELOW_PATH:
    case SoAction::NO_PATH:
    { 
	  SbViewportRegion vp;
      SoGetBoundingBoxAction bba(vp);   
      //extend boundingBox with existing Geometry
      if (bodySwitch.getValue()!=NULL)
        bba.apply(bodySwitch.getValue());
	  switch (action->getWhatReset())
	  {
        case SoGetBoundingBoxAction::ALL:
		{   
          action->extendBy(bba.getXfBoundingBox()); 
		}
        break;
        case SoGetBoundingBoxAction::BBOX:
		{
          action->extendBy(bba.getBoundingBox()); 
		}
        break;
        case SoGetBoundingBoxAction::TRANSFORM:
        break;
	  }
    }
    break;
  }	
}

void 
SoBase3DButton::setupButtonMesh(void *data, SoSensor *)
{
	int i;
	
	SoBase3DButton *button = (SoBase3DButton *) data;
		
	float height= button->height.getValue();
	float width = button->width.getValue();
	float depth = button->depth.getValue();
	float top=height;

	if (width < top) top=width;
	if (depth < top) top=depth;
	top*=0.1;

	float vertex1Data[14][3]=
		{{-width/2,-height/2,depth/2}, 
		 {-width/2,height/2-top,depth/2},
		 {-width/2+top,height/2,depth/2-top},
		 {-width/2+top,height/2,-depth/2+top},
		 {-width/2,height/2-top,-depth/2},
		 {-width/2,-height/2,-depth/2}, 
		 {-width/2,-height/2,depth/2}, 

		 {width/2,-height/2,depth/2}, 
		 {width/2,height/2-top,depth/2},
		 {width/2-top,height/2,depth/2-top},
		 {width/2-top,height/2,-depth/2+top},
		 {width/2,height/2-top,-depth/2},
		 {width/2,-height/2,-depth/2}, 
		 {width/2,-height/2,depth/2}};

	float vertex2Data[12][3]=
		{{width/2-top,height/2,-depth/2+top},
		 {width/2,height/2-top,-depth/2},
		 {width/2,-height/2,-depth/2}, 
		 {-width/2,-height/2,-depth/2}, 
		 {-width/2,height/2-top,-depth/2},
		 {-width/2+top,height/2,-depth/2+top},

		 {width/2-top,height/2,depth/2-top},
		 {width/2,height/2-top,depth/2},
		 {width/2,-height/2,depth/2}, 
		 {-width/2,-height/2,depth/2}, 
		 {-width/2,height/2-top,depth/2},
		 {-width/2+top,height/2,depth/2-top}};


	SoGroup *bGroup = new SoGroup;

	SoCoordinate3 *bCoord1 = new SoCoordinate3;
	for (i=0;i<14;i++)
		bCoord1->point.set1Value(i,vertex1Data[i]);
	bGroup->addChild(bCoord1);
	
	SoQuadMesh *bMesh1 = new SoQuadMesh;
	bMesh1->verticesPerRow=7;
	bMesh1->verticesPerColumn=2;
	bGroup->addChild(bMesh1);

	SoCoordinate3 *bCoord2 = new SoCoordinate3;
	for (i=0;i<12;i++)
		bCoord2->point.set1Value(i,vertex2Data[i]);
	bGroup->addChild(bCoord2);
	
	SoQuadMesh *bMesh2 = new SoQuadMesh;
	bMesh2->verticesPerRow=6;
	bMesh2->verticesPerColumn=2;
	bGroup->addChild(bMesh2);
	
	button->setGeometry(bGroup, setupButtonMesh);

	for (i=0;i<14;i++)
		if (vertex1Data[i][1]>0) vertex1Data[i][1]-=height/2;
	for (i=0;i<12;i++)
		if (vertex2Data[i][1]>0) vertex2Data[i][1]-=height/2;

	bGroup = new SoGroup;

	bCoord1 = new SoCoordinate3;
	for (i=0;i<14;i++)
		bCoord1->point.set1Value(i,vertex1Data[i]);
	bGroup->addChild(bCoord1);
	
	bMesh1 = new SoQuadMesh;
	bMesh1->verticesPerRow=7;
	bMesh1->verticesPerColumn=2;
	bGroup->addChild(bMesh1);

	bCoord2 = new SoCoordinate3;
	for (i=0;i<12;i++)
		bCoord2->point.set1Value(i,vertex2Data[i]);
	bGroup->addChild(bCoord2);
	
	bMesh2 = new SoQuadMesh;
	bMesh2->verticesPerRow=6;
	bMesh2->verticesPerColumn=2;
	bGroup->addChild(bMesh2);
	
	button->setPressedGeometry(bGroup);

	SoCube *bCube = new SoCube;
	bCube->width = width*1.05;
	bCube->height = height*1.3;
	bCube->depth = depth*1.05;
	button->setSurrounding(bCube);
}


//###################################################################
//## FUNCTIONS FOR HANDLING THE BUTTON-SURROUNDING
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setSurrounding
// Purpose:		This function inserts the given node surNode in the
//				button-graph for use as surrounding geometry
// Parameters:	surNode:	pointer to the surrounding-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DButton::setSurrounding(SoNode *surNode)
{
	theSurroundingSep->replaceChild(0,surNode);
}
	
/////////////////////////////////////////////////////////////////////
// Name:		getSurrounding
// Purpose:		This function returns a pointer to the buttons
//				surrounding-geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the surrounding-geomerty
/////////////////////////////////////////////////////////////////////
SoNode * 
SoBase3DButton::getSurrounding()
{
	return theSurroundingSep->getChild(0);
}

//###################################################################
//## FUNCTIONS FOR HANDLING ADDITIONAL GEOMETRY
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setAdditionalGeometry
// Purpose:		This function inserts the given node geometry in the
//				button-graph for use as new additional geomerty
// Parameters:	geometry:	pointer to the geometry-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DButton::setAdditionalGeometry(SoNode *geometry)
{		// replace old with new geometry
	SoGroup *aSep = (SoGroup *) additionalGeometry.getValue();
		
	if (aSep->getNumChildren() == 0)
		aSep->addChild(geometry);
	else
		aSep->replaceChild(0,geometry); 
}

/////////////////////////////////////////////////////////////////////
// Name:		getAdditionalGeometry
// Purpose:		This function returns a pointer to the graph of the
//				buttons geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the dials geometry
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DButton::getAdditionalGeometry()
{		// return pointer to geometry
	SoGroup *aSep = (SoGroup *) additionalGeometry.getValue();
		
	if (aSep->getNumChildren() == 0)
		return NULL;
	else
		return aSep->getChild(0); 
}


//###################################################################
//## FUNCTIONS FOR HANDLING THE BUTTON-TEXT
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setText
// Purpose:		This function inserts the given node textNode in the
//				button-graph for use as unpressed button text-graph
// Parameters:	textNode:	pointer to the text-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DButton::setText(SoNode *textNode)
{
	theTextSwitch->replaceChild(0,textNode);
}

/////////////////////////////////////////////////////////////////////
// Name:		setPressedText
// Purpose:		This function inserts the given node textNode in the
//				button-graph for use as pressed button text-graph
// Parameters:	textNode:	pointer to the text-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DButton::setPressedText(SoNode *textNode)
{
	theTextSwitch->replaceChild(1,textNode);
}

/////////////////////////////////////////////////////////////////////
// Name:		getText
// Purpose:		This function returns a pointer to the buttons
//				unpressed text-graph
// Parameters:	---
// Returnvalue: pointer to the root-node of the buttons text-graph
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DButton::getText()
{
	return theTextSwitch->getChild(0);
}

/////////////////////////////////////////////////////////////////////
// Name:		getPressedText
// Purpose:		This function returns a pointer to the buttons
//				pressed text-graph
// Parameters:	---
// Returnvalue: pointer to the root-node of the buttons text-graph
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DButton::getPressedText()
{
	return theTextSwitch->getChild(1);
}


//###################################################################
//## FUNCTIONS FOR HANDLING THE BUTTON-TEXTURE
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setTexture
// Purpose:		This function inserts the given node textureNode in 
//				the button-graph for use as unpressed button 
//				texture-graph
// Parameters:	textureNode:	pointer to the texture-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DButton::setTexture(SoNode *textureNode)
{
	theTextureSwitch->replaceChild(0,textureNode);
}

/////////////////////////////////////////////////////////////////////
// Name:		setPressedTexture
// Purpose:		This function inserts the given node textureNode in 
//				the button-graph for use as pressed button 
//				texture-graph
// Parameters:	textureNode:	pointer to the texture-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DButton::setPressedTexture(SoNode *textureNode)
{
	theTextureSwitch->replaceChild(1,textureNode);
}

/////////////////////////////////////////////////////////////////////
// Name:		getTexture
// Purpose:		This function returns a pointer to the buttons
//				unpressed texture-graph
// Parameters:	---
// Returnvalue: pointer to the root-node of the buttons texture-graph
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DButton::getTexture()
{
	return theTextureSwitch->getChild(0);
}

/////////////////////////////////////////////////////////////////////
// Name:		getPressedTexture
// Purpose:		This function returns a pointer to the buttons
//				pressed texture-graph
// Parameters:	---
// Returnvalue: pointer to the root-node of the buttons texture-graph
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DButton::getPressedTexture()
{
	return theTextureSwitch->getChild(1);
}


//###################################################################
//## FUNCTIONS FOR HANDLING THE BUTTON-GEOMETRY
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setGeometry
// Purpose:		This function inserts the given node geometry in the
//				button-graph for use as unpressed button geomerty
// Parameters:	geometry:	pointer to the geometry-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DButton::setGeometry(SoNode *geometry, SoSensorCB *callback)
{	
	theBodySwitch->replaceChild(0,geometry);
}

/////////////////////////////////////////////////////////////////////
// Name:		setPressedGeometry
// Purpose:		This function inserts the given node geometry in the
//				button-graph for use as pressed button geomerty
// Parameters:	geometry:	pointer to the geometry-graph
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DButton::setPressedGeometry(SoNode *geometry)
{
 	theBodySwitch->replaceChild(1,geometry);
}

/////////////////////////////////////////////////////////////////////
// Name:		getGeometry
// Purpose:		This function returns a pointer to the graph of the
//				buttons unpressed geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the buttons geometry
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DButton::getGeometry()
{
	return theBodySwitch->getChild(0);
}

/////////////////////////////////////////////////////////////////////
// Name:		getPressedGeometry
// Purpose:		This function returns a pointer to the graph of the
//				buttons pressed geometry
// Parameters:	---
// Returnvalue: pointer to the root-node of the buttons geometry
/////////////////////////////////////////////////////////////////////
SoNode *
SoBase3DButton::getPressedGeometry()
{
	return theBodySwitch->getChild(1);
}


//###################################################################
//## SETTING AND READING HIGHLIGHTED/PRESSED-STATUS
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		isPressed
// Purpose:		returns FALSE, if button is not pressed (then
//				pressed == 0), else TRUE
// Parameters:	---
// Returnvalue: FALSE, when pressed == 0, else TRUE
/////////////////////////////////////////////////////////////////////
SbBool 
SoBase3DButton::isPressed()
{
	if (pressed.getValue() == 0) return FALSE;
	else return TRUE;
}

/////////////////////////////////////////////////////////////////////
// Name:		setPressed
// Purpose:		This function sets the field pressed to 1, if 
//				the parameter pressB is TRUE, else to 0
// Parameters:	pressB	: if TRUE -> pressed=1 else pressed=0
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DButton::setPressed(SbBool pressB, SbBool forceUpdate)
{	
	if(!forceUpdate&&getBase3DMode()==Base3D::BASE3D_SLAVEMODE) return;
	// only change value when necessary
	if (isPressed() != pressB)
	{
		if (pressB == TRUE) pressed=1;
		else pressed=0;
		if(getBase3DMode()==Base3D::BASE3D_MASTERMODE) SETBUTTONSTATE(this);
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		disableHighlighting
// Purpose:		sets the field disableHL with the value of the
//				parameter disHigh. If disalbeHL is TRUE, no
//				highlighting is preformed
// Parameters:	disHigh:	TRUE for disabling highlighting,
//							FALSE for enabling it
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DButton::disableHighlighting(SbBool disHigh)
{
	disableHL = disHigh;
//	if (disHigh == TRUE) 
//		highlighted.setValue(0);
}

/////////////////////////////////////////////////////////////////////
// Name:		isHighlighted
// Purpose:		returns FALSE, if button is not highlighted (then
//				highlighted == 0), else TRUE
// Parameters:	---
// Returnvalue: FALSE, when highlighted == 0, else TRUE
/////////////////////////////////////////////////////////////////////
SbBool 
SoBase3DButton::isHighlighted()
{
	if (highlighted.getValue() == 0) return FALSE;
	else return TRUE;
}

/////////////////////////////////////////////////////////////////////
// Name:		setHighlighted
// Purpose:		This function sets the field highlighted to 1, if 
//				the parameter highlightB is TRUE, else to 0
// Parameters:	highlightB	: if TRUE -> highlighted=1 
//							  else highlighted=0
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DButton::setHighlighted(SbBool highlightB)
{	
	// if highlightB == FALSE then turn off highlighting
    DISABLEDIVFIELD(highlighted);
	if (highlightB == FALSE) 
	{
		highlightCount--;
		if (highlightCount == 0) highlighted = 0;
	}
	else	// else turn it on when no one is dragging 
	{
		if (highlighted.getValue() != 2) highlighted = 1;
		highlightCount++;
	}
    ENABLEDIVFIELD(highlighted);
}


//###################################################################
//## INITIALIZATION FUNCTIONS 
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		initClass
// Purpose:		sets up type- and file-format name-information for
//				the class
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DButton::initClass()
{
	if (SoBase3DButton::baseInitClassAttempts == 0)
	{
		SoBase3DButton::baseInitClassAttempts = 1;
			// use initialization-macro
		SO_KIT_INIT_CLASS(SoBase3DButton, SoBaseKit, "BaseKit");

		BASE3D_INIT_CLASS(SoBase3DButton);
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		createButtonGraph
// Purpose:		Builds up the default button-geometry and the 
//				field-connections
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DButton::createButtonGraph()
{
		// setup colors for normal/highlighted state
	SoSwitch *cSwitch = (SoSwitch *) colorSwitch.getValue();
	cSwitch->whichChild.connectFrom(&highlighted);

	SoMaterial *bMat = new SoMaterial;
	bMat->diffuseColor.connectFrom(&buttonColor);
	cSwitch->addChild(bMat);

	bMat = new SoMaterial;
	bMat->diffuseColor.connectFrom(&highlightColor);
	cSwitch->addChild(bMat);
 
	bMat = new SoMaterial;
	bMat->diffuseColor.connectFrom(&dragColor);
	cSwitch->addChild(bMat);
 
		// setup surrounding
	SoDrawStyle *bStyle = 
		(SoDrawStyle *) surroundingDrawStyle.getValue();
	bStyle->style.setValue(SoDrawStyle::INVISIBLE);

	SoGroup *bGr = new SoGroup;

	SoSeparator *bSep = (SoSeparator *)
		surroundingSeparator.getValue();
	bSep->addChild(bGr);
    theSurroundingSep=bSep;

	SoSwitch *bSwitch = (SoSwitch *) bodySwitch.getValue();
	bSwitch->whichChild.connectFrom(&pressed);
	bSwitch->addChild(bGr);
	bSwitch->addChild(bGr);
	theBodySwitch=bSwitch;

	SoSwitch *tSwitch = (SoSwitch *) textSwitch.getValue();
	tSwitch->whichChild.connectFrom(&pressed);
	tSwitch->addChild(bGr);
	tSwitch->addChild(bGr);
    theTextSwitch=tSwitch;

	tSwitch = (SoSwitch *) textureSwitch.getValue();
	tSwitch->whichChild.connectFrom(&pressed);
	tSwitch->addChild(bGr);
	tSwitch->addChild(bGr);
    theTextureSwitch=tSwitch;

	heightSensor= new SoFieldSensor(setupButtonMesh, (void *)this);
	widthSensor = new SoFieldSensor(setupButtonMesh, (void *)this);
	depthSensor = new SoFieldSensor(setupButtonMesh, (void *)this);

	widthSensor->setPriority(0);
	heightSensor->setPriority(0);
	depthSensor->setPriority(0);

	heightSensor->attach(&height);
	widthSensor->attach(&width);
	depthSensor->attach(&depth);

	setupButtonMesh((void *) this, NULL);
}

/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		defines the fields used by SoBase3DButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
SoBase3DButton::initFields(float width, float height, float depth)
{

	SO_KIT_CONSTRUCTOR(SoBase3DButton);
	
		// declare fields and their default-values
	SO_KIT_ADD_FIELD(width,				(width));
	SO_KIT_ADD_FIELD(depth,				(depth));
	SO_KIT_ADD_FIELD(height,			(height));
	SO_KIT_ADD_FIELD(buttonColor,		(0.7,0.65,0.7));
	SO_KIT_ADD_FIELD(highlightColor,	(0.9,0.9,0.2));
	SO_KIT_ADD_FIELD(dragColor,			(1.0,0.35,0.35));
	SO_KIT_ADD_FIELD(pressed,			(0));
	SO_KIT_ADD_FIELD(highlighted,		(0));
	SO_KIT_ADD_FIELD(triggerButton,		(STB_PRIMARY_BUTTON));
	SO_KIT_ADD_FIELD(disableHL,			(FALSE));
	SO_KIT_ADD_FIELD(nodeId,			(""));

	SO_KIT_ADD_CATALOG_ENTRY(kitSeparator,SoSeparator,FALSE,this,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(additionalGeometry,SoGroup,FALSE,kitSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,SoSeparator,FALSE,kitSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(colorSwitch,SoSwitch,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(bodySeparator,SoSeparator,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(bodySwitch,SoSwitch,FALSE,bodySeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(textureSeparator,SoSeparator,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(textureSwitch,SoSwitch,FALSE,textureSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(textSeparator,SoSeparator,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(textSwitch,SoSwitch,FALSE,textSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(surroundingDrawStyle,SoDrawStyle,FALSE,topSeparator,\x0,TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(surroundingSeparator,SoSeparator,FALSE,topSeparator,\x0,TRUE);

	SO_KIT_INIT_INSTANCE();

	BASE3D_CONSTRUCTOR;

		// initialize non-field member-variables
	inside=0;
	highlightCount=0;
	dragStation=-1;
	dragging=FALSE;
}


//###################################################################
//## CONSTRUCTORS/DESTRUCTOR
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		SoBase3DButton
// Purpose:		Default-Constructor of class SoBase3DButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoBase3DButton::SoBase3DButton()
{
	firstTime=TRUE;
	// init the fiels of SoBase3DButton
	initFields(0.04,0.005,0.02);
		// and build up the default button-geometry
	createButtonGraph();
}


/////////////////////////////////////////////////////////////////////
// Name:		SoBase3DButton
// Purpose:		Constructor of class SoBase3DButton that sets the button
//				to a given size
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoBase3DButton::SoBase3DButton(float width, float height, float depth)
{
		// init the fiels of SoBase3DButton
	firstTime=TRUE;
	initFields(width, height, depth);
		// and build up the default button-geometry
	createButtonGraph();
}

/////////////////////////////////////////////////////////////////////
// Name:		SoBase3DButton
// Purpose:		Constructor of class SoBase3DButton that sets the button
//				to a given size and puts a given textture on it
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoBase3DButton::SoBase3DButton(float width, float height, float depth, 
					   SoTexture2 *buttonTexture, float xDist,
					   float yDist)
{
	// init the fiels of SoBase3DButton
	initFields(width, height,depth);
	// and build up the default button-geometry
	createButtonGraph();
    setupTexture(buttonTexture,xDist,yDist);
}



void SoBase3DButton::setupTexture(SoTexture2 *buttonTexture, float xDist,
					   float yDist)
{
    float w = width.getValue();
    float h = height.getValue();
    float d = depth.getValue();

	float top=w;
	if (h < top) top = h;
	if (d < top) top = d;
	top*=0.1;

	SoGroup *bGroup = new SoGroup;
	bGroup->addChild(buttonTexture);

	SoVertexProperty *myVertexProperty = new SoVertexProperty;
	myVertexProperty->vertex.set1Value(3, 
		SbVec3f(-w/2+top+xDist,  h*0.505, -d/2+top+yDist));
	myVertexProperty->vertex.set1Value(2, 
		SbVec3f( w/2-top-xDist,  h*0.505, -d/2+top+yDist));
	myVertexProperty->vertex.set1Value(1, 
		SbVec3f( w/2-top-xDist,  h*0.505,  d/2-top-yDist));
	myVertexProperty->vertex.set1Value(0, 
		SbVec3f(-w/2+top+xDist,  h*0.505,  d/2-top-yDist));

	myVertexProperty->normal.set1Value(0, SbVec3f(0, 1, 0));

	myVertexProperty->texCoord.set1Value(0, SbVec2f(0, 0));
	myVertexProperty->texCoord.set1Value(1, SbVec2f(1, 0));
	myVertexProperty->texCoord.set1Value(2, SbVec2f(1, 1));
	myVertexProperty->texCoord.set1Value(3, SbVec2f(0, 1));

    myVertexProperty->normalBinding = SoNormalBinding::OVERALL;

	SoFaceSet *myFaceSet = new SoFaceSet;
	bGroup->addChild(myFaceSet);
	myFaceSet->numVertices.set1Value(0, 4);
	myFaceSet->vertexProperty.setValue(myVertexProperty);

	setTexture((SoNode *) bGroup);

	SoGroup *b2Group=new SoGroup;

	SoTranslation *bTrans=new SoTranslation;
	bTrans->translation.setValue(0,-h/2,0);
	b2Group->addChild(bTrans);
	b2Group->addChild(bGroup);

	setPressedTexture((SoNode *) b2Group);
}


/////////////////////////////////////////////////////////////////////
// Name:		SoBase3DButton
// Purpose:		Constructor of class SoBase3DButton that sets the button
//				to a given size and puts a given textture on it
// Parameters:	width, height, depth: dimensions of button
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoBase3DButton::SoBase3DButton(float width, float height, float depth, 
		char *fontName, int fontSize, char *buttonText, 
		float textHeight, SbColor textColor)
{
		// init the fiels of SoBase3DButton

	if (buttonText == "") 
	{
		initFields(width,height,depth);
		createButtonGraph();
		return;
	}
	
	SoGroup *bGroup = new SoGroup;
	bGroup->ref();

	SoFont *myFont = new SoFont;
	myFont->name.setValue(fontName);
	myFont->size.setValue(fontSize);
	bGroup->addChild(myFont);

	SoMaterial *myMat = new SoMaterial;
	myMat->diffuseColor=textColor;
	bGroup->addChild(myMat);

	SoTranslation *bTrans=new SoTranslation;
	bTrans->translation.setValue(0,0,0);
	bGroup->addChild(bTrans);

	SoScale *bScale = new SoScale;
	bScale->scaleFactor.setValue(1,1,1);
	bGroup->addChild(bScale);

	SoRotationXYZ *bRot = new SoRotationXYZ;
	bRot->angle.setValue(-M_PI/2);
	bRot->axis = SoRotationXYZ::X;
	bGroup->addChild(bRot);

	SoText3 *myText = new SoText3;
	myText->string.setValue(buttonText);
	myText->justification=SoText3::CENTER;
	myText->parts=SoText3::FRONT;
	bGroup->addChild(myText);

	SbViewportRegion *bView = new SbViewportRegion();
	SoGetBoundingBoxAction *bAction = new SoGetBoundingBoxAction(*bView);

	bAction->apply(bGroup);

	const SbBox3f textBox = bAction->getBoundingBox();
	
	delete bView;
	delete bAction;

	float xmin,xmax,ymin,ymax,zmin,zmax;
	float bWidth, bDepth;
	textBox.getBounds(xmin,ymin,zmin,xmax,ymax,zmax);

	bWidth = (xmax-xmin);
	if (bWidth > width*0.9) bWidth += width*0.1; else bWidth = width;
	bDepth = (zmax-zmin);
	if (bDepth > depth*0.9) bDepth += depth*0.1; else bDepth = depth;
 	
	initFields(bWidth,height,bDepth);
	createButtonGraph();

	bScale->scaleFactor.setValue(1,textHeight/2,1);
	bTrans->translation.setValue(0,height/2+textHeight/2,-(zmin+zmax)/2);

	bGroup->unrefNoDelete();

	setText(bGroup);

	bGroup = new SoGroup;
	
	bTrans = new SoTranslation;
	bTrans->translation.setValue(0,-height/2,0);
	bGroup->addChild(bTrans);
	bGroup->addChild(getText());
	
	setPressedText(bGroup);

}

/////////////////////////////////////////////////////////////////////
// Name:		~SoBase3DButton
// Purpose:		Destructor of class SoBase3DButton
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
SoBase3DButton::~SoBase3DButton()
{
	heightSensor->detach();
	widthSensor->detach();
	depthSensor->detach();
	delete heightSensor;
	delete widthSensor;
	delete depthSensor;
}


//###################################################################
//## 3D-EVENT-HANDLING
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		isInterested
// Purpose:		returns TRUE if the handle3DEvent-method has to be
//				called ( i.e. the button is interested in the event)
// Parameters:	h3a:	event-handle
// Returnvalue: TRUE if interested, FALSE if not
/////////////////////////////////////////////////////////////////////
SbBool 
SoBase3DButton::isInterested(SoHandle3DEventAction *)
{
	return TRUE;
}
	
/////////////////////////////////////////////////////////////////////
// Name:		handle3DEvent
// Purpose:		this method is called whenever the event is
//				interesting for the button
// Parameters:	h3a:	event-handle
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
SoBase3DButton::handle3DEvent(SoHandle3DEventAction *)
{
}

void
SoBase3DButton::updateFromNetwork(SbBool pressedState)
{
	setPressed(pressedState,TRUE);
}

