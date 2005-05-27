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
/**
 * header for SoFishnet
 *
 * @author  Dieter Schmalstieg, Gerd Hesina, Jan Prikryl
 *
 * $Id: SoFishnet.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */
#ifndef _SOFISHING_H_
#define _SOFISHING_H_

#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoMFNode.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// ---------------------------------------------------------------------------

class SoFishnet;

// prototype for SoFishnet callback function
typedef void SoFishnetCallback(    //callback for each caught fish
   SoNode* node,                  //pointer to fish
   SbVec3f pos,                   //position of catching in fishnet space
   void* data,                    //user data
   SoFishnet* fishnet             //pointer to fishnet instance
   );

/** SoFishnet has the purpose to determine whether an artifact (not necessarily
 * an SoArtifactKit has penetrated the outline defined by a polygon. For each
 * penetrating artifact, a callback is executed. In that way, sweeping motions
 * with a polygon (normally the pip) can be carried out, for example to select
 * objects or to pick up floating seams. 
 */
class STBAPI_API SoFishnet
:
public SoNode
{
   SO_NODE_HEADER(SoFishnet);

  public:
   SoMFVec3f coord;     //coordinates of the fishnet outline
   SoSFFloat radius;    //tolerance radius for sweeping distance from fishnet
   SoSFNode  artifacts; //Separator containing the artifacts
   SoSFNode  root;      //scene graph root from which to compute positions

   static void initClass( //static class initialization
      );
   SoFishnet(             //constructor
      );
   ~SoFishnet(            //destructor
      );
   void setFishing(       //turn fishing behavior on/off
      SbBool onOrOff     //flag
      );
   SbBool isFishing(      //true if fishing is on
      );
   void setCallback(                 //set fishing callback function
      SoFishnetCallback* function,  //function to call
      void* userData                //user supplied data
      );

  protected:
   virtual void  GLRender(         //called by SoGLRenderAction - do fishing
      SoGLRenderAction* action  //pointer to render action
      );

  private:
   SbBool pierce(         //returns: has a point penetrated the fishnet
      SbVec3f pos,       //the point
      int num,           //number of fishnet polygon points
      SbBool lastHalf,   //halfspace in which point was previously
      SbBool& newHalf,   //halfspace in which point is now
      SbVec3f& p         //position in polygon plane where point penetrated
      );
   SbBool getNodePosition( //compute position of an object in fishnet space
      SoNode* node,      //pointer to object
      SbVec3f& pos       //computed position
      );
   SbBool getNodeMatrix(   //get trafo matrix for an object
      SoNode* node,      //the object
      SoNode* anchestor, //anchestor to define a path
      SbMatrix& matrix   //returned matrix
      );

   SbPList* lastHalf;  //SbBool list of the last halfspace of each object
   SbPList* fishnetT;  //SbVec3f list of polygon points
   SbBool fishing;     //flag for fishing mode
   SoFishnetCallback* callbackFunction; //function to execute
   void* data;                          //user supplied data

}; //SoFishnet

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOFISHING_H_
