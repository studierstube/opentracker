// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       NetComm
//  TYPE:       h header file
//  PROJECT:    Studierstube
//  CONTENT:    Definition of network related functions used in the
//              trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:18:12  rr      last modification
// ===========================================================================


#ifndef _NETCOMM_H_
#define _NETCOMM_H_

#include <stdio.h>
#include <errno.h>
#include "GlobalDef.h"
#include <util/multicast.h>


/*** function convertFloatsHToNl
 *
 * Purpose: Converts 'num' number of floats from host byte order to network
 *          byte order
 * In/Out:  floats: Pointer to an array of floats to be converted
 */
void convertFloatsHToNl(float *floats, int num);

/*** function convertPositionhtons
 *
 * Purpose: Converts int XYZ position from host byte order to network byte
 *          order
 * In:      position: Pointer to an array of ints containing the XYZ position
 * In/Out:  netposition: Pointer to an array of short ints as destination
 */
void convertPositionhtons(int* position, short int* netposition);

/*** function convertQuaternionhtons
 *
 * Purpose: Converts an int quaternion from host byte order to network byte 
 *          order
 * In:      quaternion: Pointer to an array of ints containing the quaternion
 * In/Out:  netquaternion:  Pointer to an array of short ints as destination
 */
void convertQuaternionhtons(int* quaternion, short int* netquaternion);

/*** function initNetRecordData
 *
 * Purpose: Initializes the station data part of the network record to be
 *          broadcast
 * In:      st:         Record holding the properties and data of the station
 *          buttonByte: Button byte from the station
 *          stNr:       Number of the station of the packet
 * In/Out:  nR: Network record to be initialized with station data
 */
void initNetRecordData(char* nR, StationRecord* st, char buttonByte,
                       int stNr);

/*** function initNetRecordHeader
 *
 * Purpose: Initializes the network packet to be broadcast
 * In:      servName: Pointer to a string holding the 'servername' option
 *                    from the initfile
 * In/Out:  nR: Network data record to be initialized
 */
void initNetRecordHeader(NetworkTrackerDataRecord* nR, char* servName);

/*** function initNetRecord
 *
 * Purpose: Initializes the network packet with data that will stay unchanged
 * In:      maxStationNum: Number of stations dealt with by the server
 * In/Out:  nR: Network data record to be initialized
 */
void initNetRecord(NetworkTrackerDataRecord* nR, int maxStationNum);

/*** function getNetRecSize
 *
 * Purpose: Calculates the size of the network data record
 * In:      nR: Network data record
 * Returns: Size of the network data record in bytes
 */
int getNetRecSize(NetworkTrackerDataRecord* nR);

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
int initMulticastGroup(char* toaddr, char* toport);

/*** function sendData
 *
 * Purpose: Sends Data to the MulticastGroup
 * In:      sock:   descriptor of the socket
 *          data:   pointer to the data to be sent
 *          length: Length of the data to be sent in bytes
 * Returns: -1 if an error occured
 */
int sendData(const int sock, const char *data, const int length);

#endif
