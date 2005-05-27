#define NODEBUG

#include "Bin2IV.h"

#include <stbapi/misc/SoStreamLine.h>

/****************************************************************************
prototype:      SoXXX *CBuildTree::NewXXX(char *buf);
purpose:        Build a SoXXX object out of the buffer (used for parsing)
args:           buf... pointer to object data
return:         pointer to created object
notes:          some functions are not tested yet!
****************************************************************************/
SoCoordinate3 *CBuildTree::NewCoordinate3(char *buf)
{
  SoCoordinate3 *coord;
  int numC3,curC3,t;
  float x, y, z;

  curC3=GetInt(buf);buf+=sizeof(int);
  numC3=GetInt(buf);buf+=sizeof(int);

  if (curC3==0)
  {
    coord=new SoCoordinate3;
    lastSoCoordinate3=coord;
  }
  else
  {
    coord=lastSoCoordinate3;
  }

  for (t=0;t<numC3;t++)
  {
    y=-GetFloat(buf); buf+=sizeof(float);
    z=-GetFloat(buf); buf+=sizeof(float);
    x=-GetFloat(buf); buf+=sizeof(float);
    coord->point.set1Value(curC3+t, x, y, z);
  }

  if (curC3==0) 
    return coord;
  else
    return NULL;
}

SoNormal *CBuildTree::NewNormal(char *buf) 
{
  SoNormal *myNormals;
  int curVector,numVector,t;
  float x, y, z;

  curVector=GetInt(buf);buf+=sizeof(int);
  numVector=GetInt(buf);buf+=sizeof(int);

  if (curVector==0)
  {
    myNormals=new SoNormal;
    lastSoNormal=myNormals;
  }
  else
  {
    myNormals=lastSoNormal;
  }


  for (t=0;t<numVector;t++)
  {
    y=-GetFloat(buf); buf+=sizeof(float);
    z=-GetFloat(buf); buf+=sizeof(float);
    x=-GetFloat(buf); buf+=sizeof(float);
    myNormals->vector.set1Value(curVector+t, x, y, z);
  }

  Debug(printf("Normal\n"));
  if (curVector==0) 
    return myNormals; 
  else
    return NULL;
}

SoNormalBinding *CBuildTree::NewNormalBinding(char *buf) 
{
  SoNormalBinding *myNormalBinding=new SoNormalBinding;

  switch (GetInt(buf)) 
  {
    case _PER_VERTEX:
      myNormalBinding->value=SoNormalBinding::PER_VERTEX;
    break;
    default:
      break;
  }
  Debug(printf("NormalBinding\n"));
  return myNormalBinding;
}

SoMaterialBinding *CBuildTree::NewMaterialBinding(char *buf) 
{
  SoMaterialBinding *myMaterialBinding=new SoMaterialBinding;

  switch (GetInt(buf)) 
  {
    case _PER_VERTEX:
      myMaterialBinding->value=SoMaterialBinding::PER_VERTEX;
    break;
    default:
      break;
  }
  Debug(printf("MaterialBinding\n"));
  return myMaterialBinding;
}

SoSphere *CBuildTree::NewSphere(char *buf) 
{
  SoSphere *mySphere=new SoSphere;
  
  mySphere->radius=GetFloat(buf);
  Debug(printf("Sphere\n"));
  return mySphere;
}

SoFont *CBuildTree::NewFont(char *buf) 
{
  SoFont *myFont=new SoFont;

  int len=GetInt(buf);
  buf+=sizeof(int);
  myFont->name.setValue(toValidName(buf));
  buf+=len;
  myFont->size.setValue(GetFloat(buf));
  Debug(printf("Font\n"));
  return myFont;
}

SoTranslation *CBuildTree::NewTranslation(char *buf) 
{
  SoTranslation *myTranslation=new SoTranslation;
  float dy=-GetFloat(buf); buf+=sizeof(float);
  float dz=-GetFloat(buf); buf+=sizeof(float);
  float dx=-GetFloat(buf);
  myTranslation->translation.setValue(dx, dy, dz);
  Debug(printf("Translation\n"));
  return myTranslation;
}

SoDrawStyle *CBuildTree::NewDrawStyle(char *buf) 
{
  SoDrawStyle *myDrawStyle=new SoDrawStyle;
  int style=GetInt(buf); buf+=sizeof(int);
  float lineWidth=GetFloat(buf);
  switch (style) 
  {
    case _FILLED:
      myDrawStyle->style=SoDrawStyle::FILLED;
      break;
    default:
      break;
  }
  if (lineWidth != -1)
    myDrawStyle->lineWidth=lineWidth;
  Debug(printf("DrawStyle\n"));
  return myDrawStyle;
}

