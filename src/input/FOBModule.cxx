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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/FOBModule.cxx,v 1.8 2002/01/24 17:31:07 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "FOBSource.h"
#include "FOBModule.h"
#include "../misc/serialcomm.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
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
    bool newVal;

    /// total range of positions
    float scale;

    /// constructor 
    Bird( int number_, const string & device_, float scale_ ) :
        number( number_ )
    {
        source = NULL;
        count = 0;
        newVal = false;
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

    /** reads data from an input buffer into the internal buffer.
     * this method looks for the phasing bit and then starts to read 
     * in data until the input buffer ends, or it has read a whole 
     * frame. If the frame is completed it sets the flag newVal to true.
     * @param buffer input buffer
     * @param len length of the input buffer    
     * @param framesize size of the frame for the current mode
     * @returns the number of chars read from the buffer
     */
    inline int parse( const char * buffer, int len, int framesize );
    
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

    // some bird protocol stuff
    inline int setGroupMode( bool value );

    inline int getErrorCode();

    inline int autoConfig( int number );

    inline int sleep();

};


// constructor method.
FOBModule::FOBModule() : ThreadModule(), NodeFactory()
{
}

// Destructor method, clears nodes member.
FOBModule::~FOBModule()
{
    map<int, Bird *>::iterator it;
    for( it = birds.begin(); it != birds.end(); it++)
    {
        delete it->second;
    }
    birds.clear();
}

// initializes the tracker module. 
void FOBModule::init(StringTable& attributes,  ConfigNode * localTree)
{
    // checking mode parameter
    if( attributes.get("mode").compare("multi") == 0 )
        mode = MULTI;
    else
        mode = SINGLE;
    
    // getting master parameter
    if( attributes.get("master", &master ) != 1 )
    {
        cout << "FOBModule : error in master parameter " << attributes.get("master").c_str() << endl;
        return;
    }
    
    // getting scale
    if( attributes.get("scale", &scale ) != 1 )
        scale = 36;
    
    // getting erc transmitter
    if( attributes.get("transmitter", &transmitter ) != 1 )
        transmitter = -1;
    else        
        scale = 144;

    // getting hemisphere
    if( attributes.containsKey("hemisphere"))
    {    
        if( attributes.get("hemisphere").compare("forward") == 0 )
            hemisphere = FORWARD;
        else if ( attributes.get("hemisphere").compare("rear") == 0 )
            hemisphere = REAR;
        else if ( attributes.get("hemisphere").compare("upper") == 0 )
            hemisphere = UPPER;
        else if ( attributes.get("hemisphere").compare("lower") == 0 )
            hemisphere = LOWER;
        else if ( attributes.get("hemisphere").compare("left") == 0 )
            hemisphere = LEFT;
        else if ( attributes.get("hemisphere").compare("right") == 0 )
            hemisphere = RIGHT;
    }       

    // getting reference frame
    float angles[3];
    if( attributes.get("referenceframe", angles, 3 ) == 3 )
    {
        
    }

    // getting anglealign
    if( attributes.get("anglealign", angles, 3 ) == 3 )
    {
    
    }

    // parsing birds
    for( unsigned int i = 0; i < localTree->countChildren(); i++ )
    {
        ConfigNode * child = (ConfigNode *)localTree->getChild(i);
        StringTable & childAttr = child->getAttributes();
        int number;
        if( childAttr.get("number", &number ) != 1 )
        {
            cout << "FOBModule : error parsing Bird " << i << " with number " 
                 << childAttr.get("number").c_str() << endl;
            continue;
        }
        Bird * bird = new Bird( number, childAttr.get("device"), scale );
        // birds.reserve(number+1);
        birds[number] = bird;
        cout << "created bird " << number << " on dev " << childAttr.get("device") <<endl;
    }
    
    if( birds.find(master) == birds.end())
    {
        cout << "FOBModule : error no master bird " << master << " present\n";
        return;
    }
    ThreadModule::init( attributes, localTree );
}

