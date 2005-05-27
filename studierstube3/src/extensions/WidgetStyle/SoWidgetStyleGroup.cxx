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

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>


#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/widgets/SoButtonKit.h>
#include <stbapi/widgets/SoTooltipKit.h>
#include <stbapi/widgets/SoLabelKit.h>

#include "SoWidgetStyleGroup.h"
#include "SoWidgetStyleApply.h"


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



SO_NODE_SOURCE(SoWidgetStyleGroup);


/**
* the open inventor initialization
*/
void SoWidgetStyleGroup::initClass()
{
    SO_NODE_INIT_CLASS(SoWidgetStyleGroup, SoNode, "Node");
}

/**
* the constructor inits all fields
*/
SoWidgetStyleGroup::SoWidgetStyleGroup()
{
	SO_NODE_CONSTRUCTOR(SoWidgetStyleGroup);
			
//	SO_NODE_ADD_FIELD(applyToClass, (""));
	SO_NODE_ADD_FIELD(styles, (NULL));
	SO_NODE_ADD_FIELD(styleName, (""));
	styles.setNum(0);

//	SO_NODE_ADD_FIELD(parentStyle, (NULL));

	setUpConnections(TRUE, TRUE);
}

// ---------------------------------------------------------------

SoWidgetStyleGroup::SoWidgetStyleGroup(SoWidgetStyleGroup *in)
{
	
	SO_NODE_CONSTRUCTOR(SoWidgetStyleGroup);
			
//	SO_NODE_ADD_FIELD(applyToClass, (""));
	SO_NODE_ADD_FIELD(styles, (NULL));
	SO_NODE_ADD_FIELD(styleName, (""));

	styles.setNum(0);

//	SO_NODE_ADD_FIELD(parentStyle, (NULL));
	if (in != NULL)
	{
	//	applyToClass.setValue(in->applyToClass.getValue());
		styles.setValues(0,in->styles.getNum(),in->styles.getValues(0));
		styleName.setValue(in->styleName.getValue());
	//	parentStyle.setValue(in->parentStyle.getValue());
	}
	setUpConnections(TRUE, TRUE);

}
// ---------------------------------------------------------------
// called by SoApplyWidgetStyleAction

void SoWidgetStyleGroup::applyStyle(SoBehaviorKit *widget)
{
	SoWidgetStyleGroup *style;
	
	style = getDirectStyleForClass(widget);

	if (style != NULL)
	{
		// moved this function to separate class
		SoWidgetStyleApply::applyStyleToWidget(style, widget, 0);
	}
}

// ---------------------------------------------------------------

SoNode *SoWidgetStyleGroup::getTemplateForIndex(int index, SoType &type)
{
	if (styles.getNum() == 0) return NULL;
	int idx = index % styles.getNum();
	if (!styles[idx]->getTypeId().isDerivedFrom(type))
	{
	WARNING_CODE(
		printf("SoWidgetStyleGroup::getTemplateForIndex ERROR: getStyle for index [%d] - styles[%d] is [%s] and not derived from [%s]\n",
			index, 
			idx,
			styles[idx]->getTypeId().getName().getString(),
			type.getName().getString());
		);
		return NULL;
	}
	return styles[idx];
}

// ---------------------------------------------------------------
/*
SoWidgetStyleGroup *SoWidgetStyleGroup::getStyleForClass(SbName className)
{
	DEBUG_CODE(printf("getStyleForClass: [in %s] find style for [%s] \n",applyToClass.getValue().getString(),className.getString())) ;
	const char * toclass = applyToClass.getValue().getString();
	SoWidgetStyleGroup *retStyle = NULL;
		
	if (applyToClass.getValue() == className.getString())
	{
		DEBUG_CODE(printf("getStyleForClass: found \n")) ;
		return this;
	}
	
	return NULL;
}
*/

// ---------------------------------------------------------------

