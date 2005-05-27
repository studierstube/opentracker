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
/** 
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucStateFixed.h 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef SoPucStateFixed_H
#define SoPucStateFixed_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoPucStateBase.h"

/**
 Represents a PUC fixed point state in the OIV scene graph. A FIXED state is normally represented as an edit box, 
 If SoPucStateFixed::min, SoPucStateFixed::max and SoPucStateFixed::incr are specified a slidebar can be created 
 (on devices that have this widget implemented). Please note that setting either SoPucStateFixed::min or SoPucStateFixed::max will
 result in checking the boundaries when setting the values in the scene graph (over field connections). For example setting 
 SoPucStateFixed::min to 4 will result in a invalid definition as SoPucStateFixed::max will be still 0 (min 4 , max 0 -> no values fit).
 To avoid unwanted features please allways set both fields. If both min and max have the same value boundary checks are omitted.
 
 <b>FAQ: When use SoPucStateFixed and when use SoPucStateFloat ?</b>

  If you know the range of the value for that state (eg 0.0 - 1.0 ) you should allready use SoPucStateFixed to define the state, but be careful
  to set the pointpos field to an appropriate value. Only at occations where very little restrictions apply to the state you should use 
  SoPucStateFloat. In general you should try to give the layout engine as much information as possible.
 
   CATALOG PARTS

  All Parts	
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>activeIf </td><td>SoPucActiveIfNode</td><td>SoPucActiveIfNode</td><td>YES</td></tr>	
  </table>

 
File Format / defaults:

@verbatim
SoPucStateFixed {
   	SoPucActiveIfNode	activeIf		NULL # conditions that enable / disable the state see SoPucElementBase::activeIf
	SoMFString			labels			[]   # labels of state see SoPucElementBase::labels
 	SoSFInt32			priority		0	 # priority of state see SoPucElementBase::priority

	SoMFString			valueLabels		[]	 # labels for values see SoPucStateBase::valueLabels
    SoSFEnum			access			ANY  # access level for clients see SoPucStateBase::access
	 
	SoSFFloat			min				0.0  # minimum that the state can be set to
    SoSFFloat			max				0.0  # maximum that the state can be set to
    SoSFFloat			value			0.0  # value of the state
	SoSFFloat			incr			1.0  # increase steps of value (used in generating a scrollbar)
    SoSFInt32			pointpos		1    # number of digits after the decimal point
   }

@endverbatim

See:
	SoPucStateBase::activeIf, SoPucElementBase::labels, SoPucStateBase::valueLabels
	

  @ingroup puc
 */


class PUC_API SoPucStateFixed : public SoPucStateBase
{
    SO_KIT_HEADER(SoPucStateFixed);

public:
	// special
	SoSFFloat  min;
	SoSFFloat  max;
	SoSFFloat  incr;
	SoSFInt32  pointpos;

	SoSFFloat  value;

    static void initClass();
    SoPucStateFixed();

	virtual std::string getType() const;
	virtual std::string getStateValue();
	virtual bool setStateValue(std::string value_);

	virtual bool isEqual(const SbString &value) const;
	virtual bool isGreaterThan(const SbString &value) const;
	virtual bool isLessThan(const SbString &value) const;

protected:
    virtual ~SoPucStateFixed();

	virtual void valueSetCallback(SoSensor *sensor);

};

//----------------------------------------------------------------------------

#endif // SoPucStateFixed_H