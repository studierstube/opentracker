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
/** header file for MobilabSource Node.
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section mobilabsource MobilabSource
 * The Mobilab Source node is a simple EventGenerator that outputs data received
 * on a particular channel of the g.Mobilab device.
 * It is managed by the @ref mobilabmodule, see there for more information on 
 * how to configure Mobilab support.
 * The MobilabSource element has the following attributes:
 * @li @c channel the channel number of the g.Mobilab device to use, 
 * between 0 and any positive number
 * @li @c type the data type to be used - possible values are 
 * unsigned short (default), float and double. Floating point types range from
 * 0 to 1.
 * An example element looks like this :
 * @verbatim
 <MobilabSource channel="0" type="double" />@endverbatim
*/

#ifndef _MOBILABSOURCE_H
#define _MOBILABSOURCE_H

#include <cassert>

#include "../OpenTracker.h"
#include "MobilabDriver.h"
#include "MobilabModule.h"

/**
 * A very simple EventGenerator node for outputing g.Mobilab channel data.
 * @author Alexander Bornik
 * @ingroup input
 */

namespace ot {

    enum datatypes {
        FLOAT_TYPE,
        DOUBLE_TYPE,
        USHORT_TYPE
    };

    class OPENTRACKER_API MobilabSource : public Node, public MobilabListener  
    {
    public:

        /// the event that is posted to the EventObservers
        Event event;
        /// the buffer event for data from the g.Mobilab device
        Event buffer;
        unsigned int channel;
        int datatype;
    
	/** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }

        virtual void newData( unsigned short sampleValue);

    protected:
	/// protected constructor so it is only accessible by the module
	MobilabSource() { channel = 0; datatype = USHORT_TYPE; };

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
        }

        void pullEvent() {};

	friend class MobilabModule;
    };

    inline void MobilabSource::newData( unsigned short sampleValue)
    {
        //logPrintI("MobilabSource(%x)::newData %hu\n",this,sampleValue);
        lock();

        buffer.timeStamp();
        
        buffer.getConfidence() = 1.0;

        switch (datatype)
        {
            case  FLOAT_TYPE :
                {
                    float fltval = sampleValue;
                    sampleValue /= 0xffff;
                    buffer.setAttribute<float>("bcidata",fltval); 
                }
                break;
            case DOUBLE_TYPE:
                {
                    double dblval = sampleValue;                    
                    dblval /= 0xffff;
                    buffer.setAttribute<double>("bcidata", dblval); 
                }
                break;
            case USHORT_TYPE:
                buffer.setAttribute<unsigned short>("bcidata", sampleValue); 
                break;
            default:                
                buffer.setAttribute<unsigned short>("bcidata", 0); 
                break;
        }

        unlock();
        
        //std::cout << "Position = " << buffer.getAttribute("bcidata")] 
        //<< std::endl;
        
    }
    
} // namespace ot

#endif // !defined(_MOBILABSOURCE_H)


/* 
 * ------------------------------------------------------------
 *   End of MobilabSource.h
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
