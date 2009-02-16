/****************************************************************************\
 *
 *  (C) 2008 by Daniel Wagner. All rights reserved.
 *
 *  Project: Studierstube Natural Feature Tracking Demo
 *
 *  @author Daniel Wagner
 *
 *  $Id: NFTPatchTrackerEx.h 236 2008-12-17 14:01:42Z daniel $
 *
\****************************************************************************/


#ifndef __STBES_PATCHTRACKER_HEADERFILE__
#define __STBES_PATCHTRACKER_HEADERFILE__

#ifdef USE_NFTRACKER

#include <StbCore/Vars.h>
#include <StbCore/TimingHP.h>
#include <StbCV/StbCV.h>


namespace StbCV {
    class Camera;
    class Image;
    class PatchTracker;
}


namespace ot {

class Image;

class NFTPatchTrackerEx
{
public:
	NFTPatchTrackerEx();
    ~NFTPatchTrackerEx();

    void init(const StbCV::Camera& nCamera);

    void createTargetFromVars();

    void activateTarget(size_t nTarget);

    void update(const StbCV::Image& nCamImage);

    void setPosePrior(const StbCV::PoseF& nPose);

    const StbCV::PoseF& getPose() const  {  return pose;  }

    bool isPoseValid() const  {  return poseOk;  }

    void showDetails();

    void switchShowFeatures()  {  showFeatures = !showFeatures;  }

    void switchShowInfo()  {  showInfos = !showInfos;  }

    StbMath::Vec2F getTargetSize() const;

protected:

    void drawText(int nX, int nY, const char* nStr, ...);

    StbCV::PatchTracker	*tracker;
    StbCV::PoseF        pose;
    bool                poseOk;
	Image				*refImage;

    StbCore::VarRef<bool>   visualize0,
                            visualize1,
                            visualizeLines,
                            visualizeText;

    bool				showFeatures,       ///< Render small crosses for all detected keypoints
                        showInfos;          ///< Display textual info

    StbCore::TimingHP   trackingTimeMeasure;
    float               trackingTime, t0s,t0p, t1s,t1p;
};


}  // namespace StbES

#endif //#ifdef USE_NFTRACKER

#endif //__STBES_PATCHTRACKER_HEADERFILE__
