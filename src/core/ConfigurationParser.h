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
/** header file for class ConfigurationParser.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigurationParser.h,v 1.8 2001/07/16 21:43:52 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CONFIGURATIONPARSER_H
#define _CONFIGURATIONPARSER_H

#include "../dllinclude.h"

#include "Node.h"
#include "Module.h"
#include "NodeFactory.h"
#include "WrapperNode.h"
#include "ConfigNode.h"
#include "RefNode.h"
#include "StringTable.h"
#include "Context.h"

class DOM_Element;
class DOM_Document;
class Context;

/// used to map a string to a node
typedef std::map<std::string, Node *> NodeMap;

/**
 * parses the XML configuration file. This class reads the configuration file
 * using DOM and builds the tracker tree.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API ConfigurationParser
{
// Members
protected:
    /// reference to the Context to use its modules and factories
    Context & context;
    /// maps IDs to nodes
    NodeMap references;
	/// stores the parsed document tree
	DOM_Document * document;
protected:

// Methods
   /**
     * builds a tree of configuration nodes. This is just mirroring
     * the structure of the XML document with ConfigNodes that 
     * just hold the element names and attributes. The resulting tree
     * is a parameter to the initialization method of modules.
     * @param elements reference to the XML element to be parsed.
     * @return pointer to the hew ConfigNode or NULL, if something
     *         went wrong.
     */
    ConfigNode * buildConfigTree( DOM_Element & element );
   /**
     * builds the tracker tree starting from a certain DOM_Element.
     * Is used recoursively to walk the DOMTree and create new nodes.
     * @param element reference to the element to be parsed.
     * @return pointer to the new Node or NULL
     */
    Node * buildTree( DOM_Element& element);
   /**
     * parses an Elements attributes and returns a StringTable describing them.
     * @param element reference to the element
     * @return a StringMap mapping attribute names to attribute values
     */
    StringTable & parseElement( DOM_Element& element);

public:
    /**
     * ructor method. The Xerces XML parser library is initialized here.
     * @param factory_ pointer to set the member factory
     */
    ConfigurationParser( Context & context_);
    /** Destructor method.*/
    virtual ~ConfigurationParser();
    /**
     * This method parses an XML configuration file. It returns a Node as
     * root with all the root nodes defined in the configuration file.
     * @param filename the name and path of the configuration file
     * @return pointer to the root Node or NULL
     */
    Node * parseConfigurationFile(const std::string& filename);
};

#endif

