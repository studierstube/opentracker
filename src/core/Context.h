 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Context class
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CONTEXT_H
#define _CONTEXT_H

#include "../dllinclude.h"

#include "Node.h"
#include "Module.h"
#include "NodeFactoryContainer.h"


namespace ot {

/// a string map to store modules in
typedef std::map<std::string, Module *> ModuleMap;

/**
 * This class represents one context. It keeps its own modules and tracker tree. Using
 * a ConfigurationParser it builds the tree from a given configuration file. Any 
 * factories and modules need to be added before the parsing happens, otherwise
 * the nodes can not be parsed and the modules will not be initialized with
 * their respective configuration elements. 
 *
 * Finally it implements the main loop driving the tracker. This consists of
 * the following basic algorithm described in pseudo code here :
 *@verbatim
start();
while( !stop())
{
    pushState();
    pullState();
}
close();@endverbatim
 * The whole loop is implemented in the member function run, which can be used
 * instead of implementing it yourself.
 *
 * The Context also keeps track of various information. The following items
 * are stored and used by modules :
 *
 * The Context stores a stack of directories. Any module that needs to load
 * a file can use the stack to find files relative to a number of directories.
 * By default the stack contains the path component of the configuration file,
 * followed by the current directory. See the method findFile for details.
 *
 * @author Gerhard Reitmayr
 * @ingroup core
 */

class OPENTRACKER_API Context
{

protected:
    /// A map of the modules this context works with
    ModuleMap modules;
    /// Pointer to the root node of the local tracker tree
    Node* rootNode;
    /// A NodeFactoryContainer used by the ConfigurationParser to create new nodes.
    NodeFactoryContainer factory;
    /// flag to remember whether the Context is responsible for cleaning up the modules.
    bool cleanUp;
    /// stores the filename of the current configuration.
    std::string file;
    /// stores a stack of directory names to search for files in
    std::vector<std::string> directories;
    /// stores the namespace string of the root element
    std::string rootNamespace;

// Methods
public:

   /** a constructor method. 
    * @param init If init is not equal to 0, it instantiates all known modules and 
    *        factories, adds them to its local containers and also takes care of 
    *        removing them again in the destructor.*/
    Context( int init = 0 );
   /** destructor method clears containers and removes any modules instantiated in
    * the default setup, if cleanUp is st. */
    virtual ~Context();

   /** adds a new factory to the NodeFactoryContainer.
    * @param newfactory reference to the new factory */
    void addFactory(NodeFactory& newfactory);
   /** removes a factory from the NodeFactoryContainer.
    * @param factory reference to the factory to remove */
    void removeFactory(NodeFactory & factory);

    /** adds a module to the contexts container.
     * @param name the element name of the modules configuration element
     * @param module reference to the module */
    void addModule(const std::string & name, Module & module);

    /** returns the module associated with a certain configuration element
     * @param name the element name
     * @returns pointer to the module or NULL, if name does not exist */
    Module * getModule(const std::string & name);

    /** removes a module from the map
     * @param reference to the module */
    void removeModule(Module & module);

    /** This method creates a new node based on a given element name and 
     * a table of attributes. It will delegate the construction to the
     * stored NodeFactory list and return a new Node, if one of the
     * factories can create the desired type. Otherwise it returns NULL.
     * It makes sure that the node is associated with this context, therefore
     * this is the only way to create new nodes for the user.
     * @param name the name of the element 
     * @param attributes a StringTable containing the attributes of the element
     * @return pointer to the new node or NULL
     */
    Node * createNode(const std::string & name, StringTable & attributes);

    /** returns the root node, that contains the tracker tree. This node
     * is an instance of the generic node class and only deal with the
     * children API. It does also contain the configuration element in the first node. 
     * @returns pointer to the root node */
    Node * getRootNode();

    /** returns the node with the given unique ID tag. If none is found
     * NULL is returned.
     * @param id string containing the value of the ID tag
     * @returns pointer to the found node, or NULL.
     */
    Node * findNode(const std::string & id);

    /** This method initialies the context with a tracker tree described by
     * a configuration file. It parses the file and builds the tree.
     * @param filename the path and name of the configuration file */
    void parseConfiguration(const std::string& filename);

    /** calls start on all modules to do some initialization. */
    void start();

    /** This method calls pushState on all modules to get new data into the
     * shared data tree. */
    void pushStates();

    /** calls pullState on all modules to get data out again.*/
    void pullStates();

    /** This method implements the main loop and runs until it is stopped
     * somehow. Then it calls close() on all modules. */
    void run();

    /** tests whether the mainloop should stop, by asking all modules whether
     * they need to stop.*/
    int stop();

    /** calls close on all modules to close any resources.*/
    void close();

    /** add a directory to the front of the directory stack 
     * @param dir directory to add 
     */
    void addDirectoryFirst( const std::string & dir );

    /** add a directory to the end of the directory stack 
     * @param dir directory to add 
     */
    void addDirectoryLast( const std::string & dir );

    /** remove a directory from the directory stack 
     * @param dir directory to add 
     */
    void removeDirectory( const std::string & dir );

    /** tries to locate a file by prepending the directory names in the
     * stack. If a file at that location is found, the full name is 
     * returned in fullname and true will be returned. Otherwise fullname
     * is not changed and false is returned.
     * @param filename the partial filename of the file to look for
     * @param fullname will contain the full filename upon successful return
     * @return returns true, if a file could be found, false otherwise.
     */
    bool findFile( const std::string & filename, std::string & fullname );

    /**
     * returns the XML namespace of the root element 
     * @return string reference containing the XML namespace
     */
    const std::string & getRootNamespace();

    friend class ConfigurationParser;
};

// returns the namespace of the root node
inline const std::string & Context::getRootNamespace()
{
    return rootNamespace;
}

} // namespace ot

#endif
