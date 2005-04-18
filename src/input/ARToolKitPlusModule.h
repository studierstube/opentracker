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

#define ARTOOLKITPLUS_FOR_STB3

#include "../dllinclude.h"

#include <vector>
#include <map>

#include "../OpenTracker.h"

#ifdef USE_ARTOOLKITPLUS


#ifdef ARTOOLKITPLUS_FOR_STB3

namespace ot {

	struct MemoryBufferHandle
	{
		unsigned long  n; // sample number
		__int64 t;		  // timestamp

		MemoryBufferHandle() : n(0), t(0)
		{}
	};

} // namespace ot

class CVVidCapture;

#define STEREO_L 0
#define STEREO_R 1

class CVImage;

#endif //ARTOOLKITPLUS_FOR_STB3


namespace ARToolKitPlus {
	class TrackerSingleMarker;
	class Logger;
}

class ARToolKitPlusModuleLogger;

//#include <ARToolKitPlus/TrackerSingleMarker.h>


namespace ot {

typedef std::vector<Node*> NodeVector;
typedef std::map<int,Node*> MarkerIdMap;

class ImageGrabber {
public:
	static const char* formatStrings[3];

	enum FORMAT {
		RGBX8888 = 0,
		RGB565 = 1,
		LUM8 = 2
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

    /// size of the image in pixels
    int sizeX, sizeY;

	/// should ARToolKitPPCModule flip the image in x- or y-direction?
	bool flipX, flipY;

	/// an optional prefix for pattern filenames
	std::string patternDirectory;

	/// an optional camera device name
	std::string cameraDeviceHint;

	/// if true ot will use arDetectMarkerLite instead of arDetectMarker
	bool useMarkerDetectLite;

	/// flag to stop image processing thread
	int stop;

	void init(StringTable& attributes, ConfigNode * localTree);

	bool updateARToolKit();

	void updateSource(Node *source, float cf, float matrix[3][4]);

	ImageGrabber* imageGrabber;

	float trackerNear,trackerFar;
	ARToolKitPlus::TrackerSingleMarker* tracker;
	ARToolKitPlus::Logger* logger;
	bool idbasedMarkers;

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
    void getFlipping(bool* isFlippedH, bool* isFlippedV)  {  *isFlippedH = flipX;  *isFlippedV = flipY;  }

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



#ifdef ARTOOLKITPLUS_FOR_STB3
	virtual void run();

	virtual void start();

	virtual void close();


    /** returns whether two cameras are configured */
    bool isStereo();

    /** returns the width of the grabbed image in pixels */
    int getSizeX(int stereo_buffer = STEREO_L);

    /** returns the height of the grabbed image in pixels */
    int getSizeY(int stereo_buffer = STEREO_L);

    /** returns whether the grabbed image is flipped horizontally
	  * or vertically */
    void getFlipping(bool* isFlippedH, bool* isFlippedV, int stereo_buffer = STEREO_L);

    /** returns a pointer to the grabbed image. The image format
     * is depending on the pixel format and typically RGB or RGBA 
     * times X times Y bytes. 
     * @return pointer to image buffer */
    unsigned char * lockFrame(MemoryBufferHandle* pHandle, int stereo_buffer = STEREO_L);
	// formerly getFrame()

    /** releases the pointer to the grabbed image.
     * @release frame pointer */
    void unlockFrame(MemoryBufferHandle Handle, int stereo_buffer = STEREO_L);

    /** 
     * returns the OpenGL flag that is used by ARToolkit to describe
     * the pixel format in the frame buffer. This is a simple way to 
     * pass the necessary information to use the image data in GL calls. */
    int getImageFormat(int stereo_buffer = STEREO_L);

	void setCapturedImage(CVImage* nImage);

protected:
	CVVidCapture* vidCap;
	CVImage* curCapImage;

	double		rate;
	std::string videomode;
	int			videoWidth, videoHeight;
	bool		didLockImage;
	//CRITICAL_SECTION CriticalSection; 

#endif //ARTOOLKITPLUS_FOR_STB3
};

} // namespace ot

#endif //#ifdef USE_ARTOOLKITPLUS

#endif // _ARTOOLKITMODULEPLUS_H

