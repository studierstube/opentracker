/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** The header file for the RefNode class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section refnode Ref
 * A Ref element allows to use the output of a node in another place in the
 * data flow graph. Every element can have a DEF attribute specified that
 * contains a unique id. This id can then be referenced in a Ref element
 * specifing the node the Ref element should mimik. The Ref node will then
 * resemble the referenced node in any way. 
 *
 * However it cannot have any children, as the referenced node would have no
 * information about these children. The Ref node is simply a way to reuse
 * the output of a node somewhere else. It has the following attribute :
 * @li @c USE unique id of the referenced node, this id must exist !
 *
 * An example element looks like this :
 * @verbatim
 <Ref USE="id"/>@endverbatim
*/

#ifndef _REFNODE_H
#define _REFNODE_H

#include "../dllinclude.h"

#include "Node.h"

/**
 * A Reference Node contains the reference to another node and its subtree.
 * It can only occur as a leaf node. However it reflects the interface of
 * the referenced node and also updates it's parents, whenever the referenced
 * node generates an event.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

namespace ot {

    class OPENTRACKER_API RefNode : public Node
    {

    protected:
	/** pointer to the referenced node */
	Node * reference;
 
        /**
         * basic constructor.
         */
        RefNode( Node * ref ) :
            Node(),
            reference( ref )		
            {
		reference->addReference( this );  
            }

    public:   
        /**
         * basic destructor.
         */
        virtual ~RefNode()
        {
            //reference->removeReference( this );
	}
	
        /** returns the referenced node
         * @returns pointer to the referenced node.
         */
        Node * getReferenced()
        {
            return reference;
        }
    
	// begin EventGenerator & EventObserver interface

	/**
         * this method notifies the object that a new event was generated.
         * It is called by an EventGenerator.
         * @param event reference to the new event. Do not change the
         *        event values, make a copy and change that !
         * @param generator reference to the EventGenerator object that
         *        notified the EventObserver.
         */
        virtual void onEventGenerated( Event& event,
                                       Node& generator)
	{
            updateObservers( event );
	}

        /**
         * tests for EventGenerator interface being implemented. This has to
         * be overriden in classes that subclass EventGenerator. Due to
         * inheritance constraints it cannot be done automatically.
         * @returns a pointer to the EventGenerator interface, or NULL if it
         *   is not implemented
         */
        virtual int isEventGenerator()
        {
            return reference->isEventGenerator();
        }

	// end EventGenerator interface

	// begin EventQueue interface

        /**
         * tests for EventQueue interface being present. This has to be overriden in
         * classes that subclass EventQueue. See isEventGenerator for details.
         * @returns a pointer to the EventQueue interface, or NULL if it is not
         *       implemented.
         */
        virtual int isEventQueue()
        {
            return reference->isEventQueue();
        }
	/**
         * returns a stored event by index. The index starts with 0 for the
         * latest event and goes back in time with increasing index.
         * @param index index of the desired event, if no index is given the
         *        latest is returned.
         * @return reference to the value of the event. */
        virtual Event& getEvent(unsigned int index = 0)
	{
            return reference->getEvent( index );
	}
        /**
         * returns the event closest to a given point in time.
         * @param time the point in time in milliseconds since 1.1.1970
         * @return reference to the value of the event. */
        virtual Event& getEventNearTime(double time)
	{
            return reference->getEventNearTime( time );
	}
        /**
         * returns the number of stored events.
         * @return number of stored events. */
        virtual unsigned int getSize()
	{
            return reference->getSize();
	}

	// end EventQueue interface

	// begin TimeDependend interface

        /**
         * tests for TimeDependend interface being present. This has to be overriden
         * in classes that subclass TimeDependend. See isEventGenerator for details.
         * @returns a pointer to the TimeDependend interface, or NULL if it is not
         *      implemented.
         */
        virtual int isTimeDependend()
        {
            return reference->isTimeDependend();
        }

	/**
         * returns the event of the time dependend function at a given
         * point in time.
         * @param time point in time in milliseconds since 1.1.1970
         * @return reference to the event value */
        virtual Event& getEventAtTime(double time)
	{
            return reference->getEventAtTime( time );
	}

	// end TimeDependend interface
        friend class ConfigurationParser;
#ifdef USE_LIVE
        friend class LiveContext;
#endif
    };

} // namespace ot {

#endif

/* 
 * ------------------------------------------------------------
 *   End of RefNode.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
