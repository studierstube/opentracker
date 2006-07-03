/* ========================================================================
* TODO Uni-blabla
* ========================================================================
* PROJECT: OpenTracker
* ======================================================================== */
/** header file for Merge Tracker Node.
*
* @author Gratl/Werlberger VRVU
*
* $Id:$
* @file                                                                   */
/* ======================================================================= */

/**
* @page Nodes Node Reference
* @section mergetracker MergeTracker
* TODO description
*
* inputs :
* 
* TODO
* 
* An example element using all wrapper elements looks like this :
* @verbatim
TODO example
*/

#include "MergeTrackerNode.h"
#include <ace/Log_Msg.h>
#include <iostream>

namespace ot
{
	MergeTrackerNode::MergeTrackerNode() : Node()
	{
	}

	void MergeTrackerNode::onEventGenerated(State &event, Node &generator)
	{	

		std::cout << "MergeTrackerNode:onEventGenerated" << std::endl;
		std::cout << " getType: " << generator.getType() << std::endl;
		std::cout << " getName: " << generator.getName() << std::endl;

		// final location updates:
		// FIXMEE: for a first test the positions are hard-coded
		// TODO - change if-then-else to a switch statement with the help of enum definitions
		// top tracking device
		if (generator.getName().compare("top") == 0)
		{
			topState.position[0] = event.position[0];
			topState.position[1] = event.position[1];
			topState.position[2] = event.position[2];

		}
		else if (generator.getName().compare("bLeft") == 0)
		{
			bLeftState.position[0] = event.position[0];
			bLeftState.position[1] = event.position[1];
			bLeftState.position[2] = event.position[2];

		}
		else if (generator.getName().compare("bRight") == 0)
		{
			bRightState.position[0] = event.position[0];
			bRightState.position[1] = event.position[1];
			bRightState.position[2] = event.position[2];
		}
		
		// afterwards combine the location and update the merged state
		if ((generator.getName().compare("top") == 0)| (generator.getName().compare("bLeft") == 0) |
			(generator.getName().compare("bRight") == 0))
		{
			std::cout << " merging positions and rotations" << std::endl;
			// merge Position
			mergedState.position[0] = (topState.position[0] + bLeftState.position[0] + 
				bRightState.position[0]) / 3;
			mergedState.position[1] = (topState.position[1] + bLeftState.position[1] + 
				bRightState.position[1]) / 3;
			mergedState.position[2] = (topState.position[2] + bLeftState.position[2] + 
				bRightState.position[2]) / 3;

			std::cout << "Position = " << mergedState.position[0] << mergedState.position[1] << mergedState.position[2] << std::endl;

			float orientation_vector[3];
			float tmp[3];
			float tmp2[3];
			double angletmp;
			float x_axis[3] = {1.0, 0.0, 0.0};
			float y_axis[3] = {0.0, 1.0, 0.0};
			float z_axis[3] = {0.0, 0.0, 1.0};
			double angleX, angleY, angleZ;

			//calculate the orientation as vector (vector from the middle of the two base tags to the top)
			orientation_vector[0] = topState.position[0] - (bRightState.position[0] + bLeftState.position[0])/2;
			orientation_vector[1] = topState.position[1] - (bRightState.position[1] + bLeftState.position[1])/2;
			orientation_vector[2] = topState.position[2] - (bRightState.position[2] + bLeftState.position[2])/2;
			
			mergedState.orientation[0] = 0.0;
			mergedState.orientation[1] = orientation_vector[0];
			mergedState.orientation[2] = orientation_vector[1];
			mergedState.orientation[3] = orientation_vector[2];

			// do update
			updateObservers( mergedState );


//
//			float orientation_vector[3];
//			float tmp[3];
//			float x_axis[3] = {1.0, 0.0, 0.0};
//			float y_axis[3] = {0.0, 1.0, 0.0};
//			float z_axis[3] = {0.0, 0.0, 1.0};
//			double angleX, angleY, angleZ;
//
//			//calculate the orientation as vector (vector from the middle of the two base tags to the top)
//			orientation_vector[0] = topState.position[0] - (bRightState.position[0] + bLeftState.position[0])/2;
//			orientation_vector[1] = topState.position[1] - (bRightState.position[1] + bLeftState.position[1])/2;
//			orientation_vector[2] = topState.position[2] - (bRightState.position[2] + bLeftState.position[2])/2;
//			
//			//calculate the three euler angles of the vector
//			tmp[0] = orientation_vector[0];
//			tmp[1] = orientation_vector[1];
//			tmp[2] = 0.0;
//
//
//			angleX = MathUtils::angle(x_axis,tmp, 3);
//			angleY = MathUtils::angle(tmp, orientation_vector, 3);
////			angleZ = MathUtils::angle(orientation_vector, z_axis, 3);
//
//			std::cout << "   x-angle: " << angleX << std::endl;
//			std::cout << "   y-angle: " << angleY << std::endl;
////			std::cout << "   z-angle: " << angleZ << std::endl;
//			
//			//convert them to a quaternion (type of orientation in OT)
//			MathUtils::eulerToQuaternion(angleX, angleY, 0.0,MathUtils::EULER::XYZ, mergedState.orientation );
//		    
//			// do update
//			updateObservers( mergedState );




		}
	}
	
} // namespace ot