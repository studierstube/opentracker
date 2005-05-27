#include "TCPServerClient.h"

// STL includes
#include <algorithm>

TCPHandler::~TCPHandler()
{
    this->peer().close();
    if (acceptor!=NULL) acceptor->removeClient(this);    
}

int TCPHandler::open(void *_acceptor)
{
    int result=ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open(_acceptor);
    if (result==0){
        acceptor=(TCPAcceptor*)_acceptor;
        acceptor->addClient(this);
    }
    return result;
}

int TCPHandler::handle_input(ACE_HANDLE fd)
{
    char buffer[2048];
    int count=peer().recv(buffer,sizeof(buffer));
	if (count<=0){
		acceptor->removeClient(this);
		return -1;
	}	
	// add trailing '\0' for end of string
	buffer[count++]=0;
	
    // notify acceptor
    acceptor->receivedFromClient(buffer);

    return 0;
}    

// ----------------------------------------------------------------------------------------------

TCPAcceptor::~TCPAcceptor()
{
	vector<TCPHandler*>::reverse_iterator it=clients.rbegin();
    while(it!=clients.rend()){
        (*it)->destroy();
        it++;
    }
    clients.clear();
}

void TCPAcceptor::addClient(TCPHandler *client)
{
    if(find(clients.begin(),clients.end(),client)==clients.end()){ 
		clients.push_back(client);
		printf("SOCKETMODULE: client added\n");
	}
}

void TCPAcceptor::removeClient(TCPHandler *client)
{
    vector<TCPHandler*>::iterator it=find(clients.begin(),clients.end(),client);
    if (it!=clients.end()){
        clients.erase(it);
		printf("SOCKETMODULE: client removed\n");
	}
}

void TCPAcceptor::sendToAllClients(string buffer)
{
	vector<TCPHandler*>::iterator it;
    for(it=clients.begin();it!=clients.end();it++){
		if  (((*it)->peer().send_n(buffer.c_str(),buffer.length()))<0){
            printf("SOCKETMODULE: could not send message to client \n");
        }
    }
}

void TCPAcceptor::receivedFromClient(string buffer)
{
    server->receive(buffer);
}

// ----------------------------------------------------------------------------------------------

TCPServer::TCPServer()
{
    acceptor=new TCPAcceptor(this);
    reactor=ACE_Reactor::instance();
}

TCPServer::~TCPServer()
{
    delete acceptor;
}

int TCPServer::open(int port)
{
    if (acceptor->open(ACE_INET_Addr(port),reactor)==-1) {
        ACE_ERROR_RETURN((LM_ERROR,"%p\n","open"),-1);
    }
    ACE_DEBUG((LM_DEBUG,"(%P|%t) starting up server daemon\n"));

    return 0;
}

void TCPServer::send(string msg)
{
    acceptor->sendToAllClients(msg);	
}

void TCPServer::receive(string msg)
{
    callback(msg,userData);
    //printf("SOCKETMODULE: TCPServer received '%s'\n",msg.c_str());
}

void TCPServer::setReceiveCB(TCPReceiveCB *cb)
{
    callback=cb;
}

void TCPServer::setUserData(void *data)
{
    userData=data;
}

//--------------------------------------------------------------------------------

TCPClient::TCPClient() :
  initialized_(false)
{
    connector=new ACE_SOCK_Connector;
}

TCPClient::TCPClient(const char *server,int port) :
  initialized_(false)
{
    connector=new ACE_SOCK_Connector;
    addr.set(port,server);
}

TCPClient::~TCPClient()
{
    delete connector;
}

int TCPClient::open(){
    connector->connect(*this, addr,&ACE_Time_Value::zero);
    if (errno!=EWOULDBLOCK && connector->complete(*this,0,&ACE_Time_Value::zero)!=-1){
        initialized_=true;
        return 0;
    }
    else return -1;
}

int TCPClient::send(string msg)
{
  if (initialized_ && (initialized_=((unsigned int)this->send_n(msg.c_str(),msg.length())==msg.length()))) return 0;
    else return -1;
}

int TCPClient::receive()
{
    char buffer[2048];
    int cnt;
    if (initialized_){
        if ((cnt=this->recv(buffer,sizeof(buffer),&ACE_Time_Value::zero))>0){
            buffer[cnt]='\0';
            string msg=buffer;
            callback(msg,userData);
            //printf("SOCKETMODULE: TCPClient received '%s'\n",msg.c_str());
        }
        else {
            if (errno!=ETIME){
                initialized_=false;
                ACE_ERROR((LM_ERROR,"%p\n","open"));
                return -1;
            }
        }
    }

    return 0;
}

void TCPClient::setReceiveCB(TCPReceiveCB *cb)
{
    callback=cb;
}

void TCPClient::setUserData(void *data)
{
    userData=data;
}
