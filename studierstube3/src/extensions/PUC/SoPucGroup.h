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
/** Header file for API definitions of SoPucGroup
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucGroup.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef SoPucGroup_H
#define SoPucGroup_H

#include "SoPucElementBase.h"

#include <string>

class SoPucDevice;
/**
 Represents a PUC group. It is a good practice to group states that are logical connected. The layout engine will try to keep those states 
 close together. An activeIf clause can be used to set the context for a whole group of states. A group may also contain groups. Note that every 
 PUC Device must have at least one group, as the definition of the SoPUCDevice allows only SoPucGroups in the SoPUCDevice::groups field.
 Also at some devices a tabbed dialog will be used by the layout engine to represent groups of states.

  CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>members </td><td>NodeKitListPart</td><td>---</td><td>NO</td></tr>
  <tr> <td>activeIf </td><td>SoPucActiveIfNode</td><td>SoPucActiveIfNode</td><td>YES</td></tr>	
  </table>


  Extra Information for List Parts from Above Table

  <table>
	<tr> <td>Part Name</td><td>Container Type</td><td>Possible Types</td></tr>
	<tr> <td>members</td><td>SoGroup</td><td>SoPucElementBase</td></tr>
   </table>

File Format / defaults:

@verbatim
SoPucGroup {
	SoPucActiveIfNode	activeIf		NULL # see SoPucElementBase::activeIf
	SoMFString			labels			[]   # see SoPucElementBase::labels
	SoSFInt32			priority		0	 # see SoPucElementBase::priority
  }
@endverbatim

See:
	SoPucStateBase::activeIf, SoPucElementBase::labels, SoPucStateBase::valueLabels

  
	
  @ingroup puc
*/

class PUC_API SoPucGroup : public SoPucElementBase
{
    SO_KIT_HEADER(SoPucGroup);

    SO_KIT_CATALOG_ENTRY_HEADER(members);

public:

    static void initClass();
    SoPucGroup();
	virtual std::string getElementName() const;

protected:
    virtual ~SoPucGroup();

	friend SoPucDevice;
};

//----------------------------------------------------------------------------

#endif // STBPUCVALUEGROUP_H