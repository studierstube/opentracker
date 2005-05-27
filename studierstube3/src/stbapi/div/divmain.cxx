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
 * @author  Gerd Hesina, Jan Prikryl, Gerhard Reitmayr
 *
 * $Id: divmain.cxx 4149 2005-04-01 09:35:27Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#define ACE_NLOGGING

#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4800)
#endif

#include <ace/OS.h>

#include <stbapi/div/ControlModule.h>

#include <stbapi/div/divmain.h>

#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFImage.h>

#include <assert.h>

#include <stbapi/util/ivio.h>
#include <stbapi/util/buffer.h>

#define DIV_MAGIC "div"

using namespace std;

SoNode     * lastTriggerNode = NULL;
SoFullPath * lastTriggerPath = NULL;

SoOneShotSensor * CDivMain::oneShotSensor = NULL;

set<SoNode *> CDivMain::globalExcludedNodes;
set<SoNode *> CDivMain::globalExcludedTrees;    
set<SoField *> CDivMain::globalExcludedFields;
set<SbName, lessString> CDivMain::globalExcludedNames;
set<SoType> CDivMain::globalExcludedTypes;

void OneShotSensorCB(void *data, SoSensor *s)
{    
    if( ACE_Reactor::instance()->work_pending() )
    {
        ACE_Time_Value time( 0, 1000 );
        ACE_Reactor::instance()->handle_events( time );
    }
    ((SoOneShotSensor*)s)->schedule();
}

CDivMain::CDivMain(const char* address, unsigned int port, Mode theMode, bool initFromRemote, const char * nic ) :
    block( false ),
    atomicActionCounter(0),
    dagState(0),
    masterToken(0),
    masterChangeCB(NULL),
    userData(NULL),
    buffer(64 * 1024)
{   
    if( theMode == MODE_MASTER )
    {
        state = MASTER;
    }
    else
    {
        if( initFromRemote )
        {
            ACE_DEBUG((LM_DEBUG,"CDivMain : scheduling initTimeCB\n"));
            timeSensor.setData( this );
            timeSensor.setFunction( CDivMain::initTimeCB );
            // timeSensor.setBaseTime( SbTime::getTimeOfDay() + 0.1);
            timeSensor.setInterval( 1.0 );
            timeSensor.schedule();
            state = INIT;
        }
        else
            state = SLAVE;
    }

    ACE_OS::srand(ACE_OS::gettimeofday().msec());

    mcast_group = new ACE_INET_Addr;    
    if (mcast_group->set( port, address ) != 0)
    {
        ACE_DEBUG((LM_DEBUG,"CDivMain : error parsing address %s:%s\n", address, port));
        return;
    }
    /*
    localAddr = new ACE_INET_Addr;
    if (localAddr->set( 0, localip ) != 0 )
    {
        ACE_DEBUG((LM_DEBUG,"CDivMain : error parsing address %s:%s\n", address, port));
        return;
    }
    */
    
	control = new ControlModule( this, mcast_group, true, nic );
}

void CDivMain::startEventHook(void)
{
    // schedule one shot sensor for network processing
    if( oneShotSensor == NULL )
    {
        oneShotSensor = new SoOneShotSensor(OneShotSensorCB, NULL);
        oneShotSensor->schedule();
    }
}

void CDivMain::disableDiv(void)
{
    ACE_DEBUG((LM_DEBUG,"CDivMain : disabling DIV\n"));
    if( timeSensor.isScheduled())
        timeSensor.unschedule();
    excludedNames.clear();
    excludedNodes.clear();
    excludedTrees.clear();
    excludedFields.clear();
    if(state == MASTER)
    {
        for(int i=0;i<shareSensorList.getLength();i++)
        {
            SoNodeSensor *shareSensor=(SoNodeSensor*)(shareSensorList[i]);
            shareSensor->detach();
            shareSensorList.remove(i);
	    if (shareSensor)
	    {
            	delete shareSensor;
	    }
        }    
    }
    control->close();
}

CDivMain::~CDivMain()
{
    disableDiv();
    delete control;
    delete mcast_group;
}

void CDivMain::sendData(const char* data, unsigned long dataLen)
{
   // the control module does any compression and splitting
   // of data etc. is then handled by RMCast classes.
   control->send_data( data, dataLen );
}

void CDivMain::resetDagState()
{
    dagState=0;
    for(int x=0;x<dagHistory.getLength();x++)
        ((SoFullPath*)dagHistory[x])->unref();
    dagHistory.truncate(0); //clear history
}

SbBool CDivMain::newDagAction(SoNode* node, SoFullPath *path)
{
    if(((node==lastTriggerNode)&&(*path==*lastTriggerPath)) ||
        (node!=lastTriggerNode))
    {
        resetDagState();
        return TRUE; //send update
    }
    else
    {
        dagState=1;
        SoFullPath *copyPath=(SoFullPath*)lastTriggerPath->copy();
        copyPath->ref();
        dagHistory.append((void*)copyPath);
        copyPath=(SoFullPath*)path->copy();
        copyPath->ref();
        dagHistory.append((void*)copyPath);
        return FALSE; //do not send update
    }
}

