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
/** source file for FastTrakModule module.
  *
  * @author Rainer Splechtna 
  *
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"
#include "../tool/OT_ACE_Log.h"

#include <ace/Log_Msg.h>

#include "FastTrakSource.h"
#include "FastTrakModule.h"
#include "../misc/serialcomm.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <ace/Log_Msg.h>

//using namespace std;

namespace ot {

const int FASTTRAK = 1;
const int ISOTRAK = 2;

// constructor method.
FastTrakModule::FastTrakModule() : ThreadModule(), NodeFactory(), stations( NULL )
{
}

// Destructor method, clears nodes member.
FastTrakModule::~FastTrakModule()
{
	if( stations != NULL )
		delete[] stations;
    nodes.clear();
}

// initializes the tracker module. 
void FastTrakModule::init(StringTable& attributes,  ConfigNode * localTree)
{
    int num;
    // getting number of stations parameter
    num = sscanf( attributes.get("stations").c_str(), " %i", &numberOfStations);
    if( num != 1 )
    {
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FastTrakModule : can't read attribute\"stations\"\n")));
        exit(-1);
    }
    if (numberOfStations < 1)
    {
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FastTrakModule : attribute \"stations\" invalid\n")));
        exit(-1);
    }

    stations = new tmpStationState[numberOfStations];

    if (attributes.get("type").compare("fasttrak") == 0)
        trackerType = FASTTRAK;
    else if (attributes.get("type").compare("isotrak") == 0)
        trackerType = ISOTRAK;
    else
    {
		LOG_ACE_ERROR("ot:FastTrakModule : unknown trackertype %s\n", attributes.get("type"));
        exit(-1);
    }

    strncpy( port.pathname, attributes.get("device").c_str(), 255 );

    hemisphere = attributes.get("hemisphere");
    referenceFrame = attributes.get("reference-frame");
    initString = attributes.get("init-string");
    
    ThreadModule::init( attributes, localTree );
	LOG_ACE_INFO("ot:FastTrakModule : initialized !\nusing tracker protocol for %s\n", attributes.get("type"));
}

// This method is called to construct a new Node 
Node * FastTrakModule::createNode( const std::string& name,  StringTable& attributes)
{
    if( name.compare("FastTrakSource") == 0 )
    {
        int num, number;
        num = sscanf( attributes.get("number").c_str(), " %i", &number );
        if( num != 1 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FastTrakModule : error reading FastTrakSource number !\n")));
            return NULL;
        }
        if ((number < 0) || (number > numberOfStations-1))
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FastTrakModule : number out of range 0 to %d\n"), numberOfStations-1));
            return NULL;
        }
        FastTrakSource * source = new FastTrakSource(number);
        nodes.push_back( source );

		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Build FastTrakSource for %d\n"), number));
        return source;
    }
    return NULL;
}
 
// starts the module and opens all the devices etc.   
void FastTrakModule::start()
{
    if( isInitialized() == 1 )
    {
        SerialParams params;
        initSerialParams( &params );
        strcpy(params.pathname, port.pathname);
        params.baudrate = 115200;
        params.parity = 0;
        params.bits = 8;
        params.sbit = 1;
        params.hwflow = 1;
        params.swflow = 0;
        params.blocking = 0;

        if( openSerialPort( &port, &params ) < 0 )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FastTrakModule : error opening port\n")));
           initialized = 0;
           return;
        }

        ThreadModule::start();
    }
}

// runs the whole initialization sequence
int FastTrakModule::initFastTrak()
{
    char buffer[256];

    // toggle reset
    //setRTSSerialPort( &port, 1);
    //OSUtils::sleep(100);
    //setRTSSerialPort( &port, 0);    

    // ping 
    static char *PingString;
    if (trackerType == FASTTRAK)
        PingString = "\rP";
    else if (trackerType == ISOTRAK)
        PingString = "P";
    
	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:pinging tracker")));
    int ping = 0;
    int pong = 0;
    int dr;
    while (ping < 5)
    {
        ping++;
		ACE_DEBUG((LM_INFO, ACE_TEXT(".")));
    	writetoSerialPort(&port, PingString, strlen(PingString));
        pong = 0;
        while (((dr = readfromSerialPort( &port, buffer, 255)) <= 0) && (pong < 100))
        {
            OSUtils::sleep(10);
            pong++;
        }
        if (pong < 100) 
        {
            buffer[dr]='\0';
            if (strstr(buffer,"0") != NULL)
                break;
        }
    }    
	ACE_DEBUG((LM_INFO, ACE_TEXT("\n")));
    
    if (ping == 5)
    {        
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:FastTrakModule : can't ping tracker\n")));
        return -1;
    }

    // init sequence

    // Continous mode off
    writetoSerialPort(&port,"\rc\r", 3); 
    
    char init_string[256];
    int p = -1;
    while(1)
        if ((p = initString.find("\\r")) > -1)
            initString.replace(p,2,"\r",1);
        else
            break;
    strncpy(init_string, initString.c_str(), 255);
    writetoSerialPort(&port, init_string, strlen(init_string));

    for (int i=0; i<numberOfStations; i++)
    {
        // set hemisphere
        sprintf(buffer,"H%d,%s\r",i+1,hemisphere.c_str());
        writetoSerialPort(&port, buffer, strlen(buffer));
        // set reference frame
        sprintf(buffer,"R%d\rA%d,%s\r",i+1,i+1,referenceFrame.c_str());
        writetoSerialPort(&port, buffer, strlen(buffer));
        // set preferred output
        if (trackerType == FASTTRAK)
            // always use quaternions (14 Bytes)
            sprintf(buffer,"O%d,18,20\r",i+1);
        else if (trackerType == ISOTRAK)
            // always use quaternions (49 Bytes)
    	    sprintf(buffer,"O2,11,1\r");

        writetoSerialPort(&port, buffer, strlen(buffer));
    }    

    // draining input buffer of junk
    while (readfromSerialPort( &port, buffer, 255) > 0)
        ;

    // Continous mode on
    writetoSerialPort(&port, "u\rC\r", 4);

    // --- init end

    return 0;
}

// closes FastTrakModule, closing the serial port
void FastTrakModule::close()
{
    // stop thread
    lock();
    stop = 1;
    unlock();

    // close serial ports
    if( isInitialized() == 1 )
    {
        closeSerialPort(&port);
    }
}

// this method is the code executed in its own thread
void FastTrakModule::run()
{
    // return value of parser
    int stationNr = -1;
    // buffer for a data record
    char inputBuffer[50];

    // data buffer for incoming data
    const int maxCyclesWithoutData = 50;
    char readBuffer[128];
    int bytesRead = 0;
    int noData = 0;
    
    while(1)
    {
        if (initFastTrak() == -1)
            continue;

        noData = 0;
        stationNr = -1;

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
            if( (bytesRead = readfromSerialPort( &port, readBuffer, 128 )) < 1 )
                noData++;
            else
                noData = 0;
            
            for (int j=0; j<bytesRead; j++)
            {
                if (trackerType == FASTTRAK)
                    stationNr = parseRecordFT(readBuffer[j], inputBuffer);
                else if (trackerType == ISOTRAK)
                    stationNr = parseRecordIT(readBuffer[j], inputBuffer);
                if (stationNr > -1)
                {
                    // we got a full buffer, set the data of the addressed station
                    lock();
                    convert(stationNr, inputBuffer); 
                    unlock();
                }
                else if (stationNr == -2)
                    break;
            }
            // if to much junk was read or no data was read, 
            // try to init tracker again
            if ((stationNr == -2) || (noData > maxCyclesWithoutData))
                break;
            else
                OSUtils::sleep(10); 
        } // data processing loop
        
        if (stationNr == -2)
			ACE_DEBUG((LM_WARNING, ACE_TEXT("ot:FastTrakModule: too much junk received.\n")));
        else
			ACE_DEBUG((LM_WARNING, ACE_TEXT("ot:FastTrakModule: no data received.\n")));
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:FastTrakModule: trying to reinitialize tracker ...\n")));

    } // reinitialization loop
}
    
// pushes events into the tracker tree
void FastTrakModule::pushState()
{
    FastTrakSource *source;

    if( isInitialized() == 1 )
    {
        for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
        {
            source = (FastTrakSource *) *it;
            lock();
            if (stations[source->station].newVal == 1)
            {
                source->state = stations[source->station].state;
                stations[source->station].newVal = 0;
                unlock();
                source->state.timeStamp();
                source->updateObservers(source->state);
            } 
            else
                unlock();
        }
    }
}


// states of parser
const int tsStart=0;    
const int tsStationNumber=1;
const int tsSpace=2;
const int tsSyncBit=3;
const int tsGetRec=4;

int FastTrakModule::parseRecordFT(char c, char *inputBuffer)
{
    static int state=tsStart;  // state variable of parser
    static int stNr = -1;  // state variable of parser
    static int pos = 0; // number of the current position in the buffer
    static int junk = 0;

    int rc = -1;
    int n = numberOfStations;
    /// number of bytes of the tracker's output record     
    int numOfBytes = 14;    

    junk++;
    // reset variables
    if (junk > 128)
    {
        state=tsStart;  // state variable of parser
        stNr = -1;  
        pos = 0; // number of the current position in the buffer
        junk = 0;
        return -2;
    }

    switch (state)
    {
        case tsStart:
            if (c=='0')
            {              // Record Type "0", possible start of output record
                (state)=tsStationNumber;
            }
            break;

        case tsStationNumber:
            stNr=c-'1';
            if ((stNr>=0) && (stNr<n))
            {                                                // station number
                (state)=tsSpace;
            } else (state)=tsStart;
            break;

        case tsSpace:
            if ( c==' ') (state)=tsSyncBit; else (state)=tsStart;
            break;

        case tsSyncBit:
            pos=0;
            if ((c&0x80)!=0)        // expecting to find a byte with hibit set
            {
                (state)=tsGetRec;
                inputBuffer[pos++]=(c&0x7f);
            } else { (state)=tsStart; }
            break;

        case tsGetRec:
            if ((c&0x80)!=0) { (state)=tsStart; } else
            {
                inputBuffer[pos++]=c;
                if (pos == numOfBytes)
                {                       // got one whole data record in input buffer
                    (state)=tsStart;
                    junk = 0;
                    rc = stNr;
                }
            }
    }

    return rc;
}

int FastTrakModule::parseRecordIT(char c, char *inputBuffer)
{
    static int state=tsStart;  // state variable of parser
    static int stNr = -1;  // state variable of parser
    static int pos = 0; // number of the current position in the buffer
    static int junk = 0;

    int rc = -1;
    int n = numberOfStations;
    // number of bytes of the tracker's output record     
    int numOfBytes = 49;    
    
    junk++;
    // reset variables
    if (junk > 512)
    {
        state=tsStart;  // state variable of parser
        stNr = -1;  
        pos = 0; // number of the current position in the buffer
        junk = 0;
        return -2;
    }

    switch (state)
    {
        case tsStart:
            // Record Type "0", possible start of output record
            if (c=='0')
               (state)=tsStationNumber;
            break;

        case tsStationNumber:
            // station number
            stNr=c-'1';
            if ((stNr>=0) && (stNr<n))
                (state)=tsSpace;
            else (state)=tsStart;
            break;

        case tsSpace:
		if (( c==' ') || (( c=='*' )  || ( c=='@' )))
		{
			if ( c=='*' ) setButtonIT(stNr, 0);
            if ( c=='@' ) setButtonIT(stNr, 1);
			(state)=tsGetRec;
		} else (state)=tsStart;
            break;

        case tsGetRec:
            // got one whole data record in buffer
            if (pos == numOfBytes)
            {       
                if (c=='\r')
                {
                    (state)=tsStart;
                    pos = 0;
                    junk = 0;
                    rc = stNr;
                }
                else
                {
                    (state)=tsStart;
                    pos = 0;
                }
            }
            else
                inputBuffer[pos++]=c;
            break;
    }

    return(rc);
}


/*** function getBinary
 *
 * Purpose: Returns the two bytes from the buffer as a Fastrak 16 bit integer
 * In:	    buffer: 	Pointer to array of bytes to be converted
 * Returns: Fastrak 16 bit integer
 * Roman Rath, Zsolt Szalavari
 */

