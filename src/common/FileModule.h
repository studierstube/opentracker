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
/** header file for FileModule module.
  *
  * @author Gerhard Reitmayr
  * @todo implement constraint to have only one source per (file,station) pair
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/FileModule.h,v 1.5 2001/08/07 09:21:09 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section filemodule FileModule
 * The FileModule allows to read or write events from or to files. Every file can
 * contain several input or output streams identified by station numbers. However,
 * a single file can only serve as input or output at any given time. There is no
 * configuration section as all information is stored in the @ref filesink and 
 * @ref filesource nodes. However it reserves the name @c FileConfig for future use.
 */

#ifndef _FILEMODULE_H
#define _FILEMODULE_H

#include "../OpenTracker.h"
// #include "File.h"

class File;

/**
 * This class manages the files and FileSink and FileSource objects. FileSources are
 * driven by the main loop, whereas FileSinks write directly to the associated file,
 * whenever they receive an event.
 * @ingroup common
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API FileModule: public Module, public NodeFactory
{
// Members
protected:
    /// map of name to list of either FileSink or FileSource nodes    
    std::map<std::string, NodeVector> nodes;
    /// map of name to File objects
    std::map<std::string, File *> files;

public:
    /** constructor method. initializes internal and static data
     * such as the functionMap and keyMap tables. */
    FileModule()
    {} 
    /** Destructor method, clears nodes member. */
    virtual ~FileModule();
    /** This method is called to construct a new Node. It compares
     * name to the FileSink or FileSource element name, and if it matches
     * creates the necessary File and Node objects.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name, StringTable& attributes);
    /**
     * reads from the input files and fires new events, if necessary.
     */
    virtual void pushState();
    /**
     * Closes the files and cleans up datastructures.*/
    virtual void close();
};

#endif
