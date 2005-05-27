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
/** Header file for API definitions of SoPucStateFloat
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucStateFloat.h 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef SoPucStateFloat_H
#define SoPucStateFloat_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFFloat.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoPucStateBase.h"

/**
 Represents a PUC floating point state in the OIV scene graph. A FLOAT state is normally represented as an edit box. If you
 would like PUC to create a slidebar for this state you should use SoPucStateFixed.

 Please note that setting either SoPucStateFloat::min or SoPucStateFloat::max will
 result in checking the boundaries when setting the values in the scene graph (over field connections). For example setting 
 SoPucStateFloat::min to 3 will result in a invalid definition as SoPucStateFloat::max will be still 0 (min 3 , max 0 -> no values fit).
 To avoid unwanted features please allways set both fields. If both min and max have the same value boundary checks are omitted.
 
 <b>FAQ: When use SoPucStateFixed and when use SoPucStateFloat ?</b>

  If you know the range of the value for that state (eg 0.0 - 1.0 ) you should allready use SoPucStateFixed to define the state. 
  Only at occations where very little restrictions apply to the state you should use 
  SoPucStateFloat (for example if the number of digits behind the decimal point are of relevance). 

  CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>activeIf </td><td>SoPucActiveIfNode</td><td>SoPucActiveIfNode</td><td>YES</td></tr>
  </table>


File Format / defaults:

@verbatim
SoPucStateFloat {
   	SoPucActiveIfNode	activeIf		NULL # conditions that enable / disable the state see SoPucElementBase::activeIf
	SoMFString			labels			[]   # labels of state see SoPucElementBase::labels
 	SoSFInt32			priority		0	 # priority of state see SoPucElementBase::priority

	SoMFString			valueLabels		[]	 # labels for values see SoPucStateBase::valueLabels
    SoSFEnum			access			ANY  # access level for clients see SoPucStateBase::access
	 
	SoSFFloat			min				0    # minimum that the state can be set to
    SoSFFloat			max				0    # maximum that the state can be set to
    SoSFFloat			value			0    # value of the state
 }

@endverbatim

See:
	SoPucStateBase::activeIf, SoPucElementBase::labels, SoPucStateBase::valueLabels


  @ingroup puc
 */

class PUC_API SoPucStateFloat : public SoPucStateBase
{
    SO_KIT_HEADER(SoPucStateFloat);

public:
	// special
	SoSFFloat  min;
	SoSFFloat  max;
	SoSFFloat  value;

    static void initClass();
    SoPucStateFloat();

	virtual std::string getType() const;
	virtual std::string getStateValue();
	virtual bool setStateValue(std::string value_);

	virtual bool isEqual(const SbString &value) const;
	virtual bool isGreaterThan(const SbString &value) const;
	virtual bool isLessThan(const SbString &value) const;

protected:
    virtual ~SoPucStateFloat();
	virtual void valueSetCallback(SoSensor *sensor);
};

//----------------------------------------------------------------------------

#endif // SoPucStateFloat_H