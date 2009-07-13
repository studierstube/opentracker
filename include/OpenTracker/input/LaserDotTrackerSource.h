/* ========================================================================
 * Copyright (c) 2008,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for VideoLocationModule.
 *
 * @author Markus Sareika
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section LaserDotTrackerSource
 * @li @c ovSink: name of the video sink which provides the tracking image
 * An example configuration element looks like this :
 * @verbatim
 <LaserDotTrackerConfig /> 
 <LaserDotTrackerSource ovSink="VideoStream" threshold="15" filter="G"/> @endverbatim
 */


#ifndef _LASERDOTTRACKERSOURCE_H
#define _LASERDOTTRACKERSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_LASERDOTTRACKER

#include <ace/OS.h>


namespace ot {
 
    class OPENTRACKER_API LaserDotTrackerSource : public Node
    {
    public:        

		/// the new event
        Event event;
		

    
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator(){ return 1; }
    
        void pushEvent()
		{ 
			if(newData)
			{
				newData=0;
				updateObservers(event); 
			}
		}
        void pullEvent(){}

    protected:
		
		int filter, width, height, info;
		float threshold, thresholdR, thresholdG, thresholdB;
		unsigned char* imgBuffer;

	friend class LaserDotTrackerModule;

	bool initialized;

	LaserDotTrackerSource() : Node()
    { 
		initialized = false;
		info=0;
		filter=0;
        threshold=0;
		thresholdR=0;
		thresholdG=0;
		thresholdB=0;
	}

	void init(int w , int h)
    {
		width = w;
		height = h;
		initialized = true;
		imgBuffer = new unsigned char[width*height*3];
        memset(imgBuffer, 0, width*height*3);
    }

	void process()
    {
        rx=0;
        ry=0;
		int n(0);
        for (int i(0),p(0); i<3*width*height; i=i+3, p++)
        {
            float strength =((float)imgBuffer[i]+
                (float)imgBuffer[i+1]+
                (float)imgBuffer[i+2])/3;
			switch(filter)
            {
			// threshold only
            case 0:
                {
					if( strength>threshold )
					{
						rx+= p%width;
						ry+= p/width;
						n++;
					}
                    break;
                }
			// threshold & red channel dominant
            case 1:
                {
					if( strength>threshold && 
						((float)imgBuffer[i]>(float)imgBuffer[i+1]) &&
						((float)imgBuffer[i]>(float)imgBuffer[i+2]) )
					{
						rx+= p%width;
						ry+= p/width;
						n++;
					}
                    break;
                }
			// threshold & green channel dominant
            case 2:
                {
					if( strength>threshold && 
						((float)imgBuffer[i+1]>(float)imgBuffer[i]) &&
						((float)imgBuffer[i+1]>(float)imgBuffer[i+2]) &&
						((float)imgBuffer[i+1]>thresholdG))
					{
						rx+= p%width;
						ry+= p/width;
						n++;					
						if(info == 2)
						{
							printf("pixel strength: %f RGB: %i, %i, %i \n" , strength, imgBuffer[i], imgBuffer[i+1], imgBuffer[i+2]);
							OSUtils::sleep(1000);
						}
					}

                    break;
                }
			// threshold & blue channel dominant
			case 3:
			{
				if( strength>threshold && 
					((float)imgBuffer[i+2]>(float)imgBuffer[i+1]) &&
					((float)imgBuffer[i+2]>(float)imgBuffer[i]) )
				{
					rx+= p%width;
					ry+= p/width;
					n++;
				}
				break;
			}
			// threshold RGB
			case 4:
			{
				if( strength>threshold && 
					((float)imgBuffer[i]>thresholdR) &&
					((float)imgBuffer[i+1]>thresholdG) &&
					((float)imgBuffer[i+2]>thresholdB) )
				{
					rx+= p%width;
					ry+= p/width;
					n++;
				}
				break;
			}
            default:
                break;
            }
        }
		if(info)
		{
			printf("number of filtered pixels:%i \n", n);
		}
        if(rx||ry)
        {
			rx/=n;
			ry/=n;
            // put result into event
            event.getPosition()[0]=(float)rx/(float)width;
            event.getPosition()[1]=(float)ry/(float)height;
            event.timeStamp();
			newData=1;
        }
    }

	private:
	int rx, ry, newData;

    };
}  // namespace ot


#endif //USE_LASERDOTTRACKER

#endif // _LASERDOTTRACKERSOURCE_H

