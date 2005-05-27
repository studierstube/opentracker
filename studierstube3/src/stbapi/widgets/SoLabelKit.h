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

/** class definition for SoLabelKit
  *
  * base class for scene objects the user can interact with
  *
  * @author  Thomas Psik
  *
  * $Id: SoLabelKit.h 4101 2004-12-16 10:07:36Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SO_LABEL_KIT_
#define _SO_LABEL_KIT_

#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <Inventor/nodekits/SoWrapperKit.h>
#include <stbapi/interaction/SoBehaviorKit.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


class SoAsciiText;
/**
 * Represents a label (text) as part of a widget. 
 * 
 *
 * @ingroup widgets
 */

class STBAPI_API SoLabelKit : public SoWrapperKit {

	SO_KIT_HEADER(SoLabelKit);
	
    /**
	 * root separator
	 */

public:

	///
    SoMFString    text;

	/// the translation value (of labelTransform) is stored in this field
	SoMFVec3f     translations;


	enum triState
	{
		NO = 0,		/// ignore lable
		YES = 1,		/// label will be placed above the widget 
		DEFAULT = 2,		/// label will be placed left of the widget 
	} ;

	///
	SoSFEnum     resizeToWidget;
	
	///
	SoSFEnum     uniformScale;

	/**
	 * constructor
	 */
	SoLabelKit();

	/**
	 * destructor
	 */
	virtual ~SoLabelKit();

	///
	void setTranslation(int i);

	///
	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );

	///
	void resizeToWidgetCB(const SbVec3f &size, const SbVec3f & transl, const SbVec3f & transl2);

	///
//	void adaptToTemplate(SoLabelKit *labelTempl);

	///
	SoAsciiText * getAsciiText();

SoINTERNAL public:
	
	///
	static void initClass();

protected:
	int lastTranslationSelected; ///< store last setTranslation 

	SbXfBox3f getContentsBBox();

};

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#endif //_SO_LABEL_KIT_

