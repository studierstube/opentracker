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
/** header file for LaserPointerSource Node.
 *
 * @author Markus Sareika
 * 
 * $Id: LaserPointerModule.h
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <OpenTracker/tool/disable4786.h>
#include <ace/OS.h>
#include <OpenTracker/input/LaserPointerSource.h>
#include <OpenTracker/input/LaserPointerModule.h>

#ifdef USE_LASERPOINTER

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
    OT_MODULE_REGISTER_FUNC(LaserPointerModule){
        OT_MODULE_REGISTRATION_DEFAULT(LaserPointerModule, "LaserPointerConfig");
    }
    // Destructor method
    LaserPointerModule::~LaserPointerModule()
    {
        nodes.clear();
    }

    // This method is called to construct a new Node.
    Node * LaserPointerModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("LaserPointerSource") == 0 )
        {       
            LaserPointerSource * source = new LaserPointerSource;
            // read values from xml config file and initialize PTU
            if ( !attributes.get("exposure").empty() ) 
                source->exposure = atoi(attributes.get("exposure").c_str());
            if ( !attributes.get("threshold").empty() ) 
                source->threshold = atof(attributes.get("threshold").c_str());
            if ( !attributes.get("width").empty() ) 
                source->width = atoi(attributes.get("width").c_str()); 
            if ( !attributes.get("height").empty() ) 
                source->height = atoi(attributes.get("height").c_str()); 
            if(source->threshold&&source->width&&source->height&&source->exposure&&!source->initCamera)
            {
                if (!source->initializeCamera()) 
                    std::cerr << "init prosilica failed.\n" << std::endl;
                else std::cerr << "initialized prosilica "<<std::endl;
            }
            
            source->event.setConfidence( 1.0f );
            nodes.push_back( source );
            logPrintS("Built LaserPointerSource node\n");
            initialized = 1;
            return source;
        }
        // no node configured 
        return NULL;
    }

    // starts the ptu thread
    void LaserPointerModule::start()
    {
        if( isInitialized() == 1 && !nodes.empty())
        {
            ThreadModule::start();
        }
    }


    void LaserPointerModule::close()
    {
        // stop thread
        lockLoop();
        stop = true;
        unlockLoop();

        if( isInitialized() == 1 && !nodes.empty()) 
        {
            LaserPointerSource * source;
            for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
            {
                source = (LaserPointerSource *) ((Node*)*it);
            }
        }
    }


    void LaserPointerModule::run()
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


    void LaserPointerModule::pushEvent()
    {
		
    }

    void LaserPointerModule::processLoop()
    {
        if( isInitialized() == 1 )
        {
            bool newdata = false;
            LaserPointerSource *source;
            for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
            {
                source = (LaserPointerSource *) ((Node*)*it);
                source->lock();
                source->captureFrame();
                source->processFrame();
                if(source->process) newdata=true;
                source->unlock();
            }	

            if (newdata && context != NULL)
            {
                if (context->doSynchronization())
                {
                    context->dataSignal();
                    context->consumedWait();
                }
            }
        }
    }

} // namespace ot

#endif //USE_LASERPOINTER

/* 
 * ------------------------------------------------------------
 *   End of LaserPointerModule.cxx
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
