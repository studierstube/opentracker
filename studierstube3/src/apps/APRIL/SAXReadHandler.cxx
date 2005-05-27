
#include "SAXReadHandler.h"


SAXReadHandler::SAXReadHandler(StateEngine * engine)
{
	myEngine = engine;
    currentState = engine;
    currentRegion = "_default";
}

SAXReadHandler::~SAXReadHandler()
{

}

void SAXReadHandler::startElement(const XMLCh* const name, AttributeList& attributes)
{
#ifdef DEBUG
    printf("starting element %s\n",XMLString::transcode(name));
#endif
	if (XMLString::equals(XMLString::transcode(name),CONCURRENT_EL)) {
		const XMLCh * rName = attributes.getValue(NAME_ATTR);
		if (rName != NULL) {
			currentRegion = (currentState->addRegion(XMLString::transcode(rName)))->name;
		}
        // todo: if no name is given, provide default unique name
    }

	if (XMLString::equals(XMLString::transcode(name),STATE_EL)) {
        // todo: handle kind="final"
		const XMLCh * stateName = attributes.getValue(NAME_ATTR);
		if (stateName != NULL) {
            bool initial = false;
            const XMLCh * sInitial = attributes.getValue(INITIAL_ATTR);
            if ((sInitial != NULL) && (XMLString::equals("true",XMLString::transcode(sInitial)))) {
                initial = true;
            }
			currentState = currentState->addSubState(XMLString::transcode(stateName),currentRegion,initial);
		}
	}

	if (XMLString::equals(XMLString::transcode(name),TRANSITION_EL)) {
		const XMLCh * trigger = attributes.getValue(EVENT_ATTR);
		const XMLCh * source = attributes.getValue(SOURCE_ATTR);
		const XMLCh * target = attributes.getValue(TARGET_ATTR);
		const XMLCh * guard = attributes.getValue(GUARD_ATTR);
        if (guard == NULL) guard = (XMLCh*)"";

        #ifdef DEBUG
            printf("  connection from %s\n",XMLString::transcode(source));
            printf("  connection to %s\n",XMLString::transcode(target));
        #endif

        State * sourceState = currentState->getSubState(XMLString::transcode(source));
        State * targetState = currentState->getSubState(XMLString::transcode(target));

		if ( (trigger != NULL) && (sourceState != NULL) && (targetState != NULL) ){
			sourceState->addTransition(targetState, XMLString::transcode(trigger),XMLString::transcode(guard),NULL);
		}
	}
}


void SAXReadHandler::endElement (const XMLCh *const name){
#ifdef DEBUG
    printf("end element %s\n",XMLString::transcode(name));
#endif
	if (XMLString::equals(XMLString::transcode(name),STATE_EL)) {
        if (currentState->parentRegion != NULL) {       // not top level state
            currentRegion = currentState->parentRegion->name;
            currentState = currentState->parentRegion->parentState;
        }
    }
	if (XMLString::equals(XMLString::transcode(name),CONCURRENT_EL)) {
        currentRegion = "_default";
    }
}

void SAXReadHandler::characters(const XMLCh* const chars, const unsigned int length)
{

}

void SAXReadHandler::ignorableWhitespace(const XMLCh* const chars, const unsigned int length)
{

}

void SAXReadHandler::resetDocument()
{

}


void SAXReadHandler::warning(const SAXParseException& exception)
{
	printf("SAXReadHandler: warning %s\n",XMLString::transcode(exception.getMessage()));

}

void SAXReadHandler::error(const SAXParseException& exception)
{
	printf("SAXReadHandler: error %s\n",XMLString::transcode(exception.getMessage()));

}

void SAXReadHandler::fatalError(const SAXParseException& exception)
{
	printf("SAXReadHandler: fatalError %s\n",XMLString::transcode(exception.getMessage()));

}

void SAXReadHandler::resetErrors()
{

}

