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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ARToolKitModule.h,v 1.20 2002/01/12 13:49:53 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section artoolkitmodule ARToolKitModule
 * The ARToolKitModule interfaces to the ARToolKit library to track the 
 * position and orientation of visual markers with a video camera. It uses
 * the information provided by @ref artoolkitsource nodes and inserts the state
 * events through @ref artoolkitsource nodes into the tree. The configuration 
 * element is @c ARToolKitConfig and has the following attributes :
 * @li @c camera-parameter file containing camera calibration data
 * @li @c treshhold 100 bounding value for treshholding the greyscale video
 *        image
 * @li @c framerate 10 a desired maximum framerate, the module will not exceed it.
 * @li @c videomode a string selecting the videomode for ARToolKit. This depends on 
 *        video interface linked with ARToolKit.
 *
 * An example configuration element looks like this :
 * @verbatim
<ARToolKitConfig camera-parameter="../data/camera" treshhold="150" framerate="5" videomode="0,5,3"/>@endverbatim
 */

/**
 * @defgroup input Device Input Classes
 * This group contains modules and nodes implementing device drivers for
 * OpenTracker. It is expected to grow in the future to accomodate a
 * wide range of devices.
 */

#ifndef _ARTOOLKITMODULE_H
#define _ARTOOLKITMODULE_H

#include "../OpenTracker.h"

#ifdef USE_ARTOOLKIT

#ifdef WIN32
class ARFrameGrabber;
#endif

/**
 * A video tracking source module using the ARToolKit library to track a 
 * number of markers in a video image. It sets up the video library and acts
 * as a NodeFactory for ARToolKitSource nodes. It also keeps a vetor of 
 * the created nodes to update them. It is implemented as a threaded module
 * doing the grabbing and video processing in a separate thread.
 *
 * Moreover it provides a programmatic interface to get the current video image.
 * It returns information about the used format and size and a pointer to
 * the image data.
 * @ingroup input
 */
class OPENTRACKER_API ARToolKitModule : public ThreadModule, public NodeFactory
{
// members
protected:
    /// stores the sources
    NodeVector sources;
    int did;
    /// treshhold value to use in image processing
    int treshhold;
    /// flag to stop image processing thread
    int stop;
    /// file name of cameradata file
    std::string cameradata;
    /// desired maximal framerate
    double rate;
    /// videomode string
    std::string videomode;

    /// pointer to the buffer map
    unsigned char * frame;
    /// size of the image in pixels
    int sizeX, sizeY;

#ifdef WIN32
    ARFrameGrabber * camera;
#endif

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
    ARToolKitModule();

    /// destructor method
    virtual ~ARToolKitModule();

    /** This method is called to construct a new Node. It compares
     * name to the ARToolKitSource element name, and if it matches
     * creates a new ARToolKitSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name, StringTable& attributes);

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
    virtual void init(StringTable& attributes, ConfigNode * localTree);

    /** returns the width of the grabbed image in pixels */
    int getSizeX();

    /** returns the height of the grabbed image in pixels */
    int getSizeY();

    /** returns a pointer to the grabbed image. The image format
     * is depending on the pixel format and typically RGB or RGBA 
     * times X times Y bytes. 
     * @return pointer to image buffer */
    unsigned char * getFrame();

    /** 
     * returns the OpenGL flag that is used by ARToolkit to describe
     * the pixel format in the frame buffer. This is a simple way to 
     * pass the necessary information to use the image data in GL calls. */
    int getImageFormat();
};

#endif

#endif
