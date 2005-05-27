/* ===========================================================================
    (C) 2000 Vienna University of Technology
   ===========================================================================
    NAME:       sman
    TYPE:       c++ code
    PROJECT:    Studierstube Tools
    CONTENT:    Session manager
    VERSION:    2.0
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
                gh      Gerd Hesina
   ===========================================================================
    HISTORY:

    22-Feb-01 14:00:00  gh      last modification (cleanup, restructuring)
    14-Dec-00 14:35:48  jp      Linux port
    12-Dec-90 10:54:36  gh      created
=========================================================================== */

#include "sman.h"
#include <assert.h>

Sman::Sman()
:
monitorInterface(NULL),
acceptedMonitorSock(-1),
lastLoadApplicationTime(0.0),
lastLoadApplicationUserId(-1),
lastSystemBarChangedTime(0.0),
lastSetFocusTime(0.0),
ownAddress(NULL)
{
    lastLoadApplicationName[0]='\0';
	resetAddresses();
}

Sman::~Sman()
{
    // nil
}

void Sman::getOwnAddress()
{
   char charBuf[1024];
   struct hostent *host;
   struct in_addr interfaceAddress;
   if (gethostname(charBuf, sizeof(charBuf))<0)
   {
      perror("joinMulticastGroup: can't get own hostname");
      return;
   }

   if ((host=gethostbyname(charBuf))==NULL)
   {
      perror("joinMulticastGroup: can't get own internet address");
      return;
   }
   memcpy(&interfaceAddress, host->h_addr_list[0], sizeof(interfaceAddress));
   ownAddress=strdup(inet_ntoa(interfaceAddress));
}

void Sman::resetAddresses()
{
    divAddress = 210;
	divPort = 62400;
	serverPort = 63400;
}

void Sman::setNewAddresses(ContextParameters *buffer, SockInterface *master)
{

#ifndef FORCE_UNICAST
    sprintf(buffer->divAddress,"224.100.200.%d",divAddress);
#else
    strcpy(buffer->divAddress,"127.0.0.1");
#endif
    sprintf(buffer->divPort, "%d", divPort);
	sprintf(buffer->serverPort,"%d",serverPort);
    char *serverAddress = inet_ntoa(master->getAddress()->sin_addr);
    if((strcmp(serverAddress,"127.0.0.1")==0) &&
		(ownAddress != NULL)) serverAddress = ownAddress;
    strcpy(buffer->serverAddress,serverAddress);
}

void Sman::advanceAddresses()
{    
	if(divAddress==255) divAddress=99; //address rollover
    divAddress++;
    divPort++;
	serverPort++;
}

void Sman::loadBalance(SockInterface *net)
{
    // strategy:
	// load = numMasterProcesses + 0.5*numSlaveProcesses
    // find host with highest and lowest load

	float load = 0.0f;
	float lowestLoad = 0.0f;
	float highestLoad = 0.0f;

	SockInterface *highestInterface = NULL;
	SockInterface *lowestInterface = NULL;

    for( clientIterator socketIterator=clientSockets.begin(); socketIterator != clientSockets.end(); ++socketIterator )
    {
	    SockInterface* socketInterface = socketIterator->second;
		if(socketInterface == net) continue;
		load = masterProcesses[socketInterface].size() + 0.5f*slaveProcesses[socketInterface].size();
		if(load > highestLoad) 
		{
			highestLoad = load;
			highestInterface = socketInterface;
		}
		else if(load < lowestLoad)
		{
            lowestLoad = load;
			lowestInterface = socketInterface;
		}
	}

    if(highestInterface == NULL) return;
	if(lowestInterface == NULL) lowestInterface = highestInterface;

    float thisLoad = 1.0f + masterProcesses[net].size() + 0.5f*slaveProcesses[net].size();

    if(thisLoad > highestLoad)
	{
		ContextParameters *migrationContextParams=new ContextParameters;
		if(masterProcesses[net].size()==0) return;
		CProcess *sourceProcess=*(masterProcesses[net].begin());
		strcpy(migrationContextParams->contextName,
			sourceProcess->getContextName());
		strcpy(migrationContextParams->appName,
			sourceProcess->getAppName());
        migrationContextParams->userID=htonl(sourceProcess->getUid());
        migrate(net, lowestInterface, migrationContextParams);
		delete migrationContextParams;
	}
}

void Sman::migrate(SockInterface *source, SockInterface *destination, ContextParameters *ctxParams)
{
	if(source == destination) return;
    fprintf(stderr,"migrate from=%s:%d migrate to=%s:%d appName=%s contextName=%s\n",
                   inet_ntoa(source->getAddress()->sin_addr),
                   ntohs(source->getAddress()->sin_port),
                   inet_ntoa(destination->getAddress()->sin_addr),
                   ntohs(destination->getAddress()->sin_port),
				   ctxParams->appName,
		           ctxParams->contextName);

    source->initOutput();
    source->addInt32ToOutput(STB_SET_SLAVE_MODE);
	ctxParams->smanSocket=htonl(source->getSocket());
    source->addBytesToOutput(sizeof(ContextParameters), ctxParams); 
    if(!source->sendOutput()) removeClient(source);

    destination->initOutput();
    destination->addInt32ToOutput(STB_SET_MASTER_MODE);
    destination->addBytesToOutput(sizeof(ContextParameters), ctxParams); 
    if(!destination->sendOutput()) removeClient(destination);
}

