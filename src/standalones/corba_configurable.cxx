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
/** source file containing the main function for standalone use.
 *
 * @author Joseph Newman
 *
 * $Id: corba_configurable.cxx 1164 2006-06-14 05:27:23Z jfn $
 * @file                                                                   */
/* ======================================================================= */

#include <ace/Thread.h>
#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/network/CORBAUtils.h>
#include <OpenTracker/skeletons/uts.hh>

#include <iostream>

using namespace std;
using namespace ot;

class UbitrackClient_i : public POA_UTS::UbitrackClient,
			 public PortableServer::RefCountServantBase
{
protected:
  Configurator * config;

public:
    //  UbitrackClient_i(Context * ctx) {
  UbitrackClient_i() {
    // UbitrackClient_i constructor
    //config = new Configurator(1, ctx);
      //config = new Configurator();
      config = Configurator::instance(); //->getContext();
  };
  virtual ~UbitrackClient_i() {
    // UbitrackClient_i destructor
    delete config;
  };
  virtual void updateConfiguration(const char* xmlstring) {
    // method responsible for updating the OT configuration
    cerr << "xmlstring = " << xmlstring << endl;
    config->changeConfiguration(xmlstring);
  };
  virtual char* getConfiguration() {return NULL;};

    Configurator * getConfigurator() {return config;};
};



/**
 * The main function for the standalone program. It expects a
 * filename as argument, tries to parse the configuration file
 * and starts the main loop, if successful
 */
int main(int argc, char **argv)
{
    if( argc < 2 || argc > 3)
    {
        cout << "Usage : " << argv[0] << " NamingContextName <rate>" << endl;
        return 1;
    }
    try {
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);


      // important parts of the system get a context
      //Context context( 0 );
      
      cout << "Context established." << endl;
      
      //UbitrackClient_i* configurator_impl = new UbitrackClient_i(&context);
      UbitrackClient_i* configurator_impl = new UbitrackClient_i();
      PortableServer::ObjectId_var configurator_id = poa->activate_object(configurator_impl);

      // Obtain a reference to the object, and register it in
      // the naming service.
      cout << "Obtaining reference" << endl;
      obj = configurator_impl->_this();
      cout << "Binding reference to name" << endl;
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup(argv[1]);
      CORBAUtils::bindObjectReferenceToName(orb, obj, string_name);
      cout << "Bound reference to name" << endl;
      PortableServer::POAManager_var pman = poa->the_POAManager();
      cout << "activating POA Manager" << endl;
      pman->activate();
      Context & context = Configurator::instance() ->getContext();
      //delete configurator;      
      //orb->run(); 
      if (argc == 2) {
          context.run();
      } else if (argc == 3) {
          int rate;
          sscanf(argv[2], "%d", rate);
          context.runAtRate(rate);
      }
      poa->deactivate_object(configurator_id);
    }
    catch(CORBA::SystemException&) {
      cerr << "Caught CORBA::SystemException." << endl;
    }
    catch(CORBA::Exception&) {
      cerr << "Caught CORBA::Exception." << endl;
    }
    catch(omniORB::fatalException& fe) {
      cerr << "Caught omniORB::fatalException:" << endl;
      cerr << "  file: " << fe.file() << endl;
      cerr << "  line: " << fe.line() << endl;
      cerr << "  mesg: " << fe.errmsg() << endl;
    }
    catch(...) {
      cerr << "Caught unknown exception." << endl;
    }
    return 1;
}

/* 
 * ------------------------------------------------------------
 *   End of configurable.cxx
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
