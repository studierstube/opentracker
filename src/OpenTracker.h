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
/** Common include file for OpenTracker classes.
  * This file declares and includes the central classes of the
  * OpenTracker framework. It is typically needed in all header files.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/OpenTracker.h,v 1.7 2001/03/27 06:08:50 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _OPENTRACKER_H
#define _OPENTRACKER_H

/**
 * @mainpage OpenTracker
 *
 * OpenTracker is a new hardware and event processing software for
 * VR and AR applications. It is designed to be extensible and
 * easily configureable.
 * 
 * Its functionality is divided into modules that implement specific functions
 * like device drivers, network interfaces etc. This simplifies adding new
 * functionality. For a list of currently implemented modules see the 
 * @ref modules page.
 *
 * Modules exchange data via a shared memory that is structured into a tree.
 * Different nodes of the tree are managed and observed by different modules.
 * The nodes exchange data via three interfaces :
 * @li EventGenerator is the interface of a node sending state updates ( events )
 * @li EventQueue is the interface of a node holding a queue of past events
 * @li TimeDependend is the interface of a node providing a state for any moment
 *     in time. 
 *
 * A node may implement several interfaces. However parent child relationships
 * are only allowed betweem nodes that understand each other. This is specified
 * in the configuration file format DTD. For a list currently implemented nodes
 * see the @ref Nodes page.
 *
 * There is more information about how to extend the framework. See  
 * @ref module, which explains how to implement a new module. 
 *
 * @author Gerhard Reitmayr
 * @date 2000
 */

#include "../dllinclude.h"

// ACE needs to be included before any Windows headers, because there are 
// some problems with Windows.h file.
// #include <ace/ACE.h>

#include <string>
#include <vector>
#include <map>
#include <deque>

using namespace std;

#include "core/ThreadModule.h"
#include "core/Node.h"
#include "core/NodeFactory.h"
#include "core/Context.h"
#include "core/MathUtils.h"

// Some types used throughout OpenTracker
// some forward declarations to be able to declare containers
class Node;

/**
 * a Vector of Node pointers. Very useful to implement a simple
 * container of Nodes such as a parent node, or to keep pointers
 * to several nodes around.
 */
typedef vector<Node *> NodeVector;

/**
 * maps a string to another string. Mostly used to map element attributes
 * to values.
 */
typedef map<string, string> StringMap;

/** initializes a context by instantiating the available modules and factories
 * and registering them with the given context. So for each passed context 
 * there is a new set of objects instantiated. Any compile time definitions 
 * go in here, to define which modules are compiled in and can be instantiated.
 * Also the modules configuration element names are defined here.
 *
 * @param context reference of the context to be initialized.
 */
void OPENTRACKER_API initializeContext( Context & context );

#endif
