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
/** header file for FileSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/FileSource.h,v 1.1 2001/08/04 13:26:58 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filesource FileSource 
 * The FileSource node reads event data from an input file and passes new
 * events into the graph. It is created and controlled by the @ref filemodule. 
 * It associates its input stream with 
 * a station number, that is contained in the input file to demultiplex different input
 * streams from one file. There may be more then one FileSource using the same input file, 
 * however they need to use different station numbers. Moreover the file may 
 * only be used for input.
 *
 * The event data is processed in the order appearing in the file. During each cycle, 
 * each source fires at most once. Data that has a station number which is not used
 * by any FileSource is dropped. 
 *
 * A FileSource has the following attributes :
 * @li @c file the file name to use
 * @li @c station the station number to record
 *
 * An example element looks like this :
 * @verbatim
<FileSource file="name" station="0"/>@endverbatim
 */

#ifndef _FILESOURCE_H
#define _FILESOURCE_H

#include "../OpenTracker.h"
#include "File.h"

/**
 * This class implements a simple EventGenerator source node. It is driven by the
 * FileModule class which reads the data from the file and passes it to the
 * right FileSource object.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API FileSource : public Node
{
// Members
public:
    /// station number
    int station;
    /// flag whether state was changed this round
    int changed;	

// Methods
protected:
    /** constructor method,sets the station number and other default values.    
     * @param station the station number to use.
     */
    FileSource( int station_ = 0 ) :
        Node(), 
		station( station_ ),
        changed( 0 )
    {}

public:
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }   

    friend class FileModule;
};

#endif
