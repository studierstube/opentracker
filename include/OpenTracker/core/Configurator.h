#ifndef CONFIGURATOR_HH
#define CONFIGURATOR_HH
#include <string>
#include "../dllinclude.h"

namespace ot {

  class Context;
  class ConfigurationThread;

  class OPENTRACKER_API Configurator {
  public:
	  typedef void (*ModuleInitFunc) (Context *, void * data);
	 
  protected:
	  struct MIFunctor{
		ModuleInitFunc function;
		void * data;
		//MIFunctor(ModuleInitFunc f, void * d):function(f), data(d){};
	  };
  public:
	  typedef std::map<std::string, MIFunctor> Registry;
  protected:
	static Registry initFunctions;
	static Configurator * self;
    Context ctx;
	bool isDefaultContext;

	ConfigurationThread * thread;

	static void doInitialization(Context & newctx);
	
	Configurator();
  public:
	

	static Configurator * instance();
	virtual ~Configurator();

	Context & getContext();

	
	void parseConfigurationString(std::string configString);
	void changeConfiguration(std::string configString);
	void changeConfigurationFile(const char* xmlstring);
	void changeConfigurationString(const char* xmlstring);

	void runConfigurationThread();

	static void addModuleInit(const char * name, ModuleInitFunc func, void * data);
	static void loadModule(Context & newctx, const char * module);
	friend void initializeContext(Context *, void *);
  };

}


#endif //CONFIGURATOR_HH



/* 
 * ------------------------------------------------------------
 *   End of Configurator.h
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
