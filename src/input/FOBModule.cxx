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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/FOBModule.cxx,v 1.4 2001/11/22 12:37:30 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "FOBSource.h"
#include "FOBModule.h"
#include "../misc/serialcomm.h"

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

    /// constant to convert inches to meters
    static const float inchesToMeters ; //= 0.0254;

    /// the bird number
    int number;
    
    /// port structure for the serial port data
    SerialPort port;

    /// data buffer for incoming data
    char buffer[20];

    /// number of the current position in the buffer
    int count;

    /// associated source node
    FOBSource * source;

    /// the local state buffer state
    State state;

    /// flag indicating a new value in state
    int newVal;

    /// total range of positions
    float scale;

    /// constructor 
    Bird( int number_, const string & device_, float scale_ ) :
        number( number_ )
    {
        source = NULL;
        count = 0;
        newVal = 0;
        scale = scale_ * inchesToMeters / 32767;
        state.confidence = 1;
        strncpy( port.pathname, device_.c_str(), 255 );
    }
    
    /** converts the buffer data and writes it to
      * the local state member */
    inline void convert();

    /** converts data from another buffer and writes it
     * to the local state member. It assumes that data is
     * in the POSITION/QUATERNION format.
     * @param buffer pointer to the char buffer storing
     * the input data. */
    inline void convert( const char * buffer );
    
    /** opens the serial port associated with the bird.
     * @return the error value from the serial library. */
    inline int open();

    /** writes data to the serial port associated with the 
     * bird.
     * @param data pointer to the char data to write
     * @param count number of chars to write
     * @return the error value from the serial library. */
    inline int write( const char * data, int count );

    /** reads data from the serial port associated with the 
     * bird.
     * @param data pointer to the char data buffer to read into
     * @param count maximal number of chars to read
     * @return the error value from the serial library. */
    inline int read( char * data, int count );

    /** closes the serial port associated with the bird.
     * @return the error value from the serial library. */
    inline int close();

    /** toggles some lines on the serial interface to 
     * reset the bird. 
     * @return the error value from the serial library. */
    inline int reset();
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
    float scale = 144;

    // getting hemisphere

    // getting reference frame

    // getting anglealign

    // getting scale

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
        Bird * bird = new Bird( number, childAttr.get("device"), scale );
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
    char buffer[5];

    // toggle reset on master bird
    masterBird->reset();
    
    // set report mode
    if( mode == MULTI )
    {
        // send all birds the POSITION/QUATERNION command
        buffer[0] = ']';
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            (*it)->write( buffer, 1 );
        }
        OSUtils::sleep(1000);
    } else {
        // send all birds the POSITION/QUATERNION command
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            buffer[0] = 0xF0+(*it)->number;
            buffer[1] = ']';
            // TO FOB command and POS / QUAT command
            masterBird->write( buffer, 2 );
            OSUtils::sleep(600);
        }
    }
    
    // set all kinds of other parameters here


    if( mode == SINGLE )
    {
        // GROUP mode to master bird
        buffer[0] = 'P';
        buffer[1] = 0x23;
        buffer[2] = 1;
        masterBird->write( buffer, 3 );
        OSUtils::sleep( 600 );
    }
    // AUTOCONFIG to master bird
    buffer[0] = 'P';
    buffer[1] =  0x32;
    buffer[2] = (*birds.end())->number;
    masterBird->write( buffer, 3 );
    OSUtils::sleep( 600 );
    // start STREAM mode
    buffer[1] = '@';
    if( mode == SINGLE )
    {
        // only to master bird
        buffer[0] = 0xF0+masterBird->number;
        masterBird->write( buffer, 2 );
    } else {
        // to all
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            buffer[0] = 0xF0+(*it)->number;
            (*it)->write( buffer, 2 );
        }
    }
    return 0;
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
        char buffer[100];
        int count = 0;
        int maxBytes = 15; // whatever is the correct number for pos/quat
        int num;
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
            if( bird->read( buffer + count, 1 ) < 0 )
            {
                cout << "FOBModule : error reading from port\n";
                break;
            }
            // wait for phasing bit to start a full record
            if( count == 0 && !( buffer[0] & 0x80))
            {
                continue;
            }
            // if we got a phasing bit before the whole record was read
            // trash the old buffer and start again.
            if( count > 0 && ( buffer[count] & 0x80 ))
            {
                buffer[0] = buffer[count];
                count = 0;
                continue;
            }
            count++;
            if( count == maxBytes )
            {
                // we got a full buffer, set the data of the addressed bird
                lock();
                birds[buffer[count - 1]]->convert( buffer );
                birds[buffer[count - 1]]->newVal = 1;
                unlock();
                count = 0;
            }
        }
    } else {
        // prepare handles for the serial ports
        SerialPort * ports[32];
        int numBirds = 0;
        int portSet, i;
        int maxBytes = 14;

        vector<Bird *>::iterator it;
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            if((*it) != NULL )
            {
                ports[numBirds] = &(*it)->port;
                numBirds ++;
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
         //   if( waitforallSerialPorts( ports, numBirds, &portSet, 1000 ) < 0 )
            {
                cout << "FOBModule : error waiting for port\n";
                break;
            }
            // read in data and parse it 
            for( i = 0; i < numBirds; i ++ )
            {
              /*  if( portSet & i )
                {
                    // get the right bird here, this is just an assumption
                    Bird * bird = birds[i+1];
                    // read in data and parse it 
                    if( bird->read( bird->buffer + bird->count, 1 ) < 0 )
                    {
                        cout << "FOBModule : error reading from bird " << bird->number << endl;
                        break;
                    }
                    // wait for phasing bit to start a full record
                    if( bird->count == 0 && !( bird->buffer[0] & 0x80))
                    {
                        continue;
                    }
                    // if we got a phasing bit before the whole record was read
                    // trash the old buffer and start again.
                    if( bird->count > 0 && ( bird->buffer[bird->count] & 0x80 ))
                    {
                        bird->buffer[0] = bird->buffer[count];
                        count = 0;
                        continue;
                    }
                    count++;
                    if( count == maxBytes )
                    {
                        // we got a full buffer, set the data of the addressed bird
                        lock();
                        birds[buffer[count - 1]].convert( buffer );
                        birds[buffer[count - 1]].newVal = 1;
                        unlock();
                        count = 0;
                    }                    
                }
                */
            }
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

const float Bird::inchesToMeters = 0.0254f;

// convert Bird data in buffer to OpenTracker State format
void Bird::convert()
{
    convert( buffer );
}

void Bird::convert( const char * data )
{
    int d,i;

    for (i=0; i<3; i++)                         // x,y,z
    {
        d=(data[i*2]&0x7f)+(data[i*2+1]<<7);
        if (d&0x2000) 
            d|=0xffffc000;        // Extend signbit
        state.position[i] = ((float)d)*scale;
    }    
    for (i=0; i<4; i++)                         // qa,qx,qy,qz
    {
        d=(data[6+2*i]&0x7f)+(data[7+2*i]<<7);
        if (d&0x2000) 
            d|=0xffffc000;        // Extend signbit
        state.orientation[(i+3)%4] = ((float)d)/0x8000; // we need qx, qy, qz, qa
                                                        // so (i+3)%4 should do the
                                                        // trick. scale ?????
    }
    state.timeStamp();
}

int Bird::open()
{
    SerialParams params;
    initSerialParams( &params );
    params.baudrate = 115200;
    params.parity = 0;
    params.bits = 8;
    params.sbit = 1;
    params.hwflow = 0;
    params.swflow = 0;
    params.blocking = 0;
    return openSerialPort( &port, &params );
}

int Bird::write( const char * data, int count )
{
    return writetoSerialPort( &port,(char *) data, count );
}

int Bird::read( char * data, int count )
{
    return readfromSerialPort( &port, data, count );
}

int Bird::close()
{
    return closeSerialPort(&port);
}

int Bird::reset()
{
    setRTSSerialPort( &port, 1);
    OSUtils::sleep(100);
    return setRTSSerialPort( &port, 0);    
}