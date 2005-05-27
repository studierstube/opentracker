#ifdef WIN32
#pragma warning(disable:4284)
#endif

#include "SessionVisitorAdaptor.h"
#include "SessionManager.h"

SessionVisitorAdaptor::SessionVisitorAdaptor(Host * host_)
: host( host_ )
, manager( &SessionManager::getInstance() )
{
	ACE_Message_Block * mb = new ACE_Message_Block( sizeof( PipeMessage ));
	PipeMessage * pipe = (PipeMessage *) mb->wr_ptr();
	pipe->message = NULL;
	pipe->adapter = this;
	mb->wr_ptr(sizeof( PipeMessage));
	mb->msg_type( ACE_Message_Block::MB_START );
	manager->putq( mb );
}

SessionVisitorAdaptor::~SessionVisitorAdaptor()
{

}

void SessionVisitorAdaptor::execute( StartApplicationMessage & message )
{
    manager->execute( message, *host );
}

void SessionVisitorAdaptor::execute( SetApplicationModeMessage & message )
{
    manager->execute( message, *host );
}

void SessionVisitorAdaptor::execute( StopApplicationMessage & message )
{
    manager->execute( message, *host );
}

void SessionVisitorAdaptor::execute( AddUserMessage & message )
{
    manager->execute( message, *host );
}

void SessionVisitorAdaptor::execute( RemoveUserMessage & message )
{
    manager->execute( message, *host );
}

void SessionVisitorAdaptor::execute( JoinLocaleMessage & message )
{
    manager->execute( message, *host );
}

void SessionVisitorAdaptor::execute( LeaveLocaleMessage & message )
{
    manager->execute( message, *host );
}

void SessionVisitorAdaptor::execute( SetDIVParamMessage & message )
{
    manager->execute( message, *host );
}

void SessionVisitorAdaptor::execute( UserKitMessage & message )
{
    manager->execute( message, *host );
}

void SessionVisitorAdaptor::put( SessionMessage * message )
{
	ACE_Message_Block * mb = new ACE_Message_Block(sizeof( PipeMessage ));
	PipeMessage * pipe = (PipeMessage *) mb->wr_ptr();
	pipe->message = message;
	pipe->adapter = this;
	mb->wr_ptr(sizeof( PipeMessage));
	manager->putq( mb );
}

void SessionVisitorAdaptor::removeHost(void)
{
	ACE_Message_Block * mb = new ACE_Message_Block( sizeof( PipeMessage ));
	PipeMessage * pipe = (PipeMessage *) mb->wr_ptr();
	pipe->message = NULL;
	pipe->adapter = this;
	mb->wr_ptr(sizeof( PipeMessage));
	mb->msg_type( ACE_Message_Block::MB_STOP );
	manager->putq( mb );
}

