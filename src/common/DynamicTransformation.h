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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for DynamicTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * @todo think about using other node types than only the EventGenerator
  * as the base child. What semantics do make sense ??
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/DynamicTransformation.h,v 1.6 2001/04/12 06:41:38 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 *
 * @section dynamictransform DynamicTransformation
 * This section describes the DynamicTransformation configuration element. 
 * A DynamicTransformation transforms the data child's state value by the base
 * child's state value. The base childs position and orientation are assumed
 * to be the affine base of the data childs values. Then the data childs values
 * are transformed to world coordinates. Note that there is no complement inverse
 * element, because te 
 *
 * There are three different Transformation elements to provide a strong structure
 * in the configuration file syntax. They have no attributes but use a single
 * wrapper tag to define the base child.
 * 
 * An example element looks like this :
 * @verbatim
 <EventDynamicTransformation>
    <Any EventGenerator element type>
    <TransformBase>
        <Any EventGenerator element type>
    </TransformBase>
 </EventDynamicTransformation>@endverbatim
 */

#ifndef _DYNAMICTRANSFORMATION_H
#define _DYNAMICTRANSFORMATION_H

#include "StaticTransformation.h"

/**
 * The DynamicTransformation class implements a transformation that puts
 * the data childs data into the affine space defined by the base childs
 * data. If no data was received by the child sofar, it uses the
 * identity transformation. It builds upon the StaticTransformation by
 * setting the members of the StaticTransformation to the base childs
 * data. 
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API DynamicTransformation : public StaticTransformation
{
protected:

    /** stores the original event data to compute new values
     * on base changes.*/
    State store;

	// tree navigation interface   
		
	unsigned int countChildren();

	Node * getChild( unsigned int index );

public:
    /** constructor method. It sets default values on the
     * underlying StaticTransformation. */
    DynamicTransformation();

    /**
     * this method is called by the EventGenerator to update it's observers.
     * This class computes a transformed state, stores it in its local variable
     * and notifies its observers in turn, propagating the change.
     * If the EventGenerator equals the baseChild, its data is stored in the
     * underlying StaticTransformation fields, to change the transformation itself.
     */
    virtual void onEventGenerated( State& event, Node& generator);

};

#endif
