/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoMenuWidget.h contains the definition of SoMenuWidget.
 * SoMenuWidget is the base class for all classes that are to be integrated
 * into the iOrb UI framework's SoMenuManager structure.
 *
 * @author  EMC
 *
 * $Id: SoMenuWidget.h,v 1.21 2005/01/26 23:16:52 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SO_MENU_WIDGET_H_
#define _SO_MENU_WIDGET_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoFont.h>

#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFName.h>
#include <Inventor/fields/SoSFColor.h>

#include "extension.h"

class SoMenuManager;

/**
 * SoMenuWidget is the base class for all classes that are to be integrated
 * into the iOrb UI framework's SoMenuManager structure.
 *
 * @author  EMC
 * @ingroup iorb
 */
class IORB_API SoMenuWidget: public SoBaseKit
{
	SO_KIT_HEADER(SoMenuWidget);

  	SO_KIT_CATALOG_ENTRY_HEADER(geometry);
	
public:
	// construction
	SoMenuWidget(void);
	static void initClass(void);

	// fields

	SoSFBool	active;
	SoSFFloat	rotationIn;
	SoSFTrigger	trigger;
	SoSFName	name;
	SoSFName	fontName;
	SoSFFloat	fontSize;
	
	// template geometry nodes
	SoSFNode	templateGeometry;
	SoSFNode	templateGeometryHighLight;
	SoSFNode	templateGeometrySelected;
	SoSFNode	templateGeometrySelectedHighLight;
	SoSFNode	templateGeometryUp;
	SoSFNode	templateGeometryDown;

	SoSFNode	testGeometry;

	SoSFColor	fontColor;

	// methods
	SbString getName();

	// user input methods
	virtual void onClickSelect();
	virtual void onClickConfirm();
	virtual void onPrimaryRotationChanged(float angleIncrement);
	virtual void onSecondaryRotationChanged(float angle);
	virtual void onRotationInChanged(const SbRotation& rotation);
	
	void setParent(SoMenuManager* parentmanager);

protected:
	// methods
	virtual ~SoMenuWidget(void);
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

	// template geometry
	void setTemplateGeometryStrings(SbString tG, SbString tGHL, SbString tGS, SbString tGSHL, SbString tGU, SbString tGD);
	SbString file_templateGeometry;
	SbString file_templateGeometryHighLight;
	SbString file_templateGeometrySelected;
	SbString file_templateGeometrySelectedHighLight;
	SbString file_templateGeometryUp;
	SbString file_templateGeometryDown;
	
	void loadDefaultTemplate(SoSFNode& _template, SbString filename);
	virtual void loadDefaultTemplates();
	SoNode* getTemplateGeometry();
	SoNode* getTemplateGeometryHighLight();
	SoNode* getTemplateGeometrySelected();
	SoNode* getTemplateGeometrySelectedHighLight();
	SoNode* getTemplateGeometryUp();
	SoNode* getTemplateGeometryDown();

	SoSeparator* getGeometryNode();
	void deleteNode(SoNode** node);
	virtual void createGeometry();
	virtual void destroyGeometry();
	
	SoMenuManager* getMenuManager();
	void setMenuWidget(SbString name);
	void setStartWidget();
	
	SoFont*		font;
	void createFontNode(SoGroup* parent);
	void deleteFontNode();

	// members
	SoMenuManager* parent;

};

#endif // _SO_MENU_WIDGET_H_
