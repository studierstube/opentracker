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
/** source file for Group Gate Node
  *
  * @author Michael Knapp
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/GroupGateNode.cxx,v 1.2 2003/01/09 04:14:12 tamer Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "GroupGateNode.h"

#include <cstdio>
#include <iostream>

using namespace std;

// constructor method
GroupGateNode::GroupGateNode(const char *name, GroupGateGroup *owner)
{
    Number = -1;
    Name = name;
    Owner = owner;
    IsActive = false;
} 

GroupGateNode::~GroupGateNode()
{
    Neighbors.clear();
} 

void
GroupGateNode::setNumber(int num)
{
    Number = num;
}

int
GroupGateNode::getNumber()
{
    return Number;
}

void 
GroupGateNode::addNeighbor(const char *neighbor)
{
    Neighbors.push_back(neighbor);
cout << "Neighbor: " << neighbor << endl;
}

bool
GroupGateNode::isActive()
{
    return IsActive;
}

void
GroupGateNode::activate()
{
    IsActive = true;
}

void
GroupGateNode::deactivate()
{
    IsActive = false;
}

const char *
GroupGateNode::getGroupGateName()
{
    return (Name.c_str());
}

void 
GroupGateNode::onEventGenerated(State &event, Node &generator)
{
	if (generator.isNodePort() == 1)
	{
		NodePort &wrap = (NodePort &)generator;
		if (wrap.getType().compare("Override") == 0)
        {
            Owner->deactivateAll();
            activate();
            Owner->setActiveGroupGate(this);
            Owner->notifyActiveGate();
   		    updateObservers(event);
            return;
        }
    }
    if (isActive())
    {
        activate();
        Owner->notifyActiveGate();
      	updateObservers(event);
        return;
    }
    else
    {
        for (NeighborsVector::iterator it = Neighbors.begin(); it != Neighbors.end(); it++)
        {
            GroupGateNode *node = (GroupGateNode *)(Owner->getNode((* it).c_str()));
            if (node->isActive())
            {
                node->deactivate();
                activate();
                Owner->setActiveGroupGate(this);
                Owner->notifyActiveGate();
        		updateObservers(event);
                return;
            }
        }
    }
}
