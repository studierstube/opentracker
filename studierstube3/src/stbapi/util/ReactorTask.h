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
/** header file for ReactorTask class
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ReactorTask.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */


#ifndef __REACTORTASK_H_ 
#define __REACTORTASK_H_

#include <ace/Task.h>
#include <ace/Reactor.h>
#include <stbapi/stbapi.h>

/**
 * This is a utility class to run an ACE_Reactor instance from a dedicated
 * thread. It is used by the DIV reliable multicast protocol to handle all
 * network communication in a dedicated thread. However it could also be used
 * run other reactors as well. The implemented methods exist to correctly
 * implement this class in the ACE framework.
 *
 * @author Gerhard Reitmayr
 * @ingroup div
 */
class STBAPI_API ReactorTask : public ACE_Task<ACE_MT_SYNCH>  
{
protected:
    ACE_Reactor reactor;

public:
	ReactorTask(bool start = false);

	virtual ~ReactorTask();

    virtual int open (void *args=0);

    virtual int close (u_long flags=0);

    virtual int svc (void);
    
    ACE_Reactor * getReactor();
};

#endif 