SbBool CDivMain::repeatedDagAction(SoNode* node, SoFullPath *path)
{
    int i;
    for(i=0;i<dagHistory.getLength();i++)
    {
        SoFullPath *historyPath = (SoFullPath*)dagHistory[i];
        SoNode *historyNode = historyPath->getTail();
        if((node!=historyNode) ||
            (*path==*historyPath))
        {
            resetDagState();
            return TRUE; //send update
        }
    }
    SoFullPath *copyPath=(SoFullPath*)path->copy();
    copyPath->ref();
    dagHistory.append((void*)copyPath);
    return FALSE;
}

void CDivMain::printPath(SoFullPath *path)
{
    printf("PATH: ");
    for(int i=0;i<path->getLength();i++)
    {
        //      SbString name = path->getNode(i)->getName().getString();
        const char *type=path->getNode(i)->getTypeId().getName().getString();
        printf("%s ", type);
        /*      if(name!=SbString(""))
        printf("%s %s ", name.getString(),type);
        else
        printf("%p %s ", path->getNode(i),type);
        */
    }
    printf("\n");
}


SbBool CDivMain::checkDag(SoNode* node, SoFullPath* path, int action)
{
/*
printPath(path);
if(lastTriggerPath)
printPath(lastTriggerPath);
else
printf("NULLPATH node: %p", lastTriggerNode);
printf("node %p lastnode %p\n",node, lastTriggerNode);
printf("action = %d\n", action);
    */
    static int lastAction=0;
    if(action!=lastAction)
    {
        resetDagState();
        lastAction=action;
        return TRUE; //send update
    }
    lastAction=action;
    if(dagState==0)
    {
        return newDagAction(node, path);
    }
    else
    {
        return repeatedDagAction(node, path);
    }
}


void CDivMain::setLastNodeAndPath(SoNode* node, SoFullPath *path)
{
    lastTriggerNode=node;
    if(lastTriggerPath) lastTriggerPath->unref();
    if(!path) lastTriggerPath=NULL;
    else
    {
        lastTriggerPath=(SoFullPath*)path->copy();
        lastTriggerPath->ref();
    }
}

void CDivMain::rootDeleted(SoSensor *s)
{
    SoNodeSensor *changedSensor = (SoNodeSensor *)s;
    SoNode  *changedNode  = changedSensor->getAttachedNode();
    
    ACE_DEBUG((LM_DEBUG,"root deleted %s\n",changedNode->getName().getString()));

    buffer.resetBuffer();
    buffer.addShort(DIV_ROOTDELETED);
    buffer.addBytes(changedNode->getName().getLength()+1,
        (void*)changedNode->getName().getString());
    sendData((const char*)buffer.getBuffer(), buffer.getSize());
}

void CDivMain::setNodeName(SoNode* node)
{
    char nodeName[1000];
    SbName realName=node->getName();
    
    if(!realName)
    {
        sprintf(nodeName,"%s%p", DIV_MAGIC, (void*)node);
        node->setName(nodeName);
    }
}

void CDivMain::rootChangedCB(void *data, SoSensor *s)
{
    ((CDivMain*)data)->rootChanged(s);
}

void CDivMain::rootDeletedCB(void *data, SoSensor *s)
{
    ((CDivMain*)data)->rootDeleted(s);
}

void CDivMain::lazyNaming(SoNode *changedNode, SoFullPath *changedPath, SoNode *observer)
{
    SbString   observerName = observer->getName().getString();
    SbName nodeName;
    int pathLength=changedPath->getLength();
    buffer.resetBuffer();
    buffer.addShort(DIV_SETNAME);
    buffer.addBytes(observerName.getLength()+1, (void*)observerName.getString());
    buffer.addInt32(pathLength);
    for(int i=1;i<pathLength;i++)
    {
        int index=((SoFullPath*)changedPath)->getIndex(i);
        buffer.addInt32(index);
    }
    setNodeName(changedNode);
    nodeName=changedNode->getName();
    buffer.addBytes(nodeName.getLength()+1, 
        (void*)nodeName.getString());
    sendData((const char*)buffer.getBuffer(), buffer.getSize());
}

