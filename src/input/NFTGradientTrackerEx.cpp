/****************************************************************************\
 *
 *  (C) 2008 by Daniel Wagner. All rights reserved.
 *
 *  Project: Studierstube Natural Feature Tracking Demo
 *
 *  @author Daniel Wagner
 *
 *  $Id: NFTGradientTrackerEx.cpp 237 2008-12-18 16:47:24Z daniel $
 *
\****************************************************************************/

#include <OpenTracker/OpenTracker.h>

#ifdef USE_NFTRACKER

#include <OpenTracker/input/NFTGradientTrackerEx.h>

#include <StbCore/InFile.h>
#include <StbCore/FileSystem.h>
#include <StbCV/GradientTracker/GradientTracker.h>
#include <StbCV/Camera.h>
#include <StbCV/Render.h>


namespace ot {


NFTGradientTrackerEx::NFTGradientTrackerEx() : showKeypoints("gradienttracker.visualize.keypoints", false),
                                     showMatches("gradienttracker.visualize.matches", false),
                                     showInfos("gradienttracker.visualize.text", false)
{
	tracker = NULL;
	refImage = NULL;
}


NFTGradientTrackerEx::~NFTGradientTrackerEx()
{
}


void
NFTGradientTrackerEx::init(const StbCV::Camera& nCamera)
{
    StbCore::Logger::getInstance()->logInfo("Creating GradientTracker");
	tracker = new StbCV::GradientTracker::GradientTracker();
    tracker->init(&nCamera);

	//StbCV::RenderTarget* target = StbCV::Renderer::getInstance()->getRenderTarget();
 //   if(target->getHeight()>(int)nCamera.getIntrinsics().size(1))
 //   {
	//    StbCore::String imgFileName;
	//    if(StbCore::Vars::getInstance()->getString("gradienttracker.dataset.ref-image", imgFileName))
	//    {
	//	    if(StbCore::InFileAutoPtr imgFile = StbCore::FileSystem::getInstance()->openFileForReading(imgFileName.c_str()))
 //           {
 //               RenderTarget* target = Renderer::getInstance()->getRenderTarget();
	//	        refImage = target->createImage();
	//	        refImage->load(imgFile);
	//	        tracker->setVisImage(Vec2(0, target->getHeight()-refImage->getHeight()), Vec2(refImage->getWidth(),refImage->getHeight()));
 //           }
	//    }
 //   }
}


void
NFTGradientTrackerEx::createTargetFromVars()
{
    tracker->createTargetFromVars();
}


size_t
NFTGradientTrackerEx::getActiveTarget() const
{
    return tracker->getActiveTarget();
}


void
NFTGradientTrackerEx::update(const StbCV::Image& nCamImage)
{
    tracker->update(nCamImage);
}


const StbCV::PoseF&
NFTGradientTrackerEx::getPose() const
{
    return tracker->getPose();
}

bool
NFTGradientTrackerEx::isPoseValid() const
{
    return tracker->isPoseValid();
}


void
NFTGradientTrackerEx::showDetails()
{
/*
    RenderTarget* target = Renderer::getInstance()->getRenderTarget();
    unsigned short* pixels = (unsigned short*)target->getPixels();

    //const_cast<int&>(Kernel::getInstance()->getConfig()->getLogging().drawFPS) = showInfos ? 1 : 0;

    if(target->getWidth()<=320)
    {
        StbCV::Render render(pixels, target->getWidth(),target->getHeight());

        if(showKeypoints)
            tracker->visualize(render, showKeypoints, false);

        if(showInfos)
        {
            reportTrackingShort(5,target->getHeight()-30);
            reportProfilingShort(5,target->getHeight()-15);
        }
        //else
        //    reportQuality();
        return;
    }

    if(pixels!=NULL)
    {
        if(refImage!=NULL)
            refImage->draw(0, target->getHeight()-refImage->getHeight());

        StbCV::Render render(pixels, target->getWidth(),target->getHeight());
        tracker->visualize(render, showKeypoints, showMatches);
    }

    if(showInfos)
    {
        if(false)
        {
            reportProfiling(5,255);
            reportTracking(5+170,255);
        }
        else
        {
            reportProfiling(325,5);
            reportTracking(495,5);
        }
    }
	*/
}

void
NFTGradientTrackerEx::reportProfilingShort(int nX, int nY)
{
    const StbCV::GradientTracker::Profiler::Timings& timings = tracker->getProfiler().getTimings();

	drawText(nX,nY, "Time: %.1f ms (%.1f %.1f %.1f %.1f %.1f %.1f %.1f)",
             timings.overall,
             timings.imageBlurring,
             timings.keypointDetection,
             timings.keypointDescribing,
             timings.keypointMatching,
             timings.outlierRemoval,
             timings.poseEstimation,
             timings.keypointTracking);
}


void
NFTGradientTrackerEx::reportTrackingShort(int nX, int nY)
{
    const StbCV::GradientTracker::FrameStats& stats = tracker->getFramStats();

    drawText(nX,nY, "K: %d  F: %d",
        stats.numKeypoints,
        stats.refinement.numOutPoints);
}


void
NFTGradientTrackerEx::reportQuality()
{
/*
    int numPts = tracker->getFramStats().refinement.numOutPoints;
    int minPts = 15, maxPts = 50;

    int quality = 100*(numPts-minPts)/(maxPts-minPts);
    if(quality>99)
        quality = 99;
    if(quality<0)
        quality = 0;

    int green = (255*quality)/50;       if(green>255) green = 255;
    int red = (255*(99-quality))/50;    if(red>255) red = 255;
    unsigned short color = StbIO::ImageTool::convertRGB888_to_RGB565(red,green,0);
    RenderTarget* target = Renderer::getInstance()->getRenderTarget();
    unsigned short* pixels0 = (unsigned short*)target->getPixels();

    int x0=5,y0=5, w=10,h=10, x1=x0+w, y1=y0+h;
    pixels0 += x0 + y0*target->getWidth();

    for(int y=y0; y<y1; y++)
    {
        unsigned short* pixels = pixels0;
        for(int x=x0; x<x1; x++)
        {
            *pixels++ = color;
        }
        pixels0 += target->getWidth();
    }
	*/
}


void
NFTGradientTrackerEx::reportProfiling(int nX, int nY)
{
	unsigned int x=nX, y=nY, dy=15;

	const StbCV::GradientTracker::Profiler::Timings& timings = tracker->getProfiler().getTimings();

	drawText(x,y, "Overall: %.2f ms", timings.overall);								y+=dy;
    drawText(x,y, "Image blurring: %.2f ms", timings.imageBlurring);   				y+=dy;
	drawText(x,y, "Kpt detect: %.2f ms", timings.keypointDetection);				y+=dy;
	drawText(x,y, "Kpt describe: %.2f ms", timings.keypointDescribing);				y+=dy;
	drawText(x,y, "Kpt match: %.2f ms", timings.keypointMatching);					y+=dy;
	drawText(x,y, "Outlier: %.2f ms", timings.outlierRemoval);						y+=dy;
	drawText(x,y, "Pose: %.2f ms", timings.poseEstimation);							y+=dy;
    drawText(x,y, "Kpt track: %.2f ms", timings.keypointTracking);			        y+=dy;
}


void
NFTGradientTrackerEx::reportTracking(int nX, int nY)
{
	unsigned int x=nX, y=nY, dy=15;

	const StbCV::GradientTracker::FrameStats& stats = tracker->getFramStats();

	drawText(x,y, "Rotation: %d", stats.mainRotation);									y+=dy;
	drawText(x,y, "Keypoints: %d", stats.numKeypoints);									y+=dy;
	drawText(x,y, "Matched: %d", stats.numMatchedKeypoints);							y+=dy;

	y+=dy;
	drawText(x,y, "Outliers:");															y+=dy;
	drawText(x,y, "  angle: %d", stats.outlierRemoval.numFilteredByAngle);				y+=dy;
	drawText(x,y, "  doubles: %d", stats.outlierRemoval.numFilteredDoubles);			y+=dy;
	drawText(x,y, "  line-test: %d", stats.outlierRemoval.numFilteredByLineTest);		y+=dy;
	drawText(x,y, "  homography: %d", stats.outlierRemoval.numFilteredByHomography);	y+=dy;
	drawText(x,y, "  refinement: %d", stats.numFinalMatches-stats.refinement.numOutPoints);							y+=dy;

	y+=dy;
	drawText(x,y, "  reproj: %.2f->%.2f", stats.refinement.originalReprojErr, stats.refinement.refinedReprojErr);	y+=dy;

	y+=dy;
	drawText(x,y, "FINAL MATCHES: %d", stats.refinement.numOutPoints);												y+=dy;

	y+=dy;
	drawText(x,y, stats.refinement.numOutPoints<8 ? "FAILED": "");													y+=dy;
}


void
NFTGradientTrackerEx::drawText(int nX, int nY, const char* nStr, ...)
{
    //Font* font = WidgetManager::getInstance()->getFont();
    //va_list marker;
    //char str[256];

    //va_start(marker, nStr);
    //vsprintf(str, nStr, marker);

    ////font->setColor(StbES::ImageTool::convertRGB888_to_RGB565(0,200,200));
    //font->drawText(nX,nY, str);
}




}  // namespace StbES

#endif //USE_NFTRACKER