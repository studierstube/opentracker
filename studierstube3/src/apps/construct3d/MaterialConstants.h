// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       MaterialConstants.h
//  TYPE:       c++ header file
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//  AUTHOR:    hk      Hannes Kaufmann
// ===========================================================================
//  HISTORY:   Time of development: Dezember 1999 - 
//
// ===========================================================================


#ifndef _MATERIALCONSTANTS_H_
#define _MATERIALCONSTANTS_H_

#include <Inventor/nodes/SoMaterial.h>
#include "SoPlaneKit.h"
#include "SoCubeKit.h"

class MaterialConstants
{
public:
   static int assignUserColors(int userID);
   static void definePointMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID);
   static void defineLineMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID);
   /* static void defineIntersectionCurveMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID); */
   static void definePlaneMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID);
   static void defineBoolMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID);
   static void defineTransparentMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID);
   static void defineConeMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID);
   static void defineSphereMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID);
   static void defineCylinderMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID);
   static void defineCubeMaterials(SoMaterial& Pw, SoMaterial& Ps, SoMaterial& Pd, SoMaterial& Ph, SoMaterial& Pl, int userID);
   static void setHighlightGridColor(SoMaterial& Ph, int userID);
   static void changePlaneTexture(SoPlaneKit* plane, int state, int currentUserID);
   static void changeCubeTexture(SoCubeKit* cube, int state, int currentUserID);
};

#endif//_MATERIALCONSTANTS_H_
