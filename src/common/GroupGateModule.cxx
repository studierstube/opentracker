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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Group Gate Module
  *
  * @author Michael Knapp
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/GroupGateModule.cxx,v 1.5 2003/07/18 20:26:46 tamer Exp $
  *
  * @file                                                                    */
 /* ======================================================================= */

#include "GroupGateModule.h"
#include "GroupGateNode.h"

#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

GroupGateGroup::GroupGateGroup(const char *name)
{
    Name = name;
    ActiveGate = 0;
    ActiveGroupGate = 0;
}

GroupGateGroup::~GroupGateGroup()
{
    Gates.clear();
}

Node *
GroupGateGroup::addNode(const char *name)
{
    GroupGateNode *node = new GroupGateNode(name, this);
    Gates.insert(NodeMap::value_type(name, node));
    return node;
}

Node *
GroupGateGroup::getNode(const char *name)
{
    NodeMap::iterator find = Gates.find(name);
    if (find != Gates.end()) return (GroupGateNode *)((* find).second);
    return 0;
}

Node * 
GroupGateGroup::setActiveGate(bool enable)
{
    if (ActiveGate != 0) delete ActiveGate;
    ActiveGate = 0;
    if (enable) ActiveGate = new ActiveGateNode();
    return ActiveGate;
}

void 
GroupGateGroup::deactivateAll()
{
    for (NodeMap::iterator it = Gates.begin(); it != Gates.end(); it++)
    {
        ((GroupGateNode *)((* it).second))->deactivate();
    }
}

void 
GroupGateGroup::setActiveGroupGate(Node *activegroupgate)
{
    ActiveGroupGate = activegroupgate;
}

void 
GroupGateGroup::notifyActiveGate()
{
    if ((ActiveGate != 0) && (ActiveGroupGate != 0))
    {
        GroupGateNode *node = (GroupGateNode *)ActiveGroupGate;
        ((ActiveGateNode *)ActiveGate)->pushState(node->getNumber());
    }
}

//-----------------------------------------------------------------------------

// Destructor method
GroupGateModule::~GroupGateModule()
{
    Groups.clear();
    NameList.clear();
}

// This method is called to construct a new Node.
Node *
GroupGateModule::createNode(const string &name, StringTable &attributes)
{
    if(name.compare("GroupGate") == 0)
    {
        const char *cname = attributes.get("DEF").c_str();
        const char *cgroup = attributes.get("group").c_str();
        const char *cneighbors = attributes.get("neighbors").c_str();

        GroupGateGroup *group = 0;
        GroupMap::iterator find = Groups.find(cgroup);
        if (find == Groups.end()) 
        {
            group = new GroupGateGroup(cgroup);
            Groups.insert(GroupMap::value_type(cgroup, group));
        }
        else
        {
            group = (* find).second;
        }
        GroupGateNode *node = (GroupGateNode *)(group->addNode(cname));    
        node->setNumber(NameList.size());
        NameList.push_back(cname);

        stringstream neighborstream;
        neighborstream << cneighbors;
        while (true)
        {
            string neighbor;
            neighborstream >> neighbor;
            if (neighbor == "") break;
            node->addNeighbor(neighbor.c_str());
        }
        return node;
    }
    else if (name.compare("ActiveGate") == 0)
    {
        Node *node = 0;
        const char *cgroup = attributes.get("group").c_str();
        GroupMap::iterator find = Groups.find(cgroup);
        if (find != Groups.end()) 
        {
cout << "*** ActiveGroup: " << cgroup << endl;
            node =  (* find).second->setActiveGate(true);
        }
        return node;
    }
    else if (name.compare("Override") == 0)
    {
        Override *node = new Override();
        return node;
    }
    return 0;
}

const char *
GroupGateModule::getGroupGateName(int groupgatenum)
{
    return (NameList[groupgatenum].c_str());
}

void 
GroupGateModule::activateGroupGate(const char *groupname, const char *groupgatename)
{
    GroupMap::iterator find = Groups.find(groupname);
    if (find == Groups.end()) return;
    GroupGateGroup *group = (*find).second;
    if (group == 0) return;
    GroupGateNode *node = (GroupGateNode *)(group->getNode(groupgatename));
    if (node == 0) return;
    group->deactivateAll();
    node->activate();
}
