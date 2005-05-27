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
/** Header file for API definitions of SoPucActiveIfClause
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucActiveIfClause.h 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoPucActiveIfClause_H
#define SoPucActiveIfClause_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFEnum.h>

#include <Inventor/sensors/SoFieldSensor.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "PUC.h"
#include <string>
#include "SoPucActiveIfBase.h"


/**
  * SoPucActiveIfClause represents a conditional statement in the PUC State graph. 
  *
File Format / defaults:

@verbatim
SoPucActiveIfClause {
	SoSFString			state			""      # if it is connected, it has to be connected to a value field derived from SoPucStateBase 
	SoSFEnum			op				EQULAS  # valid enums EQUALS, GREATER_THAN, LESS_THAN
	SoSFString			value			""		# value that the value of state.value is compared to (using operator spec in op)
	# from SoPucActiveIfBase
	SoSFBool			active			TRUE    # if TRUE the condition is statisfied
 }
@endverbatim

See:
	SoPucStateBase::activeIf,
	

Examples:
SoPucGroup {
  activeIf SoPucActiveIfNode {
    activeIf SoPucActiveIfClause { state = USE SELECT_PAGE.value op EQUALS value 1 }
    }
}
  
  * @ingroup puc
  */

class PUC_API SoPucActiveIfClause : public SoPucActiveIfBase
{
    SO_KIT_HEADER(SoPucActiveIfClause);

public:
	enum ActiveIfClauseOperator
	{
		EQUALS = 0,
		GREATER_THAN = 1,
		LESS_THAN = 2
	} ;

	SoSFString state; 
	SoSFEnum   op;
	SoSFString value;

    static void initClass();
    SoPucActiveIfClause();

	///< sets up the connection of the sensors
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways); 

protected:
    virtual ~SoPucActiveIfClause();
	
	SoFieldSensor stateSensor;
	SoFieldSensor valueSensor;
	SoFieldSensor opSensor;
	static void evaluateActiveCB( void * data, SoSensor * sensor  );
	
	void evaluateActive(bool alwaysSetValue = false);

};

//----------------------------------------------------------------------------

#endif // STBPUCVALUEGROUP_H