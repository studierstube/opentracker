/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** main header file for div
  *
  * @author   Gerd Hesina, Jan Prikryl, Gerhard Reitmayr
  *
  * $Id: divmain.h 4078 2004-12-02 13:37:44Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _DIVMAIN_H_INCLUDED_
#define _DIVMAIN_H_INCLUDED_

#ifdef WIN32
#pragma warning(disable:4284)
#endif

#include <Inventor/nodes/SoNode.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/SbLinear.h>

#include <stbapi/stbapi.h>
#include <stbapi/util/StbBuffer.h>
#include <set>

class ACE_INET_Addr;
class ControlModule;
class SoSensor;
class SoOneShotSensor;
class SoFullPath;

/** a Functor to impose an ordering on SbName values to be able to 
 * store them in a set. This is not a meaningful ordering but faster
 * than string comparison. It works because SbNames store their strings
 * as unique representations in an internal hashtable !
 * @ingroup div
 */
class lessString {
public :
    bool operator()( const SbName & n1, const SbName & n2 ) const
    {
        return (n1.getString() < n2.getString());
    }
};

class CDivMain;

typedef void MasterChangeCB( void * data, CDivMain * );

/** This class implements the DIV functionality by keeping distributed copies 
 * of a scene graph synchronized. It listens for changes to one or more 
 * sub scene graphs, marshals them and distributes them to other instances of 
 * it in the distributed system. These instances then demarshal the changes and
 * apply them to their own local copy. As a distribution mechanism it uses a
 * reliable multicast protocol that is driven by the class ControlModule.
 *
 * Any set of connected DIV objects has exactly one master that listens for changes
 * of it's scene graph copy and distributes it to all others (the slaves). Slaves
 * can initiate a master switch to become master. At startup a slave can choose
 * to get a current copy from the master or to work with a copy provided by the
 * application.
 *
 * This class also allows the set filters to suppress the communication of changes
 * to nodes and fields based on node type, node name, node and field instances 
 * (by pointers). These filters can either be set globally for all CDivMain objects
 * or for each CDivMain object individually. They only apply to the master objects.
 *
 * @author Gerd Hesina, Gerhard Reitmayr
 * @ingroup div
 */
class STBAPI_API CDivMain
{
public:
	
	enum Messages {
        DIV_MODIFY = 1,         ///< Field modified
		DIV_SOMF_MODIFY,        ///< MFField modified
		DIV_ADDCHILD,           ///< add a child to a group node
		DIV_INSERTCHILD,        
		DIV_REMOVEALLCHILDREN,
		DIV_REMOVECHILD,
		DIV_REPLACECHILD,
		DIV_TOUCHMODIFIED,
		DIV_SETNAME,
		DIV_SETSOSFNAME,
		DIV_ROOTDELETED,
		DIV_USERMESSAGE,
		DIV_ATOMIC_ACTION_BEGIN,
		DIV_ATOMIC_ACTION_END,
        DIV_JOINED,        
		DIV_HEARTBEAT,
        DIV_REQUEST_NODETRANSFER,
        DIV_TRANSFERNODE,
        DIV_REQUEST_MASTER,
        DIV_TRANSFER_MASTER,
        DIV_LAST_MESSAGE_ID,
        DIV_COMPRESSED = 0xffff    ///< the buffer is compressed, used by ControlModule		
	};

    /** enums for mode definition: */
    enum Mode {
        MODE_MASTER, ///< MODE_MASTER: use this define to initialize the CDivMain class as server
        MODE_SLAVE   ///< MODE_SLAVE:  use this define to initialize the CDivMain class as client
    };

    /** internal state for different functionality like node and master transfer */
    enum State {
        MASTER,         ///< instance is a master for the group
        SLAVE,          ///< instance is a slave for the group
        REQUEST_MASTER, ///< instance requested to become a master
        INIT            ///< instance waits for initial transfer of shared scene graph
    };

    /** constructor. creates the necessary network objects, joins the multicast group. 
     *  if in slave mode and initFromRemote is true, it will initialize the scene graphs
     *  by requesting a copy of the current ones. otherwise it will just start working.
     */
    CDivMain(const char* address, unsigned int port, Mode theMode, bool initFromRemote = false, const char * nic = NULL); 