SoText2 *CBuildTree::NewText2(char *buf) 
{
  SoText2 *myText=new SoText2;
  int len=GetInt(buf); buf+=sizeof(unsigned int);
  myText->string=buf;         // ?? copy it SoMFString ??
  buf+=len;
  unsigned char justification=GetByte(buf);
  switch (justification) 
  {
    case _LEFT:
      myText->justification=SoText2::LEFT;
    break;
    case _RIGHT:
      myText->justification=SoText2::RIGHT;
    break;
    case _CENTER:
      myText->justification=SoText2::CENTER;
    break;
  }
  Debug(printf("Text2\n"));
  return myText;
}

/*
SoBaseColor *CBuildTree::NewBaseColor(char *buf) 
{
  SoBaseColor *myBaseColor=new SoBaseColor;
  int num_rgbtriplets=GetInt(buf); buf+=sizeof(int);
  int i;
  float r, g, b;
  for(i=0; i<num_rgbtriplets; ++i) 
  {
    r=GetFloat(buf); buf+=sizeof(float);
    g=GetFloat(buf); buf+=sizeof(float);
    b=GetFloat(buf); buf+=sizeof(float);
    myBaseColor->rgb.set1Value(i, r, g, b);
  }
  Debug(printf("BaseColor\n"));

  lastSoBaseColor=myBaseColor;
  return myBaseColor;
}
*/

SoBaseColor *CBuildTree::NewBaseColor(char *buf)
{
  SoBaseColor *myBaseColor;
  int numBC,curBC,t;
  float r, g, b;

  curBC=GetInt(buf);buf+=sizeof(int);
  numBC=GetInt(buf);buf+=sizeof(int);

  if (curBC==0)
  {
    myBaseColor=new SoBaseColor;
    lastSoBaseColor=myBaseColor;
  }
  else
  {
    myBaseColor=lastSoBaseColor;
  }

  for (t=0;t<numBC;t++)
  {
    r=GetFloat(buf); buf+=sizeof(float);
    g=GetFloat(buf); buf+=sizeof(float);
    b=GetFloat(buf); buf+=sizeof(float);
    myBaseColor->rgb.set1Value(curBC+t, r, g, b);
  }

  if (curBC==0) 
    return myBaseColor;
  else
    return NULL;
}


SoNode *CBuildTree::NewPoint(const SbVec3f *pos,SbColor *col) 
{
  SoSeparator *sep = new SoSeparator;
  sep->renderCaching.setValue(SoSeparator::OFF);
  SoTranslation *trans = new SoTranslation;
  SoCube *cube=new SoCube;

  sep->addChild(trans);
  if (col!=NULL)
  {
    SoBaseColor *myCol=new SoBaseColor;
    sep->addChild(myCol);
    myCol->rgb.setValue(*col);
  }
  sep->addChild(cube);
  trans->translation=*pos;
  cube->width.setValue(0.0025f);
  cube->height.setValue(0.0025f);
  cube->depth.setValue(0.0025f);
  cube->setName("Cube_as_Point");

  return sep;
}

void CBuildTree::setPointPos(SoSeparator *point,const SbVec3f *pos)
{
  ((SoTranslation*)point->getChild(0))->translation=*pos;
}


SoNode *CBuildTree::NewLine(const SbVec3f *from,const SbVec3f *to,SbColor *col)
{ 

  SbVec3f size,origin(0.0f,1.0f,0.0f);
  SoSeparator *sep = new SoSeparator;
  sep->renderCaching.setValue(SoSeparator::OFF);
  SoTranslation *trans1 = new SoTranslation;
  SoTranslation *trans2 = new SoTranslation;
  SoRotation *rot = new SoRotation;
  SoCube *cube=new SoCube;
  SbRotation theRotation;

  size=(*to)-(*from);


  sep->addChild(trans1);
  sep->addChild(rot);
  sep->addChild(trans2);
  if (col!=NULL)
  {
    SoBaseColor *myCol=new SoBaseColor;
    sep->addChild(myCol);
    myCol->rgb.setValue(*col);
  }
  sep->addChild(cube);
  

  trans1->translation=(*from);
  trans2->translation.setValue(0,(size.length()/2.0f),0);
  cube->width.setValue(0.005f);
  cube->depth.setValue(0.005f);
  cube->height.setValue(size.length());
  cube->setName("Cube_as_Line");
  theRotation.setValue(origin,size);
  rot->rotation=theRotation;

  return sep;
}

