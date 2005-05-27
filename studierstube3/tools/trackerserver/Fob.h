// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Fob
//  TYPE:       h header file
//  PROJECT:    Studierstube
//  CONTENT:    Definition of Ascension FOB specific functions used in the
//              trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:18:12  rr      last modification
// ===========================================================================


#ifndef _FOB_H_
#define _FOB_H_

#include <string.h>
#include <curses.h>
#include "GlobalDef.h"
#include "SerComm.h"
#include "CfgOpts.h"

/*** function readFobRecordSingle
 *
 * Purpose: Reads data from a station via the master bird, checks the sync bit
 *          if it's the beginning of a record and returns the number of a
 *          station if a record has been completely read into the buffer
 * In:      numOfStations: Number of stations to be read from
 *          stNr:          Number of station which is expected to belong the
 *                         data to
 * In/Out:  station: Pointer to the record of the stations holding the
 *                   filedescriptor of the serial interconnection, the
 *                   buffer where the record being read from the bird
 *                   is stored, as well as the length of that record.
 * Returns: Number of station whose record has been completed
 *          -1 else
 */
int readFobRecordSingle(StationRecord* station, int numOfStations, int stNr);

/*** function readFobRecordMulti
 *
 * Purpose: Reads data from the specified number of birds, checks the sync
 *          bit if it's the beginning of a record and returns the number of
 *          a station if a record has been completely read into the buffer
 * In:      numOfStations: Number of stations to be read from
 * In/Out:  station: Pointer to the record of the stations holding the
 *                   filedescriptor of the serial interconnection, the
 *                   buffer where the record being read from the bird
 *                   is stored, as well as the length of that record.
 * Returns: -1 if a timeout occured
 *          Number of station whose record has been completed
 *          -2 else
 */
int readFobRecordMulti(StationRecord* station, int numOfStations);

/*** function buildPosition
 *
 * Purpose: Convert position bytes from Ascension into actual position float
 *          values
 * In:      inputBuffer: Buffer containing bytes received from Ascension FOB
 * In/Out:  position: Pointer to an array of floats for the position to
 *                    be put in
 */
void buildPosition(char *inputBuffer, float *position);

/*** function buildQuaternion
 *
 * Purpose: Convert quaternion bytes from Ascension into actual quaternion
 *          float values
 * In:      inputBuffer: Buffer containing bytes received from Ascension
 * In/Out:  orientation: Pointer to an array of floats for the quaternion to
 *                       be put in
 */
void buildQuaternion(char *inputBuffer, float *orientation);

/*** function buildAngles
 *
 * Purpose: Convert Euler angle bytes from Ascension into actual Euler angle
 *          (azimuth, elevation, roll) float values
 * In:      inputBuffer: Buffer containing bytes received from Ascension
 * In/Out:  angles: Pointer to an array of floats for the angles to be
 *                  put in.
 */
void buildAngles(char *inputBuffer, float *angles);

/*** function buildMatrix
 *
 * Purpose: Convert matrix bytes from Ascension into actual matrix float values
 * In:      inputBuffer: Buffer containing bytes received from Ascension
 * In/Out:  matrix: Pointer to an array of floats for the matrix to be
 *                  put in.
 */
void buildMatrix(char *inputBuffer, float *matrix);

/*** function recoverTimeout
 *
 * Purpose: Tries to recover the stations from a timeout.
 * In:      numOfStations: Number of stations being used in the FOB
 *          station: Pointer to the record of the stations holding the
 *                   filedescriptors of serial interconnections to the
 *                   birds
 */
void recoverTimeout(int numOfStations, StationRecord* station);

/*** function initTrackersFob
 *
 * Purpose: Initializes the stations of the tracker with the config file
 * In:      initFilename: Pointer to the name of the config file
 *          mode:         Mode of operation, either fobSingle or fobMulti
 *          fdCount:      Number of stations in the Flock Of Birds
 * In/Out:  station: Pointer to the record of the stations used to keep
 *                   track of its properties
 */
void initTrackersFob(char* initFilename, int mode, StationRecord* station,
                    int fdCount);

/*** function birdSleep
 *
 * Purpose: Puts Flock Of Birds to Sleep
 * In:      fdMaster: Filedescriptor of the Masterbird
 */
void birdSleep(int fdMaster);

/*** function startFlock
 *
 * Purpose: Starts Stream Mode and enables Button Mode and/or Group Mode
 *          if necessary
 * In:      fdCount: Number of stations
 *          mode:    Mode of operation, either fobSingle or fobMulti
 * In/Out:  station: Pointer to the record of the station used to keep
 *                   track of its properties
 */
void startFlock(int fdCount, int mode, StationRecord* station);

#endif
