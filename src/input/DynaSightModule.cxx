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
/** source file for DynaSightModule module. Version 1.02
  *
  * @author Alexander Schaelss
  *
  * $Id$
  * @file                                                                    */
 /* ======================================================================== */

// this will remove the warning 4786
#include "../tool/disable4786.h"
#include "../tool/OT_ACE_Log.h"

#include <ace/Log_Msg.h>

#include "DynaSightSource.h"
#include "DynaSightModule.h"

#include "../core/MathUtils.h"
#include "../misc/serialcomm.h"

#include <assert.h>
#include <math.h>

#include <iostream>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//using namespace std;

namespace ot {

// constructor initializing the thread manager
DynaSightModule::DynaSightModule() : 
ThreadModule(),
NodeFactory()
{
    // cout << "DynaSightModule::Constructor" << endl;
    
    lookAtVector[0] = 0.0;
    lookAtVector[1] = 0.0;
    lookAtVector[2] = 0.0;
    
    stop = FALSE;
    serialportIsOpen = FALSE;
    hasLookAt = FALSE;
} // DynaSightModule

// destructor cleans up any allocated memory
DynaSightModule::~DynaSightModule()
{
    // cout << "DynaSightModule::Destructor" << endl;
    
    TargetVector::iterator it;
    for (it = targets.begin(); it != targets.end(); it++)
    {
        assert((*it) != NULL);
        delete (*it)->source;
    }
    targets.clear();
} // ~DynaSightModule

void DynaSightModule::init(StringTable& attributes, ConfigNode * localTree)
{
    int myResult = 0;
    
    // cout << "DynaSightModule::init" << endl;
    
    ThreadModule::init(attributes, localTree);
    
    // scanning port name from XML-File
    strncpy (port.pathname, attributes.get("device").c_str(), 255);
	LOG_ACE_INFO("ot:use device on port: %s\n", port.pathname);
    
    // check if we need to calculate the orientation
    myResult = attributes.get("lookat", lookAtVector, 3);
    
    if (myResult == 3)
    {
        hasLookAt = TRUE;
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:looking at point: %f %f %f\n"), lookAtVector[0], lookAtVector[1], lookAtVector[2]));
    }
    else
    {
        hasLookAt = FALSE;
    }
} // init

//  constructs a new DynaSightSource node
Node * DynaSightModule::createNode(const std::string& name,  StringTable& attributes)
{
    int number = 0;
    
    // cout << "DynaSightModule::createNode" << endl;
    
    if (name.compare("DynaSightSource") == 0)
    { 
        if (attributes.get("target", &number ) != 1)
        {
            // error message
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting DynaSightSource target number !\n")));
            return NULL;
        }
        else if ((number < 0) || (number > DYNASIGHT_MAX_TARGETS))
        {
            // error message
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:The DynaSightSource target number is out of range !\n")));
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
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:DynaSight Source with number %d already exists !\n"), number));
            return NULL;
        }
        
        DynaSightSource * source = new DynaSightSource; 
        assert(source);
        
        // add the source object to the target list
        Target *target = new Target(number, source);
        assert(target);
        targets.push_back(target);
                
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Built DynaSightSource node - target %d\n"), number));
        
        // return pointer to the source node
        return source;
    }
    
    return NULL;
} // createNode

// opens the sockets needed for communication and starts the receive thread
void DynaSightModule::start()
{
    SerialParams params;
    int myResult = 0;
    
    // cout << "DynaSightModule::start" << endl;
    
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
			LOG_ACE_ERROR("ot:DynaSightModule: error opening port %s\n", port.pathname);
            return;
        }
        
        serialportIsOpen = TRUE;
        
        ThreadModule::start();
    }
} // start

// closes the module and closes any communication sockets and stops thread 
void DynaSightModule::close()
{
    int myResult = 0;
    
    // cout << "DynaSightModule::close" << endl;
    
    // stop the thread
    // critical section start
    lock();
    stop = true;
    unlock();
    // end of critical section
    
    if (isInitialized() == 1)
    {
        // close the serial port
        if (serialportIsOpen)
            myResult = closeSerialPort (&port);
    }
} // stop

// pushes state information into the tree
void DynaSightModule::pushState()
{
    // cout << "DynaSightModule::pushState" << endl;
    
    if (isInitialized() == 1)
    {
        if (targets.empty())
            return;
        
        TargetVector::iterator it;
        for (it = targets.begin(); it != targets.end(); it ++)
        {       
            // critical section start
            lock();
            
            assert((*it) != NULL);
            
            // DEBUG
            // (*it)->modified = 1;
            // DEBUG
            
            if ((*it)->modified == 1)
            {
                // update the state information
                assert((*it)->source != NULL);
                
                // DEBUG
                /*
                cout << "DynaSightModule::pushState" << endl;
                (*it)->state.position[0] = 1.0;
                (*it)->state.position[1] = 2.0;
                (*it)->state.position[2] = 3.0;
                (*it)->state.orientation[0] = 0.0;
                (*it)->state.orientation[1] = 0.0;
                (*it)->state.orientation[2] = 0.0;
                (*it)->state.orientation[3] = 1.0;
                (*it)->state.confidence = 1.0;
                (*it)->state.timeStamp();
                */
                // DEBUG
                
                (*it)->source->state = (*it)->state;
                (*it)->modified = 0;
                unlock();
                (*it)->source->updateObservers ((*it)->source->state);
            }
            else
                unlock();
            // end of critical section
        } // for
    }  // if
} // pushState

