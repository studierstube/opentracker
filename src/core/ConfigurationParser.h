 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for class ConfigurationParser.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigurationParser.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
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

/**
 * maps a string to another string. Mostly used to map element attributes
 * to values.
 */
typedef std::map<string, string> StringMap;

/** Used to map a XML element to a Module. */
typedef std::map<string, Module *> ModuleMap;

/**
 * parses the XML configuration file. This class reads the configuration file
 * using DOM and builds the tracker tree.
 * @author Gerhard Reitmayr
 */
class ConfigurationParser
{
// Members
protected:
    /// maps configuration element names to modules
    ModuleMap modules;
    /// pointer to a NodeFactory used to create new Nodes.
    NodeFactory & factory;

// Methods
protected:
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

