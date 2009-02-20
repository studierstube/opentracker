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
/** header file for PosOrientCalibSinkSource Node.
 *
 * @author Markus Tatzgern
 *
 * $Id: CalibSinkSource.h
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/input/CalibSinkSource.h>
#ifdef USE_CALIBMODULE

#include <cmath>

namespace ot {


    // Process Data received from incoming Events
    void CalibSinkSource::onEventGenerated( Event& event, Node& generator) 
    {
        // wait for calibration trigger
        if (generator.getName().compare("CalibTrigger") == 0) {
            unsigned short button = event.getButton();

            if(button != oldButton)
            {
                oldButton = button;

                if(button & 0x01)
                    compensateOrientationKey=true;
                if(button & 0x02)
                    compensatePositionKey=true;		
            }
            //return;
        }
        // the tracked position
        if (generator.getName().compare("Position") == 0) {
            // store input event from sink		
            newPositionEvent=true;
            positionEvent=event;
            //return;
        }
        // the tracked orientation
        if (generator.getName().compare("Orientation") == 0) {
            // store input event from sink
            newOrientationEvent=true;
            orientEvent=event;
            //return;
        }
        // the offset between position sensor and camera
        // offset to camera from position sensor. the offset is measured from sensor to camera, where the 
        // coordinate frame is oriented to be the camera viewing plane (calibration of orientation 
        // has to be done beforehand.
        if (generator.getName().compare("CameraOffset") == 0) {
            // store input event from sink
            cameraOffsetEvent=event;
            //return;
        }

        // after calibrating the camera to lie in the origin of 
        // the coordinate frame, it can be modified using this transformation
        if (generator.getName().compare("CoordFrameOffset") == 0) {
            // store input event from sink
            coordFrameOffsetEvent=event;
            //return;
        }
    }

    void CalibSinkSource::pushEvent()
    {
        lock();

        bool data_processed = false;

        // if new events arrived and corresponding calibration key was pressed
        // sample values for compensation
        if (newOrientationEvent && compensateOrientationKey)
        {
            compensateOrientationKey = false;
            sampleCompOrientation();
        }
        if (newPositionEvent && compensatePositionKey)
        {
            compensatePositionKey = false;
            sampleCompPosition();
        }

        if(newOrientationEvent || newPositionEvent)
        {
            newOrientationEvent = false;
            newPositionEvent = false;

            calculateCompensation();
            unlock();
            updateObservers(event);
        }
        else
            unlock();
    }
    
    void CalibSinkSource::pullEvent()
    {
    }

    void CalibSinkSource::sampleCompOrientation()
    {
        MathUtils::invertQuaternion(orientEvent.getOrientation(), qCompOr);
    }

    void CalibSinkSource::sampleCompPosition()
    {
        for(int i(0); i < 3; i++) vCompPos[i] = (-1.0) * positionEvent.getPosition()[i];
    }

    void CalibSinkSource::calculateCompensation()
    {
        // compensate orientation
        MathUtils::Quaternion qOrIn = {orientEvent.getOrientation()[0], orientEvent.getOrientation()[1], orientEvent.getOrientation()[2], orientEvent.getOrientation()[3]};
        MathUtils::Quaternion qCompOrInv = { qCompOr[0],qCompOr[1],qCompOr[2],qCompOr[3]};

        MathUtils::Quaternion qOrRes;
        MathUtils::multiplyQuaternion(qCompOrInv, qOrIn, qOrRes);
        MathUtils::Matrix4x4 mOrRes = { {1,0,0, 0},
                                        {0,1,0, 0},   
                                        {0,0,1, 0},
                                        {0,0,0, 1}};
        MathUtils::quaternionToMatrix(qOrRes, mOrRes);

        // compensate position
        std::vector<float> vPos(3);
        for(int i(0); i < 3; i++) vPos[i] = positionEvent.getPosition()[i] + vCompPos[i];

        MathUtils::Matrix4x4 mPos = {{1,0,0, vPos[0]},
                                     {0,1,0, vPos[1]},   
                                     {0,0,1, vPos[2]},
                                     {0,0,0, 1}};

        // initialize offset and coordinate frame matrices
        MathUtils::Matrix4x4 mPosOffset = { {1,0,0, cameraOffsetEvent.getPosition()[0]},
                                            {0,1,0, cameraOffsetEvent.getPosition()[1]},   
                                            {0,0,1, cameraOffsetEvent.getPosition()[2]},
                                            {0,0,0, 1}};
        MathUtils::Matrix4x4 mCoordFrame = {{1,0,0, coordFrameOffsetEvent.getPosition()[0]},
                                            {0,1,0, coordFrameOffsetEvent.getPosition()[1]},   
                                            {0,0,1, coordFrameOffsetEvent.getPosition()[2]},
                                            {0,0,0, 1}};

        // calculate final position
        MathUtils::Matrix4x4 mTemp, mTemp2, mPosRes;
        MathUtils::matrixMultiply(mPos, mCoordFrame, mTemp);
        MathUtils::matrixMultiply(mTemp, mOrRes, mTemp2);
        MathUtils::matrixMultiply(mTemp2, mPosOffset, mPosRes);

        // calculate final orientation
        // do not need orientation offset because it is included in the compensated
        // orientation. hence mOrRes is already the final orientation
        // could include an orientation change from coordFrameOffsetEvent.

        // put result into event
        for (int i(0);i<3;i++) event.getPosition()[i]=(float)mPosRes[i][3];
        for (int i(0);i<4;i++) event.getOrientation()[i]=(float)qOrRes[i];
            
        event.timeStamp();
    }

	
    CalibSinkSource::~CalibSinkSource()
    {
        // Destructor method
    }
	
}

#endif // USE_CALIBMODULE

/* 
 * ------------------------------------------------------------
 *   End of PosOrientCalibSinkSource.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
