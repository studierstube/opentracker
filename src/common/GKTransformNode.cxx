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
/** source file for GKTransformNode Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/GKTransformNode.cxx,v 1.2 2003/04/03 15:50:59 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "GKTransformNode.h"
#include <cmath>

using namespace std;

GKTransformNode::GKTransformNode( double a_ , double b_, double m_, 
								  double alpha_, double beta_, double gamma_, double delta_, 
								  Mode mode_ ) :
a( a_ ),
b( b_ ),
meridian( m_ * MathUtils::GradToRad ),  // we get the meridian in degrees
alpha( alpha_ ),
beta( beta_ ),
gamma( gamma_ ),
delta( delta_ ),
mode( mode_ )
{}

State* GKTransformNode::transformState( State* state)
{
	// the zero meridian of the GK map projection goes through Ferro,
	// a Canarian island 17° 40' left of Greenwhich zero meridian.
	const double ferro = (17 + 40 / 60) * MathUtils::GradToRad;

	if( to == mode )
	{
		double B = state->position[0];
		double sinB = sin(B);
		double cosB = cos(B);
		double L = state->position[1];
		double dL = L + ferro - meridian;

		double Sm = alpha * B - beta * sin(2 * B) + gamma * sin(4 * B) - delta * sin(6 * B);

		double e2 = 1 - (b*b) / (a*a);
		double N = a / (sqrt( 1 - e2 * sinB * sinB ));
		double eta2 = ((a*a) / (b*b) - 1)*cosB*cosB;
		double t = tan( B );
		double corr_x = 1 + (dL*dL)*(cosB*cosB)*(5 - t*t + 9*eta2 + 4*eta2*eta2) / 12 +
			            (dL*dL*dL*dL)*(cosB*cosB*cosB*cosB)*(61-58*t*t+t*t*t*t) / 360;
		double corr_y = 1 + (dL*dL)*(cosB*cosB)*(1 - t*t + eta2) / 6 +
						(dL*dL*dL*dL)*(cosB*cosB*cosB*cosB)*(5 - 18*t*t + t*t*t*t) / 120;
		double x = Sm + N * dL * dL * sinB * cosB * corr_x / 2;
		double y = N * dL * cosB * corr_y;

		localState.position[0] = x;
		localState.position[1] = y;
	}
	else
	{
		double x = state->position[0];
		double y = state->position[1];
		double xa = x * MathUtils::GradToRad / alpha;
		double Bf = (xa + beta*sin(2*xa) + gamma*sin(4*xa) + delta*sin(6*xa)) * MathUtils::GradToRad;
		double t = tan(Bf);
		double t2 = t*t;
		double e2 = 1 - (b*b) / (a*a);
		double eta2 = ((a*a) / (b*b) - 1)*cos(Bf)*cos(Bf);
		double W = sqrt(1 - e2 *sin(Bf)*sin(Bf));
		double N = a / W;
		double M = b*b / (a*W*W*W);
		double corr_b = 1 - (y*y)*(5 + 3*t2 + eta2 - 9*t2*eta2)/(12*N*N) +
							(y*y*y*y)*(61+90*t2+45*t2*t2)/(360*N*N*N*N);
		double corr_l = 1 - (y*y)*(1 + 2*t2 + eta2)/(6*N*N) + 
							(y*y*y*y)*(5 + 28*t2 + 24*t2*t2)/(120*N*N*N*N);
		double B = Bf - (y*y*t)*corr_b/(2*M*N);
		double L = y*corr_l/(N*cos(Bf)) + meridian - ferro;

		localState.position[0] = B;
		localState.position[1] = L;

	}

	// height over the ellipsoid surface is identical to the height over the map
	localState.position[2] = state->position[2];

	// copy the rest over
	// we don't deal with orientation so far...
	localState.orientation[0] = state->orientation[0];
	localState.orientation[1] = state->orientation[1];
	localState.orientation[2] = state->orientation[2];
	localState.orientation[3] = state->orientation[3];
	
	localState.confidence = state->confidence;
	localState.button = state->button;
	localState.time = state->time;
	return & localState;
}