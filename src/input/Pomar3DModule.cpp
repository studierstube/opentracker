/*------------------------------------------------------------------------------
 * Pomar3D
 * Customer: ICG
 
 *------------------------------------------------------------------------------

 * Supplier:
 * Institute of Navigation and Satellite Geodesy
 * Graz University of Technology
 * Steyrergasse 30
 * 8010 Graz
 * Austria

 *------------------------------------------------------------------------------

 * Copyright (C) 2008 Institute of Navigation and Satellite Geodesy
 * All rights reserved. 

 * THE AUTHORING COMPANIES DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
 * SHALL THE AUTHORING COMPANY BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUEN-
 * TIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR 
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS AC-
 * TION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS 
 * SOFTWARE. 
 
 *------------------------------------------------------------------------------

 * File Type:   C/C++ File
 * File Name:   Pomar3DModule.cpp
 * Authors:     P. Berglez 
 * Description: Implementation file for the DLL 
 
 *------------------------------------------------------------------------------

 * Versions / History
 * Version      Date      Author        Comment
 * 01.00.00.00  25062008  P. Berglez      First issue
 *----------------------------------------------------------------------------*/ 
#ifdef WIN32
#pragma warning(disable:4244)
#endif

#pragma once

#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Thread.h>
#include <ace/FILE_Connector.h>
#include <ace/FILE_IO.h>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/input/Pomar3DModule.h>
#include <OpenTracker/input/Pomar3DSource.h>
/*#include <OpenTracker/input/GPSSource.h>
#include <OpenTracker/input/GPSDirectionSource.h>
#include <OpenTracker/input/GPSGarminCompass.h>
#include <OpenTracker/input/GPSGarminAltitude.h>
#include <OpenTracker/input/GPSInfoSource.h>*/

#include <iostream>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

namespace ot 
{

	OT_MODULE_REGISTER_FUNC(Pomar3DModule){
		OT_MODULE_REGISTRATION_DEFAULT(Pomar3DModule, "Pomar3DConfig");
	}

	//--------------------------------------------
	// Pomar3DModule Constructor 
	//--------------------------------------------
	Pomar3DModule::Pomar3DModule() : source( NULL )
	{

		Pomar3D_reactor = (void *)new ACE_Reactor;

		m_success = false;
		m_first_run = true;
		hEkfDLL = NULL;

		hEkfDLL = LoadLibrary("Pomar3D_EKF_DLL.dll");

		hXsensDLL = NULL;
		hXsensDLL = LoadLibrary("XsensCMT.dll");

		memset(&EKFDLLparams,'\0',sizeof(SDLLparams));
		memset(&StartEKFDLLparams,'\0',sizeof(SStartDLLparams));

		StartEKFDLLparams.pDLLparams = &EKFDLLparams;
		StartEKFDLLparams.pInitEKFDLL = NULL;
		StartEKFDLLparams.pStartEKFDLL = NULL;
		StartEKFDLLparams.pPushEventEKFDLL = NULL;
		StartEKFDLLparams.pCloseEKFDLL = NULL;

		if (hXsensDLL == NULL) 
		{    
			//			fprintf(fid,"XXX1 种种种种种种种种种种种种种种謀n");
			LOG_ACE_ERROR("Fatal error Constructor: XSENS-DLL not found - programme terminated!\n");
			//printf("Fatal error: XSENS-DLL not found - programme terminated!\n");
		}

		if (hEkfDLL == NULL) 
		{   
			//			fprintf(fid,"XXX2 种种种种种种种种种种种种种种謀n");
			LOG_ACE_ERROR("Fatal error Constructor: EKF-DLL not found - programme terminated!\n");
			//printf("Fatal error: EKF-DLL not found - programme terminated!\n");
		}
	}

	//--------------------------------------------
	// Pomar3DModule Destructor 
	//--------------------------------------------

	Pomar3DModule::~Pomar3DModule()
	{
		if( source != NULL )
			delete source;
		if ( Pomar3D_reactor != NULL)
			delete (ACE_Reactor *)Pomar3D_reactor;
	}

