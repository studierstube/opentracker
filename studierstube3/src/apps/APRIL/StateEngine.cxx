// StateEngine.cpp: implementation of the StateEngine class.
//
//////////////////////////////////////////////////////////////////////

#include <xercesc/parsers/SAXParser.hpp>
#include "StateEngine.h"
#include "SAXReadHandler.h"

using namespace xercesc;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

State::State(string name, Region * parent){
    parentRegion = parent;
    this->name = name;
    exited = false;
}

State::~State(){
}

Region * State::addRegion(string name){
#ifdef _DEBUG
    //printf("%s.addRegion(%s)\n",this->name.c_str(), name.c_str());
#endif
    Region * r = new Region(name, this);
    regions.insert(make_pair(name,r));
    return r;
}

vector<State*> State::getCurrentStates(){
    vector<State*> v;
    appendCurrentStates(&v);
    return v;
}


void State::appendCurrentStates(vector<State*>* v){
    if (parentRegion != NULL){  // only false if we are the top level state machine
        v->push_back(this);
    }

    map<string, Region*>::const_iterator it;
    for (it = regions.begin(); it != regions.end(); it++){
        if (it->second->currentState != NULL) {
            it->second->currentState->appendCurrentStates(v);
        }
    }
}

vector<Transition*> State::getAvailableTransitions(){
    vector<Transition*> v;
    appendAvailableTransitions(&v);
    return v;
}

void State::appendAvailableTransitions(vector<Transition*>* v){
    if (parentRegion != NULL){  // only false if we are the top level state machine
        multimap<string, Transition*>::const_iterator tit;
        for (tit = transitions.begin(); tit != transitions.end(); tit++){
            if (tit->second->checkGuard()){
                v->push_back(tit->second);
            }
        }
    }

    map<string, Region*>::const_iterator it;
    for (it = regions.begin(); it != regions.end(); it++){
        if (it->second->currentState != NULL) {
            it->second->currentState->appendAvailableTransitions(v);
        }
    }
}

vector<State*> State::getExitStates(){
    vector<State*> v;
    appendExitStates(&v);
    return v;
}


void State::appendExitStates(vector<State*>* v){
    if (exited){ 
        v->push_back(this);
    }

    map<string, State*>::const_iterator it;
    for (it = substates.begin(); it != substates.end(); it++){
        it->second->appendExitStates(v);
    }
}


State * State::addSubState(
    string name,
    string region,
    bool initial,
    void (* enterCallback)(const char *),
    void (* doCallback)(const char *),
    void (* exitCallback)(const char *))
{
#ifdef DEBUG
    printf("%s.addSubState(%s)\n",this->name.c_str(), name.c_str());
#endif
    if (name.find(PATH_SEPARATOR,0) != string::npos) {    // is this addressed to a substate?
        int idx = name.find_last_of(PATH_SEPARATOR);
        State * s = getSubState(name.substr(0,idx));
        return s->addSubState(name.substr(idx+1,name.length()),region,initial,enterCallback,doCallback,exitCallback);
    }
    else {
        Region * preg;
        map<string, Region*>::iterator it = regions.find (region);
        if (it != regions.end()){
            preg = it->second;
        }
        else {
            preg = addRegion(region);
        }

        State * pstate = new State(name, preg);
        pstate->enterCallback = enterCallback;
        pstate->doCallback = doCallback;
        pstate->exitCallback = exitCallback;


        preg->states.insert(make_pair(name,pstate));
        if (initial) {
            preg->initialState = pstate;
        }

        substates.insert(make_pair(name,pstate));

        return pstate;
    }
}

void State::setCurrent(Region * scope) {
    setCurrentChild();
    if (parentRegion != NULL && parentRegion != scope)
        parentRegion->parentState->setCurrentParent(this, scope);
}

void State::setCurrentParent(State * child, Region * scope) {
    map<string, Region*>::const_iterator it;
    for (it = regions.begin(); it != regions.end(); it++){
        if (it->second->currentState != child)
            it->second->initialState->setCurrentChild();
    }
    if (parentRegion != NULL) {
        parentRegion->currentState = this;
        if (parentRegion != scope) {
            parentRegion->parentState->setCurrentParent(this, scope);
        }
    }
}

void State::setCurrentChild() {
    if (parentRegion != NULL) {
        parentRegion->currentState = this;
    }
    map<string, Region*>::const_iterator it;
    for (it = regions.begin(); it != regions.end(); it++){
        it->second->initialState->setCurrentChild();
    }
}

Transition * State::addTransition(
    State * toState,
    string event,
    string guard,
    void (*doCallback)(const char *))
{
#ifdef _DEBUG
    //printf("%s.addTransition(%s)\n",this->name.c_str(), toState->name.c_str());
#endif
    Transition * t = new Transition;
    t->from = this;
    t->to = toState;
    t->event = event;
    t->guard = guard;
    t->doCallback = doCallback;

    transitions.insert(make_pair(event,t));

    return t;
}

