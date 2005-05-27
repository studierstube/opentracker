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
/** Header file for API definitions of SoPucElementBase
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucElementBase.h 3565 2004-08-02 14:47:20Z tomp $
  * @file                                                                   */
 /* ======================================================================= */


#ifndef SoPucElementBase_H
#define SoPucElementBase_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFInt32.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "PUC.h"
#include <string>
/**
 Is a basis class for all PUC elements in the OIV scene graph. The purpose of this class is to define the common attributes for a Puc state.

See:
	SoBaseKit

  @ingroup puc
 */

class SoGetPucSpecificationAction;
class PUC_API SoPucElementBase : public SoBaseKit
{
    SO_KIT_HEADER(SoPucElementBase);
	SO_KIT_CATALOG_ENTRY_HEADER(activeIf); ///<	can be used to specifiy the context in which this state is used. Note that <em>"not active"</em> is not the same as disabled (setting the "readonly" access flag), as <em>"not active"</em> also means that there are no changes to that state (or at least they are not distributed to the clients).


public:
	SoMFString labels;				///< each state must have at least one label. A PUC device is able to select any label, so every label should be representative for the state. It is a good idea to supply at least 2 labels for each state. One that give the user a good hint what the state is used for in the scene and a second one that is short but descriptive.
	SoSFInt32 priority;				///< each state has a priority, which alows the layout engine to make more clever decisions, than without the information of the priority <em size=-1>(meaning I dont really understand the need for the priority - just leave the default value)</em>
	SoMFString explanation;			///< possible tooltip text

	std::string pucName;			///< updated by the puc tree, do not overwrite this !!
	
    static void initClass();
    SoPucElementBase();

	virtual std::string getElementName() const;
	std::string getStateName() const;

protected:
 	virtual ~SoPucElementBase();

};

//----------------------------------------------------------------------------

#endif // STBPUCVALUEGROUP_H