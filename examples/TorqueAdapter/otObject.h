#ifndef _OTOBJECT_H_
#define _OTOBJECT_H_


#ifndef _SIMBASE_H_
#include "console/simBase.h"
#endif


class OtObject: public SimObject{
 public:
  //  typedef std::vector<OtObject*> ObjTable;
protected:
  static  bool initialized;
  std::string scriptCallback;
  static OtObject * objTable[10];
  static int objcount;

  //  static ObjTable objTable;
public:

  /// opentracker initialization
  static void initialize(const char * config);
  static void runOnce();
  static void terminate();
  /// the callback that will be registered with opentracker
  static void callback( ot::CallbackNode * node,  ot::Event & event, void * data ) ;  
  
  /// use this method to register a callback for a node named nodename
  void registerNode(const char * nodename);
  void setScriptCallback(const char * cbname);
  
  virtual void theCallback(ot::CallbackNode * node, ot::Event & event);

  DECLARE_CONOBJECT(OtObject);
  
  
  //virtual void sendData(const char *);
  //virtual void sendDataStrings(const char *, int argc, char ** argv);
};


#endif //otobject
