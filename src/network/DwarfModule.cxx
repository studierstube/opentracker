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
/** source file for DwarfModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/DwarfModule.cxx,v 1.1 2003/07/24 10:08:54 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "DwarfModule.h"
#include "DwarfSource.h"
#include "DwarfSink.h"

#include "MathUtils.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

// Destructor method

DwarfModule::~DwarfModule()
{
}

void DwarfModule::init(StringTable & attributes, ConfigNode * localTree)
{

    Module::init( attributes, localTree );
}

// This method is called to construct a new Node.

Node * DwarfModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("DwarfSource") == 0 )
    {
        return source;
    }
    else if( name.compare("DwarfSink") == 0 )
    {
        return sink;
    }
    return NULL;
}

// pushes events into the tracker tree.

void DwarfModule::pushState()
{
}

void DwarfModule::pullState()
{
}


