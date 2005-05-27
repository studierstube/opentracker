#include <xercesc/sax/HandlerBase.hpp>

#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "StateEngine.h"

using namespace xercesc;

#define STATE_EL "scene"
#define CONCURRENT_EL "concurrentScene"
#define TRANSITION_EL "transition"

#define NAME_ATTR "name"
#define INITIAL_ATTR "initial"
#define EVENT_ATTR "event"
#define SOURCE_ATTR "source"
#define TARGET_ATTR "target"
#define GUARD_ATTR "guard"



/** 
@ingroup april

  Helper class for StateEngine class. Handles event from the sax parser, building up the internal representation
  of a state engine parsed from an XML file.
*/
class SAXReadHandler : public HandlerBase
{

public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SAXReadHandler(StateEngine * engine);
    ~SAXReadHandler();


    // -----------------------------------------------------------------------
    //  Handlers for the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
	void startElement(const XMLCh* const name, AttributeList& attributes);
    virtual void endElement (const XMLCh *const name);
    void characters(const XMLCh* const chars, const unsigned int length);
    void ignorableWhitespace(const XMLCh* const chars, const unsigned int length);
    void resetDocument();


    // -----------------------------------------------------------------------
    //  Handlers for the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
	void warning(const SAXParseException& exception);
    void error(const SAXParseException& exception);
    void fatalError(const SAXParseException& exception);
    void resetErrors();

private:

	StateEngine * myEngine;

    string currentRegion;
    State * currentState;

};