void CDivMain::rootChanged(SoSensor *s)
{
    // short cut, this change is triggered by our own update !
    if( block )
        return;
#ifdef USE_DIV
    SoNodeSensor *changedSensor = (SoNodeSensor *)s;
    SoNode       *changedNode  = changedSensor->getTriggerNode();
    SoField      *changedField = changedSensor->getTriggerField();
    SoNode     *changedChild = changedSensor->getTriggerChild();
    SoFullPath *changedPath  = (SoFullPath*)changedSensor->getTriggerPath();
    
    // check if a node (name or pointer) or field exists in any of the excludelists
    // if yes: do nothing (return)
    if( changedNode != NULL )
    {
        if( excludedNames.find( changedNode->getName())!=excludedNames.end() ||
            excludedNodes.find( changedNode ) != excludedNodes.end() ||
            excludedTypes.find( changedNode->getTypeId()) != excludedTypes.end() ||
            globalExcludedNames.find( changedNode->getName()) != globalExcludedNames.end() ||
            globalExcludedNodes.find( changedNode ) != globalExcludedNodes.end() ||
            globalExcludedTypes.find( changedNode->getTypeId()) != globalExcludedTypes.end ()
           )
        {
            return;
        }
        if( changedField != NULL &&
            ( excludedFields.find( changedField ) != excludedFields.end() ||
              globalExcludedFields.find( changedField ) != globalExcludedFields.end()
            )
          )
        {
            return;
        }
        if( changedPath != NULL )
        {
            set<SoNode*>::iterator i=NULL;
            for (i=excludedTrees.begin();i!=excludedTrees.end();i++)
            {
                if(changedPath->containsNode(*i)) 
                {
                    return;
                }
            }        
            for (i=globalExcludedTrees.begin();i!=globalExcludedTrees.end();i++)
            {	
                if(changedPath->containsNode(*i)) 
                {
                    return;
                }
            }
        }
        else
        {
            // FIXME: Coin3d triggers the sensor too often - which can lead to 
            //        NULL values in changedPath. 20030430 tamer.
#ifdef __COIN__
            return;
#endif
        }
    }
    else
    {
        ACE_DEBUG((LM_DEBUG,"CDivMain: change without a node ???\n"));
        return;
    }
    
    int         changedType  = changedSensor->getTriggerType();
    int         changedIndex = changedSensor->getTriggerIndex();
    SoNode     *observer     = changedSensor->getAttachedNode();

#if 0
    // this is usefull to debug widgets that produce too much div messages.
    printf("Node %x[%s](%s)", changedNode, changedNode->getName().getString(), changedNode->getTypeId().getName().getString());
    if( changedField )
    {
        SbName fieldName;
        changedField->getContainer()->getFieldName( changedField, fieldName );
        SbString fieldValue;
        changedField->get(fieldValue);
        printf(".%x[%s](%s) = %s\n", changedField, fieldName.getString(),changedField->getTypeId().getName().getString(), fieldValue.getString());
    }
    else
    {
        printf("\n");
    }
#endif
    
    if(changedField)
    {
    /*
    Sensor was triggered because of a field change.
    Action: 1) sanity check. changedNode!=NULL
    2) If !nodeName then perform lazyNaming
    3) construct modify buffer
    4) ### print/send modify string
        */  
        SbName nodeName, fieldName;
        SbString fieldString;
        nodeName=changedNode->getName();  // get node name
        if(!nodeName) 
        {
            lazyNaming(changedNode, changedPath, observer); // if no name: lazy naming
            nodeName=changedNode->getName();  // get node name
        }
        
        if(changedField->isOfType(SoSFNode::getClassTypeId()))
        {   
            SoNode* newChangedNode=changedSensor->getTriggerPrevNode();
            SoField *newChangedField=changedSensor->getTriggerPrevField();
            
            if(newChangedNode&&newChangedField)
            {
                SbName newChangedNodeName;
                newChangedNodeName=newChangedNode->getName();
                changedNode->getFieldName(changedField, fieldName);
                if(!newChangedNodeName)
                {
                    newChangedNode->ref();
                    SoSearchAction sa;
                    sa.setSearchingAll(TRUE);
                    sa.setNode(newChangedNode);
                    sa.apply(((SoSFNode*)changedField)->getValue());
                    SoFullPath* path = (SoFullPath*)sa.getPath();
                    assert(path != NULL);
                    path=(SoFullPath*)path->copy();
                    path->ref();
                    int pathLength=path->getLength();

                    buffer.resetBuffer();                        
                    buffer.addShort(DIV_SETSOSFNAME);
                    buffer.addBytes(nodeName.getLength()+1,
                        (void*)nodeName.getString());
                    buffer.addBytes(fieldName.getLength()+1,
                        (void*)fieldName.getString());
                    buffer.addInt32(pathLength);
                    for(int i=1;i<pathLength;i++)
                    {
                        int index = ((SoFullPath*)path)->getIndex(i);
                        buffer.addInt32(index);
                    }
                    setNodeName(newChangedNode);
                    newChangedNodeName=newChangedNode->getName();
                    buffer.addBytes(newChangedNodeName.getLength()+1, 
                        (void*)newChangedNodeName.getString());
                    sendData((const char*)buffer.getBuffer(), buffer.getSize());
                    path->unref();
                    newChangedNode->unrefNoDelete();
                }
                nodeName=newChangedNodeName;
                changedNode=newChangedNode;
                changedField=newChangedField;
            }
        }
        // construct modify buffer
        changedNode->getFieldName(changedField, fieldName);
        
        SbString valueString;
        int theChangedIndex;
        
        if(changedField->isOfType(SoMField::getClassTypeId()) &&
            ((SoMField*)changedField)->getChangedIndexString(theChangedIndex,valueString) )
        {
            buffer.resetBuffer();                
            buffer.addShort(DIV_SOMF_MODIFY);
            buffer.addBytes(nodeName.getLength()+1, (void*)nodeName.getString());
            buffer.addBytes(fieldName.getLength()+1, (void*)fieldName.getString());
            buffer.addInt32(theChangedIndex);
            buffer.addInt32(((SoMField*)changedField)->getNum());
            buffer.addBytes(valueString.getLength()+1, (void*)valueString.getString());
        }
        else
        {
            // Write images as binary fields for performance reasons
            if (changedField->isOfType(SoSFImage::getClassTypeId())) {
                SbName changedNodeName=changedNode->getName();  // get node name
                if(!changedNodeName) {
                    lazyNaming(changedNode, changedPath, observer); // if no name: lazy naming
                    changedNodeName=changedNode->getName();  // get node name
                }
                
                SbVec2s size;
                int nc;
                ((SoSFImage *)changedField)->getValue(size,nc);

                buffer.resetBuffer();                    
                buffer.addShort(DIV_TOUCHMODIFIED);
                buffer.addBytes(changedNodeName.getLength()+1, 
                                (void*)changedNodeName.getString());
#ifdef __COIN__
                // FIXME: for some reason if the node is transferred binary in
                //        Coin it runs into a refcount assertion. further
                //        investigations revealed, that the SoContext node never
                //        passes through if not set to binary. 20030515 tamer.
                buffer.addNode( changedNode, FALSE, TRUE );
#else
                buffer.addNode( changedNode, TRUE, TRUE );
#endif
            }
            else {
                changedField->get(fieldString);
                buffer.resetBuffer();                    
                buffer.addShort(DIV_MODIFY);
                buffer.addBytes(nodeName.getLength()+1, (void*)nodeName.getString());
                buffer.addBytes(fieldName.getLength(), (void*)fieldName.getString());
                buffer.addChar(' ');
                buffer.addBytes(fieldString.getLength()+1, 
                    (void*)fieldString.getString());
                // printf("modify string %s\n", fieldString.getString());
            }
        }
        
        //     const char *buf = (const char*)outBuffer.getBuffer();
        //     fprintf(stderr,"%d: %d %s\n",*((int*)buf),*(short*)(buf+4),buf+6);
        
        // ### print/send modify string
        sendData((const char*)buffer.getBuffer(), buffer.getSize());
        return;
    }
    /*
    changedNode!=NULL, changedField==NULL.
    Sensor was triggered because of a node change. No fields were changed!
    Possible cases: 
    1) no group node. assume touch on this node
    2) addChild
    3) insertChild
    4) removeAllChildren
    5) removeChild
    6) replaceChild
    Action: 1) Get changedNodeName
    2) If !changedNodeName then perform lazyNaming
    3) Determine case. Use changedType
    4) Execute necessary code
    4a) Determine DAG. If a dag notification has been detected: do not send update
    */
    
    SbName changedNodeName=changedNode->getName();  // get node name
    if(!changedNodeName)
    {
        lazyNaming(changedNode, changedPath, observer); // if no name: lazy naming
        changedNodeName=changedNode->getName();  // get node name
    }
    printf("Node Changed: %s\n",changedNodeName.getString());
    switch(changedType)
    {
    case SoGroup::GROUP_NO_GROUP: // not a structure change. assume touch
        if(checkDag(changedNode, changedPath, changedType))
        {
            if(changedNode&&changedPath)
            {
                buffer.resetBuffer();                    
                buffer.addShort(DIV_TOUCHMODIFIED);
                buffer.addBytes(changedNodeName.getLength()+1, 
                                (void*)changedNodeName.getString());
#ifdef __COIN__
                // FIXME: for some reason if the node is transferred binary in
                //        Coin it runs into a refcount assertion. further
                //        investigations revealed, that the SoContext node never
                //        passes through if not set to binary. 20030515 tamer.
                buffer.addNode( changedNode, FALSE, TRUE );
#else
                buffer.addNode( changedNode, TRUE, TRUE );
#endif

                // ### print/send touch-modified values string
                sendData((const char*)buffer.getBuffer(), buffer.getSize());
            }
        }
        break;
    case SoGroup::GROUP_ADDCHILD:
        {
            setNodeName(changedChild);
            if(checkDag(changedNode, changedPath, changedType))
            {
                // construct addChild string
                buffer.resetBuffer();    
                buffer.addShort(DIV_ADDCHILD);
                buffer.addBytes(changedNodeName.getLength()+1, 
                                (void*)changedNodeName.getString());
                buffer.addBytes(changedChild->getName().getLength()+1, 
                                (void*)changedChild->getName().getString());

#ifdef __COIN__
                // FIXME: for some reason if the node is transferred binary in
                //        Coin it runs into a refcount assertion. further
                //        investigations revealed, that the SoContext node never
                //        passes through if not set to binary. 20030515 tamer.
                buffer.addNode( changedChild, FALSE, TRUE );
#else
                buffer.addNode( changedChild, TRUE, TRUE );
#endif
                
                // ### print/send addChild string
                sendData((const char*)buffer.getBuffer(), buffer.getSize());
            }
        }
        break;
    case SoGroup::GROUP_INSERTCHILD:
        {
            setNodeName(changedChild);
            if(checkDag(changedNode, changedPath, changedType))
            {
                buffer.resetBuffer();    
                buffer.addShort(DIV_INSERTCHILD);
                buffer.addBytes(changedNodeName.getLength()+1, 
                                (void*)changedNodeName.getString());
                buffer.addBytes(changedChild->getName().getLength()+1, 
                                (void*)changedChild->getName().getString());
                buffer.addInt32(changedIndex);
#ifdef __COIN__
                // FIXME: for some reason if the node is transferred binary in
                //        Coin it runs into a refcount assertion. further
                //        investigations revealed, that the SoContext node never
                //        passes through if not set to binary. 20030515 tamer.
                buffer.addNode( changedChild, FALSE, TRUE );
#else
                buffer.addNode( changedChild, TRUE, TRUE );
#endif
                
                // ### print/send insertChild string
                sendData((const char*)buffer.getBuffer(), buffer.getSize());
            }
        }
        break;
    case SoGroup::GROUP_REMOVEALLCHILDREN:
        {
            if(checkDag(changedNode, changedPath, changedType))
            {
                // construct removeAllChildren string
                buffer.resetBuffer();    
                buffer.addShort(DIV_REMOVEALLCHILDREN);
                buffer.addBytes(changedNodeName.getLength()+1, 
                    (void*)changedNodeName.getString());
                // ### print send removeAllChildren string
                sendData((const char*)buffer.getBuffer(), buffer.getSize());
                
            }
        }
        break;
    case SoGroup::GROUP_REMOVECHILD:
        {
            if(checkDag(changedNode, changedPath, changedType))
            {
                // construct removeChild string
                buffer.resetBuffer();    
                buffer.addShort(DIV_REMOVECHILD);
                buffer.addBytes(changedNodeName.getLength()+1, 
                    (void*)changedNodeName.getString());
                buffer.addInt32(changedIndex);
                
                // ### print/send removeChild string
                sendData((const char*)buffer.getBuffer(), buffer.getSize());
            }
        }
        break;
    case SoGroup::GROUP_REPLACECHILD:
        {
            setNodeName(changedChild);
            if(checkDag(changedNode, changedPath, changedType))
            {
                // construct replaceChild string 
                buffer.resetBuffer();    
                buffer.addShort(DIV_REPLACECHILD);
                buffer.addBytes(changedNodeName.getLength()+1, 
                                (void*)changedNodeName.getString());
                buffer.addBytes(changedChild->getName().getLength()+1, 
                                (void*)changedChild->getName().getString());
                buffer.addInt32(changedIndex);
#ifdef __COIN__
                // FIXME: for some reason if the node is transferred binary in
                //        Coin it runs into a refcount assertion. further
                //        investigations revealed, that the SoContext node never
                //        passes through if not set to binary. 20030515 tamer.
                buffer.addNode( changedChild, FALSE, TRUE );
#else
                buffer.addNode( changedChild, TRUE, TRUE );
#endif
                
                // ### print/send replaceChild string
                sendData((const char*)buffer.getBuffer(), buffer.getSize());
            }
        }
        break;
    default:
        ACE_DEBUG((LM_DEBUG,"changedType=%d changedNode = %x changedNodeType = %s\n",
            changedType, changedNode, changedNode->getTypeId().getName().getString()));
        return;
    }
    setLastNodeAndPath(changedNode, changedPath);
#endif
}


