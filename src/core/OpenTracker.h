 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Common include file for OpenTracker classes.
  * This file declares and includes the central classes of the
  * OpenTracker framework. It is typically needed in all header files.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/OpenTracker.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _OPENTRACKER_H
#define _OPENTRACKER_H

/**
 * \mainpage OpenTracker
 *
 * OpenTracker is a new hardware and event processing software for
 * VR and AR applications. It is designed to be extensible and
 * easily configureable.
 *
 * \author Gerhard Reitmayr
 * \date 2000
 */

#include <string>
#include <vector>
#include <map>
#include <deque>

using namespace std;

#include "Node.h"
#include "EventGenerator.h"
#include "EventQueue.h"
#include "TimeDependend.h"

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

#endif