void Sman::removeClient(SockInterface *net)
{
    fprintf(stderr,"client at address %s:%d is unreachable ==> removed!\n",
                   inet_ntoa(net->getAddress()->sin_addr),
                   ntohs(net->getAddress()->sin_port));
    int thisSocket=net->getSocket();
    delete net;
    clientSockets.erase(thisSocket);
    int size = clientSockets.size();
	if(size == 1)
        fprintf(stderr,"currently holding 1 client\n");
	else
        fprintf(stderr,"currently holding %d clients\n", size);
	if(size == 0)
		resetAddresses();
}

void Sman::createSlaveContext(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;

    buffer->getRemainingBytes(sendBuf, sendBufLen);
    ContextParameters *ctxParams=(ContextParameters*)sendBuf;

    // loadBalancing
	loadBalance(net);

	ctxParams->smanSocket=htonl(net->getSocket());
	CProcess* newMasterProcess=new CProcess(ctxParams);

	newMasterProcess->setToRespond(clientSockets.size()-1); //subscribe all slaves

	masterProcesses[net].push_front(newMasterProcess);
    setNewAddresses(ctxParams, net);

	broadcastMessage(sendBuf, sendBufLen, net, messageType);
	advanceAddresses();
}

void Sman::contextStreamed(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;

	int smanSock = buffer->getInt32();
    SockInterface *slave=clientSockets[smanSock];
	slave->initOutput();
	slave->addInt32ToOutput(messageType);
	slave->addInt32ToOutput(net->getSocket());
	if(messageType == STB_CONTEXT_STREAMED)
	{
	    ContextParameters *ctxParams = new ContextParameters;
	    setNewAddresses(ctxParams, net);
	    advanceAddresses();
	    slave->addStringToOutput(ctxParams->divAddress);
	    slave->addStringToOutput(ctxParams->divPort);
	    slave->addStringToOutput(ctxParams->serverAddress);
	    slave->addStringToOutput(ctxParams->serverPort);
	}
    buffer->getRemainingBytes(sendBuf, sendBufLen);
    slave->addBytesToOutput(sendBufLen, sendBuf);
    if(!slave->sendOutput()) removeClient(slave);
}

void Sman::slaveContextCreated(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;

    buffer->getRemainingBytes(sendBuf, sendBufLen);

	ContextParameters *ctxParams=(ContextParameters*)sendBuf;
	CProcess *newSlaveProcess=new CProcess(ctxParams);
            
	slaveProcesses[net].push_front(newSlaveProcess);
    int smanSock=newSlaveProcess->getSmanSocket();
	if(smanSock==-1)
	{
	    fprintf(stderr,"sman error: smanSock not initialized!\n");
		exit(-1);
	}

    SockInterface *master=clientSockets[smanSock];
	for(processIterator masterIterator=masterProcesses[master].begin();
	    masterIterator != masterProcesses[master].end();
        ++masterIterator)
	{
        CProcess *thisProcess=*masterIterator;
        if(strcmp(thisProcess->getContextName(),ctxParams->contextName)==0)
		{
		    int clientResponded=thisProcess->getResponded();
			int toRespond=thisProcess->getToRespond();
            if(toRespond==clientResponded+1)
			{
                thisProcess->setResponded(toRespond);
                master->initOutput();
                master->addInt32ToOutput(STB_START_DIV);
                master->addBytesToOutput(sendBufLen, sendBuf); 
                if(!master->sendOutput()) removeClient(master);
			}
			else
			    thisProcess->setResponded(clientResponded+1);
			break;
		}
	}
}

void Sman::slaveContextStreamed(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;

    buffer->getRemainingBytes(sendBuf, sendBufLen);

	StreamParameters *streamParams=(StreamParameters*)sendBuf;
    ContextParameters *ctxParams=new ContextParameters;
    strcpy(ctxParams->contextName, streamParams->contextName);
    strcpy(ctxParams->appName, streamParams->appName);
    ctxParams->userID = streamParams->userID;
    ctxParams->smanSocket = streamParams->smanSocket;
	CProcess *newSlaveProcess=new CProcess(ctxParams);
            
	slaveProcesses[net].push_front(newSlaveProcess);
    int smanSock=newSlaveProcess->getSmanSocket();
	if(smanSock==-1)
	{
	    fprintf(stderr,"sman error: smanSock not initialized!\n");
		exit(-1);
	}

    SockInterface *master=clientSockets[smanSock];
	fprintf(stderr,"check1\n");
	for(processIterator masterIterator=masterProcesses[master].begin();
	    masterIterator != masterProcesses[master].end();
        ++masterIterator)
	{
        CProcess *thisProcess=*masterIterator;
		fprintf(stderr,"thisProcess->getContextName()=%s , ctxParams->contextName=%s\n",
			thisProcess->getContextName(),ctxParams->contextName);
        if(strcmp(thisProcess->getContextName(),ctxParams->contextName)==0)
		{
		    int clientResponded=thisProcess->getResponded();
			int toRespond=thisProcess->getToRespond();
			if(clientResponded+1>toRespond)
				toRespond=clientResponded+1;
			fprintf(stderr,"toRespond=%d, clientResponded+1=%d\n",
				toRespond,clientResponded+1);
            if(toRespond==clientResponded+1)
			{
                thisProcess->setResponded(toRespond);
				if(messageType == STB_SLAVE_CONTEXT_STREAMED)
				{
                    master->initOutput();
                    master->addInt32ToOutput(STB_START_DIV_STREAMED);
                    master->addBytesToOutput(sendBufLen, sendBuf); 
                    if(!master->sendOutput()) removeClient(master);
				}
			}
			else
			    thisProcess->setResponded(clientResponded+1);
			break;
		}
	}
	delete ctxParams;
}