void CBuildTree::setLinePos(SoSeparator *line,const SbVec3f *from,const SbVec3f *to)
{
  SbVec3f size,origin(0.0f,1.0f,0.0f);
  SoCube *cube=new SoCube;
  size=(*to)-(*from);
  SbRotation theRotation;
  theRotation.setValue(origin,size);

  ((SoTranslation*)line->getChild(0))->translation=*from;
  ((SoTranslation*)line->getChild(2))->translation.setValue(0,(size.length()/2.0f),0);
  ((SoRotation*)line->getChild(1))->rotation=theRotation;
  if (line->getChild(3)->isOfType(SoCube::getClassTypeId()))
  {
    ((SoCube*)line->getChild(3))->height.setValue(size.length());
  }
  else
  {
    ((SoCube*)line->getChild(4))->height.setValue(size.length());
  }
}

/*====================================================================
	erzeugt einen neuen "Ring" aus vertices mit "radius" um den Punkt P
	der erste Punkt liegt in Richtung "start" von P aus, die restlichen
	step-1 Punkte liegen in der Ebene, die von P/normal und P/start 
	aufgespannt wird
	die Punkte werden hinten in VP eingehaengt
	die vertices des Rings werden gegebenfalls mit Farbinformationen gefuellt
*/
void CBuildTree::addRing(int steps, float radius,SbVec3f P,SbVec3f start, SbVec3f normal, SbColor *col,SoVertexProperty *VP)
{
  int num = VP->vertex.getNum();
  float x, y, da, alpha=0.0;  

  start.normalize(); start*=radius;
  normal.normalize(); normal*=radius;

  da=M_PI*2.0/steps;

  for (int i=0; i<steps; i++) 
  {
    x=sin(alpha); y=cos(alpha); alpha+=da;
    VP->vertex.set1Value(num+i, (P+(start*x)+(normal*y)));
    VP->normal.set1Value(num+i, ((start*x)+(normal*y)));
    if (col!=NULL)
    {
      VP->orderedRGBA.set1Value(num+i, col->getPackedValue());
    }
    //VP->texCoord.set1Value(num+i, SbVec2f(0.0, texcoord));
  }
}


