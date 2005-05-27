/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** ReactorTask implementation
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ReactorTask.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#define ACE_NLOGGING

#include "ReactorTask.h"

ReactorTask::ReactorTask(bool start)
{
	ACE_DEBUG((LM_DEBUG,"ReactorTask::constructor\n"));
	if( start ) 
		open();
}

ReactorTask::~ReactorTask()
{
	ACE_DEBUG((LM_DEBUG,"ReactorTask::destructor\n"));
	close();
};

int ReactorTask::open (void *args)
{
	ACE_DEBUG((LM_DEBUG,"ReactorTask::activate\n"));
	return activate();
}

int ReactorTask::close (u_long flags)
{
	ACE_DEBUG((LM_DEBUG,"ReactorTask::close\n"));
	reactor.end_reactor_event_loop();
	int ret =  wait();        
    ACE_DEBUG((LM_DEBUG,"ReactorTask::close end\n"));
    return ret;
}    

int ReactorTask::svc (void)
{
	ACE_DEBUG((LM_DEBUG,"ReactorTask::service\n"));
	reactor.owner( ACE_Thread::self());
	int ret = reactor.run_reactor_event_loop();
    ACE_DEBUG((LM_DEBUG,"ReactorTask::service end\n"));
    return ret;
}

ACE_Reactor * ReactorTask::getReactor()
{
	return & reactor;
}