// This method is called to construct a new Node 
Node * FOBModule::createNode( const std::string& name,  StringTable& attributes)
{
    if( name.compare("FOBSource") == 0 )
    {
        int number;
        if( attributes.get("number", &number ) != 1 )
        {
            cout << "FOBModule : error reading FOBSource number !\n";
            return NULL;
        }
        if( birds.find(number) == birds.end())
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
                cout << "FOBModule : error opening " << master
                     << " port for master " << birds[master]->number << endl;
                initialized = 0;
                return;
            }
        } else {
            map<int, Bird *>::iterator it;
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                if( it->second->open() < 0 )
                {
                    cout << "FOBModule : error opening port for " << it->first << endl;
                    initialized = 0;
                    return;
                }
            }           
        }
        ThreadModule::start();
    }
}

// runs the whole initialization sequence
int FOBModule::initFoB()
{
    Bird * masterBird = birds[master];
    char buffer[5];
    int result;

    // reset birds
    if((result = resetBirds()) != 0 )
    {
        cout << "FOBModule : error reseting birds " << result << endl;
        return result;
    }
    OSUtils::sleep(500);

    if((result = masterBird->setGroupMode( false )) != 0 )
    {
        cout << "FOBModule : error clear group mode " << result << endl;
        return result;
    }
    OSUtils::sleep(500);

    if((result = masterBird->autoConfig((transmitter > birds.rbegin()->first)?(transmitter):(birds.rbegin()->first))) 
         != 0 )
    {
        cout << "FOBModule : error sending autoconfig " << result << endl;
        return result;
    }
    OSUtils::sleep(500);

    if((result = setReportMode()) != 0 )
    {
        cout << "FOBModule : error setting report mode " << result << endl;
        return result;
    }
    OSUtils::sleep(500);

    // set all kinds of other parameters here
    if( scale == 72 )
    {
        buffer[0] = 'P';
        buffer[1] = 0x3;
        buffer[2] = 0;
        buffer[3] = 1;
        masterBird->write( buffer, 3 );
        OSUtils::sleep( 600 );
    }
    OSUtils::sleep(500);

    if( hemisphere != FORWARD )
    {
        buffer[0] = 'L';
        switch( hemisphere )
        {
            case REAR:
                buffer[1] = 0x00;
                buffer[2] = 0x01;
                break;
            case UPPER:
                buffer[1] = 0x0c;
                buffer[2] = 0x01;
                break;
            case LOWER:
                buffer[1] = 0x0c;
                buffer[2] = 0x00;
                break;
            case LEFT:
                buffer[1] = 0x06;
                buffer[2] = 0x01;
                break;
            case RIGHT:
                buffer[1] = 0x06;
                buffer[2] = 0x00;
                break;
        }
        masterBird->write( buffer, 3 );
        OSUtils::sleep( 600 );
    }
    OSUtils::sleep(500);

    if((result = setNextTransmitter()) != 0 )
    {
        cout << "FOBModule : error setting transmitter " << result << endl;
        // return result;
    }
    OSUtils::sleep(500);

    if( mode == SINGLE )
    {
        if((result = masterBird->setGroupMode( true )) != 0 )
        {
            cout << "FOBModule : error setting group mode " << result << endl;
           // return result;
        }
    }
    OSUtils::sleep(500);
    if((result = startStreamMode()) != 0 )
    {
        cout << "FOBModule : error starting data streaming " << result << endl;
        //return result;
    }
    return 0;
}

// closes FOBModule, closing all serial ports
void FOBModule::close()
{
    // stop thread
    lock();
    stop = true;
    unlock();

    // close serial ports
    if( isInitialized() == 1 )
    {
    	birds[master]->sleep();
        if( mode == SINGLE )
        {
            birds[master]->close();
        } else {
            map<int, Bird *>::iterator it;
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                it->second->close();
            }
        }
    }
}

