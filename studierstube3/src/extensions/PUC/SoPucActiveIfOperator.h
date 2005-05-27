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
/** Header file for API definitions of SoPucActiveIfOperator
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucActiveIfOperator.h 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef SoPucActiveIfOperator_H
#define SoPucActiveIfOperator_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>

#include <Inventor/sensors/SoFieldSensor.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "PUC.h"
#include <string>
#include "SoPucActiveIfBase.h"

// OIV graph:
//
// active-if SoPucActiveIfOperator {
// op AND
// clauses NodeKitListPart { containerNode Group {
// 	SoPucActiveIfClause { state USE SIZE op EQUALS value "3" }
//  SoPucActiveIfClause { state USE RADIUS op GREATER_THAN value "3" }
//  SoPucActiveIfClause { state USE RADIUS op LESS_THAN value "5" }
//   } }
// }
//
// converted to PUC xml:
//
// <active-if>
//   <and>
//        <equals state="SIZE">3</equals>
//        <greaterthan state="RADIUS">3</greaterthan>
//        <lessthan  state="RADIUS">5</lessthan>
//   </and>
// </active-if>
//
//
// active-if SoPucActiveIfOperator {
// op OR
// clauses NodeKitListPart { containerNode Group {
// 	SoPucActiveIfClause { state USE SIZE op EQUALS value "3" }
//  SoPucActiveIfClause { state USE RADIUS op GREATER_THAN value "5" }
//  SoPucActiveIfClause { state USE RADIUS op LESS_THAN value "3" }
//   } }
// }
//
// converted to PUC xml:
//
// <active-if>
//    <or>
//        <equals state="SIZE">3</equals>
//        <greaterthan state="RADIUS">5</greaterthan>
//        <lessthan  state="RADIUS">3</lessthan>
//    </or>
// </active-if>

/**
  * @ingroup puc
  */
//----------------------------------------------------------------------------

class PUC_API SoPucActiveIfOperator : public SoPucActiveIfBase
{
    SO_KIT_HEADER(SoPucActiveIfOperator);

	SO_KIT_CATALOG_ENTRY_HEADER(clauses);

public:
	enum ActiveIfOperator
	{
		OR = 0,
		AND = 1,
	} ;

	SoSFEnum   op;

    static void initClass();
    SoPucActiveIfOperator();

	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways);

protected:
    virtual ~SoPucActiveIfOperator();

	SoFieldSensor clausesSensor;
	static void clausesChangedCB(void * data, SoSensor * sensor );
	void evaluateClause(bool alwaysSetValue = false);

};

//----------------------------------------------------------------------------

#endif // STBPUCVALUEGROUP_H