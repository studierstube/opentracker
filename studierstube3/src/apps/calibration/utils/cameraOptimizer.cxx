// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       camopt
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    HMD calibration camera optimization
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rcs		Rainer Splechtna
//            
// ===========================================================================
//  HISTORY:
//
//  21-oct-99 16:15:00  rcs      created
// ===========================================================================

#include <iostream>
#include <cassert>

#include <Inventor/SbLinear.h>
#include <Inventor/fields/SoMFInt32.h>

#include <stbapi/util/option.h>

#include "../powell/powell.h"
#include "../powell/nrutil.h"

#include <stbapi/util/ivio.h>

#include "cameraOptimizer.h"

// maximum number of allowed rays (used just for array size limitation)
const int MAX_RAYS = 8;  
// sampled points per reference point
const int SPPRP = 2;   

/* Pointer to logfile */
FILE *logfile;

/*
*/
//class cameraOptimizer;

class cameraOptimizer 
{
    SoOffAxisCamera *testCam;
    SbBool optSize;
	SbBool optEyeAgain;
    SbBool secondOpt;
    const SoMFVec3f &samples;
    const SoMFVec2f &referencePoints; 
    float averageErrorThreshold;
    SoMFInt32 badSamples;

public:
    cameraOptimizer(SoOffAxisCamera *testCam,                  
                    float averageErrorThreshold,
                    const SoMFVec3f &samples,
                    const SoMFVec2f &referencePoints,
                    SbBool optSize
                    ) : 
    testCam(testCam), optSize(optSize), 
    samples(samples), referencePoints(referencePoints), 
    averageErrorThreshold(averageErrorThreshold), optEyeAgain(TRUE)
    {
        assert(testCam);
    };

    ~cameraOptimizer()
    {
        camOpt = NULL;
    }

    SbBool runOptimizer(SbBool secondOpt, int badSamplesCount);

private:

    void optimizeCamera();
    /*  Returns the error of the sample specified by its index.
        The error is calculated as the deviation of the sampled
        3D-point projected to the screen to the screen position of the
        associated reference point (the point where the crosshair is 
        displayed). The error is measured in normalized screen
        coordinate (nsc). (nsc squared)
        @param sampleIndex index of sample to be tested
        @param xError error in x (normalized screen coordinates)
        @param yError error in y (normalized screen coordinates) 
        @return cummulative quadratic error of x and y error 
    */
    float testSample(int sampleIndex, float &xError, float &yError);
    float testSample(int sampleIndex);
    int findWorstSample();
    void errorLog();
    SbBool isBadSample(int index);
	SbBool onSameRay(int index);
    float calculateAverageError(void);
    static float testCamera(float t[]);
  static cameraOptimizer *camOpt;
};

cameraOptimizer *cameraOptimizer::camOpt = NULL;

// forward declaration 
SbRotation 
estimateCameraOrientation(const SoMFVec3f &sampledPoints, 
                     const SoMFVec2f &referencePoints,
                     const SbVec3f &eyepointPosition,
                     SoSFVec3f &position);

// IMPORTANT: order of points: all far then all near sampled points