void Sman::slaveModeSet(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;

    buffer->getRemainingBytes(sendBuf, sendBufLen);

	ContextParameters *ctxParams=(ContextParameters*)sendBuf;
	CProcess *newSlaveProcess=new CProcess(ctxParams);
            
	slaveProcesses[net].push_front(newSlaveProcess);

    SockInterface *master=net;
	for(processIterator masterIterator=masterProcesses[master].begin();
	    masterIterator != masterProcesses[master].end();
        ++masterIterator)
	{
        CProcess *thisProcess=*masterIterator;
        if(strcmp(thisProcess->getContextName(),ctxParams->contextName)==0)
		{
            delete thisProcess;
            masterProcesses[master].erase(masterIterator);
			fprintf(stderr,"erasing mastercontext %s from host %s:%d. holding %d master processes\n",
                                   ctxParams->contextName,
                                   inet_ntoa(master->getAddress()->sin_addr),
                                   ntohs(master->getAddress()->sin_port),
                                   masterProcesses[master].size());

			break;
		}
	}
}

void Sman::masterModeSet(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;

    buffer->getRemainingBytes(sendBuf, sendBufLen);

	ContextParameters *ctxParams=(ContextParameters*)sendBuf;
	ctxParams->smanSocket=htonl(net->getSocket());
	CProcess *newMasterProcess=new CProcess(ctxParams);
    newMasterProcess->setToRespond(clientSockets.size()-1); //subscribe all slaves

	masterProcesses[net].push_front(newMasterProcess);
             
    SockInterface *slave=net;
	for(processIterator slaveIterator=slaveProcesses[slave].begin();
	    slaveIterator != slaveProcesses[slave].end();
        ++slaveIterator)
	{
        CProcess *thisProcess=*slaveIterator;
        if(strcmp(thisProcess->getContextName(),ctxParams->contextName)==0)
		{
            delete thisProcess;
            slaveProcesses[slave].erase(slaveIterator);
			fprintf(stderr,"erasing slavecontext %s from host %s:%d. holding %d slave processes\n",
                                   ctxParams->contextName,
                                   inet_ntoa(slave->getAddress()->sin_addr),
                                   ntohs(slave->getAddress()->sin_port),
                                   masterProcesses[slave].size());

			break;
		}
	}
    setNewAddresses(ctxParams, net);
	broadcastMessage(sendBuf, sendBufLen, net, STB_MIGRATE_SLAVE_CONTEXT);
    advanceAddresses();
}


void Sman::slaveContextDeleted(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;

    buffer->getRemainingBytes(sendBuf, sendBufLen);

	ContextParameters *ctxParams=(ContextParameters*)sendBuf;
             
    SockInterface *slave=net;
	for(processIterator slaveIterator=slaveProcesses[slave].begin();
	    slaveIterator != slaveProcesses[slave].end();
        ++slaveIterator)
	{
        CProcess *thisProcess=*slaveIterator;
        if(strcmp(thisProcess->getContextName(),ctxParams->contextName)==0)
		{
            delete thisProcess;
            slaveProcesses[slave].erase(slaveIterator);
			fprintf(stderr,"erasing slavecontext %s from host %s:%d. holding %d slave processes\n",
                                   ctxParams->contextName,
                                   inet_ntoa(slave->getAddress()->sin_addr),
                                   ntohs(slave->getAddress()->sin_port),
                                   masterProcesses[slave].size());

			break;
		}
	}
}

void Sman::masterContextDeleted(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;

    buffer->getRemainingBytes(sendBuf, sendBufLen);

	ContextParameters *ctxParams=(ContextParameters*)sendBuf;
             
    SockInterface *master=net;
	for(processIterator masterIterator=masterProcesses[master].begin();
	    masterIterator != masterProcesses[master].end();
        ++masterIterator)
	{
        CProcess *thisProcess=*masterIterator;
        if(strcmp(thisProcess->getContextName(),ctxParams->contextName)==0)
		{
            delete thisProcess;
            masterProcesses[master].erase(masterIterator);
			fprintf(stderr,"erasing mastercontext %s from host %s:%d. holding %d master processes\n",
                                   ctxParams->contextName,
                                   inet_ntoa(master->getAddress()->sin_addr),
                                   ntohs(master->getAddress()->sin_port),
                                   masterProcesses[master].size());

			break;
		}
	}
}

void Sman::getMasterContext(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;

    buffer->getRemainingBytes(sendBuf, sendBufLen);

	GetMasterContextMessage *gmcm=(GetMasterContextMessage*)sendBuf;
             
    SockInterface *slave=net;
    for( clientIterator socketIterator=clientSockets.begin();
		     socketIterator != clientSockets.end(); ++socketIterator )
	{
	    SockInterface* socketInterface = socketIterator->second;
	    for(processIterator masterIterator=masterProcesses[socketInterface].begin();
	        masterIterator != masterProcesses[socketInterface].end();
            ++masterIterator)
        {
            CProcess *thisProcess=*masterIterator;
            if(strcmp(thisProcess->getContextName(),gmcm->contextName)==0)
            {
                ContextParameters ctxParams;
		        strcpy(ctxParams.contextName, thisProcess->getContextName());
		        strcpy(ctxParams.appName,thisProcess->getAppName());
                ctxParams.userID=htonl(thisProcess->getUid());
                migrate(socketInterface, slave, &ctxParams);
	            fprintf(stderr,"migrating mastercontext %s from host %s:%d to %s:%d\n",
                                ctxParams.contextName,
                                inet_ntoa(slave->getAddress()->sin_addr),
                                ntohs(slave->getAddress()->sin_port),
                                inet_ntoa(socketInterface->getAddress()->sin_addr),
                                ntohs(socketInterface->getAddress()->sin_port)
                                );
		        return;
            }
		}
	}
}

