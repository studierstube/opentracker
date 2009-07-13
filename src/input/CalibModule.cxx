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
/** header file for CalibModule Node.
 *
 * @author Markus Tatzgern
 * 
 * $Id: CalibModule.h
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <OpenTracker/tool/disable4786.h>
#include <ace/OS.h>
#include <OpenTracker/input/CalibSinkSource.h>
#include <OpenTracker/input/CalibModule.h>

#ifdef USE_CALIBMODULE

#include <cstdio>
#if defined (WIN32) || defined (GCC3)
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream>
#endif

#include <Windows.h>
#include <math.h>

#include <opentracker\tool\OT_ACE_Log.h>


namespace ot {
	const float CalibModule::SAMPLING_FREQ_DEFAULT = 25;


	OT_MODULE_REGISTER_FUNC(CalibModule) {
        OT_MODULE_REGISTRATION_DEFAULT(CalibModule, "CalibConfig");
    }
    // Destructor method
    CalibModule::~CalibModule()
    {
        nodes.clear();
    }

    // This method is called to construct a new Node.
    Node * CalibModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("CalibSinkSource") == 0 )
        {       
            CalibSinkSource * source = new CalibSinkSource;
            
            // read values from xml config file 
            if ( !attributes.get("samplingFreq").empty() )
                samplingIntervall = 1.0 / (float) atoi(attributes.get("samplingFreq").c_str()) * 1.0E6;
            else
                samplingIntervall = 1.0 / SAMPLING_FREQ_DEFAULT * 1.0E6;
                
            source->event.setConfidence( 1.0f );
            nodes.push_back( source );
            logPrintS("Built CalibSinkSource node\n");
            initialized = 1;
            return source;
        }
        if( (name.compare("CalibTrigger") == 0) ||
            (name.compare("Position") == 0) || (name.compare("Orientation") == 0) ||
            (name.compare("CameraOffset") == 0) || (name.compare("CoordFrameOffset") == 0) ) 
        {
            // create just a pass-through node
            NodePort *np = new NodePort();
            np->name = name;
            return np;
        }

        // no node configured 
        return NULL;
    }

    // starts the ptu thread
    void CalibModule::start()
    {
        if( isInitialized() == 1 && !nodes.empty())
        {
            ThreadModule::start();
        }
    }

    // stops the ptu thread and closes all COM port streams
    void CalibModule::close()
    {
        // stop thread
        lockLoop();
        stop = true;
        unlockLoop();
    }


    void CalibModule::run()
    {
        static int init = 0;

        if( init == 0 )
        {
            initialized = 1;
            init = 1;
        }
        while(1)
        {
            lockLoop();
            if (stop == true) 
            {
                unlockLoop();
                break;
            }
            else
            {
                unlockLoop();
            }

            processLoop();
        }

        lockLoop();
        stop = 0;
        unlockLoop();
    }


    void CalibModule::pushEvent()
    {
		
    }

    void CalibModule::processLoop()
    {
        if( isInitialized() == 1 )
        {
            if (context != NULL)
            {
                if (context->doSynchronization())
                {
                    context->dataSignal();
                    context->consumedWait();
                }
            }
        }
        ACE_OS::sleep(ACE_Time_Value(0, samplingIntervall));
    }

} // namespace ot

#endif // USE_CALIBMODULE

/* 
 * ------------------------------------------------------------
 *   End of PosOrientCalibModule.cxx
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
