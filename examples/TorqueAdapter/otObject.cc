//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------


#include "otObject.h"
#include <string>

bool OtObject::initialized=false;

void OtObject::initialize(const char * config)
{
    // important parts of the system
    // get a context, the default modules and factories are
    // added allready ( because of the parameter 1 )
    // this call creates the context only, so that the configurator does not start 
    // the configuration thread with an empty context.

    ot::Context * ct = otContext;

    printf( "Context established.\n");
    otCallbackModule;
    otCallforwardModule;

    ct->parseConfiguration(std::string(config));
    otConfigurator->runConfigurationThread( config );
    
    printf( "Parsing complete.\nStarting UbiTrackClient !\n");

    //run is done by the otInterface method, by calling context.loopOnce()
    //    context.run();        
}


void OtObject::terminate()
{
  otContext->stopLoop();
}

void OtObject::runOnce( void )
{

  otContext->loopOnce();

}




void OtObject::callback(ot::CallbackNode * node,  ot::Event & event, void * data ){
  OtObject * self = reinterpret_cast<OtObject*> (data);
  self->theCallback(node, event);

};

void OtObject::registerNode(const char * nodename){
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

}

void OtObject::sendDataStrings(const char *, int argc, char ** argv){
  ot::Event event;
  event.timeStamp();
  for (int i = 0; i < argc; i++)
    {
      // get the type the name and the value and call
      
      event.setAttribute(std::string("string"), std::string("attr"), argv[i]);
    }
  otCallforwardModule ->callForward("ASCD", event);
}

ConsoleFunction(OpenTrackerInit, S32, 2, 2, "OpenTrackerInit( configurationFile )"){
OtObject::initialize(argv[2]);
};

ConsoleFunction(OpenTrackerRunOnce, S32, 1, 1, "OpenTrackerRunOnce( )"){
OtObject::runOnce();
};


ConsoleFunction(OpenTrackerTerminate, S32, 1, 1, "OpenTrackerTerminate( )"){
OtObject::terminate();
};