SbBool
buildCamera(    SoOffAxisCamera *&camera,

                const SoMFVec2f &referencePoints,
                const SoMFVec3f &sampledPenPosition,  
                const SoMFRotation &sampledPenRotation,
                const SoMFVec3f &sampledHeadPosition,
                const SoMFRotation &sampledHeadRotation,

                SbBool relativeSampling,
                float averageErrorThreshold,
                float validDistance, 
                const char *logFileName,
                SbBool eyeOpt,
                SbBool sizeOpt,
                SbBool logging,
                SbBool secondOpt,
                int badSamplesCount    
            )
{
    // init log file
    //logfile = fopen("camopt_log.txt", "w");
    logfile = fopen(logFileName, "w");
    fprintf(logfile, "\nLOGSTART: \n\n");

    // calculate penPosition (in head-tracker space)
    SoMFVec3f markerPositions;
    SbVec3f markerPosHelp;
    int i;
    for (i=0; i<referencePoints.getNum()*SPPRP; i++)
    {
        if (relativeSampling)
        {
            sampledHeadRotation[i].inverse().multVec(
                sampledPenPosition[i] - sampledHeadPosition[i], markerPosHelp);
            markerPositions.set1Value(i, markerPosHelp);
        }
        else 
            markerPositions.set1Value(i, sampledPenPosition[i]);
    }

    // build start camera for optimization
    SbBool valid = FALSE;
    // calculate eyepointPosition
    SbVec3f cep = calculateEyepointPosition(markerPositions, validDistance, eyeOpt);
        
    if (cep != SbVec3f(-1,-1,-1))
    {
        camera = new SoOffAxisCamera();
        camera->ref();
        // set eyepointPosition
        camera->eyepointPosition = cep;
        // calculate start orientation 
        camera->orientation = estimateCameraOrientation(
            markerPositions, referencePoints, camera->eyepointPosition.getValue(), 
        // and start position
        camera->position);
        // start with usual aspect ratio 1.33333 (4:3)
	    camera->size.setValue(1,0.75);
	    // set remaining fields
	    camera->nearDistance.setValue(0.01);
	    camera->farDistance.setValue(100);
	    camera->focalDistance.setValue(2);
        camera->viewportMapping.setValue(SoCamera::LEAVE_ALONE);

        writeFile("cameraBeforeOpt.iv", camera);
        // optimize camera
        cameraOptimizer co = cameraOptimizer(camera, averageErrorThreshold, 
            markerPositions, referencePoints, sizeOpt);
        valid = co.runOptimizer(secondOpt, badSamplesCount);
    
	    camera->aspectRatio.setValue(camera->size.getValue()[0]/
                                     camera->size.getValue()[1]);       
        if (valid)
            camera->unrefNoDelete();
        else
            camera->unref();
    }

	if (!valid)
        camera = NULL;

    fclose(logfile);

    return valid;
}    


// changes position(of pp) and orientation(of pp) of testCam 
float 
cameraOptimizer::testCamera(float t[]) 
{
    // set position and orientation of camera
    camOpt->testCam->position.setValue(SbVec3f(t[1],t[2],t[3]));
	camOpt->testCam->orientation.setValue(SbVec3f(t[4],t[5],t[6]), t[7]);
	// and size (width)
	int o=0;
	if (camOpt->optSize)
	{
	    camOpt->testCam->size.setValue(camOpt->testCam->size.getValue()[0],t[8]);
		o=1;
	}
	if (camOpt->optEyeAgain)
		camOpt->testCam->eyepointPosition.setValue(SbVec3f(t[8+o],t[9+o],t[10+o]));

	float fret = 0;
	for (int i=0; i<camOpt->samples.getNum(); i++)
		if (!camOpt->isBadSample(i))
            fret += camOpt->testSample(i);
	return fret;
}


float 
cameraOptimizer::testSample(int sampleIndex, float &xError, float &yError)
{
    SbVec2f projectedPoint;
	float sx,sy,cx,cy;

    testCam->projectToScreen(samples[sampleIndex], projectedPoint);

	referencePoints[sampleIndex % referencePoints.getNum()].getValue(sx,sy);
	projectedPoint.getValue(cx,cy);
    xError = cx-sx;
    yError = cy-sy;
	return pow(cx-sx,2) + pow(cy-sy,2);
}


float 
cameraOptimizer::testSample(int sampleIndex)
{
	float dummy;
	return testSample(sampleIndex, dummy, dummy);
}


SbBool 
cameraOptimizer::isBadSample(int index)
{
    for (int i=0; i<badSamples.getNum(); i++)
        if (badSamples[i] == index)
            return TRUE;
    return FALSE;
}


SbBool 
cameraOptimizer::onSameRay(int index)
{
    for (int i=0; i<badSamples.getNum(); i++)
	{
        if (badSamples[i] % referencePoints.getNum() == 
			index % referencePoints.getNum())
            return TRUE;
		if (badSamples[i] / referencePoints.getNum() == 
			index / referencePoints.getNum())
            return TRUE;
	}
    return FALSE;
}

