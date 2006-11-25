/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
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
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institut for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** header file for PhantomMiddlewareSink Node.
  *
  * @author Joseph Newman
  *
  * $Id: PhantomMiddlewareSink.h 760 2004-11-14 18:20:34Z jfn $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filesink PhantomMiddlewareSink 
 * The PhantomMiddlewareSink node is an object that 
 * takes an OpenTracker event.
 * It has the following attributes :
 * @li @c group the group to send multicast Phantom events
 * @li @c pid the unique phantom id of the object
 * @li @c frequency the frequency at which OT events 
 * are converted to Phantom events
 *
 * An example element looks like this :
 * @verbatim
<PhantomMiddlewareSink group="225.0.0.12" pid="147" frequency="1">
@endverbatim
 */

#ifndef _PHANTOMMIDDLEWARESOURCE_H
#define _PHANTOMMIDDLEWARESOURCE_H

#include <cmath>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/tool/OT_ACE_Log.h>
#include <ace/Thread_Mutex.h>
#include <phantom/MulticastSocket.hh>
#include <phantom/PhantomMessageV1.hh>

/**
 * This class implements a simple EventGenerator that passes any incoming events
 * on to the associated CORBA object.
 * @author Joseph Newman
 * @ingroup core
 */


namespace ot {

class PhantomMiddlewareModule;

class OPENTRACKER_API PhantomMiddlewareSource : public Node
{
// Members
public:
  /// Phantom Node (sink) object associated with this node.
  /// the original event
  Event event;

  int pid;
  short eid;
  std::string source;
  //Phantom::Utils::MulticastSocketReceiver* msr;
    

  // Methods
protected:
    /** constructor method,sets commend member
     * @param multicast_group_ the multicast group on which the node should
     * broadcast events */
  PhantomMiddlewareSource( const char* multicast_group, int pid_, short eid_, const std::string& source_) :
    Node(), 
    pid( pid_ ), 
    eid( eid_ ),
    source( source_ )
      {
	mu = new ACE_Thread_Mutex("phantommiddlewaresource");
      }

    
    PhantomMiddlewareSource( const char* multicast_group, int pid_, short eid_) :
    Node(), 
    pid( pid_ ), 
    eid( eid_ )
      {
	mu = new ACE_Thread_Mutex("phantommiddlewaresource");
	source = "";
      }
      

    virtual ~PhantomMiddlewareSource() {
      //delete msr;
      delete mu;
      logPrintE("PhantomMiddlewareSource destructor\n");
    }
private:
    ACE_Thread_Mutex* mu;
    bool modified;
    
public:
    void lock() { mu->acquire(); };
    void unlock() { mu->release(); };
    bool isModified() {return modified;};
    void setEvent(float x, float y, float z, float theta, int t1, int t2, short event_id, char* event_source) {
      //      if ( (event_id == eid) && strcmp(event_source, source) ) {
      setEvent(x, y, z, theta, t1, t2, event_id);
      //      }
    };

    void setEvent(float x, float y, float z, float alpha, int t1, int t2, short event_id) {
      //      if ( (event_id == eid) && strcmp(event_source, source) ) {
	std::vector<float> position(3);
	std::vector<float> orientation(4);
	position[0] = x; position[1] =  z; position[2] = -y;
	float theta    = 180.0f - alpha; 
	float thetaby2 = theta * MathUtils::GradToRad / 2.0;
	orientation[0] = 0.0;
	orientation[1] = sin(thetaby2);
	orientation[2] = 0.0; 
	orientation[3] = cos(thetaby2);
	lock();
	event.setPosition(position);
	event.setOrientation(orientation);
	event.time = (double) t1 + ((double) t2)/1000000.0;
	modified=true;
	unlock();
	//      }
    };


    int getEventId() {
      return eid;
    }

    void push()
    {
      lock();
      if (modified) {
	updateObservers( event );
	modified = false;
      } 
      unlock();
    }
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
      return 1;
    }
    
    friend class PhantomMiddlewareModule;
};

}

#endif
