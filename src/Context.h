// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Context.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'Context'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 01, 2000 13:18 Gerhard Reitmayr & implemented Context
//     Added method 'DestructorInclude'
//     Added method 'ConstructorInclude'
//     Added method 'run'
//     Added method 'parseConfiguration'
//     Added method 'addModule'
//     Added method 'addFactory'
//     Added method '~Context'
//     Added method 'Context'
//     Added member 'parser'
//     Added member 'factory'
//     Added member 'rootNodes'
//     Added member 'modules'
// ===========================================================================
#ifndef _CONTEXT_H
#define _CONTEXT_H


//@START_USER1
//@END_USER1


/*@NOTE_4746
This class represents one context. It keeps its own modules and tracker tree. Furthermore
it implements the main loop driving the tracker.
*/
class Context
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    ModuleVector modules;
    NodeVector* rootNodes;
    NodeFactoryContainer factory;
    ConfigurationParser* parser;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    Context();
    virtual ~Context();
    void addFactory(NodeFactory& factory);
    void addModule(const char* name, Module& module);
    void parseConfiguration(const char* filename);
    void run();
};

#endif


#ifdef CB_INLINES
#ifndef _CONTEXT_H_INLINES
#define _CONTEXT_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
