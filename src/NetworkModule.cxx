// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NetworkModule.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'NetworkModule'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 13:44 gr added NetworkSource classes
//     Updated code of method 'NetworkModule'
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
#include <string>
#include <stdio.h>
#ifdef WIN32
#include <iostream>    // VisualC++ has two incompatible iostreams libraries !
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


/*@NOTE_3781
Constructor method.
*/
NetworkModule::NetworkModule() //@INIT_3781
    : NodeFactory(),
      Module()
{//@CODE_3781
    ConstructorInclude();

    // Put in your own code
}//@CODE_3781


/*@NOTE_3204
Destructor method.
*/
NetworkModule::~NetworkModule()
{//@CODE_3204
    DestructorInclude();

    // Put in your own code
}//@CODE_3204


/*@NOTE_4424
Called before the update cycle. Sets some variables in the NetworkData structure.
*/
void NetworkModule::beginUpdate()
{//@CODE_4424
    data.numOfStations = 0;
    data.maxStationNum = htons( stations.size()); 
}//@CODE_4424


/*@NOTE_3841
Closes the NetworkModule. Closes the multicast group.
*/
void NetworkModule::close()
{//@CODE_3841
    closeMulticastSocket( socket );
}//@CODE_3841


/*@NOTE_4604
Converts num floats to network byte order. Stores the converted floats in
result.
*/
void NetworkModule::convertFloatsHToNl(float* floats, float* result, int num)
{//@CODE_4604
    int i;
    union
    {
        float f;
        long int l;
    } convert;

    for (i=0; i<num; i++)                 // x,y,z
    {
	convert.f = floats[i];
	convert.l = htonl(convert.l);    // Convert host to network byte order
	result[i] = convert.f;
    }
}//@CODE_4604


/*@NOTE_3617
This method is called to construct a new Node. It constructs the Station nodes
as the NetworkModule keeps track of those nodes. A Station node has 
the following parameters :

number    a station number #REQUIRED

name      a station name   #REQUIRED
*/
TreeNode* NetworkModule::createNode(char* const name, StringMap& attributes)
{//@CODE_3617
    std::string strName( name );
    if( strName.compare("Station") == 0 )        
    {
        int stationNumber;
        int num = sscanf( attributes["number"], " %i", &stationNumber );
        if( num == 0 ){
            stationNumber = 1;
        }
        Station * station = new Station( stationNumber, attributes["name"] );
        stations.push_back( station );

	cout << "Build station " << station->getName() << endl;
        return station;        
    }
    return NULL;               
}//@CODE_3617


