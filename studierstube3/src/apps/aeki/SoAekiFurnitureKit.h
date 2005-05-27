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
/** c++ header for SoAekiFurnitureKit
  *
  * @author Manuel Linsmayer <manuel.linsmayer@expersite.com>, Reza Mostafaie
  *
  * $Id: SoAekiFurnitureKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOAEKIFURNITUREKIT_H_
#define _SOAEKIFURNITUREKIT_H_

#ifdef WIN32
#pragma warning (disable: 4786)
#pragma warning (disable: 4788)
#endif

#include <map>
#include <string>
#include <vector>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include "aeki.h"
#include "AssemblyStep.h"
#include "ComponentInstance.h"

/**
 * 
 * @author Manuel Linsmayer , Reza Mostafaie
 * @ingroup aeki
 */
class AEKI_API SoAekiFurnitureKit : public SoBaseKit {

	// Inserts header
	SO_KIT_HEADER(SoAekiFurnitureKit);

	// Defines fields for the new parts in the catalog
	SO_KIT_CATALOG_ENTRY_HEADER(trackerList);
	SO_KIT_CATALOG_ENTRY_HEADER(modelTracker);
	SO_KIT_CATALOG_ENTRY_HEADER(modelTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(setupList);
	SO_KIT_CATALOG_ENTRY_HEADER(rotationList);
	SO_KIT_CATALOG_ENTRY_HEADER(centerList);
	SO_KIT_CATALOG_ENTRY_HEADER(componentInstanceList);

public:
	SoAekiFurnitureKit();   // Constructor
	SoSFString filename;    // Field filename
	SoSFUInt32 position;    // Field position
	/// station to use for modell tracking
	SoSFInt32 modelStation;		
	unsigned int getMaxPosition();

SoINTERNAL public:
	static void initClass();

protected:
	virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);
    SoFieldSensor* filenameSensor;
	SoFieldSensor* positionSensor;
    void static filenameCB(void* data, SoSensor* sensor);
	void static positionCB(void* data, SoSensor* sensor);
	std::map<std::string, ComponentInstance>* componentInstances;
	std::vector<AssemblyStep*>* assemblySteps;

private:
	virtual ~SoAekiFurnitureKit();   // Desctructor
	void createInitialFurniture();

};

#endif

/* ===========================================================================
End of SoAekiFurnitureKit.h
=========================================================================== */
