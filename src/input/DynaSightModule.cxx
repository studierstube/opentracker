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
/** source file for DynaSightModule module.
  *
  * @author Alexander Schaelss
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/DynaSightModule.cxx,v 1.2 2003/07/01 14:05:30 tamer Exp $
  * @file                                                                    */
 /* ======================================================================== */

#include "DynaSightSource.h"
#include "DynaSightModule.h"

#include "../misc/serialcomm.h"

#include <iostream>

using namespace std;

// constructor initializing the thread manager
DynaSightModule::DynaSightModule() : ThreadModule(), NodeFactory(), stop(false)
{
    // nil
} // DynaSightModule

// destructor cleans up any allocated memory
DynaSightModule::~DynaSightModule()
{
    targets.clear();
} // ~DynaSightModule

void DynaSightModule::init(StringTable& attributes, ConfigNode * localTree)
{
    // cout << "DynaSightModule::init" << endl;
    
    // scanning port name from XML-File
    strncpy (port.pathname, attributes.get("device").c_str(), 255);
    
    ThreadModule::init(attributes, localTree);
} // init

//  constructs a new DynaSightSource node
Node * DynaSightModule::createNode(const string& name,  StringTable& attributes)
{
    if (name.compare("DynaSightSource") == 0)
    { 
        int number;
        if (attributes.get("target", &number ) != 1)
        {
            // error message
            cout << "Error in converting DynaSightSource target number !" << endl;
            return NULL;
        }
        else if ((number < 0) || (number > DYNASIGHT_MAX_TARGETS))
        {
            // error message
            cout << "The DynaSightSource target number is out of range !" << endl;
            return NULL;
        }
        
        TargetVector::iterator it;
        for (it = targets.begin(); it != targets.end(); it++)
        {
            Target * target = (Target*)(*it);
            if (target->number == number)
            {
                break;
            }
        }
        if (it != targets.end())
        {
            // error message
            cout << "DynaSight Source with number "<< number << " exists allready !" << endl;
            return NULL;
        }
        
        DynaSightSource * source = new DynaSightSource; 
        
        // add the source object to the target list
        Target *target = new Target(number, source);
        targets.push_back(target);
        
        cout << "Built DynaSightSource node - target " << number << "." << endl;
        
        return source;
    }

    return NULL;
} // createNode

// opens the sockets needed for communication and starts the receive thread
void DynaSightModule::start()
{
    SerialParams params;
    int myResult;
    
    if ((isInitialized() == 1) && !targets.empty())
    {
        // init serial port
        initSerialParams (&params);
        params.baudrate = 19200;
        params.parity = 0;
        params.bits = 8;
        params.sbit = 1;
        params.hwflow = 0;
        params.swflow = 0;
        params.blocking = 0;
        strncpy (params.pathname, port.pathname, 255);
        
        // open the serial port
        myResult = openSerialPort ( &port, &params );
        
        if (myResult < 0)
        {
            // error message
            cout << "DynaSightModule: error opening port " << port.pathname << endl;
            return;
        }
        
        ThreadModule::start();
    }
} // start

// closes the module and closes any communication sockets and stops thread 
void DynaSightModule::close()
{
    int myResult;
    
    // stop the thread
    // critical section start
    lock();
    stop = true;
    unlock();
    // end of critical section
    
    if (isInitialized() == 1)
    {
        // close the serial port
        myResult = closeSerialPort (&port);
    }
} // stop

// pushes state information into the tree
void DynaSightModule::pushState()
{
    if (isInitialized() == 1)
    {
        if (targets.empty())
            return;
        
        // critical section start
        lock();
        TargetVector::iterator it;
        for (it = targets.begin(); it != targets.end(); it ++)
        {       
            if ((*it)->modified == 1)
            {
                // update the state information
                (*it)->source->state = (*it)->state;
                (*it)->modified = 0;
                (*it)->source->updateObservers ((*it)->source->state);
            }
        }
        unlock();
        // end of critical section
    }
} // pushState

