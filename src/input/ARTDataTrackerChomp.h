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
/** header file for ARTDataTrackerChomp class.
*
* @author Christopher Schmidt
*
* $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ARTDataTrackerChomp.h,v 1.5 2003/07/18 17:08:24 tamer Exp $
* @file                                                                   */
/* ======================================================================= */

#ifndef _ARTDATATRACKERCHOMP_H
#define _ARTDATATRACKERCHOMP_H

#include <string>

/**
* Class chomp the received ASCII String (frame) 
* and brings the Data in two structured Arrays
* One for the bodies and one for the markers
* @author Christopher Schmidt
* @ingroup input
*
* @note Documentation of the ART Data tracker format:
@verbatim
ASCII Format des A.R.T. Trackers

Normal 3 Zeilen  mit aktivierter Zustatzfunktion 4 Zeilen

1. Zeile:		Framenummer

fr			21753

2. Zeile:		6D-Werte, ANzahl der Bodies, Bodies

6d			1 [0 1.000] [326.848 -187.216 109.503 -160.4704 -3.6963 -7.0913]
			[-0.940508 -0.332545 0.333588 -0.932566 0.137735 -0.064467 0.123194 0.990286]
			
			1
			gibt die Anzahl der Bodies die folgen

			[0 1.000]
			1. Zahl --> Index beginennd bei 0
			2. Zahl --> Qualitaet

			[326.848 -187.216 109.503 -160.4704 -3.6963 -7.0913]
			1. Zahl --> Ort sx
			2. Zahl --> Ort sy
			3. Zahl --> Ort sz
			4. Zahl --> Winkel n
			5. Zahl --> Winkel o
			6. Zahl --> Winkel p
			
			[-0.940508 -0.332545 0.333588 -0.932566 0.137735 -0.064467 0.123194 0.990286]
			1. Zahl --> Rotationsmatrix Element b0
			2. Zahl --> Rotationsmatrix Element b1
			3. Zahl --> Rotationsmatrix Element b2
			4. Zahl --> Rotationsmatrix Element b3
			5. Zahl --> Rotationsmatrix Element b4
			6. Zahl --> Rotationsmatrix Element b5
			7. Zahl --> Rotationsmatrix Element b6
			8. Zahl --> Rotationsmatrix Element b7
			9. Zahl --> Rotationsmatrix Element b8

3. Zeile		3D-Werte, Anzahl der Marker, Marker

3d			2 [79 1.000] [210.123 -90.123 -108.123] [80 1.000] [-90.123 -50.123 108.123]

			[0 1.000]
			1. Zahl --> Index beginennd bei 0
			2. Zahl --> Qualitaet

			[210.123 -90.123 -108.123]
			1. Zahl --> Ort sx
			2. Zahl --> Ort sy
			3. Zahl --> Ort sz

Zusatzzeile am Ende des Pakets gibt Anzahl der einkalibrierten 6D Bosies an

6dcal			3
@endverbatim
*/
class ARTDataTrackerChomp
{	
	
public:
	
	typedef struct BodyRecord		// Structur for the 6d Bodies
	{
		unsigned long id;			// Body ID taken from the Datagramm
		float quality;				// Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
		float location[3];			// Array for the loaction of the Body (s0 s1 s2)
		float eulerAngles[3];		// Array for the Eulerangles
		float rotationMatrix[9];	// Array for the Rotation Matrix
		float orientation[4];		// Array for the Quaternion (calculated by ARTDataTrackerModule) pushed in the Nodetree
		float position[3];			// Array for the position (location) pushed in the Nodetree
		bool  valid;				// Flag is true if body is tracked by DTrack
	} BodyRecord;
	
	typedef struct MarkerRecord     // Structur for the 3d Markers
	{
		unsigned long id;			// Marker ID taken from the Datagramm
		float quality;				// Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
		float location[3];			// Array for the loaction of the Body (s0 s1 s2)		
	} MarkerRecord;

	int bodieID;
	int frameNumber;
	int numberTrackedBodies;
	int numberTRackedMarkers;
	int numberTrackedCalBodies;
	int maxBodyNumber;
	int i;
	int j;
	int endj;
	int positionStart;
	int positionEnd;
	int subLength;

	const char *tempChar;
	BodyRecord *tempBodyRecord;
	MarkerRecord *tempMarkerRecord;
	std::string temp;

/* Methods of ARTDataTrackerChomp
 *
 */


public:
	ARTDataTrackerChomp(int maxBodyNumber);
    virtual ~ARTDataTrackerChomp();
	virtual void displayRecords();			// Dsiplay the contens of the RecordArray
	virtual void chomp(std::string Datagramm);		// Main part that chomp the string
	virtual int getFrameNumber();								// return the framenumber
	virtual int getTrackedBodyNumber();						// return the Number of tracked Bodies
	virtual int getTrackedMarkerNumber();						// return the Number of tracked Markers	
	virtual int getCalibratedTrackedBodyNumber();				// return the Number of calibrated & tracked Bodies	
	virtual BodyRecord* getBodyRecord();						// return the BodyRecord
	virtual void ApushBodyRecord(BodyRecord* bodyrecord);		// push the BodyRecord
	virtual MarkerRecord* getMarkerRecord();					// return the MarkerRecord
	virtual void pushMarkerRecord(MarkerRecord* markerrecord);	// push the MarkerRecord
};

#endif
