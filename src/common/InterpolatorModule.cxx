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
/** source file for InterpolatorModule module.
  *
  * @author Flo Ledermann
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/InterpolatorModule.cxx,v 1.1 2003/03/24 12:28:46 flo Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "InterpolatorModule.h"
#include "InterpolatorModule.h"

#include "ElasticFilterNode.h"

#include <cstdio>
#include <iostream>

using namespace std;

// Destructor method

InterpolatorModule::~InterpolatorModule()
{
    nodes.clear();
}

// This method is called to construct a new Node.

Node * InterpolatorModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("ElasticFilter") == 0 )
    {
        int frequency;
        int offset;
        float force = 0.1f, damp = 0.1f;
        int num = sscanf(attributes.get("frequency").c_str(), " %i", &frequency );
        if( num == 0 ){
            frequency = 1;
        }
        num = sscanf(attributes.get("offset").c_str(), " %i", &offset );
        if( num == 0 ){
            offset = 0;
        }
        num = sscanf(attributes.get("force").c_str(), " %f", &force );
        num = sscanf(attributes.get("damp").c_str(), " %f", &damp );
        ElasticFilterNode * source = new ElasticFilterNode( force, damp, frequency, offset );
        nodes.push_back( source );
        cout << "InterpolatorModule: Built ElasticFilter node " << endl;
        initialized = 1;
        return source;
    }
    return NULL;
}

// pushes events into the tracker tree.
void InterpolatorModule::pushState()
{
    for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
    {
        ElasticFilterNode *source = (ElasticFilterNode *) *it;
        //if((cycle + source->offset) % source->frequency == 0 )
        //{
            source->push();
        //} 
    }
    cycle++;
}
