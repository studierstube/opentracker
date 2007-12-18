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

#include <TinyXML/tinyxml.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
namespace ot{

    UbitrackClient * UbitrackClient::self = NULL;
    UbitrackClient::Registry UbitrackClient::initFunctions;

    UbitrackClient::UbitrackClient(ACE_INET_Addr nServerAddr, int ctx_type) 
        :
        ctx(NULL),
        server_addr(nServerAddr),
        requestHandler(this)

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

        // start up connection
        start();
    }


    UbitrackClient * UbitrackClient::instance(ACE_INET_Addr nServerAddr, int ctx_type)
    {
	if (self == NULL)
        {
            initializeOpenTracker();            
            self = new UbitrackClient(nServerAddr, ctx_type);
	}
        else if (self->server_addr !=nServerAddr)
        {
            delete self;
            self = new UbitrackClient(nServerAddr, ctx_type); 
        }

	return self;
    }

    UbitrackClient * UbitrackClient::instance(int ctx_type)
    {
	if (self == NULL)
        {
            initializeOpenTracker();  
            ACE_INET_Addr localhost(3000, "localhost");
            self = new UbitrackClient(localhost, ctx_type);
	}

	return self;
    }

    int
    UbitrackClient::start()
    {
        
        int ret = activate();
        
        if(ret==-1)
        {
            logPrintI("UbitrackClient: failed to activate server daemon\n");
        }
        else
        {
            logPrintI("UbitrackClient: daemon activated\n");
        }
        
        return ret;
    }

    int
    UbitrackClient::stop()
    {     
        logPrintI("UbitrackClient: stop called\n");
        
        ACE_Reactor::end_event_loop () ;
        
        wait();
        return 0;
    }

    int UbitrackClient::svc (void)
    {
        UbitrackClientRequestHandler *request_handler = &requestHandler;
    
        if (requestConnector.connect (request_handler, server_addr) == -1)
        {  
            logPrintI("UbitrackClient: could not connect to server\n");
        }

        logPrintI("UbitrackClient: starting service\n");

        ACE_Reactor::instance()->owner (ACE_OS::thr_self ());

        ACE_Reactor::run_event_loop();

        logPrintI("UbitrackClient: service finished\n");

        return 0;
    }

    int UbitrackClient::sendMessage(UbitrackMessage & inMsg)
    {
        // send message
        requestHandler.sendMessage(inMsg);
        return 0;
    }

    UbitrackClient::~UbitrackClient()
    {

        stop();

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
        
        UbitrackMessage um;
        um.setXMLString(utql);
        sendMessage(um);
        //cout << "UTQL string: \"" 
        //     << utql << "\"" << endl;            
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

    bool UbitrackClient::handleMessage(UbitrackMessage &retMsg)
    {
        using namespace std;
        logPrintI("UbitrackClient::handleMessage()\n");

        TiXmlDocument document;

        if  (!document.Parse(retMsg.XMLString().c_str()))
        {
            logPrintE("UbitrackClient::handleMessage:\n Parsing Error:\n Message: %s\n", document.ErrorDesc());
            return false;
        }
        else
        {
            TiXmlElement* root = document.RootElement();  
            const string patternString("Pattern");
            const string inputString("Input");
            const string outputString("Output");
            const string dfgString("DataflowConfiguration");
            const string otNodeString("UbitrackLib");
            const string edgeString("Edge");
            const string attrString("Attribute");
            //const string otNodeString("OpenTracker");
            
            TiXmlElement* pattern = 
                root->FirstChildElement(patternString.c_str());
            while (pattern)
            {
                // read pattern's attributes

                auto_ptr<StringTable> attributes(new StringTable());
                TiXmlAttribute* attribute = pattern->FirstAttribute();
                while (attribute)
                {
                    if (string(attribute->Name()) == "id")
                    {
                        cerr << attribute->Value() << endl;
                        attributes->put(attribute->Name(), attribute->Value());
                    }
                    attribute = attribute->Next();
                }

                // obtain input child
                TiXmlElement *cinput = 
                    pattern->FirstChildElement(inputString.c_str());
                if (cinput)
                {
                    cerr << " input:" << cinput->Value() << endl;
                    // search for references
                    TiXmlElement *inode = cinput->FirstChildElement();
                    while (inode)
                    {
                        TiXmlAttribute *inattr = inode->FirstAttribute();
                        while (inattr)
                        {
                            if (string(inattr->Name()) == "pattern-ref")
                            {
                                cerr << "    " 
                                     << inattr->Name() 
                                     << " -> " << inattr->Value() << endl;
                            }
                            inattr = inattr->Next();
                        }

                        inode = inode->NextSiblingElement();
                    }
                }
                
                // obtain output child
                TiXmlElement *coutput = 
                    pattern->FirstChildElement(outputString.c_str());
                if (coutput)
                {
                    cerr << " output:" << coutput->Value() << endl;
                    // search for the edge 
                    TiXmlElement *oedge = 
                        coutput->FirstChildElement(edgeString.c_str());
                    if (!oedge)
                    {
                         // No edge specification -> strange dataflow
                         logPrintW("No edge in output - strange dataflow!\n");
                    }
                    else
                    {
                        // find OT relevant attributes
                        TiXmlElement * outattrel = oedge->FirstChildElement(attrString.c_str());
                        while (outattrel)
                        {
                            const char *oattrname = outattrel->Attribute("name");
                            const char *oattrval = outattrel->Attribute("value");
                            if (oattrname && oattrval)
                            {
                                string oattrns(oattrname);
                                if (oattrns.find_first_of("OT") == 0)
                                {
                                    oattrns = oattrns.erase(0,2);
                                    attributes->put(oattrns, oattrval);
                                    //cerr << "    " << oattrname 
                                    //     << " -> " << oattrval << endl;
                                }
                                
                            }
                            else
                            {
                                logPrintW("Attribute element is missing name and  value attributes!\n");
                            }

                            outattrel = outattrel->NextSiblingElement(attrString.c_str());
                        }
                        oedge = oedge->NextSiblingElement(edgeString.c_str());
                        if (oedge)
                        {
                            logPrintW("found multiple edge output for one pattern!\n");
                        }
                    }
                }

                // obtain dataflow configuration child
                TiXmlElement *cdfg = 
                    pattern->FirstChildElement(dfgString.c_str());
                if (cdfg)
                {
                    cerr << " dfg:" << cdfg->Value() << endl;

                    // search for OpenTracker configuration stuff

                    TiXmlElement *dfgnode = 
                        cdfg->FirstChildElement(otNodeString.c_str());
                     if (!dfgnode)
                     {
                         // No OT configuration -> DFG will be incomplete!
                         logPrintW("No OpenTracker configuration for pattern available!\n");
                     }
                     else
                     {
                         
                         TiXmlAttribute *dfgattr = dfgnode->FirstAttribute();
                         while (dfgattr)
                         {
                             if (string(dfgattr->Name()) == "class")
                             {
                                 cerr << "    " 
                                      << dfgattr->Name() 
                                      << " -> " << dfgattr->Value() << endl;
                                 KeyIterator it(*attributes);
                                 while (it.hasMoreKeys())
                                 {
                                     string akey(it.nextElement());
                                     cerr << "      "
                                          << akey << " -> " 
                                          << attributes->get(akey) << endl;
                                 }
                             }
                             dfgattr = dfgattr->Next();
                         }
                                                  
                         dfgnode = dfgnode->NextSiblingElement(otNodeString.c_str());
                         // Each Ubitrack pattern should have a unique node 
                         // representation in OpenTracker!
                         if (dfgnode)
                         {
                             logPrintW("Multiple OpenTracker representations for pattern found! Ignoring all but the first.\n");
                         }
                     }
                }

                cerr << endl;
                pattern = pattern->NextSiblingElement(patternString.c_str());
            }
        }
        return true;
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