SbBool CDivMain::networkProcessing( const char* data, const unsigned long len )
{
    inBuffer.setBuffer((void *) data, len );    
    short divCommand=inBuffer.getShort();        

    if(divCommand==DIV_ATOMIC_ACTION_BEGIN)
    {
        atomicActionCounter++;
        return TRUE;
    }
    else
    {
        block = true;  // critical section for processing changes to the scene graph !
        if(divCommand==DIV_ATOMIC_ACTION_END)
        {
            atomicActionCounter--;
            if(atomicActionCounter==0)
            {
                int atomicSize=atomicList.getLength();
                for(int packetCounter=0; packetCounter<atomicSize; packetCounter++)
                {
                    StbBuffer *thisBuffer=(StbBuffer*)(atomicList[packetCounter]);
                    (void)thisBuffer->getULong();
                    int thisCommand=thisBuffer->getShort();
                    //                  fprintf(stderr,"atomic action command: %d\n",thisCommand);
                    processMessage(thisBuffer, thisCommand);
                    delete thisBuffer;
                }
                atomicList.truncate(0);
            }
            return TRUE;
        }
        if(atomicActionCounter==0)
            processMessage(&inBuffer, divCommand);
        else
        {
            StbBuffer* helpBuffer=new StbBuffer(&inBuffer);
            atomicList.append((void*)helpBuffer);
        }
        block = false;  // end of scene graph critical section
    }   
    return TRUE;
}

