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
/** source file for MagicYModule module.
  *
  * @author Christoph Traxler
  *
  * $Id$
  * @file                                                                    */
 /* ======================================================================== */

// A trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "MagicYModule.h"

#include <iostream>
#include <string>

using namespace std;


// constructor initializing the thread manager
MagicYModule::MagicYModule() : ThreadModule(), NodeFactory(), stop(0)
{
    
}

// destructor cleans up any allocated memory
MagicYModule::~MagicYModule()
{
    unsigned int i;
    for (i=0; i<magicYs.size(); i++) delete magicYs[i];
    for (i=0; i<screens.size(); i++) delete screens[i];
}

// open all sockets
int MagicYModule::connect()
{
    int retval; 
    char buffer[32];
    ACE_Time_Value timeOut(1,0);
    
    for(unsigned int i=0; i<screens.size(); i++)
    {
        retval = connector.connect(screens[i]->socket, screens[i]->address, &timeOut);
        if(retval == -1 && errno != ETIME && errno != 0 )
        {
            std::cout << "Error " << errno << " connection failed for socket nr.:" << i << endl;
            return -1;
        }
        else 
        {
            std::cout << "connected to socket nr.:" << i << " - sending GO command" << endl;
            sprintf(buffer, "GO\n\r");
            retval = screens[i]->socket.send_n(buffer, sizeof(buffer), &timeOut);
            if(retval == -1 && errno != ETIME && errno != 0 )
            {
                std::cout << "Error " << errno << " sending command for socket nr.:" << i << endl;
                return -1;
            }
        }
        screens[i]->connected = true;	
    }
    return 0;
}

// set FD_SET for ACE::select function
void MagicYModule::setSelect()
{
    readHandles.reset();
    for(unsigned int i=0; i<screens.size(); i++)
        if(screens[i]->connected)
            readHandles.set_bit(screens[i]->socket.get_handle());
}

// receive data from all active sockets
int MagicYModule::receive()
{
    ACE_Time_Value timeOut(0,5000);
    string message(""), accumulated("");
    char buffer[32], t='X';
    int retval, x, y, pos; 
    unsigned int trans_bytes = 0;
    bool complete, trigger;
    
    points.clear();	
    for(unsigned int i=0; i<screens.size(); i++)
    {
        complete = false;
        message.erase(message.begin(), message.end());
        
        if(readHandles.is_set(screens[i]->socket.get_handle()))
        {
            //std::cout << "Reading socket " << i << endl;
            do
            {
                retval = screens[i]->socket.recv_n(buffer, sizeof(buffer), &timeOut, &trans_bytes);
                if(retval == -1 && errno != ETIME && errno != 0)
                {
                    std::cout << "Error " << errno << " receiving data for socket nr.:" << i << endl;
                    return -1;
                }  
                else
                {
                    accumulated.append(buffer, trans_bytes); 
                    pos = accumulated.find("\n\r", 0);
                    if (pos < 0)
                        pos = accumulated.find("\r\n", 0);
                    
                    if (pos >= 0)	
                    { 
                        message.assign(accumulated, 0, pos);
                        accumulated.erase(0, pos+2);
                        complete = true;
                    }
                }
            } while(!complete && stop == 0);
            
            //std::cout << "MESSAGE S" << i << ": " << message.c_str() << endl;	
            if(message.compare("READY") && message.compare("0"))
            {
                pos = message.find(',', 0);
                message.erase(0, pos+1);
                //std::cout << "MESSAGE: " << message.c_str() << endl;
                // extract state and points
                while (1)
                {		
                    if (sscanf(message.c_str(), "%c,%d,%d", &t, &x, &y) == 3)
                    {		
                        if(t == 'Y')
                            trigger = true;
                        else
                            trigger = false;
                        
                        //std::cout << "POINT: (" << x << "," << y << "," << trigger << ")" << endl;
                        x += screens[i]->x_offset;
                        y += screens[i]->y_offset;
                        
                        points.push_back(MagicPoint(x,y,trigger));
                        
                        pos = message.find(',', 0);
                        pos = message.find(',', pos+1);
                        pos = message.find(',', pos+1);
                        message.erase(0, pos+1);
                        
                        if (pos < 0)
                            break;
                    }
                    else
                        break;
                }
            }
        }
    }
    return 0;
}

