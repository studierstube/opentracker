 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for ConsoleModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConsoleModule.cxx,v 1.4 2001/02/20 22:50:49 reitmayr Exp $
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
           STATION_9 = 29,
           RESET = 30,
           QUIT = 31;

vector<string> ConsoleModule::functionMap;
          
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

// constructor method.

ConsoleModule::ConsoleModule() : Module(), NodeFactory()
{
    // initialize some variables
    cycle = 0;
    angularSpeed = 0.1;
    posSpeed = 0.1;

    // initialize key map
    keyMap[MOVE_X_PLUS] = 'o';
    keyMap[MOVE_X_MINUS] = 'l';
    keyMap[MOVE_Y_PLUS] = 'p';
    keyMap[MOVE_Y_MINUS] = 'ö';
    keyMap[MOVE_Z_PLUS] = 'ü';
    keyMap[MOVE_Z_MINUS] = 'ä';
    keyMap[ROT_X_PLUS] = 'e';
    keyMap[ROT_X_MINUS] = 'd';
    keyMap[ROT_Y_PLUS] = 'r';
    keyMap[ROT_Y_MINUS] = 'f';
    keyMap[ROT_Z_PLUS] = 't';
    keyMap[ROT_Z_MINUS] = 'g';
    keyMap[ACCELL] = 'y';
    keyMap[BRAKE] = 'x';
    keyMap[BUTTON_1] = ' ';
    keyMap[BUTTON_2] = ',';
    keyMap[BUTTON_3] = '.';
    keyMap[BUTTON_4] = '-';
    keyMap[STATION_0] = '0';
    keyMap[STATION_1] = '1';
    keyMap[STATION_2] = '2';
    keyMap[STATION_3] = '3';
    keyMap[STATION_4] = '4';
    keyMap[STATION_5] = '5';
    keyMap[STATION_6] = '6';
    keyMap[STATION_7] = '7';
    keyMap[STATION_8] = '8';
    keyMap[STATION_9] = '9';
    keyMap[RESET] = 'ß';
    keyMap[QUIT] = 'q';

    // initialize function map, if no one has done it yet 
    if( functionMap.size() == 0 )
    {
        functionMap[MOVE_X_PLUS] = "Move_X_plus";
        functionMap[MOVE_X_MINUS] = "Move_X_minus";
        functionMap[MOVE_Y_PLUS] = "Move_Y_plus";
        functionMap[MOVE_Y_MINUS] = "Move_Y_minus";
        functionMap[MOVE_Z_PLUS] = "Move_Z_plus";
        functionMap[MOVE_Z_MINUS] = "Move_Z_minus";
        functionMap[ROT_X_PLUS] = "Rot_X_plus";
        functionMap[ROT_X_MINUS] = "Rot_X_minus";
        functionMap[ROT_Y_PLUS] = "Rot_Y_plus";
        functionMap[ROT_Y_MINUS] = "Rot_Y_minus";
        functionMap[ROT_Z_PLUS] = "Rot_Z_plus";
        functionMap[ROT_Z_MINUS] = "Rot_Z_minus";
        functionMap[ACCELL] = "Accelerate";
        functionMap[BRAKE] = "Brake";
        functionMap[BUTTON_1] = "Button_1";
        functionMap[BUTTON_2] = "Button_2";
        functionMap[BUTTON_3] = "Button_3";
        functionMap[BUTTON_4] = "Button_4";
        functionMap[STATION_0] = "Station_0";
        functionMap[STATION_1] = "Station_1";
        functionMap[STATION_2] = "Station_2";
        functionMap[STATION_3] = "Station_3";
        functionMap[STATION_4] = "Station_4";
        functionMap[STATION_5] = "Station_5";
        functionMap[STATION_6] = "Station_6";
        functionMap[STATION_7] = "Station_7";
        functionMap[STATION_8] = "Station_8";
        functionMap[STATION_9] = "Station_9";
        functionMap[RESET] = "Reset";
        functionMap[QUIT] = "Quit";
    }
}

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

// pulls events out of the tracker tree

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
    if( localTree != NULL )
    {
        ConfigNode * base = (ConfigNode *)localTree;
        NodeVector & nodes = base->getChildren();
        for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
        {
            ConfigNode * config = (*it);
            if( config->getName().compare("KeyDefinition") == 0 )
            {
                
            }
        }
    }
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
