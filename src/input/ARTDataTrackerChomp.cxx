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
/** source file for ARTDataTrackerChomp class.
*
* @author Christopher Schmidt
*
* $Id$
* @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "ARTDataTrackerChomp.h"

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

#include <iostream>

#include <ace/Log_Msg.h>

using namespace std;

namespace ot {

// Destructor method

ARTDataTrackerChomp::~ARTDataTrackerChomp()
{	
}


ARTDataTrackerChomp::ARTDataTrackerChomp()
{
}

void ARTDataTrackerChomp::chomp(std::string datagramm)
{
	
	positionStart = 0;
	positionStart = datagramm.find("fr");
	if (positionStart != 0)
	{
		//cout << "Error receiving correct Data!!! [#001]" << endl;
		//cout << "Check if Format in ARTTracker Software is set to ASCII !!!" << endl;
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error receiving correct Data!!! [#001]\n")));
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Check if Format in ARTTracker Software is set to ASCII !!!\n")));
		return;
	}
	
	// get the frame number
	positionStart = 3;
	positionEnd = datagramm.find("\n", positionStart +1);
	subLength = positionEnd - positionStart;	
	// c++ string format
	temp = datagramm.substr(positionStart, subLength);
	// c string format needed to be able to convert to int
	tempChar = temp.c_str();
	// convert c string to int
	frameNumber = atol(tempChar);
	// Bodies
	// get position of substring for Number of Bodies
	positionStart = datagramm.find("6d") + 3;
	positionEnd = datagramm.find(" ", positionStart +1); 
	subLength = positionEnd - positionStart;	
	// get the number of bodies 
	// c++ string format
	temp = datagramm.substr(positionStart, subLength);
	// c string format needed to be able to covert to int
	tempChar = temp.c_str();
	// convert c string to int
	numberTrackedBodies = atol(tempChar);
	// remove the [] from the body data and replace them with blanks
	// also check if there are two blanks in row. If so erase one of them
	endj = 3 * numberTrackedBodies;
	for (j = 0; j < endj; j++)
	{
		positionEnd = datagramm.find("[");
		datagramm.erase(positionEnd, 1);
		positionEnd = datagramm.find("]");
		datagramm.replace(positionEnd, 1, " ", 1);
		positionEnd = datagramm.find("  ");
		if (positionEnd >= 0)
		{
			datagramm.erase(positionEnd, 1);
		}
	} // end for j
	
	//-------------------------------
	// Find if extended Options are used the Number of calibrated Bodies
	/*
	positionStart = datagramm.find("6dcal");
	if (positionStart >= 0)
	{
		positionStart = positionStart + 6;
		positionEnd = datagramm.size();
		subLength = positionEnd - positionStart;
		temp = datagramm.substr(positionStart, subLength);
		tempChar = temp.c_str();
		numberTrackedCalBodies = (int)atof(tempChar);
		if (numberTrackedCalBodies != maxBodyNumber)
		{
			//cout << "Error receiving correct Data!!! [#002]" << endl;
			//cout << "Check if the No. of calibrated bodies is equal to maxbodies in XML !!!" << endl;
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error receiving correct Data!!! [#002]\n")));
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Check if the No. of calibrated bodies is equal to maxbodies in XML !!!\n")));
			exit ( -1 );
		}
	}
	else
	{
		numberTrackedCalBodies = 0;
		//cout << "Error receiving correct Data!!! [#003]" << endl;
		//cout << "Check if the Trackersoftware DTRacke is set to send the Number of calibrated bodies !!!" << endl;
		//cout << "Has to be set in ../Setup/system.ini" << endl;
		//cout << "at [SYSTEM] OutpuNoOf6DTargets=1 !!!!!" << endl;
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error receiving correct Data!!! [#003]\n")));
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Check if the Trackersoftware DTRacke is set to send the Number of calibrated bodies !!!\n")));
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Has to be set in ../Setup/system.ini\n")));
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:at [SYSTEM] OutpuNoOf6DTargets=1 !!!!!\n")));
		exit ( -1 );
	}
    */
	
	//-------------------------------
	
    std::map<int, BodyRecord>::iterator it;
    for ( it = tempBodyRecord.begin(); it != tempBodyRecord.end(); it++)
	{
		((*it).second).valid = false;
	}
	
	positionStart = datagramm.find("6d") + 5;
	
	for (i = 0; i < numberTrackedBodies; i++)
	{
		positionEnd = datagramm.find(" ",positionStart +1); 
		subLength = positionEnd - positionStart;	
		temp = datagramm.substr(positionStart, subLength);
		tempChar = temp.c_str();
		bodieID = atol(tempChar);
		tempBodyRecord[bodieID].id = atol(tempChar);
		positionStart = positionEnd;
		
		positionEnd = datagramm.find(" ",positionStart +1); 
		subLength = positionEnd - positionStart;	
		temp = datagramm.substr(positionStart, subLength);
		tempChar = temp.c_str();
		tempBodyRecord[bodieID].quality = (float)atof(tempChar);
		positionStart = positionEnd;
		tempBodyRecord[bodieID].valid = true;
		
		for (j = 0; j < 3; j++)
		{
			positionEnd = datagramm.find(" ",positionStart +1); 
			subLength = positionEnd - positionStart;	
			temp = datagramm.substr(positionStart, subLength);
			tempChar = temp.c_str();
			tempBodyRecord[bodieID].location[j] = (float)atof(tempChar);
			positionStart = positionEnd;
		}
		
		for (j = 0; j < 3; j++)
		{
			positionEnd = datagramm.find(" ",positionStart +1); 
			subLength = positionEnd - positionStart;	
			temp = datagramm.substr(positionStart, subLength);
			tempChar = temp.c_str();
			tempBodyRecord[bodieID].eulerAngles[j] = (float)atof(tempChar);
			positionStart = positionEnd;
		}
		
		for (j = 0; j < 9; j++) 
		{
			positionEnd = datagramm.find(" ",positionStart +1); 
			subLength = positionEnd - positionStart;	
			temp = datagramm.substr(positionStart, subLength);
			tempChar = temp.c_str();
			tempBodyRecord[bodieID].rotationMatrix[j] = (float)atof(tempChar);
			positionStart = positionEnd;
		}
		
	}// end for i

	/*
	// Markers
	// get position of substring for Number of Markers
	positionStart = datagramm.find("3d");
	if (positionStart >= 0)
	{
		positionEnd = datagramm.find(" ",positionStart +1); 
		subLength = positionEnd - positionStart;	
		// get the number of Markers 
		// c++ string format
		temp = datagramm.substr(positionStart, subLength);
		// c string format needed to be able to convert to int
		tempChar = temp.c_str();
		// convert c string to int
		numberTRackedMarkers = atol(tempChar);
		// remove the [] from the marker data and replace them with blanks
		// also check if there are two blanks in row. If so erase one of them
		endj = 2 * numberTRackedMarkers;
		for (j = 0; j < endj; j++)
		{
			positionEnd = datagramm.find("[");
			datagramm.erase(positionEnd, 1);
			positionEnd = datagramm.find("]");
			datagramm.replace(positionEnd, 1, " ", 1);
			positionEnd = datagramm.find("  ");
			if (positionEnd >= 0)
			{
				datagramm.erase(positionEnd, 1);
			}
		} // end for j
		tempMarkerRecord = new MarkerRecord[numberTRackedMarkers];
		
		positionStart = datagramm.find("3d") + 5;
		for (i = 0; i < numberTRackedMarkers; i++)
		{
			positionEnd = datagramm.find(" ",positionStart +1); 
			subLength = positionEnd - positionStart;	
			temp = datagramm.substr(positionStart, subLength);
			tempChar = temp.c_str();
			bodieID = atol(tempChar);
			tempMarkerRecord[bodieID].id = atol(tempChar);
			positionStart = positionEnd;
			
			positionEnd = datagramm.find(" ",positionStart +1); 
			subLength = positionEnd - positionStart;	
			temp = datagramm.substr(positionStart, subLength);
			tempChar = temp.c_str();
			tempMarkerRecord[bodieID].quality = atof(tempChar);
			positionStart = positionEnd;
			
			for (j = 0; j < 3; j++)
			{
				positionEnd = datagramm.find(" ",positionStart +1); 
				subLength = positionEnd - positionStart;	
				temp = datagramm.substr(positionStart, subLength);
				tempChar = temp.c_str();
				tempMarkerRecord[bodieID].location[j] = atof(tempChar);
				positionStart = positionEnd;
			}
		}	
		delete [] tempMarkerRecord;
	}
	*/
}


