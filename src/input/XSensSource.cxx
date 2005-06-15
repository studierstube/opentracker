 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for XSensSource
  *
  * @author Gerhard Reitmayr
  * 
  * $Id: XSensSource.cxx 717 2004-07-27 11:54:49Z reitmayr $
  *
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include <ace/Log_Msg.h>

#ifdef WIN32

#include <objbase.h>
#include <xsens/IMTObj.h>
#endif

// return values for MT_GetOrientation function
#define MT_NEWDATA			1
#define MT_NODATA			2
#define MT_NOSENSORID		3
#define MT_INCOMPLETE		4
#define MT_CHECKSUMERROR	5
#define MT_NOPORT			6
#define MT_NOCALIBVALUES	7
#define MT_POWERLOSS		8

// output possiblities for MT object
#define MT_LOGQUATERNION	0
#define MT_LOGEULER		    1
#define MT_LOGROTMATRIX	    2

#include "XSensSource.h"

//using namespace std;


namespace ot {


XSensSource::XSensSource( int _comport, bool _amd ):
comport( _comport ),
amd( _amd )
{
#ifdef WIN32
    HRESULT hRes = CoCreateInstance(CLSID_MotionTracker, NULL, CLSCTX_SERVER, IID_IMotionTracker, (void**) &pMT);
	if (FAILED(hRes))
	{
		ACE_DEBUG((LM_ERROR, "XSensSource : Error %x in CoCreateInstance for MT object!",hRes));
		exit(1);
	}

    // pMT->MT_SetCalibratedOutput( TRUE );

    pMT->MT_SetOutputMode(MT_LOGQUATERNION);

    pMT->MT_SetCOMPort( comport );
#endif
}

XSensSource::~XSensSource()
{
#ifdef WIN32
    if( pMT != NULL )
    {
        pMT->Release();
        pMT = NULL;
    }
#endif
}

void XSensSource::push()
{
#ifdef WIN32
    // do something with xsens
    float orientationData[9] = {0};
	VARIANT orientationBuffer;
    void * pDest;
    short nNew = 0;

    pMT->MT_GetOrientationData( &nNew, &orientationBuffer );
    if( nNew == MT_NEWDATA )
    {
        if( orientationBuffer.vt != VT_EMPTY )
        {
            HRESULT hr = SafeArrayAccessData(orientationBuffer.parray, &pDest);
            // One dimensional array. Get the bounds for the array.
            
            if (SUCCEEDED(hr))
            {				
                __try{
                    // Copy data from the VARIANT array to the local fData array
                    memcpy(orientationData,pDest,(orientationBuffer.parray->rgsabound->cElements * sizeof(float)));
                }
                __except(GetExceptionCode() == STATUS_ACCESS_VIOLATION){
                    return;                
                }	
                
                SafeArrayUnaccessData(orientationBuffer.parray);	// Invalidate pointer
                
                // Variant must be cleared. This also destroys the SafeArray
                VariantClear(&orientationBuffer);
                
                // orientationData now contains orientation data
                
                // skalar component
                state.timeStamp();
                state.orientation[Q_W] = orientationData[0];
                state.orientation[Q_X] = orientationData[1];
                state.orientation[Q_Y] = orientationData[2];
                state.orientation[Q_Z] = orientationData[3];

                updateObservers( state );

			}
        }
    }
#if 0
    else
    {
        ACE_DEBUG((LM_DEBUG,"XSensSource : got result %i\n", nNew));
    }
#endif
#endif
}


}  // namespace ot
