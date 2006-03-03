/* ========================================================================
 * Copyright (C) 2005  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** The header file for the OTOpenVideoContext class.
  *
  * @author Denis Kalkofen
  * @author Erick Mendez
  *
  * $Id: OTOpenVideoContext.cxx 25 2005-11-28 16:11:59Z denis $
  * @file                                                                   */
 /* ======================================================================= */

#include "OTOpenVideoContext.h"

#include "../OpenTracker.h"
#ifdef USE_ARTOOLKITPLUS

#include <ace/Thread.h>
#include <ace/Condition_Thread_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <ace/Mutex.h>

#ifdef _DEBUG
	#pragma comment(lib,"openVideod.lib")
#else
	#pragma comment(lib,"openVideo.lib")
#endif


#include <openvideo/Node.h>
#include <openvideo/Manager.h>
#include <openvideo/State.h>
#include "gl/gl.h"


OTOpenVideoContext* OTOpenVideoContext::ovContext=NULL;

OTOpenVideoContext::OTOpenVideoContext()
{ 
	isOVContextStarted=false;
	contextLock=new ACE_Thread_Mutex();
	contextLockCond=new ACE_Condition_Thread_Mutex(*contextLock);
	ovManager=openvideo::Manager::getInstance(); 
}

OTOpenVideoContext::~OTOpenVideoContext()
{
	delete ovManager;
	delete contextLockCond;
	delete contextLock;
}

OTOpenVideoContext* OTOpenVideoContext::getInstance()
{
	if(!ovContext)
		ovContext=new OTOpenVideoContext();
	return ovContext;
}

openvideo::Node* OTOpenVideoContext::getOpenVideoNode(std::string name)
{
	if(!ovContext) return NULL;
	return (ovContext->ovManager)->getNode(name);
}

bool
OTOpenVideoContext::startOpenVideo(const char* _ovConfigName)
{
	if(isOVContextStarted)
		return true;
	ovConfigName=_ovConfigName;
	if (strlen(_ovConfigName)==0)
	{
		printf("WARNING: No config file provided for OpenVideo\n");
		return false;
	}

	// We lock till the manager has fully started
	contextLock->acquire();
	ACE_hthread_t* threadHandle = new ACE_hthread_t();
	if(ACE_Thread::spawn((ACE_THR_FUNC)OTOpenVideoContext::mainOVLoop,
		0, 	
		THR_NEW_LWP|THR_JOINABLE, 	
		0, 	
		threadHandle,
		0, 	
		0, 	
		0
		)==-1)
	{ 
		printf("ERROR: OpenVideo thread could not be spawned\n");
	}
	isOVContextStarted=true;

	//wait till the manager is started
	contextLockCond->wait();
	contextLock->release();

	return true;
}

void*
OTOpenVideoContext::mainOVLoop(void*)
{
	OTOpenVideoContext *self=OTOpenVideoContext::getInstance();

	// Parse the config file
	if(!self->ovManager->parseConfiguration(self->ovConfigName))
		return 0;

	// Initialize the traversal
	self->ovManager->initTraversal();

	// Notify that we are done
	self->contextLock->acquire();
	self->contextLockCond->broadcast();
	self->contextLock->release();

	// Go!
	ovContext->ovManager->run();

	return 0;
}


#endif //USE_ARTOOLKITPLUS

/* ========================================================================
 * End of file
 * ========================================================================
 * Local Variables:
 * mode: c++
 * c-basic-offset: 4
 * eval: (c-set-offset 'substatement-open 0)
 * eval: (c-set-offset 'case-label '+)
 * eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 * eval: (setq indent-tabs-mode nil)
 * End:
 * ========================================================================
 */