int 
cameraOptimizer::findWorstSample(void)
{
	int sampleIndex = 0;
	float temp, worst = 0;
    int i;

	for (i=0; i<samples.getNum(); i++)
        if (!isBadSample(i) &&
			!onSameRay(i) &&
        	((temp = testSample(i)) > worst))
        {
		    worst = temp;
		    sampleIndex = i;
        }	

	return sampleIndex;
}


void 
cameraOptimizer::errorLog(void)
{
	for (int i=0; i<samples.getNum(); i++) 
    {
		float ex,ey, error;
        error = sqrt(testSample(i,ex,ey));
		fprintf(logfile, "sample Point #%d diff: %+2.4f %+2.4f %+2.4f\n",
                i, ex*100, ey*100, error*100);
	}
    fprintf(logfile, "\naverageError: %f\n\n", calculateAverageError());
}


float
cameraOptimizer::calculateAverageError(void)
{
    float c = 0;
    float mean = 0;
	for (int i=0; i<samples.getNum(); i++) 
        if (!isBadSample(i))
        {
            c++;
	        mean += testSample(i);
        }
    assert(c);
    return 100*sqrt(mean/c);
}


void 
cameraOptimizer::optimizeCamera()
{
	int n = optSize ? 8 : 7;
	n += optEyeAgain ? 3 : 0;

	SbVec3f ppos = testCam->position.getValue();
	SbVec3f epos = testCam->eyepointPosition.getValue();
	SbVec3f axis;
	float angle;
	testCam->orientation.getValue(axis, angle);
	float ySize = testCam->size.getValue()[1];
    
	// init start vector [n+1]
	float startWithSize[12] =
	    {0,ppos[0],ppos[1],ppos[2],axis[0],axis[1],axis[2],angle,ySize,epos[0], epos[1], epos[2]};
	float startWithoutSize[11] =
	    {0,ppos[0],ppos[1],ppos[2],axis[0],axis[1],axis[2],angle,epos[0], epos[1], epos[2]};
    float *start;
	if (optSize)
        start = startWithSize;
	else
        start = startWithoutSize;

	float *p = vector(1,n);
	float **xi = matrix(1,n,1,n);
	
	// init p and xi
	int r,c;
	for (r=1; r<=n; r++) {
		p[r] = start[r];
		for (c=1; c<=n; c++) 
			xi[r][c] = r==c ? 1 : 0;
	}

	int iter;
	float fret;	
	// POWELL
	powell(p, xi,n,0.000001,&iter,&fret,testCamera);

    testCam->position.setValue(SbVec3f(p[1],p[2],p[3]));
	testCam->orientation.setValue(SbVec3f(p[4],p[5],p[6]), p[7]);
	int o=0;
	if (optSize)
	{
	    testCam->size.setValue(testCam->size.getValue()[0],p[8]);
		o=1;
	}
	if (optEyeAgain)
		testCam->eyepointPosition.setValue(SbVec3f(p[8+o],p[9+o],p[10+o]));

	fprintf(logfile, "iterations: %d\n",iter);

	free_vector(p,1,n);
	free_matrix(xi,1,n,1,n);
}


SbBool
cameraOptimizer::runOptimizer(SbBool secondOpt, int badSamplesCount) 
{
    using namespace std;

    // global varibale needed for powell test function
    camOpt = this;

    errorLog();
    optimizeCamera();
    errorLog();

	if (secondOpt) 
	{
        fprintf(logfile, "bad samples: ");	
        for (int i=0; i<badSamplesCount; i++)
        {
            badSamples.set1Value(i, findWorstSample());
          	fprintf(logfile, "%d  ", badSamples[i]);
        }
        fprintf(logfile, "\n");	

	    optimizeCamera();
        errorLog();
    }
	
    cerr << " optimizer: " << calculateAverageError() << " " << averageErrorThreshold << endl;
    return calculateAverageError() < averageErrorThreshold;
}


