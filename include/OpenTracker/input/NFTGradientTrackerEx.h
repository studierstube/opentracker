/****************************************************************************\
 *
 *  (C) 2008 by Daniel Wagner. All rights reserved.
 *
 *  Project: Studierstube Natural Feature Tracking Demo
 *
 *  @author Daniel Wagner
 *
 *  $Id: NFTGradientTrackerEx.h 236 2008-12-17 14:01:42Z daniel $
 *
\****************************************************************************/


#ifndef __STBES_GRADIENTTRACKER_HEADERFILE__
#define __STBES_GRADIENTTRACKER_HEADERFILE__

#ifdef USE_NFTRACKER

#include <StbCore/Vars.h>
#include <StbCV/StbCV.h>


namespace StbCV {
    class Camera;
    class Image;
    namespace GradientTracker {
        class GradientTracker;
    }
}


namespace ot {

class Image;

class NFTGradientTrackerEx
{
public:
	NFTGradientTrackerEx();
    ~NFTGradientTrackerEx();

    void init(const StbCV::Camera& nCamera);

    void createTargetFromVars();

    size_t getActiveTarget() const;

    void update(const StbCV::Image& nCamImage);

    const StbCV::PoseF& getPose() const;

    bool isPoseValid() const;

    void showDetails();

    void switchShowKeypoints()  {  showKeypoints = !showKeypoints;  }

    void switchShowMatches()  {  showMatches = !showMatches;  }

    void switchShowInfo()  {  showInfos = !showInfos;  }

protected:
    void reportProfiling(int nX, int nY);
    void reportProfilingShort(int nX, int nY);
    void reportTrackingShort(int nX, int nY);
    void reportTracking(int nX, int nY);
    void reportQuality();

    void drawText(int nX, int nY, const char* nStr, ...);

    StbCV::GradientTracker::GradientTracker	*tracker;
	Image				                    *refImage;

    
    StbCore::VarRef<bool>                   showKeypoints,      ///< Render small crosses for all detected keypoints
                                            showMatches,        ///< Draw lines to matched features in reference image
                                            showInfos;          ///< Display textual info

};


}  // namespace StbES

#endif //#ifdef USE_NFTRACKER

#endif //__STBES_GRADIENTTRACKER_HEADERFILE__
