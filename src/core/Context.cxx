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
/** source file for Context class
 *
 * @author Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */     
/* ======================================================================= */


#include <cstdlib>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/OS.h>
#include <ace/FILE.h>
#include <ace/Condition_Thread_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <algorithm>

#include <OpenTracker/OpenTracker.h>

// selects between usage of XERCES and TinyXML
#include <OpenTracker/tool/XMLSelection.h>

#ifdef USE_XERCES
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#endif //USE_XERCES

#include <OpenTracker/core/ConfigurationParser.h>
#include <OpenTracker/common/CommonNodeFactory.h>
#include <OpenTracker/common/CallbackModule.h>

#include <OpenTracker/core/Configurator.h>

#ifdef USE_XERCES
XERCES_CPP_NAMESPACE_USE;
const XMLCh ud_node[] = { chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull };
#endif //USE_XERCES

//using namespace std;

// constructor method.

namespace ot {

    Context::Context( int init ) :
        rootNode( NULL ),
        cleanUp( false ),
        stoploopflag(false),
        rootNamespace( "" )
    {
		
		
        if( init != 0 )
        {
            logPrintS("Context::Constructor initializing self with %p\n", this);
            initializeContext( this , NULL);
            cleanUp = true;
        }
        else {
            cleanUp = false;
        }
        directories.push_back(".");
        _havedatamutex = new ACE_Thread_Mutex("contex_havedatamutex");
        _havedatacondition = new ACE_Condition_Thread_Mutex((*_havedatamutex));//, "context_havedatacondition");
        _mutex = new ACE_Thread_Mutex("context_mutex");
    }

    // Destructor method.
    Context::~Context()
    {
        // stop the loop, if it is running
        stopLoop();

        // delete all modules
        if( cleanUp )
        {
            for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
            {
                delete (*it).second;
            }
        }
        modules.clear();
        if (rootNode != NULL) {
#ifdef NO_OT_LOCAL_GRAPH
            delete rootNode;
#endif  //NO_OT_LOCAL_GRAPH
        }
        delete _havedatacondition;
        delete _havedatamutex;
        delete _mutex;
    }

    
    inline void Context::lock() { _mutex->acquire(); };
    inline void Context::unlock() { _mutex->release(); };


    // adds a new factory to the NodeFactoryContainer

    void Context::addFactory(NodeFactory& newfactory)
    {
        factory.addFactory( newfactory );
    }

    // removes a factory from the NodeFactoryContainer

    void Context::removeFactory(NodeFactory & oldfactory)
    {
        factory.removeFactory( oldfactory );
    }

    // adds a module to the contexts collection

    void Context::addModule(const std::string & name, Module & module)
    {
        modules[name] = &module;
        module.context = this;
    }

    // returns a module indexed by its configuration elements name

    Module * Context::getModule(const std::string & name)
    {	Module * result = NULL;
        ModuleMap::iterator it = modules.find( name );
        if( it != modules.end()){
            result = (*it).second;
        } else {

            std::string modname;
            std::string::size_type loc = name.find("Config");
            modname = name.substr(0, loc) + "Module";
            //            logPrintW("Context could not find module %s, requesting Configurator to load %s\n", name.c_str(), modname.c_str());            
            Configurator::loadModule(*this, modname.c_str());
            it = modules.find(name);
            if (it != modules.end())
                result = (*it).second;
        }

        return result;
    }

    // removes a module

    void Context::removeModule(Module & module)
    {
        ModuleMap::iterator it = modules.begin();
        while( it != modules.end())
        {
            if((*it).second == &module )
                modules.erase( it-- );
            it++;
        }
    }

    // calls start on all modules to do some initialization.

