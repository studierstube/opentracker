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
/** header file for FileSink Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filesink FileSink 
 * The FileSink node writes incoming event data to an output file. It is created
 * and controlled by the @ref filemodule. It associates its output stream with 
 * a station number, that is written into the file to multiplex different sinks
 * into one file. There may be more then one FileSink using the same station number
 * and / or output file. However the file may only be used for output.
 * It has the following attributes :
 * @li @c file the file name to use
 * @li @c station the station number to record
 *
 * An example element looks like this :
 * @verbatim
<FileSink file="test.out" station="0">
    <Any EventGenerator element type>
</FileSink>@endverbatim
 */

#ifndef _FILESINK_H
#define _FILESINK_H

#include "../OpenTracker.h"
#include "File.h"

/**
 * This class implements a simple EventGenerator that writes any incoming events
 * to the associated file.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

class OPENTRACKER_API FileSink : public Node
{
// Members
public:
   	/// Output File object associated with this node.
    File & file;
    /// station number
    int station;	

// Methods
protected:
    /** constructor method,sets commend member
     * @param file_ the File object to write to
     * @param station_ the station number to use */
    FileSink( File & file_, int station_ = 0 ) :
        Node(), 
        file( file_ ),
		station( station_ )
    {}

public:
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
    
    /**
     * this method notifies the object that a new event was generated.
     * It writes the event value out to the file and passes it
     * on to its parent.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event, Node& generator)
    {
        file.write( event, station );
        updateObservers( event );
    }

    friend class FileModule;
};

}

#endif
