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
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/FileModule.h,v 1.8 2003/11/30 17:37:32 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section filemodule FileModule
 * The FileModule allows to read or write events from or to files. Every file can
 * contain several input or output streams identified by station numbers. However,
 * a single file can only serve as input or output at any given time. The configuration
 * element @c FileConfig takes a single attribute as parameter.
 * @li @c append (true|false) default is @c false. denotes whether data should be appended
 * to existing output files or they should be overwritten.
 * @li @c loop (true|false) default is @c false. denotes whether input files should
 * start from the begining (loop is true) or stop after they end.
 *
 * An example configuration element looks like this :
 * @verbatim
<FileConfig append="true" loop="true" />@endverbatim
 */

#ifndef _FILEMODULE_H
#define _FILEMODULE_H

#include "../OpenTracker.h"

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
    /// flag whether output files should be appended to or not
    bool append;
    /// flag whether to loop input files or not
    bool loop;

public:
    /** constructor method. initializes internal and static data
     * such as the functionMap and keyMap tables. */
    FileModule()
    {} 
    /** Destructor method, clears nodes member. */
    virtual ~FileModule();

    /**
     * initializes the tracker module. This class provides an implementation
     * that sets the initialization flag to true. Subclasses should call this
     * method if they override this method. It takes the attributes of the
     * element configuring this module and a local tree consisting of the
     * children of the element. This tree must be build of Nodes.
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes,  ConfigNode * localTree);
 
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
