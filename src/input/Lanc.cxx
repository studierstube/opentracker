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
/** source file for Lanc Node.
*
* @author Markus Sareika
*
* $Id: Lanc.cpp
* @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <ace/OS.h>
#ifdef WIN32
#include <windows.h>
#endif //WIN32
#include <iostream>

#include <OpenTracker/input/Lanc.h>

#ifdef USE_ZOOM
#include <OpenTracker/core/OSUtils.h>


namespace ot {
    
	//constructor
	Lanc::Lanc():zoomout(false),
		zoomin(false),
		wideAngle(37.1f),
		startTime(0),
		maxTimePos(8000.f),
		initLanc(false),
		shuttle(false),
		setTimePos(0),
		zoomFactor(1.),
		relativeIn(false),
		relativeOut(false),
		emitEvent(false)
	{
		timePos=maxTimePos;
		#ifdef WIN32
		//initialize lanc
		int id = FindLancUSBDevice();
		if (GetUSBID(0, usb_id))
		{
			// get handle
			int retCode = OpenLancDeviceEx(&lancDevice,  usb_id,  NULL, LANCCALLBACK_NULL);
			int messageId = GetRegisteredLancMessage(lancDevice);
			// zoom out to maximum
			SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_8, NULL);
			OSUtils::sleep(3000);
			SendLancCommand(lancDevice,  0, 0);
			initLanc=true;
		}
		else
			std::cerr << "failed to get lanc usb id" << std::endl;
		#endif
	}
	
	Lanc::~Lanc()
	{
		//destructor
		#ifdef WIN32
		if (initLanc) CloseLancDevice(lancDevice);
		#endif
	}

	void Lanc::setTeleAngle(float tele)
	{
		teleAngle=tele;
	}

	void Lanc::setWideAngle(float wide)
	{
		wideAngle=wide;
	}

	float Lanc::getFieldOfView()
	{
		zoomFactor = (timePos/maxTimePos);
		float fieldOfView = wideAngle*(float)zoomFactor;
		return fieldOfView;
	}

	void Lanc::noShuttle()
	{
		shuttle = false;
	}

	bool Lanc::isShutteling()
	{
		return shuttle;
	}

	void Lanc::adjustToZoomFactor(float zoomFactor)
	{
		setTimePos = zoomFactor * maxTimePos;
		if (setTimePos < 200.f) setTimePos = 200.f;
		if (setTimePos > maxTimePos) setTimePos = maxTimePos;
		if (setTimePos == timePos) return;
		shuttle = true;
	}

	void Lanc::relativeZoom(float speed)
	{
		relativeZooming = true;
		
		if (speed > 0.2f){
			relativeIn = true;
			shuttle=true;
			return;
		}
		if (speed < -0.2f){
			relativeOut = true;
			shuttle=true;
			return;
		}
		relativeIn = false;
		relativeOut = false;
		shuttle=false;
	}

	void Lanc::updateTimePos()
	{
		double time = OSUtils::currentTime();
		double diff = (time-startTime);
		startTime = time;

		if( zoomin )
		{
			timePos -= diff;
			if (timePos < 200.f){
				timePos = 200.f;
				shuttle= false;
			}
		}
		if( zoomout )
		{
			timePos += diff;
			if (timePos > maxTimePos) {
				timePos = maxTimePos;
				shuttle = false;
			}
		}

		if(relativeIn)setTimePos=maxTimePos;
		if(relativeOut)setTimePos=0;
		
		//printf("set: %f  timepos: %f\n",setTimePos, timePos);

		if( (setTimePos < timePos && shuttle) || (setTimePos > timePos && shuttle) )
		{
			double precision = 50;
			if(setTimePos < timePos-precision){ 
				Zoom( 0.2f );
                emitEvent=true;
				return;
			}
			if(setTimePos > timePos+precision){ 
				Zoom( -0.2f );
                emitEvent=true;
				return;
			}
		}
		
		shuttle = false;
		Zoom( 0.f );
		emitEvent=true;
	}

	bool Lanc::isZooming()
	{
		if( zoomin || zoomout || shuttle) return true;
		return false;
	}

	// mapping: 1 > zoominspeed8-1 > 0.2 stop -0.2 > zoomoutspeed1-8 > -1
    void Lanc::Zoom( float speed )         
    {
        #ifdef WIN32

		if ( (speed<0.2f) && (speed>-0.2f) )   // zoom stop
		{
			relativeZooming = false;
			if(initLanc)
				if (!SendLancCommand(lancDevice,  0, 0)) 
					std::cerr << "LANC Device Not Responding" << std::endl;
			double time = OSUtils::currentTime();
			if (zoomin)
			{
				//timePos -= (time-startTime);
				//if (timePos < 200.f) timePos = 200.f;
				zoomin=false;
			}
			if (zoomout)
			{
				//timePos += (time-startTime);
				//if (timePos > maxTimePos) timePos = maxTimePos;
				zoomout=false;
			}
		}
		if (speed>=0.2f)
		{
			if(initLanc)
				if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SLOW, NULL)) 
					std::cerr << "LANC Device Not Responding" << std::endl;
			startTime = OSUtils::currentTime();
			zoomin=true;
			zoomout=false;
		}
		if (speed<=-0.2f)
		{
			if(initLanc)
				if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SLOW, NULL)) 
					std::cerr << "LANC Device Not Responding" << std::endl;
			startTime = OSUtils::currentTime();
			zoomout=true;
			zoomin=false;
		}

		/////////////////////////// multiple speeds mapping +8/-8
		//if (speed>=0.9)				       // zoom in speed 8
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_8, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.9) && (speed>=0.8) )   // zoom in speed 7
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_7, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.8) && (speed>=0.7) )   // zoom in speed 6
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_6, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.7) && (speed>=0.6) )   // zoom in speed 5
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_5, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}		
		//if ( (speed<0.6) && (speed>=0.5) )   // zoom in speed 4
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_4, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.5) && (speed>=0.4) )   // zoom in speed 3
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_3, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.4) && (speed>=0.3) )   // zoom in speed 2
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_2, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//
		//if ( (speed<0.3) && (speed>=0.2) )   // zoom in speed 1
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_IN_SPEED_1, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;   
		//}
		//if ( (speed<0.2) && (speed>-0.2) )   // zoom stop
		//{
		//	if (!SendLancCommand(lancDevice,  0, 0)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.2) && (speed>-0.3) ) // zoom out speed 1
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_1, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.3) && (speed>-0.4) ) // zoom out speed 2
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_2, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.4) && (speed>-0.5) ) // zoom out speed 3
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_3, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.5) && (speed>-0.6) ) // zoom out speed 4
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_4, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.6) && (speed>-0.7) ) // zoom out speed 5
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_5, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.7) && (speed>-0.8) ) // zoom out speed 6
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_6, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if ( (speed<=-0.8) && (speed>-0.9) ) // zoom out speed 7
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_7, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		//if (speed<=-0.9)                     // zoom out speed 8
		//{
		//	if (!SendContinuousLancCommand(lancDevice,  COMMANDTYPE_CAMERA, COMMAND_ZOOM_OUT_SPEED_8, NULL)) 
		//		std::cerr << "LANC Device Not Responding" << std::endl;
		//}
		#endif
    }
}

#endif //USE_ZOOM