    /** destructor does nothing */
    virtual ~CDivMain();

	/** In DIV standalone mode (not used in the Studierstube or somewhere else)
	 * make sure this method is called at startup. It initializes a OneShotSensor
	 * that will call the ACE Reactor regularely. It can be omitted in an environment
	 * where the reactor is running anyway. */
	static void startEventHook(void);

    /** prints the types of nodes along a path separated by spaces. */
    void printPath(SoFullPath *path);

    /** sets a node as the root of a shared scene graph. a new SoNodeSensor
     * is created. The node name is set to the given shareName. */
    void shareNode(SoNode *shareNode, const char *shareName=NULL);
    /** removes the SoNodeSensor associated to the given shared node */
    void unshareNode(SoNode *shareNode);

    /** disables the sharing of a node within a shared scene graph by adding
     * its name to the set of disabled names */
    void disableNode(SbName & nodeName);
    /** enables a node name again */
    void enableNode(SbName & nodeName);
    /** disables the sharing of a node within a shared scene graph by adding
     * the node to the set of disabled nodes */
    void disableNode(SoNode *node);
    /** enables a node again */
    void enableNode(SoNode *node);
    /** disables the sharing of a node (with all sub-nodes) within a shared scene graph by adding
     * the node to the set of disabled nodes */
    void disableTree(SoNode *node);
    /** enables a node (with all sub-nodes) again */
    void enableTree(SoNode *node);
    /** disables the sharing of a field within a shared scene graph by adding
    * it to the set of disabled fields */
    void disableField(SoField *field);
    /** enables the field again */
    void enableField(SoField *field);    
    /** disables sharing of a node type */
    void disableNodeType( SoType & type );
    /** enables sharing of a node type */
    void enableNodeType( SoType & type );

    /** disables the sharing of a node within a shared scene graph by adding
     * its name to the set of disabled names */
    static void globalDisableNode(SbName & nodeName);
    /** enables a node name again */
    static void globalEnableNode(SbName & nodeName);
    /** disables the sharing of a node (with all sub-nodes) within a shared scene graph by adding
     * the node to the set of disabled nodes */
    static void globalDisableTree(SoNode *node);
    /** enables a node (with all sub-nodes) again */
    static void globalEnableTree(SoNode *node);
    /** disables the sharing of a node within a shared scene graph by adding
     * the node to the set of disabled nodes */
    static void globalDisableNode(SoNode *node);
    /** enables a node again */
    static void globalEnableNode(SoNode *node);
	/** disables the sharing of a field within a shared scene graph by adding
    * it to the set of disabled fields */
    static void globalDisableField(SoField *field);
    /** enables the field again */
    static void globalEnableField(SoField *field);    
    /** disables sharing of a node type */
    static void globalDisableNodeType( SoType & type );
    /** enables sharing of a node type */
    static void globalEnableNodeType( SoType & type );

    
    /** send an atomic action begin message to all slaves ??
     * @note appearantly this message is only used by the master copy. */
    void atomicActionBegin();
    /** send an atomic action end message to all slaves ??
     * @note again, appearantly this message is only used by the master copy. */
    void atomicActionEnd();

    /** processes received messages within the OpenInventor thread. This 
     * method is called by the SoOneShotSensor callback on slave copies and
     * first processes any user messages and then any DIV messages received
     * in the entryThread method. */
    SbBool networkProcessing( const char* data, const unsigned long len );

    /** called by an SoNodeSensor when the root node got deleted. This removes
     * the sensor as well as sending the appropriate message */
    void rootDeleted(SoSensor *s);

    /** static method registered with a SoNodeSensor to call the method on
     * the CDivMain object passed as data parameter. */    
    static void rootDeletedCB(void *data, SoSensor *s);

    /** This is the most-inner function of DIV. It is called as a callback
     * function from Inventor. Its purpose is to handle all changes to a
     * scene graph.
     * @param s pointer to the triggered sensor    
     */
    void rootChanged(SoSensor *s);

    /** static method registered with a SoNodeSensor to call the method on
     * the CDivMain object passed as data parameter. */
    static void rootChangedCB(void *data, SoSensor *s);