class eyepointTestdata {
	SbVec3f eyepoint;
	SbLine *rays;
	int numberOfRays;

	public:
		eyepointTestdata(SbVec3f ep,SbLine *r,int nofr) : eyepoint(ep), rays(r), numberOfRays(nofr)
			{};
		~eyepointTestdata(){};
		friend float testEyepoint(float t[]);
};


class eyepointTestdata *et;

float testEyepoint(float t[]) 
{
	float fret = 0;	
	et->eyepoint.setValue(t[1],t[2],t[3]);	

	// for eyery ray
	for (int i=0; i<et->numberOfRays; i++) 
		// fret += (shortest distance between ray[i] and eyepoint)^2
		fret += pow((et->rays[i].getClosestPoint(et->eyepoint) - et->eyepoint).
                    length(),2);

	return fret;
}


// returns optimized eyepoint
SbVec3f 
optimizeEyepoint(SbVec3f eyepoint, SbLine rays[], int nrOfRays) 
{
	et = new eyepointTestdata(eyepoint, rays, nrOfRays);

	const int n = 3;
	float start[n+1] = {0,eyepoint[0],eyepoint[1],eyepoint[2]};

	float *p = vector(1,n);
	float **xi = matrix(1,n,1,n);

	// init p and xi
	int r,c;
	for (r=1; r<=n; r++) {
		p[r] = start[r];
		for (c=1; c<=n; c++) 
			xi[r][c] = r==c ? 1 : 0;
	}

	int iter;
	float fret;	
	// POWELL
	powell(p, xi,n,0.00001,&iter,&fret,testEyepoint);

	eyepoint.setValue(p[1],p[2],p[3]);

	delete(et);
	free_vector(p,1,n);
	free_matrix(xi,1,n,1,n);

	return eyepoint;
}


// calculate intersection Position 
// if no valid distance occured the vector (-1,-1,-1) is returned
SbVec3f 
calculateEyepointPosition(SoMFVec3f &sampledPoints, 
                          float validDistance, 
                          SbBool opt)
{
    int i,j,cuts = 0;
	int validCuts = 0;
    int numberOfRays = sampledPoints.getNum()/SPPRP;
	float length;
	SbVec3f intersectionPoint(0,0,0), cutPos;
    SbVec3f closest, pOnLine1, pOnLine2;

    assert(validDistance > 0);

    // init rays
    SbLine rays[MAX_RAYS];
	for (i=0; i<numberOfRays; i++)
        rays[i].setValue(sampledPoints[i+numberOfRays],sampledPoints[i]);

	for (i=0; i<numberOfRays; i++)
		for (j=i+1; j<numberOfRays; j++) 
        {
			rays[i].getClosestPoints(rays[j], pOnLine1, pOnLine2);
			cuts++;
			closest = pOnLine2 - pOnLine1;
			if (closest.length() <= validDistance) 
            {
				length = closest.normalize();
				cutPos = pOnLine1 + (closest*(float)(length/2.0));
				intersectionPoint += cutPos;
                validCuts++;
			}
		}

    if (validCuts < cuts)
        return SbVec3f(-1,-1,-1);
	
    intersectionPoint /= validCuts;

    if (opt)
        return optimizeEyepoint(intersectionPoint, rays, numberOfRays);
    else 
	    return intersectionPoint;
}


