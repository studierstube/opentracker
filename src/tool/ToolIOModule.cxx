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
/** source file for special module handling ConsoleSink / ConsoleSource nodes
  * instead of the usuall ConsoleModule.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/tool/ToolIOModule.cxx,v 1.1 2001/11/21 11:46:05 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ToolIOModule.h"
#include "ToolSource.h"
#include "ToolSink.h"

// constructor method

ToolIOModule::ToolIOModule( QTable * out_ ) :
	Module(), NodeFactory(), output( out_ ), quit(0)
{
    output->setNumRows( 0 );    
}
	 
// Destructor method, clears nodes member.

ToolIOModule::~ToolIOModule()
{
    sinks.clear();
    sources.clear();
}

// initializes the tracker module.

void ToolIOModule::init(StringTable& attributes, ConfigNode * localTree)
{
    Module::init( attributes, localTree );
}

// creates new nodes

Node * ToolIOModule::createNode( const std::string& name, StringTable& attributes)
{
    if( name.compare("ConsoleSink") == 0 || name.compare("ToolSink") == 0 )
    {
        ToolSink * sink = new ToolSink( attributes.get("comment"));
        sinks.push_back( sink );
//        cout << "Built ToolSink node for ConsoleSink" << endl;       
        return sink;
    } else if( name.compare("ConsoleSource") == 0 || name.compare("ToolSource") == 0)
    {
        int number;
        if( sscanf( attributes.get("number").c_str()," %i", &number ) == 1 )
        {
            if( number >= 0 && number < 10 )
            {
                ToolSource * source = new ToolSource( number );
                sources.push_back( source );
//                cout << "Build ToolSource node." << endl;
                return source;
            } else
            {
//                cout << "ToolSource station number not in [0,9] : " << number << endl;
            }
        }
    }
    return NULL;
}

// reads out node states and displays them

void ToolIOModule::pullState()
{
    QString s;
    int row;
    double diff;
    for(NodeVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
    {
        ToolSink * sink = (ToolSink *)(*it);
        if( sink->changed == 1 )
        {
            row = it - sinks.begin();
            State & state = sink->state;
            diff = OSUtils::currentTime() - state.time;
            s.sprintf("%f %f %f", state.position[0],state.position[1],state.position[2]);
            output->setText( row, 0, s );
            s.sprintf("%f %f %f %f", state.orientation[0], state.orientation[1], 
                                     state.orientation[2], state.orientation[3] );
            output->setText( row, 1, s );
            s.sprintf("%hx", state.button );
            output->setText( row, 2, s );
            s.sprintf("%f", state.confidence );
            output->setText( row, 3, s );
            s.sprintf("%lf", state.time );
            output->setText( row, 4, s );
            s.sprintf("%lf", diff );
            output->setText( row, 5, s );
            sink->changed = 0;
        }
    }
}

// sets the table properties

void ToolIOModule::start()
{
    output->setNumRows( sinks.size());
    QHeader * head = output->verticalHeader();
    for(NodeVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
    {
        ToolSink * sink = (ToolSink *)(*it);
        head->setLabel(it - sinks.begin(),sink->comment.c_str()); 
    }
    head->adjustSize();
    Module::start();
}

void ToolIOModule::close()
{
}

int ToolIOModule::stop()
{
    return quit;
}
