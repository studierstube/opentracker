 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for ConsoleModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/output/Attic/ConsoleModule.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ConsoleModule.h"
#include "ConsoleSink.h"

// Destructor method, this is here because curses seem to define some macro
// which replaces clear with wclear !!!!!

ConsoleModule::~ConsoleModule()
{
    nodes.clear();
}

#include "stdio.h"
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#include <curses.h>
#include <unistd.h>
#endif

// This method is called to construct a new Node.

Node * ConsoleModule::createNode( string& name,
                               StringMap& attributes)
{
    if( name.compare("ConsoleSink") == 0 )
    {
        ConsoleSink * sink = new ConsoleSink( attributes["comment"] );
        nodes.push_back( sink );
        cout << "Built ConsoleSink node." << endl;       
        return sink;
    }
    return NULL;
}

// pushes events into the tracker tree.

void ConsoleModule::pullState()
{
    cycle = (cycle+1) % interval;
    if( cycle == 0 ){    
#ifndef WIN32
        move(1,1);            
#endif        
        for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
        {
            ConsoleSink * sink = (ConsoleSink *) *it;
            State & state = sink->state;
#ifdef WIN32            
            cout << sink->comment << endl;
            cout << "  Pos : " << state.position[0] << " " <<
                                  state.position[1] << " " <<
                                  state.position[2] << endl;
            cout << "  Rot : " << state.orientation[0] << " " <<
                                  state.orientation[1] << " " <<
                                  state.orientation[2] << " " <<
                                  state.orientation[3] << endl;
            cout << "  Button : " << state.button << " ";
            cout << "  Confidence : " << state.confidence << endl;
            cout << "  Time : " << state.time << endl << endl;            
#else
            printw(" %s : ",sink->comment);
            printw("  Pos : %f %f %f\n",state.position[0], 
                   state.position[1], state.position[2]);
            printw("  Rot : %f %f %f %f\n", state.orientation[0], 
                   state.orientation[1], state.orientation[2],
                   state.orientation[3] );
            printw("  Buttons : %hx\n", state.button );
            printw("  Confidence : %f\n", state.confidence );
            printw("  Time : %lf\n\n", state.time );
#endif            
        }   
    }
}

// initializes ConsoleModule

void ConsoleModule::init(StringMap& attributes,  Node * localTree)
{
    int num = sscanf(attributes.find("interval")->second.c_str(), " %i", &interval );
    if( num == 0 ){
        interval = 10;
    }
    Module::init( attributes, localTree );
}

// start the module and init curses

void ConsoleModule::start()
{
#ifndef WIN32
    if( nodes.size() > 0 )
    {
        initscr();
        cbreak();
        noecho();
        nonl();
        intrflush(stdscr,FALSE);
        keypad(stdscr,TRUE);
        nodelay(stdscr, TRUE);
        leaveok(stdscr, TRUE);
    }
#endif
}

// close ConsoleModule 

void ConsoleModule::close()
{
#ifndef WIN32
    endwin();
#endif
}