SbRotation 
estimateCameraOrientation(const SoMFVec3f &sampledPoints, 
                          const SoMFVec2f &referencePoints,
                          const SbVec3f &eyepointPosition,
                          SoSFVec3f &position)
{
    int numberOfRays = sampledPoints.getNum()/SPPRP;
    
    // init rays
    SbLine rays[MAX_RAYS];
    int helpIndex = 0;
    float xReference = -1;
    SbVec3f zDirection(0,0,0), yDirection(0,0,0), yDirHelp1, yDirHelp2;
	for (int i=0; i<numberOfRays; i++)
    {
        rays[i].setValue(eyepointPosition, sampledPoints[i]);
        zDirection += rays[i].getDirection();
        if (referencePoints[i][0] == xReference)
            helpIndex = i;
        else if (i == 0)
            xReference = referencePoints[i][0];
    }
    zDirection /= numberOfRays;
    zDirection = -zDirection;
    zDirection.normalize();

    position.setValue(eyepointPosition - 2*zDirection);
    SbPlane xyPlane(zDirection, position.getValue());
	xyPlane.intersect(rays[0], yDirHelp1);
    xyPlane.intersect(rays[helpIndex], yDirHelp2);
    if (referencePoints[0][1] > referencePoints[helpIndex][1])
        yDirection =  yDirHelp1 - yDirHelp2;
    else
        yDirection =  yDirHelp2 - yDirHelp1;
    yDirection.normalize();

	SbRotation oZ, oY, oC;
	
	// rotate from camera default z-axis to zDirection
	oZ.setValue(SbVec3f(0,0,1), zDirection);
	// get default y-direction in oZ-space
	oZ.multVec(SbVec3f(0,1,0), yDirHelp1);
    // rotate default y-direction in oZ-space to real yDirection
	oY.setValue(yDirHelp1, yDirection);
 
	oC = oZ * oY;

	return oC;
}

//-----------------------------------------------------------------------------------------

static SoMFVec3f planeSamples;

float testPlane(float t[]) 
{
	float fret = 0;	
    SbVec3f normal, pointOnPlane;
    normal.setValue(t[1],t[2],t[3]);
    pointOnPlane.setValue(t[4],t[5],t[6]);
    SbPlane testPlane(normal, pointOnPlane);

	// for eyery sample
	for (int i=0; i<planeSamples.getNum(); i++) 
    {
        SbLine testLine;
        SbVec3f intersection;
        testLine.setValue(planeSamples[i], planeSamples[i] + normal);
        testPlane.intersect(testLine, intersection);
        float d = (planeSamples[i] - intersection).length();
		// fret += (shortest distance between point and plane)^2
		fret += d*d;
    }
	return fret;
}


// returns optimized plane
SbBool
optimizePlane(const SoMFVec3f &samples, SbPlane &plane, SoMFVec3f &pointsOnPlane) 
{
    if (samples.getNum() < 3)
        return FALSE;
    
    plane = SbPlane(samples[0],samples[1],samples[2]);    

        printf("pp-normal: %f %f %f\n", plane.getNormal()[0],
        plane.getNormal()[1], plane.getNormal()[2]);


    if (samples.getNum() == 3)
    {
        pointsOnPlane = samples;
        return TRUE;
    }

    planeSamples = samples;
	const int n = 6;
	float start[n+1] = {0,  plane.getNormal()[0], 
                            plane.getNormal()[1],
                            plane.getNormal()[2],
                            samples[0][0],
                            samples[0][1],
                            samples[0][2]};

	float *p = vector(1,n);
	float **xi = matrix(1,n,1,n);

	// init p and xi
	int r,c;
	for (r=1; r<=n; r++) {
		p[r] = start[r];
		for (c=1; c<=n; c++) 
			xi[r][c] = r==c ? 1 : 0;
	}

	int iter;
	float fret;	
	// POWELL
	powell(p, xi,n,0.00001,&iter,&fret,testPlane);

    SbVec3f normal, pointOnPlane;
    normal.setValue(p[1],p[2],p[3]);
    pointOnPlane.setValue(p[4],p[5],p[6]);

	free_vector(p,1,n);
	free_matrix(xi,1,n,1,n);

    plane = SbPlane(normal, pointOnPlane);

    printf("pp-normal (optimized): %f %f %f\n", plane.getNormal()[0],
        plane.getNormal()[1], plane.getNormal()[2]);

    // project points onto plane
    for (int i=0; i<samples.getNum(); i++)
    {
        SbLine line;
        SbVec3f intersection;
        line.setValue(samples[i], samples[i] + normal);
        plane.intersect(line, intersection);
        pointsOnPlane.set1Value(i, intersection);
    }
        
	return TRUE;
}
