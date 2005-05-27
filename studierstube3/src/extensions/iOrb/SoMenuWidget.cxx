/* =======================================================================
 * (C) 2005 Vienna University of Technology
 * =======================================================================
 * PROJECT: iOrb
 * ======================================================================= */
/** SoMenuWidget.cxx
 *
 * @author  EMC
 *
 * $Id: SoMenuWidget.cxx,v 1.21 2005/01/26 23:16:52 chris Exp $
 * @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoFile.h>
#include <Inventor/nodes/SoMaterial.h>

#include "SoMenuWidget.h"
#include "SoMenuManager.h"

SO_KIT_SOURCE(SoMenuWidget);

void SoMenuWidget::initClass(void) 
{
	if (SoType::fromName("SoMenuWidget").isBad())
	{
		SO_KIT_INIT_CLASS(SoMenuWidget, SoBaseKit, "BaseKit");
	}
}

SoMenuWidget::SoMenuWidget(void)
{
	font = NULL;
	setTemplateGeometryStrings("","","","","","");

	SO_KIT_CONSTRUCTOR(SoMenuWidget);

	SO_KIT_ADD_FIELD(active, (FALSE));
	SO_KIT_ADD_FIELD(rotationIn, (0));
	SO_KIT_ADD_FIELD(trigger, ());
	SO_KIT_ADD_FIELD(name, ("Default"));
	SO_KIT_ADD_FIELD(fontName, ("Arial"));
	SO_KIT_ADD_FIELD(fontSize, (20));
	SO_KIT_ADD_FIELD(templateGeometry, (NULL));
	SO_KIT_ADD_FIELD(templateGeometryHighLight, (NULL));
	SO_KIT_ADD_FIELD(templateGeometrySelected, (NULL));
	SO_KIT_ADD_FIELD(templateGeometrySelectedHighLight, (NULL));
	SO_KIT_ADD_FIELD(templateGeometryUp, (NULL));
	SO_KIT_ADD_FIELD(templateGeometryDown, (NULL));
	SO_KIT_ADD_FIELD(fontColor, (SbColor(1, 1, 1)));

	SO_NODE_ADD_FIELD(testGeometry, (NULL));

	SO_KIT_ADD_CATALOG_ENTRY(geometry, SoSeparator, FALSE, this, \0x0 , TRUE);
	
	SO_KIT_INIT_INSTANCE();

	setUpConnections(TRUE, TRUE);
}

SoMenuWidget::~SoMenuWidget(void)
{
	destroyGeometry();
	if (font != NULL) { deleteFontNode(); }
}

SbString SoMenuWidget::getName()
{
	return name.getValue().getString();
}

SbBool SoMenuWidget::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;

	if (onOff)
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onOff, doItAlways);
		loadDefaultTemplates();
	}
	else
	{
		// We disconnect BEFORE base class.
		SoBaseKit::setUpConnections(onOff, doItAlways);
	}
	return !(connectionsSetUp = onOff);
}

void SoMenuWidget::setTemplateGeometryStrings(SbString tG, SbString tGHL, SbString tGS, SbString tGSHL, SbString tGU, SbString tGD)
{
	this->file_templateGeometry = tG;
	this->file_templateGeometryHighLight = tGHL;
	this->file_templateGeometrySelected = tGS;
	this->file_templateGeometrySelectedHighLight = tGSHL;
	this->file_templateGeometryUp = tGU;
	this->file_templateGeometryDown = tGD;
}

void SoMenuWidget::loadDefaultTemplate(SoSFNode& _template, SbString filename)
{
	if ((_template.getValue() == NULL) && (filename != ""))
	{
		SoFile* filenode = new SoFile();
		filenode->name.setValue(filename);
		_template.setValue(filenode);
	}
}

void SoMenuWidget::loadDefaultTemplates()
{
	loadDefaultTemplate(templateGeometry,file_templateGeometry);
	loadDefaultTemplate(templateGeometryHighLight,file_templateGeometryHighLight);
	loadDefaultTemplate(templateGeometrySelected,file_templateGeometrySelected);
	loadDefaultTemplate(templateGeometrySelectedHighLight,file_templateGeometrySelectedHighLight);
	loadDefaultTemplate(templateGeometryUp,file_templateGeometryUp);
	loadDefaultTemplate(templateGeometryDown,file_templateGeometryDown);
}

SoNode* SoMenuWidget::getTemplateGeometry()
{
	return (templateGeometry.getValue());
}

SoNode* SoMenuWidget::getTemplateGeometryHighLight()
{
	return (templateGeometryHighLight.getValue());
}

SoNode* SoMenuWidget::getTemplateGeometrySelected()
{
	return (templateGeometrySelected.getValue());
}

SoNode* SoMenuWidget::getTemplateGeometrySelectedHighLight()
{
	return (templateGeometrySelectedHighLight.getValue());
}

SoNode* SoMenuWidget::getTemplateGeometryUp()
{
	return (templateGeometryUp.getValue());
}

SoNode* SoMenuWidget::getTemplateGeometryDown()
{
	return (templateGeometryDown.getValue());
}



SoSeparator* SoMenuWidget::getGeometryNode()
{
	return SO_GET_PART(this, "geometry", SoSeparator);
}

void SoMenuWidget::deleteNode(SoNode** node)
{
	if ((*node) != NULL)
	{
		(*node)->unref();
		(*node) = NULL;
	}
}

void SoMenuWidget::setMenuWidget(SbString name)
{
	if (parent != NULL) { parent->currentWidget.setValue(name); }
}

void SoMenuWidget::setStartWidget()
{
	if (parent != NULL) 
	{ 
		parent->exitCurrentWidget();
	}
}

void SoMenuWidget::createFontNode(SoGroup* parent)
{
	if (font != NULL) { deleteFontNode(); }

	SoMaterial* fontMaterial = new SoMaterial();
	fontMaterial->diffuseColor.setValue(fontColor.getValue());
	parent->addChild(fontMaterial);

	font = new SoFont();
	font->ref();
	font->name.setValue(fontName.getValue().getString());
	font->size.setValue(fontSize.getValue());
	parent->addChild(font);
}

void SoMenuWidget::deleteFontNode()
{
	font->unref();
}



void SoMenuWidget::createGeometry()
{
}

void SoMenuWidget::destroyGeometry()
{
}

	
SoMenuManager* SoMenuWidget::getMenuManager()
{	
	return parent;
}

void SoMenuWidget::setParent(SoMenuManager* parentmanager)
{
	this->parent = parentmanager;
}


void SoMenuWidget::onClickSelect()
{
	// User clicks to select an item in the list
}

void SoMenuWidget::onClickConfirm()
{
	// User clicks to confirm selection
}

void SoMenuWidget::onPrimaryRotationChanged(float angleIncrement)
{
	// React on user input
}

void SoMenuWidget::onSecondaryRotationChanged(float angle)
{
	// React on user input
}

void SoMenuWidget::onRotationInChanged(const SbRotation& rotation)
{
	// React on user input
}

