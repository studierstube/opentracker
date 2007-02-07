#ifndef _OTOBJECT_H_
#define _OTOBJECT_H_

//#ifndef 
#include <console/simBase.h>
//#endif

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/common/CallbackModule.h>
#include <OpenTracker/common/CallforwardModule.h>

#define otConfigurator ot::Configuratorinstance()
#define otContext      ot::Configurator::instance() ->getContext()
#define otCallforwardModule dynamic_cast<ot::CallforwardModule*> (otContext->getModule("CallforwardConfig") )
#define otCallbackModule dynamic_cast<ot::CallbackModule*> (otContext->getModule("CallbackConfig") )

class OtObject{
protected:
  static  bool initialized;

public:
  virtual ~OtObject() {};
  /// opentracker initialization
  static void initialize(const char * config);
  static void runOnce();
  static void terminate();
  /// the callback that will be registered with opentracker
  static void callback( ot::CallbackNode * node,  
			ot::Event & event, void * data );  
  
  /// use this method to register a callback for a node named nodename
  void registerNode(const char * nodename);
  
  virtual void theCallback(ot::CallbackNode * node, ot::Event & event);
  virtual void sendData(const char *);
  virtual void sendDataStrings(const char *, int argc, char ** argv);
};

#endif