void Sman::slaveContextMigrated(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;
    
	buffer->getRemainingBytes(sendBuf, sendBufLen);

	ContextParameters *ctxParams=(ContextParameters*)sendBuf;
	CProcess *newSlaveProcess=new CProcess(ctxParams);

    int smanSock=newSlaveProcess->getSmanSocket();
	if(smanSock==-1)
	{
	    fprintf(stderr,"sman error: smanSock not initialized!\n");
		exit(-1);
	}

    SockInterface *master=clientSockets[smanSock];
	for(processIterator masterIterator=masterProcesses[master].begin();
	    masterIterator != masterProcesses[master].end();
        ++masterIterator)
	{
        CProcess *thisProcess=*masterIterator;
        if(strcmp(thisProcess->getContextName(),ctxParams->contextName)==0)
		{
		    int clientResponded=thisProcess->getResponded();
			int toRespond=thisProcess->getToRespond();
            if(toRespond==clientResponded+1)
			{
                thisProcess->setResponded(toRespond);
                master->initOutput();
                master->addInt32ToOutput(STB_START_DIV_MIGRATED);
                master->addBytesToOutput(sendBufLen, sendBuf); 
                if(!master->sendOutput()) removeClient(master);
			}
			else
				thisProcess->setResponded(clientResponded+1);
			break;
		}
	}
}

void Sman::loadApplication(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;
    
	double now = osTimeOfDay();
	buffer->getRemainingBytes(sendBuf, sendBufLen);

	int loadFlag = FALSE;
	ContextParameters *ctxParams=(ContextParameters*)sendBuf;
	int userId = ntohl(ctxParams->userID);
    if((lastLoadApplicationUserId == userId) && 
	   (strcmp(lastLoadApplicationName,ctxParams->appName)==0)
	   )
	{
		if((now - lastLoadApplicationTime)>2000.0)
            loadFlag = TRUE;
	}
	else
	{
        loadFlag = TRUE;
	}

	if(loadFlag)
	{
		lastLoadApplicationTime = now;
		lastLoadApplicationUserId = userId;
		strcpy(lastLoadApplicationName, ctxParams->appName);

        float load = 0.0f;
	    float lowestLoad = 0.0f;

	    SockInterface *lowestInterface = NULL;

        for( clientIterator socketIterator=clientSockets.begin();
		     socketIterator != clientSockets.end(); ++socketIterator )
		{
	        SockInterface* socketInterface = socketIterator->second;
		    load = masterProcesses[socketInterface].size() + 0.5f*slaveProcesses[socketInterface].size();
		    if(load < lowestLoad)
			{
                lowestLoad = load;
			    lowestInterface = socketInterface;
			}
		}
        if(lowestInterface == NULL) lowestInterface = net;
        lowestInterface->initOutput();
        lowestInterface->addInt32ToOutput(messageType);
        lowestInterface->addBytesToOutput(sendBufLen, sendBuf); 
        if(!lowestInterface->sendOutput()) removeClient(lowestInterface);
	}
}

void Sman::systemBarChanged(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;
    
	double now = osTimeOfDay();
	SbBool forceUpdate=FALSE;
	buffer->getRemainingBytes(sendBuf, sendBufLen);

	SystemBarMessage *sbm=(SystemBarMessage*)sendBuf;
	if((now - lastSystemBarChangedTime)>2000.0)
    {
	    lastSystemBarChangedTime = now;
		forceUpdate = TRUE;
	}
    if((forceUpdate)||(ntohl(sbm->buttonId) == 1)||(memcmp(sendBuf, (void*)&lastSystemBar, sendBufLen)!=0))
	{
	    fprintf(stderr,"processed\n");
        memcpy((void*)&lastSystemBar, sendBuf, sendBufLen);
        broadcastMessage(sendBuf, sendBufLen, NULL, messageType);
	}
	else
		fprintf(stderr,"ignored\n");
}

void Sman::setFocus(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;
    
	double now = osTimeOfDay();
	SbBool forceUpdate=FALSE;
	buffer->getRemainingBytes(sendBuf, sendBufLen);

	if((now - lastSetFocusTime)>2000.0)
    {
	    lastSetFocusTime = now;
		forceUpdate = TRUE;
	}
    if((forceUpdate)||(memcmp(sendBuf, (void*)&lastSetFocus, sendBufLen)!=0))
	{
        memcpy((void*)&lastSetFocus, sendBuf, sendBufLen);
        broadcastMessage(sendBuf, sendBufLen, NULL, messageType);
	}
}

void Sman::queryApps(SockInterface *net, CBuffer *buffer, int32 messageType)
{
    void *sendBuf;
    ulong sendBufLen;
    
	buffer->getRemainingBytes(sendBuf, sendBufLen);

	int masterProcessSize = 0;
	clientIterator socketIterator;
    for( socketIterator=clientSockets.begin();
	     socketIterator != clientSockets.end(); ++socketIterator )
    {
	    SockInterface* socketInterface = socketIterator->second;
	    if(socketInterface==net) continue;
        masterProcessSize += masterProcesses[socketInterface].size();
	}
	if(masterProcessSize == 0) return;
    net->initOutput();    
    net->addInt32ToOutput(messageType);
    net->addInt32ToOutput(masterProcessSize);
    for( socketIterator=clientSockets.begin();
	     socketIterator != clientSockets.end(); ++socketIterator )
    {
	    SockInterface* socketInterface = socketIterator->second;
	    if(socketInterface==net) continue;
       	for(processIterator masterIterator=masterProcesses[socketInterface].begin();
            masterIterator != masterProcesses[socketInterface].end();
            ++masterIterator)
		{
            CProcess *thisProcess=*masterIterator;
            net->addStringToOutput(thisProcess->getAppName());
            net->addStringToOutput(thisProcess->getContextName());
			net->addInt32ToOutput(thisProcess->getUid());
			net->addInt32ToOutput(thisProcess->getSmanSocket());
			socketInterface->initOutput();
			socketInterface->addInt32ToOutput(STB_STREAM_CONTEXT);
            socketInterface->addStringToOutput(thisProcess->getAppName());
            socketInterface->addStringToOutput(thisProcess->getContextName());
			socketInterface->addInt32ToOutput(thisProcess->getUid());
			socketInterface->addInt32ToOutput(net->getSocket());
            if(!socketInterface->sendOutput())
			{
				removeClient(socketInterface);
				break;
			}
		}
	}
    if(!net->sendOutput()) removeClient(net);
}