    /** disable DIV. This clears all data structures, unshares all shared nodes
     * and closes the network connection. It is also called from the destructor.
     */
    void disableDiv(void); 

    /** returns the multicast address used */
    const char * getDivAddress() const;

    /** returns the port number used */
    unsigned int getDivPort() const;

    /** returns the mode of the DIV instance 
     * @return the mode */
    const Mode getMode() const {
        return state == MASTER ? MODE_MASTER : MODE_SLAVE;
    }

    /** sets a callback function that is called when the mode of the div object
     * changes. 
     * @param cb pointer to the callback function
     * @param data user data
     */
    void setMasterChangeCB( MasterChangeCB * cb, void * data = NULL );

    /**
     * triggers getting the master mode from the group. if the div is
     * already master nothing happens. if there is a concurrent request
     * to get the master mode, only one gets trought. therefore this
     * method does not guarantee to get the master mode.
     */
    void getMasterMode();

protected:
    SbBool repeatedDagAction(SoNode* node, SoFullPath *path);
    SbBool newDagAction(SoNode* node, SoFullPath *path);
    void resetDagState();
    SbBool checkDag(SoNode* node, SoFullPath *path, int action);
    void setLastNodeAndPath(SoNode* node, SoFullPath *path);
    void lazyNaming(SoNode *changedNode, SoFullPath *changedPath, SoNode *observer);

    /** puts the data into an message block and stuffs it down the output queue */
    void sendData(  const char* data, unsigned long dataLen);

    /** creates a unique name for a node. The naming scheme is as follows:
     * If no name is set, use DIV_MAGIC and the memory pointer node as the new node name.
     * If a name is already set, do nothing.
     * @param node pointer to the node 
     */
    void setNodeName(SoNode* node);

    /** processes an actual DIV message by changing the scene graph accordingly.*/
    virtual SbBool processMessage(StbBuffer *inBuffer, short divCommand);

    /** this flag denotes whether current changes are from message processing or not.
     * if it is true, DIV will not react to any changes. this allows to use several masters
     * without generating loops !
     */
    bool block;

    int atomicActionCounter;
    
    /** set of nodes that are excluded from updates. */
    std::set<SoNode *> excludedNodes;
    /** set of nodes, that define paths starting with that node, are excluded from updates. */
    std::set<SoNode *> excludedTrees;
    /** set of fields that are excluded from updates. */
    std::set<SoField *> excludedFields;
    /** set of node names that are excluded from updates. */
    std::set<SbName, lessString> excludedNames;
    /** set of node types that are excluded from updates. */
    std::set<SoType> excludedTypes;

    /** set of nodes that are excluded from updates by all DIV objects globally.*/    
    static std::set<SoNode *> globalExcludedNodes;
    /** set of nodes, that define paths starting with that node, are excluded from updates by all DIV objects globally.*/
    static std::set<SoNode *> globalExcludedTrees;    
    /** set of fields that are excluded from updates by all DIV objects globally.*/
    static std::set<SoField *> globalExcludedFields;
    /** set of node names that are excluded from updates by all DIV objects globally.*/
    static std::set<SbName, lessString> globalExcludedNames;
    /** set of node types that are excluded from updates by all DIV objects globally.*/
    static std::set<SoType> globalExcludedTypes;

    SbPList dagHistory;
    SbPList atomicList;
    SbPList shareSensorList;
    int dagState;

    /// interface object to the ACE based reliable multicast implementation
    ControlModule * control;
    /// address of the multicast group
	ACE_INET_Addr * mcast_group;
    /// address of the local interface to use
    ACE_INET_Addr * localAddr;
    
    /// stores the state the instance operates in. see the enum State for possible states.
    State state;
    
    /// token to make sure only one gets the master !
    int masterToken;

    /// pointer storing the callback function called when the mode changes
    MasterChangeCB * masterChangeCB;

    /// pointer storing user data
    void * userData;

    /// buffer for oiv marshalling
    StbBuffer buffer;
    /// buffer for demarshalling
    StbBuffer inBuffer;
    
    /// TimeSensor used for initialisation, if required
    SoTimerSensor timeSensor;        
    static void initTimeCB( void * data, SoSensor * sensor );

	static SoOneShotSensor * oneShotSensor;

    friend class ControlModule;
};

#endif
//eof
