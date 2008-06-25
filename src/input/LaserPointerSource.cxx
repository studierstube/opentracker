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

#include <OpenTracker/input/LaserPointerSource.h>
#ifdef USE_LASERPOINTER

#include <tinyxml/tinyxml.h>
#include <vector>
#include <cmath>

namespace ot {

    typedef struct 
    {
        unsigned long   UID;
        tPvHandle       Handle;
        tPvFrame        Frame[2];
        bool            Stop;
    } tCamera;

    tCamera GCamera;


    LaserPointerSource::LaserPointerSource() : Node(), 
        publishEvent(false),
        process(false)
    {
        initCamera = false;
        initTracker = false;
        initScreen = false;
        threshold=0;
    }


    LaserPointerSource::~LaserPointerSource()
    {
        std::cerr  << "Stopping image acquisition ..." << std::endl;
        PvCommandRun(GCamera.Handle,"AcquisitionStop");
        PvCaptureEnd(GCamera.Handle);
    }


    bool
    LaserPointerSource::initializeCamera(int exposure)
    { 
        PvInitialize();
        int width=1024;
        int height=768;
        imgBuffer=new LaserPointerBuffer(width, height, this);

        memset(&GCamera, 0, sizeof(tCamera));

        printf("Waiting for camera ... \n");
        while (!PvCameraCount() && !GCamera.Stop)
            ::Sleep(250);

        tPvUint32 count,connected;
        tPvCameraInfo list;
        count = PvCameraList(&list,1,&connected);

        if(count == 1)
        {
            GCamera.UID = list.UniqueId;
            printf("grabbing camera %s\n",list.SerialString);
        }
        else
            return false;

        printf("Setting up camera ... ");

        if (!PvCameraOpen(GCamera.UID,ePvAccessMaster,&(GCamera.Handle)))
        {
            printf("success.\n");
        }
        else
        {
            printf(" failed.\n");
            return false;
        }

        tPvErr sErr;

        std::cerr << "Setting pixel format to Rgb24 " << std::endl;
        sErr = setCamParameter(GCamera.Handle, "PixelFormat", "Rgb24");
        if (sErr) { handlePvErr(sErr); return false; }

        std::cerr << "Setting ExposureMode to Manual " << std::endl;
        sErr = setCamParameter(GCamera.Handle, "ExposureMode", "Manual");
        if (sErr) { handlePvErr(sErr); return false; }

        std::cerr << "Setting exposure to: " << exposure << std::endl;
        sErr =PvAttrUint32Set(GCamera.Handle, "ExposureValue", exposure);
        if (sErr) { handlePvErr(sErr); return false; }

        std::cerr << "Setting Gain Value to 0 " << std::endl;
        sErr = setCamParameter(GCamera.Handle, "GainValue", "0");
        if (sErr) { handlePvErr(sErr); return false; }

        std::cerr << "Setting FrameRate Value to 30 " << std::endl;
        sErr = setCamParameter(GCamera.Handle, "FrameRate", "30");
        if (sErr) { handlePvErr(sErr); return false; }

        std::cerr << "Setting WhitebalMode Value to AutoOnce " << std::endl;
        sErr = setCamParameter(GCamera.Handle, "WhitebalMode", "AutoOnce");
        if (sErr) { handlePvErr(sErr); return false; }
       
        std::cerr << "Setting Width: " <<  width << std::endl;
        sErr =PvAttrUint32Set(GCamera.Handle, "Width", width);
        if (sErr) { handlePvErr(sErr); return false; }

        std::cerr << "Setting Height: " << height << std::endl;
        sErr =PvAttrUint32Set(GCamera.Handle, "Height", height);
        if (sErr) { handlePvErr(sErr); return false; }

        std::cerr << "Setting up image frame buffers ..." << std::endl;
        unsigned long FrameSize = 0;

        // how big should the frame buffers be?
        if(!PvAttrUint32Get(GCamera.Handle,"TotalBytesPerFrame",&FrameSize))
        {
            // allocate the buffer for the frame
            GCamera.Frame[0].ImageBuffer = new char[FrameSize];
            GCamera.Frame[1].ImageBuffer = new char[FrameSize];
            if (GCamera.Frame[0].ImageBuffer)
            {
                std::cout << "first succeed.  size: " << FrameSize <<std::endl;
                GCamera.Frame[0].ImageBufferSize = FrameSize; 

                GCamera.Frame[1].ImageBuffer = new char[FrameSize];
                if (GCamera.Frame[1].ImageBuffer)
                {
                    std::cout << "second succeeded. size: " << FrameSize <<std::endl;
                    GCamera.Frame[1].ImageBufferSize = FrameSize; 
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }            
        }

        if(!PvCaptureStart(GCamera.Handle))
        {
            std::cerr << "Setting AcquisitionMode to Continuous " << std::endl;
            sErr = setCamParameter(GCamera.Handle, "AcquisitionMode", "Continuous");
            if (sErr) { handlePvErr(sErr); return false; }

            // start the acquisition and make sure the trigger mode is "freerun"
            if(PvCommandRun(GCamera.Handle,"AcquisitionStart"))
            {
                std::cerr << "AcquisitionStart failed ..." << std::endl;
                PvCaptureEnd(GCamera.Handle) ;
            }
        }
        else
        {
            std::cerr << "PvCaptureStart failed ..." << std::endl;    
        }

        initCamera=true;
        return true;
    }


    void 
    LaserPointerSource::initializeTracker()
    {
        int x(0);
        int y(0);
        std::vector<rawPoint> rawPoints;
        while(!x||!y)
        {
            captureFrame();
            int i,p;
            for (i=0, p=0; i<3*imgBuffer->width*imgBuffer->height; i=i+3, p++)
            {
                float strength =((float)imgBuffer->buffer[i]+
                                 (float)imgBuffer->buffer[i+1]+
                                 (float)imgBuffer->buffer[i+2])/3;
                if(strength>threshold)
                {
                    rawPoint rp; 
                    rp.pixel = p;
                    rp.red=(int)imgBuffer->buffer[i];
                    rp.green=(int)imgBuffer->buffer[i+1];
                    rp.blue=(int)imgBuffer->buffer[i+2];
                    rp.value = strength;
                    rawPoints.push_back(rp);
                }
            }
            sort( rawPoints.begin(), rawPoints.end(), cmp );
            
            for (int i(0) ; i<rawPoints.size(); i++)
            {
                x+= rawPoints[i].pixel%imgBuffer->width;
                y+= rawPoints[i].pixel/imgBuffer->width;
                printf("x: %i y: %i ", rawPoints[i].pixel%imgBuffer->width, rawPoints[i].pixel/imgBuffer->width);
                printf("pixel %i value %f ", rawPoints[i].pixel, rawPoints[i].value);
                printf("%i %i %i \n",rawPoints[i].red, rawPoints[i].green, rawPoints[i].blue);
            }
        }
        x/=rawPoints.size();
        y/=rawPoints.size();
        printf("Raw Point x: %i y: %i \n", x, y);
        OSUtils::sleep(1000);
        initTracker =true;
    }

    void 
    LaserPointerSource::initializeScreen()
    {
        if(!initTracker)
            if(initCamera)
            {
                initializeTracker();
                return;
            }
        char c;
        printf("press c to calibrate screen\n");
        c=getchar();
        if(c!='c' && loadHm());
        else
        {
            float sourcePoints[4][3];
            float targetPoints[4][3] = {{0,0,1},{1,0,1},{1,1,1},{0,1,1}};

            initScreen=true;
            // do screen calibration
            printf("indicate top left corner ... ");
            getScreenPoint();
            sourcePoints[0][0] = rx;
            sourcePoints[0][1] = ry;
            sourcePoints[0][2] = 1;
            printf("captured\n");
            OSUtils::sleep(2000);
            printf("indicate top right corner ... ");
            getScreenPoint();
            sourcePoints[1][0] = rx;
            sourcePoints[1][1] = ry;
            sourcePoints[1][2] = 1;
            printf("captured\n");
            OSUtils::sleep(2000);
            printf("indicate bottom right corner ... ");
            getScreenPoint();
            sourcePoints[2][0] = rx;
            sourcePoints[2][1] = ry;
            sourcePoints[2][2] = 1;
            printf("captured\n");
            OSUtils::sleep(2000);       
            printf("indicate bottom left corner ... ");
            getScreenPoint();
            sourcePoints[3][0] = rx;
            sourcePoints[3][1] = ry;
            sourcePoints[3][2] = 1;
            printf("captured\n");

            SetHomographyMatrix(hm, sourcePoints, targetPoints);
            saveHm();
            printf("screen calibrated\n");
        }
    }


    void 
    LaserPointerSource::processFrame()
    {
        if(!initScreen) initializeScreen();

        // filter pixels
        int i(0);
        int p(0);
        int n(0);
        rx=0;
        ry=0;
        for (; i<3*imgBuffer->width*imgBuffer->height; i=i+3, p++)
        {
            float strength =((float)imgBuffer->buffer[i]+
                (float)imgBuffer->buffer[i+1]+
                (float)imgBuffer->buffer[i+2])/3;
            //float strength =((float)GCamera.Frame[0].ImageBuffer[i]+
            //                (float)GCamera.Frame[0].ImageBuffer[i+1]+
            //                (float)GCamera.Frame[0].ImageBuffer[i+2])/3;
            if(strength>threshold)
            {
                rx+= p%imgBuffer->width;
                ry+= p/imgBuffer->width;
                n++;
            }
        }
        if(rx||ry)
        {
            rx/=n;
            ry/=n;
            // compute screen point
            float x = hm[0] * rx + hm[4] * ry + hm[12]; 
            float y = hm[1] * rx + hm[5] * ry + hm[13]; 
            float w = hm[3] * rx + hm[7] * ry + hm[15]; 
            // put result into event
            event.getPosition()[0]=x / w;
            event.getPosition()[1]=y / w;
            event.timeStamp();

            if(initScreen) process = true;
        }
        DWORD timeNow = GetTickCount(), diffTime = timeNow-frameRateLastTime;
        if(diffTime>1000 && frameRateCtr>0)	// update every second
        {
            frameRate = frameRateCtr*1000.0f/diffTime;
            logPrint("FPS: %.f \n", frameRate);
            frameRateLastTime = timeNow;
            frameRateCtr = 0;
        }
        else
            frameRateCtr++;
    }


    void
    LaserPointerSource::getScreenPoint()
    {
        rx=0;
        ry=0;
        while(!rx || !ry)
        {
            captureFrame();
            processFrame();
        }
    }


    void 
    LaserPointerSource::captureFrame()
    {
        tPvErr Err = ePvErrSuccess;
        Err = PvCaptureQueueFrame(GCamera.Handle,&GCamera.Frame[0],LaserPointerSource::FrameDoneCB);
        if(!Err)
        {
            Err = PvCaptureWaitForFrameDone(GCamera.Handle,&GCamera.Frame[0],PVINFINITE);
            if(!Err)
            {
                if(GCamera.Frame[0].Status == ePvErrCancelled)
                    Err = ePvErrCancelled;
            }            
        }
        if(Err)
        {
            printf("if(Err) \n ");
            handlePvErr(Err);
            GCamera.Stop = true;
        }
        memcpy(const_cast<unsigned char*>(imgBuffer->buffer),GCamera.Frame[0].ImageBuffer,imgBuffer->width*imgBuffer->height*3);
    }


    void _STDCALL LaserPointerSource::FrameDoneCB(tPvFrame* pFrame)
    {
        //LaserPointerSource* lps = (LaserPointerSource*)pFrame->Context[0];
        // if the frame was completed we re-enqueue it
        //if(pFrame->Status != ePvErrUnplugged && pFrame->Status != ePvErrCancelled)
        //{
            //memcpy(const_cast<unsigned char*>(lps->imgBuffer->buffer),GCamera.Frame[0].ImageBuffer,lps->imgBuffer->width*lps->imgBuffer->height*3);
            //PvCaptureQueueFrame(GCamera.Handle,&GCamera.Frame[0],LaserPointerSource::FrameDoneCB);
        //}
        //printf("frame done!\n");
    }


    void 
    LaserPointerSource::GetInverseMatrix(Matrix3x3 &copyMatrix, Matrix4x4 m)
    {
        SetMatrix(copyMatrix, m); 

        float det = 
            m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + 
            m[2] * m[3] * m[7] - m[0] * m[5] * m[7] - 
            m[1] * m[3] * m[8] - m[2] * m[4] * m[6]; 

        copyMatrix[0] = (m[4] * m[8] - m[5] * m[7]) / det; 
        copyMatrix[1] = (m[2] * m[7] - m[1] * m[8]) / det; 
        copyMatrix[2] = (m[1] * m[5] - m[2] * m[4]) / det; 
        copyMatrix[3] = (m[5] * m[6] - m[3] * m[8]) / det; 
        copyMatrix[4] = (m[0] * m[8] - m[2] * m[6]) / det; 
        copyMatrix[5] = (m[2] * m[3] - m[0] * m[5]) / det; 
        copyMatrix[6] = (m[3] * m[7] - m[4] * m[6]) / det; 
        copyMatrix[7] = (m[1] * m[6] - m[0] * m[7]) / det; 
        copyMatrix[8] = (m[0] * m[4] - m[1] * m[3]) / det; 
    }


    void 
    LaserPointerSource::SetHomographyMatrix(Matrix4x4 &output, float sourcePoints[4][3], float targetPoints[4][3])
    {
        Matrix3x3 sourceMatrix, targetMatrix, inverseSourceMatrix; 
        SetUnitSquareHomographyMatrix(sourceMatrix, sourcePoints[0][0], sourcePoints[1][0], 
            sourcePoints[2][0], sourcePoints[3][0], 
            sourcePoints[0][1], sourcePoints[1][1], 
            sourcePoints[2][1], sourcePoints[3][1]); 
        SetUnitSquareHomographyMatrix(targetMatrix, targetPoints[0][0], targetPoints[1][0], 
            targetPoints[2][0], targetPoints[3][0], 
            targetPoints[0][1], targetPoints[1][1], 
            targetPoints[2][1], targetPoints[3][1]); 
        GetInverseMatrix(inverseSourceMatrix, sourceMatrix); 
        ConcatHomographyMatrizes(output, inverseSourceMatrix, targetMatrix); 
    }

    void 
    LaserPointerSource::SetUnitSquareHomographyMatrix(Matrix3x3 &output, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4){
        SetZeroMatrix(output); 
        float S = (x2 - x3) * (y4 - y3) - (x4 - x3) * (y2 - y3); 

        float a31 = ((x1 - x2 + x3 - x4) * (y4 - y3) - (y1 - y2 + y3 - y4) * (x4 - x3)) / S; 
        float a32 = ((y1 - y2 + y3 - y4) * (x2 - x3) - (x1 - x2 + x3 - x4) * (y2 - y3)) / S; 

        float a11 = x2 - x1 + a31 * x2; 
        float a12 = x4 - x1 + a32 * x4; 
        float a13 = x1; 

        float a21 = y2 - y1 + a31 * y2; 
        float a22 = y4 - y1 + a32 * y4; 
        float a23 = y1; 

        output[0] = a11; 
        output[1] = a12; 
        output[2] = a13; 
        output[3] = a21; 
        output[4] = a22; 
        output[5] = a23; 
        output[6] = a31; 
        output[7] = a32; 
        output[8] = 1.0f; 
    }

    void 
    LaserPointerSource::ConcatHomographyMatrizes(Matrix4x4 &output, Matrix3x3 sourceMatrix, Matrix3x3 targetMatrix)
    {
        output[0] = targetMatrix[0] * sourceMatrix[0] + targetMatrix[1] * sourceMatrix[3] + targetMatrix[2] * sourceMatrix[6]; 
        output[4] = targetMatrix[0] * sourceMatrix[1] + targetMatrix[1] * sourceMatrix[4] + targetMatrix[2] * sourceMatrix[7]; 
        output[12] = targetMatrix[0] * sourceMatrix[2] + targetMatrix[1] * sourceMatrix[5] + targetMatrix[2] * sourceMatrix[8]; 

        output[1] = targetMatrix[3] * sourceMatrix[0] + targetMatrix[4] * sourceMatrix[3] + targetMatrix[5] * sourceMatrix[6]; 
        output[5] = targetMatrix[3] * sourceMatrix[1] + targetMatrix[4] * sourceMatrix[4] + targetMatrix[5] * sourceMatrix[7]; 
        output[13] = targetMatrix[3] * sourceMatrix[2] + targetMatrix[4] * sourceMatrix[5] + targetMatrix[5] * sourceMatrix[8]; 

        output[3] = targetMatrix[6] * sourceMatrix[0] + targetMatrix[7] * sourceMatrix[3] + targetMatrix[8] * sourceMatrix[6]; 
        output[7] = targetMatrix[6] * sourceMatrix[1] + targetMatrix[7] * sourceMatrix[4] + targetMatrix[8] * sourceMatrix[7]; 
        output[15] = targetMatrix[6] * sourceMatrix[2] + targetMatrix[7] * sourceMatrix[5] + targetMatrix[8] * sourceMatrix[8]; 

        //ignore z-axis
        output[2] = 0.0; 
        output[6] = 0.0; 
        output[8] = 0.0; 
        output[9] = 0.0; 
        output[10] = 1.0; 
        output[11] = 0.0; 
        output[14] = 0.0; 
    }

    void LaserPointerSource::SetMatrix(Matrix4x4 &m1, Matrix4x4 m2)
    {
        for(int i = 0; i < 16; i++){
            m1[i] = m2[i]; 
        }
    }

    void LaserPointerSource::SetMatrix(Matrix3x3 &m1, Matrix3x3 m2)
    {
        for(int i = 0; i < 9; i++){
            m1[i] = m2[i]; 
        }
    }
    void LaserPointerSource::SetZeroMatrix(Matrix4x4 &m)
    {
        for(int i = 0; i < 16; i++){
            m[i] = 0; 
        }
    }

    void LaserPointerSource::SetZeroMatrix(Matrix3x3 &m)
    {
        for(int i = 0; i < 9; i++){
            m[i] = 0; 
        }
    }

    //BOOL WINAPI LaserPointerSource::CtrlCHandler(DWORD dwCtrlType)
    //{
    //    printf("interrupting!\n");    
    //    GCamera.Stop = true;    
    //    return true;
    //}


    //void _STDCALL LaserPointerSource::CameraEventCB(void* Context,
    //    tPvInterface Interface,
    //    tPvLinkEvent Event,
    //    unsigned long UniqueId)
    //{
    //    switch(Event)
    //    {
    //    case ePvLinkAdd:
    //        {
    //            printf("camera %lu plugged\n",UniqueId);
    //            break;
    //        }
    //    case ePvLinkRemove:
    //        {
    //            printf("camera %lu unplugged\n",UniqueId);
    //            break;
    //        }
    //    default:
    //        break;
    //    }
    //}
	
    // Process Data received from incomming Events
    void LaserPointerSource::onEventGenerated( Event& event, Node& generator) 
    {
        
    }


    void LaserPointerSource::pushEvent()
    {
        if (process)
        {
            process = false;
            updateObservers(event);
        }
    }
    
    void LaserPointerSource::pullEvent()
    {
    }

    void LaserPointerSource::push()
    {
      
    }

    tPvErr 
        LaserPointerSource::setCamParameter(tPvHandle Camera,const char* Label,const char* Value)
    {
        tPvErr              Err;
        tPvAttributeInfo    Info;

        if(!(Err = PvAttrInfo(Camera,Label,&Info)))
            switch(Info.Datatype)
        {
            case ePvDatatypeEnum:
                {
                    Err = PvAttrEnumSet(Camera,Label,Value);

                    break;
                }
            case ePvDatatypeUint32:
                {
                    tPvUint32 Uint;

                    if(sscanf(Value,"%lu",&Uint) == 1)
                        Err = PvAttrUint32Set(Camera,Label,Uint);
                    else
                        Err = ePvErrWrongType;

                    break;
                }
            case ePvDatatypeFloat32:
                {
                    tPvFloat32 Float;

                    if(sscanf(Value,"%f",&Float) == 1)
                        Err = PvAttrFloat32Set(Camera,Label,Float);
                    else
                        Err = ePvErrWrongType;

                    break;
                }
            default:
                break;
        }

        return Err;
    }

    void 
        LaserPointerSource::handlePvErr(tPvErr Err)
    {
        if(Err)
        {
            switch(Err)
            {
            case ePvErrNotFound:
                {
                    fprintf(stderr,"this attribute was not found\n");
                    break;
                }
            case ePvErrOutOfRange:
                {
                    fprintf(stderr,"the value is out of range\n");
                    break;
                }
            case ePvErrWrongType:
                {
                    fprintf(stderr,"this attribute is not of this type\n");
                    break;
                }
            default:
                fprintf(stderr,"sorry, an error occured (%u)\n",Err);
            }
        }
    }

    bool 
    LaserPointerSource::cmp( rawPoint a, rawPoint b ) {
            return a.value > b.value;
    }   

    void
    LaserPointerSource::saveHm()
    {
        TiXmlDocument* document = new TiXmlDocument();
        TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
        document->LinkEndChild( decl );
        TiXmlElement * element = new TiXmlElement( "homographyMatrix" );
        document->LinkEndChild( element );
        TiXmlElement * set = new TiXmlElement( "matrix" );
        element->LinkEndChild( set );
        char  tmpChar[20];
        sprintf(tmpChar, "%f", hm[0] );
        set->SetAttribute("zero", tmpChar);
        sprintf(tmpChar, "%f", hm[1] );
        set->SetAttribute("one", tmpChar);
        sprintf(tmpChar, "%f", hm[2] );
        set->SetAttribute("two", tmpChar);
        sprintf(tmpChar, "%f", hm[3] );
        set->SetAttribute("three", tmpChar);
        sprintf(tmpChar, "%f", hm[4] );
        set->SetAttribute("four", tmpChar);
        sprintf(tmpChar, "%f", hm[5] );
        set->SetAttribute("five", tmpChar);
        sprintf(tmpChar, "%f", hm[6] );
        set->SetAttribute("six", tmpChar);
        sprintf(tmpChar, "%f", hm[7] );
        set->SetAttribute("seven", tmpChar);
        sprintf(tmpChar, "%f", hm[8] );
        set->SetAttribute("eight", tmpChar);
        sprintf(tmpChar, "%f", hm[9] );
        set->SetAttribute("nine", tmpChar);
        sprintf(tmpChar, "%f", hm[10] );
        set->SetAttribute("ten", tmpChar);
        sprintf(tmpChar, "%f", hm[11] );
        set->SetAttribute("eleven", tmpChar);
        sprintf(tmpChar, "%f", hm[12] );
        set->SetAttribute("twelve", tmpChar);
        sprintf(tmpChar, "%f", hm[13] );
        set->SetAttribute("thirteen", tmpChar);
        sprintf(tmpChar, "%f", hm[14] );
        set->SetAttribute("fourteen", tmpChar);
        sprintf(tmpChar, "%f", hm[15]  );
        set->SetAttribute("fiveteen", tmpChar);
        document->SaveFile("laserPointerCalib.xml");
    }


    bool
    LaserPointerSource::loadHm()
    {
        TiXmlDocument* document = new TiXmlDocument();

        if(!document->LoadFile("laserPointerCalib.xml"))
        {
            logPrintE("laserPointerCalib.xml does not exist \n");
            logPrint(" Message: ");
            logPrint(document->ErrorDesc());
            logPrint("\n");
            return false;
        }

        TiXmlElement* root = document->RootElement();
        if(!_stricmp(root->Value(),"homographyMatrix"))
        {
            //parse file
            TiXmlElement* element = root->FirstChildElement();
            while(element)
            {
                if(!_stricmp(element->Value(),"matrix"))
                {
                    TiXmlAttribute* attribute = element->FirstAttribute();
                    while(attribute) ////////////  parameter's
                    {
                        if(!_stricmp(attribute->Name(),"zero"))
                            hm[0]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"one"))
                            hm[1]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"two"))
                            hm[2]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"three"))
                            hm[3]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"four"))
                            hm[4]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"five"))
                            hm[5]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"six"))
                            hm[6]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"seven"))
                            hm[7]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"eight"))
                            hm[8]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"nine"))
                            hm[9]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"ten"))
                            hm[10]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"eleven"))
                            hm[11]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"twelve"))
                            hm[12]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"thirteen"))
                            hm[13]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"fourteen"))
                            hm[14]=(float)atof(attribute->Value());
                        else if(!_stricmp(attribute->Name(),"fiveteen"))
                            hm[15]=(float)atof(attribute->Value());

                        attribute = attribute->Next();
                    }
                }
                element = element->NextSiblingElement();
            }
        }
        document->Clear();
        delete document;
        initScreen=true;
        return true;
    }

}

#endif //USE_LASERPOINTER