    void Context::start()
    {
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            (*it).second->start();
        }
    }

    // calls close on all modules to close any resources.

    void Context::close()
    {
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            (*it).second->close();
        }
        // HACK: give some threads time to close down
        OSUtils::sleep(1000);
    }

    // parses the file and builds the tree.

    void Context::parseConfiguration(const std::string& filename)
    {
#ifndef NO_OT_LOCAL_GRAPH
        file = filename;
        std::string::size_type limit = file.find_last_of( "/\\" );
        if( limit != std::string::npos )
            addDirectoryFirst( file.substr(0, limit));
		
        ConfigurationParser parser( *this );
        rootNode = parser.parseConfigurationFile( filename );
        
#else //NO_OT_LOCAL_GRAPH
#ifdef USE_XERCES
        file = filename;
        std::string::size_type limit = file.find_last_of( "/\\" );
        if( limit != std::string::npos )
            addDirectoryFirst( file.substr(0, limit));

        ConfigurationParser parser( *this );
        rootNode = parser.parseConfigurationFile( filename );

        DOMDocument * doc = ((DOMNode *)(rootNode->parent))->getOwnerDocument();
        doc->setUserData( ud_node, this, NULL );
        
        const XMLCh* xmlspace = ((DOMNode *)(rootNode->parent))->getNamespaceURI();
        
        if (xmlspace != NULL) {
            char * tempName = XMLString::transcode( xmlspace );
            rootNamespace = tempName;
            XMLString::release( &tempName );
        }
        else {
            rootNamespace = "";
        }

#endif //USE_XERCES


#ifdef USE_TINYXML
        file = filename;
        std::string::size_type limit = file.find_last_of( "/\\" );
        if( limit != std::string::npos )
            addDirectoryFirst( file.substr(0, limit));
		
		ConfigurationParser parser( *this );
        rootNode = parser.parseConfigurationFile( filename );
        
        TiXmlDocument * doc = ((TiXmlNode *)(rootNode->parent))->GetDocument();
        doc->SetUserData(this);

#endif //USE_TINYXML
#endif //NO_OT_LOCAL_GRAPH
    }

    void Context::parseConfigurationString(const char* xmlstring)
    {
#ifndef NO_OT_LOCAL_GRAPH
        ConfigurationParser parser( *this );
        rootNode = parser.parseConfigurationString( xmlstring );

        rootNamespace = "";
#else //NO_OT_LOCAL_GRAPH
#ifdef USE_XERCES
        ConfigurationParser parser( *this );
        rootNode = parser.parseConfigurationString( xmlstring );
        
        DOMDocument * doc = ((DOMNode *)(rootNode->parent))->getOwnerDocument();
        doc->setUserData( ud_node, this, NULL );
        
        const XMLCh* xmlspace = ((DOMNode *)(rootNode->parent))->getNamespaceURI();
        
        if (xmlspace != NULL) {
            char * tempName = XMLString::transcode( xmlspace );
            rootNamespace = tempName;
            XMLString::release( &tempName );
        }
        else {
            rootNamespace = "";
        }
#endif //USE_XERCES

#ifdef USE_TINYXML
        ConfigurationParser parser( *this );
        rootNode = parser.parseConfigurationString( xmlstring );
        TiXmlDocument * doc = ((TiXmlNode *)(rootNode->parent))->GetDocument();
        doc->SetUserData(this);
#endif //USE_TINYXML
#endif //NO_OT_LOCAL_GRAPH
    }


    // calls pullEvent on all modules to get data out again.

    void Context::pullEvents()
    {
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            (*it).second->pullEvent();
        }
    }

    // This method calls pushEvent on all modules to get new data into the shared data tree.

    void Context::pushEvents()
    {
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            (*it).second->pushEvent();
        }
    }


    int Context::loopOnce(){
            int stopflag=1;
            // lock the Graph first
            lock();
            // push and pull parts of the main loop
            pushEvents();
            pullEvents();
            // check for stop flag
            stopflag = stop(); 
            // unlock the graph
            unlock();
            return stopflag;
	}
    // This method implements the main loop and runs until it is stopped somehow.

    void Context::run()
    {
        using namespace std;

        start();
        int stopflag = stop();
        while ( stoploopflag == 0 && stopflag == 0 )
        {
            stopflag=loopOnce();
        }

        stoploopflag = 0;

        cout << "closing loop" << endl;
        close();
    }

    // This method implements the main loop and runs until it is stopped somehow.
    // This differs from the run() method in that a (crude) attempt is made 
    // to run at a particular rate. Affected by OS-specific implementation 
    // of wakeup and preemption events; however, can act as a throttle.

    void Context::runAtRate(double rate)
    {
        double t1 = OSUtils::currentTime(); // in milliseconds
        start();
        int stopflag = stop();
        while ( stoploopflag == 0 && stopflag == 0 )
        {
            // push and pull parts of the main loop
  
            stopflag = loopOnce();
  
            double t2 = OSUtils::currentTime(); // in milliseconds
            double sleep_time = 1/rate*1000.0 - (t2 - t1);
            if (sleep_time > 0.f) {
                OSUtils::sleep(sleep_time);
            }
            t1 = t2;
        }

        stoploopflag = 0;

        close();
    }

    void Context::runOnDemand()
    {
        start();
        int stopflag = stop();
        while ( stopflag == 0 )
        {
            waitDataSignal();            
            pushEvents();
            pullEvents();
        }
        close();
    }

    void Context::waitDataSignal() { _havedatacondition->wait(); };
    void Context::dataSignal() { _havedatacondition->signal(); };
    void Context::dataBroadcast() { _havedatacondition->broadcast(); };

    // tests all modules for stopping

    int Context::stop()
    {
        int value = 0;
        for( ModuleMap::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            int retval = (*it).second->stop();
            /*
            if (retval != 0)
            {
                cout << it->first << " not ready for stop" << endl;
            }
            else
            {
                cout << it->first << " ready for stop" << endl;
                }
            */
            value |= retval;
        }
        return value;
    }

    void Context::stopLoop()
    {
        lock();
        stoploopflag = 1;
        unlock();
    }

    // creates a new node from a given element name and an attribute table

    Node * Context::createNode( const std::string & name, StringTable & attributes)
    {
#ifndef NO_OT_LOCAL_GRAPH
        Node * value = factory.createNode( name , attributes );
        if( value != NULL )
        {
            KeyIterator keys(attributes);
            while( keys.hasMoreKeys())
            {

                const std::string & key = keys.nextElement();

                value->put( key, attributes.get( key ));

            }
        }
        return value;
#else //NO_OT_LOCAL_GRAPH
#ifdef USE_XERCES
        Node * value = factory.createNode( name , attributes );
        if( value != NULL )
        {
            // add a correctly created DOM_Element to the node here and return

            DOMDocument * doc = ((DOMNode *)(rootNode->parent))->getOwnerDocument();
            std::auto_ptr<XMLCh> tempName ( XMLString::transcode( name.c_str()));
            std::auto_ptr<XMLCh> tempNS ( XMLString::transcode(rootNamespace.c_str()));
            DOMElement * el = doc->createElementNS( tempNS.get(), tempName.get());
            value->setParent( el );

            // set attributes on the element node
            KeyIterator keys(attributes);
            while( keys.hasMoreKeys())
            {

                const std::string & key = keys.nextElement();

                value->put( key, attributes.get( key ));

                std::auto_ptr<XMLCh> attName ( XMLString::transcode( key.c_str()));
                std::auto_ptr<XMLCh> attVal ( XMLString::transcode( attributes.get( key ).c_str()));
                el->setAttributeNS(tempNS.get(), attName.get(), attVal.get());

            }
        }
        return value;
#endif //USE_XERCES


#ifdef USE_TINYXML
        Node * value = factory.createNode( name , attributes );
        if( value != NULL )
        {
            // add a correctly created DOM_Element to the node here and return
        
            TiXmlDocument * doc = ((TiXmlNode *)(rootNode->parent))->GetDocument();
            const char * tempName = name.c_str();
            const char * tempNS = rootNamespace.c_str();
            TiXmlElement * el = new TiXmlElement(tempName);
            value->setParent( el );

            // set attributes on the element node
            KeyIterator keys(attributes);
            while( keys.hasMoreKeys())
            {
                const std::string & key = keys.nextElement();
                value->put( key, attributes.get( key ));
        
                const char * attName = key.c_str();
                const char * attVal = attributes.get( key ).c_str();
                el->SetAttribute(attName, attVal);

            }
        }
        return value;
#endif //USE_TINYXML
#endif //NO_OT_LOCAL_GRAPH
    }