void Sman::broadcastMessage(void *sendBuf, ulong sendBufLen, SockInterface *net, int32 messageType)
{
    for( clientIterator socketIterator=clientSockets.begin();
	     socketIterator != clientSockets.end(); ++socketIterator )
    {
	    SockInterface* socketInterface = socketIterator->second;
        if(socketInterface==net) continue;
        socketInterface->initOutput();
        socketInterface->addInt32ToOutput(messageType);
        socketInterface->addBytesToOutput(sendBufLen, sendBuf); 
        if(!socketInterface->sendOutput()) removeClient(socketInterface);
    }
}

SockInterface* Sman::getClientWithNumber(int clientNumber)
{
    int i=1;
    for( clientIterator socketIterator=clientSockets.begin();
	     socketIterator != clientSockets.end(); ++socketIterator, ++i )
    {			
        if(i==clientNumber)
		    return socketIterator->second;
	}
	return NULL;
}

CProcess* Sman::getMasterProcessWithNumber(SockInterface *net, int masterProcessNumber)
{
    int i=1;
    for( processIterator masterIterator=masterProcesses[net].begin();
	     masterIterator != masterProcesses[net].end(); ++masterIterator, ++i )
    {			
        if(i==masterProcessNumber)
		    return *masterIterator;
	}
	return NULL;
}

void Sman::processMessage(char *msg, long msgLength, int s)
{
    if(msgLength==0) return;
    CBuffer buffer((void*)msg, msgLength, CBuffer::CBUFFER_STATIC);
    SockInterface *net=clientSockets[s];
    int32 messageType=buffer.getInt32();

    void *sendBuf;
    ulong sendBufLen;

    switch(messageType)
    {
        case STB_CREATE_SLAVE_CONTEXT:
        {
            fprintf(stderr,
				"SessionManager: STB_CREATE_SLAVE_CONTEXT received\n");
            createSlaveContext(net, &buffer, messageType);
			break;
		}
        case STB_SLAVE_CONTEXT_CREATED:
        {
            fprintf(stderr,
				"SessionManager: STB_SLAVE_CONTEXT_CREATED received\n");
            slaveContextCreated(net, &buffer, messageType);
			break;
		}
        case STB_SLAVE_MODE_SET:
        {
            fprintf(stderr,
				"SessionManager: STB_SLAVE_MODE_SET received\n");
			slaveModeSet(net, &buffer, messageType);
			break;
		}
        case STB_MASTER_MODE_SET:
        {
            fprintf(stderr,
				"SessionManager: STB_MASTER_MODE_SET received\n");
			masterModeSet(net, &buffer, messageType);
			break;
		}
        case STB_SLAVE_CONTEXT_MIGRATED:
        {
            fprintf(stderr,
				"SessionManager: STB_SLAVE_CONTEXT_MIGRATED received\n");
			slaveContextMigrated(net, &buffer, messageType);
			break;
		}
        case STB_SLAVE_CONTEXT_DELETED:
		{
            fprintf(stderr,
				"SessionManager: STB_SLAVE_CONTEXT_DELETED received\n");
			slaveContextDeleted(net, &buffer, messageType);
			break;
		}
        case STB_MASTER_CONTEXT_DELETED:
		{
            fprintf(stderr,
				"SessionManager: STB_MASTER_CONTEXT_DELETED received\n");
			masterContextDeleted(net, &buffer, messageType);
			break;
		}
        case STB_LOAD_APPLICATION:
		{
            fprintf(stderr,
				"SessionManager: STB_LOAD_APPLICATION received\n");
			loadApplication(net, &buffer, messageType);
			break;
		}
        case STB_SYSTEMBAR_CHANGED:
		{
            fprintf(stderr,
				"SessionManager: STB_SYSTEMBAR_CHANGED received\n");
			systemBarChanged(net, &buffer, messageType);
			break;
		}
        case STB_SET_FOCUS:
		case STB_USE_RESOURCES:
		{
            fprintf(stderr,
				"SessionManager: STB_SET_FOCUS or STB_USE_RESOURCES received\n");
			setFocus(net, &buffer, messageType);
			break;
		}
		case STB_QUERY_APPS_FROM_SMAN:
		{
            fprintf(stderr,
				"SessionManager: STB_QUERY_APPS_FROM_SMAN received\n");
			queryApps(net, &buffer, messageType);
			break;
		}
		case STB_CONTEXT_STREAMED:
		case STB_CONTEXT_STREAMED_WITH_DIV:
		{
            fprintf(stderr,
				"SessionManager: STB_CONTEXT_STREAMED or _WITH_DIV received\n");
			contextStreamed(net, &buffer, messageType);
			break;
		}
		case STB_SLAVE_CONTEXT_STREAMED_WITH_DIV:
		case STB_SLAVE_CONTEXT_STREAMED:
		{
            fprintf(stderr,
				"SessionManager: STB_SLAVE_CONTEXT_STREAMED or _WITH_DIVreceived\n");
			slaveContextStreamed(net, &buffer, messageType);
			break;
		}
        case STB_GET_MASTER_CONTEXT:
        {
            fprintf(stderr,
				"SessionManager: STB_GET_MASTER_CONTEXT\n");
			getMasterContext(net, &buffer, messageType);
			break;
        }
        //distribute the following messages to all clients
		//(except to the original sender)
        case STB_FORCE_FOCUS_CHANGE:
        case STB_WINDOWBAR_CHANGED:
        case STB_TASKBAR_CHANGED:
        case STB_SHEETBAR_CHANGED:
        {
            buffer.getRemainingBytes(sendBuf, sendBufLen);
			broadcastMessage(sendBuf, sendBufLen, net, messageType);
			break;
		}
        default:
            fprintf(stderr,"SessionManager: unknown message received (%d)\n",
               messageType);
    }    
}