// this method is the code executed in its own thread
void FOBModule::run()
{
    char buffer[100];
    int len = 0;
    int count = 0;
    // the number of bytes in one data frame for group mode (single serial interface)
    const int GROUP_FRAME_SIZE = 15;
    // the number of bytes in one data frame for multi mode
    const int MULTI_FRAME_SIZE = 14;
    // timeout before we think that something is wrong, right now 1 second
    const double DATA_TIME_OUT = 1000 * 1;
    // number of allowed failures 
    const int MAX_FAILURES = 5;

    int failure;

    initFoB();

    unsigned int iter = 0;    
    double startTime = OSUtils::currentTime();
    double lastDataTime = OSUtils::currentTime();

    if( mode == SINGLE )
    {
        Bird * bird = birds[master];
        int num;

        while(1)
        {
            // yield to let other processes do something
            OSUtils::sleep(1);
            iter++;            
            lock();
            if( stop == true )
            {           
                unlock();
                break;
            } else { 
                unlock();
            }      
            
            // test if something is wrong, if so do an init again 
            if((OSUtils::currentTime() - lastDataTime) > DATA_TIME_OUT ||
                failure > MAX_FAILURES )
            {
                initFoB();
                lastDataTime = OSUtils::currentTime();
                failure = 0;
            }

            // read in data and parse it 
            if( (len = bird->read( buffer, 100 )) <= 0 )
            {
                if( errno != EAGAIN )
                {
                    failure++;
                }
                continue;
            }
            lastDataTime = OSUtils::currentTime();
            while( count < len )
            {
                // let the master bird parse the buffer
                count += bird->parse( &buffer[count], len - count, GROUP_FRAME_SIZE );
                if( bird->count == GROUP_FRAME_SIZE )
                {
                    bird->count = 0;
                    num = bird->buffer[GROUP_FRAME_SIZE - 1];
                    if( birds.find(num) != birds.end())
                    {
                        lock();                    
                        birds[num]->convert( bird->buffer );
                        birds[num]->newVal = true;
                        unlock();                        
                    }
                }
            }
            count = 0;
        }
    } else {
        while(1)
        {
            // yield to let other processes do something
            OSUtils::sleep(1);
            iter++;
            lock();
            if( stop == true )
            {           
                unlock();
                break;
            } else { 
                unlock();
            }

           // test if something is wrong, if so do an init again 
            if((OSUtils::currentTime() - lastDataTime) > DATA_TIME_OUT ||
                failure > MAX_FAILURES )
            {
                initFoB();
                lastDataTime = OSUtils::currentTime();
                failure = 0;
            }

            map<int,Bird *>::iterator it;
            for( it = birds.begin(); it != birds.end(); it++ )
            {
                Bird * bird = it->second;
                if( (len = bird->read( buffer, 100 )) <= 0 )
                {
                    if( errno != EAGAIN )
                    {
                       failure++;
                    }                    
                    continue;
                }
                lastDataTime = OSUtils::currentTime();
                while( count < len )
                {
                   // let the bird itself parse the buffer
                   count += bird->parse( &buffer[count], len - count, MULTI_FRAME_SIZE );
                   if( bird->count == MULTI_FRAME_SIZE )
                   {
                        bird->count = 0;
                        lock();
                        bird->convert();
                        bird->newVal = true;
                        unlock();
                    }
                }
                count = 0;
            }
        }
    }
    cout << "FOBModule Framerate " << 1000 * iter / ( OSUtils::currentTime() - startTime ) << endl;
}
    
// pushes events into the tracker tree
void FOBModule::pushState()
{
    if( isInitialized() == 1 )
    {
        map<int,Bird *>::iterator it;
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            lock();
            if(it->second->newVal == 1 && it->second->source != NULL)
            {
                it->second->source->state = it->second->state;
                it->second->newVal = 0;
                unlock();
                it->second->source->updateObservers(it->second->source->state);
            } else
                unlock();
        }
    }
}

// toggle reset on the birds
int FOBModule::resetBirds()
{    
    if( mode == SINGLE )
    	birds[master]->reset();
    else
    {
        map<int, Bird *>::iterator it;
    	for( it = birds.begin(); it != birds.end(); it++ )
    	{
    		it->second->reset();
     	}
    }
    OSUtils::sleep(600);
    return birds[master]->getErrorCode();
}

// set report mode to POSITION/QUATERNION
int FOBModule::setReportMode()
{ 
    char buffer[2];
    map<int, Bird *>::iterator it;
    if( mode == MULTI )
    {
        // send all birds the POSITION/QUATERNION command
        buffer[0] = ']';
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            it->second->write( buffer, 1 );
        }
        OSUtils::sleep(600);
    } else {
        // send all birds the POSITION/QUATERNION command
        for( it = birds.begin(); it != birds.end(); it++ )
        {
           	buffer[0] = 0xF0+it->first;
            buffer[1] = ']';
            // TO FOB command and POS / QUAT command
            birds[master]->write( buffer, 2 );
            OSUtils::sleep(600);
        }
        // write again as it doesn't work otherwise :(
         buffer[0] = ']';
         birds[master]->write( buffer, 1 );
         OSUtils::sleep(600);
    }
    return birds[master]->getErrorCode();
}