SbBool CDivMain::processMessage(StbBuffer *inBuffer, short divCommand)
{
    // ignore everything until the one of the startup messages comes through !
    ACE_DEBUG((LM_DEBUG,"CDivMain::processMessage command %T, %d\n", divCommand ));
    if( state == INIT && divCommand != DIV_TRANSFERNODE )
        return TRUE;
    switch(divCommand)
    {
    case DIV_MODIFY:
        {
            char *theString=inBuffer->getString();
            char *nodeString=theString;
            SoNode *modifyNode=SoNode::getByName(theString);
            //findNode(SoNode::getByName(rootString),theString);
            theString=inBuffer->getString();
            // printf("modify: %s set %s\n", nodeString, theString);
            if(modifyNode) 
            {
                if( modifyNode->set(theString) == FALSE)
                    printf("error setting above field\n");
            }
            else
                ACE_DEBUG((LM_DEBUG,"modify: %s set %s NOT FOUND!!!\n", nodeString, theString));
            break;
        }
    case DIV_SOMF_MODIFY:
        {
            char *nodeString=inBuffer->getString();
            SoNode *modifyNode=SoNode::getByName(nodeString);
            char *fieldString=inBuffer->getString();
            if(modifyNode)
            {
                SoMField *theMField = (SoMField*) modifyNode->getField(fieldString);
                int changedIndex = inBuffer->getInt32();
                int numValues=inBuffer->getInt32();
                if(numValues!=theMField->getNum())
                    theMField->setNum(numValues);
                char *valueString = inBuffer->getString();
                theMField->set1(changedIndex, valueString);
                //                      fprintf(stderr,"mf_modify: node: %s field: %s set1(%d, %s)\n", nodeString, fieldString, changedIndex, valueString);
            }
            else
                ACE_DEBUG((LM_DEBUG,"mf_modify: %s set1 %s NOT FOUND!!!\n", nodeString, fieldString));
            break;
        }
    case DIV_ADDCHILD:
        {
            char *theString=inBuffer->getString();
            SoNode *addChildNode=SoNode::getByName(theString);
            assert(addChildNode);
            char *newChildString=inBuffer->getString();
            SoNode *newNode=NULL;
            if(newChildString&&*newChildString!='\0') newNode=SoNode::getByName(newChildString);
            if(!newNode)
                newNode = inBuffer->getNode();
            assert(newNode);
            ((SoGroup*)addChildNode)->addChild(newNode);
            break;
        }
    case DIV_INSERTCHILD:
        {
            char *theString=inBuffer->getString();
            SoNode *insertChildNode=SoNode::getByName(theString);
            assert(insertChildNode);
            char *newChildString=inBuffer->getString();
            SoNode *newNode=NULL;
            if(newChildString&&*newChildString!='\0') newNode=SoNode::getByName(newChildString);
            int index=inBuffer->getInt32();
            if(!newNode)
                newNode = inBuffer->getNode();
            assert(newNode);
            ((SoGroup*)insertChildNode)->insertChild(newNode, index);
            break;
        }
    case DIV_REMOVECHILD:
        {
            char *theString=inBuffer->getString();
            SoNode *removeChildNode=SoNode::getByName(theString);
            assert(removeChildNode);
            int index=inBuffer->getInt32();
            ((SoGroup*)removeChildNode)->removeChild(index);
            break;
        }
    case DIV_REPLACECHILD:
        {
            char *theString=inBuffer->getString();
            SoNode *replaceChildNode=SoNode::getByName(theString);
            assert(replaceChildNode);
            char *newChildString=inBuffer->getString();
            int index=inBuffer->getInt32();
            SoNode *newNode=NULL;
            if(newChildString&&*newChildString!='\0') newNode=SoNode::getByName(newChildString);
            if(!newNode)
                newNode = inBuffer->getNode();
            assert(newNode);
            ((SoGroup*)replaceChildNode)->replaceChild(index, newNode);
            break;
        }
    case DIV_REMOVEALLCHILDREN:
        {
            char *theString=inBuffer->getString();
            SoNode *removeAllChildrenNode=SoNode::getByName(theString);
            assert(removeAllChildrenNode);
            ((SoGroup*)removeAllChildrenNode)->removeAllChildren();
            break;
        }
    case DIV_SETNAME:
        {
            char *theString=inBuffer->getString();
            SoNode *setNameNode=SoNode::getByName(theString);
            if(!setNameNode)
            {
                ACE_DEBUG((LM_DEBUG,"DIV_SETNAME couldn't find : %s\n",theString));
                break;
            }
            int pathLength=inBuffer->getInt32();
            for(int i=1;i<pathLength;i++)
            {
                int index=inBuffer->getInt32();
                SoChildList *thisChildList=setNameNode->getChildren();
                setNameNode=(SoNode*)(thisChildList->get(index));
            }
            assert(setNameNode);
            theString=inBuffer->getString();
            setNameNode->setName(theString);
            break;
        }
    case DIV_SETSOSFNAME:
        {
            char *theString=inBuffer->getString();
            SoNode *setNameNode=SoNode::getByName(theString);
            if(!setNameNode)
            {
                ACE_DEBUG((LM_DEBUG,"DIV_SETSOSFNAME couldn't find : %s\n",theString));
                break;
            }
            char *fieldName=inBuffer->getString();
            SoField *field=setNameNode->getField(fieldName);
            assert(field);
            setNameNode=((SoSFNode*)field)->getValue();
            assert(setNameNode);
            int pathLength=inBuffer->getInt32();
            for(int i=1;i<pathLength;i++)
            {
                int index=inBuffer->getInt32();
                SoChildList *thisChildList=setNameNode->getChildren();
                setNameNode=(SoNode*)(thisChildList->get(index));
            }
            assert(setNameNode);
            theString=inBuffer->getString();
            setNameNode->setName(theString);
            break;
        }
    case DIV_ROOTDELETED:
        {
            ACE_DEBUG((LM_DEBUG,"root deleted: >>%s<<\n",inBuffer->getString()));
            break;
        }
    case DIV_TOUCHMODIFIED:
        {
            char *theString=inBuffer->getString();
            SoNode *touchModifiedNode=SoNode::getByName(theString);
            // assert(touchModifiedNode);
            if( touchModifiedNode == NULL )
                break;
            SoNode *newNode=NULL;
            newNode = inBuffer->getNode();
            assert(newNode);
            newNode->ref();
            touchModifiedNode->copyFieldValues(newNode);
            newNode->unref();
            touchModifiedNode->touch();
            break;
        }
    case  DIV_REQUEST_NODETRANSFER:
        {            
            // only for master, pack everything in a group and send it over !
            if( state == MASTER )
            {                                   
                ACE_DEBUG((LM_DEBUG,"CDivMain:processMessage request node transfer %T\n"));
                SoGroup * group = new SoGroup;
                group->ref();
                for(int i=0; i < shareSensorList.getLength(); i++)
                {
                    SoNodeSensor *shareSensor=(SoNodeSensor*)(shareSensorList[i]);
                    if( group->findChild( shareSensor->getAttachedNode()) == -1 )
                        group->addChild( shareSensor->getAttachedNode());
                }
                buffer.resetBuffer();
                buffer.addShort(DIV_TRANSFERNODE);
#ifdef __COIN__
                // FIXME: for some reason if the node is transferred binary in
                //        Coin it runs into a refcount assertion. further
                //        investigations revealed, that the SoContext node never
                //        passes through if not set to binary. 20030515 tamer.
                buffer.addNode( group, FALSE, TRUE );
#else
                buffer.addNode( group, TRUE, TRUE );
#endif
                sendData((const char*)buffer.getBuffer(), buffer.getSize());                
                group->unref();
            } 
            break;
        }
    case DIV_TRANSFERNODE:
        {
            // only for slaves
            // find the nodes and replace they children with the new ones
            if( state == INIT )  // this one is for us !
            {
                SoNode * newNode;
                newNode = inBuffer->getNode();
                assert( newNode );
                assert( newNode->isOfType( SoGroup::getClassTypeId()));
                SoGroup * group = (SoGroup *)newNode;
                group->ref();
                for( int i = 0; i < group->getNumChildren(); i++ )
                {
                    newNode = group->getChild( i );
                    if( newNode->isOfType( SoGroup::getClassTypeId()))
                    {
                        // this is necessary to find the old node of the given name
                        // because the new node has the same name, getByName would return 
                        // the new node.
                        SoNodeList list;
                        (void)SoNode::getByName( newNode->getName(), list );
                        SoNode * oldNode = NULL;
                        for( int j = 0; j < list.getLength(); j++ )
                        {
                            if( list[j] != newNode )
                            {
                                oldNode = list[j];
                                break;
                            }
                        }
                        if( oldNode != NULL && oldNode->isOfType( SoGroup::getClassTypeId()))
                        {
                            SoGroup * newGroup = (SoGroup *) newNode;
                            SoGroup * oldGroup = (SoGroup *) oldNode;                            
                            oldGroup->removeAllChildren();
                            for( int j = 0; j < newGroup->getNumChildren(); j++ )
                            {
                                oldGroup->addChild( newGroup->getChild(j));
                            }
                        }
                        else
                            printf("CDivMain::NODETRANSFER oldNode not present or not a group\n");
                    }
                    else
                        printf("CDivMain::NODETRANSFER newNode not a group\n");
                }
                group->unref();
                state = SLAVE;
            }
            break;
        }
    case DIV_REQUEST_MASTER:
        {
            if( state == MASTER )
            {
                int token = inBuffer->getInt32();
                buffer.resetBuffer();
                buffer.addShort(DIV_TRANSFER_MASTER);
                buffer.addInt32( token );
                state = SLAVE;
                for(int i=0;i<shareSensorList.getLength();i++)
                {
                    SoNodeSensor *shareSensor=(SoNodeSensor*)(shareSensorList[i]);
                    SoNode * node = shareSensor->getAttachedNode();
                    shareSensor->detach();
                    shareSensorList.remove(i);
                    buffer.addString((char *)node->getName().getString());
                    delete shareSensor;
                }
                sendData((const char*)buffer.getBuffer(), buffer.getSize());
                if( masterChangeCB != NULL )
                    (*masterChangeCB)( userData, this );                
            }
            break;
        }
    case DIV_TRANSFER_MASTER:
        {            
            if( state == REQUEST_MASTER )
            {
                int token = inBuffer->getInt32();
                if( token == masterToken )
                {
                    state = MASTER;
                    char * nodeName;
                    while((nodeName = inBuffer->getString()) != NULL )
                    {
                        SoNode * node = SoNode::getByName( nodeName );
                        if( node != NULL )
                            shareNode( node, nodeName );
                    }
                    if( masterChangeCB != NULL )
                        (*masterChangeCB)( userData, this );

                }
                else
                {
                    // reset because of collision....
                    state = SLAVE;  
                }
            }
            break;
        }
    default:
        ACE_DEBUG((LM_DEBUG,"DIV: unknown message type %d\n", divCommand ));
        return FALSE;
    }
    
    return TRUE;
}

