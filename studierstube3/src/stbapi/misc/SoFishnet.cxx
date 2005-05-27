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
/** Implementation of SoFishnet class
 *
 * @author Dieter Schmalstieg, Gerd Hesina, Jan Prikryl
 *
 * $Id: SoFishnet.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <cstdio>
#include <cassert>
#include <cmath>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>

#include <stbapi/seams/SoSeamKit.h>
#include <stbapi/misc/SoArtifactKit.h>
#include "SoFishnet.h"

// ---------------------------------------------------------------------------

SO_NODE_SOURCE(SoFishnet);

// ---------------------------------------------------------------------------
// static class initialization

void
SoFishnet::initClass()
{
   SO_NODE_INIT_CLASS(SoFishnet, SoNode, "Node");
}

// ---------------------------------------------------------------------------
// constructor

SoFishnet::SoFishnet() :
   lastHalf(new SbPList),
   fishnetT(new SbPList),
   fishing(FALSE),
   callbackFunction(NULL),
   data(NULL)
{
   SO_NODE_CONSTRUCTOR(SoFishnet);
   SO_NODE_ADD_FIELD(coord,     (0,0,0));
   SO_NODE_ADD_FIELD(artifacts,  (NULL));
   SO_NODE_ADD_FIELD(radius,    (1.0));
   SO_NODE_ADD_FIELD(root,      (NULL));
}

// ---------------------------------------------------------------------------
// destructor. does nothing

SoFishnet::~SoFishnet()
{
   delete lastHalf;
   delete fishnetT;
}

// ---------------------------------------------------------------------------
// switch fishing on or off
// onOrOff: flag for setting fishing mode

void
SoFishnet::setFishing(SbBool onOrOff)
{
   fishing = onOrOff;
}

// ---------------------------------------------------------------------------
// returns: fishing mode

SbBool
SoFishnet::isFishing()
{
   return fishing;
}

// ---------------------------------------------------------------------------
// set function to be called when an object is "caught" in the fishnet
// function: callback to be executes
// userData: user supplied data argument for callback

void
SoFishnet::setCallback(SoFishnetCallback* function, void* userData)
{
   callbackFunction = function;
   data = userData;
}

// ---------------------------------------------------------------------------
// get trafo matrix for given object
// node: object to be considered
// anchestor: node in the path to that object to make it unique
// matrix: returns the computed matrix
// returns: whether the matrix could be computed

SbBool
SoFishnet::getNodeMatrix(SoNode* node, SoNode* anchestor, SbMatrix& matrix)
{
   if(this->root.getValue()==NULL)
   {
      printf("SoFishnet warning: no root set\n");
      return FALSE;
   }
   assert(node);

   // use search action to find path to node
   static SoSearchAction sa;
   sa.setNode(node);
   sa.setInterest(SoSearchAction::ALL);
   sa.apply(this->root.getValue());

   SoPathList& pl = sa.getPaths();
   if(pl.getLength() == 0) 
   {
      printf("SoFishnet warning: path to node not found\n");
      return FALSE;
   }
   //find path that contains anchestor
   int i;
   for(i=0; i<pl.getLength() && !pl[i]->containsNode(anchestor); i++) ;
    
   if(i==pl.getLength())
   {
      printf("SoFishnet warning: path with given ancestor not found\n");
      return FALSE;
   }

   // get matrix from path
   static SbViewportRegion vp;
   static SoGetMatrixAction ma(vp);
   ma.apply(pl[i]);
   matrix = ma.getMatrix();
   return TRUE;
}

// ---------------------------------------------------------------------------
// find the position of a sub graph
// special cases are made for artifacts and seams
// node: object to find position for
// pos: returns the found position
// returns: whether the object's position could be found

SbBool
SoFishnet::getNodePosition(SoNode* node, SbVec3f& pos)
{
   SbMatrix matrix;
   SbBool found = FALSE;
   if(node->isOfType(SoArtifactKit::getClassTypeId()))
   {
      // if it is an artifact then seach for its geometry
      SoArtifactKit* artif = (SoArtifactKit*)node;
      SoNode* geometry = artif->getPart("geometry",TRUE);
      found = getNodeMatrix(geometry,artif,matrix);
   }
   else if(node->isOfType(SoSeamKit::getClassTypeId()))
   {
      SoSeamKit* seam = (SoSeamKit*)node;
      SoNode* geometry = seam->getPart("seamGeometry",TRUE);
      found = getNodeMatrix(geometry,seam,matrix);
   }
   else
   {
      // search for first occurence of node
      found = getNodeMatrix(node,node,matrix);
   }

   if(!found) return FALSE;

   // extract position from matrix
   SbVec3f s; SbRotation r, so;
   matrix.getTransform(pos,r,s,so);
   return TRUE;
}

// ---------------------------------------------------------------------------
// pierce: determine whether an object has penetrated the fishnet polygon
// pos: position of the object in fishnet space
// num: number of polygon vertices
// lastHalf: halfspace in which the object was in last frame
// newHalf: returns the new halfspace in which the object is
// p: position of the projection of pos onto fishnet polygon plane
// returns: true if object has penetrated

SbBool
SoFishnet::pierce(SbVec3f pos, int num, SbBool lastHalf, 
                  SbBool& newHalf, SbVec3f& p)
{
   if(num<3)
   {
      printf("SoFishnet warning: no fishnet coords\n");
      return FALSE;
   }
   
   // expand the list as necessary
   if(fishnetT->getLength() < num)
   {
      int cnt = num - fishnetT->getLength();
      while(cnt--) fishnetT->append(new SbVec3f(0,0,0));
   }

   SbPlane plane(*((SbVec3f*)&fishnetT[0]),
                 *((SbVec3f*)&fishnetT[1]),
                 *((SbVec3f*)&fishnetT[2]));
   newHalf = plane.isInHalfSpace(pos);
   if(lastHalf == newHalf) return FALSE;

   float l = (p-pos).length();
   if(l > radius.getValue()) return FALSE;
   int valid = TRUE;
   int i = 0;
   while(i<num && valid)
   {
      int i2 = (i+1) % num;
      SbVec3f third = *((SbVec3f*)&fishnetT[i]) + plane.getNormal();
      SbPlane wall(*((SbVec3f*)&fishnetT[i]),third,*((SbVec3f*)&fishnetT[i2]));
      SbBool res = wall.isInHalfSpace(p);
      valid &= res;
      i++;
   }
   return valid;
}

// ---------------------------------------------------------------------------
// execute the fishing procedure; called for every render frame

void
SoFishnet::GLRender(SoGLRenderAction*)
{
   // check if fishing mode on
   if(!fishing) return;

   //get matrix of this node
   int i;
   SbMatrix matrix;
   if(!getNodeMatrix(this,this,matrix))
   {
      printf("SoFishnet warning: could not get matrix for coords\n");
      return;
   }

   // transform polygon vertices to fishnet space
   int num = coord.getNum();
   for(i=0; i<num; i++) 
   {
      matrix.multVecMatrix(coord[i],*((SbVec3f*)&fishnetT[i]));
   }

   // check for all artifact whether they have been caught in fishnet
   SoSeparator* sep = ((SoSeparator*)artifacts.getValue());

   // expand the list as necessary
   if(lastHalf->getLength() < sep->getNumChildren())
   {
      int cnt = sep->getNumChildren() - lastHalf->getLength();
      while(cnt--) {
         SbBool * val = new SbBool;
         *val = 0;
         lastHalf->append(val);
      }
   }

   for(i=0; i<sep->getNumChildren() && fishing; i++)
   {
      SbBool res;
      SbBool newHalf;
      SbVec3f p,q, pos;
      SoNode* node = sep->getChild(i);
      if(!getNodePosition(node,pos)) //find position for artifact
      {
         printf("SoFishnet warning: could not find node\n");
      }

      res = pierce(pos,num,*((SbBool*)&lastHalf[i]),newHalf,p);
      *((SbBool*)&lastHalf[i]) = newHalf;
      if(res) //we have caught it
      {
         matrix.inverse().multVecMatrix(p,q);
         if (callbackFunction != NULL) 
            callbackFunction(node,q,data,this); //callback
      }
   }
}

// ---------------------------------------------------------------------------
//eof
