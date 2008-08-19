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
/** header file for StbTrackerModule module.
 *
 * @author Erick Mendez
 * @author Daniel Wagner
 *
 * $Header$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section StbTrackerModule StbTrackerModule
 * The StbTrackerModule interfaces to the StbTracker library to track the
 * position and orientation of visual markers with a video camera. It uses
 * the information provided by @ref stbtracker nodes and inserts the state
 * events through @ref stbtracker nodes into the tree. The configuration
 * element is @c StbTrackerConfig and has the following attributes :
 * @li @c camera-parameter: file containing camera calibration data
 * @li @c treshold: either a numerical value (0-255) or 'auto' for automatic thresholding. Default is '100'
 * @li @c undist-mode: undistortion mode; one of 'none', 'lut' or 'std' (default)
 * @li @c detect-mode: marker detection mode (marker history); either 'lite' or 'std' (default)
 * @li @c pose-estimator: pose estimator selection; one of the following 'std' (default), 'cont' (better version of std) or 'rpp' (robust pose estimator)
 * @li @c marker-mode: one of the following: 'template' (default), 'idbased' or 'bch'
 * @li @c border-width: width of the border; default value is '0.250'
 * @li @c pattern-dir an optional string that is prefixed to any pattern filename or
 *        the camera data filename. It tries to find the file under the original as
 *        well the prefixed name, in that order.
 * @li @c ovSink: name of the video sink which provides the tracking image
 *
 * An example configuration element looks like this :
 * @verbatim
 <StbTrackerConfig camera-parameter="quickcampro400.dat" marker-mode="idbased" border-width="0.125" treshold="auto" pose-estimator="cont" ovSink="VideoStream"/> @endverbatim
*/


#ifndef _STBTRACKERMODULE_H
#define _STBTRACKERMODULE_H

#include "../dllinclude.h"

#include <vector>
#include <map>

#include "../OpenTracker.h"
#include "../core/VideoUser.h"

#ifdef USE_STBTRACKER

#include <OpenTracker/input/StbTrackerMarkerSource.h>
#include <StbTracker/TrackerMain.h>
#include <StbTracker/Base/Image.h>
#include <StbTracker/Base/Logger.h>
#include <StbTracker/Base/MarkerFrameSimpleId.h>
#include <StbTracker/Base/MarkerTemplate.h>
#include <StbTracker/Base/Marker_PIMPL.h>
#include <StbTracker/Base/TargetSingleMarker.h>
#include <StbTracker/Features/ThresholderAuto.h>
#include <StbTracker/Features/ThresholderNonUniform.h>
#include <StbTracker/Features/FiducialDetectorRect.h>
#include <StbTracker/Features/MarkerDetectorSimpleId.h>
#include <StbTracker/Features/MarkerDetectorSimpleIdFixed.h>
#include <StbTracker/Features/MarkerDetectorFrameSimpleIdFixed.h>
#include <StbTracker/Features/MarkerDetectorBCH.h>
#include <StbTracker/Features/MarkerDetectorBCHFixed.h>
#include <StbTracker/Features/MarkerDetectorTemplate.h>
#include <StbTracker/Features/MarkerDetectorDataMatrix.h>
#include <StbTracker/Features/MarkerDetectorDataMatrixFixed.h>
#include <StbTracker/Features/MarkerDetectorSplitFixed.h>
#include <StbTracker/Features/PoseEstimatorLM.h>
#include <StbTracker/Features/PoseEstimatorLMFixed32.h>
#include <StbTracker/Features/PoseEstimatorNull.h>
#include <StbTracker/Features/PoseEstimatorRPPSingle.h>
#include <StbTracker/Features/PoseEstimatorRPPMulti.h>
#include <StbTracker/Features/TargetManagerDefault.h>
#include <StbTracker/Features/Camera.h>
#include <StbTracker/Features/PoseFilterDESP.h>
#include <StbTracker/Features/CornerFilterDESP.h>
#include <StbTracker/Util/ImageTool.h>


namespace StbTracker
{
  class TrackerSingleMarker;
}

class StbTrackerModuleLogger;


namespace ot
{

  class StbTrackerModule;
  typedef std::vector<Node::Ptr> NodeVector;
  typedef std::map<int,Node::Ptr> MarkerIdMap;


  /**
   * The module and factory to drive the test source nodes. It constructs
   * TestSource nodes via the NodeFactory interface and pushes events into
   * the tracker tree according to the nodes configuration.
   * @author Gerhard Reitmayr
   * @ingroup core
   */
  class OPENTRACKER_API StbTrackerModule : public ThreadModule, public NodeFactory, public VideoUser
    {
      // Members
    protected:

      /// map to find nodes that contain markers
      MarkerIdMap sourcesMap;

      /// list of nodes in the tree
      NodeVector sources;

      /// file name of cameradata file
      std::string cameradata;

      /// size of the image in pixels
      int sizeX, sizeY;

      /// flag to stop image processing thread
      int stop;

      void init(StringTable& attributes, ConfigNode * localTree);
      void updateMarkerSource(StbTrackerMarkerSource *source, float cf, float matrix[3][4]);
      void updateEvent(Event &event, float matrix[3][4]);

      float trackerNear,trackerFar;
	  StbTracker::TrackerMain* tracker;
      StbTracker::Logger* logger;
	  StbTracker::Image* image;
	  unsigned char* imageGray;
	  StbTracker::Camera* camera;
	  StbTracker::TargetManagerDefault* targetManager;

      // Methods
    public:
      enum {
	MAX_MARKERID = 511
      };

      /** constructor method. */
      StbTrackerModule();

      /** Destructor method, clears nodes member. */
      virtual ~StbTrackerModule();

      virtual Node * createNode( const std::string& name, const StringTable& attributes);

      /**
       * pushes events into the tracker tree. Checks all TestSources and
       * pushes new events, if a TestSource fires.
       */
      virtual void pushEvent();

      /**
       * queries the camera and pushes data into the tree
       */
      void update();

      /** returns the width of the grabbed image in pixels */
      int getSizeX() const  {  return sizeX;  }

      /** returns the height of the grabbed image in pixels */
      int getSizeY() const  {  return sizeY;  }

      StbTracker::TrackerMain* getStbTracker()  {  return tracker;  }

      void newVideoFrame(const unsigned char* image, int width, int height, PIXEL_FORMAT format, void *usrData=NULL);
    };

	OT_MODULE(StbTrackerModule);

} // namespace ot


#endif //#ifdef USE_STBTRACKER

#endif // _STBTRACKERMODULE_H

/*
 * ------------------------------------------------------------
 *   End of StbTrackerModule.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------
 */
