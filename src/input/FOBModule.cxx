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
/** source file for FOBModule module.
  *
  * @author Thomas Peterseil, Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/FOBModule.cxx,v 1.2 2001/10/20 17:24:29 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "FOBSource.h"
#include "FOBModule.h"
#include "../misc/serial.h"

#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <iostream>    /// VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

using namespace std;

/** This class is a datatype helper class for the FOBModule class. It stores 
 * the relevant data for a single bird station and provides buffer storage and
 * conversion routines.
 * @author Gerhard Reitmayr */
class Bird {
public:
    /// the bird number
    int number;
    
    /// port structure for the serial port data
    SerialPort port;

    /// data buffer for incoming data
    char buffer[1024];

    /// pointer to the current position in the buffer
    char * current;

    /// associated source node
    FOBSource * source;

    /// the local state buffer state
    State state;

    /// flag indicating a new value in state
    int newVal;

    /// constructor 
    Bird( int number_, const string & device_ ) :
        number( number_ )
    {
        source = NULL;
        current = buffer;
        newVal = 0;
        strncpy( port.pathname, device_.c_str(), 255 );
    }
    
    /** converts the buffer data and writes it to
      * the associated source node, if present */
    void convert();
    
    /** opens the serial port associated with the bird.
     * @returns the error value from the serial library. */
    inline int open();

    /** writes data to the serial port associated with the 
     * bird.
     * @param data pointer to the char data to write
     * @param count number of chars to write
     * @returns the error value from the serial library. */
    inline int write( const char * data, int count );

    /** reads data from the serial port associated with the 
     * bird.
     * @param data pointer to the char data buffer to read into
     * @param count maximal number of chars to read
     * @returns the error value from the serial library. */
    inline int read( char * data, int count );

    /** closes the serial port associated with the bird.
     * @returns the error value from the serial library. */
    inline int close();
};

// constructor method.
FOBModule::FOBModule() : ThreadModule(), NodeFactory()
{
}

// Destructor method, clears nodes member.
FOBModule::~FOBModule()
{
    vector<Bird *>::iterator it;
    for( it = birds.begin(); it != birds.end(); it++)
    {
        delete (*it);
    }
    birds.clear();
}

// initializes the tracker module. 
void FOBModule::init(StringTable& attributes,  ConfigNode * localTree)
{
    int num;

    // checking mode parameter
    if( attributes.get("mode").compare("multi") == 0 )
        mode = MULTI;
    else
        mode = SINGLE;
    
    // getting master parameter
    num = sscanf( attributes.get("master").c_str(), " %i", &master );
    if( num != 1 )
    {
        cout << "FOBModule : error in master parameter " << attributes.get("master").c_str() << endl;
        return;
    }
    
    // getting erc transmitter

    // getting hemisphere

    // getting reference frame

    // getting anglealign

    // parsing birds
    for( unsigned int i = 0; i < localTree->countChildren(); i++ )
    {
        ConfigNode * child = (ConfigNode *)localTree->getChild(i);
        StringTable & childAttr = child->getAttributes();
        int number;
        num = sscanf( childAttr.get("number").c_str(), " %i", &number );
        if( num != 1 )
        {
            cout << "FOBModule : error parsing Bird " << i << " with number " 
                 << childAttr.get("number").c_str() << endl;
            continue;
        }
        Bird * bird = new Bird( number, childAttr.get("device"));
        birds[number] = bird;
    }
    
    if( birds[master] != NULL )
    {
        cout << "FOBModule : error no master bird " << master << " present\n";
        return;
    }
    ThreadModule::init( attributes, localTree );
    cout << "FOBModule : initialized !\n";
}

// This method is called to construct a new Node 
Node * FOBModule::createNode( const std::string& name,  StringTable& attributes)
{
    if( name.compare("FOBSource") == 0 )
    {
        int num, number;
        num = sscanf( attributes.get("number").c_str(), " %i", &number );
        if( num != 1 )
        {
            cout << "FOBModule : error reading FOBSource number !\n";
            return NULL;
        }
        if( birds[number] == NULL )
        {
            cout << "FOBModule : no Bird with number " << number << "present\n";
            return NULL;
        }
        FOBSource * source = new FOBSource();
        birds[number]->source = source;
        cout << "Build FOBSource for " << number << endl;
        return source;
    }
    return NULL;
}
 
