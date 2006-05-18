 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ARToolKitPlusModule module.
  *
  * @author Daniel Wagner
  * @author Erick Mendez
  *
  * $Header$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section artoolkitplusmodule ARToolKitPlusModule
 * The ARToolKitModule interfaces to the ARToolKit library to track the 
 * position and orientation of visual markers with a video camera. It uses
 * the information provided by @ref artoolkitplussource nodes and inserts the state
 * events through @ref artoolkitplussource nodes into the tree. The configuration 
 * element is @c ARToolKitConfig and has the following attributes :
 * @li @c camera-parameter: file containing camera calibration data
 * @li @c ov-config: file containing the openvideo configuration
 * @li @c treshold: either a numerical value (0-255) or 'auto' for automatic thresholding. Default is '100'
 * @li @c ov-sink: name of the openvideo sink
 * @li @c undist-mode: undistortion mode; one of 'none', 'lut' or 'std' (default)
 * @li @c detect-mode: marker detection mode (marker history); either 'lite' or 'std' (default)
 * @li @c pose-estimator: pose estimator selection; one of the following 'std' (default), 'cont' (better version of std) or 'rpp' (robust pose estimator)
 * @li @c marker-mode: one of the following: 'template' (default), 'idbased' or 'bch'
 * @li @c border-width: width of the border; default value is '0.250'
 * @li @c pattern-dir an optional string that is prefixed to any pattern filename or
 *        the camera data filename. It tries to find the file under the original as
 *        well the prefixed name, in that order.
 *
 * An example configuration element looks like this :
 * @verbatim
 <ARToolKitPlusConfig camera-parameter="quickcampro400.dat" marker-mode="idbased" border-width="0.125" treshold="auto" pose-estimator="cont" ov-config="openvideo.xml" ov-sink="artoolkitPluSink"/>
 */


#ifndef _ARTOOLKITMODULEPLUS_H
#define _ARTOOLKITMODULEPLUS_H

#include "../dllinclude.h"

#include <vector>
#include <map>

#include "../OpenTracker.h"

#ifdef USE_ARTOOLKITPLUS

#include <openvideo/configOV.h>
#include <openvideo/State.h>
#include <openvideo/nodes/VideoSink.h>
#include <openvideo/nodes/VideoSinkSubscriber.h>
#include "OTOpenVideoContext.h"

namespace openvideo
{
	class State;
	class VideoSink;
	class VideoSinkSubscriber;
}

namespace ARToolKitPlus
{
	class TrackerSingleMarker;
	class Logger;
}

class ARToolKitPlusModuleLogger;



namespace ot 
{

class ARToolKitPlusModule;
typedef std::vector<Node*> NodeVector;
typedef std::map<int,Node*> MarkerIdMap;

/*
 * Abstract Image grabbing function
 */
class ImageGrabber
{
public:
	static const char* formatStrings[3];

	// These are the supported Pixel Formats
	enum FORMAT {
		XBGR8888 = 0,
		BGRX8888 = 1,
		BGR888 = 2,
		RGBX8888 = 3,
		RGB888 = 4,
		RGB565 = 5,
		LUM8 = 6
	};

	/*
	 * This function is called by updateARToolkit() and is in charge of returning the pointer to the image frame
	 */
	virtual bool grab(const unsigned char*& nImage, int& nSizeX, int& nSizeY, FORMAT& nFormat) = 0;
};

/*
 * Inherits from both Open Video's VideoSinkSubscriber and the ImageGrabber.
 * This is the class that actually links OpenVideo and OpenTracker.
 */
class OVImageGrabber : public ot::ImageGrabber, public openvideo::VideoSinkSubscriber
{
public:
	const unsigned char* image;
	int sizeX, sizeY;
	FORMAT format;
	ot::ARToolKitPlusModule *myARToolKitPlusMod;
	bool isStarted;

	OVImageGrabber(){isStarted=false;};

	/*
	 * Registers itself with VideoSink
	 */

	void init(const char *name);

	/*
	 * Registers itself with ARToolKitPlusModule 
	 */
	void registerARToolkitPlusMod(ot::ARToolKitPlusModule *newARToolkitPlusMod);

	/*
	 * Implementation of inherited grab function. Passes along the pointer to the image frame.
	 */
	bool grab(const unsigned char*& nImage, int& nSizeX, int& nSizeY, ImageGrabber::FORMAT& nFormat);

	/*
	 * Called by VideoSink. This way OpenVideo sends the pointer to the Video Frame.
	 */
	void update(openvideo::State* curState);

	/*
	 * All supported pixel formats on the OpenVideo end should be added here.
	 */
	void initPixelFormats();
};


/**
 * The module and factory to drive the test source nodes. It constructs
 * TestSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

class OPENTRACKER_API ARToolKitPlusModule : public ThreadModule, public NodeFactory
{
// Members
protected:
	/// map to find nodes that contain markers
	MarkerIdMap sourcesMap;

    /// list of TestSource nodes in the tree
    NodeVector sources;

	/// stores the list of all markers that were visible during the last update
	NodeVector visibleMarkers;

	/// stores a list of the best marker confidences found in an image
	float	*bestCFs;
	int		maxMarkerId;

    /// file name of cameradata file
    std::string cameradata;

	/// config file of openvideo
	std::string ovConfigFile;

	/// openvideo sink name
	std::string ovSinkName;

    /// size of the image in pixels
    int sizeX, sizeY;

	/// should ARToolKitPPCModule flip the image in x- or y-direction?
	//bool flipX, flipY;

	/// an optional prefix for pattern filenames
	std::string patternDirectory;

	/// an optional camera device name
	std::string cameraDeviceHint;

	/// if true ot will use arDetectMarkerLite instead of arDetectMarker
	bool useMarkerDetectLite;

	/// flag to stop image processing thread
	int stop;

	void init(StringTable& attributes, ConfigNode * localTree);

	void updateSingleMarkerSource(Node *source, float cf, float matrix[3][4]);
	void updateMultiMarkerSource(Node *source, float cf, float matrix[3][4]);
	void updateState(State &state, float matrix[3][4]);

	ImageGrabber* imageGrabber;

	float trackerNear,trackerFar;
	ARToolKitPlus::TrackerSingleMarker* tracker;
	ARToolKitPlus::Logger* logger;

	// implement ARToolKitPlus::Logger
	//virtual void artLog(const char* nStr);
	//virtual void artLogEx(const char* nStr, ...);

// Methods
public:
	enum {
		MAX_MARKERID = 511
	};

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
    //void getFlipping(bool* isFlippedH, bool* isFlippedV)  {  *isFlippedH = flipX;  *isFlippedV = flipY;  }

	void registerImageGrabber(ImageGrabber* nGrabber)  {  imageGrabber = nGrabber;  }

	ARToolKitPlus::TrackerSingleMarker* getARToolKitPlus()  {  return tracker;  }

	const char* getARToolKitPlusDescription() const;

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

	// this method should be protected
	bool updateARToolKit();

	bool doBench;

    /** returns the config file name of OpenVideo */
	const char *getOVConfigFileName();

	/** returns the VideoSink name*/
	const char *getOVSinkName();
};

} // namespace ot

#endif //#ifdef USE_ARTOOLKITPLUS

#endif // _ARTOOLKITMODULEPLUS_H

