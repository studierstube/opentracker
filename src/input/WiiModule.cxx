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
/** source file for LinmouseModule.
 *
 * @authors Michele Fiorentino Alexander Bornik
 * starting from cWiimote 0.2 by Kevin Forbes 
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this is a linux implementation

#include <OpenTracker/input/WiiModule.h>
#include <OpenTracker/input/WiiSource.h>
#include <OpenTracker/input/WiiSink.h>


#include <cstdio>
#include <iostream>
#include <cassert>
#include <ace/Log_Msg.h>

#define OT_NAMESPACE_BEGIN namespace ot {
#define OT_NAMESPACE_END }

OT_NAMESPACE_BEGIN

	OT_MODULE_REGISTER_FUNC(WiiModule){
			OT_MODULE_REGISTRATION_DEFAULT(WiiModule, "WiiConfig");
	}

// -------------------------------------------------------------------------------------------------------
// constructor initializing the thread manager
WiiModule::WiiModule() : ThreadModule(), NodeFactory(), stop(0)
{
   wiimote = new WiiHandler;
	if (wiimote->ConnectToDevice() &&
      wiimote->StartDataStream())
   {
   logPrintI("Wii Cannection: OK\n");
   }
   else
   {
   logPrintE("Wii Cannection Failed: OK\n");
   } 
}

// ------
// Destructor method
WiiModule::~WiiModule() {
    sources.clear();
//    sinks.clear();
    delete wiimote;
}

// This method is called to construct a new Node.
Node * WiiModule::createNode( const std::string& name, StringTable& attributes) 
{
   
    if( name.compare("WiiSource") == 0 ) {       
        stop = 0;
        if ( sources.size() > 0) {
            ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Can only have one WiiSource!\n")));
            return NULL;
        }
      
        WiiSource * source = new WiiSource(wiimote);

        source->event.getButton() = 0;
        source->event.getConfidence() = 1.0f;
        sources.push_back( source );

        logPrintI("Build WiiSource node: OK\n");

        initialized = 1; // this is still to be explained mf
        return source;
    }
    if( name.compare("WiiSink") == 0 ) {       
       stop = 0;
       if ( sinks.size() > 0) {
          ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Can only have one WiiSink!\n")));
          return NULL;
       }

       WiiSink * mysink = new WiiSink( wiimote );
       sinks.push_back( mysink );

       initialized = 1; // this is still to be explained mf
       return mysink;
    } 

    return NULL;
}
// opens connection
void WiiModule::start() 
{
    //ACE_DEBUG((LM_INFO, ACE_TEXT("LinmouseModule::start() \n")));
    if( isInitialized() && !sources.empty())
        ThreadModule::start();    
}

// closes CyberMouse library
void WiiModule::close() {
    //ACE_DEBUG((LM_INFO, ACE_TEXT("LinmouseModule::close() \n")));
    lockLoop();
    stop = 1;
    unlockLoop();
}
void WiiModule::run()
{
   ACE_DEBUG((LM_INFO, ACE_TEXT("WiiModule::run() \n")));

   while(1) {
      lockLoop();
      if (stop == TRUE) 
      {
         unlockLoop();
         break;
      }
      else
      {
         unlockLoop();
      }
      wiimote->HeartBeat(); // retrives data s from the device

      NodeVector::iterator it;
      for( it = sources.begin(); it != sources.end(); it++) 
         {
         WiiSource * source = (WiiSource*)((Node*) *it);
         source->lock();

         float wX,wY,wZ;
         float cX,cY,cZ;
         float sX,sY;
         float irX,irY;
         wX =wY=wZ=cX=cY=cZ=sX=sY=irX=irY=0.f;

         wiimote->GetCalibratedAcceleration(wX,wY,wZ);
//         printf("W:[%+1.2f %+1.2f %+1.2f] ",wX,wY,wZ);

         if (wiimote->mNunchuckAttached)
         {
            wiimote->GetCalibratedChuckAcceleration(cX,cY,cZ);
//            printf("N:[%+1.2f %+1.2f %+1.2f] ",cX,cY,cZ);

            wiimote->GetCalibratedChuckStick(sX,sY);
//            printf("S:[%+1.2f %+1.2f] ",sX,sY);
         }

         if (wiimote->mIRRunning)
         {
            if (wiimote->GetIRP1(irX,irY))
            {
               //printf("P1:[%+1.2f %+1.2f]",irX,irY);
            }
            if (wiimote->GetIRP2(irX,irY))
            {
               //printf("P2:[%+1.2f %+1.2f]",irX,irY);
            }
         }
         source->event.getPosition()[0] = irX;
         source->event.getPosition()[1] = irY;
         source->event.getPosition()[2] = 0.0f;

         float rotationQuaternion[4];
         MathUtils::eulerToQuaternion (wX, wY, wZ, rotationQuaternion);
         source->event.setOrientation(rotationQuaternion);

         // buttons
         source->event.getButton() = wiimote->GetLastButtonDataStatus();
         source->event.timeStamp();
         source->event.getConfidence() = 1.0;
         source->changed = 1;

         source->unlock();

         if (context != NULL)   {  context->dataSignal(); } 
      }
   } // end while

}
void WiiModule::pushEvent() 
{
    /// nothing to do
}

// initializes the ARTDataTracker module

void WiiModule::init(StringTable& attributes, ConfigNode * localTree)
{
    using namespace std;
    ACE_DEBUG((LM_INFO, ACE_TEXT("WiiModule::init() \n")));

    ThreadModule::init( attributes, localTree );      
   
    initialized = 1;
}

OT_NAMESPACE_END

//#endif


/* 
 * ------------------------------------------------------------
 *   End of LinMouseModule.cxx
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