void CDivMain::shareNode(SoNode *shareNode, const char *shareName)
{
    if(!shareName)
    {
        SbName realName=shareNode->getName();
        if(!realName)
        {
            printf("ERROR: CDivMain::shareNode root node has no name ! You have to specify one for DIV to work correctly! \n");
            exit(1);
        }
    }
    else
        shareNode->setName(shareName);
    
    SoNodeSensor *shareSensor = new SoNodeSensor(CDivMain::rootChangedCB, (void*)this);
    shareSensor->setPriority(0);
    shareSensor->setDeleteCallback(CDivMain::rootDeletedCB, (void*)this);
    shareSensor->setTriggerPathFlag(TRUE);
    shareSensor->attach(shareNode);
    shareSensorList.append((void*)shareSensor);
}

void CDivMain::unshareNode(SoNode *shareNode)
{
    for(int i=0;i<shareSensorList.getLength();i++)
    {
        SoNodeSensor *shareSensor=(SoNodeSensor*)(shareSensorList[i]);
        if(shareSensor->getAttachedNode()==shareNode)
        {
            shareSensor->detach();
            shareSensorList.remove(i);
            delete shareSensor;
            break;
        }    
    }
}

void CDivMain::setMasterChangeCB( MasterChangeCB * cb, void * data )
{
    masterChangeCB = cb;
    userData = data;
}

