 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Network data structures
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/Network.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */

#ifndef _NETWORK_H
#define _NETWORK_H

/** the data structure for a record of the Flexible Network Protocol used by
 * the Studierstube. */
typedef struct
{
    short int headerId;         // Id (magic number)
    short int revNum;           // Revision Number
    short int maxStationNum;    // Number of stations dealt with by the server
    short int headerLength;     // Length of the header (from the start of the
                                // record; Offset to the first station data)
    short int numOfStations;    // Number of station packets to follow
    short int commentLength;    // Length of the comment
    char data[10000];           // Holds the following data:
                                // n bytes of comment

                                // Data per Station:
                                // short int number of the station
                                // short int format (Quaternion, Euler, Matrix)
                                // short int button states (binary coded)
                                // short int bytes per station (incl. this header)
                                // short int length of the name of the station
                                // n bytes name of the station
                                // position and orientation according to format
} FlexibleTrackerDataRecord;

#endif
