/****************************************************************************\
 *
 *  (C) 2008 by Daniel Wagner. All rights reserved.
 *
 *  Project: Studierstube Natural Feature Tracking Demo
 *
 *  @author Daniel Wagner
 *
 *  $Id: NFTPatchTrackerEx.cpp 236 2008-12-17 14:01:42Z daniel $
 *
\****************************************************************************/

#include <OpenTracker/OpenTracker.h>

#ifdef USE_NFTRACKER

#include <OpenTracker/input/NFTPatchTrackerEx.h>
//#include <StbIO/ImageTool.h>
//#include <StbIO/ImageLoader.h>
#include <StbCore/Vars.h>
#include <StbCore/InFile.h>
#include <StbCore/FileSystem.h>
#include <StbCore/Logger.h>
#include <StbCV/PatchTracker.h>
#include <StbCV/Camera.h>
#include <StbCV/Render.h>
#include <StbTracker/TrackerMain.h>
#include <StbTracker/Features/TargetManager.h>


namespace ot {


NFTPatchTrackerEx::NFTPatchTrackerEx() : visualize0("patchtracker.visualize.level0", false),
                               visualize1("patchtracker.visualize.level1", false),
                               visualizeLines("patchtracker.visualize.lines", false),
                               visualizeText("patchtracker.visualize.text", false)
{
	tracker = NULL;
	refImage = NULL;

    trackingTime = 0.0f;
    t0s = t0p = t1s = t1p = 0.0f;
    showInfos = true;
    showFeatures = true;
}


NFTPatchTrackerEx::~NFTPatchTrackerEx()
{
}


StbMath::Vec2F
NFTPatchTrackerEx::getTargetSize() const
{
    return tracker->getTargetSize();
}


void
NFTPatchTrackerEx::init(const StbCV::Camera& nCamera)
{
    StbCore::Logger::getInstance()->logInfo("Creating PatchTracker");
    tracker = new StbCV::PatchTracker();
    tracker->setCamera(nCamera);
    //if(tracker->createTargetFromVars())
    //    StbCore::Logger::getInstance()->logInfo("PatchTracker created");

    //RenderTarget* target = Renderer::getInstance()->getRenderTarget();
    //if(target->getHeight()>(int)nCamera.getIntrinsics().size(1))
    //{
    //    StbCore::String imgFileName;
    //    if(StbCore::Vars::getInstance()->getString("patchtracker.dataset.ref-image", imgFileName))
    //    {
    //        if(StbCore::InFileAutoPtr imgFile = StbCore::FileSystem::getInstance()->openFileForReading(imgFileName.c_str()))
    //        {
    //            refImage = target->createImage();
    //            refImage->load(imgFile);
    //            tracker->setVisImage(Vec2(0, target->getHeight()-refImage->getHeight()), Vec2(refImage->getWidth(),refImage->getHeight()));
    //        }
    //    }
    //}
}


void
NFTPatchTrackerEx::createTargetFromVars()
{
    tracker->createTargetFromVars();
}


void
NFTPatchTrackerEx::activateTarget(size_t nTarget)
{
    tracker->activateTarget(nTarget);
}


void
NFTPatchTrackerEx::setPosePrior(const StbCV::PoseF& nPose)
{
    tracker->setRawPose(nPose);
}


float
blend1000(float t0, float t1)
{
    const float f = 0.05f;
    return t0*(1-f) + t1*1000.0f*f;
}


void
NFTPatchTrackerEx::update(const StbCV::Image& nCamImage)
{
    //StbTracker::TrackerMain* stbTracker = Kernel::getInstance()->getTracker()->getTrackerStbTracker()->getStbTrackerMain();
    //StbTracker::TargetManager* stbTargetManager = StbCore::Base::cast<StbTracker::TargetManager>(stbTracker->getFeature(StbTracker::TargetManager::getClassType()));
    //StbTracker::TargetVector targets = stbTargetManager->getVisibleTargets();
    //StbCV::PoseF stbTrackerPose;

    //if(targets.size()>0)
    //{
    //    StbTracker::Target* target = targets[0];
    //    target->getPose(stbTrackerPose, StbTracker::Target::TYPE_RAW);
    //    tracker->setPosePrior(stbTrackerPose);
    //}
    //else
        tracker->setPosePrior(tracker->getRawPose());

    trackingTimeMeasure.reset();
    trackingTimeMeasure.beginSection();
    poseOk = tracker->update(nCamImage);
    trackingTimeMeasure.endSection();

    trackingTime = blend1000(trackingTime, trackingTimeMeasure.getDuration());

    if(poseOk)
    {
        pose = tracker->getFilteredPose();
    }
}



void
NFTPatchTrackerEx::showDetails()
{
/*
    RenderTarget* target = Renderer::getInstance()->getRenderTarget();
    unsigned short* pixels = (unsigned short*)target->getPixels();

    StbCV::Render render(pixels, target->getWidth(), target->getHeight());

    if(pixels!=NULL && visualize1)
    {
        const StbCV::PatchTracker::Vec2Vector& cms = tracker->getMatches(1);
        for(size_t i=0; i<cms.size(); i++)
        {
            int x = StbMath::asInt(cms[i](0)),
                y = StbMath::asInt(cms[i](1));

            render.drawSquare(x,y, 5, 0xff00);
        }
    }

    if(pixels!=NULL && visualize0)
    {
        const StbCV::PatchTracker::Vec2Vector& fms = tracker->getMatches(0);
        for(size_t i=0; i<fms.size(); i++)
        {
            int x = StbMath::asInt(fms[i](0)),
                y = StbMath::asInt(fms[i](1));

            render.drawCross(x,y, 5, 0x00ff);
        }
    }

    if(visualizeText)
    {
        Font* font = WidgetManager::getInstance()->getFont();
        char str[256];
        const StbCV::PatchTracker::STATS& stats = tracker->getStats();

        t0s = blend1000(t0s, stats.level[1].timingSearch);
        t0p = blend1000(t0p, stats.level[1].timingPose);
        t1s = blend1000(t1s, stats.level[1].timingSearch);
        t1p = blend1000(t1p, stats.level[1].timingPose);

        sprintf(str, "%.2f [%.2f %.2f | %.2f %.2f]", trackingTime, t0s, t0p, t1s, t1p);
        font->drawText(5,target->getHeight()-20, str);

        //sprintf(str, "%d", stats.relocalizeCtr);
        //font->drawText(5,target->getHeight()-40, str);
    }

    //if(pixels!=NULL && visualizeLines)
    //{
    //    if(refImage!=NULL)
    //        refImage->draw(0, target->getHeight()-refImage->getHeight());

    //    const StbCV::PatchTracker::Vec2Vector& fms = tracker->getMatches(0);
    //    for(size_t i=0; i<fms.size(); i++)
    //    {
    //        int x = StbMath::asInt(fms[i](0)),
    //            y = StbMath::asInt(fms[i](1));

    //        render.drawCross(x,y, 0x00ff);
    //    }

    //}
	*/
}


}  // namespace StbES

#endif //USE_NFTRACKER