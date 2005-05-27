 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
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
/** c++ header for FurnitureFactory
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: FurnitureFactory.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _FURNITUREFACTORY_H_
#define _FURNITUREFACTORY_H_

#ifdef WIN32
#pragma warning (disable: 4786)
#pragma warning (disable: 4788)
#endif

#include <map>
#include <string>
#include <vector>
#include <Inventor/nodes/SoSwitch.h>
#include "AssemblyStep.h"
#include "ComponentInstance.h"

/**
 * 
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class FurnitureFactory {

public:
	static std::vector<SoSwitch*>* buildComponentInstanceList(std::map<std::string, ComponentInstance>& componentInstances,
	                                                   std::vector<AssemblyStep*>& assemblySteps);
    static std::vector<SoSwitch*>* buildRotationList(std::vector<AssemblyStep*>& assemblySteps);
	static std::vector<SoStationKit*>* FurnitureFactory::buildTrackerList(std::map<std::string, ComponentInstance>& componentInstances,
																   std::vector<AssemblyStep*>& assemblySteps);

};

#endif

/* ===========================================================================
End of FurnitureFactory.h
=========================================================================== */