void Sman::monitorUsage()
{
	if(monitorInterface == NULL) return;
	monitorInterface->initOutput();
	monitorInterface->addStringToOutput("sman help\n\r");
	monitorInterface->addStringToOutput("=========\n\r\n\r");
	monitorInterface->addStringToOutput("?, h ... print help page\n\r");
	monitorInterface->addStringToOutput(
		"l ... list all clients and their m/s processes\n\r");
	monitorInterface->addStringToOutput(
		"m c1 mp c2 ... migrate mp (master process) from c1 to c2\n\r");
	monitorInterface->addStringToOutput(
		"k client_number ... kick client with client_number\n\r");
	monitorInterface->addStringToOutput(
		"c ... clear sman (kick all clients)\n\r");
	monitorInterface->addStringToOutput(
		"s ... shutdown system (close monitor, sman and all clients)\n\r");
	monitorInterface->addStringToOutput("q ... quit monitor\n\r");
	monitorInterface->sendAsciiOutput();
}

void Sman::processMonitorMessage(char *msg, long msgLength)
{
    if(msgLength==0) return;
    
	char buf[1024];

	switch(*msg)
	{
	    case 'l':
	    case 'L':
		{
			cerr<<"SessionManager: Listing clients"<<endl;
			int size = clientSockets.size();
			sprintf(buf, "%d %s\n\r", size, (size==1)?"client":"clients");
            monitorInterface->initOutput();
            monitorInterface->addStringToOutput(
				"Current client list\n\r");
            monitorInterface->addStringToOutput(
				"===================\n\r\n\rholding ");
			monitorInterface->addStringToOutput(buf);
            monitorInterface->sendAsciiOutput();
			if(size == 0) break;
            monitorInterface->initOutput();
			int clientNumber = 0;
            for( clientIterator socketIterator=clientSockets.begin();
			     socketIterator != clientSockets.end(); ++socketIterator )
			{
				clientNumber++;
				SockInterface *socketInterface = socketIterator->second;
                sprintf(buf, "%d ... ", clientNumber);
                monitorInterface->addStringToOutput(buf);
				sprintf(buf,"@%s:%d\n\r",
                inet_ntoa(socketInterface->getAddress()->sin_addr),
                ntohs(socketInterface->getAddress()->sin_port));
                monitorInterface->addStringToOutput(buf);
				int masterProcessesSize = masterProcesses[socketInterface].size();
				int slaveProcessesSize = slaveProcesses[socketInterface].size();
				monitorInterface->addStringToOutput("  masterProcesses: ");
                sprintf(buf, "%d\n\r", masterProcessesSize);
                monitorInterface->addStringToOutput(buf);
                int masterNumber = 0;
               	for(processIterator masterIterator=masterProcesses[socketInterface].begin();
                    masterIterator != masterProcesses[socketInterface].end();
                    ++masterIterator)
				{
					masterNumber++;
                    sprintf(buf, "  %d ... ", masterNumber);
                    monitorInterface->addStringToOutput(buf);
                    CProcess *thisProcess=*masterIterator;
					sprintf(buf,"appName: >>%s<< contextName: >>%s<< userId: %d\n\r",
          						thisProcess->getAppName(),
								thisProcess->getContextName(),
								thisProcess->getUid()
								); 
                    monitorInterface->addStringToOutput(buf);
				}
				monitorInterface->addStringToOutput("  slaveProcesses: ");
                sprintf(buf, "%d\n\r", slaveProcessesSize);
                monitorInterface->addStringToOutput(buf);
                int slaveNumber = 0;
               	for(processIterator slaveIterator=slaveProcesses[socketInterface].begin();
                    slaveIterator != slaveProcesses[socketInterface].end();
                    ++slaveIterator)
				{
					slaveNumber++;
                    sprintf(buf, "  %d ... ", slaveNumber);
                    monitorInterface->addStringToOutput(buf);
                    CProcess *thisProcess=*slaveIterator;
					sprintf(buf,"appName: >>%s<< contextName: >>%s<< userId: %d\n\r",
          						thisProcess->getAppName(),
								thisProcess->getContextName(),
								thisProcess->getUid()
								); 
                    monitorInterface->addStringToOutput(buf);
				}
			}
            monitorInterface->sendAsciiOutput();
			break;
		}
	    case 'q':
	    case 'Q':
		{
            cerr<<"SessionManager: Monitor disappeared"<<endl;
            delete monitorInterface;
			monitorInterface = NULL;
			acceptedMonitorSock = -1;
			break;
		}
		case 'c':
		case 'C':
		{
            SockInterface *thisClient=getClientWithNumber(1);
			do
			{
   			    if(thisClient) removeClient(thisClient);
                thisClient=getClientWithNumber(1);
			} while(thisClient!=NULL);
			break;
		}
		case 'k':
		case 'K':
		{
			cerr<<"SessionManager: Kick client"<<endl;
			int clientNumber=-1;
			if((msgLength>3)&&(msg[1]==' '))
			    clientNumber = atoi(&msg[2]);
            if(clientNumber<=0)
			{
				monitorInterface->initOutput();
				monitorInterface->addStringToOutput("help kick: k client_number\n\r");
				monitorInterface->sendAsciiOutput();
				break;
			}
			int size = clientSockets.size();
			if(clientNumber>size)
			{
				monitorInterface->initOutput();
				monitorInterface->addStringToOutput("holding only ");
                sprintf(buf, "%d %s\n\r", size, (size==1)?"client":"clients");
				monitorInterface->addStringToOutput(buf);
				monitorInterface->sendAsciiOutput();
				break;
			}
			SockInterface *thisClient=getClientWithNumber(clientNumber);
			if(thisClient) removeClient(thisClient);
			break;
		}
		case 'm':
		case 'M':
		{
			cerr<<"SessionManager: Migrate client"<<endl;
			int clientNumber=-1;
			int clientNumber2=-1;
			int masterProcessNumber=-1;
			if((msgLength>3)&&(msg[1]==' '))
			{
				char *token = strtok(&msg[2], " ");
				if(token != NULL)
				{
			        clientNumber = atoi(token);
					token = strtok(NULL, " ");
					if(token != NULL)
					{
			            masterProcessNumber = atoi(token);
					    token = strtok(NULL, " ");
					    if(token != NULL)
                            clientNumber2 = atoi(token);
					}
                }
			}
            if(clientNumber<=0||clientNumber2<=0||masterProcessNumber<=0)
			{
				monitorInterface->initOutput();
				monitorInterface->addStringToOutput("help migrate: m c1 mp c2\n\r");
				monitorInterface->sendAsciiOutput();
				break;
			}
			int size = clientSockets.size();
			if(clientNumber>size||clientNumber2>size)
			{
				monitorInterface->initOutput();
				monitorInterface->addStringToOutput("holding only ");
                sprintf(buf, "%d %s\n\r", size, (size==1)?"client":"clients");
				monitorInterface->addStringToOutput(buf);
				monitorInterface->sendAsciiOutput();
				break;
			}
			SockInterface *c1=getClientWithNumber(clientNumber);
			SockInterface *c2=getClientWithNumber(clientNumber2);
			if(!c1||!c2||c1==c2) break;
			size=masterProcesses[c1].size();
			if(masterProcessNumber>size)
			{
				monitorInterface->initOutput();
				monitorInterface->addStringToOutput("holding only ");
                sprintf(buf, "%d %s\n\r", size, (size==1)?"process":"processes");
				monitorInterface->addStringToOutput(buf);
				monitorInterface->sendAsciiOutput();
				break;
			}
            ContextParameters *migrationContextParams=new ContextParameters;
		    CProcess *sourceProcess=getMasterProcessWithNumber(c1,masterProcessNumber);
			if(sourceProcess==NULL) break;
		    strcpy(migrationContextParams->contextName,
			    sourceProcess->getContextName());
		    strcpy(migrationContextParams->appName,
			    sourceProcess->getAppName());
            migrationContextParams->userID=htonl(sourceProcess->getUid());
            migrate(c1, c2, migrationContextParams);
			delete migrationContextParams;
			break;
		}
		case '?':
		case 'h':
		case 'H':
			cerr<<"SessionManager: Print help"<<endl;
			monitorUsage();
			break;
		case 's':
		case 'S':
			cerr<<"SessionManager: shutdown"<<endl;
			exit(EXIT_SUCCESS);
			break;
	    default:
			cerr<<"SessionManager: unknow monitor message received. ignored."<<endl;
		break;
	}
}

