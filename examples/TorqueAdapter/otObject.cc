//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _OTOBJECT_H_
#define _OTOBJECT_H_

class OtObject{
protected:
  static  bool initialized;
#ifdef USE_LIVECONTEXT
  static ot::Context * runtime;
#endif //USE_LIVECONTEXT  
public:
  /// opentracker initialization
  static void initialize(const char * config);
  static void runOnce();
  static void terminate();
  /// the callback that will be registered with opentracker
  static void callback( ot::CallbackNode * node,  ot::Event & event, void * data ) ;  
  
  /// use this method to register a callback for a node named nodename
  void register(const char * nodename);

  
  virtual void theCallback(ot::CallbackNode * node, ot::Event & event);
  virtual void sendData(const char *);
  virtual void sendDataStrings(const char *, int argc, char ** argv);
};

bool OtObject::initialized=false;
#ifdef USE_LIVECONTEXT
  ot::Context *  OtObject::runtime=NULL;
#endif //USE_LIVECONTEXT  


void OtObject::initialize(const char * config){

  

    // important parts of the system
    // get a context, the default modules and factories are
    // added allready ( because of the parameter 1 )
    // this call creates the context only, so that the configurator does not start 
    // the configuration thread with an empty context.
#ifdef USE_LIVECONTEXT
	runtime = ot::getLiveContext(config);
#endif //USE_LIVECONTEXT
    ot::Context & ct = otContext;
    printf( "Context established.\n");
    otCallbackModule;
    otCallforwardModule;



#ifndef USE_LIVECONTEXT
    ct .parseConfiguration(std::string(config));
    otConfigurator ->runConfigurationThread( config );
#endif //USE_LIVECONTEXT

	printf( "Parsing complete.\nStarting UbiTrackClient !\n");

    otUbiTrackClient->initialize();
    

    //run is done by the otInterface method, by calling context.loopOnce()
    //    context.run();
    
    
};

};

void OtObject::terminate(const char * config){
#ifdef USE_LIVECONTEXT
  delete runtime;
#endif //USE_LIVECONTEXT  
};

void OtObject::runOnce( void )
{

  otContext .loopOnce();

}




void OtObject::callback(ot::CallbackNode * node,  ot::Event & event, void * data ){
  OtObject * self = reinterpret_cast<OtObject*> (data);
  self->theCallback(node, event);

};

void OtObject::register(const char * nodename){
  otCallbackModule ->setCallback(nodename, callback, (void *)this);	
  
}

void OtObject::theCallback(ot::CallbackNode * node, ot::Event & event){    
  // call some script defined function with whatever we got from here
}

void OtObject::sendData(const char * name){
  ot::Event event;
  event.timeStamp();
  event.setAttribute("obj", std::string(name));
  event.setAttribute("val", std::string(" not implemented, sending default message"));
  otCallforwardModule ->callForward(name, event);

};

void OtObject::sendDataStrings(const char *, int argc, char ** argv){
  ot::Event event;
  event.timeStamp();
  for (int i = 0; i < argc; i++){
    // get the type the name and the value and call
    event.setAttribute(type, name, value);
  }
  otCallforwardModule ->callForward(name, event);
};
#endif  // _H_OTOBJECT_
