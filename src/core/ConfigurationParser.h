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
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CONFIGURATIONPARSER_H
#define _CONFIGURATIONPARSER_H

#include "../dllinclude.h"
#include "../OpenTracker.h"

#include <xercesc/dom/DOM.hpp>

class Context;

/// used to map a string to a node
typedef std::map<std::string, Node *> NodeMap;

/**
 * parses the XML configuration file. This class reads the configuration file
 * using DOM and builds the tracker tree. It is not part of the public API of
 * OpenTracker, therefore do not use it !
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class ConfigurationParser
{
// Members
protected:
    /// reference to the Context to use its modules and factories
    Context & context;
    /// maps IDs to nodes
    NodeMap references;
	/// stores the parsed document tree
	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument * document;
protected:

// Methods
   /**
     * builds a tree of configuration nodes. This is just mirroring
     * the structure of the XML document with ConfigNodes that 
     * just hold the element names and attributes. The resulting tree
     * is a parameter to the initialization method of modules.
     * @param elements pointer to the XML element to be parsed.
     * @return pointer to the hew ConfigNode or NULL, if something
     *         went wrong.
     */
    ConfigNode * buildConfigTree( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * element );
   /**
     * builds the tracker tree starting from a certain DOM_Element.
     * Is used recoursively to walk the DOMTree and create new nodes.
     * @param element pointer to the element to be parsed.
     * @return pointer to the new Node or NULL
     */
    Node * buildTree( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * element);
   /**
     * parses an Elements attributes and returns a StringTable describing them.
     * @param element pointer to the element
     * @return a StringMap mapping attribute names to attribute values. This map
     *    was created on the heap and it is now owned by the caller.
     */
    StringTable * parseElement( XERCES_CPP_NAMESPACE_QUALIFIER DOMElement * element);

public:
    /**
     * constructor method. The Xerces XML parser library is initialized here.
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
