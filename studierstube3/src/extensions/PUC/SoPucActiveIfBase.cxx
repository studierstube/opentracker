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
* $Id: SoPucActiveIfBase.cxx 3426 2004-07-14 12:11:27Z tamer $
* @file                                                                   */
/* ======================================================================= */

#include "SoPucActiveIfBase.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucActiveIfBase);

//----------------------------------------------------------------------------

void SoPucActiveIfBase::initClass()
{
	SO_KIT_INIT_CLASS(SoPucActiveIfBase, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------

SoPucActiveIfBase::SoPucActiveIfBase()
{
	SO_KIT_CONSTRUCTOR(SoPucActiveIfBase);
	SO_NODE_ADD_FIELD(active, (TRUE));	
	SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------

SoPucActiveIfBase::~SoPucActiveIfBase()
{
	
}

//----------------------------------------------------------------------------
