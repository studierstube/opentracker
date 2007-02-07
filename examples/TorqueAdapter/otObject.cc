//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _SIMBASE_H_
#include "console/simBase.h"
#endif
#include "math/mPoint.h"

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Configurator.h>
#include <OpenTracker/common/CallbackModule.h>
#include <OpenTracker/common/CallforwardModule.h>

#include "otObject.h"



#  define otConfigurator ot::Configurator::instance()
#  define otContext      ot::Configurator::instance() ->getContext()
#define otCallforwardModule dynamic_cast<ot::CallforwardModule*> ( ( otContext ).getModule("CallforwardConfig") )
#define otCallbackModule dynamic_cast<ot::CallbackModule*> ( ( otContext ).getModule("CallbackConfig") )


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


    

    //run is done by the otInterface method, by calling context.loopOnce()
    //    context.run();
    
    
};

void OtObject::terminate(){
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

void OtObject::registerNode(const char * nodename){
  otCallbackModule ->setCallback(nodename, callback, (void *)this);	
  
}

void OtObject::setScriptCallback(const char * cbname){
  scriptCallback = cbname;
};

void OtObject::theCallback(ot::CallbackNode * node, ot::Event & event){    
  // call some script defined function with whatever we got from here
  std::vector<float> & pos = event.getPosition();
  Point3F tpos;
  tpos.x = pos[0];
  tpos.y = pos[1];
  tpos.z = pos[2];
  Con::executef(2, scriptCallback.c_str(), tpos);
}

/*
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
*/
ConsoleMethod( OtObject, sendPosition, void, 4, 0, ("String NodeName, Point3F position "))
{
  ot::Event event;
	
  //Point3F pos;
	float pos[3];
  dSscanf(argv[3], "%g %g %g",
	  &pos[0],
	  &pos[1],
	  &pos[2]);
  

  event.timeStamp();
  event.setPosition(pos);
 // event.setAttribute(std::string("vector<float>"), std::string("position"), std::string(argv[3]));
  otCallforwardModule ->callForward(argv[2], event);
}

ConsoleMethod( OtObject, registerNode, void, 3, 3, "OtObject -> registerNode( nodename )"){
  object->registerNode( argv[2] );
}

ConsoleMethod( OtObject, setScriptCallback, void, 3, 3, "OtObject -> setScriptCallback( scriptfn )"){
  object->setScriptCallback( argv[2] );
}

ConsoleFunction(OpenTrackerInit, void, 2, 2, "OpenTrackerInit( configurationFile )"){
  OtObject::initialize(argv[1]);
};

ConsoleFunction(OpenTrackerRunOnce, void, 1, 1, "OpenTrackerRunOnce(  )"){
  OtObject::runOnce();
};


ConsoleFunction(OpenTrackerTerminate, void, 1, 1, "OpenTrackerTerminate(  )"){
  OtObject::terminate();
};



