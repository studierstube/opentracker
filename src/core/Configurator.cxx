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
  	    this->doInitialization(ctx);
    }


	Configurator * Configurator::instance(){
	if (self == NULL){
		initializeOpenTracker();
		//addModuleInit("OpenTracker", initializeContext, NULL);
		self = new Configurator;
	}
	return self;
}

Configurator::~Configurator(){
	if (thread != NULL){
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

void Configurator::loadModule(Context & newctx, const char * module){
    logPrintS("Configurator loading module %s\n", module);
	Registry::iterator i = initFunctions.find(module);
	if (i != initFunctions.end()){
		(* (i->second).function) (&newctx, (i->second).data);
	}
};


void Configurator::addModuleInit(const char * name, ModuleInitFunc function, void * data){
    cerr << "adding module " << name << " to map of initFunctions" << endl;
	MIFunctor functor;
	functor.function = function;
	functor.data = data;
    //logPrintS("Configurator adding %s = %p \n", name, function);
	initFunctions[name]=(functor);
}



void Configurator::parseConfigurationString(std::string configString){
  
}

Context & Configurator::getContext(){
	return ctx;
}

void Configurator::changeConfiguration(std::string configString){


	// check whether we have a file or string
	if ( (configString.substr(configString.length() - 4)) == ".xml"){
		// its a file
		changeConfigurationFile(configString.c_str());
	} else {
		// its a string
		changeConfigurationString(configString.c_str());
	}

}

void Configurator::changeConfigurationFile(const char * file){

// check whether the runtime context is already configured
	if (ctx.isConfigured()){
		//if it is, we want to change its configuration

		//create a new context object
		Context newContext(0);
		this->loadModule(newContext, "OpenTracker");
		//this->doInitialization(newContext);

		newContext.parseConfiguration(file);

		this->ctx.copyFrom(newContext);
  	//this->changeConfiguration(configString.c_str());
	} else {
		// if it has not been configured for the first time, do it now
		ctx.parseConfiguration(file);
	}

}


// not up to date

	void Configurator::changeConfigurationString(const char* xmlstring) {
		if(ctx.isConfigured()){
			//create a new context object
			Context newContext(0);
  
			this->doInitialization(newContext);

/*			std::ofstream of("tmp.xml");
			of << xmlstring << flush;
			printf("CONFIGURATOR::CHANGING CONFIGURATION %s\n", xmlstring);
			of.close();
*/
			//newContext.parseConfiguration("tmp.xml");
			newContext.parseConfigurationString(xmlstring);
        
			this->ctx.copyFrom(newContext);
		} else {
			this->ctx.parseConfigurationString(xmlstring);
		}
    }

	void Configurator::runConfigurationThread(){
		if (thread == NULL){
			// Create reconfiguration thread
			FileConfigurationThread * fct= new FileConfigurationThread( "reconfig.xml");
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
