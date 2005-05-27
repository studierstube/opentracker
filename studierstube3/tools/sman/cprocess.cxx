// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       cprocess.h
//  TYPE:       c++ code
//  PROJECT:    Studierstube: session manager
//  CONTENT:    master/slave CProcess structure
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  16-jan-01 10:59:18  gh      created
// ===========================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cprocess.h"

// default constructor
CProcess::CProcess()
{
    init();
}

//destructor
CProcess::~CProcess()
{
}

CProcess::CProcess(ContextParameters* ctxParams)
{
	init();
    setContextName(ctxParams->contextName);
	setAppName(ctxParams->appName);
    setUid(ntohl(ctxParams->userID));
	setSmanSocket(ntohl(ctxParams->smanSocket));
}

void CProcess::init()
{
	appName[0]='\0';
	contextName[0]='\0';
	uid=0;
	toRespond=0;
	responded=0;
	socket=-1;
}

void
CProcess::setContextName(char *theContextName)
{
	strcpy(contextName,theContextName);
}

char*
CProcess::getContextName()
{
	return contextName;
}

void
CProcess::setAppName(char *theAppName)
{
	strcpy(appName,theAppName);
}

char*
CProcess::getAppName()
{
	return appName;
}

void
CProcess::setUid(int theUid)
{
	uid=theUid;
}

int
CProcess::getUid()
{
	return uid;
}

void
CProcess::setToRespond(int theInt)
{
	toRespond=theInt;
}

int
CProcess::getToRespond()
{
	return toRespond;
}

void
CProcess::setResponded(int theInt)
{
	responded=theInt;
}

int
CProcess::getResponded()
{
	return responded;
}

void
CProcess::setSmanSocket(int theInt)
{
	socket=theInt;
}

int
CProcess::getSmanSocket()
{
	return socket;
}
