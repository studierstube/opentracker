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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Callback module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "CallbackModule.h"

#include <iostream>

using namespace std;

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

// called to construct a new Node.

namespace ot {

Node * CallbackModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("Callback") == 0 )
    {
        const string & nameVal = attributes.get("name");
        NodeMap::iterator it = nodes.find( nameVal );
        if( it == nodes.end())
        {
            CallbackNode * node = new CallbackNode( nameVal );
            nodes[nameVal] = node;
			LOG_ACE_INFO("ot:Build Callback node %s.\n", nameVal.c_str());
            return node;
        }
    } 
    return NULL;
}

//  sets a callback on a certain node.

void CallbackModule::setCallback( const string& name, CallbackFunction * function, void * data )
{
    NodeMap::iterator it = nodes.find( name );
    if( it != nodes.end())
    {
        ((CallbackNode *)(*it).second)->function = function;
        ((CallbackNode *)(*it).second)->data = data;
    }
}

} // namespace ot
