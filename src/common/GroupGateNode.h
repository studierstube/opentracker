 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Group Gate Node
  *
  * @author Michael Knapp
  *
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */
/**
  * @page Nodes Node Reference
  * @section groupgate GroupGate
  * 
  * The GroupGate element is a node within a adjacency graph of other nodes
  * within a group. Per default all nodes are deactived. The Override element
  * has to be used to activate at least one node. This node is managed by a 
  * dedicated @ref groupgatemodule.
  *
  * A GroupGate only generates an event if the node itself or its neighbors
  * are active.
  *
  * The @c GroupGate element has the following attributes :
  *
  * @li @c DEF defines the name of the node. This name must not be used more 
  *        than once within a group
  * @li @c group defines the group to which the node is added
  *        if the group does not exist, a new one is created
  * @li @c neighbors defines the neighbors of the node within a group
  * 
  * The @c Override element is used to activate a node within a group. See
  * also the @ref activegate element that outputs the currently active
  * GroupGate of a given group. 
  *
  * example: elements for a cyclic three noded graph looks like this 
  * @verbatim
<GroupGate DEF="Node1" group="CyclicGraph" neighbors="Node3">
   <Override>
       <...incomming events...>
   </Override>
   <...incomming events...>
</GroupGate>
<GroupGate DEF="Node2" group="CyclicGraph" neighbors="Node1">
   <...incomming events...>
</GroupGate>
<GroupGate DEF="Node3" group="CyclicGraph" neighbors="Node2">
   <...incomming events...>
</GroupGate>
<ActiveGate group="CyclicGraph"/>@endverbatim
  *
  *
  * @section activegate ActiveGate
  *
  * The @c ActiveGate node is used to determine the currently active @ref groupgate
  * of a group.
  * It returns a numeric value in the button field of the event structure
  * The value is used to call the getGroupGateName() method of @ref groupgatemodule
  * to return the name of the @ref groupgate. See the @ref groupgate element for an
  * example of the ActiveGate element.
  * It has the following attributes :
  *
  * @li @c group defines the group of which the active node should be traced.
  */

#ifndef _GROUPGATENODE_H
#define _GROUPGATENODE_H

#include "../OpenTracker.h"
#include "GroupGateModule.h"

#include <string>
#include <vector>

namespace ot {

typedef std::vector<std::string> NeighborsVector;
typedef std::vector<Node *> NeighborPtrsVector;

/**
 * A GroupGateNode is an EventGenerator node that ... TO DO
 * @author Michael Knapp
 * @ingroup common
 */
class OPENTRACKER_API GroupGateNode : public Node   
{
private:
    int Number;
    GroupGateGroup *Owner;
    std::string Name;
    NeighborsVector Neighbors;
    NeighborPtrsVector NeighborPtrs;
    bool IsActive;

protected:
   
    /** constructor method. It sets initial values for the timeout. */
    GroupGateNode(const char *name, GroupGateGroup *owner);
    ~GroupGateNode();

    void setNumber(int num);
    int getNumber();
    void addNeighbor(const char *neighbor);
    bool isActive();
    void activate();
    void deactivate();
    const char *getGroupGateName();

public:
    virtual int isEventGenerator()
    {
    	return 1;
    }

    virtual void onEventGenerated(State &event, Node &generator);                                

    friend class GroupGateModule;
    friend class GroupGateGroup;
};


class OPENTRACKER_API Override : public Node
{
protected:
    Override() : Node() 
    { 
    }

public:

    virtual int isNodePort()
    {
        return 1;
    }

    virtual void onEventGenerated(State &event, Node &generator)
	{
		updateObservers(event);
	}

    friend class GroupGateModule;
};

/**
 * An ActiveGateNode is an EventGenerator node that ... TO DO
 * @author Michael Knapp
 * @ingroup common
 */
class OPENTRACKER_API ActiveGateNode : public Node   
{
private:
    State state;

protected:
   
    ActiveGateNode() : Node()
    {
    }
  
public:

    virtual int isEventGenerator()
    {
    	return 1;
    }

    void pushState(unsigned short groupgatenum)
    {
        state.button = groupgatenum;
        state.timeStamp();
		updateObservers(state);
    }

    friend class GroupGateModule;
    friend class GroupGateGroup;
};

} // namespace ot

#endif
