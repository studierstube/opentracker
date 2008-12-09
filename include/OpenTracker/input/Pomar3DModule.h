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

 * File Type:   C/C++ Header File
 * File Name:   Pomar3DModule.h
 * Authors:     P. Berglez 
 * Description: Implementation file for the DLL 
 
 *------------------------------------------------------------------------------

 * Versions / History
 * Version      Date      Author        Comment
 * 01.00.00.00  25062008  P. Berglez      First issue
 *----------------------------------------------------------------------------*/ 
//#ifndef _GPSMODULE_H
//#define _GPSMODULE_H

#include "../OpenTracker.h"
//#include "GPSDriver.h"

#include <string>

#define PI              3.14159265358979323e0 /* PI     */


class ACE_FILE_IO;


//#ifndef OT_NO_GPS_SUPPORT
#ifndef _POMAR3DMODULE_H
#define _POMAR3DMODULE_H

namespace ot {

	class Pomar3DSource;

	class OPENTRACKER_API Pomar3DModule : public ThreadModule , public NodeFactory
	{  //, public GPSListener
	public:
		Pomar3DModule();
		virtual ~Pomar3DModule();

		// virtual void init(StringTable& attributes,  ConfigNode * localTree);

		virtual Node * createNode( const std::string & name, const StringTable & attributes );

		virtual void pushEvent();

		virtual void start();

		virtual void close();

		//virtual void newData( const GPResult * point, const char * line, void * userData );


		Pomar3DSource *source;

	protected:

		/// Indicates whether the tracker is used for head tracking
        bool headtracking;

		virtual void run();

		void * Pomar3D_reactor;

	public:

		bool m_success;
		bool			m_first_run;
		HMODULE         hEkfDLL;
		HMODULE			hXsensDLL;


		// Kalman output structure
		typedef struct
		{

			bool            blnNewData;
			double			roll;	// Roll [deg]
			double			pitch;	// Pitch [deg]
			double			yaw;	// Yaw [deg]
			double			AR[3];	// angular rates [deg/s]
			double			ARd[3];	// first derivative of the angular rates [deg/s²]
			double			AR_bias[3]; // bias of the angular rates [deg/s]
			double			DEV;
			double			DEVd;
			double			POS_PLH[3]; // Position  phi [deg], lambda [deg], h [m]
			double			POSd_PLH[3]; // first derivative of the Position  phi [deg], lambda [deg], h [m]
			double			BAR_offset;	// barometric offset [m]

			unsigned short  shiPosStatus;

			float			ORI_RPY[3]; // Vector containing Roll Pitch Yaw
			float			QUAT[4];    // Quaternions calc. out of roll, pitch, yaw

			short           numSVs;     // Number of used SVs
			int				GPSweek;    // actual GPS Week
			double			GPSsecond;  // actual GPS Second

			double			STD_roll;	// STD of the Roll [deg]
			double			STD_pitch;	// STD of the Pitch [deg]
			double			STD_yaw;	// STD of the Yaw [deg]
			double			STD_AR[3];	// STD of the angular rates [deg/s]
			double			STD_ARd[3];	// STD of the first derivative of the angular rates [deg/s²]
			double			STD_AR_bias[3]; // STD of the bias of the angular rates [deg/s]
			double			STD_DEV;
			double			STD_DEVd;
			double			STD_POS_PLH[3]; // STD of the Position [deg] respectively [m]
			double			STD_POSd_PLH[3]; // STD of the first derivative of the Position  phi [deg], lambda [deg], h [m]
			double			STD_BAR_offset;	// STD of the barometric offset [m]

			//bool			blnNewData;

			//double			roll;	// Roll [deg]
			//double			pitch;	// Pitch [deg]
			//double			yaw;	// Yaw [deg]
			//double			AR[3];	// angular rates [deg/s]
			//double			DEV;	// deviation
			//double			DEVd;	// first derivative of the deviation
			//double			AR_bias[3]; // bias of the angular rates [deg/s]
			//double			ARd_bias[3]; // bias of the first derivative of the angular rates [deg/s²]
			//double			ARdd_bias; // bias of the second derivative of the yaw angular rate [deg/s³]
			//double			POS_PLH[3]; // Position  phi [deg], lambda [deg], h [m]
			//double			POSd_PLH[3]; // first derivative of the Position  phi [deg], lambda [deg], h [m]
			//double			lever_arm[3];	// lever arm correction dx, dy, dz [m]

			//short           numSVs;     // Number of used SVs
			//int				GPSweek;    // actual GPS Week
			//double			GPSsecond;  // actual GPS Second

			//float			ORI_RPY[3]; // Vector containing Roll Pitch Yaw
			//float			QUAT[4];    // Quaternions calc. out of roll, pitch, yaw

			//double			STD_roll;	// STD of the Roll [deg]
			//double			STD_pitch;	// STD of the Pitch [deg]
			//double			STD_yaw;	// STD of the Yaw [deg]
			//double			STD_AR[3];	// STD of the angular rates [deg/s]
			//double			STD_DEV;	// STD of the deviation
			//double			STD_DEVd;	// STD of the first derivative of the deviation
			//double			STD_AR_bias[3]; // STD of the bias of the angular rates [deg/s]
			//double			STD_ARd_bias[3]; // STD of the bias of the firt derivative of the angular rates [deg/s²]
			//double			STD_ARdd_bias; // STD of the bias of the second derivative of the yaw angular rate [deg/s³]
			//double			STD_POS_PLH[3]; // STD of the Position [deg] respectively [m]
			//double			STD_POSd_PLH[3]; // STD of the first derivative of the Position  phi [deg], lambda [deg], h [m]
			//double			STD_lever_arm[3];	// STD of the lever arm correction [m]

		}SOutput;



