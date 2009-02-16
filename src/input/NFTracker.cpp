/****************************************************************************\
 *
 *  (C) 2008 by Daniel Wagner. All rights reserved.
 *
 *  Project: Studierstube Natural Feature Tracking Demo
 *
 *  @author Daniel Wagner
 *
 *  $Id: NFTracker.cpp 237 2008-12-18 16:47:24Z daniel $
 *
\****************************************************************************/


#include <OpenTracker/OpenTracker.h>

#ifdef USE_NFTRACKER

#include <OpenTracker/input/NFTracker.h>

#include <StbCore/InFile.h>
#include <StbCore/FileSystem.h>
#include <StbCore/OS.h>
#include <StbCore/Logger.h>

#include <StbTracker/TrackerMain.h>
#include <StbTracker/Features/Camera.h>
#include <StbTracker/Math/MathTool.h>


namespace ot {


NFTracker::NFTracker()
{
    trackerToCreate = 0; //TRACKER_GRADIENT|TRACKER_PATCH;
    previouslyActiveTracker = currentlyActiveTracker = TRACKER_GRADIENT;
    gradientTracker = NULL;
    patchTracker = NULL;
    poseValid = false;
}


NFTracker::~NFTracker()
{

}


bool
NFTracker::init(const char* nFile, std::string cameraCalib, std::string dbConfig)
{
    if(StbCore::InFileAutoPtr varFile = StbCore::FileSystem::getInstance()->openFileForReading(dbConfig.c_str()))
    {
        if(!StbCore::Vars::getInstance()->readFromFile(varFile))
            return false;
    }
    else
        return false;


	StbTracker::Camera* stbCamera = StbTracker::Camera::createFromFile(cameraCalib.c_str());

    if(stbCamera==NULL)
        return false;


    camera.set(stbCamera);                          // Import the camera settings from StbTracker

    StbCore::VarRef<bool> createGradient("nftracker.use-gradient", false),
                          createPatch("nftracker.use-patch", false);

    if(createGradient)
        trackerToCreate |= TRACKER_GRADIENT;
    if(createPatch)
        trackerToCreate |= TRACKER_PATCH;



    // Create the Gradient Tracker
    //
    if(trackerToCreate&TRACKER_GRADIENT)
    {
        gradientTracker = new NFTGradientTrackerEx;
        gradientTracker->init(camera);
    }

    // Create the Patch Tracker
    //
    if(trackerToCreate&TRACKER_PATCH)
    {
        patchTracker = new NFTPatchTrackerEx;
        patchTracker->init(camera);
    }

    loadAllVarFiles(nFile);

    return true;
}


void
NFTracker::loadAllVarFiles(const char* nFilter)
{
    StbCore::StringW filter;
    StbCore::FileSystem::getInstance()->extendToAbsolutePath(nFilter, filter);
    StbCore::StringW filterPath = StbCore::FileSystem::getPathFromFileName(filter);
    StbCore::Vector<StbCore::SmallString<128> > files;
    files.reserve(16);

    size_t numFiles = StbCore::FileSystem::getInstance()->findFiles(filter, files, StbCore::FileSystem::OPEN_ABSOLUTEPATH);
    if(numFiles!=files.size())
        StbCore::Logger::getInstance()->logWarning("Too many tracking targets found");

    for(size_t i=0; i<files.size(); i++)
    {
        StbCore::StringW fileName(filterPath);
        fileName += StbCore::StringW(files[i].c_str());
        createFromVarFile(STBCORE_PARAMETER_CHAR(fileName));
    }
}


bool
NFTracker::createFromVarFile(const char* nFile)
{
    if(StbCore::InFileAutoPtr varFile = StbCore::FileSystem::getInstance()->openFileForReading(nFile))
    {
        if(!StbCore::Vars::getInstance()->readFromFile(varFile))
            return false;
    }
    else
        return false;

    if(gradientTracker!=NULL)
        gradientTracker->createTargetFromVars();
    if(patchTracker!=NULL)
        patchTracker->createTargetFromVars();

    return true;
}


void
NFTracker::update(const StbCV::Image& nCamImage)
{
    previouslyActiveTracker = currentlyActiveTracker;

    if(currentlyActiveTracker==TRACKER_GRADIENT)
    {
        gradientTracker->update(nCamImage);
        pose = gradientTracker->getPose();
        poseValid = gradientTracker->isPoseValid();
        if(poseValid && patchTracker!=NULL)
        {
            patchTracker->activateTarget(gradientTracker->getActiveTarget());
            patchTracker->setPosePrior(pose);
            currentlyActiveTracker=TRACKER_PATCH;
        }
    }
    else if(currentlyActiveTracker==TRACKER_PATCH)
    {
        patchTracker->update(nCamImage);
        pose = patchTracker->getPose();
        poseValid = patchTracker->isPoseValid();
        if(!poseValid)
            currentlyActiveTracker=TRACKER_GRADIENT;
    }
}


size_t
NFTracker::getActiveTarget() const
{
    return gradientTracker->getActiveTarget();
}


void
NFTracker::visualize()
{
    if(!poseValid)
        return;

    if(previouslyActiveTracker==NFTracker::TRACKER_GRADIENT)
    {
        if(gradientTracker)
            gradientTracker->showDetails();
    }
    else if(previouslyActiveTracker==NFTracker::TRACKER_PATCH)
    {
        if(patchTracker)
            patchTracker->showDetails();
    }
}


StbMath::Vec2F
NFTracker::getTargetSize() const
{
    if(patchTracker!=NULL)
        patchTracker->getTargetSize();

    return StbMath::Vec2F(0,0);
}


}  // namespace StbES

#endif //USE_NFTRACKER