void CDivMain::getMasterMode()
{
    if( state != SLAVE )
        return;
    state = REQUEST_MASTER;
    masterToken = ACE_OS::rand() + 1;
    buffer.resetBuffer();
    buffer.addShort(DIV_REQUEST_MASTER);   
    buffer.addInt32( masterToken );
    sendData((const char*)buffer.getBuffer(), buffer.getSize()); 
    ACE_DEBUG((LM_DEBUG,"CDivMain::getMasterMode %T\n"));
}

void CDivMain::atomicActionBegin()
{
    buffer.resetBuffer();
    buffer.addShort(DIV_ATOMIC_ACTION_BEGIN);
    sendData((const char*)buffer.getBuffer(), buffer.getSize());    
}

void CDivMain::atomicActionEnd()
{
    buffer.resetBuffer();
    buffer.addShort(DIV_ATOMIC_ACTION_END);
    sendData((const char*)buffer.getBuffer(), buffer.getSize());    
}

void CDivMain::disableNode(SbName & nodeName)
{
    excludedNames.insert( nodeName );
}

void CDivMain::enableNode(SbName & nodeName)
{
    excludedNames.erase( nodeName );
}

void CDivMain::disableNode(SoNode *node)
{
    excludedNodes.insert( node );
}

void CDivMain::enableNode(SoNode *node)
{
    excludedNodes.erase( node );
}

