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
  * @todo finish documentation about file module
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/FileModule.h,v 1.2 2001/07/11 22:34:07 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section filemodule FileModule
 
<FileConfig>
    <File name="path/name" io="out" id="test" />
</FileConfig>@endverbatim
 */

#ifndef _FILEMODULE_H
#define _FILEMODULE_H

#include "../OpenTracker.h"
#include "File.h"

/**
 *
 * @ingroup common
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API FileModule: public Module, public NodeFactory
{
// Members
protected:
    /// list of ConsoleSink nodes in the tree
    NodeVector sinks;
    /// list of ConsoleSource nodes  in the tree
    NodeVector sources;
    /// vector of File objects
    std::map<string, File *> files;

public:
    /** constructor method. initializes internal and static data
     * such as the functionMap and keyMap tables. */
    FileModule(); 
    /** Destructor method, clears nodes member. */
    virtual ~FileModule();
    /**
     * initializes the tracker module. 
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, ConfigNode * localTree);
    /** This method is called to ruct a new Node. It compares
     * name to the ConsoleSink element name, and if it matches
     * creates a new ConsoleSink node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringTable& attributes);
    /**
     * reads from the input files and fires new events, if necessary.
     */
    virtual void pushState();
    /**
     * reads out the FileSink nodes and writes them to the files, if
	 * they changed.
     */
    virtual void pullState();
     /**
     * Opens configured files, if some sinks and sources where built.*/
    virtual void start();
    /**
     * Closes the files.*/
    virtual void close();
};

#endif
