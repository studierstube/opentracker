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
/** header file for P5Glove module.
  *
  * @author Hannes Kaufmann, Istvan Barakonyi
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/P5GloveModule.h,v 1.2 2003/03/22 15:44:47 kaufmann Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section p5glovemodule P5GloveModule
 * The P5GloveModule provides and drives @ref p5glovesource nodes 
 */

#ifndef _P5GLOVEMODULE_H
#define _P5GLOVEMODULE_H

#include "../OpenTracker.h"

#ifdef USE_P5GLOVE

#include "P5dll.h"

/**
 * The module and factory to drive the P5GloveSource nodes. It constructs
 * P5GloveSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Hannes Kaufmann, Istvan Barakonyi
 * @ingroup input
 */
class OPENTRACKER_API P5GloveModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of P5GloveSource nodes in the tree
    NodeVector nodes;

// Methods
public:
    CP5DLL *P5device;
    int P5Id;

    #define P5MOTION_XYZFILTERSIZE	5
    #define P5MOTION_YPRFILTERSIZE	18

    #define P5MOTION_YPRABSOLUTE	0
    #define P5MOTION_YPRRELATIVE	1

    #define	P5MOTION_NORMALAXIS		1
    #define	P5MOTION_INVERTAXIS		-1

    int nXPos, nYPos, nZPos;
    float fXMickey, fYMickey, fZMickey;
    float fAbsYawPos, fAbsPitchPos, fAbsRollPos;
    float fRelYawPos, fRelPitchPos, fRelRollPos;
    float fFilterX, fFilterY, fFilterZ;

    /** constructor method. */
    P5GloveModule();

    /** Destructor method, clears nodes member. */
    virtual ~P5GloveModule();
	/**
     * initializes the P5Glove module
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes,  ConfigNode * localTree);
    /** This method is called to construct a new Node. It compares
     * name to the P5GloveSource element name, and if it matches
     * creates a new P5GloveSource node.
     * @param name reference to string containing element name
     * @attributes reference to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);
	/**
     * closes P5Glove */
    virtual void close();
    /**
	  * starts P5Glove
      */
    virtual void start();
    /**
     * pushes events into the tracker tree. Checks all P5GloveSources and
     * pushes new events, if a P5GloveSource fires. The events store
	 * structure with position and status of the buttons.
     */
    virtual void pushState();

    void P5Motion_SetClipRegion(int xstart, int xend, int ystart, int yend, int zstart, int zend);
    void P5Motion_InvertMouse (int xaxis, int yaxis, int zaxis);

    void P5Motion_Process();
    void P5Motion_FilterXYZ();
    void P5Motion_FilterYPR();

};

#endif

#endif
