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
/** source file for SoWidgetStyleGroup class.
*
* @author Thomas Psik
*
* $Id: SoWidgetStyleGroup.cxx 4030 2004-11-22 10:47:09Z reitmayr $
* @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoAsciiText.h>

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>

#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/widgets/SoButtonKit.h>
#include <stbapi/widgets/SoTooltipKit.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/widgets/SoSlider.h>
#include <stbapi/widgets/SoSimpleSlider.h>
#include <stbapi/widgets/SoPushButton.h>

#include <stbapi/widgets/SoIncrementalSlider.h>
#include <stbapi/widgets/SoLabelListBox.h>


#include "SoWidgetStyleApply.h"
#include "SoWidgetStyleGroup.h"


#define _DEBUG_STYLE_ACTION

#define NO_WARNINGS
#if defined(_DEBUG_STYLE_ACTION) && defined(_DEBUG)

#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif

#if defined(NO_WARNINGS)
#define WARNING_CODE(a) ;
#else
#define WARNING_CODE(a) a;
#endif

// helper macro

#define SET_PART_IF_AVAILABLE(widg, templ, str)  {SoSeparator *geom = SO_CHECK_PART(templ, str, SoSeparator); if (geom != NULL) widg->setPart(str, geom ); }

// ------------------------------------------------------------------------------------------------------------

void SoWidgetStyleApply::applyStyleToWidget(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index)
{
	assert(widget != NULL);
	if (widget->getTypeId().isDerivedFrom(SoBehaviorKit::getClassTypeId()))
	{
		applyStyleToBehaviorKit(style, widget, index);
	}
	if (widget->getTypeId().isDerivedFrom(SoButtonKit::getClassTypeId()))
	{
		applyStyleToButtonKit(style, widget, index);
	}
	if (widget->getTypeId().isDerivedFrom(SoSlider::getClassTypeId()))
	{
		applyStyleToSlider(style, widget, index);
	}
	if (widget->getTypeId().isDerivedFrom(SoSimpleSlider::getClassTypeId()))
	{
		applyStyleToSimpleSlider(style, widget, index);
	}
	if (widget->getTypeId().isDerivedFrom(SoIncrementalSlider::getClassTypeId()))
	{
		applyStyleToIncrementalSlider(style, widget, index);
	}
	if (widget->getTypeId().isDerivedFrom(SoLabelListBox::getClassTypeId()))
	{
		applyStyleToLabelListBox(style, widget, index);
	}
	widget->touch();

}

// ------------------------------------------------------------------------------------------------------------

SoNode *SoWidgetStyleApply::getStyleNodeForIndex(SoWidgetStyleGroup *style, int index, SoType &type)
{
	assert(style != NULL);
	assert(style->styles.getNum() > 0);

	int idx = index % style->styles.getNum();

	SoNode *styleNode = style->styles[idx];
	
	assert (styleNode);

	if (!styleNode->getTypeId().isDerivedFrom(type))
	{
		printf("SoButtonKit::applyStyleToWidget: Major problem !! Style is not of type %s !!\n", type.getName().getString());
		return NULL;
	}

	return styleNode;
}

// ------------------------------------------------------------------------------------------------------------

void SoWidgetStyleApply::applyStyleToLabelKit(SoLabelKit *label, SoLabelKit *labelTempl)
{
	if (label == NULL) return;
	if (labelTempl == NULL) return;
	SoNode *testPart = NULL;
	
	// check if template label has parts set and replace then in current label
	// TODO widget style !!
	// only if the current label is not a SoAsciiText, set the label from the template
	// else do not override with template !!
	testPart = labelTempl->getPart("contents", FALSE);
	if (testPart != NULL )
	{
		if (testPart->isOfType(SoSeparator::getClassTypeId() ) )
		{
			if (((SoSeparator*)testPart)->getNumChildren() > 0)
			{
				SoNode *child = ((SoSeparator*)testPart)->getChild(0);
				if (child!= NULL && child->isOfType(SoAsciiText::getClassTypeId()))
				{
					SoNode *contentsValue = SO_GET_PART(label,"contents", SoNode);
					if (contentsValue->isOfType(SoSeparator::getClassTypeId()))
					{	
						SoSeparator *sep = (SoSeparator *)contentsValue;
						if (!sep->getChild(0)->isOfType(SoAsciiText::getClassTypeId()))
							label->setPart("contents", testPart);
					}
					else
						label->setPart("contents", testPart);
				}
				else
					label->setPart("contents", testPart);
			}
		}
	}
	
	 

	testPart = labelTempl->getPart("appearance.font", FALSE);
	if (testPart != NULL)
	{
		label->setPart("appearance.font", testPart);
	}

	testPart = labelTempl->getPart("appearance.material", FALSE);
	if (testPart != NULL)
	{
		label->setPart("appearance.material", testPart);
	}

	testPart = labelTempl->getPart("appearance.texture2", FALSE);
	if (testPart != NULL)
	{
		label->setPart("appearance.texture2", testPart);
	}

	testPart = labelTempl->getPart("transform", FALSE);
	if (testPart != NULL)
	{
		label->setPart("transform", testPart->copy());
	}

	if (labelTempl->resizeToWidget.getValue() != SoLabelKit::DEFAULT)
		label->resizeToWidget.setValue(labelTempl->resizeToWidget.getValue());
	
	if (labelTempl->uniformScale.getValue() != SoLabelKit::DEFAULT)
		label->uniformScale.setValue(labelTempl->uniformScale.getValue());

	DEBUG_CODE(	printf("######## checking translations from template transNum %d\n", label->translations.getNum()));
	if (label->translations.getNum() == 3)
	{
		if (label->translations[2] == SbVec3f(-1,-1,-1))
		{
			DEBUG_CODE(	printf("######## setting translations from template\n"));
			label->translations.copyFrom(labelTempl->translations);
		}
	}
}

// ------------------------------------------------------------------------------------------

void SoWidgetStyleApply::applyStyleToTooltipKit(SoTooltipKit *tooltip, SoTooltipKit *tooltipTempl)
{
	if (tooltipTempl == NULL) return;
	SoNode *testPart = NULL;
	
	if (tooltip->getMasterTooltip() != NULL) 
	{
		applyStyleToTooltipKit(tooltip->getMasterTooltip(), tooltipTempl);
		return;
	}
	// check if template label has parts set and replace then in current label
	// TODO widget style !!
	// only if the current label is not a SoAsciiText, set the label from the template
	// else do not override with template !!
	testPart = tooltipTempl->getPart("label", FALSE);
	if (testPart != NULL )
	{
		SoLabelKit * labelTempl = (SoLabelKit * )testPart;
		SoLabelKit * label = (SoLabelKit * )tooltip->getPart("label", FALSE);
		if (label !=  NULL)
		{
			applyStyleToLabelKit(label, labelTempl); //->adaptToTemplate(labelTempl);
		}
	}

	testPart = tooltipTempl->getPart("plane.appearance.font", FALSE);
	if (testPart != NULL)
	{
		tooltip->setPart("plane.appearance.font", testPart);
	}

	testPart = tooltipTempl->getPart("plane.appearance.material", FALSE);
	if (testPart != NULL)
	{
		tooltip->setPart("plane.appearance.material", testPart);
	}

	testPart = tooltipTempl->getPart("plane.transform", FALSE);
	if (testPart != NULL)
	{
		tooltip->setPart("plane.transform", testPart->copy());
	}
}

// ------------------------------------------------------------------------------------------------------------

void SoWidgetStyleApply::applyStyleToBehaviorKit(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index)
{
	assert(widget != NULL);
	SoBehaviorKit *templateBehave = (SoBehaviorKit *)getStyleNodeForIndex(style, index, SoBehaviorKit::getClassTypeId());

	// if no style is found, do nothing (warning in debug mode)
	if (templateBehave == NULL)	return;
	
	if (widget->labelPlacing.getValue() == SoBehaviorKit::NONE)
	{
		widget->labelPlacing.setValue(templateBehave->labelPlacing.getValue());
		// function to apply labelPlacing is called 
	}
	
	// check label
	SoLabelKit *label= SO_CHECK_PART(widget, "label", SoLabelKit);
	if (label != NULL)
	{
		SoLabelKit *labelTempl = SO_CHECK_PART(templateBehave, "label", SoLabelKit);
		if (labelTempl != NULL)
			applyStyleToLabelKit(label, labelTempl); //->adaptToTemplate(labelTempl);
		label->touch();
	}

	// check tooltip
	SoTooltipKit *tooltip= SO_CHECK_PART(widget, "tooltip", SoTooltipKit);
	if (tooltip != NULL)
	{
		SoTooltipKit *tooltipTempl = SO_CHECK_PART(templateBehave, "tooltip", SoTooltipKit);
		if (tooltipTempl != NULL)
			applyStyleToTooltipKit(tooltip, tooltipTempl); //->adaptToTemplate(labelTempl);
		//	tooltip->adaptToTemplate(tooltipTempl);
	}

	// check tooltipDisabled
    tooltip = SO_CHECK_PART(widget, "tooltipDisabled", SoTooltipKit);
	if (tooltip != NULL)
	{
		SoTooltipKit *tooltipTempl = SO_CHECK_PART(templateBehave, "tooltipDisabled", SoTooltipKit);;
		if (tooltipTempl != NULL)
			applyStyleToTooltipKit(tooltip, tooltipTempl); //->adaptToTemplate(labelTempl);
		//	tooltip->adaptToTemplate(tooltipTempl);
	}

	// now update values ??
	widget->labelPlacing.touch();

	//	widget->adaptToLabelPlacing();
}


void SoWidgetStyleApply::applyStyleToButtonKit(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index)
{
	SoButtonKit *templateButton = (SoButtonKit *)getStyleNodeForIndex(style, index, SoButtonKit::getClassTypeId());
	
	// if no style is found, do nothing (warning in debug mode)
	if (templateButton==NULL)	return;

	// now replace geometry with WidgetStyle
	SET_PART_IF_AVAILABLE(widget, templateButton, "onGeometry");
	SET_PART_IF_AVAILABLE(widget, templateButton, "offGeometry");
	SET_PART_IF_AVAILABLE(widget, templateButton, "highlightedOnGeometry");
	SET_PART_IF_AVAILABLE(widget, templateButton, "highlightedOffGeometry");
	SET_PART_IF_AVAILABLE(widget, templateButton, "externalOnGeometry");
	SET_PART_IF_AVAILABLE(widget, templateButton, "disabledOnGeometry");
	SET_PART_IF_AVAILABLE(widget, templateButton, "disabledOffGeometry");

	// no special cases for different buttons
	//if (widget->getTypeId()->isDerivedFrom(SoPushButton::getClassTypeId())) { }
	//else if (widget->getTypeId()->isDerivedFrom(SoToggleButton::getClassTypeId())) { }
}

// --------------------------------------------------------------------------


void SoWidgetStyleApply::applyStyleToSlider(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index)
{
	SoSlider *templateSlider = (SoSlider  *)getStyleNodeForIndex(style, index, SoSlider::getClassTypeId());
	
	// if no style is found, do nothing (warning in debug mode)
	if (templateSlider==NULL)	return;

	SET_PART_IF_AVAILABLE(widget, templateSlider, "normalGeometry");
	SET_PART_IF_AVAILABLE(widget, templateSlider, "highlightedGeometry");
	SET_PART_IF_AVAILABLE(widget, templateSlider, "draggedGeometry");
	SET_PART_IF_AVAILABLE(widget, templateSlider, "disabledGeometry");
	SET_PART_IF_AVAILABLE(widget, templateSlider, "staticGeometry");
	SET_PART_IF_AVAILABLE(widget, templateSlider, "staticGeometryHighlight");
	SET_PART_IF_AVAILABLE(widget, templateSlider, "staticGeometryDisabled");
}
// --------------------------------------------------------------------------

void SoWidgetStyleApply::applyStyleToSimpleSlider(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index)
{
	SoSimpleSlider *templateSlider = (SoSimpleSlider  *)getStyleNodeForIndex(style, index, SoSimpleSlider::getClassTypeId());
	
	// if no style is found, do nothing (warning in debug mode)
	if (templateSlider==NULL)	return;

	SoSimpleSlider *slider = (SoSimpleSlider *)widget;
	
	// set values
	if (templateSlider->scaleDraggerPath.getValue() > 0.0)
		slider->scaleDraggerPath.setValue(templateSlider->scaleDraggerPath.getValue());
}

// ---------------------------------------------------------------
void SoWidgetStyleApply::applyStyleToIncrementalSlider(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index)
{
	SoIncrementalSlider *templateSlider = (SoIncrementalSlider *)getStyleNodeForIndex(style, index, SoIncrementalSlider::getClassTypeId());

	// if no style is found, do nothing (warning in debug mode)
	if (templateSlider==NULL)	return;

	SoIncrementalSlider *slider = (SoIncrementalSlider *)widget;
	
	// apply design values 	
	slider->buttonSpace.setValue(templateSlider->buttonSpace.getValue());

	// incremental slider style has  sub style groups, which can consist of 
	// SoSimpleSlider && (2) SoPushButtons
	SoWidgetStyleGroup *styleGroup = NULL;
	
	styleGroup = style->getTemplateGroupForType(SoPushButton::getClassTypeId());
	if (styleGroup != NULL)	
	{
		// we have now a group with SoPushButton templates
		SoPushButton *button = SO_GET_PART(slider, "up", SoPushButton);
		assert(button != NULL);
		
		applyStyleToWidget(styleGroup, button, 0);

		button = SO_GET_PART(slider, "down", SoPushButton);
		assert(button != NULL);

		applyStyleToWidget(styleGroup, button, 1);
	}

	styleGroup = style->getTemplateGroupForType(SoSimpleSlider::getClassTypeId());
	if (styleGroup != NULL)	
	{
		SoSimpleSlider *slid = SO_GET_PART(slider, "slider", SoSimpleSlider);
		assert(slid != NULL);
		applyStyleToWidget(styleGroup, slid, 0);

	}
}

// ---------------------------------------------------------------
void SoWidgetStyleApply::applyStyleToLabelListBox(SoWidgetStyleGroup *style, SoBehaviorKit *widget, int index)
{
	SoLabelListBox *templateList = (SoLabelListBox *)getStyleNodeForIndex(style, index, SoLabelListBox::getClassTypeId());

	// if no style is found, do nothing (warning in debug mode)
	if (templateList==NULL)	return;

	SoLabelListBox *listBox = (SoLabelListBox *)widget;
	
	// apply design values 
	// possible properties
	if (listBox->navigationPlacing.getValue() == SoLabelListBox::NONE)
		listBox->navigationPlacing.setValue(templateList->navigationPlacing.getValue());

	if (listBox->navigationDisplay.getValue() == SoLabelListBox::_NONE)
		listBox->navigationDisplay.setValue(templateList->navigationDisplay.getValue());
	//properties not set by Style
	/*
	if (listBox->spacingWidth.getValue())
	if (listBox->spacingDepth.getValue())
	if (listBox->startEndSpacingWidth.getValue())
	if (listBox->startEndSpacingDepth.getValue())
	if (listBox->navigationButtonToSliderRatio.getValue())
	if (listBox->interval.getValue())
	*/

	SET_PART_IF_AVAILABLE(widget, templateList, "backplane");

	// SoLabelListBox style has sub style groups, which can consist of 
	// SoIncrementalSlider && (n) SoToggleButtons
	SoWidgetStyleGroup *styleGroup = NULL;
	
	// find SoIncrementalSlider style and apply
	styleGroup = style->getTemplateGroupForType(SoIncrementalSlider::getClassTypeId());
	if (styleGroup != NULL)	
	{
		// we have now a group with SoPushButton templates
		SoIncrementalSlider *slider = SO_GET_PART(widget, "slider", SoIncrementalSlider);
		assert(slider != NULL);
		DEBUG_CODE(printf("in applyStyleToLabelListBox appling style for SoIncrementalSlider\n"));
		applyStyleToWidget(styleGroup, slider, 0);
	}

	// find SoToggleButtons style and apply
	styleGroup = style->getTemplateGroupForType(SoToggleButton::getClassTypeId());
	if (styleGroup != NULL)	
	{
		// if there is only 1 style do not set the pointer,
		// no need for re-appplying the style to the buttons if the displayIndex changes
	//	if (styleGroup->styles.getNum() > 1)
			listBox->buttonStyles = styleGroup;
	//	else
	//		listBox->buttonStyles = NULL;

		SoToggleButton *button = NULL;
		SoNodeKitListPart *buttonList = SO_GET_PART(listBox, "buttons", SoNodeKitListPart);
		assert(buttonList != NULL);
		int counter = 0;
		for(int i = 0; i < buttonList->getNumChildren(); i++ )
		{
			SoSeparator * sep = (SoSeparator *)buttonList->getChild(i);
			if (sep->getNumChildren() > 1) 
			{
				SoNode * candidate = sep->getChild(1);
				assert( candidate != NULL );
				if( candidate->isOfType( SoToggleButton::getClassTypeId()))
				{
					button = (SoToggleButton *)candidate;
					DEBUG_CODE(printf("in applyStyleToLabelListBox appling style for SoToggleButtons idx [%d]\n",counter));
					applyStyleToWidget(styleGroup, button, counter);
					counter ++;
				}
			}
		}
	}
	
}
// ---------------------------------------------------------------
