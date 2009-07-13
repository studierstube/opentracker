 /* ========================================================================
  * Copyright (c) 2006,
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
/** header file for CalibSinkSource Node.
  *
  * @author Markus Tatzgern
  *
  * $Id: CalibSinkSource.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section CalibSinkSource CalibSinkSource
 * The CalibSinkSource node is a simple EventGenerator that outputs a calibrated
 * position and orientation. calibration is done using an EventKeyboardSource.
 * It is driven by the @ref CalibModule. 
 *
 * An example element looks like this :
 * @verbatim
 <PosOrientCalibSinkSource samplingFreq="20">
     <CalibTrigger>               
     <EventKeyboardSource number="8" DEF="keyboard"/>               
     </CalibTrigger>

     <CameraOffset> 
         <EventTransform scale="1 1 1" translation="0 0 0" rotationtype="euler" rotation="0.0 0.0 0.0">
            <TestSource/>
         </EventTransform>
     </CameraOffset>

     <CoordFrameOffset>
         <EventTransform scale="1 1 1" translation="0 0 0" rotationtype="euler" rotation="0.0 0.0 0.0">
            <TestSource/>
         </EventTransform>
     </CoordFrameOffset>

     <Position>                
         <EventTransform scale=1 1 1" translation="0 0 0" rotationtype="euler" rotation="0.0 0.0 0.0">
             <Ref USE="keyboard"/> 
         </EventTransform>                
     </Position>

     <Orientation>
         <EventTransform scale="1 1 1" translation="0 0 0" rotationtype="euler" rotation="0.0 0.0 0.0">
            <Ref USE="keyboard"/>
         </EventTransform> 
     </Orientation> 
 </PosOrientCalibSinkSource>
 @endverbatim
 */

#ifndef _CALIBSINKSOURCE_H
#define _CALIBSINKSOURCE_H

#include <stdio.h>
#include <vector>


#include "../OpenTracker.h"
#include "../common/ConsoleSource.h"

#ifdef USE_CALIBMODULE

namespace ot {

/**
 * This class implements a source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Markus Tatzgern
 * @ingroup input
 */
class OPENTRACKER_API CalibSinkSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    Event event;
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    /** pushes event down the line. */
    void push();
	void pushEvent( );
    void pullEvent( );

	/** simple constructor, sets members to initial values */
	CalibSinkSource() : Node(), 
        compensateOrientationKey(false),compensatePositionKey(false),
        newPositionEvent(false),newOrientationEvent(false)
	{	
        float initOri[4]={0.0f, 0.0f, 0.0f, 1.0f};
        float initPos[3]={0.0f, 0.0f, 0.0f};

        oldButton = 0;

        //qCompOr = initOri;
        //vCompPos = initPos;
        qCompOr = std::vector<float>(4);
        qCompOr[3] = 1.0;
        vCompPos = std::vector<float>(3);
        positionEvent.setPosition(initPos);
        positionEvent.setOrientation(initOri);
        cameraOffsetEvent.setPosition(initPos);
        cameraOffsetEvent.setOrientation(initOri);
        coordFrameOffsetEvent.setPosition(initPos);
        coordFrameOffsetEvent.setOrientation(initOri);
        orientEvent.setOrientation(initOri);
        orientEvent.setOrientation(initOri);
	}

	~CalibSinkSource();
	

virtual void onEventGenerated( Event& event, Node& generator);
protected:

	
private:

    void sampleCompOrientation();
    void sampleCompPosition();
    void calculateCompensation();

    bool compensateOrientationKey;
    bool compensatePositionKey; 

    bool newPositionEvent;
    bool newOrientationEvent;

    Event positionEvent;
    Event orientEvent;
    Event cameraOffsetEvent;
    Event coordFrameOffsetEvent;

    std::vector<float> qCompOr;
    std::vector<float> vCompPos;

    unsigned short oldButton;

};

}  // namespace ot

#endif // USE_CALIBMODULE

#endif //_CALIBSINKSOURCE_H