int Sman::createRegistrationSocket(int port)
{
    // Create a registration socket for new clients
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // bind it to a port on the local host
    struct sockaddr_in srvr_addr = {
                AF_INET,
                htons(port),
                INADDR_ANY};

    int on=1;
#ifdef WIN32
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
#else
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
#endif

    if( bind(sock, (const struct sockaddr *)&srvr_addr, sizeof(srvr_addr)) < 0 )
    {
        perror("SessionManager: bind");
        exit(-1);
    }

    // make it passive
    if( listen(sock, 5) < 0 )
    {
        perror("SessionManager: listen");
        exit(-1);
    }
	return sock;
}

 
#define SMAN_USAGE fprintf(stderr,"usage: sman [client port] [monitor port]\n");

void Sman::run(int argc, char **argv)
{
	// default values for port and monitorPort
    int port = 1234;
    int monitorPort = 1235;

    // command line: check port and monitorport
    if(argc == 1)
	{
        SMAN_USAGE
        fprintf(stderr,"using default port %d for client connections\n", port);
        fprintf(stderr,"using default port %d for monitors\n", monitorPort);
	}
    else if(argc == 2)
	{
        port = atoi(argv[1]);
        fprintf(stderr,"using port %d for client connections\n", port);
        fprintf(stderr,"using default port %d for monitors\n", monitorPort);
	}
	else if(argc == 3)
	{
        port = atoi(argv[1]);
        monitorPort = atoi(argv[2]);
        fprintf(stderr,"using port %d for client connections\n", port);
        fprintf(stderr,"using port %d for monitors\n", monitorPort);        
	}
	else
	{
		SMAN_USAGE
		exit(EXIT_FAILURE);
	}

#ifdef WIN32
    initNetwork();
#endif
    getOwnAddress();
	int sock = createRegistrationSocket(port);
    int monitorSock = createRegistrationSocket(monitorPort);
	int biggestSockNumber = monitorSock;

    // main loop
    while(1)
    {
        // listen to incoming messages
        // init select: all sockets
        fd_set readfds;
		clientIterator socketIterator;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        FD_SET(monitorSock, &readfds);
	    for( socketIterator=clientSockets.begin();
		     socketIterator != clientSockets.end(); ++socketIterator )
            FD_SET(socketIterator->first, &readfds);
        if(acceptedMonitorSock>0)
		    FD_SET(acceptedMonitorSock, &readfds);
        // check if some sockets have been hit
        int s = select(biggestSockNumber+1,&readfds, NULL, NULL, 0);
	    if( s < 0 )
	        perror("SessionManager: select");

        // registration socket hit by a new client?
        if (FD_ISSET(sock, &readfds))
        {
            struct sockaddr_in addr;
#ifdef _LINUX_SOURCE
            socklen_t len;
#else
            int len;
#endif
            int s;

            len = sizeof(addr);

            // accept new client
            if( (s = accept(sock, (struct sockaddr *)&addr, &len)) < 0 )
            {
	            fprintf(stderr, "SessionManager: accept returned %d", s);
		        perror("SessionManager: accept");
		        continue;
			}

	        // prepare to accept connection
            fprintf(stderr, "SessionManager: Accepting client connection from %s:%d\n",
                    inet_ntoa(addr.sin_addr),
                    ntohs(addr.sin_port));
            // create new socket interface and bind it 
            clientSockets[s] = new SockInterface; // put a new socket iface into the map
            clientSockets[s]->bind(s, &addr);            // bind the soket
            clientSockets[s]->setVerbose(0);      // no debug

            // biggest sock number is required by function select
            if( biggestSockNumber < s ) biggestSockNumber = s;
	        FD_CLR(sock, &readfds);
        }

        // monitor registration socket hit by a new monitor?
        if (FD_ISSET(monitorSock, &readfds))
        {
            struct sockaddr_in addr;
#ifdef _LINUX_SOURCE
            socklen_t len;
#else
            int len;
#endif
            int s;

            len = sizeof(addr);

            // accept new client
            if( (s = accept(monitorSock, (struct sockaddr *)&addr, &len)) < 0 )
            {
	            fprintf(stderr, "SessionManager: monitor accept returned %d", s);
		        perror("SessionManager: accept");
		        continue;
			}

	        // prepare to accept monitor connection
            fprintf(stderr, "SessionManager: Accepting monitor connection from %s:%d\n",
                    inet_ntoa(addr.sin_addr),
                    ntohs(addr.sin_port));

			if(monitorInterface)
			{
				char buf[1024];
                sprintf(buf,"closing this connection. reason: current active monitor connection from %s:%d\n",
                   inet_ntoa(monitorInterface->getAddress()->sin_addr),
                   ntohs(monitorInterface->getAddress()->sin_port));
				SockInterface *tempInterface = new SockInterface;
				tempInterface->bind(s, &addr);
				tempInterface->setVerbose(0);
				tempInterface->initOutput();
				tempInterface->addBytesToOutput(strlen(buf), buf);
				tempInterface->addStringToOutput("\r");
				tempInterface->sendAsciiOutput();
                delete tempInterface;
				fprintf(stderr,"%s", buf);
                FD_CLR(monitorSock, &readfds);
			}
            else
			{
                // create new socket interface and bind it 
                monitorInterface = new SockInterface;  // store monitor interface
                monitorInterface->bind(s, &addr);     // bind the soket
                monitorInterface->setVerbose(0);      // no debug

                // biggest sock number is required by function select
                if( biggestSockNumber < s ) biggestSockNumber = s;
	            FD_CLR(monitorSock, &readfds);
			    acceptedMonitorSock = s;

				monitorUsage();
			}
        }

		int check=1;
		do
		{
            // iterate through registered clients
            for( socketIterator=clientSockets.begin();
			     socketIterator !=clientSockets.end(); socketIterator++ )        
			{
	            if ( FD_ISSET(socketIterator->first, &readfds))
				{
	                SockInterface* socketInterface = socketIterator->second;
                    long msgLength=0;
                    char* msg = socketInterface->getMessage(0, &msgLength);
			        if( ((int)msg) == SockInterface::SOCKET_CLOSED)
					{
                        cerr<<"SessionManager: Client disappeared"<<endl;
                        removeClient(socketInterface);
				        int size=clientSockets.size();
				        if(size==0) check=1;
				        else check=0;
				        break;
					}
                    else
					{
                        // process message
//                      if(msgLength>0) cerr<<"SessionManager: message received"<<endl;
	                    processMessage(msg, msgLength, socketInterface->getSocket());
                        delete msg;
				        check=1;
				        FD_CLR(socketIterator->first, &readfds);
					} 
				} //if(FD_ISSET)
			} //for
    	    if(check==1) break;
	        check=1;
		} while (1);

        if ((acceptedMonitorSock > 0) && (FD_ISSET(acceptedMonitorSock, &readfds)))
		{
            long msgLength=0;
            char* msg = monitorInterface->getAsciiMessage(&msgLength);
			if( ((int)msg) == SockInterface::SOCKET_CLOSED)
			{
                cerr<<"SessionManager: Monitor disappeared"<<endl;
				delete monitorInterface;
				monitorInterface = NULL;
				acceptedMonitorSock = -1;
			}
            else
			{
                // process monitor message
                if(msgLength>0) cerr<<"SessionManager: monitor message received"<<endl;
	            processMonitorMessage(msg, msgLength);
                delete msg;
                FD_CLR(acceptedMonitorSock, &readfds);
			} 
		} //if(FD_ISSET)
    } //while
}

/* ===========================================================================
   End of sman.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   eval: (setq indent-tabs-mode nil)
   End:
=========================================================================== */
