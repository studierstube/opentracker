/* ========================================================================
* Copyright (c) 2006,
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
/** header file for PanTiltUnitSource Node.
*
* @author Markus Sareika
*
* $Id: PanTiltUnitSource.h
* @file                                                                   */
/* ======================================================================= */

#include "PanTiltUnitSource.h"

#ifdef USE_PANTILTUNIT

//#include <cstdio>
//#include <sstream>
//#include <iostream>


namespace ot {

	bool PanTiltUnitSource::initComPort(int comPort)
	{
		// aquire com port name
		char COMportName[256];
		#ifdef _UNIX
			char COMportPrefix[10] = "/dev/cua";
		#else
			char COMportPrefix[10] = "COM";
		#endif
		sprintf(COMportName, "%s%d", COMportPrefix, comPort);
		// initialize the serial port 
		//COMstream = open_host_port(COMportName);
		//if ( COMstream == PORT_NOT_OPENED ) return false;
		return true;
	}

	// use PTU...
	//////	signed short int val=600;
	//////	set_desired(PAN,  POSITION, (PTU_PARM_PTR *) &val, ABSOLUTE);
	//////	set_desired(TILT, POSITION, (PTU_PARM_PTR *) &val, ABSOLUTE);
	//////	await_completion();

	void PanTiltUnitSource::closeComPort()
	{
		//close_host_port(COMstream);
	}

	void PanTiltUnitSource::onEventGenerated( Event& event, Node& generator) {
		static int i=0;
		
		// Process Data received from Sink
		if (generator.getName().compare("PTUgoto") == 0) {
			//event.getPosition()[0];
			//event.getPosition()[1];
			//event.getPosition()[2];
			//event.getOrientation();
			printf ("PTUgoto: x: %.2f y: %.2f z: %.2f\n", event.getPosition()[0], event.getPosition()[1], event.getPosition()[2]);
			return;
		}

		// Process Data received from the Nodekit
		//if (generator.getName().compare("SpaceMouseKitData") == 0) {
		//	// Data from the NodeKit

		//	switch(event.getButton()) {
		//			case OTCOM_RESETPOSITION:
		//				{ 
		//					static int i1=0;

		//					float v1[3], v2[3], uo[4];
		//					v1[0]=0.0f;v1[1]=0.0f;v1[2]=5.0f;
		//					
		//					uo[0] = usrpos.getOrientation()[0];
		//					uo[1] = usrpos.getOrientation()[1];
		//					uo[2] = usrpos.getOrientation()[2];
		//					uo[3] = usrpos.getOrientation()[3];

		//					MathUtils::rotateVector(uo, v1, v2);

		//					tmpEvent.getPosition()[0] = usrpos.getPosition()[0]+v2[0];
		//					tmpEvent.getPosition()[1] = usrpos.getPosition()[1]+v2[1];
		//					tmpEvent.getPosition()[2] = usrpos.getPosition()[2]+v2[2];
		//					
		//					// update distance
		//					tmpEvent.setAttribute<float>("cursorDistance", DIST(tmpEvent.getPosition(), usrpos.getPosition()));		
		//				}
		//				break;
		//			case OTCOM_RESETROTATION:
		//				{ 
		//					float np[4], cq[4], rq[4], up[4];
		//					np[0]=0.0f; 
		//					np[1]=1.0f; 
		//					np[2]=0.0f; 
		//					np[3]=0.0f;
		//					MathUtils::axisAngleToQuaternion(np, cq);

		//					up[0]=usrpos.getOrientation()[0];
		//					up[1]=usrpos.getOrientation()[1];
		//					up[2]=usrpos.getOrientation()[2];
		//					up[3]=usrpos.getOrientation()[3];
		//					MathUtils::multiplyQuaternion(cq, up ,rq);
		//					tmpEvent.setOrientation(rq);
		//				}
		//				break;
		//			case OTCOM_TOGGLE_ROTATECAMERAAXIS:
		//				{
		//					useAbsRotation?useAbsRotation=false:useAbsRotation=true;
		//				}
		//				break;
		//			
		//			default:
		//				break;
		//	}
		//	return;
		//}

		// default behaviour: store PTUgoto event from sink
		memcpy(&ptuGoto, &event, sizeof(Event));
	}
}

#endif //USE_PANTILTUNIT