 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for ButtonHoldFilterModule module.
  *
  * @author Thomas Psik
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include <stdlib.h>
#include "ButtonHoldFilterModule.h"
#include "ButtonHoldFilterModule.h"

#include "ButtonHoldFilterNode.h"

#include <stdio.h>
#include <iostream>

//using namespace std;
#include <ace/Log_Msg.h>

// Destructor method

namespace ot {

ButtonHoldFilterModule::~ButtonHoldFilterModule()
{
    nodes.clear();
}

// This method is called to construct a new Node.

Node * ButtonHoldFilterModule::createNode( const std::string& name, StringTable& attributes)
{
    if( name.compare("ButtonHoldFilter") == 0 )
    {
		int offDuration;
        int num = sscanf(attributes.get("offDuration").c_str(), " %i", &offDuration );
        if( num == 0 ){
            offDuration = 2;
        }

        ButtonHoldFilterNode * source = new ButtonHoldFilterNode(offDuration  );
        nodes.push_back( source );
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Built ButtonHoldFilter node \n")));

        initialized = 1;
        return source;
    }
    return NULL;
}

// pushes events into the tracker tree.
void ButtonHoldFilterModule::pushState()
{
    for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
    {
        ButtonHoldFilterNode *source = (ButtonHoldFilterNode *) *it;
        source->push();
    }
    cycle++;
}

} // namespace ot
