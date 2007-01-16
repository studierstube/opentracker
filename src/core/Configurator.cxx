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
/** Configurator class
 *
 * @author Eduardo Veas
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/misc/FileConfigurationThread.h>
#include <iostream>
#include <fstream>
using namespace std;
namespace ot{

    Configurator * Configurator::self = NULL;
    Configurator::Registry Configurator::initFunctions;

    Configurator::Configurator():ctx(0), thread(NULL)
    {
        this->loadModule(ctx, "OpenTracker");
        //this->doInitialization(ctx);
    }


    Configurator * Configurator::instance()
    {
	if (self == NULL){
            initializeOpenTracker();
            //addModuleInit("OpenTracker", initializeContext, NULL);
            self = new Configurator;
	}
	return self;
    }

    Configurator::~Configurator()
    {
        if (thread != NULL)
        {
            FileConfigurationThread * ct = dynamic_cast<FileConfigurationThread *>(thread);
            ct->finish();
            OSUtils::sleep(1000);
            delete ct;
	}
    }
    
    void Configurator::doInitialization(Context & newctx){
        for (Registry::iterator i = initFunctions.begin(); i != initFunctions.end(); i++)
        {
            (* (i->second).function) (&newctx, (i->second).data);
        }
    }

    void Configurator::loadModule(Context & newctx, const char * module)
    {
        //logPrintS("Configurator loading module %s\n", module);
        Registry::iterator i = initFunctions.find(module);
        if (i != initFunctions.end()){
            (* (i->second).function) (&newctx, (i->second).data);
        }
    };


    void Configurator::addModuleInit(const char * name, ModuleInitFunc function, 
                                     void * data)
    {
        MIFunctor functor;
        functor.function = function;
        functor.data = data;
        //logPrintS("Configurator adding %s = %p \n", name, function);
    
        initFunctions[name]=(functor);
    }
    
    
    
    void Configurator::parseConfigurationString(std::string configString)
    {
  
    }

    Context & Configurator::getContext(){
	return ctx;
    }

    void Configurator::changeConfiguration(std::string configString)
    {
        // check whether we have a file or string
        if ( (configString.substr(configString.length() - 4)) == ".xml"){
            // its a file
            changeConfigurationFile(configString.c_str());
        } else {
            // its a string
            changeConfigurationString(configString.c_str());
        }   
    }

    void Configurator::changeConfigurationFile(const char * file)
    {
        // check whether the runtime context is already configured
        if (ctx.isConfigured())
        {
            //if it is, we want to change its configuration
        
            //create a new context object
            Context newContext(0);
            //this->loadModule(newContext, "OpenTracker");
            this->doInitialization(newContext);
        
            newContext.parseConfiguration(file);
        
            this->ctx.copyFrom(newContext);
        } 
        else 
        {
            // if it has not been configured for the first time, do it now
            ctx.parseConfiguration(file);
        }
    
    }


    // not up to date

    void Configurator::changeConfigurationString(const char* xmlstring) 
    {
        if(ctx.isConfigured()){
            //create a new context object
            Context newContext(0);
            
            this->doInitialization(newContext);            
            /*			std::ofstream of("tmp.xml");
                                of << xmlstring << flush;
                                printf("CONFIGURATOR::CHANGING CONFIGURATION %s\n", xmlstring);
                                of.close();
            */
            newContext.parseConfigurationString(xmlstring);
            
            this->ctx.copyFrom(newContext);
        } else {
            this->ctx.parseConfigurationString(xmlstring);
        }
    }

    void Configurator::runConfigurationThread(const char * filename)
    {
        if (thread == NULL){
            // Create reconfiguration thread
            FileConfigurationThread * fct= new FileConfigurationThread( filename );
            // start it up
            fct->start();
            
            thread = fct;
        }
    }
    
} //namespace ot
/* 
 * ------------------------------------------------------------
 *   End of Configurator.cxx
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
