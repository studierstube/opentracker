 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for class ConfigurationParser.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigurationParser.h,v 1.3 2001/01/29 17:16:44 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CONFIGURATIONPARSER_H
#define _CONFIGURATIONPARSER_H

#include <dom/DOM_Element.hpp>
#include <string>
#include <map>

#include "Node.h"
#include "Module.h"
#include "NodeFactory.h"
#include "WrapperNode.h"
#include "ConfigNode.h"

/**
 * maps a string to another string. Mostly used to map element attributes
 * to values.
 */
typedef std::map<string, string> StringMap;

/** Used to map a XML element to a Module. */
typedef std::map<string, Module *> ModuleMap;

/// used to map a string to a node
typedef std::map<string, Node *> NodeMap;

/**
 * parses the XML configuration file. This class reads the configuration file
 * using DOM and builds the tracker tree.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class ConfigurationParser
{
// Members
protected:
    /// maps configuration element names to modules
    ModuleMap modules;
    /// pointer to a NodeFactory used to create new Nodes.
    NodeFactory & factory;
    /// maps IDs to nodes
    NodeMap references;

// Methods
protected:
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
     * parses an Elements attributes and returns a StringMap describing them.
     * @param element reference to the element
     * @return a StringMap mapping attribute names to attribute values
     */
    StringMap & parseElement( DOM_Element& element);

public:
    /**
     * ructor method. The Xerces XML parser library is initialized here.
     * @param factory_ pointer to set the member factory
     */
    ConfigurationParser( NodeFactory & factory_);
    /** Destructor method.*/
    virtual ~ConfigurationParser();
    /**
     * adds a named module to the internal ModuleMap. It replaces an
     * existing module of the same name.
     * @param name the elements name
     * @param module reference to the new module
     */
    void addModule( string& name, Module& module);
    /**
     * This method parses an XML configuration file. It returns a Node as
     * root with all the root nodes defined in the configuration file.
     * @param filename the name and path of the configuration file
     * @return pointer to the root Node or NULL
     */
    Node * parseConfigurationFile( string& filename);
    /**
     * removes a module with the given name from the ModuleMap.
     * @param name the elements name
     */
    void removeModule( string& name);
};

#endif