		// Define structure for EKFDLL parameters
		typedef struct
		{
			bool              *killEfkDLL;
			HANDLE            *pEfkDLLFinishedEvent;
			HANDLE	        *hEKFDLLSynchMutex;

			double          *dblPos[3];
			double			*dblDOP[3];
			short			*shiNumSVs;
			double			*dblOri[3];
			int				*intPosMode;


		}SDLLparams;

		//// Define external DLL calls
		typedef bool (* INITEKFDLL)(SDLLparams *pEKFDLLparams); 
		typedef void (* STARTEKFDLL)(void *pEKFDLLparams);
		typedef bool (* PUSHEVENTEKFDLL)(SOutput *Pomar3DOutput); 
		typedef bool (* CLOSEEKFDLL)(void *pEKFDLLparams); 
		

		// Define structure for StartPosDLL parameters
		typedef struct
		{
			SDLLparams			*pDLLparams;
			INITEKFDLL			pInitEKFDLL;
			STARTEKFDLL			pStartEKFDLL;
			PUSHEVENTEKFDLL		pPushEventEKFDLL;
			CLOSEEKFDLL			pCloseEKFDLL;

		}SStartDLLparams;





		virtual void init(StringTable& attributes,  ConfigNode * localTree);

		SDLLparams  EKFDLLparams;
		SStartDLLparams StartEKFDLLparams;

		Event buffer;


	};

	OT_MODULE(Pomar3DModule);
	//OT_MODULE(GPSModule);

}  // namespace ot


//#endif // OT_NO_GPS_SUPPORT


#endif // !defined(_POMAR3DMODULE_H)

/* 
* ------------------------------------------------------------
*   End of GPSModule.h
* ------------------------------------------------------------
*   Automatic Emacs configuration follows.
*   Local Variables:
*   mode:c++
*   c-basic-offset: 4
*   eval: (c-set-offset 'substatement-open 0)
*   eval: (c-set-offset 'case-label '+)
*   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
*   eval: (setq indent-tabs-mode nil)
*   End:
* ------------------------------------------------------------ 
*/