SoNode *CBuildTree::NewPolyLine(float radius, int numsides, int numVertices,SbVec3f *vertexPositions,SoBaseColor *cols)
{
  int i,j,num;
  SoSeparator *result = new SoSeparator;
  SbColor *col;
  SoVertexProperty *VP2 = new SoVertexProperty;
  SbVec3f h, A, B, C, AB, BC, normal, oldnorm, sym;
	
  result->ref();

  // first ring has normal parallel to next normal
  A=vertexPositions[0];
  B=vertexPositions[1];
  C=vertexPositions[2];

  if (cols!=NULL)
    col=(SbColor *)&(cols->rgb[0]);
  else
    col=NULL;

  AB=B-A; BC=B-C;
  normal=AB.cross(BC); 
  sym=AB+BC; sym.normalize();

  addRing(numsides, 0.0, A, normal, normal.cross(AB), col,VP2);	
  oldnorm=normal;

  // now generate intermediate rings (till N-1)
  for (i=1; i< numVertices-1; i++) 
  {
    A=vertexPositions[i-1];
    B=vertexPositions[i];
    C=vertexPositions[i+1];

    AB=B-A; BC=C-B; //AB.normalize(); BC.normalize();
    sym=AB+BC; sym.normalize();
    h=sym*sym.dot(oldnorm);
    normal=oldnorm-h;
    if (cols!=NULL)
      col=(SbColor *)&(cols->rgb[i]);
    addRing(numsides, radius, B, normal, normal.cross(sym), col,VP2);
    oldnorm=normal;
  } 

  // last ring like first
  A=vertexPositions[numVertices-1];
  B=vertexPositions[numVertices-2];
  C=vertexPositions[numVertices-3];
  AB=B-A; BC=B-C;
  normal=AB.cross(BC); //normal.normalize();
  //sym=AB+BC; sym.normalize();
  if (cols!=NULL)
    col=(SbColor *)&(cols->rgb[numVertices-1]);

  addRing(numsides, 0.0, A, normal, normal.cross(AB), col,VP2);	

#ifndef TRIANGLESTRIP 
  SoIndexedFaceSet *FS = new SoIndexedFaceSet;
  FS->vertexProperty.setValue(VP2);

  // generate faces of streamline ------------
  num = 0;
  for (i=1; i<numVertices; i++) 
  {
    for (j=0; j<numsides-1; j++) 
    {
      FS->coordIndex.set1Value(num++, (i-1)*numsides+j);
      FS->coordIndex.set1Value(num++, (i-1)*numsides+j+1);
      FS->coordIndex.set1Value(num++, (i)*numsides+j+1);
      FS->coordIndex.set1Value(num++, SO_END_FACE_INDEX);
      FS->coordIndex.set1Value(num++, (i)*numsides+j+1);
      FS->coordIndex.set1Value(num++, (i)*numsides+j);
      FS->coordIndex.set1Value(num++, (i-1)*numsides+j);
      FS->coordIndex.set1Value(num++, SO_END_FACE_INDEX);
    }

    // close loop 
    FS->coordIndex.set1Value(num++, (i-1)*numsides+j);
    FS->coordIndex.set1Value(num++, (i-1)*numsides);
    FS->coordIndex.set1Value(num++, (i)*numsides);
    FS->coordIndex.set1Value(num++, SO_END_FACE_INDEX);
    FS->coordIndex.set1Value(num++, (i)*numsides);
    FS->coordIndex.set1Value(num++, (i)*numsides+j);
    FS->coordIndex.set1Value(num++, (i-1)*numsides+j);
    FS->coordIndex.set1Value(num++, SO_END_FACE_INDEX);
  }

  result->addChild(FS);

#else
  SoIndexedTriangleStripSet *TS = new SoIndexedTriangleStripSet;
  TS->vertexProperty.setValue(VP2);
  
  // generate faces of streamline ------------
  num = 0;
  for (int j=0; j<numsides-1; j++) 
  {
    for (i=0; i<numVertices; i++) 
    {
      TS->coordIndex.set1Value(num++, i*numsides+j);
      TS->coordIndex.set1Value(num++, i*numsides+j+1);
    }
    TS->coordIndex.set1Value(num++, SO_END_STRIP_INDEX);
  }

  for (i=0; i<numVertices; i++) 
  {
    TS->coordIndex.set1Value(num++, i*numsides+j);
    TS->coordIndex.set1Value(num++, i*numsides);
  }
  TS->coordIndex.set1Value(num++, SO_END_STRIP_INDEX);

  result->addChild(TS);
#endif

  return result;
}


/*
SoIndexedLineSet *CBuildTree::NewIndexedLineSet(char *buf) 
{
  SoIndexedLineSet *myILS=new SoIndexedLineSet;
  int num_indices=GetInt(buf); buf+=sizeof(int);
  myILS->coordIndex.setValues(0, num_indices, (int *)buf);
  return myILS;
}
*/


//SoIndexedFaceSet *CBuildTree::NewIndexedLineSet(char *buf) 
SoNode *CBuildTree::NewIndexedLineSet(char *buf) 
{
/* well lets face it...
   Due to a Problem with inventors stereo-viewer we cant use real lines.
*/
  SoSeparator *sep;
  SbVec3f *coords;
  SbColor *col;
  int i,numCoords,c1,c2;
  int num_indices=GetInt(buf); 
  
  numCoords=lastSoCoordinate3->point.getNum(); 

  if ((numCoords==1))
  {
    return NewPointSet(buf);
  }

  buf+=sizeof(int);
  coords=(SbVec3f *)lastSoCoordinate3->point.getValues(0);
  sep=new SoSeparator; 
  sep->renderCaching.setValue(SoSeparator::OFF);

  i=1;
  c1=stbNCGetInt(&buf);
  while (i<num_indices)
  {
    if (lastSoBaseColor==NULL)
      col=NULL;
    else
      col=(SbColor *)&(lastSoBaseColor->rgb[i*2]);

    c2=stbNCGetInt(&buf);
    while ((c2==-1) && (i<num_indices))
    {
      c1=stbNCGetInt(&buf);
      c2=stbNCGetInt(&buf);
      i+=2;
      if ( (c2==-1) && (i<(num_indices-1)) )
      {
        sep->addChild(CBuildTree::NewPoint(&coords[c1],NULL));
      }
    }
    if (i<num_indices)
      sep->addChild(CBuildTree::NewLine(&coords[c1],&coords[c2],col));    

    c1=c2;
    i++;
  }
  
  return sep;
}


