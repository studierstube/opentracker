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
//  PROJECT:    Studierstube
//  CONTENT:    SoBase3DButton - implementation
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw              Hermann Wurnig
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:12:28  gh      last modification
// ===========================================================================
#ifndef __SOBASE3DBUTTON_H__
#define __SOBASE3DBUTTON_H__

#include <Inventor/SbPList.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>

#include <Inventor/engines/SoCalculator.h>

#include <Inventor/fields/SoSFBool.h>

#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoVertexProperty.h>

#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/event3d/Base3D.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoHandle3DEventAction;
class SoBase3DButton;

////////////////////////////////////////////////////////////////////
//
//  Class: SoBase3DButton
//
//  Contains all functions for handling a 3D-Button. Acts like
//  a SoSeparator-Node. 
//
////////////////////////////////////////////////////////////////////

/**
 * SoBase3DButton. Documented at
 * http://www.studierstube.org/stbapi/SoBase3DButton.html
 * @ingroup oldwidgets
 * @ingroup deprecated
 */

class STBAPI_API SoBase3DButton : public SoBaseKit, public Base3D {

	SO_KIT_HEADER(SoBase3DButton);

	BASE3D_HEADER;

	SO_KIT_CATALOG_ENTRY_HEADER(kitSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(additionalGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(colorSwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(bodySeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(bodySwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(textureSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(textureSwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(textSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(textSwitch);
	SO_KIT_CATALOG_ENTRY_HEADER(surroundingDrawStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(surroundingSeparator);

  public:
		// member-variables
	SoSFColor	    buttonColor;// diffuse color of button
	SoSFColor		highlightColor;	
								// diffuse color for highlights
	SoSFColor		dragColor;	
	SoSFFloat		width, depth, height;

	SoSFString		nodeId;
	SoSFInt32		pressed;		// 1 if button is pressed, 0 else

	SoSFInt32		highlighted;	// 1 if button is highlighted, 
									// 2 if button is pressed, 0 else
	SoSFInt32		triggerButton;	// number of pen-button used for 
										// manipulation

		// member-methods
	static void initClass();	

				SoBase3DButton();				
				SoBase3DButton(float width, float height, float depth);
				SoBase3DButton(float width, float height, float depth, 
						SoTexture2 *buttonTexture, float xDist = 0,
						float yDist = 0);
				SoBase3DButton(float width, float height, float depth, 
						char *fontName, int fontSize, char *buttonText,
						float textHeight, SbColor textColor);

	virtual SbBool isInterested(SoHandle3DEventAction *h3a);
	virtual void handle3DEvent(SoHandle3DEventAction *h3a);
	
	virtual void getBoundingBox(SoGetBoundingBoxAction *action);

	SbBool		isPressed();
	void		setPressed(SbBool pressB, SbBool forceUpdate=FALSE);
	SbBool		isHighlighted();
	void		setHighlighted(SbBool highlightB);
	void		disableHighlighting(SbBool disHigh);
    int         getInsideState() {return inside;}
	
	void		setGeometry(SoNode *geometry, SoSensorCB *callback);
	void		setPressedGeometry(SoNode *geometry);
	SoNode     *getGeometry();
	SoNode     *getPressedGeometry();

	void		setTexture(SoNode *textureNode);
	void		setPressedTexture(SoNode *textureNode);
	SoNode     *getTexture();
	SoNode     *getPressedTexture();

	void		setText(SoNode *textNode);
	void		setPressedText(SoNode *textNode);
	SoNode	   *getText();
	SoNode	   *getPressedText();

	void		setSurrounding(SoNode *surNode);
	SoNode	   *getSurrounding();

	void		setAdditionalGeometry(SoNode *geometry);
	SoNode	   *getAdditionalGeometry();

    void        setupTexture(SoTexture2 *buttonTexture,float xDist,float yDist);
    void        updateFromNetwork(SbBool pressedState);

  protected:
		// member-variables
	static void setupButtonMesh(void *data, SoSensor *);
	
	SoFieldSensor *heightSensor, *widthSensor, *depthSensor;
	SoSwitch      *theBodySwitch, *theTextSwitch, *theTextureSwitch;
    SoSeparator   *theSurroundingSep;
	SoSFBool	disableHL;		// TRUE for disabling highlighting
								// FALSE for enabling it
	int			inside;
	int			highlightCount;	// number of users highlighting the button
	int 		dragStation;	// number of tracker station that
								// holds the button pressed of -1
	SbBool		dragging;
	static		int baseInitClassAttempts;
		// member-methods

	virtual	   ~SoBase3DButton();	

  private:
	int firstTime;
		// member-methods
	void		initFields(float width, float height, float depth);
	void		createButtonGraph();

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

