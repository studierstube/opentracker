 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for ConsoleModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConsoleModule.cxx,v 1.2 2001/02/13 16:41:37 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ConsoleModule.h"
#include "ConsoleSink.h"
#include "ConsoleSource.h"


// list of key symbols as ints to provide faster lookup

static short MOVE_X_PLUS = 1,
           MOVE_X_MINUS = 2,
           MOVE_Y_PLUS = 3,
           MOVE_Y_MINUS = 4,
           MOVE_Z_PLUS = 5,
           MOVE_Z_MINUS = 6,
           ROT_X_PLUS = 7,
           ROT_X_MINUS = 8,
           ROT_Y_PLUS = 9,
           ROT_Y_MINUS = 10,
           ROT_Z_PLUS = 11,
           ROT_Z_MINUS = 12,
           ACCELL = 13,
           BRAKE = 14,
           BUTTON_1 = 15,
           BUTTON_2 = 16,
           BUTTON_3 = 17,
           BUTTON_4 = 18,
           STATION_0 = 20,
           STATION_1 = 21,
           STATION_2 = 22,
           STATION_3 = 23,
           STATION_4 = 24,
           STATION_5 = 25,
           STATION_6 = 26,
           STATION_7 = 27,
           STATION_8 = 28,
           STATION_9 = 29;
          
// Destructor method, this is here because curses seem to define some macro
// which replaces clear with wclear !!!!!

ConsoleModule::~ConsoleModule()
{
    sinks.clear();
    sources.clear();
}

#include "stdio.h"
#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#include <iomanip>
#include <conio.h>
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
        sinks.push_back( sink );
        cout << "Built ConsoleSink node." << endl;       
        return sink;
    } else if( name.compare("ConsoleSource") == 0 )
    {
        int number;
        if( sscanf( (*attributes.find("number")).second.c_str()," %i", &number ) == 1 )
        {
            if( number >= 0 && number < 10 )
            {
                ConsoleSource * source = new ConsoleSource( number );
                sources.push_back( source );
                cout << "Build ConsoleSource node." << endl;
                return source;
            } else
            {
                cout << "ConsoleSource station number not in [0,9] : " << number << endl;
            }
        }
    }
    return NULL;
}

// pushes events into the tracker tree.

void ConsoleModule::pullState()
{
    if( sinks.size() <= 0 )
    {
        return;
    }
    cycle = (cycle+1) % interval;
    if( cycle == 0 ){    
#ifndef WIN32
        move(1,1);            
#else        
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        if( hStdout == INVALID_HANDLE_VALUE )
        {
            cout << "Could not get stdout handle !" << endl;
            return;
        }
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
        if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) 
        {
            cout << "Could not get console size !" << endl;
            return;
        }
        int lines = sinks.size() * 6 + 2;
        COORD origin;
        origin.X = 0;
        origin.Y = 0;
        unsigned long written;
        if( ! FillConsoleOutputCharacter( hStdout,' ', 
                csbiInfo.dwSize.X * lines, origin, &written))
        {
            cout << "Could not clear console !" << endl;
            return;
        }
        if( ! SetConsoleCursorPosition(hStdout, origin ))
        {
            cout << "Could not reset cursor !" << endl;
            return;
        }
#endif
#ifdef WIN32
        cout << headerline << endl << endl;
#else
        printw("%s", headerline.c_str());
        printw("");
#endif
        for( NodeVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
        {
            ConsoleSink * sink = (ConsoleSink *) *it;
            State & state = sink->state;
#ifdef WIN32            
            cout.fill(' ');
            cout << sink->comment << endl;
            cout << "  Pos : " << std::setw( 8 ) << std::setprecision( 3 ) << state.position[0] << " " <<
                                  std::setw( 8 ) << std::setprecision( 3 ) << state.position[1] << " " <<
                                  std::setw( 8 ) << std::setprecision( 3 ) << state.position[2] << endl;
            cout << "  Rot : " << std::setw( 8 ) << std::setprecision( 3 ) << state.orientation[0] << " " <<
                                  std::setw( 8 ) << std::setprecision( 3 ) << state.orientation[1] << " " <<
                                  std::setw( 8 ) << std::setprecision( 3 ) << state.orientation[2] << " " <<
                                  std::setw( 8 ) << std::setprecision( 3 ) << state.orientation[3] << endl;
            cout << "  Button : " << state.button << " ";
            cout << "  Confidence : " << std::setw( 8 ) << std::setprecision( 3 ) << state.confidence << endl;
            cout << "  Time : " << std::setw( 8 ) << std::setprecision( 3 ) << state.time << endl << endl;            
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
    int num = sscanf(attributes["interval"].c_str(), " %i", &interval );
    if( num == 0 ){
        interval = 10;
    }
    headerline = attributes["headerline"];
    Module::init( attributes, localTree );
}

// start the module and init curses

void ConsoleModule::start()
{
#ifndef WIN32
    if( sinks.size() > 0 )
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

// tests whether a key was pressed, if so it stops.

int ConsoleModule::stop()
{
#ifdef WIN32
    if( _kbhit() )
        return 1;
#else
#endif
    return 0;
}
