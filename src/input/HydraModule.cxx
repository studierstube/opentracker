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
/** source file for HydraModule module.
 *
 * @author Gatterer Clemens, Ivancsics Manuel
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <OpenTracker/input/HydraModule.h>

#ifdef USE_HYDRA
#ifdef WIN32

#pragma comment(lib,"sixense.lib")
#pragma comment(lib,"sixense_utils.lib")

#include <cstdio>
#include <string>
#include <iostream>

#include <cmath>
#include <OpenTracker/input/HydraSource.h>

namespace ot {

	
	OT_MODULE_REGISTER_FUNC(HydraModule){
	      OT_MODULE_REGISTRATION_DEFAULT(HydraModule , "HydraConfig");
	}

    HydraModule::HydraModule() : Module(), NodeFactory()
    {
        // Init sixense
		sixenseInit();
        
    }

    // Destructor method
    HydraModule::~HydraModule()
    {
		sixenseExit();
    }

    // initializes trackers
    void HydraModule::init(StringTable& attributes, ConfigNode * localTree)
    {
        if( attributes.get("base",&base ) == 0 )
            base = 0;
        logPrintI("Hydra is configured for base %d\n",base);

        Module::init( attributes, localTree );
    }

    // This method is called to construct a new Node.
	Node * HydraModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if( name.compare("HydraSource") == 0 )
        {       
            int num,controller;
			std::vector<float> vector,axis;
            num = attributes.get("controller", &controller);
            if (num == 0) controller=0;
            else if( controller < 0 || controller > 1)
            {
                logPrintE("Controller index must be between 0 and 1 \n");
                return NULL;
            }
            HydraSource *source = new HydraSource(base,controller);
            nodes.push_back( source );
            logPrintI("Built HydraSource node for controller %d \n",controller);
            initialized = 1;
            return source;
        }
        return NULL;
    }

    // opens Hydra library
    void HydraModule::start()
    {
        if( isInitialized() == 1 )
		{

			


			// Init the controller manager. This makes sure the controllers are present, assigned to left and right hands, and that
			// the hemisphere calibration is complete.
			sixenseUtils::getTheControllerManager()->setGameType( sixenseUtils::controller_manager::ONE_PLAYER_TWO_CONTROLLER );

			logPrintI("Hydra started\n");
		}
    }

    // closes Hydra library
    void HydraModule::close()
    {
	
        initialized = 0;
		logPrintI("Closing Hydra \n");
		nodes.clear();
    }

    // pushes events into the tracker tree.
    void HydraModule::pushEvent()
    {
		//logPrintI("Push Event \n");
        if( isInitialized() == 1 )
        {
			

            for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); ++ it )
            {

				
		//logPrintI("New Event \n");
                HydraSource *source = (HydraSource *)((Node*)*it);
				Event &event = source->getEvent();
				event.setConfidence(1.f);
                event.timeStamp();
                source->updateObservers( event );

				
				//logPrintI("posi %d %d %d \n", event.getPosition()[0], event.getPosition()[1], event.getPosition()[2]);
            }
		}
    }

} // namespace ot

#endif
#endif

/* 
 * ------------------------------------------------------------
 *   End of HydraModule.cxx
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
