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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/CallbackModule.cxx,v 1.6 2003/01/09 04:14:12 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "CallbackModule.h"

#include <iostream>

using namespace std;

// called to construct a new Node.

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
            cout << "Build Callback node " << nameVal << "." << endl;
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