short int getBinary(const char *buffer)
{
    int d;

    d=(buffer[0]&0x7f)+(buffer[1]<<7); 
    if (d&0x2000) { d=(d&0x1ffff)-16384; }

    return d;
}

double getASC(const char *buffer)
{
    double d;
    char ch[7];
    
    strncpy(ch, buffer,7);
    d=atof(ch);
    
    return(d);
}


/*** function buildPosition
 *
 * Purpose: Convert position bytes from Fastrak into actual position int
 *  	    values
 * In:	    buffer: 	Buffer containing bytes received from Fastrak
 * In/Out:  position:	Pointer to an array of floats for the position to
 *  	    	    	be put in
 * Roman Rath, Zsolt Szalavari
 */

void buildPositionFT(char* buffer, float* position)
{
    for (int i=0; i<3; i++)
    {
    	position[i]=(float)(getBinary(buffer+i*2));
        position[i] *= 0.01f;
    }
}

void buildPositionIT(char* buffer, float* position)
{
    for (int i=0; i<3; i++)
    {
    	position[i]=(float)(getASC(buffer+i*7));
        position[i] *= 0.01f;
    }
}


/*** function buildQuaternionFT
 *
 * Purpose: Convert quaternion bytes from Fastrak into actual quaternion
 *  	    float values
 * In:	    buffer: 	    Buffer containing bytes received from Fastrak
 * In/Out:  quaternion:     Pointer to an array of floats for the quaternion
 *  	    	    	    to be put in
 * Roman Rath, Zsolt Szalavari
 */

