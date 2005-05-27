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
/** Header file for API definitions of SoPucValueBase
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucStateBase.h 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoPucValueBase_H
#define SoPucValueBase_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoMFString.h>

#include <Inventor/sensors/SoFieldSensor.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoPucElementBase.h"

#include <string>
/**
 Is a basis class for all PUC states in the OIV scene graph. Must not be in the scene graph !!

See:
	SoPucElementBase

  @ingroup puc
 */

class PUC_API SoPucStateBase : public SoPucElementBase
{
    SO_KIT_HEADER(SoPucStateBase);

public:
	enum AccessRestriction
	{
		ANY = -1,			  ///< clients can change the value of the state and get updated if state value is changed
		READ_ONLY = 0,		  ///< clients will only display the state
		WRITE_ONLY = 1,		  ///< clients can only set values (they do not get updates)
	} ;

	SoSFEnum      access;	  ///< restrict access for clients see SoPucStateBase::AccessRestriction
	SoMFString    valueLabels; ///< values of the state can get extra lables. This field stores them in sequential order

	static void initClass();
    SoPucStateBase();

	virtual std::string getElementName() const;

	virtual std::string getType() const;
	virtual std::string getStateValue();
	virtual bool setStateValue(std::string value_);

	virtual bool isEqual(const SbString &value) const;
	virtual bool isGreaterThan(const SbString &value) const;
	virtual bool isLessThan(const SbString &value) const;

	bool isValueLabelsSpecified() const;

protected:
    virtual ~SoPucStateBase();

	static void valueSetCB(void *data, SoSensor *sensor);		///<
	virtual void valueSetCallback(SoSensor *sensor);  ///< function used by implementations to check valid value set through the scenegraph

	SoFieldSensor valueSensor;	///< sensor used by implementations to check valid value set through the scenegraph

};

//----------------------------------------------------------------------------

#endif // STBPUCINTVALUE_H