State * State::getSubState(const string name) {
    #ifdef _DEBUG
        //printf("%s.getSubState(%s)\n",this->name.c_str(), name.c_str());
    #endif
    unsigned int idx;
    if ((idx = name.find(PATH_SEPARATOR,0)) != string::npos) {
        return substates.find(name.substr(0,idx))->second->getSubState(name.substr(idx+1,name.length()));
    }
    else {
        map<string, State*>::iterator it = substates.find(name);
        if (it != substates.end()){
            #ifdef _DEBUG
                //printf("    state found. returning %s\n", it->second->name.c_str());
            #endif
            return it->second;
        }
        else {
            #ifdef _DEBUG
                //printf("    state not found. returning NULL!\n");
            #endif
            return NULL;
        }
    }
}

void State::event(string token){

    if (parentRegion == NULL) { // only for top-level state
        // just reset the exit flag of all substates
        map<string, State*>::const_iterator it;
        for (it = substates.begin(); it != substates.end(); it++){
            it->second->resetExitFlag();
        }
    }

    bool consumed = false;

    if ((parentRegion != NULL) && (parentRegion->currentState == this)) { // current state?
        multimap<string, Transition*>::iterator it = transitions.find (token);
        while (it != transitions.end()){
            Transition * ptrans = it->second;
            
            if (ptrans->checkGuard()){
                consumed = true;

                exit();

                if (ptrans->doCallback != NULL) ptrans->doCallback(ptrans->event.c_str());
                ptrans->to->setCurrent(ptrans->getCommonRegion());
                if (ptrans->to->enterCallback != NULL) ptrans->to->enterCallback(ptrans->to->name.c_str());


                break;
            }

            it++;
        }
    }

    // always pass event to substates

    if (!consumed) {
        map<string, Region*>::const_iterator it;
        for (it = regions.begin(); it != regions.end(); it++){
            if (it->second->currentState != NULL)
                it->second->currentState->event(token);
        }
    }
}

void State::exit() {
    if (exitCallback != NULL) exitCallback(name.c_str());
    parentRegion->currentState = NULL;
    exited = true;

    // set exit flag also for active substates
    map<string, Region*>::const_iterator it;
    for (it = regions.begin(); it != regions.end(); it++){
        if (it->second->currentState != NULL) {
            it->second->currentState->exit();
        }
    }
}

void State::resetExitFlag(){
    exited = false;
    map<string, State*>::const_iterator it;
    for (it = substates.begin(); it != substates.end(); it++){
        it->second->resetExitFlag();
    }

}


///////////////////////////////////////////////

Region::Region(string rname, State * parent){
    initialState = NULL;
    currentState = NULL;
    name = rname;
    parentState = parent;
}

///////////////////////////////////////////////


Region * Transition::getCommonRegion(){

    bool toSearched = false;

    Region * fromRegion = from->parentRegion;
    Region * toRegion = to->parentRegion;

    while (fromRegion != toRegion) {
        if (!toSearched) {
            toRegion = toRegion->parentState->parentRegion;
        }
        else {
            fromRegion = fromRegion->parentState->parentRegion;
        }
        if (toRegion == NULL) {
            toRegion = to->parentRegion;
            toSearched = true;
        }
        if (fromRegion == NULL) break;
    }

    return fromRegion;
}

bool Transition::checkGuard() {
    if (guard.compare("") == 0) return true;

    State * state = from;
    while (state->parentRegion != NULL) {
        state = state->parentRegion->parentState;
    }

    vector<State *> currentStates = state->getCurrentStates();
    for (unsigned int i=0; i<currentStates.size(); i++) {
        if (currentStates[i]->name == guard) return true;
    }

    return false;

}

StateEngine::StateEngine() : State("_engine",NULL)
{

}

StateEngine::~StateEngine()
{

}

void StateEngine::init() {
    map<string, Region*>::const_iterator it;
    for (it = regions.begin(); it != regions.end(); it++){
        it->second->initialState->setCurrent(it->second);
    }
}

void StateEngine::readFromFile(string name)
{
	SAXParser * parser = new SAXParser;
    parser->setValidationScheme(SAXParser::Val_Never);
    parser->setDoNamespaces(false);
    parser->setDoSchema(false);
    parser->setValidationSchemaFullChecking(false);

    //
    //  Create our SAX handler object and install it on the parser, as the
    //  document and error handler.
    //
    SAXReadHandler handler(this);
    parser->setDocumentHandler(&handler);
    parser->setErrorHandler(&handler);

	parser->parse(name.c_str());

	delete parser;

    init();
}
