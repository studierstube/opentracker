/* ========================================================================
 * Copyright (c) 2008,
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
/** header file for NFTrackerModule module.
 *
 * @author Markus Sareika
 *
 * $Header$
 * @file                                                                   */
/* ======================================================================= */

/**
* @page module_ref Module Reference
* @section NFTrackerModule NFTrackerModule
* make shure to provide the ovSink attribute in the source node
* @li @c ovSink: name of the video sink which provides the tracking image
* An example configuration element looks like this :
* @verbatim
 <NFTrackerConfig ovSink="Stb4VideoSink" featureset="./jakomini/jakomini.xml" cameraCalib="../../../cameraCal/QuickCamS7500.cal"/>
 <NFTrackerSource targetName="Jakomini"/> @endverbatim
 */

#ifndef _NFTRACKERMODULE_H
#define _NFTRACKERMODULE_H

#include "../dllinclude.h"

#include <vector>
#include <map>

#include "../OpenTracker.h"
#include "../core/VideoUser.h"


#ifdef USE_NFTRACKER

#include <ace/OS.h>
#include <NFT3/NFT3_Tracker.h>
#include <StbCore/Image.h>
#include <StbCore/OS.h>
#include <StbCore/Logger.h>
#include <StbCore/FileSystem.h>
#include <StbCore/InFile.h>

#include <StbCore/Window.h>

#include <conio.h>

#if (defined(_DEBUG))
#      pragma comment(lib, "NFT3d.lib")
#      pragma comment(lib, "StbCVd.lib")
#      pragma comment(lib, "StbCored.lib")
#      pragma comment(lib, "StbMathd.lib")
#      pragma comment(lib, "StbIOd.lib")
#    else
#      pragma comment(lib, "NFT3.lib")
#      pragma comment(lib, "StbCV.lib")
#      pragma comment(lib, "StbCore.lib")
#      pragma comment(lib, "StbMath.lib")
#      pragma comment(lib, "StbIO.lib")
#    endif

namespace ot
{

class NFTrackerModule;
typedef std::vector<Node::Ptr> NodeVector;


/**
* The module and factory to drive the source nodes. It constructs
* Source nodes via the NodeFactory interface and pushes events into
* the tracker tree according to the nodes configuration.
*/
class OPENTRACKER_API NFTrackerModule : public ThreadModule, public NodeFactory,  public VideoUser
{
public:
 
	/** constructor method. */
	NFTrackerModule();

	/** Destructor method, clears nodes member. */
	virtual ~NFTrackerModule();

	//StbCV::Window debugWindow;

	void newVideoFrame(const unsigned char* frameData, int newSizeX, int newSizeY, PIXEL_FORMAT imgFormat, void* trackingData);

	void init(StringTable& attributes, ConfigNode * localTree);

	virtual Node * createNode( const std::string& name, const StringTable& attributes);

	void start();

	/**
	* pushes events into the tracker tree. 
	*/
	virtual void pushEvent();

protected:

	/// list of TestSource nodes in the tree
	NodeVector sources;

    /// stores the list of all markers that were visible during the last update
    NodeVector visibleMarkers;


private:

	std::string featureset;
	std::string cameraCalib;

	NFT3::Tracker	*tracker;
	StbCore::Image *coreImageBuffer;
	//unsigned char* lumBuffer;
	StbCV::Image cameraGray;
	bool noTracking;
	bool initialized;

	void createTracker();

	void process();

};

	OT_MODULE(NFTrackerModule);

} // namespace ot


#endif //#ifdef USE_NFTRACKER

#endif // _NFTRACKERMODULE_H
