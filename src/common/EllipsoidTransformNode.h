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
/** header file for EllipsoidTransformNode Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/EllipsoidTransformNode.h,v 1.1 2003/03/28 13:07:43 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 * @section ellipsoidtransform EllipsoidTransform
 *
 * This transformation node transforms GPS position data between ellipsoid and
 * the corresponding cartesian coordinate system. The main parameters are the
 * two semi diameters a and b of the ellipsoid which are specified in meters.
 * For ellipsoid coordinate data it uses
 * the Latitue, Longitute and Height data in the components of
 * the position value, just as the @ref gpssource node produces them. 
 *
 * The configuration elements are obtained by using the base name @c EllipsoidTransform
 * and prepending it with one of the prefixes @c Event | @c Queue | @c Time 
 * as described in the @ref transform node.
 *
 * It has the following attributes:
 * @li @c a the first semi diameter given in meters. This is required
 * @li @c b the second semi diameter given in meters. 
 *           If not specified it is set to the value of @c a
 * @li @mode (toCartesian | toEllipsoid) specifies whether to transform to cartesian or
 *           ellipsoid coordinates. The default is @c toCartesian.
 *
 * Some handy values for a and b :
 * @li a = 6378137 , b = 6356752.31424523 - WGS84 used by GPS systems
 * @li a = 6377397.155 ,  b = 6356078.963 - Bessel used by Austrian map data
 *
 * An example element looks like this :
 * @verbatim
 <EventEllipsoidTransform a="6378137" b="6356752.31424523" mode="toCartesian">
    <Any EventGenerator element type>
 </EventEllipsoidTransform>@endverbatim
 */

#ifndef _ELLIPSOIDTRANSFORMNODE_H
#define _ELLIPSOIDTRANSFORMNODE_H

#include "Transformation.h"

/** 
 * This class implements transformations between ellipsoid and cartesian
 * coordinate systems for GPS position data. It can be used as a
 * filter node for events, queues or time dependend nodes. 
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class EllipsoidTransformNode : public Transformation  
{
public:
	/// the first semi diameter in meters
	double a;
	/// the second semi diameter in meters
	double b;
	/// mode flag to denote which direction the conversion will be
	enum Mode { toEllipsoid = 0, toCartesian } mode;

protected:
	EllipsoidTransformNode( double a_ , double b_, Mode mode_ );

	/** transforms a state either to or from an ellipsoid 
	 * coordinate system to a cartesian one.
     * @param state pointer to original state
     * @return pointer to localState member
     */
    virtual State* transformState( State* state);

	friend class CommonNodeFactory;
};

#endif // !defined(_ELLIPSOIDTRANSFORMNODE_H)
