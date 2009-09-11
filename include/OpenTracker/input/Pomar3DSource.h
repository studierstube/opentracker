/*------------------------------------------------------------------------------
 * Pomar3D
 * Customer: ICG
 
 *------------------------------------------------------------------------------

 * Supplier:
 * Institute of Navigation and Satellite Geodesy
 * Graz University of Technology
 * Steyrergasse 30
 * 8010 Graz
 * Austria

 *------------------------------------------------------------------------------

 * Copyright (C) 2008 Institute of Navigation and Satellite Geodesy
 * All rights reserved. 

 * THE AUTHORING COMPANIES DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
 * SHALL THE AUTHORING COMPANY BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUEN-
 * TIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR 
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS AC-
 * TION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS 
 * SOFTWARE. 
 
 *------------------------------------------------------------------------------

 * File Type:   C/C++ Header File
 * File Name:   Pomar3DSource.h
 * Authors:     P. Berglez 
 * Description: Implementation file for the DLL 
 
 *------------------------------------------------------------------------------

 * Versions / History
 * Version      Date      Author        Comment
 * 01.00.00.00  25062008  P. Berglez      First issue
 *----------------------------------------------------------------------------*/ 
#ifndef _POMAR3DSOURCE_H
#define _POMAR3DSOURCE_H

#include <assert.h>
#include <vector>

#include "../OpenTracker.h"
#include "GPSDriver.h"

#ifndef OT_NO_GPS_SUPPORT

namespace ot 
{

    class OPENTRACKER_API Pomar3DSource : public Node, public GPSListener
    {
	    public:

			/// the event that is posted to the EventObservers
			Event event;
			/// the buffer event for data from the GPS receiver
			Event buffer;

			/** tests for EventGenerator interface being present. Is overriden to
			 * return 1 always.
			 * @return always 1 */
			virtual int isEventGenerator()
			{
				return 1;
			}
			
			
			virtual void newData( const GPResult * point, const char * line, void * userData );

		protected:
			/// protected constructor so it is only accessible by the module
			Pomar3DSource() {};

			void pushEvent()
			{
				lock();
				if(event.time < buffer.time )
				 {
					  event = buffer;
					  unlock();
					 updateObservers( event );
				 }
				 else
				{
					unlock();
				 }
				
				//////////std::vector<float> posnew;
				//////////posnew.resize(3);
				//////////posnew[0]=999.0;
				//////////posnew[1]=888.0;
				//////////posnew[2]=777.0;

				//////////printf("FUCK\n");
				////////lock();
				////////if(event.time < buffer.time)
				////////{
				//////////buffer.setPosition(posnew);

				////////	//printf("HERE: %f %f %f\n",buffer.getPosition()[0],buffer.getPosition()[1],buffer.getPosition()[2]);
				////////	event = buffer;
				////////	unlock();
				////////	updateObservers( event );
				////////}
				////////else
				////////{
				////////	unlock();
				////////}
			}

		void pullEvent() {};

		friend class Pomar3DModule;
	};

	inline void Pomar3DSource::newData( const GPResult * res, const char * line, void * userData )
    {
        ACE_ASSERT( userData != NULL );

		Pomar3DModule * module = (Pomar3DModule *)userData;
		
		//printf("!!!!!!!!!!!!!!!!!!!! HERE !!!!!!!!!!!!!!!!!!!!!!!!\n" );

	//	module->lockLoop();

		////////buffer.timeStamp();
 
		//////// float ECEF[3];
		////////
		////////std::vector<float> ECEF1;
		////////std::vector<float> posnew;

		////////ECEF1.resize(3);

		////////posnew.resize(3);

		////////ECEF[0]=6666.0;
		////////ECEF[1]=7777.0;
		////////ECEF[2]=8888.0;
		////////
		////////ECEF1[1]=ECEF[0];
		////////ECEF1[2]=ECEF[1];
		////////ECEF1[3]=ECEF[2];


  ////////          buffer.getConfidence() = (float)(1 / 2.0);

		////////	buffer.setAttribute<double>("lat", ECEF[0]); //point->lat); 
  ////////          buffer.addAttribute<double>("lon", ECEF[1]); //point->lon); 
  ////////          buffer.addAttribute<double>("alt", ECEF[2]); //point->altitude); 
  ////////          buffer.addAttribute<short>("numsats", 9); 


		
	//		buffer.setPosition(ECEF1);

		/*buffer.getPosition()[0] = ECEF[0];
        buffer.getPosition()[1] = ECEF[1];
        buffer.getPosition()[2] = ECEF[2];*/
		
		//module->unlockLoop();


	}

	
}

#else
#pragma message(">>> OT_NO_GPS_SUPPORT")
#endif // OT_NO_GPS_SUPPORT

#endif // !defined(_POMAR3DSOURCE_H)
