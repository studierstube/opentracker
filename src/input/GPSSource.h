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
/** header file for GPSSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSSource.h,v 1.1 2003/03/27 18:26:02 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section gpssource GPSSource
 * The GPSSource node is a simple EventGenerator tht outputs GPS position data encoded
 * in the position part of the event. This data is in Latitue, Longitute, Height 
 * in the WGS84 coordinate system.
 * Use a filter after that to transform the data into your local coordinate system. The node
 * has no further attributes besides the ID attribute.
 *
 * An example element looks like this :
 * @verbatim
<GPSSource/>@endverbatim
 */

#ifndef _GPSSOURCE_H
#define _GPSSOURCE_H

#include "../OpenTracker.h"

/**
 * A very simple EventGenerator node for outputing GPS position data.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API GPSSource : public Node  
{
public:

	 /// the state that is posted to the EventObservers
    State state;
    
	/** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    int isEventGenerator()
    {
        return 1;
    }

protected:
	/// protected constructor so it is only accessible by the module
	GPSSource() {};

	friend class GPSModule;
};

#endif // !defined(_GPSSOURCE_H)