SoTriangleStripSet *CBuildTree::NewTriangleStripSet(char *buf) 
{
  SoTriangleStripSet *myTSS=new SoTriangleStripSet;
  int num_vertices=GetInt(buf);
  int32_t l_num_vertices=(int32_t)num_vertices;
  myTSS->numVertices=l_num_vertices;    // ?!?
  Debug(printf("TriangleStripSet\n"));
  return myTSS;
}

/*
SoPointSet *CBuildTree::NewPointSet(char *buf) 
{
  SoPointSet *myPointSet=new SoPointSet;
  myPointSet->numPoints=GetInt(buf);
  return myPointSet; 
}
*/

SoNode *CBuildTree::NewPointSet(char *buf) 
{
/* Well lets Face it, we cant use points due to a bug in the stereoviewer */
/* so lets use cubes instead */
  SbVec3f *coords=(SbVec3f *)lastSoCoordinate3->point.getValues(0);

  return CBuildTree::NewPoint(&coords[0],NULL);
}

SoNode *CBuildTree::NewLineSet(char *buf) 
{
  int num=GetInt(buf);
  SbVec3f *coords=(SbVec3f *)lastSoCoordinate3->point.getValues(0);

  if (num<2) 
  {
    return CBuildTree::NewPoint(&coords[0],NULL);
  }
  else
  {
#ifdef WITH_GEOM_DECODE_THREAD
    return (NewPolyLine(0.02f, 3,GetInt(buf),coords,lastSoBaseColor));
#endif

#ifndef WITH_GEOM_DECODE_THREAD
	int numVertices=GetInt(buf),i;
	SoStreamLine *polyLine=new SoStreamLine;

    polyLine->generateStripSet.setValue(FALSE);
    polyLine->withTexCoord.setValue(FALSE);
    polyLine->withColor.setValue(TRUE);
    polyLine->radius.setValue(0.02f);

    for (i=0;i<numVertices;i++)
    {
      if (lastSoBaseColor!=NULL)
        polyLine->RGBA.set1Value(i,(lastSoBaseColor->rgb[i]));
      polyLine->vertex.set1Value(i,coords[i]);
    }
    polyLine->generateStripSet.setValue(TRUE);
    return (SoNode *) polyLine;
#endif

/*
    //does the polyline with cubes (not optimal)
    SoSeparator *sep=new SoSeparator;
    sep->renderCaching.setValue(SoSeparator::OFF);
    SbColor *col;
    int numVertices=GetInt(buf),i;
    for (i=1;i<numVertices;i++)
    {
      if (lastSoBaseColor==NULL)
        col=NULL;
      else
        col=(SbColor *)&(lastSoBaseColor->rgb[i]);

      sep->addChild(CBuildTree::NewLine(&coords[i-1],&coords[i],col));
    }
    return sep;
*/
  }
}

SoQuadMesh *CBuildTree::NewQuadMesh(char *buf) 
{
  SoQuadMesh *myQuadMesh=new SoQuadMesh;
  myQuadMesh->verticesPerColumn=GetInt(buf); buf+=sizeof(int);
  myQuadMesh->verticesPerRow=GetInt(buf);
  Debug(printf("QuadMesh\n"));
  return myQuadMesh;
}

SoMaterial *CBuildTree::NewMaterial(char *buf) 
{
  SoMaterial *myMaterial=new SoMaterial;
  float r, g, b;
  r=GetFloat(buf); buf+=sizeof(float);
  g=GetFloat(buf); buf+=sizeof(float);
  b=GetFloat(buf);
  myMaterial->diffuseColor.setValue(r, g, b);
  return myMaterial;
}

SoSeparator *CBuildTree::getSepBevore(SoSeparator *ssep,SoNode *toFind)
{

  if (ssep->findChild(toFind)==-1)
  {
    for (int i=0;i<ssep->getNumChildren();i++)
    {
      if (ssep->getChild(i)->isOfType(ssep->getTypeId()))
      {
        SoSeparator *helpSep=CBuildTree::getSepBevore((SoSeparator *)ssep->getChild(i),toFind);
	if (helpSep!=NULL) return helpSep;
      }
    }
    return ssep;
  }
  else
    return ssep;
}

