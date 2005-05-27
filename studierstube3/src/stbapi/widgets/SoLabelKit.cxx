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
/** source file for SoLabelKit class.
  *
  * @author Thomas Psik
  *
  * $Id: SoLabelKit.cxx 4101 2004-12-16 10:07:36Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoAsciiText.h>

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>

#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/widgets/SoLabelKit.h>

#define NO_WARNINGS
#define _DEBUG_LABEL_KIT

#if defined(_DEBUG_LABEL_KIT) && defined(_DEBUG)
#undef NO_WARNINGS
#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif

#if defined(NO_WARNINGS)
#define WARNING_CODE(a) ;
#else
#define WARNING_CODE(a) a;
#endif


#include <assert.h>

SO_KIT_SOURCE(SoLabelKit);

// ------------------------------------------------------------------------------------------

void SoLabelKit::initClass()
{
	SO_KIT_INIT_CLASS(SoLabelKit, SoWrapperKit, "WrapperKit");
}

// ------------------------------------------------------------------------------------------

SoLabelKit::SoLabelKit()
{
	lastTranslationSelected = 0;

	SO_KIT_CONSTRUCTOR(SoLabelKit);

	//---------------------------------------+------------+------+---------------+-------+---------------
	//                                       |            |      |               | right |
	//                                       |            | null |               | Sib-  | is
	//                                       |            | By   |               | ling  | public
	//                         name          | className  | Def. |  parent Name  | Name  | part
	//---------------------------------------+------------+------+---------------+-------+---------------
   	// all inherited from SoShapeKit

	SO_NODE_ADD_FIELD(text,	(""));
	SO_NODE_ADD_FIELD(translations, (0,1.05,0));


	translations.setNum(3);
	translations.set1Value(2, SbVec3f(-1,-1,-1));

	SO_NODE_DEFINE_ENUM_VALUE(triState, NO);
	SO_NODE_DEFINE_ENUM_VALUE(triState, YES);
	SO_NODE_DEFINE_ENUM_VALUE(triState, DEFAULT);
	
	SO_NODE_SET_SF_ENUM_TYPE(resizeToWidget, triState);
	SO_NODE_SET_SF_ENUM_TYPE(uniformScale, triState);

	SO_NODE_ADD_FIELD(resizeToWidget, (DEFAULT));
	SO_NODE_ADD_FIELD(uniformScale,   (DEFAULT));
	
	
	SO_KIT_INIT_INSTANCE();

//	setPart("appearance.font"	, SoBehaviorKit::getGeometryFromString("Font {size .4}  "));
	setPart("appearance.material", SoBehaviorKit::getGeometryFromString("Material { diffuseColor 0 0 0 }  "));
	
	setPart("contents"				, readMemory("AsciiText { justification CENTER }"));

	/*
	SoTransform *trans = SO_GET_ANY_PART(this, "localTransform", SoTransform);
	if (trans) trans->rotation.set("1 0 0 -1.57");
	*/

	setPart("localTransform"	, SoBehaviorKit::getGeometryFromString("Transform {rotation 1 0 0 -1.57 }"));


	setUpConnections(TRUE, TRUE);
}

// ------------------------------------------------------------------------------------------

SoLabelKit::~SoLabelKit()
{
}

// ------------------------------------------------------------------------------------------

SbBool SoLabelKit::setUpConnections( SbBool onOff, SbBool doItAlways )
{
 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoWrapperKit::setUpConnections(onOff, doItAlways);
   
	SoAsciiText *textNode = getAsciiText();
	if (textNode!= NULL)
	{
		textNode->string.connectFrom(&text);
		textNode->justification.setValue(SoAsciiText::CENTER); // TODO DELETE ??
	}
	
    setTranslation(0);
  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
    SoWrapperKit::setUpConnections(onOff, doItAlways);
  }
  return !(connectionsSetUp = onOff);
}

// ------------------------------------------------------------------------------------------

void SoLabelKit::setTranslation(int i)
{
	SoTransform* transform = SO_GET_ANY_PART(this, "transform", SoTransform);
	
	lastTranslationSelected = i;
	if (transform)
		if (i > -1 && i < translations.getNum() )
		{
			SoTransform *transf = SO_GET_PART( this, "localTransform", SoTransform);
			// if in opposite direction that widget
			SbVec3f scaleF = transf->scaleFactor.getValue();

			SbVec3f transScal = translations[i];
			transScal[1] = transScal[1] *scaleF[2];

			transform->translation.setValue(transScal);
		}
		else
			lastTranslationSelected = 0; // Correct !?!??
}

// ------------------------------------------------------------------------------------------

void SoLabelKit::resizeToWidgetCB(const SbVec3f &size, const SbVec3f & transl, const SbVec3f & transl2)
{
	printf("SoLabelKit::resizeToWidgetCB resizeToWidget [%d] \n",resizeToWidget.getValue() );

	if (resizeToWidget.getValue() == NO) return;
	float widthV = size[0];
	float heightV = size[1];
	float depthV = size[2];

	float widthSetV = widthV;
	float depthSetV = depthV;
	float heightSetV = heightV;
	
	if (uniformScale.getValue() != NO)
	{
		float finalScale = widthV;

		if (widthV < depthV) 
			widthV = depthV;
		else 
			depthV = widthV;

		heightSetV = 1;
		// if the label is not of type SoAsciiText
		// also take the height into account
		if (getAsciiText() == NULL)
		{
			if (finalScale > heightV) 
					finalScale = heightV;

			heightSetV = finalScale;
		}
		widthSetV  = finalScale;
		depthSetV  = finalScale;

		DEBUG_CODE(printf("SoLabelKit::resizeToWidgetCB [uniform scale] %f %f %f \n", widthSetV, depthSetV, heightSetV));

	}
	SoTransform *transf = SO_GET_PART( this, "localTransform", SoTransform);
	// if in opposite direction that widget
	transf->scaleFactor.setValue(widthSetV, depthSetV, heightSetV);
	DEBUG_CODE(printf("SoLabelKit::resizeToWidgetCB set scaleFactor %f %f %f \n", widthSetV, depthSetV, heightSetV));

	if (translations.getNum() == 3)
	{
		if (translations[2] == SbVec3f(-1 , -1 ,-1))
		{
			translations.setNum(3);
			translations.set1Value(0,		transl[0],  transl[1] *heightV , transl[2]);
			translations.set1Value(1,		transl2[0], transl2[1]*heightV, transl2[2]);
			DEBUG_CODE(printf("SoLabelKit::resizeToWidgetCB set translations \n[%f/%f/%f]\n[%f/%f/%f] \n"
				,transl[0],  transl[1] *heightV, transl[2]
				,transl2[0], transl2[1]*heightV, transl2[2]
				));
		}
	}
//	DEBUG_CODE(printf("SoLabelKit::resizeToWidgetCB [] %f %f %f \n", widthSetV, depthSetV, heightSetV));

	setTranslation(lastTranslationSelected );
}

// ------------------------------------------------------------------------------------------

SoAsciiText * SoLabelKit::getAsciiText()
{
	SoNode *contents = getPart("contents", TRUE);
	if (contents!= NULL && contents->isOfType(SoSeparator::getClassTypeId()))
	{
		if (((SoSeparator*)contents)->getNumChildren() > 0)
		{
			SoNode *child = ((SoSeparator*)contents)->getChild(0);
			if (child!= NULL && child->isOfType(SoAsciiText::getClassTypeId()))
			{
				return (SoAsciiText *)child;
			}
		}
	}
	return NULL;
}

// ------------------------------------------------------------------------------------------