/*@NOTE_4422
after the update cycle. Checks all station nodes to see if there are new ( valid ) states.
Adds them to a network data package and sends it to the multicast group. This is done here
so that all tracker sources are sure to have finished their stuff, because they do the
tracking in update().
*/
void NetworkModule::endUpdate()
{//@CODE_4422
    char * nR;
    short int si;
    State * state;
    float temp[4];
    int size;
    int i;

    // if we have no Stations to check for, forget about the whole thing
	if( stations.size() == 0 )
	{
		return;
	}

    size = ntohs( data.headerLength );
    nR = data.data + ntohs( data.commentLength );    
    for( StationVector::iterator it = stations.begin(); it != stations.end(); it++ )
    {
	    state = (*it)->getState();
        if( state == NULL )
            continue;
	    if( state->isValid == 1 )
    	{
	        short int psize;
	        psize = (3+4)*sizeof(float);
    	    /* 
	           // we only use quaternions, should fix that some time
	           switch (st->mode)
    	       {
	           case positionQuaternion:   psize=(3+4)*sizeof(float); break;
	           case positionMatrix:        psize=(3+9)*sizeof(float); break;
    	       case positionAngles:        psize=(3+3)*sizeof(float); break;
	           }
	        */
    	    psize=psize+strlen((*it)->getName())+sizeof(short int)*5;
	    
    	    si=htons((*it)->getStationNumber());            // Stationnumber
    	    memcpy(nR,&si,sizeof(short int));
	    
	        si=htons(positionQuaternion);                  // Output mode  (Pos_Quat|Matrix|Angles)
    	    memcpy(nR+sizeof(short int),&si,sizeof(short int));
	   
	        si=htons(state->button);                       // Button byte
    	    memcpy(nR+sizeof(short int)*2,&si,sizeof(short int));

	        si=htons(psize);                               // Size of the data packet
	        memcpy(nR+sizeof(short int)*3,&si,sizeof(short int));

    	    si=htons(strlen((*it)->getName()));             // Length of the name of the station
    	    memcpy(nR+sizeof(short int)*4,&si,sizeof(short int));
	    
	        // Name of the station
	        memcpy(nR+sizeof(short int)*5,(*it)->getName(),strlen((*it)->getName()));
	    
    	    i=strlen((*it)->getName())+sizeof(short int)*5;
    	    // Position
	        convertFloatsHToNl(state->position, temp, 3);
	        memcpy(nR+i,temp,sizeof(float)*3);
    	    i=i+sizeof(float)*3;

	        // again we always use positionQuaternion mode
    	    convertFloatsHToNl(state->orientation, temp, 4);
            memcpy(nR+i,temp,sizeof(float)*4);
	        i=i+sizeof(float)*4;

	    /*
	      switch (st->mode)
	      {
	      case positionQuaternion:
	      convertFloatsHToNl(st->orientation,4);
	      memcpy(nR+i,&st->orientation,sizeof(float)*4); break;
	      case positionMatrix:
	      convertFloatsHToNl(st->orientation,9);
	      memcpy(nR+i,&st->orientation,sizeof(float)*9); break;
	      case positionAngles:
	      convertFloatsHToNl(st->orientation,3);
	      memcpy(nR+i,&st->orientation,sizeof(float)*3); break;
	      }
	    */
	        nR = nR + psize;
    	    size += psize;
	        data.numOfStations++;
    	}
    }
    if( data.numOfStations > 0 )
    {
        data.numOfStations = htons( data.numOfStations );
    	if( sendMulticastData( socket, (char *)&data, size ) < 0 )
	    {
//	        if( errno != ECONNREFUSED )
    	    {
	        	cout << "Error sending packet !\n";
    	    }
    	}
    }
}//@CODE_4422


/*@NOTE_3842
Initializes the NetworkModule. It sets the NetworkData structure and opens the multicast group.
The local sub tree might contain additional nodes such as a transformation node to specify a 
global transformation or even a tracked node etc. This is not yet supported.

It expects the following attributes.

name                the server name    #REQUIRED

multicast-address   a multicast group address #REQUIRED

port                a port to bin to   #REQUIRED
*/
void NetworkModule::init(StringMap& attributes, const TreeNode* localTree)
{//@CODE_3842
    Module::init( attributes, localTree );    
    
    // initialize NetworkRecord
    data.headerId=htons(magicNum);
    data.revNum=htons(revNum);
    
    const char * name = attributes["name"];
    data.commentLength = strlen( name );
    strcpy( data.data, name );
    data.headerLength= strlen( name ) + (sizeof(short int))*6;
    data.commentLength=htons(data.commentLength);
    data.headerLength=htons(data.headerLength);
    
    // initialize multicast group
    initNetwork();
    socket = createMulticastGroup( attributes["multicast-address"], attributes["port"] );
    if(socket<0)
    {
        cout << "Cannot create multicastgroup at address: " << attributes["multicast-address"] <<
	    ", port: " << attributes["port"] << endl;
        exit(1);
    }      
    cout << "Module NetworkModule initialized." << endl;
}//@CODE_3842


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3203
Method which must be called first in a constructor.
*/
void NetworkModule::ConstructorInclude()
{
}


/*@NOTE_3205
Method which must be called first in a destructor.
*/
void NetworkModule::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
