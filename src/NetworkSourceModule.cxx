// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NetworkSourceModule.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'NetworkSourceModule'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
#include <string>
#include <stdio.h>
#ifdef WIN32
#include <iostream>    // VisualC++ needs the new iostream library
#else
#include <iostream.h>
#endif
#include "multicast.h"

// definitions for the Network Data protocol
const int positionQuaternion=1;
const int positionAngles=2;
const int positionMatrix=3;

const int magicNum=0xbeef;
const int revNum=0x0200;
//@END_USER2


// Static members


/*@NOTE_13358
Constructor method.
*/
NetworkSourceModule::NetworkSourceModule() //@INIT_13358
    : Module()
    , NodeFactory()
    , socket(-1)
    , threadPID(-1)

    , multicast()
    , port()
{//@CODE_13358
    ConstructorInclude();

    // Put in your own code
}//@CODE_13358


/*@NOTE_12981
Destructor method.
*/
NetworkSourceModule::~NetworkSourceModule()
{//@CODE_12981
    DestructorInclude();

    // Put in your own code
}//@CODE_12981


/*@NOTE_12994
called before the update cycle. Any preparation stuff is happening here.
If the module was initialized and we haven't started the thread so far,
we do it. 
*/
void NetworkSourceModule::beginUpdate()
{//@CODE_12994
	if(isInitialized() && threadPID == -1 )
	{
		threadPID = createThread(entryThread, (void*)this, 0);
	}
}//@CODE_12994


/*@NOTE_12995
kills the thread and closes the multicast socket.
*/
void NetworkSourceModule::close()
{//@CODE_12995
	killThread(threadPID);
	leaveMulticastGroup(socket, multicast, port);
	closeMulticastSocket( socket );
}//@CODE_12995


/*@NOTE_13377
Converts num floats from network byte order. Stores the converted floats in
result.
*/
void NetworkSourceModule::convertFloatsNToHl(float* floats, float* result,
                                             int num)
{//@CODE_13377
    int i;
    union
    {
        float f;
        long int l;
    } convert;

    for (i=0; i<num; i++)                 // x,y,z
    {
	convert.f = floats[i];
	convert.l = ntohl(convert.l);    // Convert host to network byte order
	result[i] = convert.f;
    }
}//@CODE_13377


/*@NOTE_12991
This method is called to construct a new Node. It takes the element name and
any attribute name, value pairs and returns the new Node, if it is able to 
construct the corresponding type. Otherwise it returns NULL.
*/
TreeNode* NetworkSourceModule::createNode(char* const name,
                                          StringMap& attributes)
{//@CODE_12991
    std::string strName( name );
    if( strName.compare("NetworkSource") == 0 )
    {
        int stationNumber;
        int num = sscanf( attributes["number"], " %i", &stationNumber );
        if( num == 0 ){
            stationNumber = 1;
        }
        NetworkSource * source = new NetworkSource( stationNumber );
        State * state = new State;
        NetworkSourceBuffer * buffer = new NetworkSourceBuffer;
        buffer->source = source;
        buffer->buffer = state;
        buffer->active = 10;
        sources.push_back( buffer );
        
	cout << "Build NetworkSource" << source->getStationNumber() << endl;
        return source;        
    }
    return NULL;
}//@CODE_12991


/*@NOTE_13367
thread entry function, reads from the network and handles the data.
*/
void NetworkSourceModule::entryThread(void* self)
{//@CODE_13367
    NetworkSourceModule * module = (NetworkSourceModule*)self;
#ifdef _SGI_SOURCE
    prctl(PR_TERMCHILD); // terminate child if parent dies
#endif
    module->run();
}//@CODE_13367


/*@NOTE_12996
initializes the tracker source. This class also provides an implementation that sets the initialization flag to true.
Subclasses should call this method if they override this method. It takes the attributes of the element configuring
this module and a local tree consisting of the children of the element. This tree has to
be made up of Nodes.
*/
void NetworkSourceModule::init(StringMap& attributes, const TreeNode* localTree)
{//@CODE_12996
    Module::init( attributes, localTree );    

    Semaphore::initClass();
    semid = new Semaphore; // create semaphor for locking of state record
     
    multicast = (char *)attributes["multicast-address"];
    port = (char *)attributes["port"];
    // init socket
#ifdef WIN32
    initNetwork();
#endif

    socket = createMulticastSocket(NULL,NULL,MULTICAST);
    if(socket<0)
    {
        cout << "Cannot create multicastsocket."<< endl;
        exit(-1);
    }      
    if(joinMulticastGroup(socket, multicast, port) <0)
    {
	cout << "Cannot join multicast group at address: " << multicast <<
	    ", port: " << port << endl;    
        exit(-1);
    }
    cout << "Module NetworkSourceModule initialized." << endl;
}//@CODE_12996