SoWidgetStyleGroup *SoWidgetStyleGroup::getDirectStyleForClass(SoBehaviorKit *widget)
{
	DEBUG_CODE(printf("getDirectStyleForClass: [for %s] find style with name [%s] \n",widget->getTypeId().getName().getString(), widget->styleName.getValue().getString())) ;
	const char * toclass = widget->getTypeId().getName().getString();
	SoWidgetStyleGroup *retStyle = NULL;

	assert(styles.getNum() > 0);

	SoNode *firstElm = styles[0];

	// try if this is the right styleGroup (recursive call from for loop)
	// this is true if the first element is of the same type as the widget
	// return the full SoWidgetStyleGroup (this)
	if (firstElm->getTypeId() == widget->getTypeId())
	{
		if (styleName.getValue() == widget->styleName.getValue())
		{
			DEBUG_CODE(printf("getDirectStyleForClass: found style [for %s] with name [%s] \n",widget->getTypeId().getName().getString(), widget->styleName.getValue().getString())) ;
			return this;	
		}
		else
		{
			DEBUG_CODE(printf("getDirectStyleForClass: class found, but name is wrong style[%s] != widget[%s] \n", styleName.getValue().getString(), widget->styleName.getValue().getString()));
			// do not proceed into sub styles here. try above instead (recursive call)
			return NULL;
		}
	}
	
	// go through all styles (which are SoWidgetStyleGroup again) and find the style there
	for (int i=0; i<styles.getNum(); i++)
	{
		assert(styles[i] != NULL);
		// if sub styles are groups, try to find it there
		// leaf first approach, ...
		if (styles[i]->isOfType(SoWidgetStyleGroup::getClassTypeId()))
		{
			retStyle = ((SoWidgetStyleGroup *)(styles[i]))->getDirectStyleForClass(widget);
			// recursion end condition
			if (retStyle != NULL)	return retStyle;
		}
	}
	

	// there is nothing more to do, so return NULL
	DEBUG_CODE(printf("getDirectStyleForClass: not found \n")) ;
	return NULL;
/*
	// try if this is the right styleGroup (recursive call from for loop)
	if ()
	{
		DEBUG_CODE(printf("getDirectStyleForClass: found \n")) ;
		if (widget->styleName == styleName)
		{
			DEBUG_CODE(printf("getDirectStyleForClass: found, styleName used = [%s]\n", styleName.getString())) ;
			return this;
		}
		else
		{
			DEBUG_CODE(printf("getDirectStyleForClass: class found, but name is wrong [%s]!=[%s] \n", widget->styleName.getString(),styleName.getString())) ;
		}
	}

	// go into subgroups, and try to find a style for the type there

	
	// still not found ???
	// try to follow the parentStyle pointer
	// do not use parentStyle 
*/
  /*
	if (parentStyle.getValue() != NULL)
	{
		assert(parentStyle.getValue()->isOfType(SoWidgetStyleGroup::getClassTypeId()));
		DEBUG_CODE(printf("getSubStyleForClass: try parent \n")) ;

		// but skip this as it would end in an endless loop
		return ((SoWidgetStyleGroup *)parentStyle.getValue())->getDirectStyleForClass(className, this);
	}
*/

	// if parentStyle is null there is nothing more to do so return NULL
}

// ---------------------------------------------------------------

void SoWidgetStyleGroup::setStyleForClass(SbName className)
{
//	applyToClass.setValue(className);	
}

// ---------------------------------------------------------------

void SoWidgetStyleGroup::addStyle(SoWidgetStyleGroup *style)
{
	styles.set1Value(styles.getNum(), style);
}

// ---------------------------------------------------------------

void SoWidgetStyleGroup::setStyleInGroup(SoWidgetStyleGroup *style)
{
//TODO find if same style is already set and print warning !!!

/*	bool found = false;
	for (int i=0; !found && i< styles.getNum(); i++)
	{
		SoNode *style = ;
		if (styles[i]->isOfType(SoWidgetStyleGroup::getClassTypeId()))
		{
			if (((SoWidgetStyleGroup*)styles[i])->applyToClass.getValue() == style->applyToClass.getValue())
			{
				styles.set1Value(i, style);
				found = true;
			}
		}
	}

	if (!found)
	{
	}
*/
	addStyle(style);
}

// ---------------------------------------------------------------
SoWidgetStyleGroup *SoWidgetStyleGroup::getTemplateGroupForType(SoType &type)
{
	DEBUG_CODE(printf("getTemplateGroupForType: for [%s] \n",type.getName().getString())) ;
	assert(styles.getNum() > 0);
	for (int i=0; i< styles.getNum(); i++)
	{
		if (styles[i]->isOfType(SoWidgetStyleGroup::getClassTypeId()))
		{
			// now we are in a new group,
			// check if first element is of searched type
			SoWidgetStyleGroup *subGroup =(SoWidgetStyleGroup*) styles[i];
			assert(subGroup->styles.getNum() > 0);

			SoNode *firstElm = subGroup->styles[0];
			
			DEBUG_CODE(printf("getTemplateGroupForType: found group for [%s] \n",firstElm->getTypeId().getName().getString())) ;

			if (firstElm->getTypeId().isDerivedFrom(type))
			{
				return subGroup;
			}
		}
	}

	DEBUG_CODE(printf("getTemplateGroupForType: not found return NULL \n")) ;

	return NULL;
}


SoNode *SoWidgetStyleGroup::getTemplateForClass(SoNode *baseNode)
{
/*
	assert(baseNode != NULL);

	const char *baseNodeType = baseNode->getTypeId().getName().getString();
	for (int i=0; i< styles.getNum(); i++)
	{
		if (((SoNode*)styles[i])->getTypeId().getName().getString() == baseNodeType)
		{
			return (SoNode*)styles[i];
		}
	}
*/
	return NULL;
}


// ---------------------------------------------------------------

SbBool SoWidgetStyleGroup::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways) // && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
		// SoBehaviorKit::setUpConnections(onOff, doItAlways); 
		// no base class implementation 
        // setup listeners
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		// no base class implementation
        //SoBehaviorKit::setUpConnections(onOff, doItAlways);
    }
//    return !(connectionsSetUp = onOff);
	return !(onOff);
}

// ---------------------------------------------------------------

SoWidgetStyleGroup::~SoWidgetStyleGroup()
{

}

// ---------------------------------------------------------------