// checks if all sockets are still connected by sending a PING
int MagicYModule::stillConnected() 
{
    ACE_Time_Value timeOut(1,0);
    char buffer[16];
    int retval;
    
    sprintf(buffer, "PING\n\r");
    for(unsigned int i=0; i<screens.size(); i++)
    {
        retval = screens[i]->socket.send_n(buffer, sizeof(buffer), &timeOut);
        if(retval == -1 && errno != ETIME && errno != 0 )
        {
            std::cout << "Error " << errno << " connection broken for socket nr.:" << i << endl;
            return 0;
        }
    }
    return 1;
}

// close all sockets
void MagicYModule::disconnect()
{
    for(unsigned int i=0; i<screens.size(); i++)
    {
        screens[i]->socket.close();
        screens[i]->connected = false;
    }
    OSUtils::sleep(3000);
}

// reads from the MagicY server and parses MagicY packages
void MagicYModule::run()
{
    std::cout << "starting MagicY module thread" << endl;
    
    ACE_Time_Value timeOut(1,0);
    int average_x=0, average_y=0, socks_active;	
    bool connected = false;
    
    while(stop == 0)
    {
        // connecting
        std::cout << "Trying to connect ... " << endl;
        if(connect() == 0)
        {
            connected = true;
            // do while no error occurs
            while(stop == 0)
            {	
                average_x=0;
                average_y=0;
                
                //std::cout << "Active sockets: " << socks_active << endl;
                if(! stillConnected())
                    break;
                
                // build FD_SET
                setSelect();
                // wait for sockets to be ready for reading
                socks_active = ACE::select(ACE_Handle_Set::MAXSIZE, readHandles, &timeOut);
                if(! socks_active)
                {
                    std::cout << "Error: Socket select time out" << endl;
                    break;
                }
                if(receive())
                    break;
                
                lock();
                // calculate average
                for (unsigned int i=0; i < points.size(); i++)
                {
                    average_x += points[i].x;
                    average_y += points[i].y;
                }
                if(points.size())
                {
                    average_x /= points.size();
                    average_y /= points.size();
                }
                
                // try to find source 1 to n for the n extracted points and fill their state
                MagicYVector::iterator mY_it;
                for( mY_it = magicYs.begin(); mY_it != magicYs.end(); mY_it++ )
                {
                    // critical section start, fill state
                    State & state = (*mY_it)->state;
                    if ((*mY_it)->average)
                    {
                        if(points.size())
                        {
                            state.position[0] = float(average_x);
                            state.position[1] = float(average_y);
                            state.confidence = 1.0f;
                        }
                        else
                        {
                            state.confidence = 0.0f;
                        }
                    }
                    else
                    {
                        if((*mY_it)->number >= 0 && (unsigned int)(*mY_it)->number < points.size()) 
                        {
                            state.position[0] = float(points[(*mY_it)->number].x);
                            state.position[1] = float(points[(*mY_it)->number].y);
                            state.button = points[(*mY_it)->number].trigger;
                            state.confidence = 1.0f;
                        }                  
                        else
                        {
                            state.confidence = 0.0f;
                        }
                    }
                    state.position[2] = z_value;
                    
                    correctData(state.position, positionMapping, invertPosition);
                    
                    state.orientation[0] = orientation[0];
                    state.orientation[1] = orientation[1];
                    state.orientation[2] = orientation[2];
                    state.orientation[3] = orientation[3];
                    
                    (*mY_it)->modified = 1;
                    state.timeStamp();
                    // end of critical section
                }// for all MagicY sources
                unlock();
            }// while no error 
        }// if connected
        disconnect();
    } // forever
    std::cout << "Stopping thread" << endl;
}


//  constructs a new Node
Node * MagicYModule::createNode( const string& name,  StringTable& attributes)
{
    if( name.compare("MagicYSource") == 0 )
    { 
        int number = atoi(attributes.get("number").c_str());
        
        bool average = false;
        string avrg = attributes.get("average");
        if(!avrg.empty() && !(avrg.compare("true") && avrg.compare("t") && avrg.compare("1")))
            average = true;
        
        MagicYVector::iterator it;
        for( it = magicYs.begin(); it != magicYs.end(); it++ )
        {
            MagicY *mY = (MagicY*)(*it);
            if( mY->number == number )
            {
                break;
            }
        }
        if( it != magicYs.end())
        {
            std::cout << "Source with number "<< number << " exists allready \n";
            return NULL;
        }
        
        MagicYSource *source = new MagicYSource; 
        MagicY *magicY = new MagicY(number, average, source);
        magicYs.push_back( magicY );
        std::cout << "Built MagicYSource node." << endl;
        
        return source;
    }
    return NULL;
}        


