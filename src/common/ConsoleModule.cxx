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
/** source file for ConsoleModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ConsoleModule.cxx,v 1.21 2001/12/06 11:53:29 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ConsoleModule.h"
#include "ConsoleSink.h"
#include "ConsoleSource.h"

#ifdef WIN32
#include <Windows.h>
#endif

using namespace std;


// list of key symbols as ints to provide faster lookup

const short MOVE_X_PLUS = 1,
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

// maps the function names in the config file to indices
static vector<string> functionMap;
// maps the function key names in the config file to key codes 
static map<string,int> keyCodeMap;
          
// Destructor method, this is here because curses seem to define some macro
// which replaces clear with wclear !!!!!

ConsoleModule::~ConsoleModule()
{
    sinks.clear();
    sources.clear();
}

#include <stdio.h>
#include <algorithm>
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

ConsoleModule::ConsoleModule() : Module(), NodeFactory(), sinks(), sources(), keyMap(32)
{
    // initialize some variables
    cycle = 0;
    angularSpeed = 0.1f;
    posSpeed = 0.1f;
    station = 0;
    quit = 0;
    interval = 10;
    display = 1;

    // initialize function map and keycode map,
    // if no one has done it yet 
    if( functionMap.size() == 0 )
    {
        functionMap.resize( 32 );
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
        // set keymap, this is different for windows and unix ( curses )
//        keyCodeMap.resize( 20 );
#ifdef WIN32
        keyCodeMap["down"]  = 0xe050;
        keyCodeMap["up"]    = 0xe048;
        keyCodeMap["left"]  = 0xe04b;
        keyCodeMap["right"] = 0xe04d;
        keyCodeMap["home"]  = 0xe047;
        keyCodeMap["backspace"] = 0x0008;
        keyCodeMap["F0"]    = 0x0000; 
        keyCodeMap["F1"]    = 0xe03b;
        keyCodeMap["F2"]    = 0xe03c;
        keyCodeMap["F3"]    = 0xe03d;
        keyCodeMap["F4"]    = 0xe03e;
        keyCodeMap["F5"]    = 0xe03f;
        keyCodeMap["F6"]    = 0xe040;
        keyCodeMap["F7"]    = 0xe041;
        keyCodeMap["F8"]    = 0xe042;
        keyCodeMap["F9"]    = 0xe043;
        keyCodeMap["F10"]   = 0xe044;
        keyCodeMap["F11"]   = 0xe085;
        keyCodeMap["F12"]   = 0xe086;
        keyCodeMap["del"]   = 0xe053;
        keyCodeMap["page_down"] = 0xe051;
        keyCodeMap["page_up"] = 0xe049;
        keyCodeMap["end"] = 0xe04f;
        keyCodeMap["insert"] = 0xe052;
        keyCodeMap["enter"] = 0x000d;
        keyCodeMap["escape"] = 0x001b;
#else        
        // This keycode map reflects my german sgi keyboard !!
        // Not everything makes sense, but it works :)
        keyCodeMap["down"]  = KEY_DOWN;
        keyCodeMap["up"]    = KEY_UP;
        keyCodeMap["left"]  = KEY_LEFT;
        keyCodeMap["right"] = KEY_RIGHT;
        keyCodeMap["home"]  = KEY_HOME;
        keyCodeMap["backspace"] = KEY_BACKSPACE;
        keyCodeMap["F0"]    = KEY_F0; 
        keyCodeMap["F1"]    = KEY_F(1);
        keyCodeMap["F2"]    = KEY_F(2);
        keyCodeMap["F3"]    = KEY_F(3);
        keyCodeMap["F4"]    = KEY_F(4);
        keyCodeMap["F5"]    = KEY_F(5);
        keyCodeMap["F6"]    = KEY_F(6);
        keyCodeMap["F7"]    = KEY_F(7);
        keyCodeMap["F8"]    = KEY_F(8);
        keyCodeMap["F9"]    = KEY_F(9);
        keyCodeMap["F10"]   = KEY_F(10);
        keyCodeMap["F11"]   = KEY_F(11);
        keyCodeMap["F12"]   = KEY_F(12);
        keyCodeMap["del"]   = KEY_DC;
        keyCodeMap["page_down"] = KEY_NPAGE;
        keyCodeMap["page_up"] = KEY_PPAGE;
        keyCodeMap["end"] = KEY_EIC;
        keyCodeMap["insert"] = KEY_IC;
        keyCodeMap["enter"] = KEY_ENTER;
        keyCodeMap["escape"] = 27;
#endif
    }

     // initialize key map
    keyMap[MOVE_X_PLUS] = keyCodeMap["up"];
    keyMap[MOVE_X_MINUS] = keyCodeMap["down"];
    keyMap[MOVE_Y_PLUS] = keyCodeMap["left"];
    keyMap[MOVE_Y_MINUS] = keyCodeMap["right"];
    keyMap[MOVE_Z_PLUS] = keyCodeMap["page_up"];
    keyMap[MOVE_Z_MINUS] = keyCodeMap["page_down"];
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
    keyMap[RESET] = 'w';
    keyMap[QUIT] = 'q';
}

// This method is called to construct a new Node.

Node * ConsoleModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("ConsoleSink") == 0 )
    {
        int val = 1;        
        if( attributes.get("active").compare("off") == 0)
            val = 0;
        ConsoleSink * sink = new ConsoleSink( attributes.get("comment"), val );
        sinks.push_back( sink );
        cout << "Built ConsoleSink node." << endl;       
        return sink;
    } else if( name.compare("ConsoleSource") == 0 )
    {
        int number;
        if( sscanf( attributes.get("number").c_str()," %i", &number ) != 1 )
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

// pushes new events into the tracker tree

void ConsoleModule::pushState()
{
    ConsoleSource * source;
    float data[4];

    // read all keyboard events and execute their functions
    // this may change various sources
    int key;
#ifdef WIN32
    while( _kbhit() )
    {   
        key = _getch();
        if( key == 0 || key == 0xE0 )
        {
            key = 0xE0 << 8;
            key |= _getch();
        }
#else    
    while( (key = getch()) != ERR )
    {
#endif 
        int index = find(keyMap.begin(), keyMap.end(), key ) - keyMap.begin();
        switch( index )
        {
            case QUIT : 
                quit = 1;
                break;
            case ACCELL : 
                angularSpeed += 0.01f;
                posSpeed += 0.01f;
                break;
            case BRAKE :
                angularSpeed -= 0.01f;
                if( angularSpeed < 0 ) angularSpeed = 0;
                posSpeed -= 0.01f;
                if( posSpeed < 0 ) posSpeed = 0;
                break;
            case STATION_0 :
                station = 0;
                break;
            case STATION_1 :
                station = 1;
                break;
            case STATION_2 :
                station = 2;
                break;
            case STATION_3 :
                station = 3;
                break;
            case STATION_4 :
                station = 4;
                break;
            case STATION_5 :
                station = 5;
                break;
            case STATION_6 :
                station = 6;
                break;
            case STATION_7 :
                station = 7;
                break;
            case STATION_8 :
                station = 8;
                break;
            case STATION_9 :
                station = 9;
                break;
            case BUTTON_1 :
                setButton( station , 1 );
                break;
            case BUTTON_2 :
                setButton( station , 2 );
                break;
            case BUTTON_3 :
                setButton( station , 3 );
                break;
            case BUTTON_4 :
                setButton( station , 4 );
                break;
            case MOVE_X_MINUS :
                data[0] = - posSpeed;
                data[1] = 0;
                data[2] = 0;
                movePos( station, data );
                break;
            case MOVE_X_PLUS :
                data[0] = posSpeed;
                data[1] = 0;
                data[2] = 0;
                movePos( station, data );
                break;
            case MOVE_Y_MINUS :
                data[0] = 0;
                data[1] = - posSpeed;
                data[2] = 0;
                movePos( station, data );
                break;
            case MOVE_Y_PLUS :
                data[0] = 0;
                data[1] = posSpeed;
                data[2] = 0;
                movePos( station, data );
                break;
            case MOVE_Z_MINUS :
                data[0] = 0;
                data[1] = 0;
                data[2] = - posSpeed;
                movePos( station, data );
                break;
            case MOVE_Z_PLUS :
                data[0] = 0;
                data[1] = 0;
                data[2] = posSpeed;
                movePos( station, data );
                break;
            case ROT_X_PLUS :
                data[0] = 1;
                data[1] = 0;
                data[2] = 0;
                data[3] = angularSpeed;
                MathUtils::axisAngleToQuaternion( data, data );
                rotate( station, data );
                break;
            case ROT_X_MINUS :
                data[0] = 1;
                data[1] = 0;
                data[2] = 0;
                data[3] = -angularSpeed;
                MathUtils::axisAngleToQuaternion( data, data );
                rotate( station, data );
                break;
            case ROT_Y_PLUS :
                data[0] = 0;
                data[1] = 1;
                data[2] = 0;
                data[3] = angularSpeed;
                MathUtils::axisAngleToQuaternion( data, data );
                rotate( station, data );
                break;
            case ROT_Y_MINUS :
                data[0] = 0;
                data[1] = 1;
                data[2] = 0;
                data[3] = -angularSpeed;
                MathUtils::axisAngleToQuaternion( data, data );
                rotate( station, data );
                break;
            case ROT_Z_PLUS :
                data[0] = 0;
                data[1] = 0;
                data[2] = 1;
                data[3] = angularSpeed;
                MathUtils::axisAngleToQuaternion( data, data );
                rotate( station, data );
                break;
            case ROT_Z_MINUS :
                data[0] = 0;
                data[1] = 0;
                data[2] = 1;
                data[3] = -angularSpeed;
                MathUtils::axisAngleToQuaternion( data, data );
                rotate( station, data );
                break;
            case RESET :
                reset( station );
                break;
        }
    }

    // check for changed sources and let them generate events
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    {
        source = (ConsoleSource *)(*it);
        if( source->changed == 1 )
        {          
            source->state.timeStamp();
            source->updateObservers( source->state );
            source->changed = 0;
        }
    }
}

// toggles button values on stations sources

void ConsoleModule::setButton( int station , int button )
{
    ConsoleSource * source;
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    {
        source = (ConsoleSource *)(*it);
        if( source->number == station )
        {            
            source->state.button ^= ( 1 << ( button - 1 ));
            source->changed = 1;

        }
    }
}

// moves stations sources by given amount

void ConsoleModule::movePos( int station , float * data )
{
    ConsoleSource * source;
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    {
        source = (ConsoleSource *)(*it);
        if( source->number == station )
        {
            source->state.position[0] += data[0];
            source->state.position[1] += data[1];
            source->state.position[2] += data[2];
            source->changed = 1;
        }
    }
}

// rotates stations sources by given amount

void ConsoleModule::rotate( int station, float * data )
{
    ConsoleSource * source;
    float help[4];
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    {
        source = (ConsoleSource *)(*it);
        if( source->number == station )
        {
            memcpy(help, source->state.orientation, sizeof( help ));
            MathUtils::multiplyQuaternion( data, help, source->state.orientation );
            source->changed = 1;
        }
    }
}

// resets a given stations sources to null position 

void ConsoleModule::reset( int station )
{
    ConsoleSource * source;
    State identity;
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    {
        source = (ConsoleSource *)(*it);
        if( source->number == station )
        {
            source->state = identity;
            source->changed = 1;
        }
    }
}

// pulls events out of the tracker tree

void ConsoleModule::pullState()
{
    if( sinks.size() <= 0 || isInitialized() == 0 || display == 0 )
    {
        return;
    }
    cycle = (cycle+1) % interval;
    if( cycle == 0 ){
        int display = 0, count = 0;
        NodeVector::iterator it;
        for( it = sinks.begin(); it != sinks.end(); it++ )
        {  
            display |= ((ConsoleSink *)(*it))->changed;
            ((ConsoleSink *)(*it))->changed = 0;
            count += ((ConsoleSink *)(*it))->active;
        }
        if( !display )
            return;
#ifndef WIN32
        move(0,0);            
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
        int lines = count * 6 + 2;
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
        cout << headerline << endl;
        cout << "    Station : " << station << " PosSpeed : " << posSpeed << " RotSpeed : " << angularSpeed << endl;
#else
        printw("%s\n\n", headerline.c_str());
#endif
        for( it = sinks.begin(); it != sinks.end(); it++ )
        {
            ConsoleSink * sink = (ConsoleSink *) *it;
            if( sink->active == 0 )
                continue;
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
            printw("%s :\n",sink->comment.c_str());
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

void ConsoleModule::init(StringTable& attributes,  ConfigNode * localTree)
{    
    if( attributes.get("interval", &interval ) == 0 ){
        interval = 10;
    }
    headerline = attributes.get("headerline");    
    if( attributes.get("display").compare( "off" ) == 0 )
        display = 0;
    if( localTree != NULL )
    {
        ConfigNode * base = localTree;
        for( unsigned int i = 0; i < base->countChildren(); i++ )
        {
            ConfigNode * config = (ConfigNode *)base->getChild( i );
            if( config->getType().compare("KeyDefinition") == 0 )
            {
                string function = config->getAttributes().get("function");
                string key = config->getAttributes().get("key");
                vector<string>::iterator funcIt = find( functionMap.begin(), functionMap.end(), function );                                
                if( funcIt != functionMap.end() )
                {
                    int index = funcIt - functionMap.begin();
                    map<string,int>::iterator codeIt = keyCodeMap.find( key );
                    int code;
                    if( codeIt == keyCodeMap.end())
                    {
                        int num = sscanf( key.c_str(), " %x", &code );
                        if( num == 0 )
                        {
                            code = key[0];
                        }
                    }
                    else {
                        code = (*codeIt).second;
                    }
                    keyMap[index] = code;
                }                 
            }
        }
    }
    Module::init( attributes, localTree );
}

// start the module and init curses

void ConsoleModule::start()
{
#ifndef WIN32
    if( sinks.size() > 0 || sources.size() > 0 )
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
    return quit;
}
