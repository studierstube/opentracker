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
/** header file for SysMouseSink Node.
  *
  * @author Markus Sareika
  *
  * $Id: SysMouseSink.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section SysMouseSink SysMouseSink
 * The SysMouseSink node is a sink which forwards the events to the 
 * systems mouse.
 * the @ref SysMouseModule. 
 *
 * An example element looks like this :
 * @verbatim
<SysMouseSink>
	<AbsoluteInput>               
		<Any EventGenerator element type normalized to 65535>      
	</AbsoluteInput>
	<RelativeInput>               
		<Any EventGenerator element type>          
	</RelativeInput>
</SysMouseSink>@endverbatim
 */

#ifndef _SYSMOUSESINK_H
#define _SYSMOUSESINK_H


#include "../OpenTracker.h"

#ifdef USE_SYSMOUSE

#include <OpenTracker/input/SysMouseModule.h>


namespace ot {

/**
 * This class implements a source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Markus Sareika
 * @ingroup input
 */
class OPENTRACKER_API SysMouseSink : public Node
{

public:
    /// the event that is stored
    Event relativeEvent, absoluteEvent;
    /// flag whether event was changed since last display
    int changedAbsolute, changedRelative;
	/// offsets for various screen adaptions
	int xOffset, yOffset;
	float xFactor, yFactor;

    bool updateMouse;
         
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

	/** simple constructor, sets members to initial values */
	SysMouseSink( SysMouseModule * ptr) : Node(),
		changedAbsolute(0),
		changedRelative(1),
		xOffset(0),
		yOffset(0),
		xFactor(1),
		yFactor(1),
        updateMouse(false)
	{ 
		sysMouseModule = ptr;

#ifdef WIN32
        inputPtr = new ::INPUT;
        mouseInputPtr = new ::MOUSEINPUT;
#endif
	}

	~SysMouseSink(void)
	{}

protected:
    
    /**
     * this method notifies the object that a new event was generated.
     * It stores a copy of the received event and passes the event on
     * to its observers.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( Event& e, Node& generator)
	{
		if (generator.getName().compare("AbsoluteInput") == 0) 
		{
			//sysMouseModule->lockLoop();
			absoluteEvent = e;
			//changedAbsolute = 1;
			//sysMouseModule->unlockLoop();

            buttonInput = absoluteEvent.getButton();
            float xFloat = absoluteEvent.getPosition()[0];
            float yFloat = absoluteEvent.getPosition()[1];
            int xRaw = (int) (xFloat*65535.f*xFactor);
            int yRaw = (int) (yFloat*65535.f*yFactor);
            if( xRaw > 65635 )  xRaw = 65635;
            if( xRaw < 0 )		xRaw = 0;
            if( yRaw > 65635 )  yRaw = 65635;
            if( yRaw < 0 )		yRaw = 0;
            mouseX = xRaw+xOffset;
            mouseY = yRaw+yOffset;

            mouseFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

            //sink->changedAbsolute = 0;
            //// comment out next two for overriding the systems mouse
            //sink->changedRelative = 1;
            //resetAbs = true;
            updateMouse=true;
		}
		if (generator.getName().compare("RelativeInput") == 0) 
		{
			//sysMouseModule->lockLoop();
			relativeEvent = e;
			//changedRelative = 1;
			//sysMouseModule->unlockLoop();

            if(!resetAbs)
            {
                buttonInput = relativeEvent.getButton();
                resetAbs = false;
            }
            mouseX = (int)(relativeEvent.getPosition()[0]*10);
            mouseY = (int)(relativeEvent.getPosition()[1]*10);
#ifdef WIN32
            mouseFlags = MOUSEEVENTF_MOVE;
#endif
            //changedRelative = 0;

            updateMouse=true;
		}
        if(updateMouse)
        {
#ifdef WIN32
            updateMouse=false;
            mouseInputPtr->dx = mouseX;
            mouseInputPtr->dy = mouseY;
            mouseInputPtr->mouseData = 0;
            mouseInputPtr->dwFlags = mouseFlags;
            if( lastButtonInput != buttonInput )
            {
                if( buttonInput == 1 )
                {
                    mouseInputPtr->dwFlags = mouseInputPtr->dwFlags | MOUSEEVENTF_LEFTDOWN;
                    buttonPressed = 1;
                }
                if( buttonInput == 2 )
                {
                    mouseInputPtr->dwFlags = mouseInputPtr->dwFlags | MOUSEEVENTF_RIGHTDOWN;
                    buttonPressed = 2;
                }
                if( buttonInput == 0 )
                {
                    if( buttonPressed == 1 ) mouseInputPtr->dwFlags |=  MOUSEEVENTF_LEFTUP;
                    if( buttonPressed == 2 ) mouseInputPtr->dwFlags |=  MOUSEEVENTF_RIGHTUP;
                    buttonPressed = 0;
                }
            }
            lastButtonInput = buttonInput;
            mouseInputPtr->time = 0;
            mouseInputPtr->dwExtraInfo = 0;

            inputPtr->type = INPUT_MOUSE;
            inputPtr->mi = *mouseInputPtr;
            int send = ::SendInput( 1, inputPtr, sizeof(*inputPtr) );

#endif
        }
        updateObservers( e );
	}
	SysMouseModule * sysMouseModule;
	friend class SysMouseModule;
private:

    unsigned short buttonInput, lastButtonInput;
    int mouseX, mouseY;
    bool resetAbs;
#ifdef WIN32
    PINPUT inputPtr;
    PMOUSEINPUT mouseInputPtr;
    int buttonPressed;
    DWORD mouseFlags;
#endif

};

}  // namespace ot

#endif //USE_SYSMOUSE

#endif //_SYSMOUSESINK_H
