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
/** header file for ARToolKit interface module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ARToolKitModule.h,v 1.10 2001/04/12 06:42:34 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section artoolkit ARToolKitModule
 * The ARToolKitModule interfaces to the ARToolKit library to track the 
 * position and orientation of visual markers with a video camera. It uses
 * the information provided by ARToolKitSource nodes and inserts the state
 * events through the ARToolKitSource nodes into the tree. It has the 
 * following attributes :
 * @li @c camera-parameter file containing camera calibration data
 * @li @c treshhold 100 bounding value for treshholding the greyscale video
 *        image
 *
 * An example configuration element looks like this :
 * @verbatim
<ARToolKitConfig camera-parameter="../data/camera" treshhold="150"/>@endverbatim
 */


#ifndef _ARTOOLKITMODULE_H
#define _ARTOOLKITMODULE_H

#include "../OpenTracker.h"

#ifdef USE_ARTOOLKIT

/**
 * A video tracking source module using the ARToolKit library to track a 
 * number of markers in a video image. It sets up the video library and acts
 * as a NodeFactory for ARToolKitSource nodes. It also keeps a vetor of 
 * the created nodes to update them. It is implemented as a threaded module
 * doing the grabbing and video processing in a separate thread.
 */
class OPENTRACKER_API ARToolKitModule : public ThreadModule, public NodeFactory
{
// members
protected:
    NodeVector sources;
    int did;
    int treshhold;
    int stop;
    string cameradata;

// methods
protected:
    /** the work method for the module thread. This method grabs a frame
     * and computes any marker information from it. Then it updates the
     * source nodes accordingly. If the stop flag is set, it finishes.*/
    virtual void run(); 

    /**
     * grabs a frame and processes the data */
    void grab();

public:
    /// constructor method
    ARToolKitModule() : ThreadModule(), NodeFactory(), treshhold(100), stop(0)
    {};
    /// destructor method
    virtual ~ARToolKitModule();

    /** This method is called to construct a new Node. It compares
     * name to the ARToolKitSource element name, and if it matches
     * creates a new ARToolKitSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name, StringTable& attributes);

    /**
     * This method is called after initialisation is finished and before the
     * main loop is started. It opens the artoolkit library and video system.*/
    virtual void start();

    /** closes the artoolkit library */
    virtual void close();

    /**
     * pushes events into the tracker tree. Checks all source nodes for
     * new states and pushes them into the tracker tree.
     */
    virtual void pushState();

    /**
     * initializes the ARToolKit module. 
     * @param attributes StringMap of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, Node * localTree);

};

#endif

#endif
