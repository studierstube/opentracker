// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       NetComm
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of network related functions used in the
//              trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:18:12  rr      last modification
// ===========================================================================


#include "NetComm.h"


/*** function convertFloatsHToNl
 *
 * Purpose: Converts 'num' number of floats from host byte order to network
 *          byte order
 * In:      num: Number of floats to be converted
 * In/Out:  floats: Pointer to an array of floats to be converted
 */

void convertFloatsHToNl(float *floats, int num)
{
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
        floats[i] = convert.f;
    }
}


/*** function convertPositionhtons
 *
 * Purpose: Converts int XYZ position from host byte order to network byte
 *          order
 * In:      position: Pointer to an array of ints containing the XYZ position
 * In/Out:  netposition: Pointer to an array of short ints as destination
 */

void convertPositionhtons(int* position, short int* netposition)
{
    int i;

    for (i=0; i<3; i++)
    {
        netposition[i]=htons(position[i]);
    }
}


/*** function convertQuaternionhtons
 *
 * Purpose: Converts an int quaternion from host byte order to network byte 
 *          order
 * In:      quaternion: Pointer to an array of ints containing the quaternion
 * In/Out:  netquaternion:  Pointer to an array of short ints as destination
 */

void convertQuaternionhtons(int* quaternion, short int* netquaternion)
{
    int i;

    for (i=0; i<4; i++)
    {
        netquaternion[i]=htons(quaternion[i]);
    }
}


/*** function initNetRecordData
 *
 * Purpose: Initializes the station data part of the network record to be
 *          broadcast
 * In:      st:         Record holding the properties and data of the station
 *          buttonByte: Button byte from the station
 *          stNr:       Number of the station of the packet
 * In/Out:  nR: Network record to be initialized with station data
 */

void initNetRecordData(char* nR, StationRecord* st, char buttonByte, int stNr)
{
    int i;
    short int si,psize;

    switch (st->mode)
    {
        case positionQuaternion:   psize=(3+4)*sizeof(float); break;
        case positionMatrix:        psize=(3+9)*sizeof(float); break;
        case positionAngles:        psize=(3+3)*sizeof(float); break;
    }
    psize=psize+strlen(st->name)+sizeof(short int)*5;

    si=htons(stNr);                 // Stationnumber
    memcpy(nR,&si,sizeof(short int));

    si=st->mode;                    // Output mode (Pos_Quat|Matrix|Angles)
    si=htons(si);
    memcpy(nR+sizeof(short int),&si,sizeof(short int));

    si=buttonByte;                  // Button byte
    si=htons(si);
    memcpy(nR+sizeof(short int)*2,&si,sizeof(short int));

    si=htons(psize);                // Size of the data packet
    memcpy(nR+sizeof(short int)*3,&si,sizeof(short int));

    si=strlen(st->name);            // Length of the name of the station
    si=htons(si);
    memcpy(nR+sizeof(short int)*4,&si,sizeof(short int));

                                    // Name of the station
    memcpy(nR+sizeof(short int)*5,st->name,strlen(st->name));

    i=strlen(st->name)+sizeof(short int)*5;

                                    // Position
    convertFloatsHToNl(st->position,3);
    memcpy(nR+i,&st->position,sizeof(float)*3);
    i=i+sizeof(float)*3;

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
}


/*** function initNetRecordHeader
 *
 * Purpose: Initializes the network packet header to be broadcast
 * In:      servName: Pointer to a string holding the 'servername' option
 *                    from the initfile
 * In/Out:  nR: Network data record to be initialized
 */

void initNetRecordHeader(NetworkTrackerDataRecord* nR, char* servName)
{
    nR->commentLength=strlen(servName);
    strcpy(nR->data,servName);
    nR->headerLength=strlen(servName)+(sizeof(short int))*6;

    nR->commentLength=htons(nR->commentLength);
    nR->headerLength=htons(nR->headerLength);
}


/*** function initNetRecord
 *
 * Purpose: Initializes the network packet with data that will stay unchanged
 * In:      maxStationNum:  Number of stations dealt with by the server
 * In/Out:  nR: Network data record to be initialized
 */

void initNetRecord(NetworkTrackerDataRecord* nR, int maxStationNum)
{
    nR->headerId=magicNum;
    nR->revNum=revNum;
    nR->maxStationNum=maxStationNum;

    nR->headerId=htons(nR->headerId);
    nR->revNum=htons(nR->revNum);
    nR->maxStationNum=htons(nR->maxStationNum);
}


/*** function getNetRecSize
 *
 * Purpose: Calculates the size of the network data record
 * In:      nR: Network data record
 * Returns: Size of the network data record in bytes
 */

int getNetRecSize(NetworkTrackerDataRecord* nR)
{
    short int si,size;
    int i;

    size=ntohs(nR->headerLength);

    i=ntohs(nR->commentLength);

//    fprintf(stderr,"headerLength: %d\n",size);
//   fprintf(stderr,"commentLength: %d\n",i);
//    fprintf(stderr,"numOfStations: %d\n",ntohs(nR->numOfStations));

    for (int j=0; j<ntohs(nR->numOfStations); j++)
    {
        memcpy(&si,&nR->data[i+sizeof(short int)*3],sizeof(short int));
        si=ntohs(si);

//        fprintf(stderr,"datasize: %d\n",si);
        size=size+si;                // Size of the data packet
        i=i+si;
    }
   
    return size;
}

/*** function initMulticastGroup
 *
 * Purpose: Initializes the MulticastGroup and returns the descriptor of the
 *          socket, or exits printing an errormessage if it fails
 * In:      toaddr: Pointer to string containing the address of the
 *                  multicastgroup
 *          toport: Pointer to string containing the port of the
 *                  multicastgroup
 * Returns: the descriptor of the socket
 */

int initMulticastGroup(char* toaddr, char* toport)
{
    int sock;

    initNetwork();
    sock = createMulticastGroup(toaddr, toport);
    if(sock<0)
    {
        fprintf(stderr,"cannot create multicastgroup at address: %s, port: "
                       "%s\n",toaddr,toport);
        exit(-1);
    }

    return(sock);
}


/*** function sendData
 *
 * Purpose: Sends Data to the MulticastGroup
 * In:      sock:   descriptor of the socket
 *          data:   pointer to the data to be sent
 *          length: Length of the data to be sent in bytes
 * Returns: -1 if an error occured
 */

int sendData(const int sock, const char *data, const int length)
{
    return(sendMulticastData(sock,data,length));
}