	//--------------------------------------------
	// Create Node 
	//--------------------------------------------
	Node * Pomar3DModule::createNode( const std::string & name, const StringTable & attributes )
	{
		if( name.compare("Pomar3DSource") == 0 )
		{
			if( source != NULL )
			{
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one Pomar3DSource can be build !\n")));
				return NULL;
			}
			if( !isInitialized() )
			{
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Pomar3DModule is not initialized, cannot build Pomar3DSource !\n")));
				return NULL;
			}
			source = new Pomar3DSource;
			logPrintI("Built Pomar3DSource node.\n");
			return source;
		}    
		return NULL;	
	}

	void Pomar3DModule::init(StringTable& attributes,  ConfigNode * localTree)
	{
		
		Module::init( attributes, localTree );
        headtracking = FALSE;

		StringTable & localAttrib= localTree->getAttributes();
        if( localAttrib.get("headtracking").compare("true") == 0 )
            headtracking=TRUE;

		StartEKFDLLparams.pInitEKFDLL = (INITEKFDLL)GetProcAddress(hEkfDLL,(LPCSTR)"InitEKFDLL");
		if (StartEKFDLLparams.pInitEKFDLL == NULL) 
		{
			LOG_ACE_ERROR("Fatal error: EKF-DLL not found - programme terminated!\n");
			//printf("Fatal error: Could not access EKF-DLL - programme terminated!\n");
		}
		StartEKFDLLparams.pStartEKFDLL = (STARTEKFDLL)GetProcAddress(hEkfDLL,(LPCSTR)"StartEKFDLL");
		if (StartEKFDLLparams.pStartEKFDLL == NULL) 
		{
			LOG_ACE_ERROR("Fatal error: EKF-DLL not found - programme terminated!\n");
			//printf("Fatal error: Could not access EKF-DLL - programme terminated!\n");
		}
		StartEKFDLLparams.pPushEventEKFDLL = (PUSHEVENTEKFDLL)GetProcAddress(hEkfDLL,(LPCSTR)"PushEventEKFDLL");
		if (StartEKFDLLparams.pPushEventEKFDLL == NULL) 
		{
			LOG_ACE_ERROR("Fatal error: EKF-DLL not found - programme terminated!\n");
			// printf("Fatal error: Could not access EKF-DLL - programme terminated!\n");
		}
		StartEKFDLLparams.pCloseEKFDLL = (CLOSEEKFDLL)GetProcAddress(hEkfDLL,(LPCSTR)"CloseEKFDLL");
		
		if (StartEKFDLLparams.pCloseEKFDLL == NULL) 
		{
			LOG_ACE_ERROR("Fatal error: EKF-DLL CLOSE not found - programme terminated!\n");
			// printf("Fatal error: Could not access EKF-DLL - programme terminated!\n");
		}
		//fclose(fid);


		return;

	}


	void Pomar3DModule::start()
	{
		//FILE *fid;
		//fid=fopen("TEST.txt","a");
		//fprintf(fid,"START Routine\n");
		m_success = StartEKFDLLparams.pInitEKFDLL(&EKFDLLparams);
		//fprintf(fid,"START Routine response %i\n",(int)m_success);
		//if(!m_success)
		//{
		//	//fprintf(fid,"No Success U\n");
		//	//exit 1;
		//}
		//else
		//{
		//	//fprintf(fid,"JOP\n");
		//}

		ThreadModule::start();

		//fprintf(fid,"ThreadModule Start\n");
		_beginthread(StartEKFDLLparams.pStartEKFDLL,0,(void*)&EKFDLLparams);
		//fprintf(fid,"EKFDLL Thread Start\n");
		return;
	}

	void Pomar3DModule::close()
	{
		//FILE *fid;
		//fid=fopen("TEST.txt","a");
		//fprintf(fid,"CLOSE Start\n");
		StartEKFDLLparams.pCloseEKFDLL((void*)&EKFDLLparams);		
		ThreadModule::close();
		//fprintf(fid,"CLOSE End\n");
		//fclose(fid);
		

	/*	Pomar3D_reactor->end_reactor_event_loop();*/
        /*if( logFile != NULL )
            logFile->close();*/

	}