// Turn on ERC if present (or another transmitter :)
int FOBModule::setNextTransmitter()
{
    char buffer[2];
    if( transmitter != -1 )
    {
        buffer[0] = 0x30;
        buffer[1] = (transmitter << 4) & 0xff;
        birds[master]->write( buffer, 2 );
        OSUtils::sleep( 600 );
    }
    return birds[master]->getErrorCode();
}

// start STREAM mode    
int FOBModule::startStreamMode()
{
    char buffer[2];

    if( mode == SINGLE )
    {
        // only to master bird
        buffer[0] = 0xF0+birds[master]->number;
        buffer[1] = '@';
        birds[master]->write( buffer, 2 );
    } else {
        // to all
        buffer[0] = '@';
        map<int, Bird *>::iterator it;
        for( it = birds.begin(); it != birds.end(); it++ )
        {
            it->second->write( buffer, 1 );
        }
    }
    OSUtils::sleep(500);
    // no use getting an error code :)
    return 0;
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
    state.timeStamp();
    
    for (i=0; i<3; i++)                         // x,y,z
    {
        d=(((int)(data[i*2]&0x7f))<<2)+(((int)data[i*2+1])<<9);
        if (d&0x8000)
            d|=0xffff0000;        // Extend signbit
        state.position[i] = ((float)d)*scale;
    }    
    for (i=0; i<4; i++)                         // qa,qx,qy,qz
    {
        d=(((int)(data[6+2*i]&0x7f))<<2)+(((int)data[7+2*i])<<9);
        if (d&0x8000)
            d|=0xffff0000;        // Extend signbit
        state.orientation[(i+3)%4] = ((float)d)/0x8000; // we need qx, qy, qz, qa
                                                        // so (i+3)%4 should do the
                                                        // trick. scale ?????
    }
}

int Bird::parse( const char * data, int len, int framesize )
{
    int i = 0;
    if( count == 0 )  // we are still looking for the phasing bit
    {
        for( i = 0 ; i < len ; i++ )
        {
            if( data[i] & 0x80 )
                break;
        }
        if( i == len )  // read everything but found no phasing bit, throw it away
            return len;
        buffer[0] = data[i];
        count = 1;
        i++;
    }
    // copy everything up to min(framesize - count, len - i) bytes from data[i] into buffer
    int amount = min( framesize - count, len - i );
    if( amount == 0 )
        return i;
    memcpy( &buffer[count], &data[i], amount );
    count += amount;
    return i+amount;
}

int Bird::open()
{
    SerialParams params;
    initSerialParams( &params );
    params.baudrate = 115200;
    params.parity = 0;
    params.bits = 8;
    params.sbit = 1;
    params.hwflow = 1;
    params.swflow = 0;
    params.blocking = 0;
    strncpy( params.pathname, port.pathname, 255 );
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

int Bird::setGroupMode( bool value )
{
    char buffer[3];
    buffer[0] = 'P';
    buffer[1] = 0x23;
    buffer[2] = (value)?(1):(0);
    write( buffer, 3 );
    OSUtils::sleep( 600 );
    return getErrorCode();
}

int Bird::getErrorCode()
{
    buffer[0] = 0x4f;
    buffer[1] = 10;
    write( buffer, 2 );
    OSUtils::sleep(100);
    buffer[0] = 0;
    int count = 0;
    while( read( buffer, 1 ) < 0 && count < 10)
    {
    	if( errno != EAGAIN )
            return -1;	
        count++;
        OSUtils::sleep(10);
    }
    return buffer[0];
}

int Bird::autoConfig( int number )
{
    char buffer[3];
    buffer[0] = 'P';
    buffer[1] = 0x32;
    buffer[2] = number;
    write( buffer, 3 );
    OSUtils::sleep(600);
    return getErrorCode();
}

int Bird::sleep()
{
    char buffer[4] = "GGG";
    write( buffer, 3 );
    OSUtils::sleep(300);
    return getErrorCode();
}