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
/** source file for MobilabModule
 *
 * @author Alexander Bornik
 * 
 * $Id$
 *
 * @file                                                                   */
/* ======================================================================= */

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Thread.h>
#include <ace/FILE_Connector.h>
#include <ace/FILE_IO.h>

#include <OpenTracker/input/MobilabModule.h>
#include <OpenTracker/input/MobilabSource.h>

#include <iostream>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

namespace ot {

    OT_MODULE_REGISTER_FUNC(MobilabModule)
    {
        OT_MODULE_REGISTRATION_DEFAULT(MobilabModule, "MobilabConfig");
    }

    MobilabModule::MobilabModule() :
        debug (true),
        driver( NULL ),
        logFile( NULL )
    {
        mobilab_reactor = (void *)new ACE_Reactor;
    }

    MobilabModule::~MobilabModule()
    {
        // delete sources
        SourceVector::iterator it;
        for (it = sources.begin(); it!=sources.end(); it++)
        {
            if ((*it)) delete (*it);
        }

        sources.clear();

        // delete log file
        if( logFile != NULL )
            delete logFile;

        // delete reactor
        if ( mobilab_reactor != NULL)
            delete (ACE_Reactor *)mobilab_reactor;
    }

    void MobilabModule::init(StringTable& attributes,  ConfigNode * localTree)
    {
	device = attributes.get("dev");

        if( attributes.containsKey("logfile") )
        {
            logFile = new ACE_FILE_IO;
            ACE_FILE_Connector connector; 
            if( connector.connect( *logFile, ACE_FILE_Addr(ACE_TEXT_CHAR_TO_TCHAR(attributes.get("logfile").c_str())), 0, ACE_Addr::sap_any, 0, O_WRONLY|O_CREAT|O_APPEND))
            {
                delete logFile;
                logFile = NULL;
                logPrintE("MobilabModule error opening log file %s\n", 
                          attributes.get("logfile").c_str());
            }
        }
        if (attributes.containsKey("debug"))
        {
            if (attributes.get("debug") == "on")
            {
                debug = true;
            }
            else
            {
                debug = false;
            }
        }
        
        Module::init( attributes, localTree );

        if (debug)
        {
            logPrintI("MobilabModule initialized for port %s.\n", device.c_str());
        }
    }

    Node * MobilabModule::createNode( const std::string & name, StringTable & attributes )
    {
	if( name.compare("MobilabSource") == 0 )
	{
            int channelnum;
            int num = sscanf(attributes.get("channel").c_str(), 
                             " %i", &channelnum);
            if (num == 0) 
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting MobilabSource channel!\n")));
                return NULL; 
            }
            
            if( !isInitialized() )
            {
                ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:MobilabModule is not initialized, cannot build MobilabSource !\n")));
                return NULL;
            }
            
            
            int datat = USHORT_TYPE;

            if (attributes.containsKey("datatype"))
            {
                if (attributes.get("datatype") == "USHORT")
                {
                    datat = USHORT_TYPE;
                }
                else if (attributes.get("datatype") == "FLOAT")
                {
                    datat = FLOAT_TYPE;
                }
                else if (attributes.get("datatype") == "DOUBLE")
                {
                    datat = DOUBLE_TYPE;
                }

            }
            
            std::string attname("bcidata");

            if (attributes.containsKey("attname"))
            {
                attname = attributes.get("attname");
            }

            MobilabSource *source = new MobilabSource;
            source->channel = channelnum;
            source->datatype = datat;
            source->attname = attname;
            sources.push_back(source);
            if (debug)
            {
                logPrintI("Built MobilabSource node.\n");
            }
            return source;
	}
	return NULL;	
    }

    void MobilabModule::pushEvent()
    {
        
    }

    void MobilabModule::start()
    {
	if( isInitialized() )
	{
            ThreadModule::start();
	}
    }

    void MobilabModule::close()
    {
        if (debug)
        {
            logPrintI("MobilabModule::close()\n");
        }

	if( driver != NULL )
	{
            if( driver->getReactor() != NULL )
            {
                driver->getReactor()->end_reactor_event_loop();
            }
            ThreadModule::close();
	}
        if( logFile != NULL )
            logFile->close();
    }

    void MobilabModule::run()
    {
	driver = new MobilabDriver( (ACE_Reactor *)mobilab_reactor , this );
	driver->setDebug( debug ); // only for debug purposes ...
        
        SourceVector::iterator it;
        for (it = sources.begin(); it != sources.end(); it++)
        {
            if( (*it) != NULL )
            {
                driver->addListener( (*it), this );
            }
            else
            {
                logPrintW("ot::MobilabModule source vector contains NULL element!\n");
            }
        }    

        if( logFile != NULL )
        {
            driver->addListener( this );
        }

        
	driver->getReactor()->owner(ACE_Thread::self());
        /*
        ACE_Sig_Adapter sa( );
        if (driver->getReactor()->register_handler(SIGINT, &sa) == -1)
        {
            logPrintW("MobilabModule: could not register SIGINT handler!\n");
        }
        */

        /// start device communication
	if( driver->open( device ) )
	{
            logPrintE("MobilabModule could not start MobilabDriver !\n");
            return;
	}

        if( debug )          
        {
            logPrintI("MobilabModule::run started MobilabDriver !\n");
	}

	driver->getReactor()->run_reactor_event_loop();

        if (debug)
        {
            logPrintI("MobilabModule::run reactor event loop exited ...\n");
        }

	driver->close();
	delete driver;
    }

    void MobilabModule::newData(unsigned short sampleValue)
    {
        logFile->send( sampleValue, sizeof(sampleValue));
    }


} // namespace ot


/* 
 * ------------------------------------------------------------
 *   End of MobilabModule.cxx
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
