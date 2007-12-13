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
/** Ubitrack Client class
 *
 * @author Alexander Bornik
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/ubitrack/UbitrackClient.h>
#include <OpenTracker/core/Context.h>
#include <OpenTracker/core/ThreadContext.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
namespace ot{

    UbitrackClient * UbitrackClient::self = NULL;
    UbitrackClient::Registry UbitrackClient::initFunctions;

    UbitrackClient::UbitrackClient(int ctx_type) 
        :
        ctx(NULL)
    {
        switch (ctx_type)
        {
            case NORMAL:
                ctx = new Context(0);
                break;
            case THREAD:
                ctx = new ThreadContext(0);
                break;
            default:
                ctx = new Context(0);
                break;
        }

        this->doInitialization(*ctx);
    }


    UbitrackClient * UbitrackClient::instance(int ctx_type)
    {
	if (self == NULL)
        {
            initializeOpenTracker();            
            self = new UbitrackClient(ctx_type);
	}
	return self;
    }


    UbitrackClient::~UbitrackClient()
    {
        if (ctx) 
        {
            delete ctx;
        }
    }
    
    void UbitrackClient::doInitialization(Context & newctx){
        for (Registry::iterator i = initFunctions.begin(); 
             i != initFunctions.end(); i++)
        {
            (* (i->second).function) (&newctx, (i->second).data);
        }
    }

    void UbitrackClient::loadModule(Context & newctx, const char * module)
    {
        logPrintS("UbitrackClient loading module %s\n", module);
        Registry::iterator i = initFunctions.find(module);
        if (i != initFunctions.end()){
            (* (i->second).function) (&newctx, (i->second).data);
        } else
            logPrintE("module %s not found\n", module);
    };


    void UbitrackClient::addModuleInit(const char * name, ModuleInitFunc function, 
                                     void * data)
    {
        MIFunctor functor;
        functor.function = function;
        functor.data = data;
        //logPrintS("UbitrackClient adding %s = %p \n", name, function);
    
        initFunctions[name]=(functor);
    }
    
    
    
    void UbitrackClient::sendUTQL(std::string utql)
    {
        using namespace std;
        logPrintI("UbitrackClient::sendUTQL\n");

        cout << "UTQL string: \"" 
             << utql << "\"" << endl;            
    }

    void UbitrackClient::sendUTQLFile(std::string utqlfilename)
    {
        logPrintI("UbitrackClient::sendUTQLFile(%s)\n", utqlfilename.c_str());
        using namespace std;
        ifstream utqlfile;
        ostringstream utqls;
        char c;
        try
        {
            utqlfile.open(utqlfilename.c_str(),ifstream::in);
            while (!utqlfile.eof())
            {  
                utqlfile.get(c);
                if (!utqlfile.eof()) utqls << c;
            }
            utqlfile.close();
        }
        catch (exception &e)
        {
            cerr << "UbitrackClient::sendUTQLFile: Exception: "
                 << e.what() << endl;
        }
        sendUTQL(utqls.str());
    }

    Context * UbitrackClient::getContext(){
	return ctx;
    }


    
} //namespace ot

/* 
 * ------------------------------------------------------------
 *   End of UbitrackClient.cxx
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
