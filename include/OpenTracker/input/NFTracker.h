/****************************************************************************\
 *
 *  (C) 2008 by Daniel Wagner. All rights reserved.
 *
 *  Project: Studierstube Natural Feature Tracking Demo
 *
 *  @author Daniel Wagner
 *
 *  $Id: NFTracker.h 237 2008-12-18 16:47:24Z daniel $
 *
\****************************************************************************/


#ifndef __STBES_NFTRACKER_HEADERFILE__
#define __STBES_NFTRACKER_HEADERFILE__

#ifdef USE_NFTRACKER

#include <StbCore/Vars.h>
#include <StbCore/TimingHP.h>
#include <StbCV/StbCV.h>
#include <StbCV/Camera.h>

#include <OpenTracker/input/NFTGradientTrackerEx.h>
#include <OpenTracker/input/NFTPatchTrackerEx.h>

#include "../dllinclude.h"

namespace StbCV {
    class Camera;
    class Image;
    class PatchTracker;
}


namespace ot {

class NFTracker
{
public:
    enum TRACKER {
        TRACKER_GRADIENT = 1,
        TRACKER_PATCH = 2
    };

	NFTracker();
    ~NFTracker();

    bool init(const char* nFile, std::string cameraCalib, std::string dbConfig);

    void loadAllVarFiles(const char* nFilter);

    bool createFromVarFile(const char* nFile);

    void update(const StbCV::Image& nCamImage);

    size_t getActiveTarget() const;

    void visualize();

    const StbCV::Camera& getCamera() const  {  return camera;  }

    StbMath::Vec2F getTargetSize() const;

    TRACKER getActiveTracker() const  {  return currentlyActiveTracker;  }

    TRACKER getPreviouslyActiveTracker() const  {  return previouslyActiveTracker;  }

    bool isPoseValid() const  {  return poseValid;  }

    const StbCV::PoseF& getPose() const  {  return pose;  }


protected:
    void drawText(int nX, int nY, const char* nStr, ...);

    StbCV::Camera       camera;

    unsigned int        trackerToCreate;                        ///< Flag telling which trackers to create
    TRACKER             currentlyActiveTracker,                 ///< Which tracker is currently active
                        previouslyActiveTracker;                ///< Which tracker was active in the last frame
    bool                poseValid;                              ///< Flag telling if the current pose is valid
    StbCV::PoseF        pose;                                   ///< The current pose

	NFTGradientTrackerEx   *gradientTracker;                       ///< Instance of the gradient tracker
	NFTPatchTrackerEx      *patchTracker;                          ///< Instance of the patch tracker
};


}  // namespace ot

#endif //#ifdef USE_NFTRACKER

#endif //__STBES_NFTRACKER_HEADERFILE__
