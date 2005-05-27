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
/** Source file defines the entry point for the application dll.
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucElementBase.cxx 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucElementBase.h"
#include "SoPucActiveIfNode.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucElementBase);

//----------------------------------------------------------------------------

void SoPucElementBase::initClass()
{
	SO_KIT_INIT_CLASS(SoPucElementBase, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------

SoPucElementBase::SoPucElementBase()
{
	 SO_KIT_CONSTRUCTOR(SoPucElementBase);
    
    //---                     partName, partClassName, nullByDefault, parentName, rightName, isPublicPart
    SO_KIT_ADD_CATALOG_ENTRY(activeIf, SoPucActiveIfNode,  TRUE, this, \x0, TRUE);

    SO_NODE_ADD_FIELD(labels, (""));
	SO_NODE_ADD_FIELD(priority, (0));
	SO_NODE_ADD_FIELD(explanation, (""));
	
	SO_KIT_INIT_INSTANCE();
	pucName = "";
}

//----------------------------------------------------------------------------

SoPucElementBase::~SoPucElementBase()
{
}


//----------------------------------------------------------------------------
std::string SoPucElementBase::getElementName() const
{
	return "#@! forget to set element name #@!";
}

//----------------------------------------------------------------------------

std::string SoPucElementBase::getStateName() const
{
	std::string ret;
	SbName name = getName();
	
	if (name.getLength() == 0)
		ret = "unnamed"; // TODO auto nameing ...
	else
		ret = getName().getString();
	
	return ret;
}
