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
/** header file for LaserPointerSource Node.
  *
  * @author Markus Sareika
  *
  * $Id: LaserPointerSource.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section LaserPointerSource LaserPointerSource
 * The LaserPointerSource node is a simple EventGenerator that outputs the
 * current position and button state of the captured laser  point. It is driven by
 * the @ref LaserPointerModule. 
 *
 * An example element looks like this :
 * @verbatim
<LaserPointerSource/>@endverbatim
 */

#ifndef _LASERPOINTERSOURCE_H
#define _LASERPOINTERSOURCE_H

#include <stdio.h>
#include <queue>


#include "../OpenTracker.h"
#include "../common/ConsoleSource.h"

#ifdef USE_LASERPOINTER

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define _STDCALL __stdcall
#endif


#include <PvApi.h>
#pragma comment(lib,"PvAPI")

#define FRAMESCOUNT 10

namespace ot {

/**
 * This class implements a source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Markus Sareika
 * @ingroup input
 */

class LaserPointerBuffer;

class OPENTRACKER_API LaserPointerSource : public Node
{
// Members
public: 

    /// the state that is posted to the EventObservers
    Event event;
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    /** pushes event down the line. */
    void push();
	void pushEvent( );
    void pullEvent( );

   

 // callback called when the camera is plugged/unplugged
    //static void _STDCALL CameraEventCB(void* Context, tPvInterface Interface, tPvLinkEvent Event,  unsigned long UniqueId);
    
    //static BOOL WINAPI CtrlCHandler(DWORD dwCtrlType);

    // callback called when a frame is done
    static void _STDCALL FrameDoneCB(tPvFrame* pFrame);

	/** simple constructor, sets members to initial values */
	LaserPointerSource();
	
	~LaserPointerSource();
	

virtual void onEventGenerated( Event& event, Node& generator);

protected:

	friend class LaserPointerModule;
   
    bool initializeCamera();
    int exposure, filter;
    float threshold, thresholdR, thresholdG, thresholdB;
    int width;
    int height;
    
    void captureFrame();
    void processFrame();

    LaserPointerBuffer* imgBuffer;
    bool publishEvent, process;

private:
    typedef struct 
    {
        int pixel;
        int x;
        int y;
        float value;
        int red;
        int green;
        int blue;

    } rawPoint;
    typedef float Matrix4x4[16]; 
    typedef float Matrix3x3[9]; 

    Matrix4x4 hm;

    int rx, ry;

    void initializeTracker();
    void initializeScreen();

    void getScreenPoint();
  
    static bool cmp( rawPoint a, rawPoint b );
    tPvErr setCamParameter(tPvHandle Camera,const char* Label,const char* Value);
    void handlePvErr(tPvErr Err);
    
    void GetInverseMatrix(Matrix3x3 &copyMatrix, Matrix4x4 m);
    void SetHomographyMatrix(Matrix4x4 &output, float sourcePoints[4][3], float targetPoints[4][3]);
    void SetUnitSquareHomographyMatrix(Matrix3x3 &output, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4);
    void ConcatHomographyMatrizes(Matrix4x4 &output, Matrix3x3 sourceMatrix, Matrix3x3 targetMatrix);
    void SetMatrix(Matrix4x4 &m1, Matrix4x4 m2);
    void SetMatrix(Matrix3x3 &m1, Matrix3x3 m2);
    void SetZeroMatrix(Matrix4x4 &m);
    void SetZeroMatrix(Matrix3x3 &m);

    void saveHm();
    bool loadHm();

	bool initCamera, initTracker,  initScreen;

    DWORD frameRateLastTime, frameRateCtr;
    float frameRate;
    
};

class LaserPointerBuffer
{
    friend class LaserPointerSource;
public:
    int width, height;
    std::string format, typeString;
    unsigned char* buffer;
    LaserPointerBuffer(int w, int h, LaserPointerSource *dsNode)
    {
        width=w;
        height=h;        
        buffer = new unsigned char[width*height*3];
        memset(buffer, 0, width*height*3);
        typeString="LaserPointerBuffer";
        node=dsNode;
    }

    ~LaserPointerBuffer()
    {
        //delete pixels;
        delete buffer;
        buffer = NULL;
    }

    unsigned char* getCurrentPxlBuffer(){return this->buffer;}

protected:
    LaserPointerSource *node;
};

}  // namespace ot

#endif //USE_LASERPOINTER

#endif //_LASERPOINTERSOURCE_H