#ifdef USE_TINYXML
    TiXmlElement *
    findElementRecursive(TiXmlElement* element, const std::string & id)
    {
	// does this element have the attribute we search for?
	if(element->Attribute(id.c_str()))
            return element;

	// walk through all children
	TiXmlElement* child = element->FirstChildElement();
	while(child)
	{
            if(TiXmlElement * found = findElementRecursive(child, id))
                return found;
            child = child->NextSiblingElement();
	}

	// walk through all siblings
	TiXmlElement* sibling = element->NextSiblingElement();
	while(sibling)
	{
            if(TiXmlElement * found = findElementRecursive(sibling, id))
                return found;
            sibling = sibling->NextSiblingElement();
	}

	// finally give up
	return NULL;
    }
#endif //USE_TINYXML

    Node * Context::getRootNode()
    {
        return rootNode;
    }

    Node * Context::findNode(const std::string & id)
    {
#ifndef NO_OT_LOCAL_GRAPH
        return rootNode->findNode("ID", id);
#else //NO_OT_LOCAL_GRAPH
        
        
#ifdef USE_XERCES
        // search for the right node via the DOM_Document API
        std::auto_ptr<XMLCh> tempId ( XMLString::transcode( id.c_str()));
        DOMElement * el = ((DOMNode *)(rootNode->parent))->getOwnerDocument()->getElementById( tempId.get());
        if( el != 0 )
            return (Node *)el->getUserData(ud_node);
        return NULL;
#endif //USE_XERCES


#ifdef USE_TINYXML
	TiXmlElement* el = findElementRecursive(((TiXmlNode*)rootNode->parent)->GetDocument()->RootElement(), id);
        if( el != 0 )
            return (Node *)el->GetUserData();
        return NULL;
#endif //USE_TINYXML
#endif //NO_OT_LOCAL_GRAPH        
    }

    // add a directory to the front of the directory stack

    void Context::addDirectoryFirst( const std::string & dir )
    {
        if( std::find( directories.begin(), directories.end(), dir) == directories.end())
        {
            directories.insert(directories.begin(), dir);
        }
    }

    // add a directory to the end of the directory stack

    void Context::addDirectoryLast( const std::string & dir )
    {
        if( std::find( directories.begin(), directories.end(), dir) == directories.end())
        {
            directories.push_back(dir);
        }
    }

    // remove a directory from the directory stack

    void Context::removeDirectory( const std::string & dir )
    {
        std::vector<std::string>::iterator it = std::find( directories.begin(), directories.end(), dir);
        if( it != directories.end())
        {
            directories.erase(it);
        }
    }

    // searches for a file by prepending a stack of directory names

    bool Context::findFile( const std::string & filename, std::string & fullname )
    {
        ACE_stat stat;
        if( filename.at(0) == '/' || filename.at(0) == '\\' ) // don't deal with paths from root !
        {
            if( ACE_OS::stat( ACE_TEXT_CHAR_TO_TCHAR(filename.c_str()), &stat ) != 0 )
            {
                return false;
            }
            fullname = filename;
            return true;
        }
        std::vector<std::string>::iterator it;
        for( it = directories.begin(); it != directories.end(); it++ )
        {
            std::string name = (*it) + "/" + filename;
            if( ACE_OS::stat( ACE_TEXT_CHAR_TO_TCHAR(name.c_str()), &stat ) == 0 )
            {
                fullname = name;
                return true;
            }
        }
        return false;
    }


    void Context::newVideoFrame(const unsigned char* image, int width, int height, PIXEL_FORMAT format) {
        for(VideoUserVector::iterator it=videoUsers.begin(); it!=videoUsers.end(); it++)
            (*it)->newVideoFrame(image, width, height, format);
    }


    void Context::registerVideoUser(VideoUser* videoUser)
    {
        for(VideoUserVector::iterator it=videoUsers.begin(); it!=videoUsers.end(); it++)
            if(*it == videoUser)
                return;

        videoUsers.push_back(videoUser);
    }


    void Context::unregisterVideoUser(VideoUser* videoUser)
    {
        for(VideoUserVector::iterator it=videoUsers.begin(); it!=videoUsers.end(); it++)
            if(*it == videoUser)
            {
                videoUsers.erase(it);
                return;
            }
    }


	const std::string& Context::getConfigFile(){
		return file;
	}

    void Context::copyFrom(Context & other){
        // lock the loop, so that we can destroy all datastructures
        lock();
        // safely stop all the modules
        close();

        // copy Callbacks from old context
        CallbackModule *cbmodule = dynamic_cast<CallbackModule*>
            (getModule("CallbackConfig"));
        CallbackModule *ocbmodule = dynamic_cast<CallbackModule*>
            (other.getModule("CallbackConfig"));
       
        // this is only possible, if the callback module exists
        // in both contexts
        if (cbmodule && ocbmodule)
        {
            // obtain callback mapping from current module
            fctmap_type fctmap;
            cbmodule->getLocalCallbackMap(fctmap);
           
            // and set it in the newly setup context's module
            fctmap_type::iterator it;
            for (it=fctmap.begin(); it != fctmap.end(); it++)
            {            
                ocbmodule->setCallback(it->first, it->second.first, 
                                       it->second.second);
            }

            // the same for the global callback function
            ocbmodule->setGlobalCallback(cbmodule->getGlobalCallbackFunction(),
                                         cbmodule->getGlobalCallbackData());
        }

        // destroy all the modules
        //if (cleanUp)
        for (ModuleMap::iterator it = modules.begin(); 
             it != modules.end(); it++)
        {
            delete (*it).second;
        }

        modules.clear();
   
        // remove all the factories, as they point to the already removed modules
        factory.removeAll();
        videoUsers.clear();

        // copy the modules from other
        modules = other.modules;
        // copy the rootNode from other
	Node::Ptr tmp = rootNode;
        rootNode = other.rootNode;



        // let the other context clean up the old rootNode
        other.rootNode = tmp;
        // set this context in the new rootNode
#ifdef NO_OT_LOCAL_GRAPH
        
#ifdef USE_XERCES
        DOMDocument * doc = ((DOMNode *)(rootNode->parent))->getOwnerDocument();
        doc->setUserData( ud_node, this, NULL );

        const XMLCh* xmlspace = ((DOMNode *)(rootNode->parent))->getNamespaceURI();
        if (xmlspace != NULL) {
            char * tempName = XMLString::transcode( xmlspace );
            rootNamespace = tempName;
            XMLString::release( &tempName );
        }
        else {
            rootNamespace = "";
        }

#endif //USE_XERCES


#ifdef USE_TINYXML
        TiXmlDocument * doc = ((TiXmlNode *)(rootNode->parent))->GetDocument();
        doc->SetUserData(this);
#endif //USE_TINYXML

#endif //NO_OT_LOCAL_GRAPH
        
        // copy the factories from other
        factory.copyFrom(other.factory);

	// copy the videouser vector
	videoUsers = other.videoUsers;

        // change the cleanUp flag so the other won't destroy the modules when its gone
        this->cleanUp = other.cleanUp;
        other.cleanUp = false;
        
        this->start();
        unlock();
    }

	void Context::addNode(std::string parentid, Node * newnode){
		Node * parent = findNode(parentid);
		Module * mod = getModuleFromNode(newnode);
		if (parent != NULL){
			lock();
			parent->addChild(*newnode);
			mod->addNode(newnode);
			unlock();
		}
	};

	

	Module * Context::getModuleFromNode(const Node * node){
		
		std::string nodename = node->getType();
		
		return getModuleFromNodeType(nodename);
	}

	Module * Context::getModuleFromNodeType(std::string nodename){
		// gcc does not allow doing this
		// char * nodeTypes[3] = {{"Source"},{"Node"},{"Sink"}};
		Module * result= NULL;
		char * source = "Source";
		char * node = "Node";
		char * sink = "Sink";
		char * sinksource = "SinkSource";
		char * nodeTypes[4];
                nodeTypes[1] = source;
                nodeTypes[2] = node;
                nodeTypes[3] = sink;
	        nodeTypes[0] = sinksource;
		std::string modname;
                //        logPrintI("Context GetModuleFromNode node->getType() %s \n", nodename.c_str());
		if ((nodename.compare("NetworkSource")==0) || (nodename.compare("NetworkSink")==0)){
			modname = nodename ;
			result = getModule(modname);
		} else if ((nodename.compare("EventMouseSource") == 0)){
			modname = "EventConfig";
                        result = getModule(modname);
		}else {
			std::string::size_type loc= std::string::npos;
			
			for (int i = 0; i < 4; i++){
				loc = nodename.find(nodeTypes[i]);
				if (loc != std::string::npos)
					break;
			}	

			if (loc != std::string::npos){
				modname = nodename.substr(0, loc);
				modname += "Config";
	
				result = getModule(modname);
                                //				logPrintI("Context GetModuleFromNode %s  result %p\n", modname.c_str(), result);
			}
		}
		return result;


	}

	/* 
	in order to properly remove a node, it is necessary to remove it from the module that created it.
	Most modules keep some datastructures for the nodes they create. These are actualized, for example 
	in the create node method. It seems that either a node is a common node, thus created by the 
	CommonNodeFactory, or it is some kind of specialized node, created by a module. We need to cope with
	this two situations.
	*/
	void Context::removeSubtree(std::string nodeid){

		Node * target = findNode(nodeid);
		
		
		if (target != NULL){
			
			if (CommonNodeFactory::isKnownNode(target->getType())){
				// The node is a common node, and does not belong to a module
				lock();
				// remove the node from the graph
				Node * parent = target->getParent();
				parent->removeChild(*target);
				unlock();
			}else{
				//the node belongs to a module, which must be notified of the node's removal
				// from the graph.
				Module * mod = getModuleFromNode(target);
				if (mod != NULL){
					lock();
					Node * parent = target->getParent();
					parent->removeChild(*target);
					mod->removeNode(target);
					unlock();
				} else{
					// the node cannot be removed, because its module is unknown, we must throw
					logPrintE("Context Remove_Subtree could not remove node of type %s, because its module was not found\n", (target->getType().c_str()));

				}
			}
				
		}
		
	};

	void Context::removeNode(std::string nodeid){

		Node * target = findNode(nodeid);
		logPrintI("Context Remove_Node target is %p\n", target);
		if (target != NULL){
			unsigned int count = target->countChildren();
            logPrintW("Context Remove_Node %u children before removing\n", count);


			logPrintI("Context Remove_Node removing node %p, of type %s\n", target, (target->getType().c_str()));
			if (CommonNodeFactory::isKnownNode(target->getType())){
				// The node is a common node, and does not belong to a module
				Node * parent = target->getParent();
				lock();
				unsigned int childrencount = target->countChildren();
				logPrintI("Context Remove_Node must add %u node's children to its parent's\n", childrencount);
				for(unsigned int i = 0; i < childrencount; i++){
					Node * child = target->getChild(i);
					parent->addChild(*child);
					
					logPrintI("Context Remove_Node adding child of type %s, to parent of type%s\n", ((child->getType()).c_str()), ((parent->getType()).c_str()));
					target->removeChild(*child);					
				}

				// remove the node from the graph

				parent->removeChild(*target);

#ifdef NO_OT_LOCAL_GRAPH
            				target->parent = NULL;
#endif  //NO_OT_LOCAL_GRAPH

				// delete target;
				unlock();
			}else{
				//the node belongs to a module, which must be notified of the node's removal
				// from the graph.
				Module * mod = getModuleFromNode(target);
				if (mod != NULL){
					lock();
					Node * parent = target->getParent();
					parent->removeChild(*target);
					mod->removeNode(target);
					// the node has been removed, now all its children should be passed on to
					// its father.
					unsigned int childrencount = target->countChildren();
					for(unsigned int i = 0; i < childrencount; i++){
						Node * child = target->getChild(i);
						target->removeChild(*child);
						parent->addChild(*child);
					}
					unlock();
				} else{
					// the node cannot be removed, because its module is unknown, we must throw
					logPrintE("Context Remove_Subtree could not remove node of type %s, because its module was not found\n", (target->getType().c_str()));

				}
			}
				
		}


                //		((TiXmlNode*)rootNode->parent)->GetDocument()->Print()	;
	}

	void Context::replaceNode(std::string nodeid, Node * newnode){
		if (newnode != NULL){
			Module * newmodule = getModuleFromNode(newnode);
			Node * target = findNode(nodeid);
			if (target != NULL){
				Module * targetmodule = getModuleFromNode(target);
				lock();
				Node * parent = target->getParent();
				parent->removeChild(*target);
				targetmodule->removeNode(target);

				parent->addChild(*newnode);
				newmodule->addNode(newnode);
				unlock();
			} else{
				// something bad here, should throw
			}
		} else{
			// there is something wrong here, should at least throw
		}
	}

	bool Context::isConfigured(){
		return (rootNode != NULL);
	}

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of Context.cxx
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
