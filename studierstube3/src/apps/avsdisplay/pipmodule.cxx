// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       intmodule
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Integer-Slider Input Object 
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    peci      Clemens Pecinovsky
// ===========================================================================
//  HISTORY:
//
//  7-jun-99 08:00:00  peci      created
// ===========================================================================
#define NODEBUG

#include "pipmodule.h"

#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotation.h>


//---------------------------------------------------------------------------
// Constructor
//
// sets nextPipModule and prevPipModule with NULL
// deletes helpGeom 
//
PipModule::PipModule()
{
  nextPipModule=NULL;
  prevPipModule=NULL;
  helpGeom=new SoSeparator;
  helpGeom->ref();
}


//---------------------------------------------------------------------------
// Destructor
//
// deletes the help Geometry 
//
PipModule::~PipModule()
{
  if (prevPipModule!=NULL)
  {
    prevPipModule->nextPipModule=this->nextPipModule;
  }
  if (nextPipModule!=NULL)
  {
    nextPipModule->prevPipModule=this->prevPipModule;
  }
  helpGeom->removeAllChildren();
  helpGeom->unref();
}


//---------------------------------------------------------------------------
// getType()
//
// returns the Type of the Module, 
// in this case it -1 because its an abstract class
//
int PipModule::getType()
{ 
  return -1;
}


//---------------------------------------------------------------------------
// setUpdate(char *)
//
// in this case it does nothing because its an abstract class
//
void PipModule::setUpdate(char *data)
{ 
}


//---------------------------------------------------------------------------
// isModuleForUser(int theUser)
//
// returns TRUE if the Module is enabled for the user theUser.
// the users are encoded in the corresponding bits of the user property...
// this method is used by all Pip and WindowModules....
// 
int PipModule::isModuleForUser(int theUser)
{ 
  return ( ((user>>theUser)&1)==1);
}


