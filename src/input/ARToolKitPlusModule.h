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
/** header file for ARToolKitPlusModule module.
  *
  * @author Daniel Wagner
  *
  * $Header$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section artoolkitplusmodule ARToolKitPlusModule
 */


#ifndef _ARTOOLKITMODULEPLUS_H
#define _ARTOOLKITMODULEPLUS_H

#include "../dllinclude.h"

#include <vector>

#include "../OpenTracker.h"

#ifdef USE_ARTOOLKITPLUS

#include <ARToolKitPlus/TrackerSingleMarker.h>


namespace ot {

typedef std::vector<Node*> NodeVector;


class ImageGrabber {
public:
	enum FORMAT {
		RGBX8888,
		LUM8
	};

	virtual bool grab(const unsigned char*& nImage, int& nSizeX, int& nSizeY, FORMAT& nFormat) = 0;
};


/**
 * The module and factory to drive the test source nodes. It constructs
 * TestSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

class OPENTRACKER_API ARToolKitPlusModule : public Module, public NodeFactory, ARToolKitPlus::Logger
{
// Members
protected:
    /// list of TestSource nodes in the tree
    NodeVector sources;

	/// stores the list of all markers that were visible during the last update
	NodeVector visibleMarkers;

    /// treshhold value to use in image processing
    //int treshhold;

    /// file name of cameradata file
    std::string cameradata;

    /// pointer to the buffer map
    //unsigned char * frame;
    /// size of the image in pixels
    int sizeX, sizeY;

	/// should ARToolKitPPCModule flip the image in x- or y-direction?
	bool flipX, flipY;

	/// an optional prefix for pattern filenames
	std::string patternDirectory;

	/// an optional camera device name
	std::string cameraDeviceHint;

	void init(StringTable& attributes, ConfigNode * localTree);

	bool updateARToolKit();

	ImageGrabber* imageGrabber;

	float trackerNear,trackerFar;
	ARToolKitPlus::TrackerSingleMarker tracker;
	bool idbasedMarkers;

	// implement ARToolKitPlus::Logger
	virtual void artLog(const char* nStr);
	virtual void artLogEx(const char* nStr, ...);

// Methods
public:
    /** constructor method. */
    ARToolKitPlusModule();

    /** Destructor method, clears nodes member. */
    virtual ~ARToolKitPlusModule();

    /** This method is called to ruct a new Node. It compares
     * name to the TestSource element name, and if it matches
     * creates a new TestSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);
    /**
     * pushes events into the tracker tree. Checks all TestSources and
     * pushes new events, if a TestSource fires.
     */
    virtual void pushState();

	/**
	 * queries the camera and pushes data into the tree
	 */
	void update();

    /** returns the width of the grabbed image in pixels */
    int getSizeX() const  {  return sizeX;  }

    /** returns the height of the grabbed image in pixels */
    int getSizeY() const  {  return sizeY;  }

    /** returns whether the grabbed image is flipped horizontally
	  * or vertically */
    void getFlipping(bool* isFlippedH, bool* isFlippedV)  {  *isFlippedH = flipX;  *isFlippedV = flipY;  }

	void registerImageGrabber(ImageGrabber* nGrabber)  {  imageGrabber = nGrabber;  }

	ARToolKitPlus::TrackerSingleMarker* getARToolKitPlus()  {  return &tracker;  }

	/// Sets a camera device name
	/**
	 *  This device name can be used to select between several
	 *  available camera files. This must be done before the 
	 *  ARToolKitPlus::init() is called.
	 *  If no camera hint is set or the config file does not contain
	 *  any <CameraHint> sections, the standard 'camera-parameter' value
	 *  is used.
	 */
	void setCameraDeviceHint(const char* nDeviceName)  {  cameraDeviceHint = nDeviceName;  }

	NodeVector& getVisibleMarkers()  {  return visibleMarkers;  }
};

} // namespace ot

#endif //#ifdef USE_ARTOOLKITPLUS

#endif // _ARTOOLKITMODULEPLUS_H

