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
/** Implementation of class SoWidgetSelectionFromPuc
  *
  * @author Valerie Maquil
  *
  * $Id: SoWidgetSelectionFromPuc.cxx 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/engines/SoCalculator.h>

#include <stbapi/widgets/SoSimpleSlider.h>
#include <stbapi/widgets/SoIncrementalSlider.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/widgets/SoToggleButton.h>
#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/widgets/SoTextListBox.h>
#include <stbapi/widgets/SoLabelListBox.h>
#include <stbapi/widgets/SoTextBox.h>
#include <stbapi/widgets/SoTooltipKit.h>

#include "SoWidgetSelectionFromPuc.h"

#include <extensions/PUC/SoPucActiveIfNode.h>

#include <iostream>

using namespace std;

const SbString SoWidgetSelectionFromPuc::emptyString = "";
//----------------------------------------------------------------------------

SoWidgetSelectionFromPuc::SoWidgetSelectionFromPuc()
{
	 
    
}

//----------------------------------------------------------------------------

SoWidgetSelectionFromPuc::~SoWidgetSelectionFromPuc()
{

}

//----------------------------------------------------------------------------

SoBehaviorKit *SoWidgetSelectionFromPuc::getPushButtonForCommand(SoPucCommand *state, int size)
{
	SoPushButton *widget = new SoPushButton();
	
	widget->triggerIn.connectFrom(&state->trigger);
	state->triggerIn.connectFrom(&widget->triggerOut);

	SoLabelKit *widgetLabel = SO_GET_PART(widget, "label", SoLabelKit);
	SbString label = getLabelIndex(state->labels, size);
	widgetLabel->text.setValue(label);

	addActiveIfAndTooltip(widget, state);

	return widget;
}

//----------------------------------------------------------------------------
SoBehaviorKit *SoWidgetSelectionFromPuc::getToggleButtonForStateBool(SoPucStateBool *state, int size)
{
	SoToggleButton *widget = new SoToggleButton();
	
	widget->onIn.connectFrom(&state->value);
	widget->setUpConnections(TRUE, TRUE);
	widget->onIn.setValue(state->value.getValue());
	widget->on.setValue(state->value.getValue());

	state->value.connectFrom(&widget->on);

	SoLabelKit *widgetLabel = SO_GET_PART(widget, "label", SoLabelKit);
	SbString label = getLabelIndex(state->labels, size);
	widgetLabel->text.setValue(label);
	
	addActiveIfAndTooltip(widget, state);
	
	return widget;
}

//----------------------------------------------------------------------------

SoBehaviorKit *SoWidgetSelectionFromPuc::getSliderForStateFixed(SoPucStateFixed *state, int size)
{
	SoIncrementalSlider *widget = new SoIncrementalSlider();

	widget->cropValueToIncrement.setValue(TRUE);
	
	widget->minValue.connectFrom(&state->min);
	widget->maxValue.connectFrom(&state->max);
	widget->increment.connectFrom(&state->incr);
	widget->valueIn.connectFrom(&state->value);

	widget->setUpConnections(TRUE, TRUE);

	// have to set alpha to correctly initialise slider !!
	widget->alphaIn.setValue((state->value.getValue() - state->min.getValue()) / (state->max.getValue() - state->min.getValue()));
	state->value.connectFrom(&widget->value);

	SoLabelKit *widgetLabel = SO_GET_PART(widget, "label", SoLabelKit);
	SbString label = getLabelIndex(state->labels, size);
	widgetLabel->text.setValue(label);

	addActiveIfAndTooltip(widget, state);
	return widget;
}

//----------------------------------------------------------------------------

SoBehaviorKit *SoWidgetSelectionFromPuc::getSliderForStateFloat(SoPucStateFloat *state, int size)
{
	SoIncrementalSlider *widget = new SoIncrementalSlider();
	
	widget->minValue.connectFrom(&state->min);
	widget->maxValue.connectFrom(&state->max);

	widget->increment.setValue(state->max.getValue()-state->min.getValue() / 100.0);
	widget->valueIn.connectFrom(&state->value);
	widget->setUpConnections(TRUE, TRUE);
	
	// have to set alpha to correctly initialise slider !!
	widget->alphaIn.setValue((state->value.getValue() - state->min.getValue()) / (state->max.getValue() - state->min.getValue()));
	
	state->value.connectFrom(&widget->value);

	SoLabelKit *widgetLabel = SO_GET_PART(widget, "label", SoLabelKit);
	SbString label = getLabelIndex(state->labels, size);
	widgetLabel->text.setValue(label);

	addActiveIfAndTooltip(widget, state);

	return widget;

}

//----------------------------------------------------------------------------

SoBehaviorKit *SoWidgetSelectionFromPuc::getSliderForStateInt(SoPucStateInt *state, int size)
{
	SoIncrementalSlider *widget = new SoIncrementalSlider();
	
	widget->cropValueToIncrement.setValue(TRUE);

	widget->minValue.connectFrom(&state->min);
	widget->maxValue.connectFrom(&state->max);
	widget->increment.connectFrom(&state->incr);
	widget->valueIn.connectFrom(&state->value);
	
	widget->setUpConnections(TRUE, TRUE);

	// have to set alpha to correctly initialise slider !!
	float alpha = (float)(state->value.getValue() - state->min.getValue()) / (state->max.getValue() - state->min.getValue());
	widget->alphaIn.setValue(alpha);
	state->value.connectFrom(&widget->value);

	SoLabelKit *widgetLabel = SO_GET_PART(widget, "label", SoLabelKit);
	SbString label = getLabelIndex(state->labels, size);
	widgetLabel->text.setValue(label);

	addActiveIfAndTooltip(widget, state);
	
	return widget;
}

//----------------------------------------------------------------------------

SoBehaviorKit *SoWidgetSelectionFromPuc::getTextBoxForStateString(SoPucStateString *state, int size)
{
	SoTextBox *widget = new SoTextBox ();
	
	widget->text.connectFrom(&state->value);
	state->value.connectFrom(&widget->text);

	widget->setUpConnections(TRUE, TRUE);

	SoLabelKit *widgetLabel = SO_GET_PART(widget, "label", SoLabelKit);
	SbString label = getLabelIndex(state->labels, size);
	widgetLabel->text.setValue(label);

	addActiveIfAndTooltip(widget, state);

	return widget;
}


//----------------------------------------------------------------------------

SoBehaviorKit *SoWidgetSelectionFromPuc::getTextListBoxForStateEnumerated(SoPucStateEnumerated *state, int numCols, int numRows, int navPlace, int size)
{
	
	SoTextListBox *widget = new SoTextListBox();
	
//	widget->noneSelectionAllowed.setValue(FALSE);
//	widget->multipleSelections.setValue(FALSE);

	SoLabelKit *widgetLabel = SO_GET_PART(widget, "label", SoLabelKit);
	SbString label = getLabelIndex(state->labels, size);
	widgetLabel->text.setValue(label);
	
	int num = state->valueLabels.getNum();
	widget->numOfCols.setValue(numCols);
	widget->numOfRows.setValue(numRows);
	widget->navigationPlacing.setValue(navPlace);
	widget->values.setValues(0, num, state->valueLabels.getValues(0));
	widget->selectedIndicesIn.connectFrom(&state->value);
	widget->selectedIndicesIn.setValue(state->value.getValue());
	widget->setUpConnections(TRUE, TRUE);

	state->value.connectFrom(&widget->selectedIndices);

	addActiveIfAndTooltip(widget, state);

	return widget;
}

//----------------------------------------------------------------------------

SoBehaviorKit *SoWidgetSelectionFromPuc::getTextListBoxForStateInt(SoPucStateInt *state, int numCols, int numRows, int navPlace, int size)
{
	
	SoTextListBox *widget = new SoTextListBox();
	
	SoCalculator *calc = new SoCalculator();
	calc->a.connectFrom(&state->value);
	calc->b.connectFrom(&state->incr);
	calc->c.setValue(state->min.getValue());
	calc->expression.setValue("oa = (a-c)/b");

	widget->selectedIndicesIn.connectFrom(&calc->oa);

	SoCalculator *calc2 = new SoCalculator();
	calc2->a.connectFrom(&widget->selectedIndices);
	calc2->b.connectFrom(&state->incr);
	calc2->c.setValue(state->min.getValue());
	calc2->expression.setValue("oa = a*b+c");

	state->value.connectFrom(&calc2->oa);

	SoLabelKit *widgetLabel = SO_GET_PART(widget, "label", SoLabelKit);
	SbString label = getLabelIndex(state->labels, size);
	widgetLabel->text.setValue(label);
	
	widget->numOfCols.setValue(numCols);
	widget->numOfRows.setValue(numRows);
	widget->navigationPlacing.setValue(navPlace);

	int num = (state->max.getValue() - state->min.getValue())/state->incr.getValue()+1;
	int value = state->min.getValue();

	for (int i = 0; i<num; i++)
	{
		widget->values.set1Value(i,value);
		value+=state->incr.getValue();
	}
	
	addActiveIfAndTooltip(widget, state);

	return widget;
}

//----------------------------------------------------------------------------

SoBehaviorKit *SoWidgetSelectionFromPuc::getTextListBoxForStateFixed(SoPucStateFixed *state, int numCols, int numRows, int navPlace, int size)
{
	
	SoTextListBox *widget = new SoTextListBox();
	
	SoCalculator *calc = new SoCalculator();
	calc->a.connectFrom(&state->value);
	calc->b.connectFrom(&state->incr);
	calc->c.setValue(state->min.getValue());
	calc->expression.setValue("oa = (a-c)/b");

	widget->selectedIndicesIn.connectFrom(&calc->oa);

	SoCalculator *calc2 = new SoCalculator();
	calc2->a.connectFrom(&widget->selectedIndices);
	calc2->b.connectFrom(&state->incr);
	calc2->c.setValue(state->min.getValue());
	calc2->expression.setValue("oa = a*b+c");

	state->value.connectFrom(&calc2->oa);

	SoLabelKit *widgetLabel = SO_GET_PART(widget, "label", SoLabelKit);
	SbString label = getLabelIndex(state->labels, size);
	widgetLabel->text.setValue(label);
	
	widget->numOfCols.setValue(numCols);
	widget->numOfRows.setValue(numRows);
	widget->navigationPlacing.setValue(navPlace);

	int num = (state->max.getValue() - state->min.getValue())/state->incr.getValue()+1;
	float value = state->min.getValue();

	for (int i = 0; i<num; i++)
	{
		char buffer [20];
		gcvt (value,5,buffer);

		widget->values.set1Value(i,buffer);
		value+=state->incr.getValue();
	}

	addActiveIfAndTooltip(widget, state);

	return widget;
}
//----------------------------------------------------------------------------

void SoWidgetSelectionFromPuc::addActiveIfAndTooltip(SoBehaviorKit *widget, SoPucElementBase *state)
{

	if (state->explanation.getNum() > 0 && state->explanation[0].getLength() > 0)
	{
		SoTooltipKit * tooltip = new SoTooltipKit();
		tooltip->text.connectFrom(&state->explanation);
		tooltip->timeOut.setValue(1.2);

		widget->setPart("tooltip", tooltip);
	}
	SoNode *activeNode = state->getPart("activeIf", FALSE);
	if (activeNode != NULL)
	{
		SoPucActiveIfNode * activeIf = (SoPucActiveIfNode *)activeNode;
		if (activeIf->explanation.getNum() > 0 && activeIf->explanation[0].getLength() > 0)
		{
			SoTooltipKit * tooltipDisabled = new SoTooltipKit();
			tooltipDisabled->text.connectFrom(&activeIf->explanation);
			tooltipDisabled->timeOut.setValue(0.7);
			widget->setPart("tooltipDisabled", tooltipDisabled);
		}
		widget->enable.connectFrom(&activeIf->active);
	}
	widget->adaptToTooltips();

}

//----------------------------------------------------------------------------

const SbString &SoWidgetSelectionFromPuc::getLabelIndex(const SoMFString &labels, int size)
{

	int num = labels.getNum();
	int index = -1;
	int length;

	if (size == SMALL_LABEL) 
	{
		length = 10000000;
		for (int i=0; i<num; i++)
		{
			SbString label = labels[i];
			if (label.getLength() < length) 
			{
				length = label.getLength();
				index = i;
			}
		}
		return labels[index];
	}
	else if (size == LONG_LABEL) 
	{
		length = 0;
		for (int i=0; i<num; i++)
		{
			SbString label = labels[i];
			if (label.getLength() > length) 
			{
				length = label.getLength();
				index = i;
			}
		}
		return labels[index];
		
	}
	else return emptyString;
}