void CDivMain::disableTree(SoNode *node)
{
    excludedTrees.insert( node );
}

void CDivMain::enableTree(SoNode *node)
{
    excludedTrees.erase( node );
}

void CDivMain::disableField(SoField *field)
{
    excludedFields.insert( field );
}

void CDivMain::enableField(SoField *field)
{
    excludedFields.erase( field );
}

void CDivMain::disableNodeType( SoType & type )
{
    excludedTypes.insert( type );
}

void CDivMain::enableNodeType( SoType & type )
{
    excludedTypes.erase( type );
}

void CDivMain::globalDisableNode(SbName & nodeName)
{
    globalExcludedNames.insert( nodeName );
}

void CDivMain::globalEnableNode(SbName & nodeName)
{
    globalExcludedNames.erase( nodeName );
}

void CDivMain::globalDisableNode(SoNode *node)
{
    globalExcludedNodes.insert( node );
}

void CDivMain::globalEnableNode(SoNode *node)
{
    globalExcludedNodes.erase( node );
}

void CDivMain::globalDisableTree(SoNode *node)
{
    globalExcludedTrees.insert( node );
}

void CDivMain::globalEnableTree(SoNode *node)
{
    globalExcludedTrees.erase( node );
}

void CDivMain::globalDisableField(SoField *field)
{
    globalExcludedFields.insert( field );
}

void CDivMain::globalEnableField(SoField *field)
{
    globalExcludedFields.erase( field );
}

void CDivMain::globalDisableNodeType( SoType & type )
{
    globalExcludedTypes.insert( type );
}

void CDivMain::globalEnableNodeType( SoType & type )
{
    globalExcludedTypes.erase( type );
}

const char * CDivMain::getDivAddress() const
{
    return mcast_group->get_host_addr();
}

unsigned int CDivMain::getDivPort() const
{
    return mcast_group->get_port_number();
}

void CDivMain::initTimeCB( void * data, SoSensor * sensor )
{
    CDivMain * self = (CDivMain *)data;
    if( self->state == INIT )
    {
        ACE_DEBUG((LM_DEBUG,"CDivMain: request node transfer %T\n"));
        // send DIV_REQUEST_NODETRANSFER
        self->buffer.resetBuffer();
        self->buffer.addShort(DIV_REQUEST_NODETRANSFER);
        self->sendData((const char*)self->buffer.getBuffer(), self->buffer.getSize()); 
		SoTimerSensor * timer = (SoTimerSensor *)sensor;
		if( timer->getInterval() < 10.0)
		{
			timer->setInterval( 15.0 );
		}
    }
    else
    {
        if(((SoTimerSensor *)sensor)->isScheduled())
            ((SoTimerSensor *)sensor)->unschedule();
    }
}

/* ===========================================================================
End of divmain.cxx
===========================================================================
Automatic Emacs configuration follows.
Local Variables:
mode:c++
c-basic-offset: 4
eval: (c-set-offset 'substatement-open 0)
eval: (c-set-offset 'case-label '+)
eval: (c-set-offset 'statement 'c-lineup-runin-statements)
eval: (setq indent-tabs-mode nil)
End:
=========================================================================== */