// reads from the DynaSight Sensor and parses the data
void DynaSightModule::run()
{
    // the number of bytes we read over the serial port
    int count = 0;
    // read buffer for the serial port
    char serialPortBuffer[DYNASIGHT_COMM_BUFFER_SIZE];
    // packet buffer for the target data assembly
    char packetBuffer[DYNASIGHT_PACKET_MAX_LENGTH];
    char *characterPointer = NULL;
    char newCharacter = ' ';
    bool isMarker = FALSE;
    bool packetSyncError = FALSE;
    // status of the state machine
    int packetState = 0;
    // number of target
    int targetNumber = 0;
    // base-2 exponent
    int exponent = 0;
    long temp = 0;
    // x, y, z coordinates
    long x = 0, y = 0, z = 0;
    double x_meter = 0.0, y_meter = 0.0, z_meter = 0.0;
    // status of target
    TargetStatus status = SEARCH;
    // variables for the calculating of the orientation
    double diff_x = 0.0, diff_y = 0.0, diff_z = 0.0;
    float alpha = 0.0, beta = 0.0;
        
	ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Starting DynaSight module thread\n")));
    
    assert(serialportIsOpen == TRUE);
    
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
                    // could not synchronize, stay in state 0
                    packetSyncError = TRUE;
                break;
                
            case 1:
                if (isMarker)
                {
                    // we expected marker character -> save it
                    packetBuffer[packetState++] = newCharacter;
                }
                else
                {
                    // got loss of synchronization -> reset
                    packetState = 0;
                    packetSyncError = TRUE;
                }
                break;
                
            case 2:
                if (isMarker)
                {
                    // got an unexpected marker -> shift back
                    packetBuffer[0] = packetBuffer[1];
                    packetBuffer[1] = newCharacter;
                    packetSyncError = TRUE;
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
                    // got an unexpected marker -> reset state machine
                    packetBuffer[0] = newCharacter;
                    packetState = 1;
                    packetSyncError = TRUE;
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
                    // got an unexpected marker -> reset state machine
                    packetBuffer[0] = newCharacter;
                    packetState = 1;
                    packetSyncError = TRUE;
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
                // reset the state machine
                packetState = 0;
                
                // check if we got new reliable data
                if ((status == TRACK) || (status == CAUTION))
                {
                    // get source with the same target number
                    TargetVector::iterator target;
                    for (target = targets.begin(); target != targets.end(); target++)
                    {
                        if ((*target) == NULL)
                        {
							ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:DynaSightModule::run ERROR iterator == NULL!\n")));
                        }
                        
                        if ((*target)->number == targetNumber)
                            break;
                    }
                    
                    // if source with the same target number exists
                    if (target != targets.end())
                    {
                        // start of critical section
                        lock();
                        State & myState = (*target)->state;
                        
                        // mark the state as modified
                        (*target)->modified = 1;
                        
                        // set the position and scale to meter
                        x_meter = x / SCALE_TO_METER;
                        y_meter = y / SCALE_TO_METER;
                        z_meter = z / SCALE_TO_METER;
                        myState.position[0] = (float)x_meter;
                        myState.position[1] = (float)y_meter;
                        myState.position[2] = (float)z_meter;
                        
                        // set the orientation
                        if (hasLookAt)
                        {
                            // we need to calculate the orientation
                            // works OK if the tracked point is in front (z-value) of the lookAt point
                            diff_x = x_meter - lookAtVector[0];
                            diff_y = y_meter - lookAtVector[1];
                            diff_z = z_meter - lookAtVector[2];
                            
                            alpha = (float)atan2(diff_x, diff_z);
                            beta = (float)-atan2(diff_y, diff_z);
                            
                            // DEBUG
                            /*
                            cout << "diff_x: " << diff_x << endl;
                            cout << "diff_y: " << diff_y << endl;
                            cout << "diff_z: " << diff_z << endl;
                            cout << "alpha: " << alpha << endl;
                            cout << "beta: " << beta << endl;
                            cout << endl;
                            */
                            // DEBUG
                            
                            MathUtils::eulerToQuaternion(beta, alpha, 0.0, myState.orientation);
                            MathUtils::normalizeQuaternion(myState.orientation);
                        }
                        else
                        {
                            // we can use the default orientation
                            myState.orientation[0] = 0.0;
                            myState.orientation[1] = 0.0;
                            myState.orientation[2] = 0.0;
                            myState.orientation[3] = 1.0;
                        }
                        
                        // set the confidence value
                        myState.confidence = (status == TRACK) ? 1.0f : 0.5f;
                        
                        myState.timeStamp();
                        unlock();
                        // end of critical section
                    }
                }
                
                break;
            } // switch
        } // for
    } // while forever
    
	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping DynaSight module thread\n")));
} // run

} // namespace ot
