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
/** header file for GKTransformNode Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 * @section gktransform GKTransform
 *
 * This transformation node transforms GPS position data into a local
 * Gausz - Krueger map projection. The main parameters are the
 * two semi diameters a and b of the ellipsoid which are specified in meters, 
 * the meridian around which the grid coordinates should be computed, and for
 * parameter values alpha, beta, gamma, delta. 
 * It expects the data in ellipsoid coordinate format as the Latitue, 
 * Longitute and Height data in the components of the position value, just as 
 * the @ref gpssource node produces them. It can be used to transform data 
 * to a GK map or from a GK map into ellipsoid coordinates relative to the
 * given ellipsoid.
 *
 * The configuration elements are obtained by using the base name @c GKTransform
 * and prepending it with one of the prefixes @c Event | @c Queue | @c Time 
 * as described in the @ref transform node.
 *
 * It has the following attributes:
 * @li @c a the first semi diameter given in meters. This is required
 * @li @c b the second semi diameter given in meters. 
 *        If not specified it is set to the value of @c a
 * @li @c meridian the meridian of the GK projection to use. This is given in 
 *        degree rather than radiants ! the default value is 34 which is 
          used around Vienna, Austria
 * @li @c alpha a required transformation parameter
 * @li @c beta a required transformation parameter
 * @li @c gamma a required transformation parameter
 * @li @c delta a required transformation parameter
 * @li @c mode (to | from) specifies whether to transform to or from GK
 *        coordinates. The default is @c to.
 *
 * See @ref ellipsoidtransform for some good values for a, b
 * For a transformation to the Austrian map datum, use the following values :
 * @li @c meridian = 28 | 31 | 34, 34 is best for Vienna and east Austria.
 * @li @c alpha = 111120.6196
 * @li @c beta = 15988.6385
 * @li @c gamma = 16.73
 * @li @c delta = 0.0218
 *
 * An example element looks like this :
 * @verbatim
 <EventGKTransform a="6377397.155" b="6356078.963" mode="to" meridian="34" alpha="111120.6196" beta="15988.6385" gamma="16.73" delta="0.0218">
    <Any EventGenerator element type>
 </EventGKTransform>@endverbatim
 */

#ifndef _GKTRANSFORMNODE_H
#define _GKTRANSFORMNODE_H

#include "Transformation.h"

/**
 * This class implements a transformation between an ellipsoid coordinate system
 * and the Gausz - Krueger (GK) coordinates. The corresponding element is 
 * @ref gktransform .
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

class OPENTRACKER_API GKTransformNode : public Transformation 
{
public:
	double a;
	double b;
	double meridian;
	double alpha, beta, gamma, delta;
	enum Mode { to = 0, from } mode;

protected:
	GKTransformNode( double a_ , double b_, double m_, 
					 double alpha_, double beta_, double gamma_, double delta_, 
					 Mode mode_ );

	/** transforms a state either to or from an ellipsoid 
	 * coordinate system to a cartesian one.
     * @param state pointer to original state
     * @return pointer to localState member
     */
    virtual State* transformState( State* state);

	friend class CommonNodeFactory;
};

} // namespace ot {

#endif // !defined(_GKTRANSFORMNODE_H)