#define _NORMAL -1
#define _RESET  -2
#define _EOD    -3
#define _UNKNOWN -4

/****************************************************************************
prototype:      int CBuildTree::Parse(SoSeparator *addSep, char *current);
purpose:        part of the parser
args:           addSep... Separator, where found objects become added
		current... pointer to buffer (current parsing position)
return:         _UNKNOWN if unknown primitive was found.
		On success, Parse() returns the number of bytes read from buffer
notes:          Recursive function (on SoSeparator)
		Called by CBuildTree::Do()
****************************************************************************/
int CBuildTree::Parse(char *current) 
{
  unsigned char type;
  int clen;

  type=GetByte(current); current++; //read++;

  clen=GetInt(current);
  Debug(printf("Thread: type %d len %d\n", type, clen));
  current=current+sizeof(int); // read+=sizeof(int);
  switch (type) 
  {
    case COORDINATE3:
    {
      SoCoordinate3 *c3=NewCoordinate3(current);
      if (c3!=NULL) addSep->addChild(c3);
    }
    break;
    case INFO:          // somewhat special
      if (addSep) 
      {
        addSep->setName(toValidName(current+sizeof(int)));
      }
    break;
    case NORMAL:
    {
      SoNormal *norm=NewNormal(current);
      if (norm!=NULL) addSep->addChild(norm);
    }
    break;
    case NORMALBINDING:
      addSep->addChild(NewNormalBinding(current));
    break;
    case BASECOLOR:
	{
      SoBaseColor *bc=NewBaseColor(current);
      if (bc!=NULL) addSep->addChild(bc);
	}
    break;
    case MATERIALBINDING:
      addSep->addChild(NewMaterialBinding(current));
    break;
    case DRAWSTYLE:
      addSep->addChild(NewDrawStyle(current));
    break;
    case TRANSLATION:
      addSep->addChild(NewTranslation(current));
    break;
    case SPHERE:
      addSep->addChild(NewSphere(current));
    break;
    case FONT:
      addSep->addChild(NewFont(current));
    break;
    case MATERIAL:
      addSep->addChild(NewMaterial(current));
    break;
    case TEXT2:
      //addSep->addChild(NewText2(current));
    break;
    case INDEXEDLINESET:
      addSep->addChild(NewIndexedLineSet(current));
    break;
    case TRIANGLESTRIPSET:
      addSep->addChild(NewTriangleStripSet(current));
    break;
    case LINESET:
      addSep->addChild(NewLineSet(current));
    break;
    case QUADMESH:
      addSep->addChild(NewQuadMesh(current));
    break;
    case RESET:
      Debug(printf("Parse: Reset encountered\n"));
      return _RESET;
    //break;
    case ENDOFDATA:
      Debug(printf("Parse: End of data encountered\n"));
      return _EOD;
    //break;
    case SEPARATOR_START:
    {
      SoSeparator *s = new SoSeparator;
  s->renderCaching.setValue(SoSeparator::OFF);
      addSep->addChild(s);
      addSep=s;
    }
    break;
    case SEPARATOR_END:
    {
      addSep=CBuildTree::getSepBevore(rootSep,(SoNode*)addSep);
    }
    break;
    default:
    {
      fprintf(stderr, "CBuildTree::Parse(): skipping unknown primitive type %d\n",type);
      return _UNKNOWN;
    }
   }
   Debug(printf("returning %d\n", clen));
   return clen;
}

void CBuildTree::setSeparator(SoSeparator *mySep)
{
  lastSoCoordinate3=NULL;
  lastSoNormal=NULL;
  lastSoBaseColor=NULL;
  rootSep=mySep;
  addSep=mySep;
}

char* CBuildTree::toValidName(char *buf) 
{
  int i=0;
  static char validStr[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRTSUVWXYZ0123456789";
  
  while (buf[i]!=0)
  {
	if (strchr(validStr,buf[i])==NULL)
      buf[i]='_';	  
    i++;
  }
  return buf;
}

/*
** convenience functions
*/
int GetInt(char *buf) {
        char *tmp;
        tmp=buf;
	return stbNCGetInt(&tmp);
}

float GetFloat(char *buf) {
        char *tmp;
        tmp=buf;
	return stbNCGetFloat(&tmp);
}

unsigned char GetByte(char *buf) {
        char *tmp;
        tmp=buf;
	return stbNCGetChar(&tmp);
}