// opens the sockets needed for communication and starts the receive thread
void MagicYModule::start()
{
    if (isInitialized() && !magicYs.empty())
        ThreadModule::start();
}

// closes the module and closes any communication sockets and stops thread 
void MagicYModule::close()
{
    // stop thread
    lock();
    stop = 1;
    unlock();
}   

// pushes state information into the tree
void MagicYModule::pushState()
{
    if (magicYs.empty())
        return;
    
    for (MagicYVector::iterator it = magicYs.begin(); it != magicYs.end(); it++ )
    {       
        // critical section start
        lock();
        if((*it)->modified == 1 )
        {
            (*it)->source->state = (*it)->state;
            (*it)->modified = 0;
            unlock();
            (*it)->source->updateObservers( (*it)->source->state );
        }
        else
            unlock();
        // end of critical section
    }
}          


int MagicYModule::parseVector(const string & line, int * val )
{
    int help[3];
    int num;
    num = sscanf( line.c_str()," %d %d %d", &help[0], &help[1], &help[2]);
    if( num != 3 )
    {
        return 1;
    }
    val[0] = help[0];
    val[1] = help[1];
    val[2] = help[2];
    
    return 0;
}


int MagicYModule::parseVector(const string & line, float * val )
{
    float help[4];
    int num;
    num = sscanf( line.c_str()," %f %f %f %f", &help[0], &help[1], &help[2], &help[3]);
    if( num != 4 )
    {
        return 1;
    }
    
    val[0] = help[0];
    val[1] = help[1];
    val[2] = help[2];
    val[3] = help[3];
    
    return 0;
}

// parse data for extra screens, format: "port x_offset y_offset"
// currently streams for extra screens must come from one and the same server

int MagicYModule::parseScreens(const string & line)
{
    int port, x_off, y_off, pos=0;
    string temp = line;
    
    do { 
        if(sscanf(temp.c_str(), "%d %d %d", &port, &x_off, &y_off) < 3)
            return -1;
        
        Screen *scr = new Screen(port, hostname, x_off, y_off);
        screens.push_back(scr);
        
        std::cout << "Extra screen: " << port << ":" << x_off << ":" << y_off << endl;
        
        pos = temp.find(' ', 0);
        pos = temp.find(' ', pos+1);
        pos = temp.find(' ', pos+1);
        temp.erase(0, pos+1);
    } while(pos >= 0); 
    
    return 0;
}

void MagicYModule::correctData(float* d, int *mapping, int *inversion)
{
    float h[3]; 
    int i;
    for(i=0; i<3; i++) h[i] = d[mapping[i]]*inversion[i];
    for(i=0; i<3; i++) d[i] = h[i];
}


void MagicYModule::initMappping(int *mapping)
{
    for (int i=0; i<3; i++)
        mapping[i] = i;
}

void MagicYModule::initInversion(int *inversion)
{
    for (int i=0; i<3; i++)
        inversion[i] = 1;
}

void MagicYModule::initOrientation(float *orientation)
{
    orientation[0] = 0.0f;
    orientation[1] = 0.0f;
    orientation[2] = 0.0f;
    orientation[3] = 1.0f;
}

void MagicYModule::calcMapping(int *mapping)
{
    for (int i=0; i<3; i++)
        if (mapping[i] > 2)
            mapping[i] = 2;
        else if (mapping[i] < 0)
            mapping[i] = 0;
}

void MagicYModule::calcInversion(int *inversion)
{
    for (int i=0; i<3; i++)
        inversion[i] = inversion[i] ? -1 : 1;
}


void MagicYModule::init(StringTable& attributes, ConfigNode * localTree)
{
    ThreadModule::init(attributes, localTree);
    
    // Reading hostname and port number from XML-File
    hostname = string(attributes.get("hostname"));
    
    if( parseVector(attributes.get("positionMapping"), positionMapping ) != 0 )
    {
        std::cout << "Error parsing positionMapping !" << endl;
        initMappping(positionMapping);
    }
    calcMapping(positionMapping);
    if( parseVector(attributes.get("invertPosition"), invertPosition ) != 0 )
    {
        std::cout << "Error parsing invertPosition !" << endl;
        initInversion(invertPosition);
    }
    calcInversion(invertPosition);
    if( parseVector(attributes.get("orientation"), orientation ) != 0 )
    {
        std::cout << "Error parsing orientation !" << endl;
        initOrientation(orientation);
    }
    if( parseScreens(attributes.get("screens")) != 0 )
    {
        std::cout << "Error parsing extra screens !" << endl;
    }
    z_value = atof(attributes.get("z_value").c_str());
}
