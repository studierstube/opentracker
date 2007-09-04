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
/** source file for WiiSink node.
 *
 * @authors Michele Fiorentino and Alexander Bornik
 * starting from cWiimote 0.2 by Kevin Forbes 
 *
 * $Id: WiiSink.cxx 1846 2007-05-23 08:16:47Z jfn $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/input/WiiSink.h>


#include <cassert>


namespace ot {


    WiiSink::WiiSink( WiiHandler* wiimote_):
        Node(), 
        name( ""),
        station( 0 ),
        type( TRACKER ),
        wiimote( wiimote_ )
    {
        if ( wiimote == NULL) 
        {
            logPrintE("Wii Sink has null pointer\n");
            exit (1);
        }
        // Initialization
        thread = NULL;
        wiimote->SetLEDs(0,0,0,0);
        wiimote->SetVibration(false);
    }

    WiiSink::~WiiSink()
    {

        if (thread != NULL) 
        {
            ACE_Thread::kill(*thread,0);
            delete thread;
        }
   
        if( wiimote != NULL )
        {
            wiimote = NULL;
        }        
    }



    void WiiSink::onEventGenerated( Event& event, Node& generator)
    {
        int val = 0;
        val = event.getAttribute((int*)NULL,"Led");
        if (val != 0)
        { 
            wiimote->SetLEDs(val &0x0002,val &0x0004,val &0x0008,val &0x0010);
        }
        else wiimote->SetLEDs(0,0,0,0);

        val = event.getAttribute((int*)NULL,"Vibro");
        if (val != 0)
        { 
            duration = val;
            frequency= 1000;
       
            if (thread != NULL) ACE_Thread::kill(*thread,0);

            thread= new ACE_thread_t;
            ACE_Thread::spawn((ACE_THR_FUNC)thread_func, 
                              this, 
                              THR_NEW_LWP | THR_JOINABLE,
                              thread );
      
        }
        else wiimote->SetLEDs(0,0,0,0);

        updateObservers( event );
    }



    void WiiSink::runVibro()
    {
        // Modulates vibro
        for (int i = 0 ; i< 5; i++ ) 
        {
            wiimote->SetVibration(true);
            OSUtils::sleep (20);
            logPrintI("endsleep\n");
            wiimote->SetVibration(false);
            OSUtils::sleep (20);
        }

        ACE_Thread::kill ( *thread, 0);
        logPrintI("KILL thread\n");
        delete thread;
        thread = NULL;
    }


}



/* 
 * ------------------------------------------------------------
 *   End of WiiSink.cxx
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