void ARTDataTrackerChomp::displayRecords()
{
	// Output
	cout << "Contens of tempBodyRecord & tempMarkerRecord" << endl;
    std::map<int, BodyRecord>::iterator it;
    for ( it = tempBodyRecord.begin(); it != tempBodyRecord.end(); it++)
	{
		//cout << "Framenumber of Datagramm is: " << frameNumber << endl;
		//cout << "Numer of  Tracked Bodies for 6d: " << numberTrackedBodies << endl;
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Framenumber of Datagramm is: %d\n"), frameNumber));
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Numer of  Tracked Bodies for 6d: %d\n"), numberTrackedBodies));
		if ( tempBodyRecord[i].valid == true )
		{
			//cout <<	"tempBodyRecord[" << i << "].id " << tempBodyRecord[i].id << endl;
			//cout <<	"tempBodyRecord[" << i << "].quality " << tempBodyRecord[i].quality << endl;
			ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].id  %d\n"), i, tempBodyRecord[i].id));
			ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].quality %f\n"), i, tempBodyRecord[i].quality));
			int j;
			for(j=0; j < 3; j++)
			{
				//cout <<	"tempBodyRecord[" << i << "].location[" << j << "]: " << tempBodyRecord[i].location[j] << endl;
				ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].location[%d]: \n"), i,j, tempBodyRecord[i].location[j]));
			}
			for(j=0; j < 3; j++)
			{
				//cout <<	"tempBodyRecord[" << i << "].eulerAngles[" << j << "]: " << tempBodyRecord[i].eulerAngles[j] << endl;
				ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].eulerAngles[%d]: \n"), i,j, tempBodyRecord[i].eulerAngles[j]));
			}
			for(j=0; j < 9; j++)
			{
				//cout <<	"tempBodyRecord[" << i << "].rotationMatrix[" << j << "]: " << tempBodyRecord[i].rotationMatrix[j] << endl;
				ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].rotationMatrix[%d]: \n"), i,j, tempBodyRecord[i].rotationMatrix[j]));
			}
		}// END if
		else
		{
			//cout << "#### No Valid DATA for this Body ####" << endl;
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:#### No Valid DATA for this Body ####")));
		}// END else
	}
    int i;
	for(i=0; i < numberTRackedMarkers; i++)
	{
		//cout << "Numer of Markers for 3d: " << numberTRackedMarkers << endl;
		//cout <<	"tempMarkerRecord[" << i << "].id " << tempMarkerRecord[i].id << endl;
		//cout <<	"tempMarkerRecord[" << i << "].quality " << tempMarkerRecord[i].quality << endl;
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Numer of Markers for 3d: %d\n"), numberTRackedMarkers));
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:tempMarkerRecord[%d].id %d\n"), i, tempMarkerRecord[i].id));
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:tempMarkerRecord[%d].quality %d\n"), i, tempMarkerRecord[i].quality));

		for(int j=0; j < 3; j++)
		{
			//cout <<	"tempMarkerRecord[" << i << "].location[" << j << "]: " << tempMarkerRecord[i].location[j] << endl;
			ACE_DEBUG((LM_INFO, ACE_TEXT("ot:tempBodyRecord[%d].location[%d]: \n"), i,j, tempBodyRecord[i].location[j]));
		}
	}
	if(numberTrackedCalBodies != 0)
	{
		//cout << "Number of calibrated Bodies: " << numberTrackedCalBodies << endl;
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Number of calibrated Bodies: %d\n"), numberTrackedCalBodies));
	}
}

int ARTDataTrackerChomp::getFrameNumber()
{
	return frameNumber;
}

int ARTDataTrackerChomp::getTrackedBodyNumber()
{
	return numberTrackedBodies;
}

std::map<int, ARTDataTrackerChomp::BodyRecord > & ARTDataTrackerChomp::getBodyRecord()
{
	return tempBodyRecord;
}

int ARTDataTrackerChomp::getTrackedMarkerNumber()
{
	return numberTRackedMarkers;
}

ARTDataTrackerChomp::MarkerRecord* ARTDataTrackerChomp::getMarkerRecord()
{
	return tempMarkerRecord;
}

void ARTDataTrackerChomp::pushMarkerRecord(ARTDataTrackerChomp::MarkerRecord* markerrecord)
{
	tempMarkerRecord = markerrecord;
}

int ARTDataTrackerChomp::getCalibratedTrackedBodyNumber()
{
	return numberTrackedCalBodies;
}

} // namespace ot
