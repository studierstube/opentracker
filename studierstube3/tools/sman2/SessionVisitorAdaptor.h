
#ifndef _SESSIONVISITORADAPTOR_H_
#define _SESSIONVISITORADAPTOR_H_

#include <stbapi/workspace/distrManager/SessionVisitor.h>

class SessionManager;
class Host;
class SessionMessage;

/**
 * This class interfaces between a generic SessionVisitor and the host
 * instance associated with the connection retrieving the message and the
 * SessionManager itself. It calls an appropriate method on the SessionManager
 * with the additional parameter of the host (which is not contained in the 
 * message itself).
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class SessionVisitorAdaptor : public SessionVisitor  
{
public:

	struct PipeMessage { 
		SessionMessage * message;
		SessionVisitorAdaptor * adapter;
	};

	SessionVisitorAdaptor( Host * host_ );
	virtual ~SessionVisitorAdaptor();

    void execute( StartApplicationMessage & message );
    void execute( SetApplicationModeMessage & message );
    void execute( StopApplicationMessage & message );
    void execute( AddUserMessage & message );
    void execute( RemoveUserMessage & message );
    void execute( JoinLocaleMessage & message );
    void execute( LeaveLocaleMessage & message );
    void execute( SetDIVParamMessage & message );
    void execute( UserKitMessage & message );

	void put( SessionMessage * message );
	void removeHost(void);

    Host * host;
    SessionManager * manager;
};

#endif // _SESSIONVISITORADAPTOR_H_