void buildQuaternionFT(char* buffer, float* quaternion)
{
    for (int i=0; i<4; i++)
    {
        quaternion[(i+3)%4]=((float)(getBinary(buffer+i*2+6)))/8192;
    }
    
}

void buildQuaternionIT(char* buffer, float* quaternion)
{
    for (int i=0; i<4; i++)
    {
    	quaternion[(i+3)%4]=(float)(getASC(buffer+i*7+21));
    }
}


void FastTrakModule::setButtonIT(int stationNr, int button)
{
    if (stations[stationNr].state.button != button)
    {
        lock();
        stations[stationNr].state.button = button;
        stations[stationNr].newVal = 1;
        unlock();
    }
}

// function processFastrakRecord

void FastTrakModule::convert(int stationNr, char *inputBuffer)
{
    if (trackerType == FASTTRAK)
    {
        buildPositionFT(inputBuffer, stations[stationNr].state.position);
        buildQuaternionFT(inputBuffer,stations[stationNr].state.orientation);
    }
    else if (trackerType == ISOTRAK)
    {
        buildPositionIT(inputBuffer, stations[stationNr].state.position);
        buildQuaternionIT(inputBuffer,stations[stationNr].state.orientation);
    }

    stations[stationNr].newVal = 1;
}

} // namespace ot

// EOF