// reads from the DynaSight Sensor and parses the data
void DynaSightModule::run()
{
    // the number of bytes we read over the serial port
    int count;
    // read buffer for the serial port
    char serialPortBuffer[DYNASIGHT_COMM_BUFFER_SIZE];
    // packet buffer for the target data assembly
    char packetBuffer[DYNASIGHT_PACKET_MAX_LENGTH];
    char *characterPointer;
    char newCharacter;
    bool isMarker;
    bool packetSyncError = false;
    // status of the state machine
    int packetState = 0;
    // number of target
    int targetNumber;
    // base-2 exponent
    int exponent;
    long temp;
    // x, y, z coordinates
    long x, y, z;
    // status of target
    TargetStatus status = SEARCH;
    
    
    cout << "Starting DynaSight module thread" << endl;
    
    while (1)
    {
        // yield to let other processes do something
        OSUtils::sleep(1);
        
        // critical section start
        lock();
        if (stop == true)
        {           
            unlock();
            break;
        }
        else
        { 
            unlock();
        }
        // critical section end
        
        // read data from the serial port
        count = readfromSerialPort (&port, serialPortBuffer, DYNASIGHT_COMM_BUFFER_SIZE);
        
        // process and decode serialPortBuffer[], which now contains count new bytes
        characterPointer = serialPortBuffer;
        
        for (; count > 0; count--)
        {
            newCharacter = *characterPointer++;
            
            // check for the sync pattern "1000xxxx"
            isMarker = ((newCharacter & 0xF0) == 0x80);
            
            switch (packetState) // Dyna Sight Sensor state machine
            {
            case 0:
                if (isMarker)
                {
                    packetBuffer[packetState++] = newCharacter;
                }
                else 
                    packetSyncError = true;
                break;
                
            case 1:
                if (isMarker)
                {
                    // expected marker character -> save it
                    packetBuffer[packetState++] = newCharacter;
                }
                else
                {
                    // loss of synchronization -> reset
                    packetState = 0;
                    packetSyncError = true;
                }
                break;
                
            case 2:
                if (isMarker)
                {
                    // unexpected marker -> shift back
                    packetBuffer[0] = packetBuffer[1];
                    packetBuffer[1] = newCharacter;
                    packetSyncError = true;
                    // leave packetState at 2;
                }
                else
                {
                    // save the character
                    packetBuffer[packetState++] = newCharacter;
                }
                break;
                
            case 3:
                // save the character
                packetBuffer[packetState++] = newCharacter;
                break;
                
            case 4:
                if (isMarker)
                {
                    // unexpected marker -> reset state machine
                    packetBuffer[0] = newCharacter;
                    packetState = 1;
                    packetSyncError = true;
                }
                else
                {
                    // save the character
                    packetBuffer[packetState++] = newCharacter;
                }
                break;
                
            case 5:
				            // save the character
				            packetBuffer[packetState++] = newCharacter;
                            break;
                            
            case 6:
                if (isMarker)
                {
                    // unexpected marker -> reset state machine
                    packetBuffer[0] = newCharacter;
                    packetState = 1;
                    packetSyncError = true;
                }
                else
                {
                    // save the character
                    packetBuffer[packetState++] = newCharacter;
                }
                break;
                
            case 7:
                // the 8-byte packet is complete
                // save the character
                packetBuffer[packetState] = newCharacter;
                
                // decode the completed packet
                exponent = packetBuffer[0]&0x3; // exponent is in bits 1 and 0 of first byte
                targetNumber = packetBuffer[1]&0x4 | ((packetBuffer[0]>>2)&0x3); // up to 8 targets
                status = (TargetStatus) (packetBuffer[1]&0x3);
                // get the three coordinates
                temp = ((long)(char)packetBuffer[2]<<8) | (long)packetBuffer[3]&0xFFL;
                x = temp<<exponent;
                temp = ((long)(char)packetBuffer[4]<<8) | (long)packetBuffer[5]&0xFFL;
                y = temp<<exponent;
                temp = ((long)(char)packetBuffer[6]<<8) | (long)packetBuffer[7]&0xFFL;
                z = temp<<exponent;
                packetState = 0;
                
                // check if we got new reliable data
                if ((status == TRACK) || (status == CAUTION))
                {
                    // get source with the same target number
                    TargetVector::iterator target;
                    for (target = targets.begin(); target != targets.end(); target++)
                    {
                        if ((*target)->number == targetNumber)
                            break;
                    }
                    
                    // if source with the same target number exists
                    if (target != targets.end())
                    {
                        // start of critical section
                        lock();
                        State & state = (*target)->state;
                        
                        (*target)->modified = 1;
                        
                        // set the position
                        state.position[0] = x / SCALE_TO_METER;
                        state.position[1] = y / SCALE_TO_METER;
                        state.position[2] = z / SCALE_TO_METER;
                        
                        // set the orientation
                        state.orientation[0] = 0.0;
                        state.orientation[1] = 0.0;
                        state.orientation[2] = 0.0;
                        state.orientation[3] = 1.0;
                        
                        // set the confidence value
                        state.confidence = (status == TRACK) ? 1.0 : 0.5;
                        
                        state.timeStamp();
                        unlock();
                        // end of critical section
                    }
                }
                
                break;
            } // switch
        } // for
    } // while forever
    
    cout << "Stopping DynaSight module thread" << endl;
} // run