/*@NOTE_13369
Does the work of the network thread. This method is called by entryThread.
*/
void NetworkSourceModule::run()
{//@CODE_13369
	// this runs forever
	while(1)
	{		 
		fd_set rfds;
		struct timeval tv;
		int retval; 

		//--read data from socket
		do 
		{
			// Watch socket to see when it has input
			FD_ZERO(&rfds);
			FD_SET(socket, &rfds);
			// Wait up to 1 second
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			retval = select(socket+1, &rfds, NULL, NULL, &tv);
			if (retval>0) // message ready
			{
				int bytes;
				bytes = receiveMulticastData( socket,(char *)&data, sizeof( data ));
				if(bytes<0) 
				{
					cout << "Error receiving data in NetworkSourceModule." << endl;
					exit(-1);
				}
			}
			else if (retval == 0) // timed out
			{
				// set tracker as dead, all stations inactive
				this->semid->wait();
				for( NetworkSourceBufferVector::iterator it = sources.begin(); 
					 it != sources.end(); it++ )
				{
					(*it)->buffer->isValid = 0;
				}				
				this->semid->signal();
			} 
			else // select yielded error
#ifdef _SGI_SOURCE
	        if (retval == -1)
		    {
			    if (errno != EINTR) 
				{
#else
		    if (retval == SOCKET_ERROR) 
		    { 
				int err = WSAGetLastError();
				if (err != EINTR) 
				{
#endif
					cout << "Error waiting for data in NetworkSourceModule." << endl;
					exit(-1);
				}
			}
		} while (retval <= 0);

		// process received network data		
		short int commentLen;
		short int si;
		short int mode;
		int station;

		//--early exit if invalid record
		if(((unsigned short) ntohs(data.headerId) != magicNum)
			|| ((unsigned short) ntohs(data.revNum) != revNum))
			continue;;

		int stationNumber=ntohs(data.maxStationNum);
		int num=ntohs(data.numOfStations);
		commentLen=ntohs(data.commentLength);
		/* not interested in server name so far
		char sName[256];               // Servername
		memcpy(&sName,&rcvbuf.data,commentLen);
		sName[commentLen]=0;
		*/
		char *stdata = &data.data[commentLen];
		NetworkSourceBuffer * buffer;
		State state;
		for (int cnt=0; cnt<num; cnt++)
		{
			memcpy(&si,stdata,sizeof(short int));
			station=ntohs(si);
			if((station>=1) && (station<=stationNumber))
			{
				// loop through buffers, reducing active counts and looking for
				// the right NetworkSource node.
				for( NetworkSourceBufferVector::iterator it = sources.begin();
					 it != sources.end(); it++ )
				{
				    if((*it)->active>0 ) 
					    (*it)->active--;
					if((*it)->source->getStationNumber() == station )
					{
						buffer = (*it);
						(*it)->active = 10;
					}
				}
				
				// memcpy(&tst.station[rt].serverName,&sName,commentLen+1);
				memcpy(&si,stdata+sizeof(short int)*4,sizeof(short int));
				si=ntohs(si);          // Length of name of the station
				// memcpy(&tst.station[rt].stationName,stdata+sizeof(short int)*5,si);
				// tst.station[rt].stationName[si]=0;
				si=si+sizeof(short int)*5;
				memcpy(&state.button,stdata+sizeof(short int)*2,sizeof(short int));
				state.button=ntohs(state.button);
				memcpy(&mode,stdata+sizeof(short int),sizeof(short int));
				mode=ntohs(mode);  
				memcpy(state.position,stdata+si,sizeof(float)*3);
				convertFloatsNToHl(state.position,state.position,3);
				si=si+sizeof(float)*3;

				switch(mode)
				{
					case positionQuaternion:
					{
						memcpy(state.orientation,stdata+si,sizeof(float)*4);
						convertFloatsNToHl(state.orientation,
										  state.orientation,4);
						stdata=stdata+si+sizeof(float)*4; // Next data record
						break;
					}
					case positionMatrix:
					{
						float matrix[3][3];
						memcpy(matrix,stdata+si,sizeof(float)*9);
						convertFloatsNToHl((float *)matrix,(float*)matrix,9);
						MathUtils::matrixToQuaternion( matrix, state.orientation);
						stdata=stdata+si+sizeof(float)*9; // Next data record
						break;
	                }
		            case positionAngles:
			        {
						float angles[3];
					    memcpy(angles,stdata+si,sizeof(float)*3);
						convertFloatsNToHl(angles, angles, 3);
						MathUtils::eulerToQuaternion( angles[0], angles[1], angles[2], state.orientation);
						stdata=stdata+si+sizeof(float)*3; // Next data record
						break;
					}
				}//switch
            
				// copy result - critical section
				semid->wait();
				buffer->buffer->button = state.button;
				buffer->buffer->confidence = 1;
				buffer->buffer->isValid = 1;
				buffer->buffer->orientation[0] = state.orientation[0];
				buffer->buffer->orientation[1] = state.orientation[1];
				buffer->buffer->orientation[2] = state.orientation[2];
				buffer->buffer->orientation[3] = state.orientation[3];
				buffer->buffer->position[0] = state.position[0];
				buffer->buffer->position[1] = state.position[1];
				buffer->buffer->position[2] = state.position[2];	
				semid->signal();
			} 
			else
			{
				cout << "received wrong tracker record type " << station << endl;
			}
		}//for cnt
	}
}//@CODE_13369


/*@NOTE_13366
copies the current data from the states vector into the actual NetworkSource
nodes. This must be handled threadsafe.
*/
void NetworkSourceModule::update()
{//@CODE_13366
    semid->wait();	
    for( NetworkSourceBufferVector::iterator it = sources.begin();
           it = sources.end(); it++ )
    {
        State * state = (*it)->source->getState();
        State * buffer = (*it)->buffer;
        state->button = buffer->button;
        state->confidence = buffer->confidence;
		state->isValid = buffer->isValid;
		state->orientation[0] = buffer->orientation[0];
		state->orientation[1] = buffer->orientation[1];
		state->orientation[2] = buffer->orientation[2];
		state->orientation[3] = buffer->orientation[3];
		state->position[0] = buffer->position[0];
        state->position[1] = buffer->position[1];
        state->position[2] = buffer->position[2];
    }
    semid->signal();
}//@CODE_13366


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_12980
Method which must be called first in a constructor.
*/
void NetworkSourceModule::ConstructorInclude()
{
}


/*@NOTE_12982
Method which must be called first in a destructor.
*/
void NetworkSourceModule::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
