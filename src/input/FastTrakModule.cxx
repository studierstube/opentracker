/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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


#ifndef OT_NO_FASTTRACK_SUPPORT


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

        stations = new tmpStationEvent[numberOfStations];

        if (attributes.get("type").compare("fasttrak") == 0)
            trackerType = FASTTRAK;
        else if (attributes.get("type").compare("isotrak") == 0)
            trackerType = ISOTRAK;
        else
        {
            LOG_ACE_ERROR("ot:FastTrakModule : unknown trackertype %s\n", attributes.get("type").c_str());
            exit(-1);
        }

        strncpy( port.pathname, attributes.get("device").c_str(), 255 );

        hemisphere = attributes.get("hemisphere");
        referenceFrame = attributes.get("reference-frame");
        initString = attributes.get("init-string");
    
        ThreadModule::init( attributes, localTree );
	LOG_ACE_INFO("ot:FastTrakModule : initialized !\nusing tracker protocol for %s\n", attributes.get("type").c_str());
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
    void FastTrakModule::pushEvent()
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
                    source->event = stations[source->station].event;
                    stations[source->station].newVal = 0;
                    unlock();
                    source->event.timeStamp();
                    source->updateObservers(source->event);
                }
                else
                    unlock();
            }
        }
    }


    // events of parser
    const int tsStart=0;
    const int tsStationNumber=1;
    const int tsSpace=2;
    const int tsSyncBit=3;
    const int tsGetRec=4;

    int FastTrakModule::parseRecordFT(char c, char *inputBuffer)
    {
        static int event=tsStart;  // event variable of parser
        static int stNr = -1;  // event variable of parser
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
            event=tsStart;  // event variable of parser
            stNr = -1;
            pos = 0; // number of the current position in the buffer
            junk = 0;
            return -2;
        }

        switch (event)
        {
            case tsStart:
                if (c=='0')
                {              // Record Type "0", possible start of output record
                    (event)=tsStationNumber;
                }
                break;

            case tsStationNumber:
                stNr=c-'1';
                if ((stNr>=0) && (stNr<n))
                {                                                // station number
                    (event)=tsSpace;
                } else (event)=tsStart;
                break;

            case tsSpace:
                if ( c==' ') (event)=tsSyncBit; else (event)=tsStart;
                break;

            case tsSyncBit:
                pos=0;
                if ((c&0x80)!=0)        // expecting to find a byte with hibit set
                {
                    (event)=tsGetRec;
                    inputBuffer[pos++]=(c&0x7f);
                } else { (event)=tsStart; }
                break;

            case tsGetRec:
                if ((c&0x80)!=0) { (event)=tsStart; } else
                {
                    inputBuffer[pos++]=c;
                    if (pos == numOfBytes)
                    {                       // got one whole data record in input buffer
                        (event)=tsStart;
                        junk = 0;
                        rc = stNr;
                    }
                }
        }

        return rc;
    }

    int FastTrakModule::parseRecordIT(char c, char *inputBuffer)
    {
        static int event=tsStart;  // event variable of parser
        static int stNr = -1;  // event variable of parser
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
            event=tsStart;  // event variable of parser
            stNr = -1;
            pos = 0; // number of the current position in the buffer
            junk = 0;
            return -2;
        }

        switch (event)
        {
            case tsStart:
                // Record Type "0", possible start of output record
                if (c=='0')
                    (event)=tsStationNumber;
                break;

            case tsStationNumber:
                // station number
                stNr=c-'1';
                if ((stNr>=0) && (stNr<n))
                    (event)=tsSpace;
                else (event)=tsStart;
                break;

            case tsSpace:
                if (( c==' ') || (( c=='*' )  || ( c=='@' )))
                {
                    if ( c=='*' ) setButtonIT(stNr, 0);
                    if ( c=='@' ) setButtonIT(stNr, 1);
                    (event)=tsGetRec;
                } else (event)=tsStart;
                break;

            case tsGetRec:
                // got one whole data record in buffer
                if (pos == numOfBytes)
                {
                    if (c=='\r')
                    {
                        (event)=tsStart;
                        pos = 0;
                        junk = 0;
                        rc = stNr;
                    }
                    else
                    {
                        (event)=tsStart;
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
     * In/Out:  position:	Reference to a vector of floats for the position to
     *  	    	    	be put in
     * Roman Rath, Zsolt Szalavari
     */

    void buildPositionFT(char* buffer, std::vector<float>& position)
    {
        for (int i=0; i<3; i++)
        {
            position[i]=(float)(getBinary(buffer+i*2));
            position[i] *= 0.01f;
        }
    }

    void buildPositionIT(char* buffer, std::vector<float>& position)
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

    void buildQuaternionFT(char* buffer, std::vector<float> &quaternion)
    {
        for (int i=0; i<4; i++)
        {
            quaternion[(i+3)%4]=((float)(getBinary(buffer+i*2+6)))/8192;
        }
    
    }

    void buildQuaternionIT(char* buffer, std::vector<float> &quaternion)
    {
        for (int i=0; i<4; i++)
        {
            quaternion[(i+3)%4]=(float)(getASC(buffer+i*7+21));
        }
    }


    void FastTrakModule::setButtonIT(int stationNr, int button)
    {
        if (stations[stationNr].event.getButton() != button)
        {
            lock();
            stations[stationNr].event.getButton() = button;
            stations[stationNr].newVal = 1;
            unlock();
        }
    }

    // function processFastrakRecord

    void FastTrakModule::convert(int stationNr, char *inputBuffer)
    {
        if (trackerType == FASTTRAK)
        {
            buildPositionFT(inputBuffer, stations[stationNr].event.getPosition());
            buildQuaternionFT(inputBuffer,stations[stationNr].event.getOrientation());
        }
        else if (trackerType == ISOTRAK)
        {
            buildPositionIT(inputBuffer, stations[stationNr].event.getPosition());
            buildQuaternionIT(inputBuffer,stations[stationNr].event.getOrientation());
        }

        stations[stationNr].newVal = 1;
    }

} // namespace ot


#endif // OT_NO_FASTTRACK_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of FastTrakModule.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
