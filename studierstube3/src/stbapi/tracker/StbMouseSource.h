/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** header file for StbMouseSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: StbMouseSource.h 3802 2004-09-30 14:23:46Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __STBMOUSESOURCE_H__
#define __STBMOUSESOURCE_H__

#include <OpenTracker.h>
#include <stbapi/resource/viewer/SoStudierstubeViewer.h>
#include <Inventor/events/SoMouseButtonEvent.h>

/**
 * This class implements a simple node that generates tracking events
 * based on mouse input. It is controlled by the StbKeyboardModule class
 * that uses events from the windowing system to drive tracking stations.
 * It outputs the x, y coordinates of the mosue curser in the x, y components
 * of the position field. Mouse button events are coded into the first three bits
 * of the button value (bit 0 - button 1, bit 1 - button 2, bit 3 - button 3).
 * Everything else is set to default values. The origin is
 * in the upper left corner of the window, x points to the right and y down.
 * The corresponding OpenTracker configuration element has the following attributes :
 * @li @c mode (absolute|relative) relative defines whether the position is absolute            
 *             (in pixels) or relative (0..1)
 * @li @c windows the number of the window to listen to (not implemented yet)
 *
 * An example element looks like this :
 * @verbatim
<StbMouseSource mode="relative" window="0"/>@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup tracking
 */
class STBAPI_API StbMouseSource : public Node
{
// Members
public:
    /// defining the mode the node is working in
    enum Mode { StbRelative , StbAbsolute } mode;   
    /// window number
    int window;
    /// the state that is stored
    State state;
    /// a flag to indicate whether it was changed during processing
    bool changed;

// Methods
protected:
    /** constructor method.
     * @param mode_ relative or absolute mode to use
     * @param window_ which Studierstube window to use
     */
    StbMouseSource( Mode mode_, int window_ = -1 ) :
        Node(),    
        mode( mode_ ),
        window( window_ ),
        changed( false )
    {
        state.confidence = 1;   
    }
    
    /**
     * processes the actual input event from OpenInventor
     * @param event pointer to the event
     * @param viewer pointer to the SoStudierstubeViewer generating the event.
     */
    void processEvent( const SoEvent * event, SoStudierstubeViewer * viewer )
    {
        SbVec2f pos;
        const SbVec2s & spos = event->getPosition();
        SbVec2s size = viewer->getSize();
        if( mode == StbAbsolute )
        {           
            pos.setValue( spos[0], (float)(size[1]-spos[1]));
        } else 
        {            
            pos.setValue( spos[0] / (float)size[0], (size[1]-spos[1])/(float)size[1] );         
        }
        if( state.position[0] != pos[0] ||
            state.position[1] != pos[1] )
        {
            state.position[0] = pos[0];
            state.position[1] = pos[1];
            // FIXME: event->getTime() is about 300 ms behind system time in Coin3D!
            // Therefore we set the time of the event to the current system time.
            // This is done for all implementations to guarantee identical behaviour.
            // It was also used in the rest of this method.
            // gerhard [8/26/2003]
            // state.time = event->getTime().getValue()*1000;            
            state.timeStamp();  
            changed = true;
        }
        if( event->getTypeId() == SoMouseButtonEvent::getClassTypeId())
        {
            if( SoMouseButtonEvent::isButtonPressEvent(event,SoMouseButtonEvent::BUTTON1))
            {
                state.button |= 1;
                state.timeStamp();
                changed = true;
            }
            else if( SoMouseButtonEvent::isButtonReleaseEvent(event,SoMouseButtonEvent::BUTTON1))
            {
                state.button &= 0xfffe;
                state.timeStamp();
                changed = true;
            }
            else if( SoMouseButtonEvent::isButtonPressEvent(event,SoMouseButtonEvent::BUTTON2))
            {
                state.button |= 2;
                state.timeStamp();
                changed = true;
            }
            else if( SoMouseButtonEvent::isButtonReleaseEvent(event,SoMouseButtonEvent::BUTTON2))
            {
                state.button &= 0xfffd;
                state.timeStamp();
                changed = true;
            }
            else if( SoMouseButtonEvent::isButtonPressEvent(event,SoMouseButtonEvent::BUTTON3))
            {
                state.button |= 1;
                state.timeStamp();
                changed = true;
            }
            else if( SoMouseButtonEvent::isButtonReleaseEvent(event,SoMouseButtonEvent::BUTTON3))
            {
                state.button &= 0xfffb;
                state.timeStamp();
                changed = true;
            }

#ifdef __COIN__
            // if Button 4 or 5 are pressed (usually mousewheel interaction)
            // modify the Z-Axis accordingly.
            // mousewheel rotated down - 
            else if( SoMouseButtonEvent::isButtonPressEvent(event,SoMouseButtonEvent::BUTTON4))
            {
                state.position[2] -= 0.1;
                state.timeStamp();
                changed = true;
            }
            // mousewheel rotated up
            else if( SoMouseButtonEvent::isButtonPressEvent(event,SoMouseButtonEvent::BUTTON5))
            {
                state.position[2] += 0.1;
                state.timeStamp();
                changed = true;
            }
#endif
        }
      
    }
        
public:    
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }  
    
    friend class StbInputModule;
};

#endif
