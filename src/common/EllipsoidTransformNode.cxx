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
/** source file for EllipsoidTransformNode Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include "EllipsoidTransformNode.h"
#include <math.h>

using namespace std;

EllipsoidTransformNode::EllipsoidTransformNode( double a_ , double b_, Mode mode_ ) :
a( a_ ),
b( b_ ),
mode( mode_ )
{}

State* EllipsoidTransformNode::transformState( State* state)
{
	if( toCartesian == mode )
	{
		double B = state->position[0];
		double L = state->position[1];
		double H = state->position[2];
		double e2 = 1 - (b*b) / (a*a);
		double N = a / (sqrt( 1 - e2 * sin( B )*sin( B )));
		localState.position[0] = (N + H)*cos(B)*cos(L);
		localState.position[1] = (N + H)*cos(B)*sin(L);
		localState.position[2] = ((1 - e2)*N + H)*sin(B);				
	}
	else
	{
		double x = state->position[0];
		double y = state->position[1];
		double z = state->position[2];
		double e2 = 1 - (b*b) / (a*a);
		double L = 0;
		double B = 0;
		double H = 0;
		if( x != 0 ) 
		{
			L = atan( y / x );
			B = atan( z * cos( L ) / (x * (1 - e2)));  // interestingly, this is only an approximation, 
													   // but it seems to work ?!
			double N = a / (sqrt( 1 - e2 * sin( B )*sin( B )));
			H = sqrt( x*x + y*y ) / cos(B) - N; // this avoids problems with L
		} 
		else if( y != 0 )
		{
			L = 1 / atan ( x / y );
			B = atan( z * sin( L ) / (y * (1 - e2)));  // interestingly, this is only an approximation,
													   // but it seems to work ?!poles 
			double N = a / (sqrt( 1 - e2 * sin( B )*sin( B )));
			H = sqrt( x*x + y*y ) / cos(B) - N; // this avoids problems with L
		}
		else  // otherwise we are somewhere at the poles
		{
			if( z > 0 )
			{
				B = MathUtils::Pi / 2;
				H = z - b;
			}
			else if( z < 0 )
			{
				B = - MathUtils::Pi / 2;
				H = - b -z;
			}
		}
		localState.position[0] = B;
		localState.position[1] = L;
		localState.position[2] = H;				
	}
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
