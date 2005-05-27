// StateEngine.h: interface for the StateEngine class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#if !defined(STATEENGINE_H)
#define STATEENGINE_H

#include <string>
#include <vector>
#include <map>

#define PATH_SEPARATOR ":"

using namespace std;

class Transition;
struct Region;

/** 
@ingroup april

  A State.
*/
class State
{

public:

    string name;
    Region * parentRegion;

    void (* enterCallback)(const char *);
    void (* doCallback)(const char *);
    void (* exitCallback)(const char *);

    State(string name, Region * parent);
    ~State();


    Region * addRegion(string name);

    State * addSubState(
        string name,
        string region,
        bool initial,
        void (* enterCallback)(const char *) = NULL, 
        void (* doCallback)(const char *) = NULL, 
        void (* exitCallback)(const char *) = NULL
    );

    Transition * addTransition(
        State * toState,
        string event,
        string guard,
        void (*doCallback)(const char *) = NULL 
    );



    State * getSubState(const string name);

    void setCurrent(Region * scope);

    void event(string token);
    vector<State*> getCurrentStates();
    vector<State*> getExitStates();
    vector<Transition*> getAvailableTransitions();

protected:

    multimap<string,Transition*> transitions;
    map<string,Region*> regions;
    map<string,State*> substates;

    void exit();
    bool exited;
    void resetExitFlag();

    void appendCurrentStates(vector<State*>* v);
    void appendExitStates(vector<State*>* v);
    void appendAvailableTransitions(vector<Transition*>* v);

    void setCurrentParent(State * child, Region * scope);
    void setCurrentChild();

};

/** 
@ingroup april

One of possibly multiple concurrent regions of a composite state.
*/
struct Region
{
    Region(string name, State * parent);
    ~Region();

    string name;
    map<string,State*> states;

    State * parentState;
    State * initialState;
    State * currentState;
};

/** 
@ingroup april

A Transition element.
*/
class Transition
{
public:
    string event;
    string guard;

    State * from;
    State * to;

    void (* doCallback)(const char *);

    Region * getCommonRegion();
    bool checkGuard();
};


/** 
@ingroup april

Generic state engine implementation. This is an implementation of a generic, hirarchical, concurrent
state engine as can be defined in UML. The state engines specification is read from an XML file, which
can contain the following tags (constants defined in SAXReadHandler.h):

@li\<state name="" initial="true|false"> for defining a state
@li\<compositeState name=""> for defining a state that has substates
@li\<concurrentSubState name=""> for defining concurrent areas within a substate
@li\<transition event="" source="" target="" guard=""> to define a transition

for more details, see the APRIL schema document at http://www.studierstube.org/april

*/
class StateEngine : public State 
{
public:
	StateEngine();
	virtual ~StateEngine();
    
	void readFromFile(string name);

    void init();

protected:

};

#endif // !defined(STATEENGINE_H)
