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
/** header file for Network data structures
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                    */
 /* ======================================================================== */

#ifndef _NETWORK_H
#define _NETWORK_H

/**
 * @defgroup network Network Classes
 * This group contains classes related to network input / output. Sofar only
 * a single network protocol is implemented and the whole design is very
 * basic. It is expected to grow in the future to be more versatile.
 */

/** the data structure for a record of the Flexible Network Protocol used by
 * the Studierstube. 
 * @ingroup network
 */
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
