// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       cprocess.h
//  TYPE:       c++ header file
//  PROJECT:    Studierstube: session manager
//  CONTENT:    master/slave process structure
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  16-jan-01 10:59:18  gh      created
// ===========================================================================
#ifndef _CPROCESS_H_
#define _CPROCESS_H_

#include <stbapi/workspace/ContextParameters.h>

#include <sys/types.h>
#include <stbapi/stbapi.h>
#ifndef WIN32
#include <sys/socket.h>
#include <arpa/inet.h>
#else
#include <winsock.h>
#endif

class CProcess
{

public:
	CProcess();
	~CProcess();

	CProcess(ContextParameters* ctxParams);
	void init();
	void setContextName(char *theContextName);
	char *getContextName();
	void setAppName(char *theAppName);
	char *getAppName();
	void setUid(int theUid);
	int  getUid();
	void setToRespond(int theInt);
	int  getToRespond();
	void setResponded(int theInt);
	int  getResponded();
	void setSmanSocket(int theInt);
	int  getSmanSocket();
    
private:
	char contextName[512];
	char appName[512];
	int uid;
	int toRespond;
	int responded;
	int socket;
};

#endif
//eof
