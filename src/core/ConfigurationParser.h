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

// selects between usage of XERCES and TinyXML
#include "../tool/XMLSelection.h"


namespace ot {


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
        OT_DOMDOCUMENT * document;
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
        ConfigNode * buildConfigTree( OT_DOMELEMENT * element );

        /**
         * builds the tracker tree starting from a certain DOM_Element.
         * Is used recoursively to walk the DOMTree and create new nodes.
         * @param element pointer to the element to be parsed.
         * @return pointer to the new Node or NULL
         */
        Node * buildTree( OT_DOMELEMENT * element);
        /**
         * parses an Elements attributes and returns a StringTable describing them.
         * @param element pointer to the element
         * @return a StringMap mapping attribute names to attribute values. This map
         *    was created on the heap and it is now owned by the caller.
         */
        StringTable * parseElement( OT_DOMELEMENT * element);

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


}  // namespace ot


#endif

/* 
 * ------------------------------------------------------------
 *   End of ConfigurationParser.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
