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
/** header file for Group Gate Module
  *
  * @author Michael Knapp
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/GroupGateModule.h,v 1.2 2003/04/28 13:35:53 knapp Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */
/**
 * @page module_ref Module Reference
 * @section groupgatemodule GroupGateModule 
 *
 * This module provides and drives @ref groupgate nodes that 
 * generate standard events. The configuration element is called @c GroupGateConfig 
 * and has no attributes yet.
 *
 * An example configuration element looks like this :
 * @verbatim
<GroupGateConfig/>@endverbatim
 *
 */

#ifndef _GROUPGATEMODULE_H
#define _GROUPGATEMODULE_H

#include "../OpenTracker.h"

#include <string>
#include <vector>

class GroupGateModule;

typedef std::vector<std::string> StringVec;
typedef std::map<std::string, Node *> NodeMap;

class OPENTRACKER_API GroupGateGroup
{
protected:
    std::string Name;
    NodeMap Gates;
    Node *ActiveGate;
    Node *ActiveGroupGate;

public:
    GroupGateGroup(const char *name);
    ~GroupGateGroup();

    Node *addNode(const char *name);
    Node *getNode(const char *name);
    Node *setActiveGate(bool enable);
    
    void deactivateAll();
    void setActiveGroupGate(Node *activegroupgate);
    void notifyActiveGate();
};

typedef std::map<std::string, GroupGateGroup *> GroupMap;

/**
 * The module and factory to drive the test source nodes. It constructs
 * GroupGate nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Michael Knapp
 * @ingroup common
 */
class OPENTRACKER_API GroupGateModule : public Module, public NodeFactory
{
protected:
    GroupMap Groups;
    StringVec NameList;

public:
    GroupGateModule() : Module(), NodeFactory() { };
    virtual ~GroupGateModule();
    virtual Node *createNode(const std::string &name, StringTable &attributes);

    const char *getGroupGateName(int groupgatenum);
    void activateGroupGate(const char *groupname, const char *groupgatename);
};

#endif