	void Pomar3DModule::run()
	{
		//driver = new GPSDriver( (ACE_Reactor *)gps_reactor );


		//FILE *fid;
		//fid=fopen("TEST.txt","a");
		//fprintf(fid,"RUN Routine\n");

		////////////m_success = StartEKFDLLparams.pInitEKFDLL(&EKFDLLparams);
		////////////fprintf(fid,"START Routine response %i\n",(int)m_success);
		////////////if(!m_success)
		////////////{
		////////////	fprintf(fid,"No Success U\n");
		////////////}
		////////////else
		////////////{
		////////////	fprintf(fid,"JOP\n");
		////////////}


		//////		if(m_success)
		//////		{
		//////			// Starts the Main Processing Thread
		//////		fprintf(fid,"RUN Routine\n");
		////////		_beginthread(StartEKFDLLparams.pStartEKFDLL,0,(void*)&EKFDLLparams);


		//fprintf(fid,"END RUN Routine\n");
		//fclose(fid);
	}

	void Pomar3DModule::pushEvent()
	{

		SOutput Pomar3DOutput;
		memset(&Pomar3DOutput,'\0',sizeof(Pomar3DOutput));

		buffer.timeStamp();

		float ECEF[3];
		float ORI[4];
		float ORI_0=0.0;
		float ORI_1=0.0;
		float ORI_2=0.0;
		float ORI_3=0.0;
	
		double roll, pitch, yaw;

		std::vector<float> ECEF1;
		std::vector<float> posnew;

		//posnew.resize(3);

		buffer.getConfidence() = (float)(1);

		StartEKFDLLparams.pPushEventEKFDLL(&Pomar3DOutput);		

		//// TEST
		//StartEKFDLLparams.pCloseEKFDLL();

		if(Pomar3DOutput.blnNewData)
		{
			ECEF[0] = (float)Pomar3DOutput.POS_PLH[0];
			ECEF[1] = (float)Pomar3DOutput.POS_PLH[1];
			ECEF[2] = (float)Pomar3DOutput.POS_PLH[2];


			// Use this conversion instead of Parameter transfer --> needs further work
			roll = Pomar3DOutput.roll * PI / 180.;
			pitch = Pomar3DOutput.pitch * PI / 180.;
			yaw = Pomar3DOutput.yaw * PI / 180.;


			ORI_0 = (float)( cos(roll*0.5) * cos(pitch*0.5) * cos(yaw*0.5) 
				+ sin(roll*0.5) * sin(pitch*0.5) * sin(yaw*0.5) );

			ORI_1 = (float)( sin(roll*0.5) * cos(pitch*0.5) * cos(yaw*0.5) 
				- cos(roll*0.5) * sin(pitch*0.5) * sin(yaw*0.5) );

			ORI_2 = (float)( cos(roll*0.5) * sin(pitch*0.5) * cos(yaw*0.5) 
				+ sin(roll*0.5) * cos(pitch*0.5) * sin(yaw*0.5) );

			ORI_3 = (float)( cos(roll*0.5) * cos(pitch*0.5) * sin(yaw*0.5) 
				+ sin(roll*0.5) * sin(pitch*0.5) * cos(yaw*0.5) );



            if (headtracking)
                      {
                            // If headtracking is activated, we send the "Real" component of the
                            // quaternion to the end. Plus we align the axes of the InertiaCubes
                            // so that the cable points in the positive Z direction. And its base
                            // is the ZX plane. Also, we switch from Left handed to Right handed
                            // coordinate system.
                           
							ORI[0] = ORI_2;
							ORI[1] = -ORI_3;
							ORI[2] = -ORI_1;
							ORI[3] = -ORI_0;
                          
                        }
                        else {
                            // The reason to switch the Quaterion[0] to the end is
                            // that the Intersense tracker provides the "Real" component of the
                            // quaternion as the first value, however, inventor assumes is the last value.
                            // If you find problems on your opentracker configuration files, is maybe
                            // because you found a hack to get around this problem. You should now
                            // be able to get rid of that hack. Mendez. 20061115.
							ORI[0] = ORI_0;
							ORI[1] = ORI_1;
							ORI[2] = ORI_2;
							ORI[3] = ORI_3;
                           
                        }

			


			// Update of parameters
			buffer.setOrientation(ORI);
			buffer.addAttribute<short>("numsats", Pomar3DOutput.numSVs);
			buffer.setPosition(ECEF);

			buffer.setConfidence((const float)Pomar3DOutput.shiPosStatus);


			// Update of OT
			source->updateObservers(buffer);
			Pomar3DOutput.blnNewData = false;
		}

		//// For 1Hz Output
		//Sleep(100);


	}

}