// starts the module and opens all the devices etc.   
void FOBModule::start()
{
    if( isInitialized() == 1 )
    {
        if( mode == SINGLE )
        {
            if( birds[master]->open() < 0 )
            {
                cout << "FOBModule : error opening port " << master << endl;
                return;
            }
        } else {
            vector<Bird *>::iterator it;
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                if( (*it) != NULL )
                {
                    if( (*it)->open() < 0 )
                    {
                        cout << "FOBModule : error opening port for " << (*it)->number << endl;
                        return;
                    }
                }
            }           
        }
        initFoB();
        ThreadModule::start();
    }
}

// runs the whole initialization sequence
int FOBModule::initFoB()
{
    vector<Bird *>::iterator it;
    Bird * masterBird = birds[master];

    // toggle reset on master bird

    if( mode == MULTI )
    {
        // send all birds the POSITION/QUATERNION command
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            (*it)->write( , );
        }
    } else {
        // send all birds the POSITION/QUATERNION command
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            // TO FOB command
            masterBird->write( , );
            // POS/QUAT command
            masterBird->write( , );
        }
    }
    // start up birds
}

// closes FOBModule, closing all serial ports
void FOBModule::close()
{
    // stop thread
    lock();
    stop = 1;
    unlock();

    // close serial ports
    if( isInitialized() == 1 )
    {
        if( mode == SINGLE )
        {
            birds[master]->close();
        } else {
            vector<Bird *>::iterator it;
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                if( (*it) != NULL )
                {
                    (*it)->close();
                }
            }
        }
    }
}

// this method is the code executed in its own thread
void FOBModule::run()
{
    if( mode == SINGLE )
    {
        Bird * bird = birds[master];
        char buffer[10000];
        while(1)
        {
            lock();
            if( stop == 1 )
            {           
                unlock();
                break;
            } else { 
                unlock();
            }
            if( waitforoneSerialPort( &bird->port, 1000 ) < 0 )
            {
                cout << "FOBModule : error waiting for port\n";
                break;
            }
            // read in data and parse it 
        }
    } else {
        // prepare handles for the serial ports
        SerialPort * ports[32];
        int count = 0;
        int portSet;

        vector<Bird *>::iterator it;
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            if((*it) != NULL )
            {
                ports[count] = &(*it)->port;
                count ++;
            }
        }
        while(1)
        {
            lock();
            if( stop == 1 )
            {           
                unlock();
                break;
            } else { 
                unlock();
            }
            portSet = 0;
            if( waitforallSerialPorts( ports, count, &portSet, 1000 ) < 0 )
            {
                cout << "FOBModule : error waiting for port\n";
                break;
            }
            // read in data and parse it 
        }
    }

}
    
// pushes events into the tracker tree
void FOBModule::pushState()
{
    if( isInitialized() == 1 )
    {
        vector<Bird *>::iterator it;
        for( it = birds.begin(); it < birds.end(); it++ )
        {
            if( (*it) != NULL )
            {
                lock();
                if((*it)->newVal == 1 )
                {
                    (*it)->source->state = (*it)->state;
                    (*it)->newVal = 0;
                    unlock();
                    (*it)->source->updateObservers((*it)->source->state);
                } else
                    unlock();
            }
        }
    }
}

// convert Bird data to OpenTracker State format
void Bird::convert()
{
}

int Bird::open()
{
    SerialParams params;
    initSerialParams( &params );
    return openSerialPort( &port, &params );
}

int Bird::write( const char * data, int count )
{
    return readfromSerialPort( &port, data, count );
}

int Bird::read( char * data, int count )
{
    return readfromSerialPort( &port, data, count );
}

int Bird::close()
{
    return closeSerialPort(&port);
}