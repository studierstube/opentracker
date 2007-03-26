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
/** The source file for the basic Graph class.
 *
 * @author Eduardo Veas
 *
 *
 * @file                                                                   */
/* ======================================================================= */


#include <OpenTracker/core/Node.h>
#include <OpenTracker/core/Graph.h>

#include <OpenTracker/core/OtLogger.h>
#include <algorithm>

namespace ot{

  Graph::Graph(){
#ifndef USE_LIVE
      OT_INITIALIZE_IREFCOUNTED;
#endif
  }

Graph::~Graph(){
  for (NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++){
    logPrintE("Graph contains node %p type %s\n", (*it), ((*it)->getType() .c_str()) );
  }
  NodeVector copy(nodes);
  nodes.clear();
};
  
void Graph::addNode(Node * node){
  if (node == 0) return;
  if (node->getGraph() == this) return;
  node->setGraph(this);
  nodes.push_back(node);
  //  nodes[node->getName()]=(node);
};

void Graph::remNode(Node * node){
  NodeVector::iterator search= find(nodes.begin(), nodes.end(), node);
  if (search != nodes.end()){
    // reset the graph
    node->setGraph(0);
    // disconnect from other nodes
    node->disconnect();
    // erase from the list
    
    nodes.erase(search);
  }
  
			      
};
  
void Graph::connectNodes(Node * parent, Node * child){
  if (!parent){
    logPrintE("Graph::connectNodes the parent is 0 \n");
    return;
  } else if ((parent->getGraph()) != this){
    addNode(parent);
    //    parent->setGraph(this);
  }
  
  if (!child ){
    logPrintE("Graph::connectNodes the child is 0 ");
    return;
  } else if ((child->getGraph()) != this){
    addNode(child);
    //    child->setGraph(this);
  }
  
  parent->addChild(*child);
  child->addParent(parent);

};


void Graph::disconnectNodes(Node * parent, Node * child){
  if (!parent || ((parent->getGraph()) == this)){
    logPrintE("Graph::disconnectNodes the parent is 0 or does not belong to graph");
    return;
  }
  if (!child || ((child->getGraph()) == this)){
    logPrintE("Graph::disconnectNodes the child is 0 or does not belong to graph");
    return;
  }
  
  parent->removeChild(*child);
  child->removeParent(parent);
};

Node * Graph::findNode(const std::string & key, const std::string & val){
  Node * result = NULL;
  for (NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++){
    Node * current = (*it);
    const std::string & res = current->get(key);
    if (res.compare(val) == 0){
      result = current;
      break;
    }
      
  }
    return result;
};

unsigned int Graph::countNodes(){
  return (unsigned int)nodes.size();
};

Node * Graph::getNode(unsigned int idx){
  return nodes[(int)idx];
};

#ifndef USE_LIVE
OT_IMPLEMENT_IREFCOUNTED(Graph);
#endif
}; //namespace ot

/* 
 * ------------------------------------------------------------
 *   End of Graph.cxx
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
