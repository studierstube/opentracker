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
  * $Id: OTOpenVideoContext.h 25 2005-11-28 16:11:59Z denis $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _OPENVIDEOCONTEXT_H_
#define _OPENVIDEOCONTEXT_H_

#include "../dllinclude.h"

#include "../OpenTracker.h"

#ifdef USE_ARTOOLKITPLUS

#ifdef WIN32
	#define _WIN32_WINNT  0x0400
	#define WIN32_LEAN_AND_MEAN
	
	#include "windows.h"
	#include "wingdi.h"

#endif

#include "openvideo/openVideo.h"
#include "openvideo/Node.h"
#include "openvideo/State.h"
#include "openvideo/Manager.h"
#include <string>

namespace openvideo{
	class Manager;
	class Node;
	class State;
}

class  ACE_Thread_Mutex;
class  ACE_Condition_Thread_Mutex;
class  ACE_Mutex;

/*
 * This class is in charge of starting the OpenVideo instance.
 */
class OPENTRACKER_API OTOpenVideoContext
{
public:
	/*
	 * This is a singleton and therefore need a getInstance() function.
	 */
	 static OTOpenVideoContext* getInstance();
	~ OTOpenVideoContext();

	/*
	 * This will parse the input config file and start OpenVideo
	 */
	bool startOpenVideo(const char* _ovConfigName);

	/*
	 * Function for Retrieval of Nodes
	 */
	openvideo::Node* getOpenVideoNode(std::string name);

protected:
	/*
	 * As a singleton the constructor must be protected.
	 */
	OTOpenVideoContext();
	static OTOpenVideoContext *ovContext;
	openvideo::Manager *ovManager;
	const char* ovConfigName;
	ACE_Thread_Mutex *contextLock;
	ACE_Condition_Thread_Mutex *contextLockCond;
	bool isOVContextStarted;

	/*
	 * This starts the main loop of the OpenVideo Manager
	 */
	static void* mainOVLoop(void*);
};

#endif //USE_ARTOOLKITPLUS

#endif //_OPENVIDEOCONTEXT_H_


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