//---------------------------------------------------------------------------
// updatePip()
//
// in this case it does nothing because its an abstract class
// 
void PipModule::updatePip(int user,SoSeparator *sep,float &left)
{
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// for all PipModule it should return TRUE
//
int PipModule::isPipModule() 
{
  return TRUE;
}


//---------------------------------------------------------------------------
// isPipModule()
//
// returns wheter the Module is a PipModule or not.
// for all PipModule it should return TRUE
//
void PipModule::setHelpText(char *text,float posy,float posz)
{
  char textName[256];
  char cubeName[256];
  char textTransName[256];
  char cubeTransName[256];

  if (text==NULL)
  {
    helpGeom->removeAllChildren();
    return;
  }

  if (strlen(text)<1)
  {
    helpGeom->removeAllChildren();
    return;
  }

  //generate the names of the cube and the text.....
  sprintf(textName,"TextText%d",id);
  sprintf(cubeName,"TextCube%d",id);
  sprintf(textTransName,"TextTrans%d",id);
  sprintf(cubeTransName,"CubeTrans%d",id);

  //no geometry ->build new geometry...
  if (helpGeom->getNumChildren()==0)
  {
    float minx,miny,minz,maxx,maxy,maxz;
    SoScale *scale=new SoScale;
    SoText3 *textObj=new SoText3;
    SoMaterial *textCol=new SoMaterial;
    SoFont *textFont=new SoFont;
    SoCube *bgCube=new SoCube;
    SoMaterial *bgCol=new SoMaterial;
    SoTranslation *bgTrans=new SoTranslation;
    SoTranslation *textTrans=new SoTranslation;
    SoRotation *rotX=new SoRotation;
    SoSeparator *bgSep=new SoSeparator;
  
    helpGeom->addChild(scale);
    helpGeom->addChild(rotX);
    helpGeom->addChild(bgSep);
    bgSep->addChild(bgCol);
    bgSep->addChild(bgTrans);
    bgSep->addChild(bgCube);
    helpGeom->addChild(textTrans);
    helpGeom->addChild(textCol);
    helpGeom->addChild(textFont);
    helpGeom->addChild(textObj);
    
    scale->scaleFactor.setValue(0.006,0.012,0.006);
    rotX->rotation.setValue(0.2,0.0,0.0,-1.0f);
    textCol->diffuseColor.setValue(0,0,0);
    textFont->size.setValue(4);

    textObj->justification= SoText3::CENTER;
    textObj->string.setValue(text);
    textObj->setName(textName);

    getBBox(textObj,minx,miny,minz,maxx,maxy,maxz);    
    textTrans->translation.setValue(0.0, posy ,posz+1.01);
    textTrans->setName(textTransName);
    bgTrans->translation.setValue(0, (posy)+(maxy-miny)/8.0 ,posz);
    bgTrans->setName(cubeTransName);
    bgCol->diffuseColor.setValue(1.0,1.0,0.4);
    bgCube->width =(maxx-minx)/2.0;
    bgCube->height=(maxy-miny)/2.0;
    bgCube->depth=1.0;
    bgCube->setName(cubeName);
  }
  else
  {
    //use the old geometry, and update its contens
    SoText3 *textObj=(SoText3*)helpGeom->getByName(textName);
    SoCube *bgCube=(SoCube*)helpGeom->getByName(cubeName);
    SoTranslation *bgTrans=(SoTranslation*)helpGeom->getByName(cubeTransName);
    SoTranslation *textTrans=(SoTranslation*)helpGeom->getByName(textTransName);

    if ((textObj!=NULL) && (bgCube!=NULL) && (textTrans!=NULL) && (bgCube!=NULL))
    {
      float minx,miny,minz,maxx,maxy,maxz;

      textObj->string.setValue(text);
      getBBox(textObj,minx,miny,minz,maxx,maxy,maxz);    
      textTrans->translation.setValue(0.0, posy ,posz+1.01);
      bgTrans->translation.setValue(0, (posy)+(maxy-miny)/8.0 ,posz);
      bgCube->width =(maxx-minx)/2.0;
      bgCube->height=(maxy-miny)/2.0;
      bgCube->depth=1.0;
    }
  }
}


//---------------------------------------------------------------------------
// static isLess(PipModule *pipModule1,PipModule *pipModule2)
//
// returns TRUE if the order of pipModule1 is less than the order of 
// pipModule2
// this method is used by the orderPipModules(0 methode to order the modules
//
int PipModule::isLess(PipModule *pipModule1,PipModule *pipModule2)
{
  if (pipModule1->parentId==pipModule2->parentId)
    return (pipModule1->order < pipModule2->order);
  else
    return (pipModule1->parentId < pipModule2->parentId);
}


//---------------------------------------------------------------------------
// getFirstPipModule(PipModule* pipModule)
//
// gets the first PipModule in the pipModule List.
//
PipModule* PipModule::getFirstPipModule(PipModule* pipModule)
{
  if (pipModule==NULL) return NULL;
  
  while (pipModule->prevPipModule!=NULL) 
    pipModule=pipModule->prevPipModule;

  return pipModule;
}


//---------------------------------------------------------------------------
// static orderPipModules(PipModule* pipModule)
//
// orders all PipModules in the PipModule List,
// the list has to be initialized before calling...
// Parameter: 
//    pipModule: the methode needs one pipModule to get the first PipModule 
//               from the list...
//
void PipModule::orderPipModules(PipModule* pipModule)
{
  PipModule *firstPipModule,*curPipModule,*hlpPipModule,*hlp2PipModule;
  int inserted;

  if (pipModule==NULL) return;

  firstPipModule=getFirstPipModule(pipModule);
  if (firstPipModule==NULL) return;

  curPipModule=firstPipModule->nextPipModule;
  if (curPipModule!=NULL)
    curPipModule->prevPipModule=NULL;
  firstPipModule->nextPipModule=NULL;
  firstPipModule->prevPipModule=NULL;

  while (curPipModule!=NULL)
  {
    if (isLess(curPipModule,firstPipModule))
    {
      //it is lower than the first, so its the new first
      hlp2PipModule=curPipModule;
      curPipModule=curPipModule->nextPipModule;
      hlp2PipModule->prevPipModule=NULL;
      hlp2PipModule->nextPipModule=firstPipModule;
      firstPipModule->prevPipModule=hlp2PipModule;
      firstPipModule=hlp2PipModule;
      curPipModule->prevPipModule=NULL;
    }
    else
    {
      hlpPipModule=firstPipModule;
      inserted=FALSE;
      while ((hlpPipModule->nextPipModule!=NULL) && (!inserted) )
      {
        if (isLess(curPipModule,hlpPipModule->nextPipModule))
        {
          //insert in the middle of the list
          hlp2PipModule=curPipModule;
          curPipModule=curPipModule->nextPipModule;
          curPipModule->prevPipModule=NULL;
          hlp2PipModule->prevPipModule=hlpPipModule;
          hlp2PipModule->nextPipModule=hlpPipModule->nextPipModule;
          hlpPipModule->nextPipModule->prevPipModule=hlp2PipModule;
          hlpPipModule->nextPipModule=hlp2PipModule;
          inserted=TRUE;   
        }
        hlpPipModule=hlpPipModule->nextPipModule;
      }
      if (!inserted)
      {
        //insert at the end of the list......
        hlp2PipModule=curPipModule;
        curPipModule=curPipModule->nextPipModule;
        hlpPipModule->nextPipModule=hlp2PipModule;
        hlp2PipModule->nextPipModule=NULL;
        hlp2PipModule->prevPipModule=hlpPipModule;        
      }
    }
  }
}


//---------------------------------------------------------------------------
// static setTransScal(SoNode *node,SoTranslation *trans,SoScale *scale...)
//
// sets the translation and scaling of the node to the spezified size....
// Parameter: 
//   node:     the node which has to be resized...
//   trans:    Pointer to the SoTranslation node
//   scale:    pointer to the SoScale node
//   sx,sy,sz: size that the node should have after scaling
//   px,py:    x and y position of the node....
//   pz:       is ignored, because every widget is positioned in the middle z
//
void PipModule::setTransScal(SoNode *node,SoTranslation *trans,SoScale *scale,float sx,float sy,float sz,float px,float py,float pz)
{
  float minx,miny,minz,maxx,maxy,maxz;
  float fx,fy,fz;

  getBBox(node,minx,miny,minz,maxx,maxy,maxz);

  if (maxx==minx) fx=0; else fx=sx/(maxx-minx);
  if (maxy==miny) fy=0; else fy=sy/(maxy-miny);
  if (maxz==minz) fz=0; else fz=sz/(maxz-minz);
  scale->scaleFactor.setValue(fx,fy,fz);
  //trans->translation.setValue(px-(minx*fx),py*fy-(miny*fy), 0  );  
  trans->translation.setValue(px,py*fy-(miny*fy), 0  );  
}


//---------------------------------------------------------------------------
// static getBBox(SoNode *geom,...)
//
// method to compute the boundingbox of a node, 
// used by setTransScal
// Parameter: 
//    geom: the node the boundign box has to be computed from.
//    minx,miny,minz: min vector of bounding Box
//    maxx,maxy,maxz: max vector of bounding Box
//
void PipModule::getBBox(SoNode *geom,float &minx,float &miny,float &minz,float &maxx,float &maxy,float &maxz)
{
  static SbViewportRegion vp;
  SoGetBoundingBoxAction bba(vp);
  bba.apply(geom);
  SbXfBox3f bbox = bba.getBoundingBox();
  
  bbox.getBounds(minx,miny,minz,maxx,maxy,maxz);
}

